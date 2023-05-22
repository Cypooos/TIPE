import numpy as np
from numpy import genfromtxt
import matplotlib.pyplot as plt

DTYPES = [("id",int),("v_nb",int),("e_nb",int),("deg",float),("algo_id",int),("nb_threads",int),("distance",int),("time",float)]
PATH = 'data/dist_1to100.csv'
DIJKSTRA, BFS, BELLMAN, BELLMAN_PAR, MATRIX, MATRIX_MEM, MATRIX_PAR, MATRIX_PAR_MEM = 0,1,2,3,4,5,6,7

data = genfromtxt(PATH,dtype=DTYPES, comments='#', delimiter=',')
data.sort(order="distance")


def get_mean(x_axis,y_axis,list):
    return [np.mean(list[list[x_axis]==x][y_axis]) for x in np.unique(data[x_axis])]

matrix = get_mean("distance","time",data[data["algo_id"]==MATRIX])
matrix_mem = get_mean("distance","time",data[data["algo_id"]==MATRIX_MEM])
# bfs = get_mean("distance","time",data[data["algo_id"]==BFS])
# dijkstra = get_mean("distance","time",data[data["algo_id"]==DIJKSTRA])
# bellman = get_mean("distance","time",data[data["algo_id"]==BELLMAN])
# bellman_par = get_mean("v_nb","time",data[data["algo_id"]==BELLMAN_PAR])

x_axis = np.unique(data["distance"])


fig, axis = plt.subplots()
#fig.tight_layout()
plt.suptitle("Temps d'execution en fonction de la distance")

axis.plot(x_axis, matrix, 'b-', lw=1,label="Matrix")
axis.plot(x_axis, matrix_mem, 'orange', lw=1,label="Matrix (mem)")
axis.plot(x_axis, [matrix[x]-matrix_mem[x] for x in range(len(matrix))], 'green', lw=1,label="Difference")
# axis.plot(x_axis, bellman, 'orange', lw=1,label="BellmanFord")
# axis.plot(x_axis, bellman_par, 'yellow', lw=1,label="BellmanFord Par.")
# axis.plot(x_axis, dijkstra, 'red', lw=1,label="Dijstra")
# axis.plot(x_axis, bfs, 'black', lw=1,label="BFS")


axis.set_ylabel("Temps d'éxecution (s)")
# plt.yscale('log')
axis.set_xlabel("distance")
axis.legend()
axis.grid(True)
plt.show()

