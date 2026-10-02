#include <bits/stdc++.h>
using namespace std;

/**
 * 二分图带权最大匹配：先最大化匹配数，再最大化匹配边权值和
 *
 * 建模：
 * 左部点为 0..n-1，右部点为 0..m-1
 * g[u] 存储左点 u 连向右点 v 的边，边权为 w
 *
 * 返回：
 * {最大匹配数, 最大匹配数前提下的最大权值和}；match 值为 -1 代表没有匹配
 *
 * 复杂度：
 * 设 V=n+m，E 为原图边数，k 为最大匹配数，有总复杂度：O(E√V+k(E+V)logV)
 *
 * 注意：
 * 1. 支持负权；2. 支持非完全二分图
 */
pair<int, long long> weightedBipartiteMatching(vector<vector<pair<int, long long>>>& g, int m, vector<int>& matchL, vector<int>& matchR) {
    int n = g.size();
    matchL.assign(n, -1);
    matchR.assign(m, -1);
    vector<int> ml(n, -1), mr(m, -1), dep(n), it(n);
    auto bfs = [&]() -> bool {
        queue<int> q;
        fill(dep.begin(), dep.end(), -1);
        for (int i = 0; i < n; ++i) {
            if (ml[i] == -1) {
                dep[i] = 0;
                q.push(i);
            }
        }
        bool ok = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& [v, w] : g[u]) {
                int x = mr[v];
                if (x == -1) {
                    ok = true;
                } else if (dep[x] == -1) {
                    dep[x] = dep[u] + 1;
                    q.push(x);
                }
            }
        }
        return ok;
    };
    auto dfs = [&](this auto&& dfs, int u) -> bool {
        for (int& i = it[u]; i < int(g[u].size()); ++i) {
            auto& [v, w] = g[u][i];
            int x = mr[v];
            if (x == -1 || dep[x] == dep[u] + 1 && dfs(x)) {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
        }
        dep[u] = -1;
        return false;
    };
    int k = 0;
    while (bfs()) {
        fill(it.begin(), it.end(), 0);
        for (int i = 0; i < n; ++i) {
            if (ml[i] == -1 && dfs(i)) ++k;
        }
    }
    if (k == 0) return {0, 0};
    long long mx = g[0].empty() ? 0 : g[0][0].second;
    for (int u = 0; u < n; ++u) {
        for (auto& [v, w] : g[u]) mx = max(mx, w);
    }
    struct Edge {
        int to, rev, cap;
        long long cost;
    };
    int S = n + m, T = S + 1, N = T + 1;
    vector<vector<Edge>> f(N);
    auto add_edge = [&](int u, int v, int cap, long long cost) {
        int a = f[u].size(), b = f[v].size();
        f[u].push_back({v, b, cap, cost});
        f[v].push_back({u, a, 0, -cost});
    };
    for (int i = 0; i < n; ++i) add_edge(S, i, 1, 0);
    for (int u = 0; u < n; ++u) {
        for (auto& [v, w] : g[u]) add_edge(u, n + v, 1, mx - w);
    }
    for (int i = 0; i < m; ++i) add_edge(n + i, T, 1, 0);
    const long long INF = 1LL << 62;
    vector<long long> dis(N), pot(N);
    vector<int> pv(N), pe(N);
    long long mn_ct = 0;
    int flow = 0;
    while (flow < k) {
        fill(dis.begin(), dis.end(), INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        dis[S] = 0;
        pq.push({0, S});
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dis[u]) continue;
            for (int i = 0; i < int(f[u].size()); ++i) {
                auto& e = f[u][i];
                if (!e.cap) continue;
                long long nd = d + e.cost + pot[u] - pot[e.to];
                if (nd < dis[e.to]) {
                    dis[e.to] = nd;
                    pv[e.to] = u;
                    pe[e.to] = i;
                    pq.push({nd, e.to});
                }
            }
        }
        for (int i = 0; i < N; ++i) {
            if (dis[i] < INF) pot[i] += dis[i];
        }
        int aug = k - flow;
        for (int u = T; u != S; u = pv[u]) aug = min(aug, f[pv[u]][pe[u]].cap);
        for (int u = T; u != S; u = pv[u]) {
            auto& e = f[pv[u]][pe[u]];
            e.cap -= aug;
            f[u][e.rev].cap += aug;
            mn_ct += 1LL * aug * e.cost;
        }
        flow += aug;
    }
    long long mx_wt = 1LL * k * mx - mn_ct;
    for (int u = 0; u < n; ++u) {
        for (auto& e : f[u]) {
            if (n <= e.to && e.to < n + m && e.cap == 0) {
                int v = e.to - n;
                matchL[u] = v;
                matchR[v] = u;
            }
        }
    }
    return {k, mx_wt};
}