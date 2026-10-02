#include <bits/stdc++.h>
using namespace std;

/**
 * 图上前后缀分解
 * 对于路径 s --> t，定义 pre[i] 为从 s 到 i 的路径中的前缀最优值，suf[i] 为从 i 到 t 的路径中的后缀最优值
 * 这样的问题可以通过在图上进行前后缀分解来解决。
 * 
 * 对于前缀信息，可以从 s 开始进行 dfs 或 dij 进行，而对于后缀信息，则是需要先建立反图 rg，然后从 t 开始进行 dfs 或 dij
 * 
 * 此模板以求 pre[i] = 从 s 到 i 的最短路径长度，suf[i] = 从 i 到 t 的最短路径长度为例
 */
void graphPreSuf(vector<vector<pair<int, int>>>& g, vector<vector<pair<int, int>>>& rg, int s, int t, vector<int>& pre, vector<int>& suf) {
    const int INF = 0x3f3f3f3f;
    int n = g.size();
    auto dij = [&](vector<vector<pair<int, int>>>& g, int s, vector<int>& dis) -> void {
        dis.assign(n, INF); dis[s] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; pq.emplace(0, s);
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dis[u]) continue;
            for (auto& [v, w] : g[u]) {
                if (dis[v] > d + w) {
                    dis[v] = d + w;
                    pq.emplace(dis[v], v);
                }
            }
        }
    };
    dij(g, s, pre);
    dij(rg, t, suf);
}

/**
 * 常用使用：
 * 1. 经过点 i: int cur = pre[i] + suf[i];
 * 2. 经过边 u -> v，边权 w: int cur = pre[u] + w + suf[v];
 * 3. 点 i 在 s -> t 的最短路上：pre[i] + suf[i] == pre[t]
 * 4. 边 u -> v 在 s -> t 的最短路上：pre[u] + w + suf[v] == pre[t]
 */