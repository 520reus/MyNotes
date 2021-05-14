"""
双向A*算法
Bidirectional A* grid planning

author: Erwin Lejeune (@spida_rwin)

See Wikipedia article (https://en.wikipedia.org/wiki/Bidirectional_search)

"""

import math

import matplotlib.pyplot as plt
import time
show_animation = True


class BidirectionalAStarPlanner:

    def __init__(self, ox, oy, resolution, rr):
        """
        Initialize grid map for a star planning

        ox: x position list of Obstacles [m]
        oy: y position list of Obstacles [m]
        resolution: grid resolution [m]
        rr: robot radius[m]
        """

        self.min_x, self.min_y = None, None
        self.max_x, self.max_y = None, None
        self.x_width, self.y_width, self.obstacle_map = None, None, None
        self.resolution = resolution
        self.rr = rr
        self.calc_obstacle_map(ox, oy)
        self.motion = self.get_motion_model()

    class Node:
        def __init__(self, x, y, cost, parent_index):
            self.x = x  # index of grid
            self.y = y  # index of grid
            self.cost = cost
            self.parent_index = parent_index

        def __str__(self):
            return str(self.x) + "," + str(self.y) + "," + str(
                self.cost) + "," + str(self.parent_index)

    def planning(self, sx, sy, gx, gy):
        """
        Bidirectional A star path search

        input:
            s_x: start x position [m]
            s_y: start y position [m]
            gx: goal x position [m]
            gy: goal y position [m]

        output:
            rx: x position list of the final path
            ry: y position list of the final path
        """
        start_node = self.Node(self.calc_xy_index(sx,self.min_x),self.calc_xy_index(sy,self.min_y),0.0,-1)
        target_node = self.Node(self.calc_xy_index(gx,self.min_x),self.calc_xy_index(gy,self.min_y),0.0,-1)
        open_setA,open_setB = dict(),dict()
        closed_setA,closed_setB = dict(),dict()
        open_setA[self.calc_grid_index(start_node)] = start_node
        open_setB[self.calc_grid_index(target_node)] = target_node

        current_A = start_node
        current_B = target_node
        meet_point_A,meet_point_B = None,None

        while 1:
            if len(open_setA) == 0:
                print("can not find a path..")
                break
            if len(open_setB) == 0:
                print("can not find a path..")
                break

            c_id_A = min(open_setA,key=lambda o:self.find_total_cost(open_setA,o,current_B))
            current_A = open_setA[c_id_A]

            c_id_B = min(open_setB,key=lambda o:self.find_total_cost(open_setB,o,current_A))
            current_B = open_setB[c_id_B]

            if show_animation:
                plt.plot(self.calc_grid_position(current_A.x, self.min_x),
                         self.calc_grid_position(current_A.y, self.min_y),
                         "xb")
                plt.plot(self.calc_grid_position(current_B.x, self.min_x),
                         self.calc_grid_position(current_B.y, self.min_y),
                         "xr")
                # for stopping simulation with the esc key.
                plt.gcf().canvas.mpl_connect(
                    'key_release_event',
                    lambda event: [exit(0) if event.key == 'escape' else None])
                if len(closed_setA.keys()) % 10 == 0:
                    plt.pause(0.001)

            if current_A.x == current_B.x and current_A.y == current_B.y:
                meet_point_A = current_A
                meet_point_B = current_B
                break

            del open_setA[c_id_A]
            closed_setA[c_id_A] = current_A

            del open_setB[c_id_B]
            closed_setB[c_id_B] = current_B

            for i,_ in enumerate(self.motion):
                nodes = [self.Node(current_A.x+self.motion[i][0],current_A.y+self.motion[i][1],
                                  current_A.cost+self.motion[i][2],c_id_A),
                        self.Node(current_B.x+self.motion[i][0],current_B.y+self.motion[i][1],
                                  current_B.cost+self.motion[i][2],c_id_B)]
                n_ids = [self.calc_grid_index(nodes[0]),self.calc_grid_index(nodes[1])]

                if not self.verify_node(nodes[0]) or n_ids[0] in closed_setA:
                    continue
                if not self.verify_node(nodes[1]) or n_ids[1] in closed_setB:
                    continue

                if n_ids[0] not in open_setA:
                    open_setA[n_ids[0]] = nodes[0]
                else:
                    if open_setA[n_ids[0]].cost > nodes[0].cost:
                        open_setA[n_ids[0]] = nodes[0]

                if n_ids[1] not in open_setB:
                    open_setB[n_ids[1]] = nodes[1]
                else:
                    if open_setB[n_ids[1]].cost > nodes[1].cost:
                        open_setB[n_ids[1]] = nodes[1]

        rx,ry = self.calc_final_bidirectional_path(meet_point_A,meet_point_B,closed_setA,closed_setB)
        return rx,ry

    # takes two sets and two meeting nodes and return the optimal path
    def calc_final_bidirectional_path(self, n1, n2, setA, setB):
        rxA,ryA = self.calc_final_path(n1,setA)
        rxB,ryB = self.calc_final_path(n2,setB)
        rxB.reverse()
        ryB.reverse()
        rx = rxB + rxA
        ry = ryB + ryA
        return rx,ry

    def calc_final_path(self, goal_node, closed_set):
        rx,ry = [self.calc_grid_position(goal_node.x,self.min_x)],\
                [self.calc_grid_position(goal_node.y,self.min_y)]
        parent_index = goal_node.parent_index
        while parent_index != -1:
            node = closed_set[parent_index]
            rx.append(self.calc_grid_position(node.x,self.min_x))
            ry.append(self.calc_grid_position(node.y,self.min_y))
            parent_index = node.parent_index

        return rx,ry


    @staticmethod
    def calc_heuristic(n1, n2):
        w = 1.0  # weight of heuristic
        d = w * math.hypot(n1.x - n2.x, n1.y - n2.y)
        return d

    def find_total_cost(self, open_set, lambda_, n1):
        g = open_set[lambda_].cost
        h = self.calc_heuristic(open_set[lambda_],n1)
        f = g + h
        return f

    def calc_grid_position(self, index, min_position):
        """
        calc grid position

        :param index:
        :param min_position:
        :return:
        """
        pos = index * self.resolution + min_position
        return pos

    def calc_xy_index(self, position, min_pos):
        return round((position - min_pos) / self.resolution)

    def calc_grid_index(self, node):
        return (node.y - self.min_y) * self.x_width + (node.x - self.min_x)

    def verify_node(self, node):
        px = self.calc_grid_position(node.x, self.min_x)
        py = self.calc_grid_position(node.y, self.min_y)

        if px < self.min_x:
            return False
        elif py < self.min_y:
            return False
        elif px >= self.max_x:
            return False
        elif py >= self.max_y:
            return False

        # collision check
        if self.obstacle_map[node.x][node.y]:
            return False

        return True

    def calc_obstacle_map(self, ox, oy):

        self.min_x = round(min(ox))
        self.min_y = round(min(oy))
        self.max_x = round(max(ox))
        self.max_y = round(max(oy))
        print("min_x:", self.min_x)
        print("min_y:", self.min_y)
        print("max_x:", self.max_x)
        print("max_y:", self.max_y)

        self.x_width = round((self.max_x - self.min_x) / self.resolution)
        self.y_width = round((self.max_y - self.min_y) / self.resolution)
        print("x_width:", self.x_width)
        print("y_width:", self.y_width)

        # obstacle map generation
        self.obstacle_map = [[False for _ in range(self.y_width)]
                             for _ in range(self.x_width)]
        for ix in range(self.x_width):
            x = self.calc_grid_position(ix, self.min_x)
            for iy in range(self.y_width):
                y = self.calc_grid_position(iy, self.min_y)
                for iox, ioy in zip(ox, oy):
                    d = math.hypot(iox - x, ioy - y)
                    if d <= self.rr:
                        self.obstacle_map[ix][iy] = True
                        break

    @staticmethod
    def get_motion_model():
        # dx, dy, cost
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
    print(__file__ + " start!!")

    # start and goal position
    sx = 10.0  # [m]
    sy = 10.0  # [m]
    gx = 50.0  # [m]
    gy = 50.0  # [m]
    grid_size = 2.0  # [m]
    robot_radius = 1.0  # [m]

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
        plt.plot(gx, gy, "ob")
        plt.grid(True)
        plt.axis("equal")

    bidir_a_star = BidirectionalAStarPlanner(ox, oy, grid_size, robot_radius)
    rx, ry = bidir_a_star.planning(sx, sy, gx, gy)

    if show_animation:  # pragma: no cover
        plt.plot(rx, ry, "-k")
        plt.pause(.0001)
        plt.show()


if __name__ == '__main__':
    start = time.time()
    main()
    end = time.time()
    print("Running time:%.4s second"%(end-start))
