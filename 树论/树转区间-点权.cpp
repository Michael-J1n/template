#include <bits/stdc++.h>
using namespace std;

/**
 * 树的 DFS 序
 *
 * dfn[u]：节点 u 的 DFS 序下标
 * sz[u]：以 u 为根的子树大小
 * subtree(u)：返回 u 的子树对应的 DFS 序闭区间
 * to_seq(a,b)：传入初始点权数组 a，返回按照 dfn 序排列的点权数组 b；原节点 u 对应的点权 a[u] 对应 b[dfn[u]]
 * 
 * 常驻空间 O(n)，初始化时间 O(n)
 */
struct DfsOrder {
    vector<int> dfn, sz;
    DfsOrder() {}
    DfsOrder(const vector<vector<int>>& g, int root = 0) { init(g, root); }
    void init(const vector<vector<int>>& g, int root = 0) {
        int n = g.size(), timer = 0;
        dfn.assign(n, -1), sz.assign(n, 0);
        vector<int> st; st.reserve(n);
        st.emplace_back(root);
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            if (u >= 0) {
                dfn[u] = timer++;
                st.emplace_back(~u);
                for (int i = g[u].size() - 1; i >= 0; --i) {
                    int v = g[u][i];
                    if (dfn[v] == -1) st.emplace_back(v);
                }
            } else {
                u = ~u;
                sz[u] = 1;
                for (auto& v : g[u]) {
                    if (dfn[v] > dfn[u]) sz[u] += sz[v];
                }
            }
        }
    }
    pair<int, int> subtree(int u) const { return {dfn[u], dfn[u] + sz[u] - 1}; }
    template<class T> void to_seq(vector<int>& a, vector<T>& b) const {
        b.resize(a.size());
        for (int u = 0; u < a.size(); ++u) b[dfn[u]] = a[u];
    }
};

// 示例使用方式
void solve() {
    int n; cin >> n;
    vector<int> a(n);  // 点权
    for (int i = 0; i < n; ++i) cin >> a[i];
    vector<vector<int>> g(n);  // 树
    for (int i = 1; i < n; ++i) {
        int u, v; cin >> u >> v;
        --u, --v;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }

    DfsOrder ord(g, 0);
    vector<long long> b; ord.to_seq(a, b);

    int u; cin >> u;
    --u;
    auto [l, r] = ord.subtree(u);

    // 原树上 u 的子树等价于序列 b 上的区间 [l, r]
    // 单点 u 对应序列 b 上的下标 dfn[u]
}