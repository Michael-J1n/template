#include <bits/stdc++.h>
using namespace std;

/**
 * 懒删除堆，支持元素的删除操作
 * cmp 使用 less<int> 为大根堆，greater<int> 为小根堆
 * 前置条件：top/pop 只在逻辑堆非空时调用；erase 只删除当前存在的元素
 * */
template <class Cmp>
struct LazyHeap {
    using ll = long long;
    priority_queue<int, vector<int>, Cmp> pq;
    unordered_map<int, int> erased;
    unordered_map<int, int> cnt;
    void prune() {
        while (!pq.empty()) {
            ll x = pq.top();
            auto it = erased.find(x);
            if (it != erased.end() && it->second > 0) {
                if (--it->second == 0) erased.erase(it);
                pq.pop();
            }
            else break;
        }
    }
    void push(int x) {
        pq.emplace(x);
        ++cnt[x];
    }
    bool erase(int x) {
        auto it = cnt.find(x);
        if (it == cnt.end() || it->second == 0) return false;
        if (--it->second == 0) cnt.erase(it);
        ++erased[x];
        return true;
    }
    void pop() {
        prune();
        if (pq.empty()) return;
        ll x = pq.top(); pq.pop();
        auto it = cnt.find(x);
        if (it != cnt.end() && --it->second == 0) cnt.erase(it);
    }
    int top() {
        prune();
        return pq.top();
    }
    bool empty() {
        prune();
        return cnt.empty();
    }
};
