#include <bits/stdc++.h>
using namespace std;

/**
 * 带权并查集；维护差值关系
 *
 * val[x] + w[x] = val[find(x)]
 *
 * unite(x, y, wt) 代表 val[x] + wt = val[y]
 * dis(x, y) 返回 x -> y 的距离，即 val[x] + dis(x, y) = val[y]
 * dis(x) 返回 x -> root 的距离
 */
struct WeightedUFS {
    vector<int> pa, sz;
    vector<long long> w;
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
        w[x] += w[p];
        return pa[x];
    }
    /**
     *         x - - wt - - y
     *        /            /
     *      w[x]         w[y]
     *      /            /
     *    px --- d --- py
     */
    bool unite(int x, int y, long long wt) {
        int px = find(x), py = find(y);
        if (px == py) return w[x] - w[y] == wt;
        long long d = w[y] - w[x] + wt;
        --cc;
        if (sz[px] > sz[py]) {
            swap(px, py);
            d = -d;
        }
        // 到这里，d 表示如果把根 px 挂到根 py 下面，应该设置的 w[px]=d，即 px 相对于 py 的权值偏差
        pa[px] = py;
        w[px] = d;
        sz[py] += sz[px];
        return true;
    }
    bool check(int x, int y) {
        return find(x) == find(y);
    }
    long long dis(int x, int y) {
        int px = find(x), py = find(y);
        assert(px == py);
        return w[x] - w[y];
    }
    long long dis(int x) {
        find(x);
        return w[x];
    }
    int size(int x) {
        return sz[find(x)];
    }
};