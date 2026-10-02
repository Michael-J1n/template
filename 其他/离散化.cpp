#include <bits/stdc++.h>
using namespace std;

/**
 * xs：离散化值域
 * get(x): 获得值在离散化数组中对应的 id
 * val(id): 使用 id 反推原值
 * size()：值域大小
 */

template<class T>
struct Disc {
    vector<T> xs;
    Disc(const vector<T>& a) {
        xs = a;
        ranges::sort(xs);
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
    }
    int get(const T& x) {
        return lower_bound(xs.begin(), xs.end(), x) - xs.begin();
    }
    T val(int id) {
        return xs[id];
    }
    int size() {
        return xs.size();
    }
};