#include <bits/stdc++.h>
using namespace std;

/**
 * 求任意两点之间最短路条数
 * 
 * 复杂度：
 * O(n^3)
 * 
 * 注意：若要判断 v 是否在 s-t 的最短路上：f[s][v] + f[v][t] == f[s][t]
 * 在这个情况下经过 v 的 s-t 最短路数：cnt[s][v] * cnt[v][t]
 */
vector<vector<int>> floyd(int n, vector<vector<int>>& edges) {
    const int INF = 0x3f3f3f3f;
    vector f(n, vector<int>(n, INF));  // f[i][j] = i-j 之间最短路长度
    vector cnt(n, vector<int>(n, 0));   // f[i][j] = i-j 之间最短路计数
    for (int i = 0; i < n; ++i) f[i][i] = 0, cnt[i][i] = 1;
    for (auto& e : edges) {
    	int u = e[0], v = e[1], w = e[2];
    	f[u][v] = min(f[u][v], w);
    	f[v][u] = min(f[v][u], w);
    	cnt[u][v] = cnt[v][u] = 1;
    }
    for (int k = 0; k < n; ++k) {
    	for (int i = 0; i < n; ++i) {
    		if (i == k || f[i][k] == INF) continue;
    		for (int j = 0; j < n; ++j) {
    			if (j == k || f[k][j] == INF) continue;
    			if (f[i][j] > f[i][k] + f[k][j]) {
    				f[i][j] = f[i][k] + f[k][j];
    				cnt[i][j] = cnt[i][k] * cnt[k][j];
    			} else if (f[i][j] == f[i][k] + f[k][j]) {
    				cnt[i][j] += cnt[i][k] * cnt[k][j];
    			}
    		}
    	}
    }
    return cnt;
}