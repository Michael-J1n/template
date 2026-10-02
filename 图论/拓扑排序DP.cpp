#include <bits/stdc++.h>
using namespace std;

/**
 * 按照拓扑序进行 DP
 * 以求最长链为例：给定一个 DAG，求从 0 到 n - 1 的最长路长度
 * 拓扑序 DP 常用刷表法
 * 
 * 注意：
 * 1. 距离全都初始化为 -inf 代表不可达（同时也是 max 操作的单位元）
 * 2. 如果要求 0 开始到任一点最长路，在取出对头后把 res 与对头的 dp 求 max
 * 3. 如果要全图任意起点的最长链：初始化所有点的 dis 均为 0（一开始都可达）；但注意无论如何入队和一开始的可达性无关
 */
int longestPathDAG(vector<vector<pair<int, int>>>& g) {
    int n = g.size();
    vector<int> ind(n);
    for (int i = 0; i < n; ++i) {
        for (auto& [j, _] : g[i]) ++ind[j];
    }
    vector<int> dp(n, INT_MIN); dp[0] = 0;
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (!ind[i]) {
            q.emplace(i);
            // dp[i] = 0;  // 如果是任意起点
        }
    }
    // int res = -1;
    while (!q.empty()) {
        auto u = q.front(); q.pop();
        // res = max(res, dp[u]);  // 如果是任意终点
        for (auto& [v, w] : g[u]) {
            dp[v] = max(dp[v], dp[u] + w);
            --ind[v];
            if (!ind[v]) q.emplace(v);
        }
    }
    return dp[n - 1];
}

/**
 * 例2：求最大链（起点入度为0，终点出度为0）的数量
 */
int countMaxChain(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> ind(n);
    for (int i = 0; i < n; ++i) {
        for (auto& j : g[i]) ++ind[j];
    }
    queue<int> q;
    vector<int> dp(n);
    for (int i = 0; i < n; ++i) {
    	if (!ind[i]) q.emplace(i), dp[i] = 1;
    }
    int res = 0;
    while (!q.empty()) {
    	int u = q.front(); q.pop();
    	if (!g[u].size()) res += dp[u];
    	for (auto& v : g[u]) {
    		dp[v] += dp[u];
    		--ind[v];
    		if (!ind[v]) q.emplace(v);
    	}
    }
    return res;
}