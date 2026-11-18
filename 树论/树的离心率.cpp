#include <bits/stdc++.h>
using namespace std;

/**
 * 树的离心率
 * 定义 ecc[u] = 顶点 u 到其他顶点的最远距离
 *
 * 性质：对于任意顶点 u，ecc[u] = max(da[u], db[u])，其中 da,db 为直径两端点出发的距离数组
 * 
 * 复杂度：时间 O(n)，空间 O(n)
 */
vector<int> tree_eccentricity(const vector<vector<int>>& g) {
    int n = g.size();
    auto bfs = [&](int s) {
        vector<int> dis(n, -1); dis[s] = 0;
        queue<int> q; q.push(s);
        int far = s;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (dis[u] > dis[far]) far = u;
            for (int v : g[u]) {
                if (dis[v] != -1) continue;
                dis[v] = dis[u] + 1;
                q.push(v);
            }
        }
        return pair{far, dis};
    };
    int a = bfs(0).first;
    auto [b, da] = bfs(a);
    auto db = bfs(b).second;
    vector<int> ecc(n);
    for (int i = 0; i < n; ++i) ecc[i] = max(da[i], db[i]);
    return ecc;
}