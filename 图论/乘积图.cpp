#include <bits/stdc++.h>
using namespace std;

/**
 * Product Graph BFS【乘积图】
 * 模板以图上 st 到 ed 的最短回文路径为例；点下标 0-based；有向图；点权为字符。
 *
 * 状态 (u, v)：左端走到 u，右端反向走到 v，当前两侧字符已经匹配。对于普通情况下的乘积图，将状态作为两个点配对即可。
 * 适用：点带字符的最短回文路径、两端同步移动的 Product Graph。
 * 
 * 复杂度：状态 O(n^2)，转移复杂度取决于出边与入边的笛卡尔积。
 */
int palindrome_path(vector<vector<int>>& g, vector<char>& s, int st, int ed) {
    int n = g.size();
    if (s[st] != s[ed]) return -1;
    vector<vector<int>> rg(n);
    for (int u = 0; u < n; ++u) {
        for (auto& v : g[u]) rg[v].push_back(u);
    }
    vector dis(n, vector<int>(n, -1)); dis[st][ed] = 0;
    queue<pair<int, int>> q; q.emplace(st, ed);
    while (!q.empty()) {
        auto [u, v] = q.front(); q.pop();
        int d = dis[u][v];
        if (u == v) return d * 2;
        for (auto& x : g[u]) {
            if (x == v) return d * 2 + 1;
        }
        for (auto& x : g[u]) {
            for (auto& y : rg[v]) {
                if (s[x] != s[y] || dis[x][y] != -1) continue;
                dis[x][y] = d + 1;
                q.emplace(x, y);
            }
        }
    }
    return -1;
}