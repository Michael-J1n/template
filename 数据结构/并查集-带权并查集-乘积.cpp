#include <bits/stdc++.h>
using namespace std;

/**
 * 带权并查集；维护倍数关系
 * val[x] * w[x] = val[find(x)]
 *
 * unite(x, y, wt) 代表 val[x] * wt = val[y]（加入 x--(wt)->y 边）
 * dis(x, y) 返回 x -> y 的距离（val[x] * dis(x, y) = val[y]）
 * dis(x) 返回 x -> root 的距离
 *
 * 要求维护的倍数关系非 0
 */
struct WeightedUFS {
    vector<int> pa, sz;
    vector<double> w;
    int cc;
    WeightedUFS(int n) {
        cc = n;
        pa.resize(n), sz.resize(n), w.resize(n);
        for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1, w[i] = 1;
    }
    int find(int x) {
        if (pa[x] == x) return x;
        int p = pa[x];
        pa[x] = find(p);
        w[x] *= w[p];
        return pa[x];
    }
    bool unite(int x, int y, double wt) {
        int px = find(x), py = find(y);
        if (px == py) return abs(w[y] * wt - w[x]) < 1e-9;
        double d = w[y] * wt / w[x];
        --cc;
        if (sz[px] > sz[py]) {
            swap(px, py);
            d = 1.0 / d;
        }
        pa[px] = py;
        w[px] = d;
        sz[py] += sz[px];
        return true;
    }
    bool check(int x, int y) {
        return find(x) == find(y);
    }
    double dis(int x, int y) {
        int px = find(x), py = find(y);
        assert(px == py);
        return w[x] / w[y];
    }
    double dis(int x) {
        find(x);
        return w[x];
    }
    int size(int x) {
        return sz[find(x)];
    }
};