#include <bits/stdc++.h>
using namespace std;

/**
 * 给定树（以0-based邻接表形式），建立最近公共祖先查询
 * 1. 建立 LCA(g)，复杂度 O(nlogn)
 * 2. 查询 get_lca(u, v)，复杂度 O(1)
 * 3. 查询 dist(u, v)，复杂度 O(1)
 * 4. 可以用类数组 depth, father 查询节点的深度和父亲，O(1) 
 */
struct LCA {
    int n;
    vector<int> depth, first, lg, euler, father;
    vector<int> dfn;
    int timer;
    vector<vector<int>> st;
    void dfs(vector<vector<int>>& g, int u, int fa) {
        first[u] = euler.size();
        euler.emplace_back(u);
        dfn[u] = timer++;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            depth[v] = depth[u] + 1;
            father[v] = u;
            dfs(g, v, u);
            euler.emplace_back(u);
        }
    }
    LCA(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        timer = 0;
        depth.resize(n);
        dfn.resize(n);
        first.assign(n, -1);
        father.resize(n, -1);
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
    int dist(int x, int y) {
        int z = get_lca(x, y);
        return depth[x] + depth[y] - 2 * depth[z];
    }
    bool on_path(int x, int u, int v) { return dist(u, x) + dist(x, v) == dist(u, v); }
};