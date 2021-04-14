# ros action总结

结合自己写的功能包理解：/home/wu/catkin_test_ws/src/tutorial_action

相关文章网址：

https://www.guyuehome.com/908



ROS中常用的通讯机制是topic和service，但是在很多场景下，这两种通讯机制往往满足不了我们的需求，比如机械臂控制，如果用topic发布一个运动目标，由于topic没有反馈，还需要另外订阅一个机械臂状态的topic，来获得运动过程的状态。如果用service来发布运动目标，虽然可以获得反馈，但是反馈只有一次，对于我们的控制来讲数据太少了，而且如果反馈迟迟没收到，也只能傻傻等待，干不了其他事情。那么有没有一种更加适合的通讯机制，来满足类似这样场景的需求呢？当然是有的，就是我们这篇要讲到的action通讯机制。

# 一、什么是action

ROS中有一个actinlib的功能包集，实现了action的通讯机制。那么什么是action呢？action也是一种类似于service的问答通讯机制，不一样的地方是action还带有一个反馈机制，可以不断反馈任务的实施进度，而且可以在任务实施过程中，中止运行。

回到我们前边提到的场景，我们使用action来发布一个机器人的运动目标，机器人接到这个action后，就开始运动，在运动过程中不断反馈当前的运动状态，在运动过程中我们可以取消运动，让机器人停止，如果机器人完成了运动目标，那么action会返回任务完成的标志。

# 二、action的工作机制

action采用了服务器端/客户端（client and server）的工作模式，如下图所示：



[![clip_image002](https://www.guyuehome.com/Uploads/wp/2017/05/clip_image002_thumb.png)](https://www.guyuehome.com/Uploads/wp/2017/05/clip_image002_thumb.png)



client和server之间通过actionlib定义的“action protocol”进行通讯。这种通讯协议是基于ROS的消息机制实现的，为用户提供了client和server的接口，接口如下图所示：



[![clip_image004](https://www.guyuehome.com/Uploads/wp/2017/05/clip_image004_thumb.png)](https://www.guyuehome.com/Uploads/wp/2017/05/clip_image004_thumb.png)



在上边的action的接口框图上，我们可以看到，client向server端发布任务目标以及在必要的时候取消任务，server会向client发布当前的状态、实时的反馈和最终的任务结果。

1. goal：任务目标  
2. cancel：请求取消任务  
3. status：通知client当前的状态  
4. feedback：周期反馈任务运行的监控数据  
5. result：向client发送任务的执行结果，这个topic只会发布一次。

