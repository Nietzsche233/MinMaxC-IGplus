#include "../src/gsum.h"

int main(int argc, char** argv) {
    int budget = atoi(argv[1]);

    printf("*****************************************************************\n");
    printf("the original graph: %s\n", argv[2]);
    printf("MaxD \n");
    printf("*****************************************************************\n");

    Graph *G = new Graph(argv[2]);

    int max_degree = 0;

    for (auto x : G->deg_)
        max_degree = max_degree > x ? max_degree : x;

    printf("%d\n", max_degree);
    return 0;
}