#include <graphe.h>
#include <matrix_algos.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

void product(int** matA,int** matB, int** result,int size) {
    for(int i =0;i<size;i++) {
        for(int j =0;j<size;j++) {
            int sum = 0;
            for (int k=0;k<size;k++) {
                sum += matA[i][k]*matB[k][j];
            }
            result[i][j] = sum;
        }
    }
}

// for strassen maybe
void product_offset(int** matA,int** matB, int** result,int offsetX,int offsetY,int size) {
    for(int i =offsetY;i<+offsetY+size;i++) {
        for(int j =offsetX;j<offsetX+size;j++) {
            int sum = 0;
            for (int k=0;k<size;k++) {
                sum += matA[i][offsetX+k]*matB[offsetY+k][j];
            }
            result[i][j] = sum;
        }
    }

}


void copy(int** in,int** out,int size) {
    for(int i =0;i<size;i++) {
        for(int j =0;j<size;j++) {
            out[i][j] = in[i][j];
        }
    }
}

int matrix_mem(graphe_mat* g) {

    //printf("matrix\n");
    g->matrice[1][1] = 1;
    int** borne_inf = malloc(sizeof(int*)*g->nb_vert);
    int** k = malloc(sizeof(int*)*g->nb_vert);
    
    for (int i=0;i<g->nb_vert;i++) {
        
        borne_inf[i] = malloc(sizeof(int)*g->nb_vert);
        k[i] = malloc(sizeof(int)*g->nb_vert);

        for (int j=0;j<g->nb_vert;j++){
            borne_inf[i][j] = (i==j);
            k[i][j] = 0;
        };
    };
    
    int nb_times = 1;
    int temp = g->nb_vert;

    while (temp >>= 1) { ++nb_times; }
    
    int*** times = malloc(sizeof(int**)*nb_times);
    for (int i=0;i<nb_times;i++) {
        times[i] = NULL;
    }
    times[0] = malloc(sizeof(int*)*g->nb_vert);
    for (int i=0;i<g->nb_vert;i++) {
        times[0][i] = malloc(sizeof(int)*g->nb_vert);
    }
    copy(g->matrice,times[0],g->nb_vert);


    int borne_inf_counter = 1;
    int times_counter = 1;
    int times_index = 0;

    //printf("alloc done\n");
    while (true) {
        product(borne_inf,times[times_index],k,g->nb_vert);
        if (k[0][1] >= 1) {
            //printf("Detected cycle in next pow, resetting\n");
            if (times_index == 0) {
                break;
            }
            times_counter = 1;
            times_index = 0;
        } else {
            
            //printf("Can raise power !\n");
            copy(k, borne_inf, g->nb_vert);
            if (times[times_index+1]==NULL) {
                times[times_index+1] = malloc(sizeof(int*)*g->nb_vert);
                for (int i=0;i<g->nb_vert;i++) {
                    times[times_index+1][i] = malloc(sizeof(int)*g->nb_vert);
                }
                product(times[times_index],times[times_index],times[times_index+1],g->nb_vert);
            }
            borne_inf_counter += times_counter;
            times_counter *=2;
            times_index +=1;
        }
    }

    for (int i=0;i<g->nb_vert;i++) {
        free(borne_inf[i]);
        free(k[i]);
    }
    for (int i=0;i<nb_times;i++) {
        if (times[i]) {
            for (int j=0;j<g->nb_vert;j++) {
                free(times[i][j]);
            }
            free(times[i]);
        }
    }
    free(borne_inf);
    free(times);
    free(k);
    return borne_inf_counter;
    
}


int matrix(graphe_mat* g) {

    //printf("matrix\n");
    g->matrice[1][1] = 1;
    int** borne_inf = malloc(sizeof(int*)*g->nb_vert);
    int** times = malloc(sizeof(int*)*g->nb_vert);
    int** k = malloc(sizeof(int*)*g->nb_vert);
    
    for (int i=0;i<g->nb_vert;i++) {
        
        borne_inf[i] = malloc(sizeof(int)*g->nb_vert);
        k[i] = malloc(sizeof(int)*g->nb_vert);
        times[i] = malloc(sizeof(int)*g->nb_vert);

        for (int j=0;j<g->nb_vert;j++){
            borne_inf[i][j] = (i==j);
            times[i][j] = g->matrice[i][j];
            k[i][j] = 0;
        };
    };
    int borne_inf_counter = 1;
    int times_counter = 1;

    //printf("alloc done\n");
    while (true) {
        product(borne_inf,times,k,g->nb_vert);
        if (k[0][1] >= 1) {
            //printf("Detected cycle in next pow, resetting\n");
            if (times_counter == 1) {
                break;
            }
            copy(g->matrice,times,g->nb_vert);
            times_counter = 1;
        } else {
            //printf("Can raise power !\n");
            copy(k, borne_inf, g->nb_vert);
            product(times,times,k,g->nb_vert);
            copy(k,times,g->nb_vert);
            borne_inf_counter += times_counter;
            times_counter *=2;
        }
    }

    for (int i=0;i<g->nb_vert;i++) {
        free(borne_inf[i]);
        free(times[i]);
        free(k[i]);
    }
    free(borne_inf);
    free(times);
    free(k);
    return borne_inf_counter;
}


// shared variables
int matrix_nb_threads;
graphe_mat* matrix_gra;
int** matrix_prod;
pthread_barrier_t matrix_barrier; 

int matrix_par(graphe_mat* g, int nb_threads){
    return 0;
}
int matrix_par_mem(graphe_mat* g, int nb_threads){
    return 0;
}