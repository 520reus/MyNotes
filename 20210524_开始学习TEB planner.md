# 开始学习TEB planner

把teb算法处理局部规划的方法学到手

teb_localplanner是以优化的方法进行局部导航的软件包。基于timed elastic band 方法进行轨迹生成。考虑了运动时的轨迹执行时间、避障、运动学、动力学的约束等条件。此笔记是对teb包的源代码进行学习的一个记录。

## 学习目标

1. teb是如何处理静态障碍物与动态障碍物的？
2. 如何把局部规划问题转化为图优化问题求解？
3. teb是如何给路径生成速度曲线的？
4. teb是如何解决局部极小问题的？
5. teb是如何将local_costmap中的obstacle cell转化为几何线、面、多边形来加速问题求解的？（ans：costmap_converter）
6. teb中的distinctive Topologies是什么？（ans：teb是利用软约束替代硬约束，所以不可避免的会陷入局部极小，因此teb利用distinctive Topologies来帮助teb生成多条轨迹，避免发生碰撞，用会进行**可行性检查**）
7. teb是如何处理小车的运动学模型的？(重点关注car like，即阿克曼结构的小车)
8. teb是如何考虑车辆在地图中的几何表示的？(小车的footprint类型直接决定了能否绕过实际障碍物，并影响算法速度)
9. teb是如何处理倒车的？

下面先不深入源码，首先来结合ros wiki的官方文档，运行`roslaunch teb_local_planner test_optim_node.launch`,看看参数对轨迹的影响。

ros wiki teb local planner: http://wiki.ros.org/teb_local_planner/Tutorials

### 实际调试过程中哪些参数可以显著影响生成轨迹的形状

1. min_obstacle_dist：与障碍物之间的最小期望距离（以米为单位）
2. inflation_dist：惩罚成本不为零的障碍物周围的缓冲区（应大于min_obstacle_dist才能生效）。  另请参阅权重weight_inflation。
3. weight_inflation：通胀惩罚的优化权重（应该很小）。
4. include_dynamic_obstacles：如果将此参数设置为true，则将通过等速模型在优化过程中预测并考虑速度为非零的障碍物的运动（通过用户在主题〜/障碍物上提供的障碍物或从costmap_converter获得）。
5. penalty_epsilon：为惩罚函数增加一个小的安全余量，以实现硬约束近似
6. dt_ref：轨迹的所需时间分辨率（由于时间分辨率是优化的一部分，因此轨迹不固定为dt_ref，但是如果违反dt_ref + -dt_hysteresis，则将在迭代之间调整轨迹的大小。
7. dt_hysteresis：用于根据当前时间分辨率自动调整大小的磁滞，通常大约为 建议使用dt_ref的10％

局部规划器的输入：

1. teb配置参数
2. 障碍物序列
3. 机器人模型
4. 可视化
5. 路标点(optional)

## Timed Elastic Band(TEB)算法

### 轨迹规划综述

<img src="图片/20210524_开始学习TEB planner/2021-06-02 15-36-41 的屏幕截图.png" style="zoom:67%;" />

**轨迹规划的目标**是计算出安全、舒适的轨迹供无人驾驶车辆完成预定的行驶任务。**安全**意味着车辆在行驶过程中与障碍物保持适当的距离，避免碰撞；舒适意味着给乘客提供**舒适的乘坐体验**，比如避免过急的加减速度，在弯道时适当减速避免过大的向心加速度等等；最后，完成行驶任务指规划出的轨迹要完成给定的行驶任务，不能因为过于保守的驾驶导致不可接受的行驶时间。

#### 如果可以搜索出局部的最优位姿点轨迹规划搜索维度

<img src="图片/20210524_开始学习TEB planner/2021-06-02 15-35-59 的屏幕截图-1622619437066.png" style="zoom:67%;" />

对于无人驾驶车辆来讲，最简单的位形描述需要使用**3个变量**：车辆某个参照点的坐标(x,  y)，以及车辆的朝向θ来表达车辆的构型，这也是很多参考文献中，对于非和谐车辆系统的位形表达方式。对于我们专门为有人乘用的无人驾驶车辆作轨迹规划的问题来讲，更好的位形组成在上面的3个变量基础上加入车辆的**即时转向曲率 κ**：如果车辆的导向轮保持一定的角度，车辆会做圆周运动。这个圆周运动的半径就是即时如果可以搜索出局部的最优位姿点转向曲率 κ 。加入 κ，有助于控制模块获得更准确的控制反馈，设计更为**精细平稳的控制器**。

#### 轨迹规划的定义

<img src="图片/20210524_开始学习TEB planner/2021-06-02 15-37-47 的屏幕截图.png" style="zoom: 67%;" />

轨迹规划的正式定义，计算一个以时间 t 为参数的函数 S ，对于定义域内（[0, t_max]）的每一个 t ，都有满足条件的状态 s ：满足目标车辆的**运动学约束**，**碰撞约束**，以及车辆的**物理极限**。

#### 轨迹规划问题的难点

轨迹规划是一个复杂的问题，首先，规划上加入了速度与时间的信息，增加了**规划的维度**。其次，由于车辆是非和谐系统，具有特殊的运动学约束条件。举个例子，车辆不能独立的横向移动，需要纵向移动的同时才能获得**横向偏移**。躲避障碍物，特别是高速动态障碍物，也是一个比较困难的问题。对于搭载乘客的无人驾驶车辆来说，非常重要的一个要求是舒适性，规划出的轨迹必须做到**平滑**，将影响乘客舒适度的因素，比如加速度、向心加速度等等，保持在能够容忍的范围。

另外，对于无人驾驶车辆来讲，为了处理多变的行车环境，轨迹规划算法需要**以短周期高频率运行**，对算法的计算效率也提出了要求。

<img src="图片/20210524_开始学习TEB planner/2021-06-02 15-39-44 的屏幕截图.png" style="zoom:67%;" />

**下面将细致阐述TEB算法是如何解决以上提出的规划问题的难点**

**TEB区别于百度在Frenet坐标上解耦成横向与纵向，再分别进行路径规划与速度规划，而是直接在笛卡尔坐标系上进行规划**

### Timed Elastic Band——时间弹性带如果可以搜索出局部的最优位姿点

**elastic band method**：连接起始、目标点，并让这个路径可以变形，变形的条件就是将所有约束当做橡皮筋的外力。关于time eletic band的简述：起始点、目标点状态由用户/全局规划器指定，中间插入N个控制橡皮筋形状的控制点（机器人姿态），当然，为了显示轨迹的运动学信息，我们在点与点之间定义运动时间Time，即为Timed-Elastic-Band算法。

#### 数学建模过程

TEB算法是用优化的方法来进行规划的。轨迹规划的本质是一个高维空间（7个搜索维度——$车辆状态s(t):[x,y,\theta,\kappa,v,a,t]$）内的带多种等式约束与不等式约束的搜索问题，且对搜索时间有很高的要求。即：
$$
{\forall}t\in[0,t_{max}]: \\
				s^*(t)=\mathop{\arg\min}_{s(t)}\ J(s(t)) \\
			subject\ to:\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ 车辆非完整型约束(\frac{\mathrm{d} y }{\mathrm{d} t}\cos{\theta}-\frac{\mathrm{d} x }{\mathrm{d} t}\sin{\theta}=0)\\
			障碍物碰撞约束\\
			动力学约束（最大速度、最大加速度）\\
			转向约束(最大转向半径、最大转向变化率)\\
			最短时间约束\\
			最短路径约束\\
$$
TEB将该问题建模为一个图优化问题，将待优化的变量，即空间搜索维度——车辆位姿$SE2:[x,y,\theta]$+时间搜索维度——位姿间的时间间隔$\Delta T$抽象为图结构中的顶点，将碰撞约束、车辆非完整型约束、动力学约束（速度、加速度限制）、最大转向半径、最大转向变化率、最短时间、最短路径等一系列的约束作为图结构中的边。利用g2o框架求解该图优化问题，得到最优的车辆状态序列$S(t):[x.y,\theta,\Delta T]$

TEB为了解决前述轨迹规划的难点，即多约束下的高维搜索问题无解或难找到解，其将硬约束转化为软约束，将全部约束项以误差项的形式加入到目标函数中，这样就把多目标优化问题转化为非线性最小二乘问题，进而将待优化变量与约束项转化为图结构中的顶点与边，调用g2o即可求解该轨迹规划问题。

因此TEB的建模过程可表示为：多目标优化->非线性最小二乘->图优化

TEB的轨迹规划非线性最小二乘问题框架：
$$
x {\forall}t\in[0,t_{max}]: \\            \qquad\qquad\qquad\qquad   J(s(t))= \sum_kE_k(s(t))^T\Omega_kE_k(s(t)) \\
 \qquad\qquad\qquad\qquad s^*(t)= \mathop{\arg\min}_{s(t)}J(s(t))
$$
误差项$E_k(s(t))$的定义，即误差函数penalty function如下，以速度约束举例：

<img src="图片/20210524_开始学习TEB planner/2021-06-02 18-59-54 的屏幕截图.png" style="zoom: 50%;" />

$x$表示速度，$e_\Gamma$表示误差，$x_r$表示约束界限，$\epsilon$表示安全裕度，$n$表示阶次,$S$表示缩放因子。这里假设最大速度为0.4，安全裕度为0.1，则当速度大于0.3时就产生误差cost，且超出bound越大，产生的误差越大，惩罚就越大。这样就把约束条件建模为误差项，再作为软约束加入到非线性最小二乘的目标函数中，即$e_\Gamma=f(s(t))$。再将其他约束做类似处理，再与对应的权重因子相乘，即完成了对所有约束条件的建模过程。

<img src="图片/20210524_开始学习TEB planner/2021-06-02 18-58-15 的屏幕截图.png" style="zoom:67%;" />



如果可以搜索出局部的最优位姿点+时间间隔序列:$[x,y,\theta,\Delta T]$，那么就可以进而求解对应的轨迹曲率$\kappa,$速度$v$与加速度$a$，并且是满足车辆动力学约束条件的。这样就完成了局部轨迹规划任务。

#### 待优化变量——图优化的顶点

**位姿**

经典的elastic band算法描述为一系列的车辆位姿$q_i=(x_i,y_i,\beta_i)^T\in\mathbb{R}^2\times S^1$,则位姿顶点集为：
$$
Q=\{{x_i}\}_{i=0...n} \qquad n\in\mathbb{N}
$$
**时间间隔**

每两个位姿之间用时间间隔$\Delta T_i$表示，则时间间隔顶点集为：
$$
\tau=\{\Delta T_i\}_{i=0...n-1}
$$
则一个TEB状态可以表示为位姿与时间间隔的组合：
$$
S=(Q,\tau)
$$
下图为全局坐标系下的时间弹性带TEB的状态序列

<img src="图片/20210524_开始学习TEB planner/2021-06-03 08-51-12 的屏幕截图.png" style="zoom:67%;" />如果可以搜索出局部的最优位姿点

有了状态$S$（待优化变量）的定义，搜索空间就确定了，下面就是定义误差项$f_k(S)$（约束条件），然后抽象为一个带软约束的多目标优化问题：
$$
f(S)= \sum_k\gamma_kf_k(S)
\\S^*= \mathop{\arg\min}_{s}f(S)
$$
最后将待优化变量（状态）与误差项（边）抽象为图结构,再将多目标优化问题转化为非线性最小二乘通过调用g2o框架解决规划问题。

**注：**需要注意的是大多数误差项都是局部的，即只含有整段局部轨迹的几个状态，而不是整个band，因此导致系统矩阵是很稀疏的，从而使得大规模数据优化方法求解高效迅速！

#### 约束条件误差项——图优化的边

**1.速度**

**2.加速度**

**3.非完整型约束**

**4.障碍物与航迹点**

**5.最短路径如果可以搜索出局部的最优位姿点如果可以搜索出局部的最优位姿点**

**6.最短时间**



#### 算法流程

再来回顾一下何为Timed Elastic Band算法，其继承了Elastic Band的思想，即局部轨迹的初始节点与终止节点是固定的！而除了头尾两个节点外，其他待优化节点都是不固定的，每增加一个误差项，就相当于对这条固定头尾的带子施加一个“外力”，比如在band的上方附近如果有一个障碍物，相当于给带子施加一个向下的力，使得带子往下方弯曲，故名弹性带。而TEB在Elastic Band的基础上引入了时间间隔这个搜索维度，使得其在$S=[x,y,\theta,\Delta T]$四个维度上进行优化求解，速度与加速度可以通过优化后的状态变量求得，进而完成轨迹规划的任务。

TEB需要输入一个全局轨迹作为参考轨迹，比如规划循环可表示为：

1. 根据车辆定位匹配当前与车辆最近的全局轨迹上的点，固定该点为TEB优化起始点
2. 根据配置参数选择该点往后一段路径的点，固定该点作为TEB优化的终止点
3. 将误差项映射成超图，调用g2o优化求解状态集$S=[x,y,\theta,\Delta T]$
4. 可行性检查，避免陷入局部极小
5. 对TEB状态序列剪枝，删掉已经走过的状态点，重新选择并固定TEB起点终点，并利用已优化过但还没走过的节点，这样可以暖启动，即利于下一次循环的优化求解，也满足了时间一致性问题。

算法流程图如下：

<img src="图片/20210524_开始学习TEB planner/2021-06-03 09-14-18 的屏幕截图.png" style="zoom: 67%;" />



**TEB建模优点**

1.求解迅速

许多误差项都是局部的，比如障碍物碰撞约束仅考虑局部轨迹距离障碍物最近的几个状态，这样黑塞矩阵就是稀疏的，求解会很迅速。

2.无解的概率变小

因为局部规划是动态进行的，硬约束下当环境复杂很容易出现无解的情况，这样只能再次进行re-plan。而teb将硬约束转化为软约束，找到解的概率就增大很多，即使其无法保证严格遵守约束条件，但是可以通过在约束条件中加上一点安全裕度来保证约束条件的满足。

2.方便通用

误差项与对应权重相乘，通过调节权重即可改变对某个约束项的重视程度

3.利于扩展

比如现在考虑对TEB加上平滑性约束，那么直接将约束项建模为误差项加入目标函数中，再抽象为图结构中的边即可。 





## 源码总结

### 头文件

teb_local_planner头文件目录如下：

![](图片/20210524_开始学习TEB planner/2021-05-28 14-32-20 的屏幕截图.png)

以下头文件与distinctive topology有关，均为了生成多簇局部轨迹、避免optimal_planner生成的teb轨迹陷入局部极小与障碍物发生碰撞 

```
equivalence_relations.h
graph_search.h
h_signature.h
homotopy_class_planner_h
homotopy_class_planner.hpp
```

以下头文件声明teb中的障碍物数据结构、障碍物碰撞检测、几何距离计算

```
distance_calculations.h
obstacles.h
```

以下头文件声明teb中的planner

```
planner_interface.h //planner抽象基类
optimal_planner.h    //1.teb默认planner，算法核心
homotopy_class_planner_h //2.同源类planner，算法核心
```

以下头文件声明teb节点的数据结构、局部轨迹的数据结构、所需的参数、机器人轮廓模型

```
pose_se2.h
timed_elastic_band.h
timed_elastic_band.hpp
teb_config.h
robot_footprint_model.h
```

以下头文件声明与ros的接口

```
teb_local_planner_ros.h // 继承nav_core::BaseLocalPlanner,实现ros定义的局部规划器接口
recovery_behaviors.h 
visualization.h        //可视化节点
visualization.hpp
```

以下头文件是一个杂项类

`misc.h`

include文件夹中的g2o_types文件夹专门用来存放teb算法与g2o的接口，即将teb中的多个目标函数转化为图优化中的边；teb中的待优化变量，即局部轨迹的位姿序列与两个位姿之间的时间间隔转化为图优化中的节点

![](图片/20210524_开始学习TEB planner/2021-05-28 14-32-55 的屏幕截图.png)

### 1.障碍物

teb对障碍物结构的声明放在了Obstacle.h中		

teb共定义了5种障碍物，声明均在Obstacle.h头文件中，分别为：

![](图片/20210524_开始学习TEB planner/2021-05-27 15-24-55 的屏幕截图.png)

障碍物外形可分别为圆形、线形、线圆结合形、点形、多边形

每个障碍物类都继承抽象基类Obstacle

![](图片/20210524_开始学习TEB planner/2021-05-27 15-37-12 的屏幕截图.png)

#### 1.Obstacle

头文件首先定义Obstacle抽象基类

**类成员变量**

基类共有两个成员变量：

```
bool dynamic_;             //动态障碍物标志
Eigen::Vector2d centroid_velocity_;        //质心速度
```

第一个变量声明障碍物是否是动态的，第二个变量用来表示障碍物的质心速度[vx,vy]

**类成员函数**

下面函数用于获得障碍物质心坐标

<img src="图片/20210524_开始学习TEB planner/2021-05-27 21-36-25 的屏幕截图.png" style="zoom:67%;" />

下面函数用于判断各种类型的障碍物是否与给定点发生碰撞，获取给定点与障碍物之间的最短距离以及最短点坐标

![](图片/20210524_开始学习TEB planner/2021-05-27 21-36-32 的屏幕截图.png)

下面函数用于获得使用恒速模型获得估计的到移动障碍物的最小时空距离，判断是否运动、设置质心速度

![](图片/20210524_开始学习TEB planner/2021-05-27 21-36-41 的屏幕截图.png)

下面函数用于将障碍物转换为多边形信息

<img src="图片/20210524_开始学习TEB planner/2021-05-27 21-36-49 的屏幕截图.png" style="zoom:67%;" />

另外定义了三个智能指针，第一个为障碍物父类指针、第二个是父类常量指针、第三个是障碍物集合

<img src="图片/20210524_开始学习TEB planner/2021-05-27 21-52-13 的屏幕截图.png" style="zoom:67%;" />

#### 2.PointObstacle

**成员变量**

PointObstacle除了从基类继承的动态标志位、质心速度外，还定义了一个pos\_用于存储障碍物位置坐标

```
bool dynamic_;                                                  //动态障碍物标志位
Eigen::Vector2d centroid_velocity_;        //质心速度
Eigen::Vector2d pos_;                                    //质心坐标
```

#### 3.CircularObstacle

**成员变量**

PointObstacle除了从基类继承的动态标志位、质心速度外，还定义了一个pos\_用于存储障碍物位置坐标和一个radius_用于存储圆形障碍物的半径

```
bool dynamic_;                                                  //动态障碍物标志位
Eigen::Vector2d centroid_velocity_;        //质心速度
Eigen::Vector2d pos_;                                    //质心（原心）坐标
double radius_ = 0.0;                                     //圆形障碍物半径
```

#### 4.LineObstacle

**成员变量**

LineObstacle除了从基类继承的动态标志位、质心速度外，还定义了线段的起点与终点坐标、以及线段的中点坐标

```
bool dynamic_;                                                  //动态障碍物标志位
Eigen::Vector2d centroid_velocity_;        //质心速度
Eigen::Vector2d start_;                                  //线段起点坐标
Eigen::Vector2d end_;	                                //线段终点坐标
Eigen::Vector2d centroid_;                          //线段中点坐标
```

#### 5.PillObstalce

**成员变量**

```
bool dynamic_;                                                  //动态障碍物标志位
Eigen::Vector2d centroid_velocity_;        //质心速度
Eigen::Vector2d start_;                                  //线段起点坐标
Eigen::Vector2d end_;	                                //线段终点坐标
Eigen::Vector2d centroid_;                          //线段中点坐标
double radius_ = 0.0;                                     //圆半径
```

#### 6.PolygonObstacle

**成员变量EquivalenceClassContainerEquivalenceClassContainer**

```
bool dynamic_;                                                  //动态障碍物标志位
Eigen::Vector2d centroid_velocity_;        //质心速度
Point2dContainer vertices_;                        //存储所有的多边形节点
Eigen::Vector2d centroid_;                           //多边形质心坐标
bool finalized_;                                                  //多边形节点是否添加完成
```

#### 碰撞检测

**点在多边形内检测**

https://blog.csdn.net/weixin_33840661/article/details/94599492

**2d碰撞检测**

https://www.spieleprogrammierer.de/wiki/2D-Kollisionserkennung#Kollision_Kreis-Strecke

**计算多边形质心**

*http://de.wikipedia.org/wiki/Geometrischer_Schwerpunkt#Polygon*

**点到线的最短距离**

https://blog.csdn.net/weixin_41316824/article/details/90699499

#### 动态障碍物处理





### 2.timed_elastic_band

该文件定义了teb中的局部规划最优轨迹的数据结构

**两个别名**

```c++
//! Container of poses that represent the spatial part of the trajectory ，轨迹的空间点序列
typedef std::vector<VertexPose*> PoseSequence; 
//! Container of time differences that define the temporal of the trajectory 轨迹的时间点序列
typedef std::vector<VertexTimeDiff*> TimeDiffSequence;
```

**成员变量**

```c++
PoseSequence pose_vec_;               //存储最优轨迹位姿点序列
TimeDiffSequence timediff_vec_;  //存储时间间隔点序列
```





### 3.robot_footprint_model

该文件定义了机器人的几何轮廓模型，用于teb优化求解中的**碰撞检测**与rviz可视化。其与costmap中的footprint model不完全一样。轮廓模型类型与参数可以在teb的配置文件中进行配置

轮廓模型类继承关系如下：

![](图片/20210524_开始学习TEB planner/2021-05-29 14-14-47 的屏幕截图.png)

轮廓模型分为：

1. 圆（重要）
2. 线
3. 点
4. 多边形（重要）
5. 两个圆的组合

机器人底盘轮廓模型可大致近似为圆（turtlebot）与多边形（无人驾驶车辆），故这两种轮廓模型比较常用。

### 4.optimal_planner

本文件是TEB算法的核心，包含以下内容：

1. 初始化g2o优化器、注册TEB算法转化为图结构后的顶点和边，添加节点和边、构造图与清除图，优化求解

   注：待优化变量(顶点)是TimedElasticBand类定义的位姿序列与时间间隔序列，优化求解后即保持在TimedElasticBand teb_中，方便后续调用与可视化

2. 结果可视化

3. 获取优化后的速度曲线、整条优化轨迹

4. 轨迹可行性检查

#### 1.BaseRobotFootprintModel——机器人底盘几何轮廓抽象基类

机器人轮廓模型基类，无成员变量，内有两个用于碰撞检测的纯虚函数需要被每一个子类实现：

第一个函数用于计算机器人在当前轮廓模型、当前位姿下与障碍物的距离

第二个函数用于计算机器人在当前轮廓模型、当前位姿下与**动态障碍物**在**t时刻**的距离

![](图片/20210524_开始学习TEB planner/2021-05-29 14-29-57 的屏幕截图.png)



该头文件声明了两个指向抽象基类的共享指针：

```c++
//! Abbrev. for shared obstacle pointers
typedef boost::shared_ptr<BaseRobotFootprintModel> RobotFootprintModelPtr;
//! Abbrev. for shared obstacle const pointers
typedef boost::shared_ptr<const BaseRobotFootprintModel> RobotFootprintModelConstPtr;
```

#### 2.PointRobotFootprint——点轮廓模型

最简单、也是最没用的一种轮廓模型，除非机器人足够小、小到相对局部地图可以看做一个点...实际上基本上是不可能的

点轮廓模型对上面提到了两个用于碰撞检测的纯虚函数的实现是通过obstacles.h中的距离计算函数进行的：

<img src="图片/20210524_开始学习TEB planner/2021-05-29 14-34-56 的屏幕截图.png" style="zoom:67%;" />

<img src="图片/20210524_开始学习TEB planner/2021-05-29 14-35-03 的屏幕截图.png" style="zoom:67%;" />

可以看到，这些轮廓模型都是通过障碍物的多态性进行碰撞检测的，即参数是Obstacle父类指针、传入子类对象，即让父类指针指向子类对象，即可满足点轮廓模型对任意类型障碍物进行距离计算！其他轮廓模型的实现是类似的。

#### 3.CircularRobotFootprint——圆轮廓模型

针对圆形底盘的机器人很适用

该类内有个rviz可视化函数,需要用自己的成员变量，即radius_对visualization_msgs::Marker的信息进行写入，这样如果发布对应消息就能在rviz中看到机器人的圆轮廓模型了

![](图片/20210524_开始学习TEB planner/2021-05-29 14-40-55 的屏幕截图.png)

#### 4.TwoCirclesRobotFootprint——双圆组合轮廓模型

当车体的前后轮距不一致时可以采用这种模型

![](图片/20210524_开始学习TEB planner/2021-05-29 14-59-06 的屏幕截图.png)

#### 5.LineRobotFootprint——线段轮廓模型

线机器人对于在纵向和横向上表现出不同扩展/长度的机器人很有用。

<img src="图片/20210524_开始学习TEB planner/2021-05-29 15-12-39 的屏幕截图.png" style="zoom:67%;" />

需注意在配置文件中写入线段轮廓模型时，其认为车辆质心在原点[0,0]，故模型参数可配置为：

```
TebLocalPlannerROS:
 footprint_model: # types: "point", "circular", "line", "two_circles", "polygon"
 type: "line"
 line_start: [-0.3, 0.0] 
 line_end: [0.3, 0.0] 
```



#### 6.PolygonRobotFootprint——多边形轮廓模型

阿克曼结构、自动驾驶车辆一般采用这种模型，尤其是矩形外形。

需注意在配置文件中写入线段轮廓模型时，其认为车辆质心在原点[0,0]，故模型参数可配置为：

```
TebLocalPlannerROS:
 footprint_model: # types: "point", "circular", "line", "two_circles", "polygon"
 type: "polygon"
vertices: [[-1,-1],[-1,1],[1,1],[1,-1] ] 
```

注：按顺序写多边形顶点，因为默认两个顶点间存在边，且不用闭合，即默认最后一个节点与第一个节点之间存在边

从代码中可以看到，每增加一个节点（边）就增加了算法时间复杂度，因此轮廓模型的顶点不要太多，这里认为4个就够了，即矩形轮廓。





## g2o

g2o全称General Graph Optimization，即通用图优化，是一个用图来存储待优化变量，用边存储约束条件的误差项，来优化非线性误差函数的c++框架。

为何叫通用呢？g2o的核里带有各种各样的求解器，而它的顶点、边的类型则多种多样。通过自定义顶点和边，事实上，只要一个优化问题能够表达成图，那么就可以用g2o去求解它。常见的，比如bundle adjustment，ICP，数据拟合，都可以用g2o来做。它是一个重度模板类的c++项目，其中矩阵数据结构多来自Eigen。

g2o理论：
 许多问题可以建模为最小二乘优化的状态估计问题。问题形式如图

![在这里插入图片描述](图片/20210524_开始学习TEB planner/20181207193525492.png)

F(x)为目标函数，目的就是求解状态x，使目标函数最小。x是一个向量，向量中的每个元素也可以为一个参数块。zij是联系状态变量i和状态变量j的观测，数学上表现为一个约束方程。由于有测量噪声，采用误差向量e()评价xi和xj满足方程的程度。

为了简化符号，采用下图描述误差向量

![在这里插入图片描述](图片/20210524_开始学习TEB planner/20181207194001506.png)

这里需要注意的是，状态变量xi和xj（左侧），已经变为了整个系统的状态x（右侧）。

这个误差向量可以表示为一个有向图，可以知道方程结构，便于可视化分析

![在这里插入图片描述](图片/20210524_开始学习TEB planner/20181207194219124.png)

这个图中，边的个数就是所有的约束，也就是方程个数。一个节点为一个状态变量，所有节点的结合组成了整个系统的状态变量。不要忘记我们的目标就是为了状态估计。

有了上述目标函数，我们就可以进行最小二乘优化。由于目标函数是以误差项的形式给出，因此考虑使用高斯牛顿法(GN)以及其的改进方法——列文伯格-马夸特（LM）法。

如果我们假设目标函数为F(x)，误差函数为f(x)，由于上面的目标函数不是针对F(x)的优化，转而考虑误差函数f(x)的优化，因此可以避免求解计算量偏大、耗时的黑塞矩阵，而是用梯度的乘积来近似黑塞矩阵，从而大大减小运算量。

最小二乘问题的四种解法——牛顿法，梯度下降法，高斯牛顿法和列文伯格-马夸特法的区别和联系详见：https://zhuanlan.zhihu.com/p/113946848

g2o理论具体求解方法见：https://blog.csdn.net/qit1314/article/details/84886612

g2o在slam中一般分解为两个任务：

1、构建图。机器人位姿作为顶点，位姿间关系作为边。

2、优化图。调整机器人的位姿（顶点）来尽量满足边的约束，使得误差最小。

g2o求解框架流程如下图：



![img](图片/20210524_开始学习TEB planner/606958-20160321233900042-681579456.png)

### 1.图的核心

SparseOptimizer是整个图的核心，我们注意右上角的 is-a 实心箭头，这个SparseOptimizer它是一个Optimizable Graph，从而也是一个超图（HyperGraph）。

### 2.顶点和边

注意看 has-many  箭头，超图包含了许多顶点（HyperGraph::Vertex）和边（HyperGraph::Edge）。而这些顶点顶点继承自 Base Vertex，也就是OptimizableGraph::Vertex，而边可以继承自 BaseUnaryEdge（单边）,  BaseBinaryEdge（双边）或BaseMultiEdge（多边），它们都叫做OptimizableGraph::Edge

### 3.配置SparseOptimizer的优化算法和求解器

整个图的核心SparseOptimizer  包含一个优化算法（OptimizationAlgorithm）的对象。OptimizationAlgorithm是通过OptimizationWithHessian 来实现的。其中迭代策略可以从Gauss-Newton（高斯牛顿法，简称GN）, Levernberg-Marquardt（简称LM法）,  Powell's dogleg 三者中间选择一个（我们常用的是GN和LM）

### 4.如何求解

OptimizationWithHessian  内部包含一个求解器（Solver），这个Solver实际是由一个BlockSolver组成的。这个BlockSolver有两个部分，一个是SparseBlockMatrix  ，用于计算稀疏的雅可比和Hessian矩阵；一个是线性方程的求解器（LinearSolver），它用于计算迭代过程中最关键的一步HΔx=−b，LinearSolver有几种方法可以选择：PCG, CSparse, Choldmod

### **一步步看懂g2o编程流程**

我们梳理是从顶层到底层，但是编程实现时需要反过来，像建房子一样，从底层开始搭建框架一直到顶层。g2o的整个框架就是按照下图中我标的这个顺序来写的。

![img](图片/20210524_开始学习TEB planner/5c3f18edaf41d.png)

高博在十四讲中g2o求解曲线参数的例子来说明，源代码地址

https://github.com/gaoxiang12/slambook/edit/master/ch6/g2o_curve_fitting/main.cpp

为了方便理解，我重新加了注释。如下所示，

```cpp
typedef g2o::BlockSolver< g2o::BlockSolverTraits<3,1> > Block;  // 每个误差项优化变量维度为3，误差值维度为1

// 第1步：创建一个线性求解器LinearSolver
Block::LinearSolverType* linearSolver = new g2o::LinearSolverDense<Block::PoseMatrixType>(); 

// 第2步：创建BlockSolver。并用上面定义的线性求解器初始化
Block* solver_ptr = new Block( linearSolver );      

// 第3步：创建总求解器solver。并从GN, LM, DogLeg 中选一个，再用上述块求解器BlockSolver初始化
g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg( solver_ptr );

// 第4步：创建终极大boss 稀疏优化器（SparseOptimizer）
g2o::SparseOptimizer optimizer;     // 图模型
optimizer.setAlgorithm( solver );   // 设置求解器
optimizer.setVerbose( true );       // 打开调试输出

// 第5步：定义图的顶点和边。并添加到SparseOptimizer中
CurveFittingVertex* v = new CurveFittingVertex(); //往图中增加顶点
v->setEstimate( Eigen::Vector3d(0,0,0) );
v->setId(0);
optimizer.addVertex( v );
for ( int i=0; i<N; i++ )    // 往图中增加边
{
  CurveFittingEdge* edge = new CurveFittingEdge( x_data[i] );
  edge->setId(i);
  edge->setVertex( 0, v );                // 设置连接的顶点
  edge->setMeasurement( y_data[i] );      // 观测数值
  edge->setInformation( Eigen::Matrix<double,1,1>::Identity()*1/(w_sigma*w_sigma) ); // 信息矩阵：协方差矩阵之逆
  optimizer.addEdge( edge );
}

// 第6步：设置优化参数，开始执行优化
optimizer.initializeOptimization();
optimizer.optimize(100);
```

结合上面的流程图和代码。下面一步步解释具体步骤。

#### 1、创建一个线性求解器LinearSolver

我们要求的增量方程的形式是：H△X=-b，通常情况下想到的方法就是直接求逆，也就是△X=-H.inv*b。看起来好像很简单，但这有个前提，就是H的维度较小，此时只需要矩阵的求逆就能解决问题。但是当H的维度较大时，矩阵求逆变得很困难，求解问题也变得很复杂。

小白：那有什么办法吗？

师兄：办法肯定是有的。此时我们就需要一些特殊的方法对矩阵进行求逆，你看下图是GitHub上g2o相关部分的代码

![img](图片/20210524_开始学习TEB planner/5c3f18ea1f3c4.png)

如果你点进去看，可以分别查看每个方法的解释，如果不想挨个点进去看，看看下面我的总结就行了

```cpp
LinearSolverCholmod ：使用sparse cholesky分解法。继承自LinearSolverCCS
LinearSolverCSparse：使用CSparse法。继承自LinearSolverCCS
LinearSolverPCG ：使用preconditioned conjugate gradient 法，继承自LinearSolver
LinearSolverDense ：使用dense cholesky分解法。继承自LinearSolver
LinearSolverEigen： 依赖项只有eigen，使用eigen中sparse Cholesky 求解，因此编译好后可以方便的在其他地方使用，性能和CSparse差不多。继承自LinearSolver
```

#### 2、创建BlockSolver。并用上面定义的线性求解器初始化。

BlockSolver 内部包含 LinearSolver，用上面我们定义的线性求解器LinearSolver来初始化。它的定义在如下文件夹内：

[g2o](https://github.com/RainerKuemmerle/g2o/tree/8564e1e365d4e719dceca6269a0ba203f7f43dec)/[g2o](https://github.com/RainerKuemmerle/g2o/tree/8564e1e365d4e719dceca6269a0ba203f7f43dec/g2o)/[core](https://github.com/RainerKuemmerle/g2o/tree/8564e1e365d4e719dceca6269a0ba203f7f43dec/g2o/core)/block_solver.h

你点进去会发现 BlockSolver有两种定义方式

一种是指定的固定变量的solver，我们来看一下定义

```cpp
 using BlockSolverPL = BlockSolver< BlockSolverTraits<p, l> >;
```

其中p代表pose的维度（注意一定是流形manifold下的最小表示），l表示landmark的维度

另一种是可变尺寸的solver，定义如下

```cpp
using BlockSolverX = BlockSolverPL<Eigen::Dynamic, Eigen::Dynamic>;
```

为何会有可变尺寸的solver呢？

这是因为在某些应用场景，我们的Pose和Landmark在程序开始时并不能确定，那么此时这个块状求解器就没办法固定变量，此时使用这个可变尺寸的solver，所有的参数都在中间过程中被确定

另外你看block_solver.h的最后，预定义了比较常用的几种类型，如下所示：

```cpp
BlockSolver_6_3 ：表示pose 是6维，观测点是3维。用于3D SLAM中的BA
BlockSolver_7_3：在BlockSolver_6_3 的基础上多了一个scale
BlockSolver_3_2：表示pose 是3维，观测点是2维
```

以后遇到了知道这些数字是什么意思就行了

#### 3、创建总求解器solver。并从GN, LM, DogLeg 中选一个，再用上述块求解器BlockSolver初始化

我们来看[g2o](https://github.com/RainerKuemmerle/g2o)/[g2o](https://github.com/RainerKuemmerle/g2o/tree/master/g2o)/core/ 目录下，发现Solver的优化方法有三种：分别是高斯牛顿（GaussNewton）法，LM（Levenberg–Marquardt）法、Dogleg法，如下图所示，也和前面的图相匹配

![img](图片/20210524_开始学习TEB planner/5c3f18eaaef6a.png)

上图最后那个OptimizationAlgorithmWithHessian 是干嘛的？

点进去 GN、 LM、 Doglet算法内部，会发现他们都继承自同一个类：OptimizationWithHessian，如下图所示，这也和我们最前面那个图是相符的

![img](图片/20210524_开始学习TEB planner/5c3f18eb07a7c.png)

然后，我们点进去看 OptimizationAlgorithmWithHessian，发现它又继承自OptimizationAlgorithm，这也和前面的相符

![img](图片/20210524_开始学习TEB planner/5c3f18eadbd27.png)

总之，在该阶段，我们可以选则三种方法：

```cpp
g2o::OptimizationAlgorithmGaussNewton
g2o::OptimizationAlgorithmLevenberg 
g2o::OptimizationAlgorithmDogleg 
```

#### 4、创建终极大boss 稀疏优化器（SparseOptimizer），并用已定义求解器作为求解方法。

创建稀疏优化器

```cpp
g2o::SparseOptimizer    optimizer;
```

用前面定义好的求解器作为求解方法：

```cpp
SparseOptimizer::setAlgorithm(OptimizationAlgorithm* algorithm)
```

其中setVerbose是设置优化过程输出信息用的

```cpp
SparseOptimizer::setVerbose(bool verbose)
```

不信我们来看一下它的定义

![img](图片/20210524_开始学习TEB planner/5c3f18eaa307c.png)

#### 5、定义图的顶点和边。并添加到SparseOptimizer中。

**（1）点Vertex**

在g2o中定义Vertex有一个通用的类模板：BaseVertex。在结构框图中可以看到它的位置就是HyperGraph继承的根源。

同时在图中我们注意到BaseVertex具有两个参数D/T，这两个参数非常重要，我们来看一下：

- D 是int 类型，表示vertex的最小维度，例如3D空间中旋转是3维的，则 D = 3
- T 是待估计vertex的数据类型，例如用四元数表达三维旋转，则 T 就是Quaternion 类型

```c++
static const int Dimension = D; ///< dimension of the estimate (minimal) in the manifold spacetypedef T EstimateType;EstimateType _estimate;
```

特别注意的是这个D不是顶点(状态变量)的维度，而是其在流形空间(manifold)的最小表示。

**如何自己定义Vertex?**

在我们动手定义自己的Vertex之前，可以先看下g2o本身已经定义了一些常用的顶点类型：

```go
ertexSE2 : public BaseVertex<3, SE2>  //2D pose Vertex, (x,y,theta)VertexSE3 : public BaseVertex<6, Isometry3> //Isometry3使欧式变换矩阵T，实质是4*4矩阵//6d vector (x,y,z,qx,qy,qz) 
```

但是！如果在使用中发现没有我们可以直接使用的Vertex，那就需要自己来定义了。一般来说定义Vertex需要重写这几个函数（注意注释）：

```go
virtual bool read(std::istream& is);
virtual bool write(std::ostream& os) const;// 分别是读盘、存盘函数，一般情况下不需要进行读/写操作的话，仅仅声明一下就可以
virtual void oplusImpl(const number_t* update);//顶点更新函数
virtual void setToOriginImpl();//顶点重置函数，设定被优化变量的原始值。
```

请注意里面的oplusImpl函数，是非常重要的函数，主要用于优化过程中增量△x 的计算。根据增量方程计算出增量后，通过这个函数对估计值进行调整，因此该函数的内容要重视。

根据上面四个函数可以得到定义顶点的基本格式：

```c++
class myVertex: public g2o::BaseVertex<Dim, Type>
  {
      public:
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
 
      myVertex(){}
 
      virtual void read(std::istream& is) {}
      virtual void write(std::ostream& os) const {}
 
      virtual void setOriginImpl()
      {
          _estimate = Type();
      }
      virtual void oplusImpl(const double* update) override
      {
          _estimate += update;
      }
  }
```

如果还不太明白，那么继续看下面的实例：

```c++
class CurveFittingVertex: public g2o::BaseVertex<3, Eigen::Vector3d>{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW  // 字节对齐
    virtual void setToOriginImpl() // 重置，设定被优化变量的原始值    {
        _estimate << 0,0,0;
    }
 
    virtual void oplusImpl( const double* update ) // 更新    {
        _estimate += Eigen::Vector3d(update);   //update强制类型转换为Vector3d    }
    // 存盘和读盘：留空    virtual bool read( istream& in ) {}
    virtual bool write( ostream& out ) const {}};
```

另外值得注意的是，优化变量更新并不是所有时候都可以像上面两个一样直接 += 就可以，这要看优化变量使用的类型（是否对加法封闭）。

\>>> 向图中添加顶点

接着上面定义完的顶点，我们把它添加到图中：

```c++
CurveFittingVertex* v = new CurveFittingVertex();
v->setEstimate( Eigen::Vector3d(0,0,0) )；  
 // 设定初始值
 v->setId(0);                              
// 定义节点编号
optimizer.addVertex( v ); // 把节点添加到图中
```

**(2)边 Edge**

图优化中的边：BaseUnaryEdge，BaseBinaryEdge，BaseMultiEdge 分别表示一元边，两元边，多元边。

顾名思义，一元边可以理解为一条边只连接一个顶点，两元边理解为一条边连接两个顶点（常见），多元边理解为一条边可以连接多个（3个以上）顶点。

以最常见的二元边为例分析一下他们的参数：D, E, VertexXi, VertexXj：

- D 是 int 型，表示测量值的维度 （dimension）
- E 表示测量值的数据类型
- VertexXi，VertexXj 分别表示不同顶点的类型

```c++
BaseBinaryEdge<2, Vector2D, VertexSBAPointXYZ, VertexSE3Expmap>
```

上面这行代码表示二元边，参数1是说测量值是2维的；参数2对应测量值的类型是Vector2D，参数3和4表示两个顶点也就是优化变量分别是三维点 VertexSBAPointXYZ，和李群位姿VertexSE3Expmap。

\>>> 如何定义一个边

除了上面那行定义语句，还要复写一些重要的成员函数：

```c++
virtual bool read(std::istream& is);
virtual bool write(std::ostream& os) const;// 分别是读盘、存盘函数，一般情况下不需要进行读/写操作的话，仅仅声明一下就可以
virtual void computeError();// 非常重要，是使用当前顶点值计算的测量值与真实测量值之间的误差，测量值维度与_error维度相一致
virtual void linearizeOplus();// 非常重要，是在当前顶点的值下，该误差对优化变量的偏导数，也就是Jacobian矩阵
```

除了上面四个函数，还有几个重要的成员变量以及函数：

```c++
_measurement； // 存储观测值
    _error;  // 存储computeError() 函数计算的误差
_vertices[]; // 存储顶点信息，比如二元边，_vertices[]大小为2
//存储顺序和调用
setVertex(int, vertex) //和设定的int有关（0或1）
setId(int);  // 定义边的编号（决定了在H矩阵中的位置）
setMeasurement(type);  // 定义观测值
setVertex(int, vertex);  // 定义顶点
setInformation();  // 定义协方差矩阵的逆
```

有了上面那些重要的成员变量和成员函数，就可以用来定义一条边了：

```c++
class myEdge: public g2o::BaseBinaryEdge<errorDim, errorType, Vertex1Type, Vertex2Type>
  {
      public:
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW      
 
      myEdge(){}    
      virtual bool read(istream& in) {}
      virtual bool write(ostream& out) const {}      
      virtual void computeError() override
      {
          // ...          _error = _measurement - Something;
      }    
  
      virtual void linearizeOplus() override  // 求误差对优化变量的偏导数，雅克比矩阵      {
          _jacobianOplusXi(pos, pos) = something;
          // ...        
          /*          _jocobianOplusXj(pos, pos) = something;          ...          */
      }      
      private:
      data
  }
```

让我们继续看curveftting这个实例，这里定义的边是简单的一元边：

```c++
// （误差）边的模型    模板参数：观测值维度，类型，连接顶点类型
class CurveFittingEdge: public g2o::BaseUnaryEdge<1,double,CurveFittingVertex>{public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    CurveFittingEdge( double x ): BaseUnaryEdge(), _x(x) {}
    // 计算曲线模型误差    void computeError()
    {
        const CurveFittingVertex* v = static_cast<const CurveFittingVertex*> (_vertices[0]);
        const Eigen::Vector3d abc = v->estimate();
        _error(0,0) = _measurement - std::exp( abc(0,0)*_x*_x + abc(1,0)*_x + abc(2,0) ) ;
    }
    virtual bool read( istream& in ) {}
    virtual bool write( ostream& out ) const {}public:
    double _x;  // x 值， y 值为 _measurement};
```

上面的例子都比较简单，下面这个是3D-2D点的PnP 问题，也就是最小化重投影误差问题，这个问题非常常见，使用最常见的二元边，弄懂了这个基本跟边相关的代码就能懂了：

\>>>向图中添加边

和添加点有一点类似，下面是添加一元边：

```c++
// 往图中增加边    for ( int i=0; i<N; i++ )
    {
        CurveFittingEdge* edge = new CurveFittingEdge( x_data[i] );
        edge->setId(i);
        edge->setVertex( 0, v );                // 设置连接的顶点        edge->setMeasurement( y_data[i] );      // 观测数值        edge->setInformation( Eigen::Matrix<double,1,1>::Identity()*1/(w_sigma*w_sigma) ); // 信息矩阵：协方差矩阵之逆        optimizer.addEdge( edge );
    }
```

我们经常要使用的二元边（前后两个位姿），那么此时：

```c++
index = 1;for ( const Point2f p:points_2d ){
    g2o::EdgeProjectXYZ2UV* edge = new g2o::EdgeProjectXYZ2UV();
    edge->setId ( index );  // 边的编号    edge->setVertex ( 0, dynamic_cast<g2o::VertexSBAPointXYZ*> ( optimizer.vertex ( index ) ) );
    edge->setVertex ( 1, pose );
    edge->setMeasurement ( Eigen::Vector2d ( p.x, p.y ) );  // 设置观测的特征点图像坐标    edge->setParameterId ( 0,0 );
    edge->setInformation ( Eigen::Matrix2d::Identity() ); //信息矩阵的维度与边的观测维度相一致，因为信息矩阵存的就是cost的权重信息
    optimizer.addEdge ( edge );
    index++;}
```

#### 6、设置优化参数，开始执行优化。

设置SparseOptimizer的初始化、迭代次数、保存结果等。

初始化

```cpp
SparseOptimizer::initializeOptimization(HyperGraph::EdgeSet& eset)
```

设置迭代次数，然后就开始执行图优化了。

```cpp
SparseOptimizer::optimize(int iterations, bool online)
```

### 参考资料

g2o框架简介及编程

https://www.cnblogs.com/CV-life/p/10286037.html

https://www.cnblogs.com/gaoxiang12/p/5304272.html

https://yongqi.blog.csdn.net/article/details/105259976

数学建模

https://blog.csdn.net/qit1314/article/details/84886612



## 存在的问题

1. 为什么有的边不用求一阶雅克比矩阵？不是后面计算的时候一定会遇到么？
2. edge_velocity里面的速度正负是怎么判断的？
3. 误差目标函数的形式？雅克比矩阵？
4. *class* EdgeObstacle : *public* BaseTebUnaryEdge<1, *const* Obstacle*, VertexPose>，为什么这里的数据类型是*const* Obstacle*？与障碍物的距离不应该是double类型么？
5. TebOptimalPlanner类中的成员变量obstacles_per_vertex_是什么？
6. 动态障碍物如何处理？



**解答**

1.在作者论文中找到g2o框架可以自动利用数值近似求得雅克比矩阵<img src="图片/20210524_开始学习TEB planner/2021-06-01 11-04-05 的屏幕截图.png" style="zoom:80%;" />



## 对TEB的改进或作者在论文中没有提到的点（论文创新点）

1. teb未考虑位姿点序列整体的平滑性，考虑将平滑代价作为误差项封装成g2o的超图的边进行图优化。实验结果可以对比优化前后的点序列的曲率
2. updateAndPruneTEB保证时间一致性问题
3. 速度的正方向判断

2021.6.3组会记录

1. 搜索数值优化比如非线性MPC类似的论文，首先看TEB作者那篇与非线性MPC有关的文章
2. 思考一个TEB算法适用的特定场景，比如况师兄说这种物流车的倒车是很重要的问题，正好TEB对倒车的处理十分简单。TEB在狭窄空间的规划做的也不好，尤其在狭窄空间的倒车问题更复杂，TEB解决的不好
3. TEB障碍物检测算法目前十分简单，就像况师兄所说障碍物如果抽象为多边形，车辆轮廓也抽象为多边形，多边形之间的障碍物检测是十分复杂的，也需要看看障碍物检测方向的论文或方法，比如AABBbox，OBBbox等，把TEB的障碍物检测改进更好
4. 通过仿真或实车调试，首先把参数都调清，第一个就是看可不可以通过调参来进行倒车
5. 况师兄提供了一个AGV底盘车的场景很适合研究，即**非结构道路下的窄路通行**，目前研究的不多且有很大研究意义，比如在狭窄道路前有一辆自行车，此时可能不能像自动驾驶那样的结构化道路下，考虑障碍物直接通行，而是可能需要停下来调整多次再规划可行轨迹。上海交大有一篇大论文，知网搜索《最优状态点》，还有自动化学报一篇小论文

## **阅读代码过程中的知识扩展**

c++中 .hpp和.h文件的区别

https://blog.csdn.net/qq_41986495/article/details/87888775

vector的remove和erase函数的区别

https://blog.csdn.net/xzymmd/article/details/83652726

static_cast和dynamic_cast强制类型转换

https://www.cnblogs.com/wft1990/p/7126812.html

c++ 四种强制类型转换

https://blog.csdn.net/u010540025/article/details/81231495

Eigen内存分配器aligned_allocator

https://blog.csdn.net/weixin_30535565/article/details/95627806

