import math
import numpy as np
import matplotlib.pyplot as plt
import random
import time

show_animation = True

class RRT:
    class Node:
        def __init__(self,x,y):
            self.x = x
            self.y = y
            self.path_x = []
            self.path_y = []
            self.parent = None

    def __init__(self, start, goal, obstacle_list, rand_area,
             expand_dis=3.0, goal_sample_rate=5, path_resolution=0.5, max_iter=500):
        '''
        Setting Parameter
        start:Start Position [x,y]
        goal:Goal Position [x,y]
        obstacleList:obstacle Positions [[x,y,size],...]
        randArea:Random Sampling Area [min,max]
        '''
        self.start = self.Node(start[0], start[1])
        self.end = self.Node(goal[0], goal[1])
        self.min_rand = rand_area[0]
        self.max_rand = rand_area[1]
        self.expand_dis = expand_dis
        self.path_resolution = path_resolution
        self.goal_sample_rate = goal_sample_rate
        self.max_iter = max_iter
        self.obstacle_list = obstacle_list
        self.node_list = []

    def planning(self,animation=True):
        self.node_list = [self.start]

        for i in range(self.max_iter):
            rnd_node = self.get_random_node()
            nearest_node_index = self.get_nearest_node_index(self.node_list,rnd_node)
            nearest_node = self.node_list[nearest_node_index]
            new_node = self.steer(nearest_node,rnd_node,self.expand_dis)

            if self.check_collison(new_node,self.obstacle_list):
                self.node_list.append(new_node)

            if animation and i%5 == 0:
                self.draw_to_graph(rnd_node)

            if self.calc_dis_to_final(self.node_list[-1]) <= self.expand_dis:
                final_node = self.steer(self.node_list[-1],self.end,self.expand_dis)
                if self.check_collison(final_node,self.obstacle_list):
                    return self.generate_final_path(len(self.node_list)-1)

            if animation and i%5:
                self.draw_to_graph(rnd_node)
        return None # can not find path

    def steer(self,from_node,to_node,expend_dis):
        new_node = self.Node(from_node.x,from_node.y)
        new_node.path_x = [from_node.x]
        new_node.path_y = [from_node.y]
        
        d,theta = self.calc_distance_and_angle(from_node,to_node)
        if expend_dis > d:
            expend_dis = d
            
        expand_range = math.floor(expend_dis/self.path_resolution)
        for _ in range(expand_range):
            new_node.x += self.path_resolution*math.cos(theta)
            new_node.y += self.path_resolution*math.sin(theta)
            new_node.path_x.append(new_node.x)
            new_node.path_y.append(new_node.y)
            
        d,_ = self.calc_distance_and_angle(new_node,to_node)
        if d < self.path_resolution:
            new_node.path_x.append(to_node.x)
            new_node.path_y.append(to_node.y)
        new_node.parent = from_node
        
        return new_node
    
    def draw_to_graph(self,node=None):
        plt.clf()
        # for stopping simulation with the esc key.
        plt.gcf().canvas.mpl_connect('key_release_event',
                                     lambda event: [exit(0) if event.key == 'escape' else None])

        for ox,oy,size in self.obstacle_list:
            self.plot_circle(ox,oy,size,'-b')

        if node is not None:
            plt.plot(node.x,node.y,"^k")
        for n in self.node_list:
            if n.parent:
                plt.plot(n.path_x,n.path_y,"-g")

        plt.plot(self.start.x,self.start.y,'Dc')
        plt.plot(self.end.x,self.end.y,'Hr')
        plt.axis([-2,15,-2,15])
        plt.axis('equal')
        plt.grid(True)
        plt.pause(0.01)

    def generate_final_path(self,goal_index):
        path = [[self.end.x,self.end.y]]
        path_node = self.node_list[goal_index]
        while path_node.parent is not None:
            path.append([path_node.x,path_node.y])
            path_node = path_node.parent
        path.append([path_node.x,path_node.y])
        return path


    def calc_dis_to_final(self,node):
        dx = node.x-self.end.x
        dy = node.y-self.end.y
        return math.hypot(dx,dy)

    def get_random_node(self):
        if random.randint(0,100) > self.goal_sample_rate:
            rnd = self.Node(random.uniform(self.min_rand,self.max_rand),
                            random.uniform(self.min_rand,self.max_rand))
        else:
            rnd = self.Node(self.end.x,self.end.y)
        return rnd

    @staticmethod
    def calc_distance_and_angle(from_node,to_node):
        dx = to_node.x-from_node.x
        dy = to_node.y-from_node.y
        d = math.hypot(dx,dy)
        angle = np.arctan2(dy,dx)
        return d,angle

    @staticmethod
    def get_nearest_node_index(node_list,rnd_node):
            dis_list = [math.hypot(node.x-rnd_node.x,node.y-rnd_node.y) for node in node_list]
            return dis_list.index(min(dis_list))

    @staticmethod
    def check_collison(node,obs_list):
        if node is None:
            return False
        for ox,oy,size in obs_list:
            dx_list = [ox-x for x in node.path_x]
            dy_list = [oy-y for y in node.path_y]
            d_list = [math.hypot(dx,dy) for dx,dy in zip(dx_list,dy_list)]
            min_dis = min(d_list)
            if min_dis < size:
                return False
        return True

    @staticmethod
    def plot_circle(x,y,size,color='-r'):
        degree = list(range(0,360,5))
        degree.append(0)
        c_x = [x+size*math.cos(np.deg2rad(d)) for d in degree]
        c_y = [y+size*math.sin(np.deg2rad(d)) for d in degree]
        plt.plot(c_x,c_y,color)



def main(gx=5.0,gy=5.0):
    obstacleList = [
        (0, 15, 7),
        (20, 20, 6),
        (20, 10, 4),
        (10, 10, 6),
        (7, 5, 2),
        (9, 5, 8),
        (8, 10, 1)
    ]  # [x, y, radius]
    rrt = RRT(start=[0,0],
              goal=[10,20],
              rand_area=[-5,30],
              obstacle_list=obstacleList,
              expand_dis=5.0,
              goal_sample_rate=5,
              )
    path = rrt.planning(animation=show_animation)

    if path is None:
        print("can not find a path!")
    else:
        print("find path!")

        if show_animation:
            rrt.draw_to_graph()
            plt.plot([x for x,y in path],[y for x,y in path],'-y')
            path.reverse()
            plt.show()
            # print(path)


if __name__ == "__main__":
    start = time.time()
    main()
    end = time.time()
    print("Running time:%0.4s seconds"%(end-start))
