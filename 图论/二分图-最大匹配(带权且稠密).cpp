#include <bits/stdc++.h>
using namespace std;

/**
 * 二分图最大权满匹配 / 矩形 KM
 *
 * 建模：
 * 左部点为 0..n-1，右部点为 0..m-1
 * g[u] 存储左点 u 连向右点 v 的边，边权为 w
 *
 * 求解目标：
 * 匹配数固定为 min(n,m)，在此前提下最大化边权和
 * 如果 n <= m，则每个左点都被匹配
 * 如果 n > m，则每个右点都被匹配
 *
 * 返回：
 * {匹配数, 最大权值和}
 * matchL[u] 为左点 u 匹配到的右点，未匹配为 -1
 * matchR[v] 为右点 v 匹配到的左点，未匹配为 -1
 *
 * 复杂度：
 * O(min(n,m)^2 * max(n,m))
 *
 * 注意：
 * 1. 支持负权；2. 支持重边，自动取同一对点之间权值最大的边；3. 要求存在大小为 min(n,m) 的匹配，否则返回 {-1, 0}
 */
pair<int, long long> KMBipartiteMatching(vector<vector<pair<int, long long>>>& g, int m, vector<int>& matchL, vector<int>& matchR) {
    int n = g.size();
    matchL.assign(n, -1);
    matchR.assign(m, -1);
    const long long INF = 1LL << 60;
    auto solve = [&](vector<vector<long long>>& a, int n, int m) -> pair<long long, vector<int>> {
        vector<long long> lx(n + 1), ly(m + 1);
        vector<int> p(m + 1), way(m + 1);
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            vector<long long> minv(m + 1, INF);
            vector<int> used(m + 1);
            while (true) {
                used[j0] = 1;
                int i0 = p[j0], j1 = 0;
                long long delta = INF;
                for (int j = 1; j <= m; ++j) {
                    if (used[j]) continue;
                    if (a[i0][j] < INF / 2) {
                        long long cur = a[i0][j] - lx[i0] - ly[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
                if (delta == INF) return {INF, {}};
                for (int j = 0; j <= m; ++j) {
                    if (used[j]) {
                        lx[p[j]] += delta;
                        ly[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
                if (p[j0] == 0) break;
            }
            while (j0) {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            }
        }
        long long res = 0;
        vector<int> match(n, -1);
        for (int j = 1; j <= m; ++j) {
            if (p[j]) {
                match[p[j] - 1] = j - 1;
                res += a[p[j]][j];
            }
        }
        return {res, match};
    };
    if (n <= m) {
        vector<vector<long long>> a(n + 1, vector<long long>(m + 1, INF));
        for (int u = 0; u < n; ++u) {
            for (auto& [v, w] : g[u]) {
                a[u + 1][v + 1] = min(a[u + 1][v + 1], -w);
            }
        }
        auto [cost, match] = solve(a, n, m);
        if (cost == INF) return {-1, 0};
        for (int u = 0; u < n; ++u) {
            int v = match[u];
            matchL[u] = v;
            matchR[v] = u;
        }
        return {n, -cost};
    } else {
        vector<vector<long long>> a(m + 1, vector<long long>(n + 1, INF));
        for (int u = 0; u < n; ++u) {
            for (auto& [v, w] : g[u]) {
                a[v + 1][u + 1] = min(a[v + 1][u + 1], -w);
            }
        }
        auto [cost, match] = solve(a, m, n);
        if (cost == INF) return {-1, 0};
        for (int v = 0; v < m; ++v) {
            int u = match[v];
            matchR[v] = u;
            matchL[u] = v;
        }
        return {m, -cost};
    }
}