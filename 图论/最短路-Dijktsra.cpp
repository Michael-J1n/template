#include <bits/stdc++.h>
using namespace std;

/**
 * Dijkstra 单源最短路模板
 * 返回从 source 出发到所有点的最短路距离；若距离为 0x3f3f3f3f 代表不可达
 * 条件：没有负权边；注意模板中 g 存储 {v, w}
 * 
 * cost 函数的意义为需要最小化的代价；例如 边权和：d + w; 边权积：d * w；最大边权：max(d, w)
 * 
 * 复杂度：
 * O((m+n)logn)
 */
int cost(int d, int w) {
    return d + w;
}
vector<int> dij(vector<vector<pair<int, int>>>& g, int source) {
    const int INF = 0x3f3f3f3f;
    int n = g.size();
    vector<int> dis(n, INF); dis[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; pq.emplace(0, source);
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dis[u]) continue;
        for (auto& [v, w] : g[u]) {
            int nd = cost(d, w);
            if (dis[v] > nd) {
                dis[v] = nd;
                pq.emplace(nd, v);
            }
        }
    }
    return dis;
}

/**
 * 使用 Dij 时，不仅权值之和满足，许多别的操作也可以使用这个操作进行更新：
 * 假设路径代价转移为：nd = d[u] op w(u, v)，其中 op 是一个二元运算；然后做松弛 d[v] = min(d[v], nd)
 * 那么 Dij 可以使用的充分条件为 nd >= d，即扩展后不变优（代价不会在某个扩展上变小）
 * 
 * 最小化操作（！！，若是最大化操作反向松弛条件即可）：
 * 
 * ______________________________________________________________
 * 路径代价               | 转移                  | 条件
 * --------------------------------------------------------------
 * 路径和最小              | d + w                |   w >= 0
 * 路径最大边权最小         | max(d, w)            |   NA
 * 路径乘积最小            | d * w                |  d >= 0, w >= 1
 * 路径代价 pair 字典序最小 | (d1, d2) + (w1, w2)  | 每维度增量非负
 * ---------------------------------------------------------------
 */