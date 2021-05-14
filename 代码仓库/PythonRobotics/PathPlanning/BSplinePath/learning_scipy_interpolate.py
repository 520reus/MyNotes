"""
插值是离散函数逼近的重要方法，利用它可通过函数在有限个点处的取值状况，估算出函数在其他点处的近似值。与拟合不同的是，
要求曲线通过所有的已知数据。SciPy的interpolate模块提供了许多对数据进行插值运算的函数，范围涵盖简单的一维插值到复杂
多维插值求解。当样本数据变化归因于一个独立的变量时，就使用一维插值；反之样本数据归因于多个独立变量时，使用多维插值。

计算插值有两种基本的方法，
1、对一个完整的数据集去拟合一个函数；
2、对数据集的不同部分拟合出不同的函数，而函数之间的曲线平滑对接。第二种方法又叫做仿样内插法，当数据拟合函数形式非常复杂
时，这是一种非常强大的工具。我们首先介绍怎样对简单函数进行一维插值运算，然后进一步深入比较复杂的多维插值运算。
"""
import scipy.interpolate as scipy_interpolate
import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
p1 = fig.add_subplot(211)

x = np.linspace(0,10,100)
y = np.sin(x)
p1.plot(x,y)
p1.set(xlim=[0.5,4.5],ylim=[-2,8],title="An",ylabel="y",xlabel="x",label="wu")

p2 = fig.add_subplot(212)
x1=[1,2,3,4,5,6,7,8]
y1=[2,1,3,5,2,6,12,7]
p2.plot(x1,y1,':b',linewidth=2.0,label="wf")

plt.legend()
plt.show()
# xnew = np.linspace(0,10,101)
# plt.plot(x,y,'ro')
# list1 = ['linear','nearest']
# list2 = [0,1,2,3]
# for kind in list2:
#     print(kind)
#     f = scipy_interpolate.interp1d(x,y,kind=kind)
#     ynew = f(xnew)
#     plt.plot(xnew,ynew,label=kind)
#
# plt.legend(loc="upper right")
# plt.show()
