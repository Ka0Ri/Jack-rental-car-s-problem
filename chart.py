import numpy as np 
import matplotlib.pyplot as plt
import seaborn as sns
import os
from mpl_toolkits.mplot3d import Axes3D

path = os.getcwd() + "/result/result10.txt"

f = open(path, "r")
value = np.zeros((21, 21))
action = np.zeros((21, 21))
i = 0
f.readline()
for line in f:
    s = line.split('\t')
    j = 0
    for tup in s[1:22]:
        value[i, j] = tup.split(',')[0]
        action[i, j] = tup.split(',')[1]
        j = j + 1
    i = i + 1

ax = sns.heatmap(action, linewidth=0.5)
ax.set_title("Iteration = 10")
ax.set_xlabel("cars at first location")
ax.set_ylabel("cars at second location")
plt.show()

hf = plt.figure()
ha = hf.add_subplot(111, projection='3d')
X, Y = np.meshgrid(range(21), range(21))
ha.plot_surface(X, Y, value)
ha.set_xlabel("cars at first location")
ha.set_ylabel("cars at second location")
ha.set_zlabel("value")
ha.set_title("Iteration = 10")
plt.show()
f.close()