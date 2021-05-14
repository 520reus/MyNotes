
"""
轨迹生成方法：四次多项式插值 f(t) = a0+a1*t+a2*t^2+a3*t^3+a4*t^4
输入：起始路点：[xs,ys,yaws,vs,as],目标路点：[xe,ye,yawe,ve,ae]
输出：插值平滑后的密集离散点序列

四次多项式共有5个参数，给定初始点[位置、速度、加速度]和目标点[位置、速度]（一）或目标点[速度、加速度]（二）与
该段的时间间隔T，可以求得全部5个参数
f(0) = a0 = ps             f(T) = a0+a1*T+a2*T^2+a3*T^3+a4*T^4 = pe
f_d(0) = a1 = vs           f_d(T) = a1+2*a2*T+3*a3*T^2+4*a4*T^3 = ve
f_dd(0) = 2*a2 = acs       f_dd(T) = 2*a2 + 6*a3+12*a4*T^2 =ace
将整体decouple为横向与纵向，分别得到横向与纵向的四次多项式，再插值计算各阶导数，最后合成为位置、速度、加速度，
jerk，在解算时间内寻找满足曲线整体满足最大加速度与最大jerk的四次多项式
"""
import numpy as np
import math
import matplotlib.pyplot as plt

MIN_T = 5
MAX_T = 100
animation = True

class Quartic_Polynomials:
    # 版本一：给定初始点[位置、速度、加速度]和目标点[位置、速度],求解系数。保证若有解则最后一定移动到目标点位置
    # def __init__(self,ps,vs,acs,pe,ve,T):
    #     self.a0 = ps
    #     self.a1 = vs
    #     self.a2 = acs/2
    #     A = np.array([[T**3,T**4],[3*T**2,4*T**3]])
    #     B = np.array([[pe-self.a0-self.a1*T],[ve-self.a1-2*self.a2*T]])
    #     res = np.linalg.solve(A,B)
    #     self.a3 = res[0]
    #     self.a4 = res[1]

    # 版本二：给定初始点[位置、速度、加速度]和目标点[速度、加速度],求解系数。保证若有解则最后一定达到目标点速度
    def __init__(self,ps,vs,acs,ve,ace,T):
        self.a0 = ps
        self.a1 = vs
        self.a2 = acs/2
        A = np.array([[3 * T ** 2, 4 * T ** 3],
                       [6 * T, 12 * T ** 2]])
        B = np.array([ve - self.a1 - 2 * self.a2 * T,
                       ace - 2 * self.a2])
        res = np.linalg.solve(A,B)
        self.a3 = res[0]
        self.a4 = res[1]

    def calc_positon(self,t):
        return self.a0 + self.a1*t + self.a2*t**2 + self.a3*t**3 + self.a4*t**4

    def calc_velocity(self,t):
        return self.a1 + 2*self.a2*t + 3*self.a3*t**2 + 4*self.a4*t**3

    def calc_accelerate(self,t):
        return 2*self.a2 + 6*self.a3*t + 12*self.a4*t**2

    def calc_jerk(self,t):
        return 6*self.a3+24*self.a4*t

    def calc_snap(self,t):
        return 24*self.a4


def Quartic_Polynomials_Planner(start,target,max_jark,max_acc,dt):
    xs = start[0]
    ys = start[1]
    yaws = start[2]
    vs = start[3]
    acs = start[4]

    xe = target[0]
    ye = target[1]
    yawe = target[2]
    ve = target[3]
    ace = target[4]

    vxs = vs*math.cos(yaws)
    vys = vs*math.sin(yaws)
    axs = acs*math.cos(yaws)
    ays = acs*math.sin(yaws)

    vxe = ve*math.cos(yawe)
    vye = ve*math.sin(yawe)
    axe = ace*math.cos(yawe)
    aye = ace*math.sin(yawe)

    flag = False
    for T in np.arange(MIN_T,MAX_T,MIN_T):
        # 版本一
        # xqp = Quartic_Polynomials(xs,vxs,axs,xe,vxe,T)
        # yqp = Quartic_Polynomials(ys,vys,ays,ye,vye,T)
        # 版本二
        xqp = Quartic_Polynomials(xs,vxs,axs,vxe,axe,T)
        yqp = Quartic_Polynomials(ys,vys,ays,vye,aye,T)

        time,rx,ry,rv,ra,rj,ryaw,rsnap = [],[],[],[],[],[],[],[]
        for t in np.arange(0,T+dt,dt):
            time.append(t)
            x = xqp.calc_positon(t)
            y = yqp.calc_positon(t)
            rx.append(x)
            ry.append(y)

            vx = xqp.calc_velocity(t)
            vy = yqp.calc_velocity(t)
            v = math.hypot(vx,vy)
            yaw = math.atan2(vy,vx)
            rv.append(v)
            ryaw.append(yaw)

            ax = xqp.calc_accelerate(t)
            ay = yqp.calc_accelerate(t)
            a = math.hypot(ax,ay)
            if len(rv) >= 2 and rv[-1] - rv[-2] < 0:
                a *= -1
            ra.append(a)

            jx = xqp.calc_jerk(t)
            jy = yqp.calc_jerk(t)
            j = math.hypot(jx,jy)
            if len(ra) >= 2 and ra[-1] - ra[-2] < 0:
                j *= -1
            rj.append(j)

            snap_x = xqp.calc_snap(t)
            snap_y = yqp.calc_snap(t)
            snap = math.hypot(snap_x,snap_y)
            if len(rj) >= 2 and rj[-1] - rj[-2] < 0:
                snap *= -1
            rsnap.append(snap)

        if max([abs(a) for a in ra]) <= max_acc and max([abs(j) for j in rj]) <= max_jark:
            print("find path!")
            flag = True
            break

    if not flag:
        print("Can not find a path within time limit!")

    if animation and flag:
        for i,_ in enumerate(time):
            plt.cla()
            # for stopping simulation with the esc key.
            plt.gcf().canvas.mpl_connect('key_release_event',
                                             lambda event: [exit(0) if event.key == 'escape' else None])
            plt.grid("True")
            plt.axis("equal")
            plot_arrow(xs,ys,yaws)
            plot_arrow(xe,ye,yawe)
            plot_arrow(float(rx[i]),ry[i],ryaw[i])
            plt.title("Times[s]:"+str(time[i])[0:4]+
                      "velocity[m/s]:"+str(rv[i])[0:4]+
                      "accelerate[m/ss]"+str(ra[i])[0:4]+
                      "jerk[m/sss]"+str(rj[i])[0:4])
            plt.pause(0.001)

    return time,rx,ry,rv,ra,rj,ryaw,rsnap,flag


def plot_arrow(x, y, yaw, length=1.0, width=0.5, fc="r", ec="k"):  # pragma: no cover
    """
    Plot arrow
    """

    if not isinstance(x, float):
        for (ix, iy, iyaw) in zip(x, y, yaw):
            plot_arrow(ix, iy, iyaw)
    else:
        plt.arrow(x, y, length * math.cos(yaw), length * math.sin(yaw),
                  fc=fc, ec=ec, head_width=width, head_length=width)
        plt.plot(x, y)


def main():
    start = [10.0,10.0,np.deg2rad(10),5.0,0]  # [position,yaw,velocity,accelerate]
    target = [20.0,20.0,np.deg2rad(90),5.0,0]
    dt = 0.1
    max_jark = 3.5
    max_acc = 5
    time,rx,ry,rv,ra,rj,ryaw,rsnap,flag = Quartic_Polynomials_Planner(start,target,max_jark,max_acc,dt)
    print("time:",time[-1])
    print("end_a:",ra[-1])
    # 绘图
    if flag and animation:
        plt.plot(rx,ry,'-r')
        plt.xlabel("X[m]")
        plt.ylabel("Y[m]")
        plt.grid("True")

        plt.subplots()
        plt.plot(time,[np.rad2deg(i) for i in ryaw],'-r')
        plt.xlabel("Times[s]")
        plt.ylabel("Yaw[deg]")
        plt.grid("True")

        plt.subplots()
        plt.plot(time,rv,'-r')
        plt.xlabel("Times[s]")
        plt.ylabel("velocity[m/s]")
        plt.grid("True")

        plt.subplots()
        plt.plot(time,ra,'-r')
        plt.xlabel("Times[s]")
        plt.ylabel("accelerate[m/ss]")
        plt.grid("True")

        plt.subplots()
        plt.plot(time,rj,'-r')
        plt.xlabel("Times[s]")
        plt.ylabel("jerk[m/sss]")
        plt.grid("True")

        plt.subplots()
        plt.plot(time,rsnap,'-r')
        plt.xlabel("Times[s]")
        plt.ylabel("snap[m/ssss]")
        plt.grid("True")

        plt.show()


if __name__ == "__main__":
    main()
