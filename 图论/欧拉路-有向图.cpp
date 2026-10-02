#include <bits/stdc++.h>
using namespace std;

/**
 * Hierholzer 求有向图欧拉路 / 欧拉回路
 *
 * 欧拉路：经过每条边恰好一次，起点和终点可以不同
 * 欧拉回路：经过每条边恰好一次，起点和终点相同
 *
 * 存在条件：
 * 1. 所有有边的点弱连通
 * 2. 满足下面两种情况之一：
 *    - 欧拉回路：所有点 in == out
 *    - 欧拉路：一个点 out == in + 1，一个点 in == out + 1，其余点 in == out
 *
 * 若存在 out == in + 1 的点，必须从它出发
 * 否则从任意有出边点出发
 *
 * 返回给定图 g （支持重边与自环的）欧拉路点序列；无解返回空数组；无边返回 {0}
 * 时间复杂度 O(n + m)
 */
vector<int> eulerPathDirected(vector<vector<int>> g, int st = 0) {
    int n = g.size(), m = 0;
    if (st < 0 || st >= n) return {};
    vector<int> bal(n);
    for (int u = 0; u < n; ++u) {
        bal[u] += g[u].size();
        m += g[u].size();
        for (auto& v : g[u]) --bal[v];
    }
    int s = 0, t = 0;
    for (int i = 0; i < n; ++i) {
        if (bal[i] == 1) ++s;
        else if (bal[i] == -1) ++t;
        else if (bal[i]) return {};
    }
    if (!((s == 1 && t == 1) || (s == 0 && t == 0))) return {};
    if (!m) return {st};
    if (g[st].empty()) return {};
    if (s == 1 && bal[st] != 1) return {};
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(st);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        if (g[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            int v = g[u].back();
            g[u].pop_back();
            stk.push_back(v);
        }
    }
    if (p != -1) return {};
    return res;
}

// 固定起点 st 下字典序最小，O(n + m log m)
vector<int> eulerPathDirected2(vector<vector<int>> g, int st = 0) {
    int n = g.size(), m = 0;
    if (st < 0 || st >= n) return {};
    vector<int> bal(n);
    for (int u = 0; u < n; ++u) {
        ranges::sort(g[u], greater<>());
        bal[u] += g[u].size();
        m += g[u].size();
        for (auto& v : g[u]) --bal[v];
    }
    int s = 0, t = 0;
    for (int i = 0; i < n; ++i) {
        if (bal[i] == 1) ++s;
        else if (bal[i] == -1) ++t;
        else if (bal[i]) return {};
    }
    if (!((s == 1 && t == 1) || (s == 0 && t == 0))) return {};
    if (!m) return {st};
    if (g[st].empty()) return {};
    if (s == 1 && bal[st] != 1) return {};
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(st);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        if (g[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            int v = g[u].back();
            g[u].pop_back();
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
vector<int> eulerPathDirected(vector<vector<int>> g, int S, int T) {
    int n = g.size(), m = 0;
    if (S < 0 || S >= n || T < 0 || T >= n) return {};
    vector<int> bal(n);
    for (int u = 0; u < n; ++u) {
        bal[u] += g[u].size();
        m += g[u].size();
        for (auto& v : g[u]) --bal[v];
    }
    if (S == T) {
        for (int i = 0; i < n; ++i) {
            if (bal[i]) return {};
        }
    } else {
        for (int i = 0; i < n; ++i) {
            if (i == S) {
                if (bal[i] != 1) return {};
            } else if (i == T) {
                if (bal[i] != -1) return {};
            } else if (bal[i]) return {};
        }
    }
    if (!m) return S == T ? vector<int>{S} : vector<int>{};
    if (g[S].empty()) return {};
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(S);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        if (g[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            int v = g[u].back();
            g[u].pop_back();
            stk.push_back(v);
        }
    }
    if (p != -1) return {};
    return res;
}

// 固定起点 S、终点 T 下字典序最小，O(n + m log m)
vector<int> eulerPathDirected2(vector<vector<int>> g, int S, int T) {
    int n = g.size(), m = 0;
    if (S < 0 || S >= n || T < 0 || T >= n) return {};
    vector<int> bal(n);
    for (int u = 0; u < n; ++u) {
        ranges::sort(g[u], greater<>());
        bal[u] += g[u].size();
        m += g[u].size();
        for (auto& v : g[u]) --bal[v];
    }
    if (S == T) {
        for (int i = 0; i < n; ++i) {
            if (bal[i]) return {};
        }
    } else {
        for (int i = 0; i < n; ++i) {
            if (i == S) {
                if (bal[i] != 1) return {};
            } else if (i == T) {
                if (bal[i] != -1) return {};
            } else if (bal[i]) return {};
        }
    }
    if (!m) return S == T ? vector<int>{S} : vector<int>{};
    if (g[S].empty()) return {};
    vector<int> stk;
    stk.reserve(m + 1);
    stk.push_back(S);
    vector<int> res(m + 1);
    int p = m;
    while (!stk.empty()) {
        int u = stk.back();
        if (g[u].empty()) {
            res[p--] = u;
            stk.pop_back();
        } else {
            int v = g[u].back();
            g[u].pop_back();
            stk.push_back(v);
        }
    }
    if (p != -1) return {};
    return res;
}