#include <bits/stdc++.h>
using namespace std;

/**
 * 图上前后缀分解：点权最小 / 最大值版本
 * 当 dijkstra 无法正确转移时，需要使用类似 spfa 的队列松弛
 * 
 * 状态定义：
 * pre[i]: 从 s 到 i 的所有路径中，能遇到的最小点权
 * suf[i]: 从 i 到 t 的所有路径中，能遇到的最大点权
 * 
 * 正图求 pre，反图求 suf
 * 
 * 状态转移：
 * pre[v] = min(pre[v], min(pre[u], w[v]))
 * suf[v] = max(suf[v], max(suf[u], w[v]))
 * 
 * 复杂度：
 * 队列松弛，最坏 O(nm)
 */

void graphPreSufRelax(vector<vector<int>>& g, vector<vector<int>>& rg, int s, int t, vector<int>& pre, vector<int>& suf, vector<int>& w) {
    const int INF = 0x3f3f3f3f;
    int n = g.size();
    pre.assign(n, INF), suf.assign(n, -INF);
    auto calcMin = [&](vector<vector<int>>& g, int s, vector<int>& dis) -> void {
        vector<char> inq(n);
        queue<int> q;
        dis[s] = w[s];
        q.emplace(s), inq[s] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = 0;
            for (auto& v : g[u]) {
                int nd = min(dis[u], w[v]);
                if (dis[v] > nd) {
                    dis[v] = nd;
                    if (!inq[v]) q.emplace(v), inq[v] = 1;
                }
            }
        }
    };
    auto calcMax = [&](vector<vector<int>>& g, int s, vector<int>& dis) -> void {
        vector<char> inq(n);
        queue<int> q;
        dis[s] = w[s];
        q.emplace(s), inq[s] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = 0;
            for (auto& v : g[u]) {
                int nd = max(dis[u], w[v]);
                if (dis[v] < nd) {
                    dis[v] = nd;
                    if (!inq[v]) q.emplace(v), inq[v] = 1;
                }
            }
        }
    };
    calcMin(g, s, pre);
    calcMax(rg, t, suf);
}