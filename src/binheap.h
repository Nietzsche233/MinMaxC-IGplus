#include<vector>
#include<cstdio>
// #include <ext/pb_ds/priority_queue.hpp>
#include <boost/heap/pairing_heap.hpp>
// #include <boost/heap/fibonacci_heap.hpp>


// class Binheap {
// private:
//     struct LinkList {
//         int _;
//         LinkList *las, *nxt;
//     };
//     //_ : id;
//     std::vector<LinkList*> val_index_;
//     //first : value; second : linklistptr
//     std::vector<std::pair<int, LinkList* > > pos_;
//     int size_;
//     int top_ptr;
//     int value_;
// public:
//     Binheap(int value) {
//         value_ = value;
//         val_index_.resize(value_ + 2);
//         pos_.resize(value_ + 2);
//         for (int i = 0; i <= value_ + 1; i++)
//             val_index_[i] = new LinkList;
//         for (int i = 1; i <= value_; i++) {
//             val_index_[i]->nxt = val_index_[i + 1];
//             val_index_[i]->las = val_index_[i - 1];
//         }
//         val_index_[0]->nxt = val_index_[1];
//         val_index_[value_ + 1]->las = val_index_[value_];
//         size_ = top_ptr = 0;
//     }
//     ~Binheap() {
//     }
//     std::pair<int, int > top() {
//         while(val_index_[top_ptr]->nxt == val_index_[top_ptr + 1])
//             top_ptr++;
//         return {top_ptr, val_index_[top_ptr]->nxt->_};
//     }
//     void pop() {
//         while(val_index_[top_ptr]->nxt == val_index_[top_ptr + 1])
//             top_ptr++;
//         LinkList* need_to_del_ = val_index_[top_ptr]->nxt;
//         need_to_del_->las->nxt = need_to_del_->nxt;
//         need_to_del_->nxt->las = need_to_del_->las;
//         size_--;
//     }
//     int push(std::pair<int, int > x) {
//         int this_index_ = x.second;
//         if (pos_[this_index_].second == NULL) {
//             pos_[this_index_].second = new LinkList;
//             pos_[this_index_].second->_ = x.second;
//         }
//         pos_[this_index_].first = x.first;
//         LinkList *this_ptr = pos_[this_index_].second;
//         this_ptr->las = val_index_[x.first];
//         this_ptr->nxt = val_index_[x.first]->nxt;
//         this_ptr->las->nxt = this_ptr;
//         this_ptr->nxt->las = this_ptr;
//         size_++;
//         if (top_ptr > x.first)  top_ptr = x.first;
//         return this_index_;
//     }
//     void down(int pos_index, int new_value) {
//         if (pos_[pos_index].first == new_value)
//             return;
//         pos_[pos_index].first = new_value;
//         LinkList *this_ptr = pos_[pos_index].second;
//         this_ptr->las->nxt = this_ptr->nxt;
//         this_ptr->nxt->las = this_ptr->las;

//         this_ptr->las = val_index_[new_value];
//         this_ptr->nxt = val_index_[new_value]->nxt;
//         this_ptr->las->nxt = this_ptr;
//         this_ptr->nxt->las = this_ptr;
//     }
//     bool empty() {
//         return size_ == 0;
//     }
//     void clear() {
//         for (int i = 1; i <= value_; i++) {
//             val_index_[i]->nxt = val_index_[i + 1];
//             val_index_[i]->las = val_index_[i - 1];
//         }
//         val_index_[0]->nxt = val_index_[1];
//         val_index_[value_ + 1]->las = val_index_[value_];
//         size_ = top_ptr = 0;
//     }
// };
//// A backet list with strict linear memory, but with constant level performance differences, which may be a deficiency in implementation.

class BinheapPlus {
private:
    //first : second value, second : id
    std::vector<boost::heap::pairing_heap<std::pair<double, int >, boost::heap::compare<std::greater<std::pair<double, int > > > > > val_index_;
    //first : value; second : handle
    std::vector<std::pair<int, boost::heap::pairing_heap<std::pair<double, int >, boost::heap::compare<std::greater<std::pair<double, int > > > >::handle_type > > pos_;
    int size_;
    int top_ptr;
public:
    BinheapPlus(int value_) {
        val_index_.resize(value_ + 2);
        size_ = top_ptr = 0;
    }
    ~BinheapPlus() {
    }
    std::pair<std::pair<int, double >, int > top() {
        while(val_index_[top_ptr].empty())
            top_ptr++;
        std::pair<int, double > x = val_index_[top_ptr].top();
        return {{top_ptr, x.first}, x.second};
    }
    void pop() {
        while(val_index_[top_ptr].empty())
            top_ptr++;
        val_index_[top_ptr].pop();
        size_--;
    }
    int push(std::pair<std::pair<int, double >, int > x) {
        auto handal_ = val_index_[x.first.first].push({x.first.second, x.second});
        int this_index_ = pos_.size();
        pos_.push_back({x.first.first, handal_});
        size_++;
        if (top_ptr > x.first.first)  top_ptr = x.first.first;
        return this_index_;
    }
    void down(int pos_index, std::pair<int, double > new_value) {
        // if (pos_[pos_index].first == new_value)
        //     return;
        int id_ = pos_[pos_index].second.node_->value.second;
        val_index_[pos_[pos_index].first].erase(pos_[pos_index].second);
        pos_[pos_index].first = new_value.first;
        pos_[pos_index].second = val_index_[new_value.first].push({new_value.second, id_});
    }
    bool empty() {
        return size_ == 0;
    }
};


class Binheap {
private:
    std::vector<std::vector<std::pair<int ,int > > > _;
    //first : pos_index; second : id;
    std::vector<std::pair<int ,int > > pos_;
    //first : value; second : pos_
    int size_;
    int top_ptr;
    int value_;
public:
    Binheap(int value) {
        value_ = value;
        _.resize(value + 1);
        size_ = top_ptr = 0;
    }
    ~Binheap() {
    }
    std::pair<int, int > top() {
        while(_[top_ptr].size() == 0)
            top_ptr++;
        return {top_ptr, _[top_ptr][_[top_ptr].size() - 1].second};
    }
    void pop() {
        while(_[top_ptr].size() == 0)
            top_ptr++;
        _[top_ptr].pop_back();
        size_--;
    }
    int push(std::pair<int, int > x) {
        int this_index_ = pos_.size(), this_pos_ = _[x.first].size();
        pos_.push_back({x.first, this_pos_});
        _[x.first].push_back({this_index_, x.second});
        size_++;
        if (top_ptr > x.first)  top_ptr = x.first;
        return this_index_;
    }
    void down(int pos_index, int new_value) {
        std::pair<int, int > old_pos = pos_[pos_index];
        if (old_pos.first == new_value)
            return;
        int this_id = _[old_pos.first][old_pos.second].second;
        pos_[pos_index] = {new_value, _[new_value].size()};
        _[new_value].push_back({pos_index, this_id});
        int old_value = old_pos.first;
        if (_[old_value].size() - 1 != old_pos.second) {
            pos_[_[old_value][_[old_value].size() - 1].first] = old_pos;
            std::swap(_[old_value][old_pos.second], _[old_value][_[old_value].size() - 1]);
        }
        _[old_value].pop_back();
    }
    bool empty() {
        return size_ == 0;
    }

    void clear() {
        for (int i = 1; i <= value_; i++) {
            _[i].clear();
        }
        pos_.resize(0);
        size_ = top_ptr = 0;
    }
};
////This is a rough implementation of the back list based on vectors, which has superior time performance despite not guaranteeing space overhead.