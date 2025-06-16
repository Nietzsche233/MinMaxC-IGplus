#include "../src/gsum.h"

int main(int argc, char** argv) {
    int budget = atoi(argv[1]);

    printf("*****************************************************************\n");
    printf("the original graph: %s\n", argv[2]);
    printf("budget is %d\n", budget);
    printf("*****************************************************************\n");

    GsumOld IGL_1(argv[2], budget);
    printf("init finish\n");
    clock_t start, end;
    start = clock();
    IGL_1.IGL_PR(0.01);
    end = clock();
    printf("Use Time: %.6f\n", ((double)(end - start) / CLOCKS_PER_SEC));

    IGL_1.GCC_all_output(argv[3]);
    if (argc == 4) 
        IGL_1.GCC_b_output("");
    else if (argc == 5) 
        IGL_1.GCC_b_output(argv[4]);
    return 0;
}