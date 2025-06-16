#include "../src/gsum.h"

int main(int argc, char** argv) {
    int budget = atoi(argv[1]);
    double Ratio = std::stoi(argv[2]) / 100.0;
    GsumOld IGL_1(argv[3], budget);
    clock_t start, end;
    start = clock();
    IGL_1.IGL_PR(Ratio);
    end = clock();
    printf("%.2lf %.6lf %.8lf\n", Ratio, ((double)(end - start) / CLOCKS_PER_SEC), IGL_1.GCC_b_());
    return 0;
}