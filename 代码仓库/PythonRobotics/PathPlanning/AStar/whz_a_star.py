import matplotlib.pyplot as plt
import math
import time
show_animation = True


class AStarPlanner(object):

    # 定义节点数据结构
    class Node:
        def __init__(self,x,y,cost,parent_index):
            self.x = x
            self.y = y
            self.cost = cost
            self.parent_node = parent_index

        def __str__(self):
            return str(self.x)+','+str(self.y)+','+str(self.cost)+','+str(self.parent_node)

    def __init__(self, ox, oy, resolution, rr,w1,w2):
        """
        Initialize grid map for a star planning

        ox: x position list of Obstacles [m]
        oy: y position list of Obstacles [m]
        resolution: grid resolution [m]
        rr: robot radius[m]
        weight:f(n)=w1*g(n)+w2*h(n),w2=0时A*退化为Dijkstra
        """

        self.resolution = resolution
        self.rr = rr
        self.min_x, self.min_y = 0, 0
        self.max_x, self.max_y = 0, 0
        self.obstacle_map = None
        self.x_width, self.y_width = 0, 0
        self.motion = self.get_motion_model()
        self.calc_obs(ox, oy)
        self.w1 = w1
        self.w2 = w2

    def planner(self, sx, sy, gx, gy):
        # 先将起始节点与目标节点的父节点都设为-1，找到最短路径再把最后的节点赋给目标节点作为父节点
        start_node = self.Node(self.calc_xy_index(sx),self.calc_xy_index(sy),0.0,-1)
        target_node = self.Node(self.calc_xy_index(gx),self.calc_xy_index(gy),0.0,-1)
        open_list,closed_list = dict(),dict()
        open_list[self.dict_label(start_node)] = start_node
        while 1:
            # 对min加key，按lambda表达式的值最小，返回字典中的最小值对应的键！
            c_id = min(open_list,key=lambda o:self.calc_heuristic(open_list[o],target_node))
            current = open_list[c_id]

            if show_animation:
                plt.plot(self.calc_grid_position(open_list[c_id].x,self.min_x),
                         self.calc_grid_position(open_list[c_id].y,self.min_y),"xb")
                # for stopping simulation with the esc key.
                plt.gcf().canvas.mpl_connect('key_release_event',
                                             lambda event: [exit(
                                                 0) if event.key == 'escape' else None])
                if len(closed_list.keys()) % 10 == 0:
                    plt.pause(0.01)

            if target_node.x == current.x and target_node.y == current.y:
                print("Find goal!")
                target_node.parent_node = current.parent_node
                target_node.cost = current.cost
                break

            del open_list[c_id]

            closed_list[c_id] = current

            for i,_ in enumerate(self.motion):
                # 记录了每个节点的当前损失值与父节点！
                node = self.Node(self.motion[i][0]+current.x,self.motion[i][1]+current.y,
                                 self.motion[i][2]+current.cost,c_id)
                n_id = self.dict_label(node)

                if n_id in closed_list:
                    continue
                if not self.verify_node(node):
                    continue
                if n_id not in open_list:
                    open_list[n_id] = node
                else:
                    if open_list[n_id].cost > node.cost:
                        open_list[n_id] = node

        rx,ry = self.calc_final_path(target_node,closed_list)
        return rx,ry

    def calc_final_path(self,target_node,closed_list):
        rx = [self.calc_grid_position(target_node.x,self.min_x)]
        ry = [self.calc_grid_position(target_node.y,self.min_y)]
        parent_index = target_node.parent_node
        while parent_index != -1:
            parent_node = closed_list[parent_index]
            rx.append(self.calc_grid_position(parent_node.x,self.min_x))
            ry.append(self.calc_grid_position(parent_node.y,self.min_y))
            parent_index = parent_node.parent_node
        return rx,ry


    def verify_node(self,node):
        px = self.calc_grid_position(node.x,self.min_x)
        py = self.calc_grid_position(node.y,self.min_y)
        if px <= self.min_x or px >= self.max_x:
            return False
        if py <= self.min_y or py >= self.max_y:
            return False
        if self.obstacle_map[node.x][node.y]:
            return False

        return True

    def calc_heuristic(self,n, target_node):
        g = n.cost
        # L2范数，欧式距离，缺点：It don't estimate the shortest distance well and have a gap to h*(n)
        h = math.hypot(n.x-target_node.x,n.y-target_node.y)
        # 对角heuristic, 优点：It has the closed-form solution!
        # dx = abs(n.x-target_node.x)
        # dy = abs(n.y-target_node.y)
        # h =dx+dy+(math.sqrt(2)-2)*min(dx,dy)
        f = self.w1*g + self.w2*h
        return f

    def dict_label(self,node):
        return (node.y - self.min_y) * self.x_width + (node.x - self.min_x)

    def calc_xy_index(self,x):
        return round((x-self.min_x)/self.resolution)

    def calc_grid_position(self,index,min_g):
        return self.resolution*index+min_g

    # 检测碰撞
    def calc_obs(self,ox,oy):
        self.min_x = round(min(ox))
        self.min_y = round(min(oy))
        self.max_x = round(max(ox))
        self.max_y = round(max(oy))
        self.x_width = round((self.max_x-self.min_x)/self.resolution)
        self.y_width = round((self.max_y-self.min_y)/self.resolution)
        obs_map = [[False for _ in range(self.x_width)]for _ in range(self.y_width)]
        for a in range(self.x_width):
            x = self.calc_grid_position(a,self.min_x)
            for b in range(self.y_width):
                y = self.calc_grid_position(b,self.min_y)
                for iox,ioy in zip(ox,oy):
                    dis_to_obs = math.hypot(x-iox,y-ioy)
                    if dis_to_obs < self.rr:
                        obs_map[a][b] = True
        self.obstacle_map = obs_map

    @staticmethod
    def get_motion_model():
        motion = [[1, 0, 1],
                  [0, 1, 1],
                  [-1, 0, 1],
                  [0, -1, 1],
                  [-1, -1, math.sqrt(2)],
                  [-1, 1, math.sqrt(2)],
                  [1, -1, math.sqrt(2)],
                  [1, 1, math.sqrt(2)]]
        return motion

def main():
    # start and goal position
    sx = 10.0  # [m]
    sy = 10.0  # [m]
    gx = 50.0  # [m]
    gy = 50.0  # [m]
    grid_size = 2.0  # [m]   机器人移动距离的最小单位
    robot_radius = 1.0  # [m]  机器人的半径，用于碰撞检测
    w1 = 1.0
    w2 = 1.0

    # set obstacle positions
    ox, oy = [], []
    for i in range(-10, 60):
        ox.append(i)
        oy.append(-10.0)
    for i in range(-10, 60):
        ox.append(60.0)
        oy.append(i)
    for i in range(-10, 61):
        ox.append(i)
        oy.append(60.0)
    for i in range(-10, 61):
        ox.append(-10.0)
        oy.append(i)
    for i in range(-10, 40):
        ox.append(20.0)
        oy.append(i)
    for i in range(0, 40):
        ox.append(40.0)
        oy.append(60.0 - i)

    if show_animation:  # pragma: no cover
        plt.plot(ox, oy, ".k")
        plt.plot(sx, sy, "og")
        plt.plot(gx, gy, "xy")
        plt.grid(True)
        plt.axis("equal")

    a_star = AStarPlanner(ox,oy,grid_size,robot_radius,w1,w2)
    rx,ry = a_star.planner(sx,sy,gx,gy)

    if show_animation:
        plt.plot(rx,ry,'-r')
        plt.pause(0.001)
        plt.show()


if __name__ == "__main__":
    start = time.time()
    main()
    end = time.time()
    print("Running time:%0.4s seconds"%(end-start))
