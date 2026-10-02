#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;

/**
 * Floyd 类点集 DP
 *
 * 状态定义:
 * f[i][j]: 当前允许使用已经枚举过的点作为中转点时, i 到 j 的最优信息
 *
 * 状态转移:
 * 枚举新加入的中转点 k，若 i -> j 的最优方案经过 k, 则拆成 i -> k 和 k -> j：
 * f[i][j] = merge(f[i][j], combine(f[i][k], f[k][j]))
 *    merge: 当前答案和新答案如何取优
 *    combine: 两段路径信息如何合并
 *
 * 复杂度:
 * 时间 O(n^3), 空间 O(n^2)
 */


// 示例：最短路为 min + sum；最小瓶颈路 min + max；最大容量路 max + min；可达性 or + and
int merge(int a, int b) {
    return min(a, b);
}
int combine(int a, int b) {
    if (a == INF || b == INF) return INF;
    return a + b;
}
vector<vector<int>> floydDP(int n, vector<vector<int>>& edges) {
    vector f(n, vector<int>(n, INF));
    for (int i = 0; i < n; ++i) f[i][i] = 0;
    for (auto& e : edges) {
        int u = e[0], v = e[1], wt = e[2];
        f[u][v] = merge(f[u][v], wt);
        f[v][u] = merge(f[v][u], wt);  // 若边有向，删除此行
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                f[i][j] = merge(f[i][j], combine(f[i][k], f[k][j]));
            }
        }
    }
    return f;
}