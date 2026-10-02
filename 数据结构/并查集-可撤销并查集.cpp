#include <bits/stdc++.h>
using namespace std;

/**
 * 可撤销并查集
 *
 * 支持：
 * unite(x, y)：合并 x, y
 * check(x, y)：判断 x, y 是否连通
 * size(x)：返回 x 所在连通块大小
 * snapshot()：记录当前状态
 * unod(): 撤销上一次 unite 调用（不管是否真的合并了）
 * rollback(t)：恢复到 snapshot t 对应的状态
 *
 * 复杂度：
 * find: O(log n)
 * unite: O(log n)
 * rollback: 每撤销一次有效合并 O(1)
 */
struct UFS {
    int n, cc;
    vector<int> pa, sz;
    vector<pair<int, int>> stk;
    UFS(int _n) : n(_n), cc(_n) {
        pa.resize(n), sz.resize(n);
        for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1;
    }
    int find(int x) {
        while (pa[x] != x) x = pa[x];
        return x;
    }
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) {
            stk.push_back({-1, -1});
            return false;
        }
        if (sz[px] < sz[py]) swap(px, py);
        stk.push_back({px, py});
        pa[py] = px;
        sz[px] += sz[py], --cc;
        return true;
    }
    bool check(int x, int y) {
        return find(x) == find(y);
    }
    int size(int x) {
        return sz[find(x)];
    }
    int snapshot() {
        return stk.size();
    }
    void undo() {
        auto [px, py] = stk.back();
        stk.pop_back();
        if (px == -1) return;
        sz[px] -= sz[py];
        pa[py] = py;
        ++cc;
    }
    void rollback(int t) {
        while (stk.size() > t) undo();
    }
};
