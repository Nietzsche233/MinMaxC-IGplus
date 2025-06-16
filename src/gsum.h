#include "graph.h"
#include "dsu.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <random>
// #include <ranges>
// #include <ext/pb_ds/priority_queue.hpp>
#include <boost/heap/pairing_heap.hpp>
// #include <boost/heap/fibonacci_heap.hpp>
#include "binheap.h"
// using my_heap_ = boost::heap::pairing_heap<std::pair<int, int >, boost::heap::compare<std::greater<std::pair<int, int > > > >;
// using heap_handle_t_ = my_heap_::handle_type;
// using my_heap_ = __gnu_pbds::priority_queue<std::pair<int, int >, std::greater<std::pair<int, int > >, __gnu_pbds::pairing_heap_tag >;
// using heap_handle_t_ = my_heap_::point_iterator;


class Gsum
{
protected:
    Graph *G, org_G;
    Dsu* dsu_;
    std::vector<int > rem_order_;
    std::vector<bool > vis_;
    std::vector<int > sz_;
    std::vector<uint32_t> tstamp_;
    std::vector<int > real_score_;
    int budget_, cnt_;
    double budget_r_;
    std::vector<int > GCC_;
    // __gnu_pbds::priority_queue<std::pair<int, int >, std::greater<std::pair<int, int > >, __gnu_pbds::pairing_heap_tag > heap_;
    boost::heap::pairing_heap<std::pair<int, int >, boost::heap::compare<std::greater<std::pair<int, int > > > > heap_;
    std::vector<boost::heap::pairing_heap<std::pair<int, int >, boost::heap::compare<std::greater<std::pair<int, int > > > >::handle_type > modify_index_;
    void Removal();

public:
    Gsum(const char* filename, int b) {
        budget_ = b;
        cnt_ = 0;
        G = new Graph(filename);
        org_G = *G;
        budget_r_ = 1.0 * budget_ / G->n_;
        dsu_ = new Dsu(G->n_);
        sz_.resize(G->n_ + 1, 0);
        tstamp_.resize(G->n_, 0);
        GCC_.resize(G->n_ + 1, G->n_);
        vis_.resize(G->n_, false);
        real_score_.resize(G->n_);
        modify_index_.resize(G->n_);
    }
    Gsum() {
        delete G;
    }

    void MIS();
    void IG();
    void IGL();
    void IGLB();
    void IGLB_PR(double ratio_);
    // void IG_PR();
    void GCC_b_output(const char* filename) {
        if (filename == "") {
            printf("budget = %d, budget ratio = %.6lf\n", budget_, budget_r_);
            printf("|GCC| = %d, |GCC| / |V| = %.8lf\n", GCC_[budget_], 1.0 * GCC_[budget_] / G->n_);
        }
        else {
            FILE* fp = fopen(filename, "w");
            if (fp == NULL) {
                printf("file open error!\n");
                exit ;
            }
            fprintf(fp, "budget = %d, budget ratio = %.6lf\n", budget_, budget_r_);
            fprintf(fp, "|GCC| = %d, |GCC| / |V| = %.8lf\n", GCC_[budget_], 1.0 * GCC_[budget_] / G->n_);
        }
    }

    double GCC_b_() {
        return 1.0 * GCC_[budget_] / G->n_;
    }

    void GCC_all_output(const char* filename) {
        FILE* fp = fopen(filename, "w");
        if (fp == NULL) {
            printf("file open error!\n");
            exit ;
        }
        for (auto GCC_sz : GCC_) {
            fprintf(fp, "%.8lf\n", 1.0 * GCC_sz / G->n_);
        }
    }
};

class GsumOld:public Gsum {
    using my_heap_ = Binheap;
    using heap_handle_t_ = int;
protected:
    my_heap_ heap_;
    std::vector<heap_handle_t_ > modify_index_;
    // __gnu_pbds::priority_queue<std::pair<int, int >, std::greater<std::pair<int, int > >, __gnu_pbds::pairing_heap_tag > heap_;
    // std::vector<__gnu_pbds::priority_queue<std::pair<int, int >, std::greater<std::pair<int, int > >, __gnu_pbds::pairing_heap_tag >::point_iterator > modify_index_;
    std::vector<std::vector<int > > update_edg_;
public:
    GsumOld(const char* filename, int b) : 
        Gsum(filename, b)
        , heap_(G->n_) 
        {
        update_edg_.resize(G->n_);
        modify_index_.resize(G->n_);
    }

    struct Timer {
        clock_t start, end;
        double res_;
        Timer(){
            res_ = 0;
        }
        void TimeStart() {
            start = clock();
        }
        void TimePause() {
            end = clock();
            res_ += ((double)(end - start) / CLOCKS_PER_SEC);
        }
        double GetResult() {
            return res_;
        }
    };

    Timer modify_time_;

    void MIS();
    void IG();
    void IGL();
    int Removal();
    void IGL_PR(double ratio_);
    void IGLL();
    void IGLB();
};

class TreeNode {
public:
    int pre_bro_, nxt_bro_, fir_son_;
    int fa_;
    TreeNode() {
        fa_ = pre_bro_ = nxt_bro_ = fir_son_ = -1;
    }
};

class GsumOldWithTree:public GsumOld {
protected:
    std::vector<TreeNode > tnode_;
    std::vector<bool > dfs_vis_;
    int root_;

    void dfs(int now);

public:
GsumOldWithTree(const char* filename, int b) : 
    GsumOld(filename, b)
    {
        root_ = G->n_;
        tnode_.resize(G->n_ + 1);
        dfs_vis_.resize(G->n_ + 1);
    }
    void IGTinit();
    void IGLonTree(int b);
    void LinkonTree(int u, int v);
    void CutonTree(const std::vector<std::pair<int, int > >&edges);
    void OrderonTree();
};