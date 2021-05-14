"""
分段三次样条曲线轨迹生成
离散目标点序列将区间[a,b]分为n个区间，n+1个点，以车辆纵向前进方向为s轴，分别建立x,y关于s的三次多项式，
将运动二维分解后，通过n-1个内部点的前后位置相等、一二阶导数连续共4n-2个等式约束，再加上自然边界，
即2个端点二阶导数为0，共4n个方程，可解出n条三次样条曲线的全部4n个参数。再插值计算各阶导数，最后合成为位置、
速度、加速度，并合成求解插值点的[x,y,yaw,k]

心得：1. 由于输入给定为n+1个航迹点way point，共有n个三次多项式，仅考虑多项式连接处的位置连续与一二阶导数连续，
        并没有最优化连接处的jerk或者snap。故不属于minimum snap or minimum accelerate

      2. 对于多项式，n阶导光滑，对应n+1阶导连续，故对于三次多项式，其零、一阶导光滑，二阶导连续，三阶导为分段定值

      3. 次数             光滑              连续          分段定值
          3          零、一、阶导          二阶导          三阶导
          4          零、一、二阶导        三阶导          四阶导
          5          零、一、二、三阶导    四阶导          五阶导
          故k次多项式，k-1阶导连续，k-1阶导往前都光滑

      4.   零阶导：位置 一阶导：速度 二阶导：加速度 三阶导：jerk 四阶导：snap

      5. n个多项式的系数不同，若想求取全局s位置对应的哪一个多项式的导数，需要先找到该多项式的起始位置，即通过
         bisect库找到排序后序列的插入位置，然后再计算对应的导数

"""

import math
import numpy as np
import bisect
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']  # 显示中文标签
plt.rcParams['axes.unicode_minus'] = False   # 这两行需要手动设置

animation = True


class Spline:
    """
    Cubic Spline class
    """

    def __init__(self, x, y):
        self.b, self.c, self.d, self.w = [], [], [], []

        self.x = x
        self.y = y

        self.nx = len(x)  # dimension of x
        h = np.diff(x)

        # calc coefficient c
        self.a = [iy for iy in y]

        # calc coefficient c
        A = self.__calc_A(h)
        B = self.__calc_B(h)
        self.c = np.linalg.solve(A, B)
        #  print(self.c1)

        # calc spline coefficient b and d
        for i in range(self.nx - 1):
            self.d.append((self.c[i + 1] - self.c[i]) / (3.0 * h[i]))
            tb = (self.a[i + 1] - self.a[i]) / h[i] - h[i] * \
                (self.c[i + 1] + 2.0 * self.c[i]) / 3.0
            self.b.append(tb)

    def calc(self, t):
        """
        Calc position

        if t is outside of the input x, return None

        """

        if t < self.x[0]:
            return None
        elif t > self.x[-1]:
            return None

        i = self.__search_index(t)
        dx = t - self.x[i]
        result = self.a[i] + self.b[i] * dx + \
            self.c[i] * dx ** 2.0 + self.d[i] * dx ** 3.0

        return result

    def calcd(self, t):
        """
        Calc first derivative

        if t is outside of the input x, return None
        """

        if t < self.x[0]:
            return None
        elif t > self.x[-1]:
            return None

        i = self.__search_index(t)
        dx = t - self.x[i]
        result = self.b[i] + 2.0 * self.c[i] * dx + 3.0 * self.d[i] * dx ** 2.0
        return result

    def calcdd(self, t):
        """
        Calc second derivative
        """

        if t < self.x[0]:
            return None
        elif t > self.x[-1]:
            return None

        i = self.__search_index(t)
        dx = t - self.x[i]
        result = 2.0 * self.c[i] + 6.0 * self.d[i] * dx
        return result

    def __search_index(self, x):
        """
        search data segment index
        """
        return bisect.bisect(self.x, x) - 1

    def __calc_A(self, h):
        """
        calc matrix A for spline coefficient c
        """
        A = np.zeros((self.nx, self.nx))
        A[0, 0] = 1.0
        for i in range(self.nx - 1):
            if i != (self.nx - 2):
                A[i + 1, i + 1] = 2.0 * (h[i] + h[i + 1])
            A[i + 1, i] = h[i]
            A[i, i + 1] = h[i]

        A[0, 1] = 0.0
        A[self.nx - 1, self.nx - 2] = 0.0
        A[self.nx - 1, self.nx - 1] = 1.0
        #  print(A)
        return A

    def __calc_B(self, h):
        """
        calc matrix B for spline coefficient c
        """
        B = np.zeros(self.nx)
        for i in range(self.nx - 2):
            B[i + 1] = 3.0 * (self.a[i + 2] - self.a[i + 1]) / \
                h[i + 1] - 3.0 * (self.a[i + 1] - self.a[i]) / h[i]
        return B


class SPline2D:
    '''
    二维插值
    '''
    def __init__(self,x,y):
        self.s = self.__calc_s(x,y)
        self.sx = Spline(self.s,x)
        self.sy = Spline(self.s,y)


    def __calc_s(self,x,y):
        dx = np.diff(x)
        dy = np.diff(y)
        ds = np.hypot(dx,dy)
        s = [0]
        s.extend(ds)
        return np.cumsum(s)


    def calc_position(self,s):
        x = self.sx.calc(s)
        y = self.sy.calc(s)
        return x,y

    def calc_yaw(self,s):
        dx = self.sx.calcd(s)
        dy = self.sy.calcd(s)
        return math.atan2(dy,dx)

    def calc_curvature(self,s):
        x_d = self.sx.calcd(s)
        x_dd = self.sx.calcdd(s)
        y_d = self.sy.calcd(s)
        y_dd = self.sy.calcdd(s)
        k = abs(x_d*y_dd-x_dd*y_d)/math.pow(math.hypot(x_d,y_d),3)
        return k

def generate_cubic_spline_path(x,y,ds=0.1):
    sp = SPline2D(x,y)
    rx,ry,ryaw,rk,rs = [],[],[],[],[]
    for s in np.arange(0,sp.s[-1],ds):
        x,y = sp.calc_position(s)
        yaw = sp.calc_yaw(s)
        k = sp.calc_curvature(s)
        rx.append(x)
        ry.append(y)
        ryaw.append(np.rad2deg(yaw))
        rk.append(k)
        rs.append(s)
    return rx,ry,ryaw,rk,rs


def main():
    x = [0.0, 10.0, 20.5, 35.0, 70.5]
    y = [0.0, -6.0, 5.0, 6.5, 0.0]
    # x = [0.0, 50.0,52.0,55.0]
    # y = [0.0, 0.0,2.0,10.0]
    rx,ry,yaw,k,rs = generate_cubic_spline_path(x,y)

    if animation:
        plt.plot(x,y,'xb')
        plt.plot(rx,ry,'-r')
        plt.grid(True)
        plt.xlabel('X[m]')
        plt.ylabel('Y[m]')
        plt.title("三次样条分段曲线轨迹生成")

        plt.subplots()
        plt.plot(rs,yaw,'-r')
        plt.grid(True)
        plt.xlabel('s[m]')
        plt.ylabel('yaw[deg]')

        plt.subplots()
        plt.plot(rs,k,'-r')
        plt.grid(True)
        plt.xlabel('s[m]')
        plt.ylabel('curvature[m-1]')

        plt.show()


if __name__ == "__main__":
    main()
