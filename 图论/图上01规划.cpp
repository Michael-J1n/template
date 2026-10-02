#include <bits/stdc++.h>
using namespace std;

/**
 * 图上 01 分数规划：最大比值环
 * 边为 u -> v，有两种权值 a 与 b，求一个环最大化 sum(a) / sum(b)
 * check(x): 是否存在正环，使 sum(a - x * b) > 0
 * 复杂度：O(Tnm)
 * 注意：0-based，b 必须为正
 */

bool hasPosCycle(int n, vector<array<int, 4>>& edges, double x) {
    vector<double> d(n);
    for (int i = 1; i <= n; ++i) {
        bool ok = false;
        for (auto& e : edges) {
            auto [u, v, a, b] = e;
            double w = a - x * b;
            if (d[v] < d[u] + w) {
                d[v] = d[u] + w;
                ok = true;
            }
        }
        if (!ok) return false;
        if (i == n) return true;
    }
    return false;
}

double maxRatioCycle(int n, vector<array<int, 4>>& edges) {
    double l = 0, r = 0;
    for (auto& e : edges) {
        auto [u, v, a, b] = e;
        r = max(r, 1.0 * a / b);
    }
    for (int i = 0; i < 70; ++i) {
        double mid = (l + r) / 2;
        if (hasPosCycle(n, edges, mid)) l = mid;
        else r = mid;
    }
    return l;
}