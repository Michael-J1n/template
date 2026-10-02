#include <bits/stdc++.h>
using namespace std;

/**
 * 树的边 DFS 序
 *
 * dfn[u]：节点 u 的 DFS 序下标
 * sz[u]：以 u 为根的子树大小
 * subtree(u)：返回 u 的子树内部所有边对应的 DFS 序闭区间，叶节点返回空区间
 * to_seq(b)：返回按照较深端点的 dfn 序排列的边权数组 b，根节点对应位置的边权为 0
 * 
 * 常驻空间 O(n)，初始化时间 O(n)
 */
struct DfsOrderW {
    vector<int> dfn, sz, w;
    DfsOrderW() {}
    DfsOrderW(const vector<vector<pair<int, int>>>& g, int root = 0) { init(g, root); }
    void init(const vector<vector<pair<int, int>>>& g, int root = 0) {
        int n = g.size(), timer = 0;
        dfn.assign(n, -1), sz.assign(n, 0), w.assign(n, 0);
        vector<int> st; st.reserve(n);
        st.emplace_back(root);
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            if (u >= 0) {
                dfn[u] = timer++;
                st.emplace_back(~u);
                for (int i = g[u].size() - 1; i >= 0; --i) {
                    auto [v, ew] = g[u][i];
                    if (dfn[v] == -1) w[v] = ew, st.emplace_back(v);
                }
            } else {
                u = ~u;
                sz[u] = 1;
                for (auto& [v, ew] : g[u]) {
                    if (dfn[v] > dfn[u]) sz[u] += sz[v];
                }
            }
        }
    }
    pair<int, int> subtree(int u) const { return {dfn[u] + 1, dfn[u] + sz[u] - 1}; }
    template<class T> void to_seq(vector<T>& b) const {
        b.resize(w.size());
        for (int u = 0; u < w.size(); ++u) b[dfn[u]] = w[u];
    }
};

// 示例使用方式
void solve() {
    int n; cin >> n;
    vector<vector<pair<int, int>>> g(n);  // 带边权树
    for (int i = 1; i < n; ++i) {
        int u, v, w; cin >> u >> v >> w;
        --u, --v;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }

    DfsOrderW ord(g, 0);
    vector<long long> b; ord.to_seq(b);

    int u; cin >> u;
    --u;
    auto [l, r] = ord.subtree(u);

    // 原树上 u 的子树内部所有边等价于序列 b 上的区间 [l, r]
    // u 与父节点之间的边对应序列 b 上的下标 dfn[u]
}