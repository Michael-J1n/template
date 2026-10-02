#include <bits/stdc++.h>
using namespace std;

/**
 * 差分约束（SPFA版本）
 * 
 * 建边：x[u]+w<=x[v] <=> u->v，边权 w
 * 返回满足所有约束的最小非负解 d；若存在正环导致无解（下界被无限抬高），返回空
 * cnt[v] 表示当前 d[v] 对应路径的边数；若 cnt[v] >= n，说明最长路中出现重复点且还能变大，因此存在正环
 *
 * 复杂度：最坏 O(nm)，一般图上可到 O(n + m)
 */
vector<int> diffConstraints(vector<vector<pair<int, int>>>& g) {
    int n = g.size();
    vector<int> d(n), cnt(n);
    vector<char> inq(n, 1);
    queue<int> q;
    for (int i = 0; i < n; ++i) q.emplace(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = 0;
        for (auto& [v, w] : g[u]) {
            if (d[v] < d[u] + w) {
                d[v] = d[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) return {};
                if (!inq[v]) inq[v] = 1, q.emplace(v);
            }
        }
    }
    return d;
}

/**
 * 差分约束（Bellman-Ford版本）
 */
struct Edge {
    int u, v, w;
};
vector<int> diffConstraints(int n, vector<Edge>& edges) {
    vector<int> d(n);
    for (int i = 0; i < n; ++i) {
        bool flag = 0;
        for (auto& [u, v, w] : edges) {
            if (d[v] < d[u] + w) {
                d[v] = d[u] + w;
                flag = 1;
                if (i == n - 1) return {};
            }
        }
        if (!flag) break;
    }
    return d;
}