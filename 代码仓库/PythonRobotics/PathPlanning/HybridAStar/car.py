"""

Car model for Hybrid A* path planning

author: Zheng Zh (@Zhengzh)

"""

from math import sqrt, cos, sin, tan, pi

import matplotlib.pyplot as plt
import numpy as np
from scipy.spatial.transform import Rotation as Rot

# 车辆以后轴中心为参考点！
WB = 3.  # rear to front wheel
W = 2.  # width of car
LF = 3.3  # distance from rear to vehicle front end
LB = 1.0  # distance from rear to vehicle back end
MAX_STEER = 0.6  # [rad] maximum steering angle

W_BUBBLE_DIST = (LF - LB) / 2.0
# 车辆轮廓最大的半径
W_BUBBLE_R = sqrt(((LF + LB) / 2.0) ** 2 + (W/2)**2)


# vehicle rectangle vertices 车辆矩形轮廓点，为了画出一个闭合的轮廓，因此需要5个点
VRX = [LF, LF, -LB, -LB, LF]
VRY = [W / 2, -W / 2, -W / 2, W / 2, W / 2]


def check_car_collision(x_list, y_list, yaw_list, ox, oy, kd_tree):
    # 遍历节点积分出的新路径点是否会发生碰撞
    for i_x, i_y, i_yaw in zip(x_list, y_list, yaw_list):
        # 计算车辆的后轴中心
        cx = i_x + W_BUBBLE_DIST * cos(i_yaw)
        cy = i_y + W_BUBBLE_DIST * sin(i_yaw)
        # 以车辆轮廓最大半径W_BUBBLE_R为半径，车辆后轴中心为圆心，计算圆内有无障碍物点，返回障碍物点的位置索引
        ids = kd_tree.query_ball_point([cx, cy], W_BUBBLE_R)

        if not ids:
            continue

        if not rectangle_check(i_x, i_y, i_yaw,
                               [ox[i] for i in ids], [oy[i] for i in ids]):
            return False  # collision

    return True  # no collision


def rectangle_check(x, y, yaw, ox, oy):
    # transform obstacles to base link frame
    # 旋转逆变换回到坐标原点的状态，再判断障碍物点是否在“平躺”的矩形框内就容易多了，否则还要判断点是否在旋转后的矩形框中，非常麻烦
    rot = Rot.from_euler('z', yaw).as_matrix()[0:2, 0:2]
    for iox, ioy in zip(ox, oy):
        tx = iox - x
        ty = ioy - y
        converted_xy = np.stack([tx, ty]).T @ rot
        rx, ry = converted_xy[0], converted_xy[1]

        if not (rx > LF or rx < -LB or ry > W / 2.0 or ry < -W / 2.0):
            return False  # collision

    return True  # no collision


def plot_arrow(x, y, yaw, length=1.0, width=0.5, fc="r", ec="k"):
    """Plot arrow."""
    # isinstance 判断类型
    if not isinstance(x, float):
        for (i_x, i_y, i_yaw) in zip(x, y, yaw):
            plot_arrow(i_x, i_y, i_yaw)
    else:
        plt.arrow(x, y, length * cos(yaw), length * sin(yaw),
                  fc=fc, ec=ec, head_width=width, head_length=width, alpha=0.4)


def plot_car(x, y, yaw):
    car_color = '-k'
    c, s = cos(yaw), sin(yaw)
    # 绕z轴旋转，-yaw表示逆时针转，yaw表示顺时针转
    rot = Rot.from_euler('z', -yaw).as_matrix()[0:2, 0:2]
    car_outline_x, car_outline_y = [], []
    # 先对5个矩形轮廓点进行一个基于偏航角yaw的旋转变换，再加上车辆的位置点[x,y]，即可画出整车当前位姿的轮廓
    for rx, ry in zip(VRX, VRY):
        converted_xy = np.array([rx, ry]).T @ rot
        car_outline_x.append(converted_xy[0]+x)
        car_outline_y.append(converted_xy[1]+y)

    arrow_x, arrow_y, arrow_yaw = c * 1.5 + x, s * 1.5 + y, yaw
    plot_arrow(arrow_x, arrow_y, arrow_yaw)

    plt.plot(car_outline_x, car_outline_y, car_color)


# 将角度转化到[-pi,pi)
def pi_2_pi(angle):
    return (angle + pi) % (2 * pi) - pi


def move(x, y, yaw, distance, steer, L=WB):
    x += distance * cos(yaw)
    y += distance * sin(yaw)
    yaw += pi_2_pi(distance * tan(steer) / L)  # distance/2

    return x, y, yaw


def main():
    x, y, yaw = 0., 0.,0.5
    plt.axis('equal')
    plot_car(x, y, yaw)
    plt.show()


if __name__ == '__main__':
    main()

