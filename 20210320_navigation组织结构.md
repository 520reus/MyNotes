# move_base学习笔记

## move_base框架结构

![attachment:overview_tf.png](图片/20210320_move_base学习笔记/move_base)



## ROS navigation中各package的作用

### 1.nav_core

该包定义了整个导航系统关键包的接口函数，包括`base_global_planner`, `base_local_planner`以及`recovery_behavior`的接口。里面的函数全是虚函数，所以该包只是起到规范接口的作用，真正功能的实现在相应的包当中。**之后添加自己编写的全局或局部导航算法也需要在这里声明**

### 2.move_base

该包是整个navigation stack当中进行宏观调控的manager。它主要干的事情是这样的：
　维护一张全局地图（基本上是不会更新的，一般是静态costmap类型），维护一张局部地图（实时更新，costmap类型），维护一个全局路径规划器global_planner完成全局路径规划的任务， 维护一个局部路径规划器base_local_planner完成局部路径规划的任务。
　然后提供一个对外的服务，负责监听nav_msgs::goal类型的消息，然后调动全局规划器规划全局路径，再将全局路径送进局部规划器，局部规划器结合周围障碍信息（从其维护的costmap中查询），全局路径信息，目标点信息采样速度并评分获得最高得分的轨迹（即是采样的最佳速度），然后返回速度值，由move_base发送Twist类型的cmd_vel消息上，从而控制机器人移动，完成导航任务。

### 3.global_planner、navfn、carrot_planner

在nav_core包中有关于BaseGlobalPlanner接口的定义，这三个包都继承并实现了nav_core中定义的BaseGlobalPlanner类，前两者都包含了A*和Dijkstra算法的实现，可以说global_planner是navfn的升级版，代码结构更优，更清晰。

> 注：当前ros版本中默认使用的是navfn。carrot_planner是另外一种全局的路径规划

**global_planner:**

A*、Dijstra、prm、人工势场、单元分解、快速搜索树（RRT）等

### 4.base_local_planner、dwa_local_planner

这两个package是关于局部路径规划，机器人运动速度的具体生成在此包当中完成。目前有两种局部路径规划算法实现，一是航迹推算法（TrajectoryROS），一是动态窗口法(DWA)，该包内部的默认实现是航迹推算法，但是留出了DWA的定义接口，DWA的实现在dwa_local_planner中。两者都是继承并实现了nav_core中定义的BaseLocalPlanner类

**local_planner:**

base_local_planner、dwa_local_planner、teb_local_planner、eband_local_planner、asr_ftc_local_planner、dwb_local_planner

### 5.map_server

该package的主要功能是通过map_server节点读取yaml配置文件、加载pgm地图文件，发布static_map service，发布map_metadata 和 map 的topic。命令行如下：

```rosrun map_server map_server 1.yaml
rosrun map_server map_server 1.yaml
```

另外map_server还提供地图保存的功能，floor 1为要保存的地图名称，命令如下：

```
rosrun map_server map_saver -f floor1
```

### 6.AMCL

该包是一个二维移动机器人的概率定位系统，它实现了自适应（或KLD采样）蒙特卡罗定位方法，该方法使用粒子滤波器根据已知地图 ( 先验地图 ) 跟踪机器人的姿态。

### 7.rotate_recovery、clear_costmap_recovery

这两个包都继承自nav_core中定义的recovery_behavior类， 具体实现是：当导航发现无路可走的时候，机器人在原地旋转，并更新周围障碍物信息看是否有动态障碍物运动开，如果能够找到路就继续走。

在move_base当中，默认使用的是recovery_behavior，先进行一次旋转，然后进行一次小半径的障碍物更新，然后再进行一次旋转，再进行一次大范围的障碍物更新，每进行一次recovery_behavior，都会重新尝试进行一次局部寻路，如果没找到，才会再执行下一个recovery_behavior。这两个包的代码非常简单，不看也不会影响理解偏差，所以可以不用浪费时间看，当然因为简单要看也是分分钟的事。

### 8.costmap_2d

该包以层的概念来组织图层，move_base中默认的层有static_layer（通过订阅map_server的/map主题）来生成，obstacle_layer根据传感器获得的反馈来生成障碍物图层， inflation_layer 则是将前两个图层的信息综合进行缓冲区扩展。

## 很棒的简介文档

https://www.zhihu.com/column/ros-nav


## 未解决的问题

ROS_DEBUG_NAMED的作用是？