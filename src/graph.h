#include<vector>
#include<algorithm>
#include<cstdint>
#include<string>
#include<cassert>
#include<set>
#include<iostream>
class Graph
{
private:

public:

    //|V|, |E|
    uint32_t n_, m_;
    std::vector<std::vector<uint32_t > > adj_list_;
    std::vector<uint32_t > deg_;

    Graph(){}

    Graph(const char* filename) {
        FILE* fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("file open error!\n");
            exit ;
        }
        fscanf(fp, "%d %d", &n_, &m_);
        std::cerr << n_ << ' ' << m_ << std::endl;
        adj_list_.resize(n_);
        deg_.resize(n_, 0);
        // std::set<std::pair<int, int > > _;
        for (int i = 0; i < m_; i++) {
            int j, k;
            fscanf(fp, "%d %d", &j, &k);
            // assert(j != k);
            // if (j > k) std::swap(j, k);
            // assert(_.find({j, k}) == _.end());
            // _.insert({j, k});
            assert(j < n_);
            assert(k < n_);
            adj_list_[j].push_back(k);
            adj_list_[k].push_back(j);
            deg_[j]++;
            deg_[k]++;
        }
    }

    ~Graph() {

    }
};
