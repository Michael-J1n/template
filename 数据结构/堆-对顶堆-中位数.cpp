#include <bits/stdc++.h>
using namespace std;

/**
 * 动态维护数据中位数
 * 
 * small 是左半边，大根堆，large 是右半边，小根堆
 * small.top == 左中位数，large.top == 右中位数
 * 不带删除，保证：sz1 == sz2 或 sz1 == sz2 + 1
 * distance_sum()：所有元素到左中位数的绝对距离和
 * 
 * 插入 O(log n)；中位数与 distance_sum 查询 O(1)
 * 前置条件：median_l / median_r / distance_sum 只在结构非空时调用
 */
template <class T>
struct DualHeap {
    priority_queue<T> small;
    priority_queue<T, vector<T>, greater<T>> large;
    long long sum1 = 0, sum2 = 0;
    int sz1 = 0, sz2 = 0;
    void adjust() {
        while (sz1 < sz2) {
            T x = large.top(); large.pop();
            sum2 -= x, sum1 += x;
            --sz2, ++sz1;
            small.push(x);
        }
        while (sz1 > sz2 + 1) {
            T x = small.top(); small.pop();
            sum1 -= x, sum2 += x;
            --sz1, ++sz2;
            large.push(x);
        }
    }
    void emplace(T x) {
        if (!small.empty() && x <= small.top()) {
            small.push(x);
            sum1 += x;
            ++sz1;
        } else {
            large.push(x);
            sum2 += x;
            ++sz2;
        }
        adjust();
    }
    int size() { return sz1 + sz2; }
    T median_l() { return small.top(); }
    T median_r() { return sz1 == sz2 ? large.top() : small.top(); }
    long long distance_sum() {
        long long mid = small.top();
        return mid * sz1 - sum1 + sum2 - mid * sz2;
    }
};
