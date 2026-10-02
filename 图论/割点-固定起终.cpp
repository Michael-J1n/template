#include <bits/stdc++.h>
using namespace std;

/**
 * 求无向图起点 s 到终点 t 路径上的割点（必经点）
 *
 * 复杂度：O(n + m)
 */
vector<int> cutPoints(vector<vector<int>>& g, int s, int t) {
    int n = g.size(), timer = 0;
    vector<int> dfn(n, -1), low(n), has(n), cut(n);
    int timer = 0;
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
    	dfn[u] = low[u] = timer++;
    	has[u] = u == t;
    	for (auto& v : g[u]) {
    		if (v == fa) continue;
    		if (dfn[v] == -1) {
    			dfs(v, u);
    			low[u] = min(low[u], low[v]);
    			has[u] |= has[v];
    			if (u != s && u != t && has[v] && low[v] >= dfn[u]) cut[u] = 1;
    		} else {
    			low[u] = min(low[u], dfn[v]);
    		}
    	}
    };
    dfs(s, -1);
    return cut;
}