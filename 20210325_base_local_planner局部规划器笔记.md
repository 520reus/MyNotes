# base_local_planner局部规划器笔记

局部导航可以说是整个导航包最为复杂的一个包了，base_lcoal_planner头文件30个，代码近1万行。从[wiki](https://link.zhihu.com/?target=http%3A//wiki.ros.org/base_local_planner)上可以看到，局部导航主要包括Trajectory Rollout 和 Dynamic Window Approach  (DWA)两种方法，理论上来说分别对应base_local_planner和dwa_local_planner两个包，但其实dwa的大部分代码都放在了base_local_planner包里面。

## 框架    

同样，对于这种大工程，我们还是从类继承图来着手：

![img](https://pic2.zhimg.com/80/v2-aa0c2c2cef7da0ee66f140a3d87578d5_720w.jpg)

TrajectoryCostFunction，这个接口主要规定了一个scoreTrajectory函数，也就是走过一个轨迹需要付出的代价，在wiki上可以看到对4种代价的详细介绍；TrajectorySampleGenerator明显是产生一系列轨迹用的，接口规定了nextTrajectory用来遍历所有轨迹；TrajectorySearch接口规定了一个findBestTrajectory，找到最好的轨迹；worldModel，ros注释里面有一句介绍：*Subclass will implement this method to check a footprint at a given position and orientation for legality in the world。*

以上接口是局部规划的核心，简单来说，TrajectorySampleGenerator产生一系列轨迹，然后TrajectoryCostFunction遍历轨迹打分，TrajectorySearch找到最好的轨迹拿来给小车导航；由于小车不是一个质点，worldModel会检查小车有没有碰到障碍物。

## 剩余的类

仔细观察include目录发现，除了前面提到的这些类，还有很多类没有覆盖到。排除掉wiki中提到的helper  class，剩下有两个类非常显眼：trajectory_planner跟trajectory_planner_ros。稍微浏览了一下这两个类的内容，**惊了**，这两个类似乎自成一系，像前面提到的产生轨迹 轨迹打分 选最好轨迹都有相应的函数。这是怎么一回事呢？为什么前面定义了半天的接口又不用了呢？

用vscode的全局搜索发现，原来这些接口根本就没在base_lcoal_planner里面用，而是在dwa_local_planner这个包里面使用。联系wiki里面提到的两种方法，我好像明白了其中的原因，原来，事情并不是这么简单。

这个trajectory_planner很可能是一开始ros设计的一个局部规划器，可是后来发现这样写成一个大类不利于扩展。于是，ros重新采用接口化的设计思路，将整个规划器分为好几个接口重新设计了一个dwa_local_planner，并且不知道为啥不放到dwa_local_planner里，而是跟之前的base_local_planner放一起了。于是，在这个包里明显感觉到这两部分是格格不入的。

好了，现在大局已定，整个框架算是清楚了（每次都是先把框架弄清楚），下面深入看看各个接口下的具体实现。

## TrajectoryCostFunction

TrajectoryCostFunction可是说是最重要的一个接口。MapGridCostFunction用来评估局部规划的轨迹离全局规划的轨迹的距离，也可以用来评估到目标的距离，它维护了一个base_local_planner::MapGrid map_，MapGrid是一系列MapCell，而MapCell包含了一个int  target_dist，也就是说，MapGridCostFunction建立后随时知道地图上一个点到全局规划轨迹的距离，或者是到目标的距离。具体是在computeTargetDistance中实现的，实现的方法跟之前amcl包维护地图网格离最近障碍物的距离用到的方法一样，也是用队列实现了一格格膨胀的效果（或者叫波动，wave propagation）。在dwa_local_planner里建立了4个mapGrid用来计算4个不同的方面，具体的内容如下：

![img](https://pic2.zhimg.com/80/v2-23972b8b7b4082942fe860d4a4247bd5_720w.jpg)

ObstacleCostFunction就是计算小车在costmap上行走的代价，看看是否撞到障碍。其中ObstacleCostFunction用到了worldModel接口来检测底座是不是撞到障碍物，从初始化函数可以看出来，ObstacleCostFunction选用了CostmapModel作为具体实现，想改为其他具体实现随时可以改这一行。

后面两个costFunction就没什么内容了，一个是判断小车是不是老是震动，一个是判断小车会不会倒着走。在具体的规划中，想用哪个就把哪个加进去就得了，非常方便，dwa_local_planer就preferForwardCostFunction没用，其他都用到了。

## SimpleTrajectoryGenerator

SimpleTrajectoryGenerator这个类就是根据加速度的限制产生一系列轨迹（轨迹就是速度跟点的集合，在trajectory.h中定义），以供后面打分挑选。比如当前速度是5，允许的速度范围是0-10，那么这个类会产生10个目标速度值，然后根据加速度限制将当前速度尽量加速或减速到目标值，持续一定的步长，并记录生成的所有轨迹。可见这个轨迹生成类也算是人如其名，too simple。

## SimpleScoredSamplingPlanner

SimpleScoredSamplingPlanner这个类目的很明确，就是打分，找到最好的轨迹。看它的两个变量就知道它要干嘛了：

```text
private:
 std::vector<TrajectorySampleGenerator*> gen_list_;
 std::vector<TrajectoryCostFunction*> critics_;
```

产生一系列轨迹，然后用一系列costFunction打分加起来，选最好的那个。这里的轨迹生成器虽然是一个list，但是其实在dwa_local_planner就放了一个进去。costFunction放了6个。

## 总结

worldModel除了costmapModel在obstacle_cost_function里面用到之外，其他两个并没用到，所以这里就不看了。回顾整个工程，可以发现这种接口化的设计有种说不出的美感，每一步的实现都具体踏实，调用又简洁可靠，将一个复杂的项目简化到游刃有余的地步，实在厉害。原来的非接口的trajectory_planner就不看了，有了电驴还要啥自行车（主要因为懒）。