#include <bits/stdc++.h>
using namespace std;
#define INF INT_MAX

/**
 * SPFA 单源最短路，支持负权边，可检测从 source 可达的负环
 * 
 * 传入图 g 与起点 source
 * neg[t]=1 代表 source 到 t 的距离可以由负环无限缩小；否则 dis[t] 代表 source 到 t 的最小距离，INF 代表不可达
 * 
 * 算法逻辑：
 * Bellman-Ford 每轮扫全图；SPFA 只把本轮距离变小的点丢进队列，用它去松弛出边
 * cnt[v] 表示当前 dis[v] 对应路径的边数；若 cnt[v] >= n，说明路径中出现重复点，且距离仍被严格变小，因此存在 source 可达负环影响 v
 * 
 * 复杂度：最坏 O(nm)，一般图上 O(n + m)
 */
vector<int> spfa(vector<vector<pair<int, int>>>& g, int source, vector<char>& neg) {
    int n = g.size();
    vector<int> dis(n, INF), cnt(n);
    vector<char> inq(n);
    neg.assign(n, 0);
    queue<int> q, nq;
    dis[source] = 0, inq[source] = 1, q.emplace(source);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = 0;
        if (neg[u]) continue;
        for (auto& [v, w] : g[u]) {
            if (neg[v]) continue;
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) neg[v] = 1, nq.emplace(v);
                else if (!inq[v]) inq[v] = 1, q.emplace(v);
            }
        }
    }
    // 搜索所有距离可以无限下降的点
    while (!nq.empty()) {
        int u = nq.front(); nq.pop();
        for (auto& [v, w] : g[u]) {
            if (!neg[v]) neg[v] = 1, nq.emplace(v);
        }
    }
    return dis;
}