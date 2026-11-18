#include <bits/stdc++.h>
using namespace std;

/**
 * 二分图最大权匹配：只最大化匹配边权值和，不要求先最大化匹配数
 *
 * 建模：
 * 左部点为 0..n-1，右部点为 0..m-1
 * g[u] 存储左点 u 连向右点 v 的边，边权为 w
 * 每个点最多与一条选中边相连，允许不匹配，也允许空匹配
 *
 * 返回：
 * {匹配数, 最大权值和}；match 值为 -1 代表没有匹配
 * 若有多个最大权值方案，返回其中匹配数最少的方案
 *
 * 复杂度：
 * 设 V=n+m，E 为原图边数，k 为返回的匹配数
 * 时间复杂度 O(k(E+V)log(E+V))，额外空间复杂度 O(E+V)，其中 k<=min(n,m)
 *
 * 注意：
 * 1. 支持负权；2. 支持非完全二分图及重边
 * 3. 边权、距离、势能及运算中间值需在 long long 安全范围内，有限距离需小于 INF
 */
pair<int, long long> maxWeightBipartiteMatching(vector<vector<pair<int, long long>>>& g, int m, vector<int>& matchL, vector<int>& matchR) {
    int n = g.size();
    matchL.assign(n, -1);
    matchR.assign(m, -1);
    long long mx = 0;
    for (int u = 0; u < n; ++u) {
        for (auto& [v, w] : g[u]) mx = max(mx, w);
    }
    if (mx == 0) return {0, 0};
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
    long long mx_wt = 0;
    int flow = 0;
    while (true) {
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
        if (dis[T] == INF) break;
        // delta 为增广路在 mx-w 建模下的原始费用，本次权值收益为 mx-delta
        long long delta = dis[T] + pot[T] - pot[S];
        if (delta >= mx) break;
        for (int i = 0; i < N; ++i) {
            if (dis[i] < INF) pot[i] += dis[i];
        }
        for (int u = T; u != S; u = pv[u]) {
            auto& e = f[pv[u]][pe[u]];
            --e.cap;
            ++f[u][e.rev].cap;
        }
        mx_wt += mx - delta;
        ++flow;
    }
    for (int u = 0; u < n; ++u) {
        for (auto& e : f[u]) {
            if (n <= e.to && e.to < n + m && e.cap == 0) {
                int v = e.to - n;
                matchL[u] = v;
                matchR[v] = u;
            }
        }
    }
    return {flow, mx_wt};
}
