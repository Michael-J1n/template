#include <bits/stdc++.h>
using namespace std;

/**
 * 求无向带权图最小权哈密顿回路/路径
 * 定义 f[mask][last] 为走过 mask 中的点，最后一个点为 last 的最小权值
 * 复杂度：O(n^2 * 2^n)
 * 注：TSP 问题即为完全图中的最小权哈密顿回路
 */

using ll = long long;
const ll INF = LLONG_MAX;

// 无向带权图最小哈密顿回路（固定 0 为起点）；若返回 -1 说明不存在哈密顿回路
ll hamiltonCycle(vector<vector<pair<ll, int>>>& g) {
    int n = g.size();
    int N = 1 << n;
    vector f(N, vector<ll>(n, INF)); 
    f[1][0] = 0;
    for (int mask = 0; mask < N; ++mask) {
        for (int last = 0; last < n; ++last) {
            if (!(mask >> last & 1) || f[mask][last] == INF) continue;
            for (auto& [w, nxt] : g[last]) {
                if (mask >> nxt & 1) continue;
                int nmask = mask | (1 << nxt);
                f[nmask][nxt] = min(f[nmask][nxt], f[mask][last] + w);
            }
        }
    }
    ll res = INF;
    for (auto& [w, last] : g[0]) {
        if (f[N - 1][last] != INF) res = min(res, f[N - 1][last] + w);
    }
    return res == INF ? -1 : res;
}


// 无向带权图最小哈密顿路径（任意起点）；若返回 -1 说明不存在哈密顿路径
ll hamiltonPath(vector<vector<pair<ll, int>>>& g) {
    int n = g.size();
    int N = 1 << n;
    vector f(N, vector<ll>(n, INF)); 
    for (int i = 0; i < n; ++i) f[1 << i][i] = 0;
    for (int mask = 0; mask < N; ++mask) {
        for (int last = 0; last < n; ++last) {
            if (!(mask >> last & 1) || f[mask][last] == INF) continue;
            for (auto& [w, nxt] : g[last]) {
                if (mask >> nxt & 1) continue;
                int nmask = mask | (1 << nxt);
                f[nmask][nxt] = min(f[nmask][nxt], f[mask][last] + w);
            }
        }
    }
    ll res = INF;
    for (int last = 0; last < n; ++last) {
        res = min(res, f[N - 1][last]);
    }
    return res == INF ? -1 : res;
}