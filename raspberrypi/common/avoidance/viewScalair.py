import common.geogebra as geogebra
from common.avoidance.obstacles import *
from common.avoidance.funct import *

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np

# Extract geogebra data
geo = geogebra.Geogebra("test.ggb")
origin = geo.get("origin")
goal = geo.get("goal")[0], geo.get("goal")[1]
polys = geo.getall("obsin*")
maps = geo.get("MAP")


fig = plt.figure()
ax = fig.gca(projection='3d')

# Make data.
X = np.arange(0, 2000, 10, dtype =np.float32)
Y = np.arange(0, 3000, 10, dtype =np.float32)
X, Y = np.meshgrid(X, Y)
Z = np.zeros_like(X, dtype=np.float32)
# Create obj
objs = list()

robot_width = 300
max_force = 10

objs.append(Map(2000, 3000, funct_list["lin"](a=max_force/robot_width, b=max_force)))
#objs.append(DiskObs(Point(1000, 1500), 100, funct_list["lin"](a=max_force/robot_width, b=max_force)))
# objs.append(PointObs(*goal, funct_list["lin"](a=0.01, b=-20)))
for poly in polys:
   objs.append(PolygonObs(poly, funct_list["lin"](a=max_force/robot_width, b=max_force)))

#objs.append(PolygonObs([(500, 500), (1500, 500), (1500, 1500), (500, 1500)], funct_list["lin"](a=0.01, b=3)))

for i in range(X.shape[0]):
    for k in range(X.shape[1]):
        for obj in objs:
            Z[i, k] += obj.get_scalar([float(X[i, k]), float(Y[i, k])])


#path, it = compute(objs, list(start), list(target))
#
#
#path_x = np.array([element[0] for element in path], dtype=np.float32)
#path_y = np.array([element[1] for element in path], dtype=np.float32)
#
#path_z = np.zeros_like(path_x, dtype=np.float32)
#for i in range(path_x.shape[0]):
#    for obj in objs:
#        path_z[i] += obj.get_scalaire([float(path_x[i]), float(path_y[i])])
#ax.plot(path_x, path_y, path_z, color="black")

# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)
# Add a color bar which maps values to colors.
#print(eval(path, target,  it))
plt.show()