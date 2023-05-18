#include <graphe.h>

#ifndef MATRIX_ALGOS_H
#define MATRIX_ALGOS_H 1

int matrix(graphe_mat* g);
int matrix_mem(graphe_mat* g);
int matrix_par(graphe_mat* g, int nb_threads);
int matrix_par_mem(graphe_mat* g, int nb_threads);

#endif