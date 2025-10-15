# ADSA Assignment 4

My first attempt was `main_old.cpp`. This used Prim's Algorithm and Kruskal's algorithm sequentially. I felt it was inefficient as the data structure had to support both algorithms, and both had to be built to work properly.

My second attempt is `main.cpp`. This algorithm only uses the union-find data structure, with only very minimal modifications. On the first pass, the algorithm calculates the cost of removing the least expensive back-edges from existing components. In the second pass it find the minimum cost edges that will connect components. It uses Kruskal's algorithm to do both, and in my opinion, is more elegant as both passes follow the same flow, and does not have two different data structures combined.