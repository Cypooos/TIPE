#include <graphe.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#define max(a,b) ((a<b)?(b):(a))
#define min(a,b) ((a<b)?(a):(b))

// --- others representations ---
// graphe_arr
graphe_arr* to_graphe_arr(graphe_t* g) {
    graphe_arr* ret = malloc(sizeof(graphe_arr));
    ret->nb_vert = g->nb_vert;
    ret->nb_edge = g->nb_edge;
    ret->nb_connected = malloc(sizeof(int)*g->nb_vert);
    ret->edges = malloc(sizeof(edges_t*)*g->nb_vert);
    for (int i =0;i<g->nb_vert;i++) {
        ret->nb_connected[i] = 0;
    }
    for(int i=0;i<g->nb_edge;i++) {
        ret->nb_connected[g->edges[i].i]++;
    }
    for(int i=0;i<g->nb_vert;i++) {
        ret->edges[i] = malloc(sizeof(edges_t)*ret->nb_connected[i]);
        ret->nb_connected[i] = 0;
    }
    for(int i=0;i<g->nb_edge;i++) {
        ret->edges[g->edges[i].i][ret->nb_connected[g->edges[i].i]] = g->edges[i];
        ret->nb_connected[g->edges[i].i]++;
    }
    return ret;
}

void print_graphe_arr(graphe_arr* g) {
    printf(" -- print_graphe_arr --\nnb_vert:%d,  nb_edge:%d\n",g->nb_vert,g->nb_edge);
    for(int i=0;i<g->nb_vert;i++) {
        printf("for %d, nb=%d:",i,g->nb_connected[i]);
        for(int j=0;j<g->nb_connected[i];j++) {
            printf("(%d->%d), ",g->edges[i][j].i,g->edges[i][j].j);
            if(j>6) {printf("...");break;}
        }
        printf("\n");
        if(i>10) {printf("...\n");break;}
    }
    printf("\n");
}

void free_graph_arr(graphe_arr* g) {
    for(int i=0;i<g->nb_vert;i++){
        free(g->edges[i]);
    }
    free(g->nb_connected);
    free(g->edges);
    free(g);
}



// graph_mat
graphe_mat* to_graphe_mat(graphe_t* g){
    graphe_mat* result = malloc(sizeof(graphe_mat));
    result->nb_vert = g->nb_vert;
    result->matrice = malloc(sizeof(int*)*g->nb_vert);
    for(int i=0;i<g->nb_vert;i++){
        result->matrice[i] = malloc(sizeof(int)*g->nb_vert);
        for(int j=0;j<g->nb_vert;j++){
            result->matrice[i][j] = 0;
        }
    }

    for(int i=0;i<g->nb_edge;i++) {
        result->matrice[g->edges[i].i][g->edges[i].j] = 1;
    }

    return result;
}
void print_graphe_mat(graphe_mat* g) {
    printf(" -- print_graphe_mat --\nnb_vert:%d,mat:\n",g->nb_vert);
    for(int i=0;i<g->nb_vert;i++) {
        printf("[");
        for(int j=0;j<g->nb_vert;j++) {
            printf("%d, ",g->matrice[i][j]);
        }
        printf("]\n");
    }
}
void free_graph_mat(graphe_mat* g) {
    for(int i=0;i<g->nb_vert;i++){
        free(g->matrice[i]);
    }
    free(g->matrice);
    free(g);
}


// --- general ---
graphe_t* make_new(int nb_v,int nb_e) {
    graphe_t* g = malloc(sizeof(graphe_t));
    g->nb_edge = nb_e;
    g->nb_vert = nb_v;
    edges_t* edges = malloc(nb_e*sizeof(edges_t));

    for(int k=0;k<nb_e;k++) {
        edges[k].i = -1;
        edges[k].j = -1;
    }
    g->edges = edges;
    return g;
}

void free_graph(graphe_t* g) {
    free(g->edges);
    free(g);
}

int add_edge_at(graphe_t* g,edges_t ed,int i) {
    assert(i<g->nb_edge);
    for(int j=0;j<i;j++) {
        if (g->edges[j].i == ed.i && g->edges[j].j == ed.j ) { // if edge already exist, don't add again
            return i;
        }
    }
    g->edges[i] = ed;
    return i+1;
}

void resize_graph(graphe_t* g) {
    int j;
    for(j=0;j<g->nb_edge;j++) {
        if(g->edges[j].i == -1 && g->edges[j].j == -1) {
            break;
        }
    }
    if(j<g->nb_edge) {
        edges_t* edges = malloc(j*sizeof(edges_t));
        for(int k=0;k<j;k++) {
            edges[k].i = g->edges[k].i;
            edges[k].j = g->edges[k].j;
        }
        free(g->edges);
        g->edges = edges;
        g->nb_edge = j; 
    }
}


void full_g(graphe_t* g) {
    int c = 0;
    for (int i=0;i<g->nb_vert;i++){
        for (int j=0;j<g->nb_vert;j++){
            if (i!=j){
                edges_t to_add = {.i=i, .j=j};
                c = add_edge_at(g,to_add,c);
            }
        }
    }
}

double get_deg_info(graphe_t* g) {
    return ((double)g->nb_edge)/(double)g->nb_vert;
}

int gen_path(graphe_t* g,int start,int end,int length,int i_) {
    if (length == 0) {return i_;}
    int i = i_;
    int current = start;
    for(int j=0;j<length-1;j++) {
        int next = rand() % g->nb_vert;
        edges_t to_add = {.i=current, .j=next};
        i = add_edge_at(g,to_add,i);
        current = next;
    }
    edges_t to_add = {.i=current, .j=end};
    return add_edge_at(g,to_add,i);
}

void print_graphe(graphe_t* g) {
    printf(" -- print_graphe --\nnb_vert:%d,  nb_edge:%d\n",g->nb_vert,g->nb_edge);
    for(int i =0;i<g-> nb_edge;i++) {
        printf("(%d -> %d), ",g->edges[i].i,g->edges[i].j);
        if (i%6 == 5) {
            printf("\n");
        }
        if (i>100) {
            printf("...\n");
            break;
        }
    }
    printf("\n");
}