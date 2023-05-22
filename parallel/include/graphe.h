#ifndef GRAPHE_H
#define GRAPHE_H 1

typedef struct edges {
    int i;
    int j;
} edges_t;

typedef struct graphe {
    int nb_vert;
    int nb_edge;
    edges_t* edges;
} graphe_t;

typedef struct graphe_arr {
    int nb_vert;
    int nb_edge;
    int* nb_connected;
    edges_t** edges;
} graphe_arr;

typedef struct graphe_mat {
    int nb_vert;
    int** matrice;
} graphe_mat;

graphe_t* make_new(int nb_v,int nb_e);

int add_edge_at(graphe_t* g,edges_t ed,int i);
void resize_graph(graphe_t* g);
double get_deg_info(graphe_t* g);
int gen_path(graphe_t* g,int start,int end,int length,int i_);
void full_g(graphe_t* g);
graphe_t* make_g_distance(int dist,int nb_v);

void print_graphe(graphe_t* g);
void free_graph(graphe_t* g);

// array of neigbours repr
graphe_arr* to_graphe_arr(graphe_t* g);
void print_graphe_arr(graphe_arr* g);
void free_graph_arr(graphe_arr* g);

// matrix repr
graphe_mat* to_graphe_mat(graphe_t* g);
void print_graphe_mat(graphe_mat* g);
void free_graph_mat(graphe_mat* g);

#endif