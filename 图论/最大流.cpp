#include <bits/stdc++.h>
using namespace std;

/**
 * Dinic 最大流
 *
 * edges 存原图有向边 {u, v, cap, flow}，调用前无需初始化 flow
 * maxFlow(n, edges, s, t) 返回从源点 s 到汇点 t 的最大流，并将每条原边实际流量写入 flow
 *
 * 最大流最小割定理：maxflow=mincut
 * 最大流结束后，从 s 沿剩余容量 > 0 的残量边能到达的点构成最小割 S 集
 *
 * 复杂度：O(n^2 m)
 */
struct Edge {
    int u, v;
    long long cap, flow;
};
long long maxFlow(int n, vector<Edge>& edges, int s, int t) {
    int m = edges.size();
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < m; ++i) {
        edges[i].flow = 0;
        g[edges[i].u].emplace_back(i, 1), g[edges[i].v].emplace_back(i, -1);
    }
    vector<int> level(n), cur(n);
    auto bfs = [&]() -> bool {
        fill(level.begin(), level.end(), -1); level[s] = 0;
        queue<int> q; q.emplace(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& [id, d] : g[u]) {
                auto& e = edges[id];
                int v = d == 1 ? e.v : e.u;
                long long cap = d == 1 ? e.cap - e.flow : e.flow;
                if (cap && level[v] == -1) {
                    level[v] = level[u] + 1;
                    q.emplace(v);
                }
            }
        }
        return level[t] != -1;
    };
    auto dfs = [&](this auto&& dfs, int u, long long f) -> long long {
        if (u == t) return f;
        for (int& i = cur[u]; i < g[u].size(); ++i) {
            auto [id, d] = g[u][i];
            auto& e = edges[id];
            int v = d == 1 ? e.v : e.u;
            long long cap = d == 1 ? e.cap - e.flow : e.flow;
            if (!cap || level[v] != level[u] + 1) continue;
            long long nf = dfs(v, min(f, cap));
            if (!nf) continue;
            e.flow += d * nf;
            return nf;
        }
        return 0;
    };
    long long res = 0;
    while (bfs()) {
        fill(cur.begin(), cur.end(), 0);
        long long f;
        while ((f = dfs(s, LLONG_MAX))) res += f;
    }
    return res;
}