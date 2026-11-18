#include <bits/stdc++.h>
using namespace std;

/**
 * 二分图最优边染色
 * 边染色数：∈ {DeltaG, DeltaG+1}. 对于二分图，总是可以用 DeltaG 种颜色进行染色。
 * 功能：用最少的 maxdeg 种颜色对二分图的边染色，相邻边颜色不同。
 * 
 * 参数：
 * szl, szr：左右部顶点数。
 * edges：边集，左部编号 [0, szl-1]，右部编号 [szl, szl+szr-1]。
 * col：每条边的颜色，0-based。
 * 
 * 返回：最少颜色数
 * 复杂度：O(nm)
 */
int ecolor(int szl, int szr, vector<pair<int, int>>& edges, vector<int>& col) {
    int n = szl + szr, m = edges.size();
    vector<int> deg(n);
    for (auto& [u, v] : edges) ++deg[u], ++deg[v];
    int d = 0;
    for (int x : deg) d = max(d, x);
    vector<vector<int>> to(n, vector<int>(d, -1));  // to[u][c]: 顶点 u 通过颜色 c 到达的邻点
    col.assign(m, -1);
    for (int i = 0; i < m; ++i) {
        auto& [u, v] = edges[i];
        int x = 0, y = 0;
        while (to[u][x] != -1) ++x;
        while (to[v][y] != -1) ++y;
        if (x != y) {
            vector<int> path;
            int p = u, c = y;
            while (to[p][c] != -1) {
                int id = to[p][c];
                path.emplace_back(id);
                auto& [a, b] = edges[id];
                p = (p == a ? b : a), c = (c == x ? y : x);
            }
            for (int id : path) {
                auto& [a, b] = edges[id];
                to[a][col[id]] = to[b][col[id]] = -1;
            }
            for (int id : path) {
                auto& [a, b] = edges[id];
                col[id] = (col[id] == x ? y : x);
                to[a][col[id]] = to[b][col[id]] = id;
            }
        }
        col[i] = y;
        to[u][y] = to[v][y] = i;
    }
    return d;
}