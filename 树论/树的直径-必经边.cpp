#include <bits/stdc++.h>
using namespace std;

/**
 * 求带权树的直径与所有直径的必经边数
 * 
 * 代码结束后所有直径的公共部分为 path[l...r]
 * 返回所有直径的必经边数
 *
 * 复杂度 O(n)
 */
int treeDiameterMandatoryEdges(vector<vector<pair<int, int>>>& g) {
    int n = g.size();
    vector<int> fa;
    vector<long long> dis;
    auto dfs = [&](this auto&& dfs, int u, int p) -> void {
        for (auto& [v, w] : g[u]) {
            if (v == p) continue;
            fa[v] = u;
            dis[v] = dis[u] + w;
            dfs(v, u);
        }
    };
    auto get = [&](int s) -> int {
        fa.assign(n, -1);
        dis.assign(n, 0);
        dfs(s, -1);
        int x = s;
        for (int i = 0; i < n; ++i) {
            if (dis[i] > dis[x]) x = i;
        }
        return x;
    };
    int s = get(0), t = get(s);
    long long D = dis[t];
    vector<int> path;
    for (int u = t; u != -1; u = fa[u]) path.emplace_back(u);
    int m = path.size();
    vector<char> in_path(n);
    for (auto& u : path) in_path[u] = 1;
    vector<long long> mx(m), L(m), R(m);
    for (int i = 0; i < m; ++i) {
        L[i] = D - dis[path[i]];
        R[i] = dis[path[i]];
    }
    auto get_depth = [&](this auto&& getDepth, int u, int p) -> long long {
        long long res = 0;
        for (auto& [v, w] : g[u]) {
            if (v == p || in_path[v]) continue;
            res = max(res, getDepth(v, u) + w);
        }
        return res;
    };
    for (int i = 1; i + 1 < m; ++i) {
        mx[i] = get_depth(path[i], -1);
    }
    int l = 0, r = m - 1;
    for (int i = 1; i + 1 < m; ++i) {
        if (mx[i] == L[i]) l = i;
        if (mx[i] == R[i]) r = min(r, i);
    }
    // 此时路径 path[l]..path[r] 都是必经部分
    return r - l;
}