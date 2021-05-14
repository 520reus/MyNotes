"""
描述：基于frenet坐标系的最优轨迹生成
      最优轨迹是在每一计算周期生成的一簇轨迹束中根据“成本函数”最小选出的唯一一条当前时刻满足道路曲率约束、障碍物
      不碰撞约束、最大速度约束、最大加速度约束的最优轨迹，该轨迹的长度由预瞄时间决定，包含位置信息、速度信息、
      航向角信息、曲率信息，即[x,y,yaw,k],也可加入时间戳信息time_stamp,这样即为完整的轨迹规划，而非路径规划
  注：每一个计算周期虽然计算出的是预瞄时间内的最优路径，但每一规划周期只选取第二个点（第一个点是上一次规划的位置点
      ，因为要满足位置和导数连续嘛）作为规划点，然后下一规划周期重新计算预瞄时间内的最优路径，有点滚动优化的感觉
步骤：1. 根据离散目标点序列，生成分段平滑的三次样条曲线，作为道路中心参考线
      2. 在frenet坐标系下分解为s方向与d方向，分别建立d方向的五次多项式曲线和s方向的四次多项式曲线
      3. 在d方向对道路宽度进行离散化，根据当前d向的信息[c_d,c_d_d,c_d_dd]（为保证下一时刻曲线位置、一、二阶导数
         连续，故当前时刻d向信息为上一时刻d向信息）,与目标点d向信息[target_d,0,0]（仅考虑在预瞄时间内达到d向位置，
         完成车道保持任务，d向速度与加速度均为0即可）生成d向五次多项式曲线；再插值计算预瞄时间内的离散点序列
         d向信息(0-3阶导)
      4. 在s方向对速度进行离散化，根据当前s向信息[s,s_d,0]与目标点s向信息[target_s_d,0](仅考虑在预瞄时间内达到
         目标速度，目标加速度为0且不考虑目标s位置，这样做的好处是目标s位置不好选择，而四次多项式不仅可以通过初始点
         的[位置、速度、加速度]与目标点的[位置、速度]求取，还可以根据起始点的[位置、速度、加速度]与目标点的
         [速度、加速度]求得曲线形状，这样无需知道目标s位置，对速度离散化即可，也是在s方向选择四次多项式的好处！)
         生成s向四次多项式曲线；再插值计算预瞄时间内的离散点序列s向信息(0-3阶导)
      5. 根据设计的成本函数，计算每条轨迹的cost。
      6. frenet坐标系转换到全局坐标系，计算每条曲线的笛卡尔坐标信息[x,y,yaw,k]。首先根据轨迹束的s信息，计算当前
         轨迹的中心参考点信息，再根据当前轨迹的d向位置，与参考点的[rx,ry,ryaw]信息求解轨迹的全局位置[x,y]
      7. 所有轨迹束过一遍最大速度、加速度以及碰撞检测，返回通过检测的路径
      8. 在符合要求的路径中选择cost最小的那条作为当前规划周期的最优轨迹
      9. 选择最优轨迹的第二个点作为规划点输出，然后将当前规划点的信息作为输入，重新进行下一规划周期的轨迹生成与
         最优化，直到规划到最后一个参考点或达到最大迭代次数停止。
心得：1. 预瞄时间的解算范围选取十分重要，若范围太大或dt太小，计算时间太长，不满足实时性；若范围太小或dt太大，
         可能有无解的情况，这样就需要re-planning,即通过运动学公式推算规划点来进行轨迹重规划
      2. 在d与s方向均为state space sampling，即状态空间离散，而不是control space（控制空间）离散，即通过
         给定期望目标信息，利用高次多项式反解当前点与目标点的轨迹点，其比控制空间离散更有目的性，但是更难实现

"""

import numpy as np
import matplotlib.pyplot as plt
import copy
import math
import sys
import os

# 添加需要的python文件路径
sys.path.append(os.path.dirname(os.path.abspath(__file__))+
            "/../QuinticPolynomialsPlanner")
sys.path.append(os.path.dirname(os.path.abspath(__file__))+
            "/../CubicSpline")

# Quartic_Polynomials用于生成s向的四阶多项式曲线，QuinticPolynomial用于生成d向的五阶多项式曲线
# whz_cubic_spline_planner用于生成道路中心参考线[x,y,yaw,k,s]
try:
    from whz_quartic_polynomials import Quartic_Polynomials
    from quintic_polynomials_planner import QuinticPolynomial
    import whz_cubic_spline_planner
except ImportError:
    raise


# 全局参数组
animation = True
MAX_CURVATURE = 1   # 道路最大曲率[1/m]
MAX_ACCELERATE = 2  # 车辆最大加速度[m/ss]
MAX_JERK = 2        # 车辆最大加加速度[m/sss]
MAX_VELOCITY = 50.0/3.6  # 车辆最大速度[m/s]
ROBOT_RADIUS = 2   # 车身最大半径[m]
HALF_ROUND_WIDTH = 4  # 路宽 = 8[m]
D_SAMPLE = 1  # d向位置离散分辨率
MIN_T = 3.0  # 预瞄时间[s]
MAX_T = 5.0
dt = 0.4  # time stick
TARGET_SPEED = 15/3.6  # 期望速度[m/s]
V_SAMPLE = 5/3.6   # s向速度离散分辨率
NUM_V = 1  # 决定离散化的速度个数

# cost weights
K_J = 0.1
K_T = 0.1
K_D = 1.0
K_LAT = 1.0
K_LON = 1.0

# frenet路径类
class Frenet_Path:

    def __init__(self):
        # frenet
        self.t = []
        self.d = []
        self.d_d = []
        self.d_dd = []
        self.d_ddd = []
        self.s = []
        self.s_d = []
        self.s_dd = []
        self.s_ddd = []
        # global
        self.x = []
        self.y = []
        self.yaw = []
        self.k = []
        self.ds = []
        # cost
        self.cv = 0.0
        self.cd = 0.0
        self.cf = 0.0

# 产生多条frenet坐标系下的轨迹束
def generate_frenet_path(c_d,c_d_d,c_d_dd,s0,c_s_d):
    f_paths = []
    for di in np.arange(-HALF_ROUND_WIDTH,HALF_ROUND_WIDTH+0.001,D_SAMPLE):
        for Ti in np.arange(MIN_T,MAX_T,dt):
            d_path = Frenet_Path()
            lat_qp = QuinticPolynomial(c_d,c_d_d,c_d_dd,di,0,0,Ti) # d(t),五次多项式
            d_path.t = [t for t in np.arange(0,Ti+dt,dt)]
            d_path.d = [lat_qp.calc_point(t) for t in d_path.t]
            d_path.d_d = [lat_qp.calc_first_derivative(t) for t in d_path.t]
            d_path.d_dd = [lat_qp.calc_second_derivative(t) for t in d_path.t]
            d_path.d_ddd = [lat_qp.calc_third_derivative(t) for t in d_path.t]
            for s_d in np.arange(TARGET_SPEED-NUM_V*V_SAMPLE,TARGET_SPEED+NUM_V*V_SAMPLE,V_SAMPLE):
                sd_path = copy.deepcopy(d_path)  # d_path + s_path
                lon_qp = Quartic_Polynomials(s0,c_s_d,0.0,s_d,0.0,Ti)  # s(t),四次多项式
                sd_path.s = [lon_qp.calc_positon(t) for t in sd_path.t]
                sd_path.s_d = [lon_qp.calc_velocity(t) for t in sd_path.t]
                sd_path.s_dd = [lon_qp.calc_accelerate(t) for t in sd_path.t]
                sd_path.s_ddd = [lon_qp.calc_jerk(t) for t in sd_path.t]

                Jd = sum(np.power(sd_path.d_ddd,2))  # square of jerk
                Js = sum(np.power(sd_path.s_ddd,2))  # square of jerk

                # square of diff from target speed
                ds = (TARGET_SPEED-sd_path.s_d[-1])**2
                sd_path.cd = K_J*Jd + K_T*Ti + K_D*sd_path.d[-1]**2
                sd_path.cv = K_J*Js + K_T*Ti + K_D*ds
                sd_path.cf = K_LAT*sd_path.cd + K_LON*sd_path.cv

                f_paths.append(sd_path)

    return f_paths

# frenet坐标系转换到笛卡尔坐标系
def frenet_to_global(fop_list,ref_path):
    for f_path in fop_list:
        for i in range(len(f_path.s)):
            si = f_path.s[i]
            di = f_path.d[i]
            ix,iy = ref_path.calc_position(si)
            if ix is None:
                break
            iyaw = ref_path.calc_yaw(si)
            fx = ix - di*math.sin(iyaw)
            fy = iy + di*math.cos(iyaw)
            f_path.x.append(fx)
            f_path.y.append(fy)


        for i in range(len(f_path.x)-1):
            dx = f_path.x[i+1] - f_path.x[i]
            dy = f_path.y[i+1] - f_path.y[i]
            yaw = math.atan2(dy,dx)
            f_path.yaw.append(yaw)
            f_path.ds.append(math.hypot(dx,dy))
        # 另最后一个点的yaw与上一个点的相同即可，否则最后一个点无法求出yaw呀
        f_path.yaw.append(f_path.yaw[-1])
        f_path.ds.append(f_path.ds[-1])

        for i in range(len(f_path.yaw)-1):
            dyaw = f_path.yaw[i+1] - f_path.yaw[i]
            ds = f_path.ds[i]
            k = dyaw / ds
            f_path.k.append(k)

        f_path.k.append(f_path.k[-1])

    return fop_list



# 采用三次样条曲线生成道路中心参考线
def generate_center_reference_path(wx,wy,ds=0.1):
    ref_path = whz_cubic_spline_planner.SPline2D(wx,wy)
    rx,ry,ryaw,rk,rs = [],[],[],[],[]
    for ds in np.arange(0,ref_path.s[-1],ds):
        x,y = ref_path.calc_position(ds)
        yaw = ref_path.calc_yaw(ds)
        k = ref_path.calc_curvature(ds)
        rx.append(x)
        ry.append(y)
        ryaw.append(np.rad2deg(yaw))
        rk.append(k)
        rs.append(ds)
    return rx,ry,ryaw,rk,rs,ref_path


# 检测碰撞
def check_collision(f_path,ob):
    dis = []
    for x,y in zip(f_path.x,f_path.y):
        for iox,ioy in zip(ob[:,0],ob[:,1]):
            dis.append(math.hypot(x-iox,y-ioy))
    check_d = [d <= ROBOT_RADIUS for d in dis]
    if any(check_d):
        return False
    return True

# 返回通过碰撞检测与满足运动/动力学约束及道路曲率约束的轨迹
def check_paths(fop_list,ob):
    ok_ind = []
    for i,f_path in enumerate(fop_list):
        if any([s_d > MAX_VELOCITY for s_d in f_path.s_d]):
            continue
        elif any([abs(s_dd) > MAX_ACCELERATE for s_dd in f_path.s_dd]):
            continue
        elif any([abs(k) > MAX_CURVATURE for k in f_path.k]):
            continue
        elif not check_collision(f_path,ob):
            continue
        ok_ind.append(i)

    return [fop_list[i] for i in ok_ind]

# 每个规划周期运行一次，规划出一个最优点，然后将该点作为下一规划周期的输入，不断迭代更新最优规划点
def frenet_optimal_trajectory_planner(ref_path,c_d,c_d_d,c_d_dd,s0,s_d,ob):
    fop_paths = generate_frenet_path(c_d,c_d_d,c_d_dd,s0,s_d)
    fop_paths = frenet_to_global(fop_paths,ref_path)
    fop_passed_paths = check_paths(fop_paths,ob)

    f_best = None
    f_cost = float("inf")
    for f_path in fop_passed_paths:
        if f_path.cf < f_cost:
            f_cost = f_path.cf
            f_best = f_path

    return f_best,fop_paths,fop_passed_paths


def main():
    # 预设参数组
    wx = [0.0, 10.0, 20.5, 35.0, 70.5]
    wy = [0.0, -6.0, 5.0, 6.5, 0.0]

    ob = np.array([[20.0, 10.0],  # obstacle lists
                   [30.0, 6.0],
                   [30.0, 8.0],
                   [35.0, 8.0],
                   [50.0, 3.0]
                   ])

    ds = 0.1               # 插值分辨率
    simulation_loop = 500  # 最大迭代次数
    area = 30

    # 生成道路中心参考线
    rx,ry,ryaw,rk,rs,ref_path = generate_center_reference_path(wx,wy,ds)

    # 初始frenet状态
    s0 = 0  # s向初始位置
    s_d = 0.0/3.6  # s向初始速度
    c_d = 2  # d向初始位置
    c_d_d = 0  # d向初始速度
    c_d_dd = 0  # d向初始加速度


    for i in range(simulation_loop):
        optimal_path,f_paths,f_checked_paths = \
                            frenet_optimal_trajectory_planner(ref_path,c_d,c_d_d,c_d_dd,s0,s_d,ob)
        c_d = optimal_path.d[1]
        c_d_d = optimal_path.d_d[1]
        c_d_dd = optimal_path.d_dd[1]
        s0 = optimal_path.s[1]
        s_d = optimal_path.s_d[1]

        if np.hypot(optimal_path.x[1]-rx[-1],optimal_path.y[1]-ry[-1]) <= 1.5:
            print("Goal")
            break

        if animation:
            plt.cla()  # 用于生成动画
            plt.grid("True")
            plt.plot(rx,ry,'-b')  # 道路中心参考线
            plt.plot(ob[:,0],ob[:,1],'xk')  # 障碍物
            # 画出每个规划周期轨迹束
            for la,f_path in enumerate(f_paths):
                if la == 0:
                    plt.plot(f_path.x[1:],f_path.y[1:],'-.k',linewidth=1,label="所有轨迹束")
                else:
                    plt.plot(f_path.x[1:],f_path.y[1:],'-.k',linewidth=1)
            # 画出规划周期可行轨迹束
            for lb,f_checked_path in enumerate(f_checked_paths):
                if lb == 0:
                    plt.plot(f_checked_path.x[1:],f_checked_path.y[1:],'--g',linewidth=1,label="可行轨迹束")
                else:
                    plt.plot(f_checked_path.x[1:],f_checked_path.y[1:],'--g',linewidth=1)
            # 最优轨迹
            plt.plot(optimal_path.x[1:],optimal_path.y[1:],'-or',label="最优轨迹")
            # 当前位置
            plt.plot(optimal_path.x[1],optimal_path.y[1],'vc')
            plt.legend()
            plt.xlabel("X[m]")
            plt.ylabel("Y[m]")
            plt.xlim([optimal_path.x[1]-area,optimal_path.x[1]+area])
            plt.ylim([optimal_path.y[1]-area,optimal_path.y[1]+area])
            plt.title("v[km/h]:"+str(s_d*3.6)[0:4])
            plt.grid(True)
            plt.pause(0.0001)

    print("Finished!")
    if animation:
        plt.grid(True)
        plt.pause(0.0001)
        plt.show()

if __name__ == "__main__":
    main()




