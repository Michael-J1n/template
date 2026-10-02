#include <bits/stdc++.h>
using namespace std;

/**
 * 求单源严格次短路
 *  
 * dis1[u] 表示 s 到 u 的最短路；dis2[u] 表示 s 到 u 的严格次短路
 *
 * 复杂度：
 * O(m log m)
 * 
 * 注意：
 * 若需要非严格最短路，第二个分支判断改成 else if (nd >= dis1[v] && nd < dis2[v]) 即可
 */
vector<int> secondShortest(int n, vector<vector<pair<int, int>>>& g, int s) {
    vector<int> dis1(n, INT_MAX), dis2(n, INT_MAX);
    dis1[s] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dis2[u]) continue;
        for (auto& [v, w] : g[u]) {
            int nd = d + w;
            if (nd < dis1[v]) {
                dis2[v] = dis1[v];
                dis1[v] = nd;
                pq.emplace(dis1[v], v);
            } else if (nd > dis1[v] && nd < dis2[v]) {
                dis2[v] = nd;
                pq.emplace(dis2[v], v);
            }
        }
    }
    return dis2;
}