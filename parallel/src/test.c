#include <stdlib.h>
#include <stdio.h>
#include <benchmark.h>
#include <algos.h>
#include <graphe.h>

int main() {
    srand(1);

    TESTS = DIJSKTRA | MATRIX | MATRIX_MEM;
    setup_benchmark("dist_1to100");
    benchmark_distance(10,130,2,129,1,3);
    close_benchmark();


    /*
    TESTS = DIJSKTRA | BFS | BELLMAN | MATRIX | MATRIX_MEM;
    setup_benchmark("full_v50to1000");
    benchmark_fullG(3,50,1000,50,3);
    close_benchmark();
    //

    TESTS = DIJSKTRA | BFS | BELLMAN | MATRIX | MATRIX_MEM;
    setup_benchmark("heatmap_regular");
    benchmark_heatmap(3,10,200,10,50,1000,10,1);
    close_benchmark();

    //
    TESTS = BELLMAN_PAR | BFS | BELLMAN | DIJSKTRA;
    setup_benchmark("th1to5_bellmanF_e100000_v1000");
    benchmark_threads(3,1000,100000,1,10);
    close_benchmark();

    TESTS = DIJSKTRA | BFS | BELLMAN | BELLMAN_PAR | MATRIX | MATRIX_MEM;
    setup_benchmark("v50to500_e500");
    benchmark_Efixe(3,50,500,50,500,3);
    close_benchmark();

    TESTS = DIJSKTRA | BFS | BELLMAN | BELLMAN_PAR | MATRIX | MATRIX_MEM;
    setup_benchmark("v50_e10to2500");
    benchmark_Vfixe(10,50,100,2500,100,3);
    close_benchmark();

    TESTS = DIJSKTRA | BFS | BELLMAN | BELLMAN_PAR;
    setup_benchmark("test_bellman_par");
    benchmark_Vfixe(3,1000,1000,1000,500,3);
    close_benchmark();
    
    /**/
}