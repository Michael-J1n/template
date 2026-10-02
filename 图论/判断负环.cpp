#include <bits/stdc++.h>
using namespace std;

/**
 * 判断图中是否存在负环
 * 
 * 使用 Bellman-Ford 算法
 * 
 * 复杂度：O(nm)
 */
bool hasNegCycle(vector<vector<pair<int, int>>>& g) {
    int n = g.size();
    vector<int> d(n);
    // vector<int> d(n, INT_MAX); d[s] = 0; // 若需要判断是否有从 s 出发可达的负环
    for (int i = 1; i <= n; ++i) {
        bool ok = false;
        for (int u = 0; u < n; ++u) {
            for (auto& [v, w] : g[u]) {
                if (d[v] > d[u] + w) {
                    d[v] = d[u] + w;
                    ok = true;
                }
            }
        }
        if (!ok) return false;
        if (i == n) return true;
    }
    return false;
}