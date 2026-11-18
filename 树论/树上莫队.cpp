#include <bits/stdc++.h>
using namespace std;

/**
 * 树上莫队（不带修，点权）
 * 将树上路径转成欧拉序上的区间，再用普通莫队移动左右指针
 *
 * 路径转区间（先保证 in[u] <= in[v]，记 p = lca(u, v)）：
 * 1. p == u：路径对应 [in[u], in[v]]，不需要额外补点
 * 2. p != u：路径对应 [out[u], in[v]]，但少了 p，回答时临时加入 p，再撤销
 *
 * 复杂度：
 * 设点数 n、询问数 q、块长 B、增删单次均摊 T、读取答案 A，欧拉序长度为 2n
 * 总时间 O((n + q) log n + q log q + (qB + n²/B)T + qA)，空间 O(n log n + q)
 * 若 q 与 n 同阶，增删和读取答案均 O(1)，总时间 O(n * sqrt n)
 */
struct Query {
    int l, r, id, extra;  // 欧拉序区间，询问编号，补点
};
// 预处理：建立进出序和倍增表 O(n log n)，每条路径转成区间 O(log n)
struct preprocess {
    int n, K;
    vector<int> depth, in, out, seq;
    vector<vector<int>> up;
    preprocess(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        K = 1;
        while ((1LL << K) <= n) ++K;
        up.assign(K, vector<int>(n, root));
        depth.resize(n), in.resize(n), out.resize(n);
        seq.reserve(2 * n);
        vector<int> st; st.reserve(n);
        st.emplace_back(root);
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            if (u >= 0) {
                in[u] = seq.size();
                seq.emplace_back(u);
                for (int k = 1; k < K; ++k) up[k][u] = up[k - 1][up[k - 1][u]];
                st.emplace_back(~u);
                for (int i = int(g[u].size()) - 1; ~i; --i) {
                    int v = g[u][i];
                    if (v == up[0][u]) continue;
                    up[0][v] = u;
                    depth[v] = depth[u] + 1;
                    st.emplace_back(v);
                }
            } else {
                u = ~u;
                out[u] = seq.size();
                seq.emplace_back(u);
            }
        }
    }
    int get_lca(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        int d = depth[u] - depth[v];
        for (int k = 0; k < K; ++k) {
            if (d >> k & 1) u = up[k][u];
        }
        if (u == v) return u;
        for (int k = K - 1; k >= 0; --k) {
            if (up[k][u] != up[k][v]) u = up[k][u], v = up[k][v];
        }
        return up[0][u];
    }
    Query query(int u, int v, int id) const {
        if (in[u] > in[v]) swap(u, v);
        int p = get_lca(u, v);
        if (p == u) return {in[u], in[v], id, -1};
        return {out[u], in[v], id, p};
    }
};

vector<int> moAlgorithm(vector<int>& a, vector<Query>& qs, const vector<int>& seq) {
    int n = a.size();
    int q = qs.size();
    // 离散化
    vector<int> xs = a;
    sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
    int tot = xs.size();
    auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
    for (int i = 0; i < n; ++i) a[i] = find(a[i]);
    // 对查询分块；蛇形排序
    int m = seq.size();
    int B = sqrt(m) + 1;
    // int B = max(1, int(m / sqrt(max(1, q))));
    ranges::sort(qs, [&](const Query& x, const Query& y) {
        int bx = x.l / B, by = y.l / B;
        if (bx != by) return bx < by;
        if (bx & 1) return x.r > y.r;
        return x.r < y.r;
    });
    /* ----------------------- 核心部分：单点维护 --------------------- */
    vector<int> cnt(tot);                   // 频率统计桶
    vector<int> ans(q);                     // 询问对应的答案
    vector<int> vis(n);                     // vis[u]：节点 u 在当前欧拉序区间中是否出现奇数次（偶数次被抵消）
    int cur = 0;                            // 维护当前答案
    int L = 0, R = -1;                      // 当前区间左右端点
    auto add = [&](int u) -> void {         // 节点 u 进入统计集合
        int x = a[u];
        if (cnt[x] == 0) ++cur;
        ++cnt[x];
    };
    auto del = [&](int u) -> void {         // 节点 u 离开统计集合
        int x = a[u];
        --cnt[x];
        if (cnt[x] == 0) --cur;
    };
    /* ------------------------------------------------------------- */
    auto toggle = [&](int i) -> void {      // 翻转状态
        int u = seq[i];
        if (vis[u]) del(u);
        else add(u);
        vis[u] ^= 1;
    };
    for (auto& [l, r, id, extra] : qs) {
        while (L > l) toggle(--L);
        while (R < r) toggle(++R);
        while (L < l) toggle(L++);
        while (R > r) toggle(R--);
        if (extra != -1) add(extra);
        ans[id] = cur;
        if (extra != -1) del(extra);
    }
    return ans;
}

/*
// 示例使用方式（输入为 1-based，模板内部为 0-based）
preprocess ord(g);
vector<Query> qs;
for (int i = 0; i < q; ++i) {
    int u, v; cin >> u >> v;
    --u, --v;
    qs.push_back(ord.query(u, v, i));
}
auto ans = moAlgorithm(a, qs, ord.seq);
*/
