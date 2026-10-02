#include <bits/stdc++.h>
using namespace std;

/**
 * Hierholzer 求无向图欧拉路 / 欧拉回路
 *
 * 欧拉路：经过每条边恰好一次，起点和终点可以不同
 * 欧拉回路：经过每条边恰好一次，起点和终点相同
 *
 * 存在条件：
 * 1. 所有有边的点连通
 * 2. 奇度点个数为 0 或 2
 *
 * 若奇度点个数为 0，存在欧拉回路，也存在欧拉路
 * 若奇度点个数为 2，只存在欧拉路，起点和终点是两个奇度点
 *
 * 返回给定图 g （支持重边）的欧拉路点序列；无解返回空数组；无边返回 {0}
 * 时间复杂度 O(n + m)
 */
vector<int> eulerPathUndirected(const vector<vector<int>>& g, int st = 0) {
    int n = g.size(), odd = 0;
    if (st < 0 || st >= n) return {};
    bool hasEdge = false;
    for (int i = 0; i < n; ++i) {
        if (g[i].size() & 1) ++odd;
        if (!g[i].empty()) hasEdge = true;
    }
    if (odd != 0 && odd != 2) return {};
    if (!hasEdge) return {st};
    if (g[st].empty()) return {};
    if (odd == 2 && !(g[st].size() & 1)) return {};
    int m = 0;
    vector<vector<array<int, 2>>> ng(n);
    for (int i = 0; i < n; ++i) ng[i].reserve(g[i].size());
    for (int u = 0; u < n; ++u) {
        for (auto& v : g[u]) {
            if (u > v) continue;
            ng[u].push_back({v, m});
            ng[v].push_back({u, m});
            ++m;
        }
    }
    vector<char> vis(m);
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(st);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        while (!ng[u].empty() && vis[ng[u].back()[1]]) ng[u].pop_back();
        if (ng[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            auto [v, id] = ng[u].back();
            ng[u].pop_back();
            vis[id] = 1;
            stk.push_back(v);
        }
    }
    if (p != -1) return {};
    return res;
}

// 固定起点 st 下字典序最小，O(n + m log m)
vector<int> eulerPathUndirected2(const vector<vector<int>>& g, int st = 0) {
    int n = g.size(), odd = 0;
    if (st < 0 || st >= n) return {};
    bool hasEdge = false;
    for (int i = 0; i < n; ++i) {
        if (g[i].size() & 1) ++odd;
        if (!g[i].empty()) hasEdge = true;
    }
    if (odd != 0 && odd != 2) return {};
    if (!hasEdge) return {st};
    if (g[st].empty()) return {};
    if (odd == 2 && !(g[st].size() & 1)) return {};
    int m = 0;
    vector<vector<array<int, 2>>> ng(n);
    for (int i = 0; i < n; ++i) ng[i].reserve(g[i].size());
    for (int u = 0; u < n; ++u) {
        for (auto& v : g[u]) {
            if (u > v) continue;
            ng[u].push_back({v, m});
            ng[v].push_back({u, m});
            ++m;
        }
    }
    for (auto& e : ng) ranges::sort(e, greater<>());
    vector<char> vis(m);
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(st);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        while (!ng[u].empty() && vis[ng[u].back()[1]]) ng[u].pop_back();
        if (ng[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            auto [v, id] = ng[u].back();
            ng[u].pop_back();
            vis[id] = 1;
            stk.push_back(v);
        }
    }
    if (p != -1) return {};
    return res;
}

/** --------------------------------------------------------
 * 以下是固定起点与终点的版本
 * ------------------------------------------------------- */

/**
 * 返回从 S 到 T 的欧拉路点序列；无解返回空数组
 * 
 * 复杂度 O(n + m)
 */
vector<int> eulerPathUndirected(const vector<vector<int>>& g, int S, int T) {
    int n = g.size(), odd = 0;
    if (S < 0 || S >= n || T < 0 || T >= n) return {};
    bool hasEdge = false;
    for (int i = 0; i < n; ++i) {
        if (g[i].size() & 1) ++odd;
        if (!g[i].empty()) hasEdge = true;
    }
    if (S == T && odd != 0) return {};
    if (S != T && (odd != 2 || !(g[S].size() & 1) || !(g[T].size() & 1))) return {};
    if (!hasEdge) return S == T ? vector<int>{S} : vector<int>{};
    if (g[S].empty()) return {};
    int m = 0;
    vector<vector<array<int, 2>>> ng(n);
    for (int i = 0; i < n; ++i) ng[i].reserve(g[i].size());
    for (int u = 0; u < n; ++u) {
        for (auto& v : g[u]) {
            if (u > v) continue;
            ng[u].push_back({v, m});
            ng[v].push_back({u, m});
            ++m;
        }
    }
    vector<char> vis(m);
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(S);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        while (!ng[u].empty() && vis[ng[u].back()[1]]) ng[u].pop_back();
        if (ng[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            auto [v, id] = ng[u].back();
            ng[u].pop_back();
            vis[id] = 1;
            stk.push_back(v);
        }
    }
    if (p != -1) return {};
    return res;
}

// 固定起点 S、终点 T 下字典序最小，O(n + m log m)
vector<int> eulerPathUndirected2(const vector<vector<int>>& g, int S, int T) {
    int n = g.size(), odd = 0;
    if (S < 0 || S >= n || T < 0 || T >= n) return {};
    bool hasEdge = false;
    for (int i = 0; i < n; ++i) {
        if (g[i].size() & 1) ++odd;
        if (!g[i].empty()) hasEdge = true;
    }
    if (S == T && odd != 0) return {};
    if (S != T && (odd != 2 || !(g[S].size() & 1) || !(g[T].size() & 1))) return {};
    if (!hasEdge) return S == T ? vector<int>{S} : vector<int>{};
    if (g[S].empty()) return {};
    int m = 0;
    vector<vector<array<int, 2>>> ng(n);
    for (int i = 0; i < n; ++i) ng[i].reserve(g[i].size());
    for (int u = 0; u < n; ++u) {
        for (auto& v : g[u]) {
            if (u > v) continue;
            ng[u].push_back({v, m});
            ng[v].push_back({u, m});
            ++m;
        }
    }
    for (auto& e : ng) ranges::sort(e, greater<>());
    vector<char> vis(m);
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(S);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        while (!ng[u].empty() && vis[ng[u].back()[1]]) ng[u].pop_back();
        if (ng[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            auto [v, id] = ng[u].back();
            ng[u].pop_back();
            vis[id] = 1;
            stk.push_back(v);
        }
    }
    if (p != -1) return {};
    return res;
}