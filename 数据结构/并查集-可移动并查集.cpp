#include <bits/stdc++.h>
using namespace std;

/**
 * 可移动并查集
 *
 * id[x] 代表逻辑点 x 当前对应的五里店
 * 
 * 支持：
 * root(x)：找到 x 所在集合的根
 * unite(x, y)：合并 x, y 所在集合
 * move(x, y)：把 x 单独移动到 y 所在集合
 * split(x)：把 x 拆出来单独形成一个新集合
 * check(x, y)：判断 x, y 是否在同一集合
 * size(x)：查询 x 所在集合大小
 */
struct MovableUFS {
    int tot, cc;
    vector<int> pa, sz, id;
    MovableUFS(int n, int expected_extra = 0) : tot(n), cc(n) {
        pa.reserve(n + expected_extra), sz.reserve(n + expected_extra);
        pa.resize(n), sz.resize(n), id.resize(n);
        for (int i = 0; i < n; ++i) pa[i] = id[i] = i, sz[i] = 1;
    }
    int find(int x) {  // find root for physical point
        if (pa[x] != x) pa[x] = find(pa[x]);
        return pa[x];
    }
    int root(int x) {  // find root for logical point
        return find(id[x]);
    }
    void unite(int x, int y) {
        int px = root(x), py = root(y);
        if (px == py) return;
        if (sz[px] < sz[py]) swap(px, py);
        pa[py] = px;
        sz[px] += sz[py], sz[py] = 0, --cc;
    }
    bool move(int x, int y) {
        int px = root(x), py = root(y);
        if (px == py) return false;
        --sz[px], ++sz[py];
        if (sz[px] == 0) --cc;
        int z = tot; ++tot;
        id[x] = z;                                // logical point x corresponds to physical point z
        pa.emplace_back(py), sz.emplace_back(0);  // pa[z] = py, sz[z] is meaningless since z is not root
        return true;
    }
    bool split(int x) {
        int px = root(x);
        if (sz[px] == 1) return false;
        --sz[px];
        ++cc;
        int z = tot; ++tot;
        id[x] = z;                                // logical point x corresponds to physical point z
        pa.emplace_back(z), sz.emplace_back(1);   // z is a new root with one logical point
        return true;
    }
    bool check(int x, int y) {
        return root(x) == root(y);
    }
    int size(int x) {
        return sz[find(id[x])];
    }
};
