#include<vector>
#include<algorithm>
#include<cstdint>
class Dsu
{
private:
    std::vector<uint32_t > fa_;
    int n_;
public:
    Dsu(int n) {
        n_ = n;
        fa_.resize(n);
        Reset();
    }

    ~Dsu() {}

    uint32_t Find(int x) {
        if (fa_[x] != x)
            fa_[x] = Find(fa_[x]);
        return fa_[x];
    }
    
    //fa[rv] = ru;
    void Merge(int u, int v) {
        fa_[Find(v)] = Find(u);
    }

    void Reset() {
        for (int i = 0; i < n_; i++)
            fa_[i] = i;
    }
};
