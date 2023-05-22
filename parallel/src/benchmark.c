#include <benchmark.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <graphe.h>
#include <algos.h>
#include <matrix_algos.h>

FILE *f_data;
FILE *f_log;


const int NB_PRETEST = 3;

int TESTS = 0;
const int DIJSKTRA       = 0b1;
const int BFS            = 0b10;
const int BELLMAN        = 0b100;
const int BELLMAN_PAR    = 0b1000;
const int MATRIX         = 0b10000;
const int MATRIX_MEM     = 0b100000;
const int MATRIX_PAR     = 0b1000000;
const int MATRIX_PAR_MEM = 0b10000000;

const char * ALGOS_NAMES[] = {
    "DIJSKTRA",
    "BELLMAN",
    "BELLMAN_PAR",
    "MATRIX",
    "MATRIX_MEM",
    "MATRIX_PAR",
    "MATRIX_PAR_MEM",
};

clock_t benchmark_start;


void setup_benchmark(char* name) {

    char path_data[100] = benchmark_data_path;
    char path_log[100] = benchmark_log_path;

    strcat(path_data, name);
    strcat(path_data, ".csv");
    strcat(path_log, name);
    strcat(path_log, ".txt");

    benchmark_start = clock();

    f_data = fopen(path_data,"w");
    f_log = fopen(path_log,"w");
    if(f_data == NULL)
    {
        printf("Can't create data file `%s` for benchmark\n",path_data);
        exit(1);
    } else if(f_log == NULL)
    {
        printf("Can't create logging file `%s` for benchmark\n",path_log);
        exit(1);
    }

    fprintf(f_data,"# -- benchmark `%s` created -- \n",name);
    fprintf(f_data,"# id, v_nb, e_nb, deg, algo_id, nb_threads, distance, time\n");
    fprintf(f_data,"# Algo_id are:\n");
    
    for(int i=0;i<7;i++){
        if((TESTS&(1<<i))){
            fprintf(f_data,"# - %s\n",ALGOS_NAMES[i]);
        }
    }
    fprintf(f_log," -- new benchmark `%s` created -- \n",name);
}

void close_benchmark() {
    fprintf(f_data,"# -- benchmark close -- \n");
    fprintf(f_log," -- benchmark close -- \n");
    fclose(f_data);
    fclose(f_log);
}

void benchmark_heatmap(int nb_times,int v_min,int v_max,int v_pad,int e_min, int e_max,int e_pad,int nb_threads) {

    for(int v_nb=v_min;v_nb<=v_max;v_nb+=v_pad) {
        benchmark_Vfixe(nb_times,v_nb,e_min,e_max,e_pad,nb_threads);
        printf(" -- V = %d done --\n",v_nb);
    }
}

void benchmark_distance(int nb_times,int nb_v,int d_min, int d_max,int d_pad,int nb_threads) {
    fprintf(f_log," -- new benchmark_distance(%d,%d,%d,%d,%d,%d) -- \n", nb_times, nb_v, d_min, d_max, d_pad, nb_threads);
    fprintf(f_data,"# -- benchmark_distance(%d,%d,%d,%d,%d,%d) -- \n", nb_times, nb_v, d_min, d_max, d_pad, nb_threads);

    // to remove outliar (first ones are non optimised by the os I'm guessing ?) :
    {
        for (int i=0;i<NB_PRETEST;i++) {

            int r = rand();
            srand(r);
            //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
            
            graphe_t* g = make_g_distance(d_min,(nb_v==-1)?d_min+1:nb_v);
            if (!test_all_on(g,-nb_times,nb_threads)){
                fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_g_distance(%d)\n",
                nb_times,r,d_min);
                exit(EXIT_FAILURE);
            }
            free_graph(g);
        }
    }
    printf("Pretest completed\n");
    for(int dist=d_min;dist<=d_max;dist+=d_pad) {
        
        if ((dist-d_min) % PRINT_PROGRES_ALL == 0) {
            printf("Completed %d out of %d.\n",(dist-d_min)*nb_times/d_pad,d_max*nb_times/d_pad);
        }
        
        int r = rand();
        srand(r);
        //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
        graphe_t* g = make_g_distance(dist,(nb_v==-1)?dist+1:nb_v);
        resize_graph(g);
        if (!test_all_on(g,nb_times,nb_threads)){
            fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_g_distance(%d)\n", nb_times,r,d_min);
            exit(EXIT_FAILURE);
        }
        free_graph(g);


    }
    fprintf(f_log," -- End of benchmark -- \n");
    printf("End of benchmark.\n");
}

void benchmark_Efixe(int nb_times,int v_min,int v_max,int v_pad,int e_nb,int nb_threads) {
    fprintf(f_log," -- new benchmark_Efixe(%d,%d,%d,%d,%d,%d) -- \n",nb_times,v_min,v_max,v_pad,e_nb,nb_threads);
    fprintf(f_data,"# -- benchmark_Efixe(%d,%d,%d,%d,%d,%d) -- \n",nb_times,v_min,v_max,v_pad,e_nb,nb_threads);

    // to remove outliar (first ones are non optimised by the os I'm guessing ?) :
    {
        for (int i=0;i<NB_PRETEST;i++) {

            int r = rand();
            srand(r);
            //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
            graphe_t* g = make_new(v_min,e_nb);
            gen_path(g,0,1,e_nb,0);
            resize_graph(g);
            if (!test_all_on(g,-nb_times,nb_threads)){
                fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in gen_path(g,0,1,%d,0)\n - in resize_graph(g)\n",
                nb_times,r,v_min,e_nb,e_nb);
                exit(EXIT_FAILURE);
            }
            free_graph(g);
        }
    }
    printf("Pretest completed\n");

    for(int v_nb=v_min;v_nb<=v_max;v_nb+=v_pad) {
        
        if ((e_nb-v_min) % PRINT_PROGRES_ALL == 0) {
            printf("Completed %d out of %d.\n",(v_nb-v_min)*nb_times/v_pad,v_max*nb_times/v_pad);
        }
        
        int r = rand();
        srand(r);
        //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
        graphe_t* g = make_new(v_nb,e_nb);
        gen_path(g,0,1,e_nb,0);
        resize_graph(g);
        if (!test_all_on(g,nb_times,nb_threads)){
            fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in gen_path(g,0,1,%d,0)\n - in resize_graph(g)\n",
            nb_times,r,v_min,e_nb,e_nb);
            exit(EXIT_FAILURE);
        }
        free_graph(g);


    }
    fprintf(f_log," -- End of benchmark -- \n");
    printf("End of benchmark.\n");
}

void benchmark_Vfixe(int nb_times,int v_nb,int e_min,int e_max,int e_pad,int nb_threads) {
    fprintf(f_log," -- new benchmark_Vfixe(%d,%d,%d,%d,%d,%d) -- \n",nb_times,v_nb,e_min,e_max,nb_threads,e_pad);
    fprintf(f_data,"# -- benchmark_Vfixe(%d,%d,%d,%d,%d,%d) -- \n",nb_times,v_nb,e_min,e_max,nb_threads, e_pad);

    // to remove outliar (first ones are non optimised by the os I'm guessing ?) :
    {
        for (int i=0;i<NB_PRETEST;i++) {

            int r = rand();
            srand(r);
            //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
            graphe_t* g = make_new(v_nb,e_min);
            gen_path(g,0,1,e_min,0);
            resize_graph(g);
            if (!test_all_on(g,-nb_times,nb_threads)){
                fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in gen_path(g,0,1,%d,0)\n - in resize_graph(g)\n",
                nb_times,r,v_nb,e_min,e_min);
                exit(EXIT_FAILURE);
            }
            free_graph(g);
        }
    }
    printf("Pretest completed\n");

    for(int e_nb=e_min;e_nb<=e_max;e_nb+=e_pad) {
        
        if ((e_nb-e_min) % PRINT_PROGRES_ALL == 0) {
            printf("Completed %d out of %d.\n",(e_nb-e_min)*nb_times/e_pad,e_max*nb_times/e_pad);
        }
        int r = rand();
        srand(r);
        //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
        graphe_t* g = make_new(v_nb,e_nb);
        gen_path(g,0,1,e_nb,0);
        resize_graph(g);
        if (!test_all_on(g,nb_times,nb_threads)){
            fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in gen_path(g,0,1,%d,0)\n - in resize_graph(g)\n",
            nb_times,r,v_nb,e_nb,e_nb);
            exit(EXIT_FAILURE);
        }
        free_graph(g);


    }
    fprintf(f_log," -- End of benchmark -- \n");
    printf("End of benchmark.\n");
}

void benchmark_fullG(int nb_times,int v_min,int v_max,int v_pad,int nb_threads) {

    fprintf(f_log," -- new benchmark_fullG(%d,%d,%d,%d,%d) -- \n",nb_times,v_min,v_max,v_pad,nb_threads);
    fprintf(f_data,"# -- benchmark_fullG(%d,%d,%d,%d,%d) -- \n",nb_times,v_min,v_max,v_pad,nb_threads);

    // to remove outliar (first ones are non optimised by the os I'm guessing ?) :
    {
        for (int i=0;i<NB_PRETEST;i++) {

            int r = rand();
            srand(r);
            //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
            graphe_t* g = make_new(v_min,v_min*v_min);
            full_g(g);
            resize_graph(g);
            if (!test_all_on(g,-nb_times,nb_threads)){
                fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in full_g(g)\n - in resize_graph(g)\n",
                nb_times,r,v_min,v_min*v_min);
                exit(EXIT_FAILURE);
            }
            free_graph(g);
        }
    }
    printf("Pretest completed\n");

    for(int v_nb=v_min;v_nb<=v_max;v_nb+=v_pad) {
        
        if ((v_nb-v_min) % PRINT_PROGRES_ALL == 0) {
            printf("Completed %d out of %d.\n",(v_nb-v_min)*nb_times/v_pad,v_max*nb_times/v_pad);
        }
        int r = rand();
        srand(r);
        //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,v_nb);
        graphe_t* g = make_new(v_nb,v_nb*v_nb);
        full_g(g);
        resize_graph(g);
        if (!test_all_on(g,nb_times,nb_threads)){
            fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in full_g(g)\n - in resize_graph(g)\n",
            nb_times,r,v_nb,v_nb*v_nb);
            exit(EXIT_FAILURE);
        }
        free_graph(g);


    }
    fprintf(f_log," -- End of benchmark -- \n");
    printf("End of benchmark.\n");
}

void benchmark_threads(int nb_times,int v_nb,int e_nb,int th_min,int th_max){
    fprintf(f_log," -- new benchmark_threads(%d,%d,%d,%d,%d) -- \n",nb_times,v_nb,e_nb,th_min,th_max);
    fprintf(f_data,"# -- benchmark_threads(%d,%d,%d,%d,%d) -- \n",nb_times,v_nb,e_nb,th_min,th_max);

    // to remove outliar (first ones are non optimised by the os I'm guessing ?) :
    {
        for (int i=0;i<NB_PRETEST;i++) {

            int r = rand();
            srand(r);
            //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
            graphe_t* g = make_new(v_nb,e_nb);
            gen_path(g,0,1,e_nb,0);
            resize_graph(g);
            if (!test_all_on(g,-nb_times,th_min)){
                //fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in gen_path(g,0,1,%d,0)\n - in resize_graph(g)\n",nb_times,r,v_nb,e_nb);
                exit(EXIT_FAILURE);
            }
            free_graph(g);
        }
    }
    printf("Pretest completed\n");

    for(int nb_threads=th_min;nb_threads<=th_max;nb_threads++) {
        
        if ((nb_threads-th_min) % PRINT_PROGRES_ALL == 0) {
            printf("Completed %d out of %d.\n",(nb_threads-th_min)*nb_times,(th_max-th_min)*nb_times);
        }

        int r = rand();
        srand(r);
        //fprintf(f_log,"g = make_new(%d,%d)\n",v_nb,e_nb);
        graphe_t* g = make_new(v_nb,e_nb);
        gen_path(g,0,1,e_nb,0);
        resize_graph(g);
        if (!test_all_on(g,nb_times,nb_threads)){
            fprintf(f_log,"In test_all_on(g,%d) (srand=%d):\n - make_new(%d,%d)\n - in gen_path(g,0,1,%d,0)\n - in resize_graph(g)\n",
            nb_times,r,v_nb,e_nb,e_nb);
            exit(EXIT_FAILURE);
        }
        free_graph(g);


    }
    fprintf(f_log," -- End of benchmark -- \n");
    printf("End of benchmark.\n");
    
}


bool test_all_on(graphe_t* g,int nb_times,int nb_threads) {
    bool save = (nb_times>=0);
    if (!save)
        nb_times*=-1;
    int id = (int)(clock() - benchmark_start);
    for (int nb=0;nb<nb_times;nb++) {
        
        double deg = get_deg_info(g);
        int results[8];
        double times[8];

        if (TESTS&DIJSKTRA){times[0] = test_dijsktra(g,nb_threads,results);};
        if (TESTS&BFS){times[1] = test_bfs(g,nb_threads,results+1);};
        if (TESTS&BELLMAN){times[2] = test_bellman_ford(g,nb_threads,results+2);};
        if (TESTS&BELLMAN_PAR){times[3] = test_bellman_ford_par(g,nb_threads,results+3);};
        if (TESTS&MATRIX){times[4] = test_matrix(g,nb_threads,results+4);};
        if (TESTS&MATRIX_MEM){times[5] = test_matrix_mem(g,nb_threads,results+5);};
        if (TESTS&MATRIX_PAR){times[6] = test_matrix_par(g,nb_threads,results+6);};
        if (TESTS&MATRIX_PAR_MEM){times[7] = test_matrix_par_mem(g,nb_threads,results+7);};

        //printf("test_dijsktra = %d WHILE test_bellman_ford_par = %d\n",results[0],results[2]);

        for(int i=0;i<8;i++){
            if((TESTS&(1<<i)) && (results[0] != results[i])){
                printf("ERROR -- Wrong result beetween tests:\n algo ID=%d says %d != %d\n",i,results[0],results[i]);
                fprintf(f_log,"ERROR -- Wrong result beetween tests:\n algo ID=%d says %d != %d\n",i,results[0],results[i]);
                return false;
            }
        }
        
        if (save){ // nb_times<0 used to not save the result
            for (int i=0;i<7;i++){
                if (TESTS&(1<<i))
                    fprintf(f_data,"%2d, %2d, %2d, %0.3f, %d, %d, %d, %.8f\n", id, g->nb_vert, g->nb_edge, deg, i, nb_threads, results[i], times[i]);
            }
        }
        
    }
    return true;
}

double test_bellman_ford(graphe_t* g, int _, int* result) {

    clock_t begin = clock();
    *result = bellman_ford(g);
    clock_t end = clock();
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}

double test_bellman_ford_par(graphe_t* g, int nb_threads, int* result) {

    clock_t begin = clock();
    *result = bellman_ford_par(g,nb_threads);
    clock_t end = clock();
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}

double test_dijsktra(graphe_t* g, int _, int* result) {
    graphe_arr* g2 = to_graphe_arr(g);
    
    clock_t begin = clock();
    *result = dijsktra(g2);
    clock_t end = clock();

    free_graph_arr(g2);
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}

double test_bfs(graphe_t* g, int _, int* result) {
    graphe_arr* g2 = to_graphe_arr(g);
    
    clock_t begin = clock();
    *result = bfs(g2);
    clock_t end = clock();

    free_graph_arr(g2);
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}

double test_matrix(graphe_t* g, int _, int* result) {
    graphe_mat* g2 = to_graphe_mat(g);

    clock_t begin = clock();
    *result = matrix(g2);
    clock_t end = clock();

    free_graph_mat(g2);

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}

double test_matrix_mem(graphe_t* g, int nb_threads, int* result) {
    graphe_mat* g2 = to_graphe_mat(g);

    clock_t begin = clock();
    *result = matrix_mem(g2);
    clock_t end = clock();

    free_graph_mat(g2);

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}

double test_matrix_par(graphe_t* g, int nb_threads, int* result) {
    graphe_mat* g2 = to_graphe_mat(g);

    clock_t begin = clock();
    *result = matrix_par(g2,nb_threads);
    clock_t end = clock();

    free_graph_mat(g2);

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}

double test_matrix_par_mem(graphe_t* g, int nb_threads, int* result) {
    graphe_mat* g2 = to_graphe_mat(g);

    clock_t begin = clock();
    *result = matrix_par_mem(g2,nb_threads);
    clock_t end = clock();

    free_graph_mat(g2);

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    return time_spent;
}