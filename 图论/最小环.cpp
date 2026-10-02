#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;

/**
 * 最小环问题：图上找一个环，使得环的边权和最小
 */


/**
 * 求有权无向图的最小环：至少包含三个点的边权和最小的环
 * 
 * 使用 Floyd 插点法，传入邻接矩阵；g[u][v] 表示原图边权，d[u][v] 表示当前最短路
 * 在用 k 松弛前，枚举 i, j < k，此时 k 还没有作为中转点更新 d[i][j]，尝试组成 i - k - j - i
 * 
 * 复杂度 O(n^3)
 */
int minCycleW(vector<vector<int>>& g) {
    int n = g.size();
    vector d = g;
    for (int i = 0; i < n; ++i) g[i][i] = d[i][i] = 0;
    int res = INF;
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < k; ++i) {
            for (int j = i + 1; j < k; ++j) {
                if (d[i][j] == INF || g[i][k] == INF || g[k][j] == INF) continue;
                res = min(res, d[i][j] + g[i][k] + g[k][j]);
            }
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (d[i][k] == INF || d[k][j] == INF) continue;
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
    return res == INF ? -1 : res;
}

/**
 * 无向无权图最小环
 * 枚举起点 BFS，遇到一条非树边 u - v 时更新 dis[u] + dis[v] + 1
 * 复杂度 O(n(n + m))
 */
int minCycle(vector<vector<int>>& g) {
    int n = g.size(), res = INF;
    for (int s = 0; s < n; ++s) {
        vector<int> d(n, INF), fa(n, -1);
        queue<int> q;
        d[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& v : g[u]) {
                if (d[v] == INF) {
                    d[v] = d[u] + 1;
                    fa[v] = u;
                    q.push(v);
                } else if (fa[u] != v && fa[v] != u) {
                    res = min(res, d[u] + d[v] + 1);
                }
            }
        }
    }
    return res == INF ? -1 : res;
}