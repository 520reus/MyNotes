import matplotlib.pyplot as plt
import numpy as np

x = np.arange(-10,10,0.1)
y1 = 1/2*x-1
y2 = -2*x+3
y3 = 3*x-5
y4 = -1/3*x+2

plt.plot(x,y1,x,y2,x,y3,x,y4)
plt.grid('True')
plt.xlim([0,8])
plt.ylim([-10,5.1])


