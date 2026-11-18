#include <bits/stdc++.h>
using namespace std;

/**
 * 差分树状数组
 * 
 * 支持区间加/异或，以及单点求值
 * 若区间加，则 f=a+b,inv=-a；区间异或，则 f=a^b,inv=a.
 * 
 * 复杂度：查询修改均 O(log n)
 */
template<class T>
struct Fenwick {
    int n;
    vector<T> tr;
    T f(T a, T b) {
        return a + b;
    }
    T inv(T a) {
        return -a;
    }
    Fenwick(int n) : n(n), tr(n + 1, 0) {}
    Fenwick(const vector<T>& a) : n(a.size()), tr(n + 1, 0) {
        for (int i = 0; i < n; ++i) {
            tr[i + 1] = f(tr[i + 1], i ? f(a[i], inv(a[i - 1])) : a[i]);
            int j = i + 1 + ((i + 1) & -(i + 1));
            if (j <= n) tr[j] = f(tr[j], tr[i + 1]);
        }
    }
    void add(int x, T v) { for (++x; x <= n; x += x & -x) tr[x] = f(tr[x], v); }
    void update(int l, int r, T v) { add(l, v), add(r + 1, inv(v)); }
    T query(int x) {
        T res = 0;
        for (++x; x > 0; x -= x & -x) res = f(res, tr[x]);
        return res;
    }
};