#include <graphe.h>

#ifndef ALGOS_H
#define ALGOS_H 1

int bellman_ford(graphe_t* g);
int dijsktra(graphe_arr* g);
int bfs(graphe_arr* g);
int bellman_ford_par(graphe_t* g, int nb_threads);

#endif