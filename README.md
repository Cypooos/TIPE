# TIPE

## Part 1 : `matrix_processor`

 - Creation of a `MatrixProcessor` class, and interpretor of a matrix-register based assembly language
 - Implementation of a distance algorithm (MATRIX) that given a graph matrix give the shortest distance beetween vertice 0 and 1
- `distance.py` is used to show the numbers of matrix products used by the MATRIX algorithm and show experimentally that the complexity is $O((\log_2(n))^2)$, and the advantage of dynamic programmation

## Part 2 : `parallel`
 - Creation of a benchmarking system in C for differents find-the-distance algorithms
 - Implementation of MATRIX and MATRIX-MEM in C
 - Implementation of parralized Bellman-Ford
 - Folder `graph` cointain the code to generate the result images from `imgs`

Theoretically, using message passing, the complexity of MATRIX is $O(n(\log_2(n))^2)$, witch is better than bellmanford for the case of graphs with a lot of edges compared to the number of verticices.