#include <bits/stdc++.h>
using namespace std;

/**
 * 正边权树的 LCA 建立，O(nlogn) 预处理
 *
 * 功能：
 * get_lca(u, v): 返回 u 和 v 的最近公共祖先
 * dist(u, v): 返回 u 到 v 的带权距离
 * len(u, v): 返回 u 到 v 的边数距离
 * get_depth(u): 返回 u 的深度，根深度为 0
 * get_root_dist(u): 返回 root 到 u 的带权距离
 * on_path(x, u, v): 判断 x 是否在 u-v 路径上
 * dfn[u]: u 的 dfs 时间戳
 *
 * 复杂度：
 * 预处理 O(nlogn)，单次查询 O(1)
 */
struct LCA {
    int n;
    vector<int> depth, first, lg, euler;
    vector<long long> dis;
    vector<int> dfn;
    vector<vector<int>> st;
    int timer;
    void dfs(vector<vector<pair<int, int>>>& g, int u, int fa) {
        first[u] = euler.size();
        euler.emplace_back(u);
        dfn[u] = timer++;
        for (auto& [v, w] : g[u]) {
            if (v == fa) continue;
            depth[v] = depth[u] + 1;
            dis[v] = dis[u] + w;
            dfs(g, v, u);
            euler.emplace_back(u);
        }
    }
    LCA(vector<vector<pair<int, int>>>& g, int root = 0) {
        n = g.size();
        timer = 0;
        depth.resize(n);
        dis.resize(n);
        dfn.resize(n);
        first.assign(n, -1);
        dfs(g, root, -1);
        int m = euler.size();
        lg.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) lg[i] = lg[i >> 1] + 1;
        int K = lg[m] + 1;
        st.assign(K, vector<int>(m));
        st[0] = euler;
        for (int k = 1; k < K; ++k) {
            for (int i = 0; i + (1 << k) <= m; ++i) {
                int x = st[k - 1][i], y = st[k - 1][i + (1 << (k - 1))];
                st[k][i] = depth[x] < depth[y] ? x : y;
            }
        }
    }
    int get_lca(int x, int y) {
        int l = first[x], r = first[y];
        if (l > r) swap(l, r);
        int k = lg[r - l + 1];
        int a = st[k][l], b = st[k][r - (1 << k) + 1];
        return depth[a] < depth[b] ? a : b;
    }
    long long dist(int x, int y) {
        int z = get_lca(x, y);
        return dis[x] + dis[y] - 2 * dis[z];
    }
    int getDepth(int x) { return depth[x]; }
    long long get_root_dist(int x) { return dis[x]; }
    int len(int x, int y) {
        int z = get_lca(x, y);
        return depth[x] + depth[y] - 2 * depth[z];
    }
    bool on_path(int x, int u, int v) { return len(u, x) + len(x, v) == len(u, v); }
};