#include <stdlib.h>
#include <stdio.h>
#include <benchmark.h>
#include <algos.h>
#include <graphe.h>

int main() {
    srand(1);

    /*
    srand(1);
    TESTS = DIJSKTRA | BFS | MATRIX | MATRIX_MEM;
    setup_benchmark("dist_1to100");
    benchmark_distance(10,130,2,129,1,3);
    close_benchmark();
    */

    /*
    TESTS = DIJSKTRA | BFS | BELLMAN | MATRIX | MATRIX_MEM;
    setup_benchmark("full_v50to1000");
    benchmark_fullG(10,50,1000,10,3);
    close_benchmark();
    /*

    TESTS = DIJSKTRA | BFS | BELLMAN | MATRIX | MATRIX_MEM;
    setup_benchmark("heatmap_regular");
    benchmark_heatmap(3,5,300,5,50,5000,50,1);
    close_benchmark();

    //*
    TESTS = BELLMAN_PAR | BFS | BELLMAN | DIJSKTRA;
    setup_benchmark("th1to5_bellmanF_e100000_v1000");
    benchmark_threads(10,1000,100000,1,10);
    close_benchmark();

    TESTS = DIJSKTRA | BFS | BELLMAN | BELLMAN_PAR | MATRIX | MATRIX_MEM;
    setup_benchmark("v50to500_e500");
    benchmark_Efixe(10,2,500,2,500,3);
    close_benchmark();

    /*
    TESTS = DIJSKTRA | BFS | BELLMAN | BELLMAN_PAR | MATRIX | MATRIX_MEM;
    setup_benchmark("v50_e10to2500");
    benchmark_Vfixe(10,50,10,2500,10,3);
    close_benchmark();
    /**/
}
