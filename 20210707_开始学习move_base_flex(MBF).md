# 开始学习move_base_flex(MBF)

官方文档：https://wiki.ros.org/move_base_flex

github：https://github.com/magazino/move_base_flex

## 概述

Move Base Flex(MBF)是一个灵活的移动机器人导航框架，它改善了move_base**紧耦合**、**不利于复杂场景应用**的缺点。也改善了flexible navigation状态处理时costmap**多次实例化**导致**算力需求激增**的问题。它的后向兼容性实现与flexible navigation类似，即提供导航模块的插件接口，只要插件是继承并重写move_base的nav_core接口，那么就可以用于MBF框架中。MBF已经在高校以及公司落地，MBF在magazino公司的机器人上已经运行超过一年，应用于六个不同的商业场景，平均每天跑1.3公里。

## 要点总结

1. MBF支持在运行时切换插件的功能，这样使得在不同场景切换规划器和控制器的愿望得以实现。同时提供action与service作为导航功能实现的接口。

2. MBF将实例层与抽象层分离，使得抽象层独立于地图存在，而实例层才是真正用到costmap和各种插件地方。整体框架利用增强版的action与plugin得以实现灵活性

   ![](图片/20210707_开始学习move_base_flex(MBF)/2021-07-07 21-05-03 的屏幕截图.png)

3. get_path:这个action用于全局路径规划，用户需要决定使用哪个全局规划器，该模块反馈计算过的路径（附加cost）以及输出一些消息。

4. exe_path：这个action用于局部轨迹规划，用户需要决定使用哪个局部规划器，在规划过程中，该模块返回现在的车辆位置，速度，与目标位置的距离以及角度。

5. recovery：这个action在导航遇到问题时执行恢复行为，反馈相关的消息。

6. move_base:这个action提供简单的导航任务，比如从A到B的点对点导航，就像move_base一样，它反馈与exe_path相同的信息。

7. MBF支持加载多个控制器、规划器、恢复行为插件，并且同一个插件可以以不同参数加载多次，这使得不同场景下切换插件变得可能。

   <img src="图片/20210707_开始学习move_base_flex(MBF)/2021-07-07 21-08-52 的屏幕截图.png" style="zoom: 80%;" />

8. MBF通过SMACH或behavior tree来实现状态的转移与插件的切换。

   <img src="图片/20210707_开始学习move_base_flex(MBF)/2021-07-07 21-12-04 的屏幕截图.png" style="zoom:80%;" />

9. MBF提高了move_base对场景的鲁棒性，并且更加节约时间。

   <img src="图片/20210707_开始学习move_base_flex(MBF)/2021-07-07 21-11-24 的屏幕截图.png" style="zoom:67%;" />

   