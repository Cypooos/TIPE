import numpy as np
from numpy import genfromtxt
import matplotlib.pyplot as plt

# Show the complexity of matrix without memoisation

DTYPES = [("id",int),("v_nb",int),("e_nb",int),("deg",float),("algo_id",int),("nb_threads",int),("distance",int),("time",float)]
PATH = 'data/heatmap_regular.csv'
DIJKSTRA, BFS, BELLMAN, BELLMAN_PAR, MATRIX, MATRIX_MEM, MATRIX_PAR, MATRIX_PAR_MEM = 0,1,2,3,4,5,6,7
NAMES = ["Dijkstra", "BFS", "Bellman Ford", "Bellman Ford (par.)", "Matrix", "Matrix (mem)", "Matrix (par)", "Matrix (par+mem)"]

data_raw = genfromtxt(PATH,dtype=DTYPES, comments='#', delimiter=',')

fig, axis = plt.subplots(len(np.unique(data_raw["algo_id"])), sharex=True, sharey=True)
fig.tight_layout()
#plt.suptitle("Temps d'execution en fonction de |V| et |E|")

data_raw.sort(order=("id"))
EXTEND = (np.min(data_raw["v_nb"]),np.max(data_raw["v_nb"]),np.min(data_raw["e_nb"]),np.max(data_raw["e_nb"]))

for id_nb,id in enumerate(np.unique(data_raw["algo_id"])):

    data = data_raw[data_raw["algo_id"]==id]
    def get_mean(y_axis,list):
        return np.mean(list[y_axis])

    x_size = len(np.unique(data["v_nb"]))
    y_size = len(np.unique(data[data["v_nb"] == data["v_nb"][0]]["id"]))
    result = np.empty(shape=(x_size,y_size))
    for i,x in enumerate(np.unique(data["v_nb"])):
        
        for j,y in enumerate(np.unique(data[data["v_nb"]==x]["id"])):
            to_get_mean = data[(data["v_nb"]==x) & (data["id"]==y)]
            result[i][j] = get_mean("time",to_get_mean)

    # bellman_par = get_mean("v_nb","time",data[data["algo_id"]==BELLMAN_PAR])

    axis[id_nb].set_title(NAMES[id])
    axis[id_nb].set_ylabel('|E|')
    im = axis[id_nb].imshow(result, origin = 'lower', cmap='hot', interpolation='nearest',extent=EXTEND, aspect='auto')

    # Add the color bar
    cbar = axis[id_nb].figure.colorbar(im, ax = axis[id_nb])
    cbar.ax.set_ylabel("Time (s)", rotation = -90, va = "bottom")


axis[-1].set_xlabel('|V|')
plt.show()

