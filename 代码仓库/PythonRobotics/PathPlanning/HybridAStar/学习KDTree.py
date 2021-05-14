from scipy import spatial
import matplotlib.pyplot as plt
import numpy as np

x, y = np.mgrid[0:5, 0:5]
points = np.c_[x.ravel(), y.ravel()]
tree = spatial.KDTree(points)
tree.query_ball_point([2, 0], 1)

points = np.asarray(points)
plt.plot(points[:,0], points[:,1], '.')
results = tree.query_ball_point(([2, 0]), 1)
for result in results:
    nearby_point = points[result]
    plt.plot(nearby_point[0], nearby_point[1], 'o')
plt.margins(0.1, 0.1)
plt.show()
