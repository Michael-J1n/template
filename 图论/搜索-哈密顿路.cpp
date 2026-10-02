#include <bits/stdc++.h>
using namespace std;

/**
 * 哈密顿回路：一条经过**每个节点恰好一次**的回路
 * 给定一个无向图的邻接表 g，判断图中是否存在（固定起点为 0 的）哈密顿回路
 * 算法：排列相邻有关形的状压 dp，定义 f[mask][last] 为经过了 mask 中的点且路径最后一点为 last 的路径是否可达
 * 复杂度： O(n^2*2^n)
 * 注意：如果是哈密顿路径，把判断是否可以回到 0 的条件去掉即可
 */

// 检查是否存在从 0 开始的哈密顿环
bool hamiltonCycle(vector<vector<int>>& g) {
    int n = g.size();
    int N = 1 << n;
    vector f(N, vector<char>(n));
    f[1][0] = 1;
    for (int mask = 0; mask < N; ++mask) {
        for (int last = 0; last < n; ++last) {
            if (!(mask >> last & 1) || !f[mask][last]) continue;
            for (auto& nxt : g[last]) {
                if (mask >> nxt & 1) continue;
                int nmask = mask | (1 << nxt);
                f[nmask][nxt] = 1;
            }
        }
    }
    for (auto& v : g[0]) {
        if (f[N - 1][v]) return 1;
    }
    return 0;
}

// 检查是否存在从任意起点开始的哈密顿路径
bool hamiltonPath(vector<vector<int>>& g) {
    int n = g.size();
    int N = 1 << n;
    vector f(N, vector<char>(n));
    for (int i = 0; i < n; ++i) f[1 << i][i] = 1;
    // f[1][0] = 1;  // 如果固定 0 才能做起点
    for (int mask = 0; mask < N; ++mask) {
        for (int last = 0; last < n; ++last) {
            if (!(mask >> last & 1) || !f[mask][last]) continue;
            for (auto& nxt : g[last]) {
                if (mask >> nxt & 1) continue;
                int nmask = mask | (1 << nxt);
                f[nmask][nxt] = 1;
            }
        }
    }
    for (int v = 0; v < n; ++v) {
        if (f[N - 1][v]) return 1;
    }
    return 0;
}

// TSP 问题求最小路径边权和；输入 d 为边权的邻接矩阵
const int INF = 0x3f3f3f3f;
int TSP(vector<vector<int>>& d) {
    int n = d.size(), N = 1 << n;
	vector f(N, vector<int>(n, INF)); f[1][0] = 0;  // f[mask][i] = 经过 mask 中的城市，最后在城市 i 的最短路
	for (int mask = 0; mask < N; ++mask) {
		for (int i = 0; i < n; ++i) {
			if (!(mask >> i & 1) || f[mask][i] == INF) continue;
			for (int j = 0; j < n; ++j) {
				if (mask >> j & 1 || d[i][j] == INF) continue;
				int nmask = mask | (1 << j);
				f[nmask][j] = min(f[nmask][j], f[mask][i] + d[i][j]);
			}
		}
	}
	int res = INF;
	for (int i = 0; i < n; ++i) {
		if (d[i][0] != INF) res = min(res, f[N - 1][i] + d[i][0]);
	}
    return res;
}