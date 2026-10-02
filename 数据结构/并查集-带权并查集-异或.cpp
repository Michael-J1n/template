#include <bits/stdc++.h>
using namespace std;

/**
 * 带权并查集；维护异或关系
 * val[x] ^ w[x] = val[find(x)]
 *
 * unite(x, y, wt) 代表 val[x] ^ wt = val[y]（加入 x--(wt)->y 边）
 * dis(x, y) 返回 x -> y 的距离（val[x] ^ dis(x, y) = val[y]）
 * dis(x) 返回 x -> root 的距离
 */
struct WeightedUFS {
    vector<int> pa, sz;
    vector<int> w;
    int cc;
    WeightedUFS(int n) {
        cc = n;
        pa.resize(n), sz.resize(n), w.resize(n);
        for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1, w[i] = 0;
    }
    int find(int x) {
        if (pa[x] == x) return x;
        int p = pa[x];
        pa[x] = find(p);
        w[x] ^= w[p];
        return pa[x];
    }
    bool unite(int x, int y, int wt) {
        int px = find(x), py = find(y);
        if (px == py) return (w[x] ^ w[y]) == wt;
        int d = w[x] ^ w[y] ^ wt;
        --cc;
        if (sz[px] > sz[py]) swap(px, py);
        pa[px] = py;
        w[px] = d;
        sz[py] += sz[px];
        return true;
    }
    bool check(int x, int y) {
        return find(x) == find(y);
    }
    int dis(int x, int y) {
        int px = find(x), py = find(y);
        assert(px == py);
        return w[x] ^ w[y];
    }
    int dis(int x) {
        find(x);
        return w[x];
    }
    int size(int x) {
        return sz[find(x)];
    }
};