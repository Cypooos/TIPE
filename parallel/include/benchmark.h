#include <stdbool.h>
#include <string.h>
#include <graphe.h>

#ifndef BENCHMARK_H
#define BENCHMARK_H 1

#define benchmark_data_path "/home/cypooos/code/TIPE/parallel/data/"
#define benchmark_log_path  "/home/cypooos/code/TIPE/parallel/logs/"

#define PRINT_PROGRES_ALL 3

/*
ORDER OF ALGO_ID :
 - dijsktra
 - bellman_ford
 - bellman_ford_par
 - matrix
 - matrix_mem
 - matrix_par
 - matrix_par_mem
*/

extern const int NB_PRETEST;

extern int TESTS;
extern const int DIJSKTRA;
extern const int BFS;
extern const int BELLMAN;
extern const int BELLMAN_PAR;
extern const int MATRIX;
extern const int MATRIX_MEM;
extern const int MATRIX_PAR;
extern const int MATRIX_PAR_MEM;

void setup_benchmark(char* name);

void benchmark_heatmap(int nb_times,int v_min,int v_max,int v_pad,int e_min, int e_max,int e_pad,int nb_threads);
void benchmark_Efixe(int nb_times,int v_min,int v_max,int v_pad,int e_nb,int nb_threads);
void benchmark_threads(int nb_times,int v_nb,int e_nb,int th_min,int th_max);
void benchmark_fullG(int nb_times,int v_min,int v_max,int v_pad,int nb_threads);
void benchmark_Vfixe(int nb_times,int v_nb, int e_min,int e_max,int e_pad,int nb_threads);
void benchmark_distance(int nb_times,int nb_v,int d_min, int d_max,int d_pad,int nb_threads);
void close_benchmark();

bool test_all_on(graphe_t* g,int nb_times, int nb_threads);

// algos.h
double test_bellman_ford(graphe_t* g, int nb_threads, int* result);
double test_dijsktra(graphe_t* g, int nb_threads, int* result);
double test_bfs(graphe_t* g, int _, int* result);
double test_bellman_ford_par(graphe_t* g, int nb_threads, int* result);

// matrix.h
double test_matrix(graphe_t* g, int nb_threads, int* result);
double test_matrix_mem(graphe_t* g, int nb_threads, int* result);
double test_matrix_par(graphe_t* g, int nb_threads, int* result);
double test_matrix_par_mem(graphe_t* g, int nb_threads, int* result);

#endif