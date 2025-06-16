#include "gsum.h"

void Gsum::MIS() {
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<int > bsort_order(G->n_, 1000000);
    // std::vector<int > bsort_rank(G->n_, 0);
    std::vector<int > bsort_bin(G->n_, 0);
    for (int i = 0; i < G->n_; i++)
        bsort_bin[G->deg_[i]]++;
    for (int i = 1; i < G->n_; i++)
        bsort_bin[i] += bsort_bin[i - 1];
    for (int i = G->n_ - 1; i > 0; i--)
        bsort_bin[i] = bsort_bin[i - 1];
    bsort_bin[0] = 0;
    for (int i = 0; i < G->n_; i++) {
        bsort_order[bsort_bin[G->deg_[i]]] = i;
        bsort_bin[G->deg_[i]]++;
    }
  
    for (auto u : bsort_order) {
        bool can_ = true;
        for (auto v : G->adj_list_[u])
            if (vis_[v]) {
                can_ = false;
                break;
            }
        if (can_) {
            cnt_++;
            rem_order_.push_back(u);
            GCC_[G->n_ - cnt_] = sz_[u] = 1;
            vis_[u] = true;
            tstamp_[u] = cnt_;
            if (cnt_ == G->n_ - budget_)
                return ;
        }
    }
    
    for (int u = 0; u < G->n_; u++) 
        if(!vis_[u]) {
            int score = 1;
            for (auto v : G->adj_list_[u])
                if(vis_[v])
                    score++;
            modify_index_[u] = heap_.push({score, u});
            real_score_[u] = score;
            tstamp_[u] = cnt_;
        }
    
}

void Gsum::IG() {
    // long long heap_modify_cnt = 0;
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, 0);
    while (cnt_ != G->n_ - budget_) {
        cnt_++;
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now = heap_.top();
        heap_.pop();
        int u = now.second;
        rem_order_.push_back(u);
        vis_[u] = true;
        GCC_[G->n_ - cnt_] = sz_[u] = now.first;
        std::vector<int > to_modify_;
        for (auto v : G->adj_list_[u])
            if (!vis_[v]) {
                if (!has_calc[v]) {
                    to_modify_.push_back(v);
                    real_score_[v] += sz_[u];
                    has_calc[v] = true;
                }
            }
            else if (dsu_->Find(v) != u) {
                int v_root = dsu_->Find(v);
                has_calc[v_root] = true;
                for (auto w : G->adj_list_[v_root]) 
                    if (!vis_[w]) {
                        if (!has_calc[w]) {
                            to_modify_.push_back(w);
                            real_score_[w] += sz_[u];
                            has_calc[w] = true;
                        }
                        real_score_[w] -= sz_[v_root];
                    }
                dsu_->Merge(u, v_root);
                G->adj_list_[v_root].clear();
                G->adj_list_[v_root].shrink_to_fit();
                G->deg_[v_root] = 0;
            }
        G->adj_list_[u].clear();
        G->deg_[u] = 0;
        // modify_time_.TimeStart();
        for (auto x : to_modify_) {
            // heap_.modify(modify_index_[x], {real_score_[x], x});
            heap_.update(modify_index_[x], {real_score_[x], x});
            // heap_modify_cnt ++;
            G->adj_list_[u].push_back(x);
            G->deg_[u]++;
            has_calc[x] = false;
        }
        // modify_time_.TimePause();
        if (cnt_ == G->n_ - budget_) {
            // printf("modify : %lld\n time : %.10lf\n", heap_modify_cnt, modify_time_.GetResult());
            return ;
        }
    }
}

void Gsum::IGL() {
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, false);
    while (cnt_ != G->n_ - budget_) {
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now = heap_.top();
        // printf("u_now = %d\n", now.second);
        heap_.pop();
        int u = now.second;
        bool need = false;
        if (tstamp_[u] != cnt_)
            for (auto v : G->adj_list_[u])
                if (vis_[v] && tstamp_[dsu_->Find(v)] > tstamp_[u]) {
                    need = true;
                    break;
                }
        if (need) {
            int score = 1;
            for (auto v : G->adj_list_[u])
                if(vis_[v]) {
                    int v_root = dsu_->Find(v);
                    if (has_calc[v_root])    continue;
                    score += sz_[v_root];
                    has_calc[v_root] = true;
                }
            for (auto v : G->adj_list_[u])
                if(vis_[v]) {
                    int v_root = dsu_->Find(v);
                    has_calc[v_root] = false;
                }
            heap_.push({score, u});
            tstamp_[u] = cnt_;
        }
        else {
            cnt_++;
            // printf("cnt=%d\n", cnt_);
            rem_order_.push_back(u);
            vis_[u] = true;
            GCC_[G->n_ - cnt_] = sz_[u] = now.first;
            tstamp_[u] = cnt_;
            for (auto v : G->adj_list_[u])
                if (vis_[v]) {
                    dsu_->Merge(u, v);
                }
            // int merge_root = u, root_sz_ = 1;
            // for (auto v : G->adj_list_[u])
            //     if (vis_[v]) {
            //         int v_root = dsu_->Find(v);
            //         if (sz_[v_root] > root_sz_) {
            //             root_sz_ = sz_[v_root];
            //             merge_root = v_root;
            //         }
            //     }
            // std::vector<int > adj_add_list;
            // for (auto v : G->adj_list_[u])
            //     if (vis_[v]) {
            //         int v_root = dsu_->Find(v);
            //         if (has_calc[v_root])    continue;
            //         has_calc[v_root] = true;
            //         if (v_root == merge_root)
            //             v_root = u;
            //         dsu_->Merge(merge_root, v_root);
            //         for (auto w : G->adj_list_[v_root])
            //             if (!vis_[w] && !has_calc[w]) {
            //                 adj_add_list.push_back(w);
            //                 has_calc[w] = true;
            //                 G->deg_[merge_root]++;
            //             }
            //     }
            // for (auto w : adj_add_list) {
            //     G->adj_list_[merge_root].push_back(w);
            //     has_calc[w] = false;
            // }
            // GCC_[G->n_ - cnt_] = sz_[merge_root] = now.first;
            // tstamp_[merge_root] = cnt_;
            // has_calc[merge_root] = false;
            if (cnt_ == G->n_ - budget_)
                return ;
        }
    }
}

void Gsum::IGLB() {
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, false);
    std::vector<int > update_list_;
    int score_now_ = 0;
    while (cnt_ != G->n_ - budget_) {
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now;
        if (heap_.empty() || score_now_ != heap_.top().first) {
            for (auto v : update_list_) {
                int score = 1;
                for (auto w : G->adj_list_[v])
                    if(vis_[w]) {
                        int w_root = dsu_->Find(w);
                        if (has_calc[w_root])    continue;
                        score += sz_[w_root];
                        has_calc[w_root] = true;
                    }
                for (auto w : G->adj_list_[v])
                    if(vis_[w]) {
                        int w_root = dsu_->Find(w);
                        has_calc[w_root] = false;
                    }
                heap_.push({score, v});
                tstamp_[v] = cnt_;
            }
            now = heap_.top();//////////////////////important
            update_list_.clear();
            score_now_ = now.first;
        }
        else
            now = heap_.top();
        heap_.pop();
        int u = now.second;
        bool need = false;
        for (auto v : G->adj_list_[u])
            if (vis_[v] && tstamp_[dsu_->Find(v)] > tstamp_[u]) {
                need = true;
                break;
            }
        if (need) {
            update_list_.push_back(u);
        }
        else {
            cnt_++;
            // printf("cnt=%d\n", cnt_);
            rem_order_.push_back(u);
            vis_[u] = true;
            GCC_[G->n_ - cnt_] = sz_[u] = now.first;
            tstamp_[u] = cnt_;
            for (auto v : G->adj_list_[u])
                if (vis_[v]) {
                    dsu_->Merge(u, v);
                }
            // int merge_root = u, root_sz_ = 1;
            // for (auto v : G->adj_list_[u])
            //     if (vis_[v]) {
            //         int v_root = dsu_->Find(v);
            //         if (sz_[v_root] > root_sz_) {
            //             root_sz_ = sz_[v_root];
            //             merge_root = v_root;
            //         }
            //     }
            // std::vector<int > adj_add_list;
            // for (auto v : G->adj_list_[u])
            //     if (vis_[v]) {
            //         int v_root = dsu_->Find(v);
            //         if (has_calc[v_root])    continue;
            //         has_calc[v_root] = true;
            //         if (v_root == merge_root)
            //             v_root = u;
            //         dsu_->Merge(merge_root, v_root);
            //         for (auto w : G->adj_list_[v_root])
            //             if (!vis_[w] && !has_calc[w]) {
            //                 adj_add_list.push_back(w);
            //                 has_calc[w] = true;
            //                 G->deg_[merge_root]++;
            //             }
            //     }
            // for (auto w : adj_add_list) {
            //     G->adj_list_[merge_root].push_back(w);
            //     has_calc[w] = false;
            // }
            // GCC_[G->n_ - cnt_] = sz_[merge_root] = now.first;
            // tstamp_[merge_root] = cnt_;
            // has_calc[merge_root] = false;
            if (cnt_ == G->n_ - budget_)
                return ;
        }
    }
}

void Gsum::Removal() {
    // printf("Removal...\n");
    dsu_->Reset();
    for (int u = 0; u < G->n_; u++)
        if (vis_[u])
            sz_[u] = 1;
        else
            sz_[u] = 0;
    for (int u = 0; u < G->n_; u++) {
        if (vis_[u]) {
            int u_root = dsu_->Find(u);
            for (auto v : G->adj_list_[u]) 
                if (vis_[v]){
                    int v_root = dsu_->Find(v);
                    if (u_root != v_root) {
                        sz_[u_root] += sz_[v_root];
                        dsu_->Merge(u_root, v_root);
                    }
                }
            tstamp_[u] = cnt_;
        }
    }
    heap_.clear();
    // heap_ = std::priority_queue<std::pair<int, int >, std::vector<std::pair<int, int > >, std::greater<std::pair<int, int > > > ();
    std::vector<bool > has_calc(G->n_, false);
    for (int u = 0; u < G->n_; u++)
        if (!vis_[u]) {
            int score = 1;
            for (auto v : G->adj_list_[u])
                if(vis_[v]) {
                    int v_root = dsu_->Find(v);
                    if (has_calc[v_root])    continue;
                    score += sz_[v_root];
                    has_calc[v_root] = true;
                }
            for (auto v : G->adj_list_[u])
                if(vis_[v]) {
                    int v_root = dsu_->Find(v);
                    has_calc[v_root] = false;
                }
            heap_.push({score, u});
            tstamp_[u] = cnt_;
        }
}

void Gsum::IGLB_PR(double ratio_) {
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, false);
    std::vector<int > update_list_;
    int score_now_ = 0;
    int threshold = 2;
    int deletenum = 0;
    while (cnt_ != G->n_ - budget_) {
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now;
        if (heap_.empty() || score_now_ != heap_.top().first) {
            for (auto v : update_list_) {
                int score = 1;
                for (auto w : G->adj_list_[v])
                    if(vis_[w]) {
                        int w_root = dsu_->Find(w);
                        if (has_calc[w_root])    continue;
                        score += sz_[w_root];
                        has_calc[w_root] = true;
                    }
                for (auto w : G->adj_list_[v])
                    if(vis_[w]) {
                        int w_root = dsu_->Find(w);
                        has_calc[w_root] = false;
                    }
                // assert(score_now_ < score);
                heap_.push({score, v});
                tstamp_[v] = cnt_;
            }
            now = heap_.top();//////////////////////important
            update_list_.clear();
            score_now_ = now.first;
        }
        else
            now = heap_.top();
        heap_.pop();
        int u = now.second;
        bool need = false;
        for (auto v : G->adj_list_[u])
            if (vis_[v] && tstamp_[dsu_->Find(v)] > tstamp_[u]) {
                need = true;
                break;
            }
        if (need) {
            update_list_.push_back(u);
        }
        else {
            cnt_++;
            deletenum++;
            // printf("cnt=%d\n", cnt_);
            vis_[u] = true;
            GCC_[G->n_ - cnt_] = sz_[u] = now.first;
            GCC_[G->n_ - cnt_] = std::max(GCC_[G->n_ - cnt_], GCC_[G->n_ - cnt_ + 1]);
            tstamp_[u] = cnt_;
            for (auto v : G->adj_list_[u])
                if (vis_[v]) {
                    dsu_->Merge(u, v);
                }
            // printf("score_now_ = %d threshold = %d\n", score_now_, threshold);
            if (score_now_ >= threshold) {
                for (int i = 0, j = 0; i < G->n_ && j < deletenum * ratio_; i++) {
                    if (vis_[i] == 1) {
                        vis_[i] = 0;
                        j++;
                        cnt_--;
                    }
                }
                deletenum = 0;
                Removal();
                if (!heap_.empty())
                    score_now_ = heap_.top().first;
                update_list_.clear();
                threshold *= 2;
            }
            if (cnt_ == G->n_ - budget_)
                return ;
        }
    }
}

void GsumOld::MIS() {
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<int > bsort_order(G->n_, 1000000);
    // std::vector<int > bsort_rank(G->n_, 0);
    std::vector<int > bsort_bin(G->n_, 0);
    for (int i = 0; i < G->n_; i++)
        bsort_bin[G->deg_[i]]++;
    for (int i = 1; i < G->n_; i++)
        bsort_bin[i] += bsort_bin[i - 1];
    for (int i = G->n_ - 1; i > 0; i--)
        bsort_bin[i] = bsort_bin[i - 1];
    bsort_bin[0] = 0;
    for (int i = 0; i < G->n_; i++) {
        bsort_order[bsort_bin[G->deg_[i]]] = i;
        bsort_bin[G->deg_[i]]++;
    }
  
    for (auto u : bsort_order) {
        bool can_ = true;
        for (auto v : G->adj_list_[u])
            if (vis_[v]) {
                can_ = false;
                break;
            }
        if (can_) {
            cnt_++;
            rem_order_.push_back(u);
            GCC_[G->n_ - cnt_] = sz_[u] = 1;
            vis_[u] = true;
            tstamp_[u] = cnt_;
            if (cnt_ == G->n_ - budget_)
                return ;
        }
    }
    
    for (int u = 0; u < G->n_; u++) 
        if(!vis_[u]) {
            int score = 1;
            for (auto v : G->adj_list_[u])
                if(vis_[v])
                    score++;
            modify_index_[u] = heap_.push({score, u});
            real_score_[u] = score;
            tstamp_[u] = cnt_;
        }
    
}

void GsumOld::IG() {
    // long long heap_modify_cnt = 0;
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, 0);
    while (cnt_ != G->n_ - budget_) {
        cnt_++;
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now = heap_.top();
        // printf("top ok\n");
        heap_.pop();
        // printf("pop ok\n");
        int u = now.second;
        rem_order_.push_back(u);
        vis_[u] = true;
        GCC_[G->n_ - cnt_] = sz_[u] = now.first;
        std::vector<int > to_modify_;
        for (auto v : G->adj_list_[u])
            if (!vis_[v]) {
                if (!has_calc[v]) {
                    to_modify_.push_back(v);
                    real_score_[v] += sz_[u];
                    has_calc[v] = true;
                }
            }
            else if (dsu_->Find(v) != u) {
                int v_root = dsu_->Find(v);
                has_calc[v_root] = true;
                for (auto w : G->adj_list_[v_root]) 
                    if (!vis_[w]) {
                        if (!has_calc[w]) {
                            to_modify_.push_back(w);
                            real_score_[w] += sz_[u];
                            has_calc[w] = true;
                        }
                        real_score_[w] -= sz_[v_root];
                    }
                dsu_->Merge(u, v_root);
                G->adj_list_[v_root].clear();
                G->adj_list_[v_root].shrink_to_fit();
                G->deg_[v_root] = 0;
            }
        G->adj_list_[u].clear();
        G->deg_[u] = 0;
        // modify_time_.TimeStart();
        for (auto x : to_modify_) {
            // heap_.modify(modify_index_[x], {real_score_[x], x});
            heap_.down(modify_index_[x], real_score_[x]);
            // heap_modify_cnt ++;
            G->adj_list_[u].push_back(x);
            G->deg_[u]++;
            has_calc[x] = false;
        }
        // modify_time_.TimePause();
        if (cnt_ == G->n_ - budget_) {
            // printf("modify : %lld\n time : %.10lf\n", heap_modify_cnt, modify_time_.GetResult());
            return ;
        }
    }
}

void GsumOld::IGL() {
    // long long heap_modify_cnt = 0;
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, false);
    while (cnt_ != G->n_ - budget_) {
        cnt_++;
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now = heap_.top();
        // modify_time_.TimeStart();
        while (real_score_[now.second] != now.first) {
            // heap_modify_cnt++;
            // assert(now.first < real_score_[now.second]);
            // heap_.modify(modify_index_[now.second], {real_score_[now.second], now.second});
            heap_.down(modify_index_[now.second], real_score_[now.second]);
            now = heap_.top();
        }
        // modify_time_.TimePause();
        heap_.pop();
        int u = now.second;
        rem_order_.push_back(u);
        vis_[u] = true;
        GCC_[G->n_ - cnt_] = sz_[u] = now.first;
        // modify_time_.TimeStart();
        std::vector<int > to_modify_;
        for (auto v : G->adj_list_[u])
            if (!vis_[v]) {
                if (!has_calc[v]) {
                    to_modify_.push_back(v);
                    real_score_[v] += sz_[u];
                    has_calc[v] = true;
                }
            }
            else if (dsu_->Find(v) != u) {
                int v_root = dsu_->Find(v);
                has_calc[v_root] = true;
                for (auto w : G->adj_list_[v_root]) 
                    if (!vis_[w]) {
                        if (!has_calc[w]) {
                            to_modify_.push_back(w);
                            real_score_[w] += sz_[u];
                            has_calc[w] = true;
                        }
                        real_score_[w] -= sz_[v_root];
                    }
                dsu_->Merge(u, v_root);
                G->adj_list_[v_root].clear();
                G->adj_list_[v_root].shrink_to_fit();
                G->deg_[v_root] = 0;
            }
        G->adj_list_[u].clear();
        G->deg_[u] = 0;
        for (auto x : to_modify_) {
            G->adj_list_[u].push_back(x);
            G->deg_[u]++;
            has_calc[x] = false;
        }
        // modify_time_.TimePause();
        if (cnt_ == G->n_ - budget_) {
            // printf("modify time : %.10lf\n", modify_time_.GetResult());
            // printf("modify : %d\n", heap_modify_cnt);
            return ;
        }
    }
}

int GsumOld::Removal() {
    // printf("%d\n", org_G.n_);
    int ret = 0;
    dsu_->Reset();
    for (int u = 0; u < org_G.n_; u++)
        if (vis_[u])
            sz_[u] = 1;
        else
            sz_[u] = 0;
    for (int u = 0; u < org_G.n_; u++) {
        G->adj_list_[u].clear();
        G->deg_[u] = 0;
        if (vis_[u]) {
            int u_root = dsu_->Find(u);
            for (auto v : org_G.adj_list_[u]) 
                if (vis_[v]) {
                    int v_root = dsu_->Find(v);
                    if (u_root != v_root) {
                        sz_[u_root] += sz_[v_root];
                        dsu_->Merge(u_root, v_root);
                    }
                }
            ret = std::max(ret, sz_[u_root]);
        }
    }
    heap_.clear();
    std::vector<bool > has_calc(G->n_, false);
    for (int u = 0; u < org_G.n_; u++) {
        if (!vis_[u]) {
            real_score_[u] = 1;
            for (auto v : org_G.adj_list_[u]) {
                if (vis_[v]) {
                    int v_root = dsu_->Find(v);
                    if (!has_calc[v_root]) {
                        G->adj_list_[v_root].push_back(u);
                        G->deg_[v_root]++;
                        G->adj_list_[u].push_back(v_root);
                        G->deg_[u]++;
                        real_score_[u] += sz_[v_root];
                        has_calc[v_root] = true;
                    }
                }
                else {
                    G->adj_list_[u].push_back(v);
                    G->deg_[u]++;
                }
            }
            for (auto v : org_G.adj_list_[u]) {
                if (vis_[v]) {
                    int v_root = dsu_->Find(v);
                    has_calc[v_root] = false;
                }
            }
            modify_index_[u] = heap_.push({real_score_[u], u});
        }
    }
    return ret;
}

void GsumOld::IGL_PR(double ratio_) {
    // long long heap_modify_cnt = 0;
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, false);
    int score_now_ = 0;
    int threshold = 2;
    int deletenum = 0;
    int lower_b_ = 0;
    while (cnt_ != G->n_ - budget_) {
        cnt_++;
        deletenum++;
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now = heap_.top();
        // modify_time_.TimeStart();
        while (real_score_[now.second] != now.first) {
            // heap_modify_cnt++;
            // assert(now.first < real_score_[now.second]);
            // heap_.modify(modify_index_[now.second], {real_score_[now.second], now.second});
            heap_.down(modify_index_[now.second], real_score_[now.second]);
            now = heap_.top();
        }
        // modify_time_.TimePause();
        heap_.pop();
        int u = now.second;
        score_now_ = now.first;
        rem_order_.push_back(u);
        vis_[u] = true;
        sz_[u] = now.first;
        GCC_[G->n_ - cnt_] = std::min(GCC_[G->n_ - cnt_], std::max(lower_b_, sz_[u]));
        // modify_time_.TimeStart();
        std::vector<int > to_modify_;
        for (auto v : G->adj_list_[u])
            if (!vis_[v]) {
                if (!has_calc[v]) {
                    to_modify_.push_back(v);
                    real_score_[v] += sz_[u];
                    has_calc[v] = true;
                }
            }
            else if (dsu_->Find(v) != u) {
                int v_root = dsu_->Find(v);
                for (auto w : G->adj_list_[v_root]) 
                    if (!vis_[w]) {
                        if (!has_calc[w]) {
                            to_modify_.push_back(w);
                            real_score_[w] += sz_[u];
                            has_calc[w] = true;
                        }
                        real_score_[w] -= sz_[v_root];
                    }
                dsu_->Merge(u, v_root);
                G->adj_list_[v_root].clear();
                G->adj_list_[v_root].shrink_to_fit();
                G->deg_[v_root] = 0;
            }
        G->adj_list_[u].clear();
        G->deg_[u] = 0;
        for (auto x : to_modify_) {
            G->adj_list_[u].push_back(x);
            G->deg_[u]++;
            has_calc[x] = false;
        }
        // modify_time_.TimePause();
        if (score_now_ >= threshold) {
            std::vector<std::pair<int, int > > to_sort;
            for (int i = 0; i < G->n_; i++)
                if (vis_[i])
                    to_sort.push_back({-org_G.deg_[i], i});
            std::sort(to_sort.begin(), to_sort.end());
            int has_del = 0;
            for (auto x : to_sort) {
                if (has_del >= deletenum * ratio_)
                    break;
                vis_[x.second] = 0;
                has_del++;
                cnt_--;
            }
            // for (int i = 0, j = 0; i < G->n_ && j < deletenum * ratio_; i++) {
            //     if (vis_[i] == 1) {
            //         vis_[i] = 0;
            //         j++;
            //         cnt_--;
            //     }
            // }
            deletenum = 0;
            lower_b_ = Removal();
            if (!heap_.empty())
                score_now_ = heap_.top().first;
            threshold *= 2;
        }
        if (cnt_ == G->n_ - budget_) {
            // printf("modify time : %.10lf\n", modify_time_.GetResult());
            // printf("modify : %d\n", heap_modify_cnt);
            return ;
        }
    }
}

void GsumOld::IGLL() {
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, false);
    while (cnt_ != G->n_ - budget_) {
        // printf("cnt_ = %d\n", cnt_);
        std::pair<int, int > now = heap_.top();
        // printf("u_now = %d\n", now.second);
        int u = now.second;
        bool need = false;
        if (tstamp_[u] != cnt_)
            for (auto v : G->adj_list_[u])
                if (vis_[v] && tstamp_[dsu_->Find(v)] > tstamp_[u]) {
                    need = true;
                    break;
                }
        if (need) {
            int score = 1;
            for (auto v : G->adj_list_[u])
                if(vis_[v]) {
                    int v_root = dsu_->Find(v);
                    if (has_calc[v_root])    continue;
                    score += sz_[v_root];
                    has_calc[v_root] = true;
                }
            for (auto v : G->adj_list_[u])
                if(vis_[v]) {
                    int v_root = dsu_->Find(v);
                    has_calc[v_root] = false;
                }
            heap_.down(modify_index_[u], score);
            tstamp_[u] = cnt_;
        }
        else {
            heap_.pop();
            cnt_++;
            // printf("cnt=%d\n", cnt_);
            rem_order_.push_back(u);
            vis_[u] = true;
            GCC_[G->n_ - cnt_] = sz_[u] = now.first;
            tstamp_[u] = cnt_;
            for (auto v : G->adj_list_[u])
                if (vis_[v]) {
                    dsu_->Merge(u, v);
                }
            if (cnt_ == G->n_ - budget_)
                return ;
        }
    }
}

void GsumOld::IGLB() {
    MIS();
    // printf("MIS\n");
    if (cnt_ == G->n_ - budget_)
        return ;

    std::vector<bool > has_calc(G->n_, false);
    std::vector<int > update_list_;
    int score_now_ = 0;
    while (cnt_ != G->n_ - budget_) {
        cnt_++;
        std::pair<int, int > now;
        while (!heap_.empty()) {
            now = heap_.top();
            if (real_score_[now.second] == now.first)
                break;
            update_list_.push_back(now.second);
            heap_.pop();
        }
        if (heap_.empty() || score_now_ != heap_.top().first) {
            for (auto v : update_list_) {
                heap_.push({real_score_[v], v});
                tstamp_[v] = cnt_;
            }
            now = heap_.top();//////////////////////important
            update_list_.clear();
            score_now_ = now.first;
        }
        heap_.pop();
        int u = now.second;
        rem_order_.push_back(u);
        vis_[u] = true;
        GCC_[G->n_ - cnt_] = sz_[u] = now.first;
        std::vector<int > to_modify_;
        for (auto v : G->adj_list_[u])
            if (!vis_[v]) {
                if (!has_calc[v]) {
                    to_modify_.push_back(v);
                    real_score_[v] += sz_[u];
                    has_calc[v] = true;
                }
            }
            else if (dsu_->Find(v) != u) {
                int v_root = dsu_->Find(v);
                has_calc[v_root] = true;
                for (auto w : G->adj_list_[v_root]) 
                    if (!vis_[w]) {
                        if (!has_calc[w]) {
                            to_modify_.push_back(w);
                            real_score_[w] += sz_[u];
                            has_calc[w] = true;
                        }
                        real_score_[w] -= sz_[v_root];
                    }
                dsu_->Merge(u, v_root);
                G->adj_list_[v_root].clear();
                G->adj_list_[v_root].shrink_to_fit();
                G->deg_[v_root] = 0;
            }
        G->adj_list_[u].clear();
        G->deg_[u] = 0;
        for (auto x : to_modify_) {
            G->adj_list_[u].push_back(x);
            G->deg_[u]++;
            has_calc[x] = false;
        }
        if (cnt_ == G->n_ - budget_)
            return ;
    }
}

void GsumOldWithTree::IGTinit() {
    MIS();
    std::vector<bool > has_calc(G->n_, false);
    while (cnt_ != G->n_) {
        cnt_++;
        std::pair<int, int > now = heap_.top();
        // modify_time_.TimeStart();
        while (real_score_[now.second] != now.first) {
            // heap_modify_cnt++;
            // assert(now.first < real_score_[now.second]);
            // heap_.modify(modify_index_[now.second], {real_score_[now.second], now.second});
            heap_.down(modify_index_[now.second], real_score_[now.second]);
            now = heap_.top();
        }
        // modify_time_.TimePause();
        heap_.pop();
        int u = now.second;
        rem_order_.push_back(u);
        vis_[u] = true;
        GCC_[G->n_ - cnt_] = sz_[u] = now.first;
        // modify_time_.TimeStart();
        std::vector<int > to_modify_;
        for (auto v : G->adj_list_[u])
            if (!vis_[v]) {
                if (!has_calc[v]) {
                    to_modify_.push_back(v);
                    real_score_[v] += sz_[u];
                    has_calc[v] = true;
                }
            }
            else if (dsu_->Find(v) != u) {
                int v_root = dsu_->Find(v);
                has_calc[v_root] = true;
                for (auto w : G->adj_list_[v_root]) 
                    if (!vis_[w]) {
                        if (!has_calc[w]) {
                            to_modify_.push_back(w);
                            real_score_[w] += sz_[u];
                            has_calc[w] = true;
                        }
                        real_score_[w] -= sz_[v_root];
                    }
                dsu_->Merge(u, v_root);
                G->adj_list_[v_root].clear();
                G->adj_list_[v_root].shrink_to_fit();
                G->deg_[v_root] = 0;
                
                //for tree
                tnode_[v_root].fa_ = u;
                tnode_[v_root].pre_bro_ = u;
                tnode_[v_root].nxt_bro_ = tnode_[u].fir_son_;
                if (tnode_[v_root].nxt_bro_ != -1)
                    tnode_[tnode_[v_root].nxt_bro_].pre_bro_ = v_root;
                tnode_[u].fir_son_ = v_root;
            }
        G->adj_list_[u].clear();
        G->deg_[u] = 0;
        for (auto x : to_modify_) {
            G->adj_list_[u].push_back(x);
            G->deg_[u]++;
            has_calc[x] = false;
        }
    }

    //vroot
    int las = -1;
    for (int i = 0; i < G->n_; i++) 
        if (tnode_[i].fa_ == -1) {
            tnode_[i].fa_ = root_;
            if (las != -1) {
                tnode_[i].nxt_bro_ = las;
                tnode_[las].pre_bro_ = i;
            }
            las = i;
        }
    tnode_[root_].fir_son_ = las;
    tnode_[las].pre_bro_ = root_;
    sz_[root_] = G->n_ + 1;
}

void GsumOldWithTree::IGLonTree(int b) {
    budget_ = b;
    cnt_ = 0;
    budget_r_ = 1.0 * budget_ / G->n_;
    //using heap
    boost::heap::pairing_heap<std::pair<int, int > > hp;
    for (int  v = tnode_[root_].fir_son_; v != -1; v = tnode_[v].nxt_bro_) {
        hp.push({sz_[v], v});
    }
    for (int i = 0; i < b; i++) {
        int u = hp.top().second;
        hp.pop();
        GCC_[i] = sz_[u];
        for (int  v = tnode_[u].fir_son_; v != -1; v = tnode_[v].nxt_bro_) {
            hp.push({sz_[v], v});
        }
    }
    GCC_[b] = hp.top().first;
    printf("%d\n", GCC_[b]);
}

void GsumOldWithTree::LinkonTree(int u, int v) {

    // std::cerr << "check -1" << std::endl;
    // std::cerr << root_ << std::endl;
    // for(int i = 0; i < G->n_; i++)
    //     assert(tnode_[i].fa_ != -1);

    org_G.m_++;
    org_G.adj_list_[u].push_back(v);
    org_G.adj_list_[v].push_back(u);
    org_G.deg_[u]++;
    org_G.deg_[v]++;

    int w = -1, lsz_u = 0, lsz_v = 0;
    while (u != v) {
        // std::cerr << u << ' ' << v << ' ' << w <<std::endl;
        // assert(u != -1);
        // assert(v != -1);
        if (sz_[u] - lsz_u > sz_[v] -lsz_v) {
            std::swap(u, v);
            std::swap(lsz_u, lsz_v);
        }
        lsz_u = sz_[u];
        if (w != -1) {
            tnode_[w].fa_ = u;
            tnode_[w].pre_bro_ = u;
            tnode_[w].nxt_bro_ = tnode_[u].fir_son_;
            tnode_[u].fir_son_ = w;
            if (tnode_[w].nxt_bro_ != -1)
                tnode_[tnode_[w].nxt_bro_].pre_bro_ = w;
            sz_[u] += lsz_v;
        }
        w = u;
        u = tnode_[u].fa_;
        if (tnode_[w].nxt_bro_ != -1)
            tnode_[tnode_[w].nxt_bro_].pre_bro_ = tnode_[w].pre_bro_;
        if (tnode_[w].pre_bro_ == u)
            tnode_[u].fir_son_ = tnode_[w].nxt_bro_;
        else
            tnode_[tnode_[w].pre_bro_].nxt_bro_ = tnode_[w].nxt_bro_;
        tnode_[w].nxt_bro_ = tnode_[w].pre_bro_= tnode_[w].fa_ = -1;
    }

    tnode_[w].fa_ = u;
    tnode_[w].pre_bro_ = u;
    tnode_[w].nxt_bro_ = tnode_[u].fir_son_;
    tnode_[u].fir_son_ = w;
    if (tnode_[w].nxt_bro_ != -1)
        tnode_[tnode_[w].nxt_bro_].pre_bro_ = w;
}

void GsumOldWithTree::OrderonTree() {
    std::vector<std::vector<int > > bin(G->n_ + 1);
    int GCC_now = G->n_;
    for (int  v = tnode_[root_].fir_son_; v != -1; v = tnode_[v].nxt_bro_) {
        bin[sz_[v]].push_back(v);
    }
    for (int i = 0; i < G->n_; i++) {
        // std::cerr << i << ' ' << G->n_ << ' ' << GCC_now <<std::endl;
        while(!bin[GCC_now].size()) GCC_now--;
        int u = bin[GCC_now].back();
        bin[GCC_now].pop_back();
        GCC_[i] = sz_[u];
        for (int  v = tnode_[u].fir_son_; v != -1; v = tnode_[v].nxt_bro_) {
            bin[sz_[v]].push_back(v);
        }
    }
    GCC_[G->n_] = 0;
}

void GsumOldWithTree::CutonTree(const std::vector<std::pair<int, int > >&edges) {
    org_G.m_-= edges.size();
    for (auto e : edges) {
        int u = e.first, v = e.second;
        for (int i = 0; ; i++)
            if(org_G.adj_list_[u][i] == v) {
                org_G.adj_list_[u][i] = org_G.adj_list_[u][org_G.adj_list_[u].size() - 1];
                org_G.adj_list_[u].pop_back();
                break;
            }
        for (int i = 0; ; i++)
            if(org_G.adj_list_[v][i] == u) {
                org_G.adj_list_[v][i] = org_G.adj_list_[v][org_G.adj_list_[v].size() - 1];
                org_G.adj_list_[v].pop_back();
                break;
            }
    }
    // std::cerr << "here!\n";
    dsu_->Reset();
    // std::cerr << "here!!\n";
    dfs(root_);
    // std::cerr << "here!!!\n";
}

void GsumOldWithTree::dfs(int now) {
    // assert(dfs_vis_[now] == false);
    dfs_vis_[now] = true;
    for (int u = tnode_[now].fir_son_; u != -1; u = tnode_[u].nxt_bro_) {
        dfs(u);
        tnode_[u].fa_ = -1;
    }
    tnode_[now].fir_son_ = -1;
    if (now != root_) {
        sz_[now] = 1;
        for (auto u : org_G.adj_list_[now]) {
            if (dfs_vis_[u]) continue;
            int r_u = dsu_->Find(u);
            if (r_u != now) {
                // assert(tnode_[r_u].fa_  == -1);
                tnode_[r_u].pre_bro_ = tnode_[r_u].fa_ = now;
                tnode_[r_u].nxt_bro_ = tnode_[now].fir_son_;
                if (tnode_[r_u].nxt_bro_ != -1)
                    tnode_[tnode_[r_u].nxt_bro_].pre_bro_ = r_u;
                tnode_[now].fir_son_ = r_u;
                dsu_->Merge(now, r_u);
                sz_[now] += sz_[r_u];
                // assert(sz_[now] <= G->n_);
            }
        }
    }
    else {
        for (int i = 0; i < org_G.n_; i++) {
            if (tnode_[i].fa_ == -1) {
                tnode_[i].fa_ = tnode_[i].pre_bro_ = now;
                tnode_[i].nxt_bro_ = tnode_[now].fir_son_;
                tnode_[now].fir_son_ = i;
                if (tnode_[i].nxt_bro_ != -1)
                    tnode_[tnode_[i].nxt_bro_].pre_bro_ = i;
            }
        }
        sz_[now] = G->n_ + 1;
    } 
    dfs_vis_[now] = false;
}