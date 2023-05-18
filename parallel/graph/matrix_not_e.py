import numpy as np
from numpy import genfromtxt
import matplotlib.pyplot as plt


# Show the complexity of matrix is constant toward e_nb but not to the distance 
DTYPES = [("id",int),("v_nb",int),("e_nb",int),("deg",float),("algo_id",int),("nb_threads",int),("distance",int),("time",float)]
PATH = 'data/v50_e10to2500.csv'
DIJKSTRA, BFS, BELLMAN, BELLMAN_PAR, MATRIX, MATRIX_MEM, MATRIX_PAR, MATRIX_PAR_MEM = 0,1,2,3,4,5,6,7

data = genfromtxt(PATH,dtype=DTYPES, comments='#', delimiter=',')
data.sort(order="e_nb")

datas = [data[data["distance"]==i] for i in np.unique(data["distance"]) if len(data[data["distance"]==i])>10 ]
datas = datas[:2]

fig, axis = plt.subplots(len(datas),sharex=True, sharey=True)
#fig.tight_layout()
plt.suptitle("Temps d'execution en fonction de |E| et de la distance pour |V|=50")

plt.yscale('log')

for i,dat in enumerate(datas):

    def get_mean(x_axis,y_axis,list):
        return [np.mean(list[list[x_axis]==x][y_axis]) for x in np.unique(dat[x_axis])]

    matrix = get_mean("e_nb","time",dat[dat["algo_id"]==MATRIX])
    matrix_mem = get_mean("e_nb","time",dat[dat["algo_id"]==MATRIX_MEM])
    dijkstra = get_mean("e_nb","time",dat[dat["algo_id"]==DIJKSTRA])
    bfs = get_mean("e_nb","time",dat[dat["algo_id"]==BFS])
    bellman = get_mean("e_nb","time",dat[dat["algo_id"]==BELLMAN])
    # bellman_par = get_mean("e_nb","time",dat[dat["algo_id"]==BELLMAN_PAR]) V=50, nope

    x_axis = np.unique(dat["e_nb"])


    axis[i].set_title("distance = "+str(dat[0]["distance"]), loc='left')
    axis[i].plot(x_axis, matrix, 'b-', lw=1,label="Matrix")
    axis[i].plot(x_axis, matrix_mem, 'c-', lw=1,label="Matrix Memoisé")
    axis[i].plot(x_axis, bellman, 'orange', lw=1,label="BellmanFord")
    # axis[i].plot(x_axis, bellman_par, 'orange', lw=1,label="BellmanFord Par") V = 50, nope
    axis[i].plot(x_axis, dijkstra, 'red', lw=1,label="Dijstra")
    axis[i].plot(x_axis, bfs, 'black', lw=1,label="BFS")


    axis[i].grid(True)

axis[0].set_ylabel("Temps d'éxecution (s)")
axis[-1].set_xlabel("|E| pour |V|=50")
axis[0].legend()
plt.show()

