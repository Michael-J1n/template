#include <bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;

/**
 * 分层图最短路
 * 在图问题中，当题目里允许 k 次特殊操作时或有其他需要维护的维度时，可以考虑将节点分层
 * 
 * 常见操作：
 * 1. 限定恰好/至多/至少 k 次 让一条边免费/打折/反向/改变代价/进行传送/剩余资源状态（例如能量）等，最小化到终点边权和/最小化沿途最大边权（适合 dij 进行更新的）
 * 2. 具有边数限制的最短路：求起点和终点间经过 k 条边的最短路；第二维开 k，表示当前经过几个边
 * 3. 要求到终点边数和为某个数倍数的最短路；第二维开 p，表示当前的 mod 值
 * 3. 题目中同时有边权以及点权，那么点权作为额外状态，增加一维代表走到这里的点权；更一般的，需要维护多个信息，那么就**对于节点状态增加一个维度**【LC3970/3977】
 * 4. 若有额外需要到达/收集的点，有必要时还可以储存一个 mask 来代表处理过的集合
 * 
 * 状态定义：dis[u][t] 为从起点到 u，使用了 t 次特殊操作 / 维护的另一维状态为 t时可到达的最优值
 * 
 * 状态转移：假设原边 u -> v，边权 w
 * 1. 不使用特殊操作，(u,t) -> (v,t), 代价 w
 * 2. 使用特殊操作，(u,t) -> (v,t+1), 对此代价特殊操作
 * 
 * 最终答案：不论是 BFS 还是 Dijks 式的更新，最先弹出的终点节点，不论其他状态如何，只要坐标是目标终点，一定是所有终点状态中距离最短的，因此可以提早返回
 * 
 * 复杂度：O(m * k * log(nk))
 * 
 * 注意：
 * 边权很大的时候要将模板中的权值换成 long long；若是恰好 k 条边，需要把返回的答案修改为 dis[t][k]
 * - cost 代表没有使用特殊操作的 cost，这里求最短路因此是加和
 *   如果题目是最多免费走 k 条，求起点到终点最大边权最小值，改成 max(d, w)
 * - ncost 填写使用对于边使用特殊操作的 cost，这里可以将边免费因此可以将 w 变成 0
 *   如果题目变成打半价，改成 d + w / 2;
 */
int cost(int d, int w) {
    return d + w;
}
int ncost(int d, int w) {
    return d;
}
int minCost(vector<vector<pair<int, int>>>& g, int s, int t, int k) {
    int n = g.size();
    vector dis(n, vector<int>(k + 1, INF));
    dis[s][0] = 0;
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    pq.emplace(0, s, 0);
    while (!pq.empty()) {
        auto [d, u, c] = pq.top(); pq.pop();
        if (d != dis[u][c]) continue;
        for (auto& [v, w] : g[u]) {
            int nd1 = cost(d, w);
            if (dis[v][c] > nd1) {
                dis[v][c] = nd1;
                pq.emplace(nd1, v, c);
            }
            if (c >= k) continue;
            int nd2 = ncost(d, w);
            if (dis[v][c + 1] > nd2) {
                dis[v][c + 1] = nd2;
                pq.emplace(nd2, v, c + 1);
            }
        }
    }
    int res = INF;
    for (int c = 0; c <= k; ++c) res = min(res, dis[t][c]);
    return res;
}