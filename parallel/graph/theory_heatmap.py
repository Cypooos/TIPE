import numpy as np
from numpy import genfromtxt
import matplotlib.pyplot as plt

functions = [
    lambda coord: (coord[0]+coord[1])*np.log(coord[0]), # dij
    lambda coord: coord[0]+coord[1], # bfs
    lambda coord: coord[0]*(coord[0]+coord[1]), # bellman
    lambda coord: coord[0]*coord[0]*coord[0]*(np.log(coord[0])*np.log(coord[0])), # matrix trad
    lambda coord: coord[0]*(np.log(coord[0])*np.log(coord[0])), # matrix parall
]
NAMES = ["Dijkstra", "BFS", "Bellman Ford", "Matrix", "Matrix (mem)"]

SIZE_X = 500
SIZE_Y = 500
data = (np.indices((SIZE_X,SIZE_Y))+1).transpose((1,2,0))

fig, axis = plt.subplots(len(functions), sharex=True, sharey=True)
fig.tight_layout()

for i,fct in enumerate(functions):
    new = [[fct(c) for c in row] for row in data]
    im = axis[i].imshow(new, origin = 'lower', cmap='hot', interpolation='nearest', aspect='auto')
    
    axis[i].set_title(NAMES[i])
    axis[i].set_ylabel('|E|')

    #cbar = axis[i].figure.colorbar(im, ax = axis[i])

axis[-1].set_xlabel('|V|')
plt.show()