#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdbool.h>

#include <graphe.h>

#define min_1(a,b) ((a==-1)?b:((b==-1)?a:((a<b)?a:b)))
// min where -1 represent +infinity
#define min(a,b) ((a>b)?(a):(b))
// regular min
#define max(a,b) ((a>b)?a:b)
// regular max

int bellman_ford(graphe_t* g) {
    int* distance = malloc(g->nb_vert*sizeof(int)); // -1 correspond à +inf
    for(int i=0; i<g->nb_vert; i++) {
        distance[i] = -1;
    }
    distance[0] = 0;
    for(int i=0; i<g->nb_vert; i++) {
        for(int j=0;j<g->nb_edge; j++) {
            if (distance[g->edges[j].i] != -1) {
                distance[g->edges[j].j] = min_1(distance[g->edges[j].j],1+distance[g->edges[j].i]);
            }
        }
    }
    int ret = distance[1];
    free(distance);
    return ret;
}

// TODO: BFS
int bfs(graphe_arr* g) {
    int* distance = malloc(g->nb_vert*sizeof(int));

    for(int i=0;i<g->nb_vert;i++){
        distance[i] = -1;
    }
    distance[0] = 0;
    
    for (int iteration=0;true;iteration++)  {
        for (int i=0;i<g->nb_vert;i++) {
            if (distance[i]==iteration) {
                for (int k=0;k<g->nb_connected[i];k++) {
                    if (g->edges[i][k].j==1) {
                        free(distance);
                        return iteration+1;
                    } else if ((distance[g->edges[i][k].j]) == -1) {
                        distance[g->edges[i][k].j] = iteration+1;
                    }
                }
            }
        }
    }

    free(distance);
    return -1;
}

int dijsktra(graphe_arr* g) {
    // distance = 0 représente +inf. Si distance[i] >0, i visité. Si distance[i]<0, i pas visité. La valeur absolue est la vraie distance.
    int* distance = malloc(g->nb_vert*sizeof(int));

    for(int i=0;i<g->nb_vert;i++){
        distance[i]=0;
    }
    distance[0]=-1; // Start at distance 1 bc 0 means +inf. We'll remove 1 at the end to compensate
    while(distance[1]<=0){
        int minimum_neg = -1;
        for(int j=0;j<g->nb_vert;j++){
            if (distance[j]<0 && (minimum_neg==-1 || distance[j]>distance[minimum_neg]))
                minimum_neg=j;
        }
        for(int k=0;k<g->nb_connected[minimum_neg];k++) {
            if(distance[g->edges[minimum_neg][k].j]<=0){
                if(distance[g->edges[minimum_neg][k].j]==0) {
                    distance[g->edges[minimum_neg][k].j] = distance[minimum_neg]-1;
                }
                else {
                    distance[g->edges[minimum_neg][k].j] = max(distance[minimum_neg]-1,distance[g->edges[minimum_neg][k].j]);
                }
            }
        }
        distance[minimum_neg]*=-1;
    }

    int ret = distance[1]-1;
    free(distance);
    return ret;
}

// shared variables
int* distance;
int nb_threads;
graphe_t* gra;
pthread_mutex_t* distance_mut;
pthread_barrier_t barrier; 


// thread function
void* thread_fn(void* arg) {

    int thread_id = (int) arg;
    
    int start = thread_id*gra->nb_edge/nb_threads;
    int end = (thread_id==nb_threads-1)?(gra->nb_edge):((thread_id+1)*gra->nb_edge/nb_threads);
    //printf("I am #%d from %d to %d\n",thread_id,start,end);

    for(int k=0;k<gra->nb_vert;k++) {
        for(int n=start;n<end;n++) {
            int i = gra->edges[n].i;
            int j = gra->edges[n].j;
            //printf("%d is checking edge %d->%d\n",thread_id,i,j);
            pthread_mutex_lock(&distance_mut[i]);
            int dist_i = distance[i]; 
            pthread_mutex_unlock(&distance_mut[i]);
            if (dist_i > 0) {
                pthread_mutex_lock(&distance_mut[j]);
                if (distance[j]==0) {
                    distance[j] = dist_i+1;
                    //printf("%d not done, distance[%d]==0\n",thread_id,j);
                } else {
                    if (distance[j]>dist_i+1) {
                        distance[j] = dist_i+1;
                        // printf("%d not done, distance[%d]>dist_i (=%d)+1\n",thread_id,j,dist_i);
                    }
                }
                pthread_mutex_unlock(&distance_mut[j]);
            }
        }
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

int bellman_ford_par(graphe_t* g,int nb_threads_) {
    gra = g;
    nb_threads = nb_threads_;
    pthread_t* threads = malloc(sizeof(pthread_t)*nb_threads_);
    distance = malloc(sizeof(int)*g->nb_vert);
    //  0 = no path yet
    //  x = path of length |x|, if x<0, needs updating
    distance_mut = malloc(sizeof(pthread_mutex_t)*g->nb_vert);
    
    for (int i=0;i<g->nb_vert;i++) {
        distance[i] = 0;
        pthread_mutex_init(&distance_mut[i],NULL);
    }
    distance[0] = 1;
    
    pthread_barrier_init(&barrier, NULL, nb_threads);

    for (int i=0;i<nb_threads;i++) {
        pthread_create(&threads[i],NULL,thread_fn,(void*)i);
    }
    //printf("Creating done\n");


    for (int i=0;i<nb_threads;i++) {
        pthread_join(threads[i],NULL);
        //printf("%d done\n",i);
    }
    //printf("Ending done\n");

    int result = distance[1]-1; // removing 1 because we started with an offset
    free(threads);
    free(distance_mut);
    free(distance);
    return result;
}