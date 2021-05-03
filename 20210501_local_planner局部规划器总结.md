# local_planner局部规划器总结

局部规划可以说是整个导航包最为复杂的一个包了，base_lcoal_planner头文件30个，代码近1万行。局部导航主要包括Trajectory Rollout 和 Dynamic Window Approach  (DWA)两种方法，理论上来说分别对应base_local_planner和dwa_local_planner两个包，但其实**dwa的大部分代码都放在了base_local_planner包里面**。

## 1.框架    

同样，对于这种大工程，我们还是从类继承图来着手：

![img](https://pic2.zhimg.com/80/v2-aa0c2c2cef7da0ee66f140a3d87578d5_720w.jpg)

TrajectoryCostFunction，这个接口主要规定了一个scoreTrajectory函数，也就是走过一个轨迹需要付出的代价，在wiki上可以看到对4种代价的详细介绍；TrajectorySampleGenerator明显是产生一系列轨迹用的，接口规定了nextTrajectory用来遍历所有轨迹；TrajectorySearch接口规定了一个findBestTrajectory，找到最好的轨迹；worldModel，ros注释里面有一句介绍：*Subclass will implement this method to check a footprint at a given position and orientation for legality in the world。*

以上接口是局部规划的核心，简单来说，TrajectorySampleGenerator产生一系列轨迹，然后TrajectoryCostFunction遍历轨迹打分，TrajectorySearch找到最好的轨迹拿来给小车导航；由于小车不是一个质点，worldModel会检查小车有没有碰到障碍物。

### 剩余的类

仔细观察include目录发现，除了前面提到的这些类，还有很多类没有覆盖到。排除掉wiki中提到的helper  class，剩下有两个类非常显眼：**trajectory_planner跟trajectory_planner_ros**。稍微浏览了一下这两个类的内容，惊了，这两个类似乎自成一系，像前面提到的产生轨迹 轨迹打分 选最好轨迹都有相应的函数。这是怎么一回事呢？为什么前面定义了半天的接口又不用了呢？

**用vscode的全局搜索发现，原来这些接口根本就没在base_lcoal_planner里面用，而是在dwa_local_planner这个包里面使用。**联系wiki里面提到的两种方法，我好像明白了其中的原因，原来，事情并不是这么简单。

这个trajectory_planner很可能是一开始ros设计的一个局部规划器，可是后来发现这样写成一个大类不利于扩展。于是，**ros重新采用接口化的设计思路，将整个规划器分为好几个接口重新设计了一个dwa_local_planner，并且不知道为啥不放到dwa_local_planner里，而是跟之前的base_local_planner放一起了。于是，在这个包里明显感觉到这两部分是格格不入的。**

好了，现在大局已定，整个框架算是清楚了（每次都是先把框架弄清楚），下面深入看看各个接口下的具体实现。

### TrajectoryCostFunction

**TrajectoryCostFunction可是说是最重要的一个接口**。**MapGridCostFunction用来评估局部规划的轨迹离全局规划的轨迹的距离，也可以用来评估到目标的距离**，它维护了一个base_local_planner::MapGrid map_，MapGrid是一系列MapCell，而MapCell包含了一个int  target_dist，也就是说，MapGridCostFunction建立后随时知道地图上一个点到全局规划轨迹的距离，或者是到目标的距离。具体是在computeTargetDistance中实现的，实现的方法跟之前amcl包维护地图网格离最近障碍物的距离用到的方法一样，也是用队列实现了一格格膨胀的效果（或者叫波动，wave propagation）。在dwa_local_planner里建立了4个mapGrid用来计算4个不同的方面，具体的内容如下：

![img](https://pic2.zhimg.com/80/v2-23972b8b7b4082942fe860d4a4247bd5_720w.jpg)

ObstacleCostFunction就是计算小车在costmap上行走的代价，看看是否撞到障碍。其中ObstacleCostFunction用到了worldModel接口来检测底座是不是撞到障碍物，从初始化函数可以看出来，ObstacleCostFunction选用了CostmapModel作为具体实现，想改为其他具体实现随时可以改这一行。

后面两个costFunction就没什么内容了，一个是判断小车是不是老是震动，一个是判断小车会不会倒着走。在具体的规划中，想用哪个就把哪个加进去就得了，非常方便，**dwa_local_planer就preferForwardCostFunction没用，其他都用到了。**

### SimpleTrajectoryGenerator

SimpleTrajectoryGenerator这个类就是根据加速度的限制产生一系列轨迹（轨迹就是速度跟点的集合，在trajectory.h中定义），以供后面打分挑选。比如当前速度是5，允许的速度范围是0-10，那么这个类会产生10个目标速度值，然后根据加速度限制将当前速度尽量加速或减速到目标值，持续一定的步长，并记录生成的所有轨迹。可见这个轨迹生成类也算是人如其名，too simple。

### SimpleScoredSamplingPlanner

SimpleScoredSamplingPlanner这个类目的很明确，就是打分，找到最好的轨迹。看它的两个变量就知道它要干嘛了：

```text
private:
 std::vector<TrajectorySampleGenerator*> gen_list_;
 std::vector<TrajectoryCostFunction*> critics_;
```

产生一系列轨迹，然后用一系列costFunction打分加起来，选最好的那个。**这里的轨迹生成器虽然是一个list，但是其实在dwa_local_planner就放了一个进去。costFunction放了6个。**

### 总结

**worldModel除了costmapModel在obstacle_cost_function里面用到之外，其他两个并没用到**，所以这里就不看了。回顾整个工程，可以发现这种接口化的设计有种说不出的美感，每一步的实现都具体踏实，调用又简洁可靠，将一个复杂的项目简化到游刃有余的地步，实在厉害。原来的非接口的trajectory_planner就不看了，有了电驴还要啥自行车（主要因为懒）。



## 2.函数调用顺序：从move_base开始

**move_base关于局部规划器的调用流程图**

![在这里插入图片描述](图片/20210501_local_planner局部规划器总结/20181204171856453.png)

### 从最外层开始——move_base.h/cpp

在move_base中声明了一个指向基类的共享指针tc_

<img src="图片/20210501_local_planner局部规划器总结/2021-05-02 15-55-57 的屏幕截图.png" style="zoom:67%;" />

通过插件类pluginlib与多态性实现对具体局部规划器的加载与初始化

<img src="图片/20210501_local_planner局部规划器总结/2021-05-02 15-58-41 的屏幕截图.png" style="zoom:67%;" />

tc_在move_base中共出现了10处、除了刚才的初始化与动态参数配置，最重要的用在executeCycle主线程循环中

tc_->setPlan()用于设置全局规划路径到局部规划器中作为参考线

tc_->isGoalReached()用于判断是否到达目标点，如果到达会停止全局规划（runPlanner\_=false）,将状态重置

tc_->computeVelocityCommands()是局部规划器工作的主要函数，调用流程如上图所示。

### 中层——dwa_planner_ros.h/cpp

dwa_planner_ros继承自BaseLocalPlanner，是一个局部规划器插件，其中重写了上述，也是局部规划基类定义的接口，即initialize、computeVelocityCommands、setPlan、isGoalReached。

它封装了底层dwa_planner的一些功能，该类的功能类似costmap中的包工头类layeredcostmap，因此它就是将底层planner功能组织在一起，来实现基类定义的接口。因此该类看上去很清爽，功能结构一目了然，底层实现都在dwa_planner中来完成。

重要的成员变量,

<img src="图片/20210501_local_planner局部规划器总结/2021-05-02 16-13-48 的屏幕截图.png" style="zoom:67%;" />

首先需要一个局部地图costmap_ros\_，planner_util\_是一个帮助局部规划实现的类，dp_是一个指向底层DWAplanner实现的指针，在重写基类接口时用来调用底层实现。

<img src="图片/20210501_local_planner局部规划器总结/2021-05-02 16-16-28 的屏幕截图.png" style="zoom: 80%;" />



### 底层——dwa_planner.h/cpp

该类才是dwa算法的核心类，设计dwa算法的核心步骤，比如采样生成轨迹簇、轨迹检查、轨迹评分等。因此先介绍DWA算法涉及的概念

dwa说白了是一种在控制空间采样的搜索算法，也算lattice规划的一种，只不过lattice大多是在状态空间进行采样。

### 最底层实现

读了dwa_planner发现它还不是最底层的类，因为诸如轨迹评分、生成轨迹都是调用这个类内定义好的对象，因此最底层是这些类对象。

其中最关键的为simple_trajectory_generator,simple_scored_sampling_planner以及几个cost_function类

## DWA算法

DWA算法全称为dynamic window approach，其原理主要是在速度空间（v,w）中采样多组速度，并模拟这些速度在一定时间内的运动轨迹，再通过一个评价函数对这些轨迹打分，最优的速度被选择出来发送给下位机。

![在这里插入图片描述](图片/20210501_local_planner局部规划器总结/20190528103444842.png)

![在这里插入图片描述](图片/20210501_local_planner局部规划器总结/20190528103512407.png)

![在这里插入图片描述](图片/20210501_local_planner局部规划器总结/20190528103555693.png)

## 算法程序分析

####  （1）初始化：

  在move_base节点中，通过类加载模块载入了BaseLocalPlanner（局部路径规划）的子类DWAPlannerROS的实例tc\_，并调用其初始化函数，获取了一些初始状态信息比如机器人当前位置等，并创建了真正实现DWA算法的DWAPlanner类的实例dp\_，最后设置了动态参数配置服务。dp\_的构造函数做了一系列参数获取的操作，最重要的是将几种cost计算方法的实例加入一个名为critics的vector容器里。

####  （2）采样速度样本：

  当move_base调用tc\_的computeVelocityCommands方法后，tc\_会调用dwaComputeVelocityCommands方法，并在其中调用dp\_的findBestPath方法。findBestPath方法里调用SimpleTrajectoryGenerator类的实例generator\_的initialise函数，这个函数就是主要负责速度采样的。
  每个维度速度需要采样的样本数存放在vsamples\_这个结构体内，vsamples\_[0]是x方向样本数，vsamples\_[1]是y方向样本数，vsamples\_[2]是z方向样本数。首先计算各个方向的最大速度和最小速度，DWA算法只在第一步进行采样，所以最大速度为:
 Max_vel=min(max_vel,vel+acc\_lim*sim_period)
 最小速度为:
 Min_vel=max(min_vel,vel-acc\_lim\*sim\_period)
 其中max_vel，min_vel为人为设定的最大和最小速度，vel是当前速度，acc_lim是人为设定的最大加速度，sim_period是第一步的模拟时间，由人为设定的局部路径规划频率决定，默认为0.05。
 当计算出各个维度的最大最小速度后，就创建三个VelocityIterator类的对象，并传入最大最小速度和样本数目，此对象的构造函数会生成同样数目的速度样本并放入samples\_这个容器内。具体做法是先计算步长step_size：
 step\_size=(max-min)/(nums_samples-1)
  max为最大速度，min为最小速度，nums_samples为样本数目。从最小速度每次多累加一次step_size即为一个速度样本，直到达到最大速度。将每个维度的速度样本取得后，再全部循环每个样本组里选择一个组合放入结构体vel_sample，最后将这些vel_sample放入sample_params\_的容器里。至此，速度采样就完成了。

####  （3）样本评分

  速度采样完成后，逐一循环对样本空间内的样本进行评分。对每一组速度调用scoreTrajectory函数计算其评分，而scoreTrajectory函数则对这一组速度调用所有critics容器里的costfunction计算每个cost从而累加算出总的cost。在计算过程中，一旦累加的cost大于当前最小的cost则抛弃这组速度。
 之前说到的几种cost成本函数为下列所示：
 ObstacleCostFunction
 这个成本函数基于感知障碍物来评估轨迹。它或者由于轨迹通过障碍物而返回负值，或者0。
 MapGridCostFunction
 这个成本函数类基于轨迹离全局路径或者接近目标点有多近来评估轨迹。这个尝试利用距离预计算地图有相同距离的路径或者目标点的所有的规划，来优惠计算速度。
 在 dwa_local_planner中，代价函数因为不同的目的，被多次实例化。保持轨迹接近于路径，使机器人朝局部目标前进，并且使机器人的前段点指向局部目标。代价函数是一个启发，可以带来坏的结果或者不合适的参数的失败。
 OscillationCostFunction
  震荡发生在X,Y,theta维度上，正/负值被连续的选择。为了阻止震荡，当机器人在任何方向移动时，与下一个循环相反的方向被标记为无效，直到机器人已经从所设置标记的位置移动而并且超过一定的距离。这个成本函数类帮助减少某些震荡，虽然这可以有效的阻止这些震荡，如果使用不合适的参数，但是有可能阻止良好的解。
 PreferForwardCostFunction
 考虑到好的激光扫描范围只在机器人的前面，这个成本函数类被设计在像PR2一样的机器人上。成本函数更喜欢正面向前运动，惩罚背面运用及扫射动作。在其他机器人上或者其他领域，这可能是非常不可取的行为。

####  （4）发布plan

 通过上述几种评分机制，选取最优的一组速度样本，传递给move_base，并发布相应的local  plan。move_base如果收到了可用的速度则发布给底盘，否则发布0速度，且如果寻找最优速度的时间超过了限制就会执行障碍物清理模式，state_会变为CLEARING。

