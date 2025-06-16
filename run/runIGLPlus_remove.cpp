#include "../src/gsum.h"

int main(int argc, char** argv) {
    int delta_sz = atoi(argv[1]);
    printf("*****************************************************************\n");
    printf("the original graph: %s\n", argv[2]);
    printf("the remove edges: %s\n", argv[3]);
    printf("*****************************************************************\n");

    GsumOldWithTree IG_1(argv[2], 0);
    printf("init finish\n");
    
    clock_t start_b, end_b;
    start_b = clock();
    
    IG_1.IGTinit();
    
    end_b = clock();
    
    printf("Build tree Use Time: %.6f\n", ((double)(end_b - start_b) / CLOCKS_PER_SEC));
    
    FILE* fp = fopen(argv[3], "r");
    if (fp == NULL) {
        printf("file open error!\n");
        exit ;
    }
    std::vector<std::pair<int, int > > edges(delta_sz);
    for(int i = 0; i < delta_sz; i++) {
        fscanf(fp, "%d %d", &edges[i].first, &edges[i].second);
    }
    
    clock_t start_i, end_i, end_o;
    start_i = clock();
    
    IG_1.CutonTree(edges);

    // for (auto e : edges) {
    //     // std::cerr << e.first << ' ' << e.second << std::endl;
    //     IG_1.CutonTree(e.first,e.second);
    // }
    
    end_i = clock();
    
    IG_1.OrderonTree();
    
    end_o = clock();

    printf("Remval Use Time: %.6f\n", ((double)(end_i - start_i) / CLOCKS_PER_SEC));

    printf("Remval and Order Use Time: %.6f\n", ((double)(end_o - start_i) / CLOCKS_PER_SEC));

    IG_1.GCC_all_output(argv[4]);

    return 0;
}