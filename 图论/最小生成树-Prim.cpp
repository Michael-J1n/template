#include <bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;

/**
 * Prim 算法求最小生成树
 * 返回MST的边权值和，若为 -1 代表图不连通
 * 
 * 复杂度：O(mlogn)
 * 
 * 注意：若需要最大生成树，将堆建反即可
 */
long long primMST(vector<vector<pair<int, int>>>& g) {
    int n = g.size(), ecnt = 0;
    long long wsum = 0;
    vector<char> vis(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = 1, wsum += w, ++ecnt;
        for (auto& [v, nw] : g[u]) {
            if (!vis[v]) pq.emplace(nw, v);
        }
    }
    return ecnt == n ? wsum : -1;
}
/**
 * 显示生成树的版本
 */
vector<vector<pair<int, int>>> getTree(vector<vector<pair<int, int>>>& g) {
    int n = g.size(), ecnt = 0;
    vector<char> vis(n);
    vector<vector<pair<int, int>>> tr(n);
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    pq.emplace(0, 0, -1);
    while (!pq.empty()) {
        auto [w, u, fa] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = 1, ++ecnt;
        if (fa != -1) {
            tr[u].emplace_back(fa, w);
            tr[fa].emplace_back(u, w);
        }
        for (auto& [v, nw] : g[u]) {
            if (!vis[v]) pq.emplace(nw, v, u);
        }
    }
    return ecnt == n ? tr : vector<vector<pair<int, int>>>{};
}

/**
 * 另：邻接矩阵版本，复杂度 O(n^2)，适合顶点数据量小或稠密图
 */
int primMST(const vector<vector<int>>& dis) {
    int n = dis.size();
    vector<int> mnd(n, INF); mnd[0] = 0;
    vector<char> vis(n, 0);
    int wsum = 0, ecnt = 0;
    while (ecnt < n) {
        int u = -1, best = INF;
        for (int i = 0; i < n; ++i) {
            if (!vis[i] && mnd[i] < best) best = mnd[i], u = i;
        }
        if (u == -1) return -1;
        vis[u] = 1, wsum += best, ++ecnt;
        for (int v = 0; v < n; ++v) {
            if (!vis[v] && dis[u][v] < mnd[v]) mnd[v] = dis[u][v];
        }
    }
    return wsum;
}
/**
 * 显示生成树的版本
 */
vector<vector<pair<int, int>>> getTree(const vector<vector<int>>& dis) {
    int n = dis.size();
    vector<int> mnd(n, INF), pa(n, -1);
    mnd[0] = 0;
    vector<char> vis(n, 0);
    vector<vector<pair<int, int>>> g(n);
    for (int t = 0; t < n; ++t) {
        int u = -1, best = INF;
        for (int i = 0; i < n; ++i) {
            if (!vis[i] && mnd[i] < best) best = mnd[i], u = i;
        }
        if (u == -1) return {};
        vis[u] = 1;
        if (pa[u] != -1) {
            g[u].emplace_back(pa[u], mnd[u]);
            g[pa[u]].emplace_back(u, mnd[u]);
        }
        for (int v = 0; v < n; ++v) {
            if (!vis[v] && dis[u][v] < mnd[v]) {
                mnd[v] = dis[u][v];
                pa[v] = u;
            }
        }
    }
    return g;
}

/**
 * 一些最小生成树性质：
 * 1. 任意一颗最小生成树一定包含无向图中权值最小的边
 * 2. 连接非树边形成一个环，非树边边权一定是环上最大值
 * 3. MST 一定是个有 n 个顶点的树（也就是说，图中所有顶点都被包含！）
 */