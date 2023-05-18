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
NAMES = ["Dijkstra", "BFS", "Bellman Ford", "Matrix", "Matrix (par)"]
COLORS = ['red','black','orange','b-','c-']
SIZE = 500
data = np.array([(x,x*x) for x in np.linspace(1,SIZE,SIZE*2)])

fig, axis = plt.subplots()
#fig.tight_layout()

for i,fct in enumerate(functions):
    y= [fct(x) for x in data]
    axis.plot(data[:,0], y, COLORS[i], lw=1,label=NAMES[i])


axis.set_ylabel("Temps d'éxecution (s)")
plt.yscale('log')
axis.set_xlabel("|V| pour |E|=|V|²")
axis.legend()
axis.grid(True)
plt.show()