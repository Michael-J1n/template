#include <bits/stdc++.h>
using namespace std;

/**
 * 求从源点 source 开始到每个点的最短路路数有几条
 * 
 * 复杂度：
 * O(mlogn)
 */
vector<int> dij(vector<vector<pair<int, int>>>& g, int source) {
    const int INF = 0x3f3f3f3f;
    const int mod = 1e9 + 7;
    int n = g.size();
    vector<int> res(n, 0); res[source] = 1;
    vector<int> dis(n, INF); dis[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; pq.emplace(0, source);
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dis[u]) continue;
        for (auto& [v, w] : g[u]) {
            if (dis[v] > d + w) {
                res[v] = res[u];
                dis[v] = d + w;
                pq.emplace(dis[v], v);
            } else if (dis[v] == d + w) {
                (res[v] += res[u]) %= mod;
            }
        }
    }
    return res;
}