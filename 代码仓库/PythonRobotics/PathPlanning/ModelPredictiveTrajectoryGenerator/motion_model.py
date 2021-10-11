'''
1.定义state类型[x,y,yaw,v]，位置，航向，和速度。涉及到state lattice速度模型的部分，使用的是匀速模型
2.update(state, v, delta, dt, L):在dt时间内，根据运动模型更新状态数据
3.generate_trajectory(s,km,kp,k0)：s是运动的弧长，由于是匀速模型，可以计算Δt；另外涉及到state lattice中的角速度模型部分，
使用二次曲线，参数为二次曲线上的三个点[k0,km,kf]，将这三个点和时间拟合得到曲线后；向前积分得到整个轨迹上的状态点。
4.generate_last_state(s,km,kp,k0)：同generate_trajectory，但只返回整个轨迹上的最后一个状态点
'''

import math
import numpy as np
import scipy.interpolate

# motion parameter
L = 1.0  # wheel base
ds = 0.1  # course distanse
v = 10.0 / 3.6  # velocity [m/s]


class State:

    def __init__(self, x=0.0, y=0.0, yaw=0.0, v=0.0):
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v


def pi_2_pi(angle):
    return (angle + math.pi) % (2 * math.pi) - math.pi


def update(state, v, curvature, dt, L):
    state.v = v
    state.x = state.x + state.v * math.cos(state.yaw) * dt
    state.y = state.y + state.v * math.sin(state.yaw) * dt
    #state.yaw = state.yaw + state.v / L * math.tan(delta) * dt
    state.yaw = state.yaw + state.v * curvature * dt
    state.yaw = pi_2_pi(state.yaw)

    return state


def generate_trajectory(s, km, kf, k0):
    """
    根据参数p[s,km,kf]向前积分得到轨迹
    Parameters
    ----------
    s 弧长
    km 经过t/2时的曲率
    kf  末尾曲率
    k0 初始曲率

    Returns
    -------
	轨迹上所有状态点
    """
    # 每ds生成一个状态数据
    n = s / ds
    # 匀速模型，经弧长s需要的时间
    time = s / v  # [s]

    if isinstance(time, type(np.array([]))): time = time[0]
    if isinstance(km, type(np.array([]))): km = km[0]
    if isinstance(kf, type(np.array([]))): kf = kf[0]

    # 曲率函数中作为自变量的三个时间样本
    tk = np.array([0.0, time / 2.0, time])
    # 曲率函数中作为因变量的三个曲率样本
    kk = np.array([k0, km, kf])
    # 轨迹中所有的时间点
    t = np.arange(0.0, time, time / n)
    # 根据三个样本点拟合得到二次项曲线，即 曲率= fkp(时间)，fkp是关于t的二次项函数，返回值是个函数，可以通过传入有关时间的参数得到曲率值
    fkp = scipy.interpolate.interp1d(tk, kk, kind="quadratic")
    # 得到所有时间点处的曲率
    kp = [fkp(ti) for ti in t]
    dt = float(time / n)

    #  plt.plot(t, kp)
    #  plt.show()

    # 轨迹中添加初始点
    state = State()
    x, y, yaw = [state.x], [state.y], [state.yaw]

    # 根据速度，曲率向前积分得到轨迹上的所有点
    for ikp in kp:
        state = update(state, v, ikp, dt, L)
        x.append(state.x)
        y.append(state.y)
        yaw.append(state.yaw)

    return x, y, yaw


def generate_last_state(s, km, kf, k0):
    """
	与generate_trajectory大致相同，区别在于generate_trajectory得到所有轨迹点，这里只要最后一个轨迹点
    """
    n = s / ds
    time = s / v  # [s]

    if isinstance(time, type(np.array([]))): time = time[0]
    if isinstance(km, type(np.array([]))): km = km[0]
    if isinstance(kf, type(np.array([]))): kf = kf[0]

    tk = np.array([0.0, time / 2.0, time])
    kk = np.array([k0, km, kf])
    t = np.arange(0.0, time, time / n)
    fkp = scipy.interpolate.interp1d(tk, kk, kind="quadratic")
    kp = [fkp(ti) for ti in t]
    dt = time / n

    #  plt.plot(t, kp)
    #  plt.show()

    state = State()

    _ = [update(state, v, ikp, dt, L) for ikp in kp]

    return state.x, state.y, state.yaw
