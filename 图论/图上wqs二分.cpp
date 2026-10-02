#include <bits/stdc++.h>
using namespace std;

/**
 * 适合图上 WQS 二分的问题：选出一个结构（例如生成树），使得特殊边被选中 **恰好k次**
 * 
 * 思想：给特殊边都加上一个权值，二分这个权值（加权越大，越不容易被选中，反之越容易，有单调性）
 * 
 * 恰好选 need 条特殊边的 MST，WQS 二分
 * 记图中的边为 e = {u, v, w, c}，c = 1 表示特殊边
 * 
 * 二分判断：
 *   将所有特殊边的权值变为 w + mid
 *   mid 越大，特殊边越贵，MST 中特殊边数量越少
 * 二分目标：
 *   找到最大的 mid 满足 cnt >= need（否则 mid 更大，边越贵越不容易被选中。因此也需要按照特殊边更优先来排序）
 * 
 * 答案：
 *   modified_sum - mid * need
 * 
 * 注意：如果是惩罚越大，边越不容易选中，那么需要二分最大的 mid 使得选出来的 cnt >= need，特殊边的优先权要更大
 */

struct UFS {
    int n, cc;
    vector<int> pa, sz;
    UFS(int _n) : n(_n), cc(_n) {
        pa.resize(n), sz.resize(n);
        for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1;
    }
    int find(int x) {
        if (pa[x] != x) pa[x] = find(pa[x]);
        return pa[x];
    }
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (sz[px] < sz[py]) swap(px, py);
        pa[py] = px;
        sz[px] += sz[py], sz[py] = 0, --cc;
    }
    bool check(int x, int y) {
        return find(x) == find(y);
    }
};
pair<int, int> kruskal(int n, vector<array<int, 4>> edges, int mid) {
    for (auto& e : edges) {
        if (e[3]) e[2] += mid;
    }
    sort(edges.begin(), edges.end(), [](const auto& e1, const auto& e2) {
        if (e1[2] != e2[2]) return e1[2] < e2[2];
        return e1[3] > e2[3];
    });
    UFS uf(n);
    int sum = 0, cnt = 0, tot = 0;
    for (auto& e : edges) {
        int u = e[0], v = e[1], w = e[2], c = e[3];
        if (uf.check(u, v)) continue;
        uf.unite(u, v);
        sum += w, cnt += c, ++tot;
        if (tot == n - 1) break;
    }
    return {sum, cnt};
}

int wqsMST(int n, vector<array<int, 4>>& edges, int need) {
    int l = -100000, r = 100000;
    auto [sumr, cntr] = kruskal(n, edges, l);
    auto [suml, cntl] = kruskal(n, edges, r);
    if (!(cntl <= need && need <= cntr)) return INT_MAX;  // 无解
    while (l < r) {
        int mid = l + r + 1 >> 1;
        auto [sum, cnt] = kruskal(n, edges, mid);
        if (cnt >= need) l = mid;
        else r = mid - 1;
    }
    auto [sum, cnt] = kruskal(n, edges, l);
    return sum - l * need;
}