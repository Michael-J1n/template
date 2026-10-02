#include <bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;

/**
 * Floyd 全源最短路
 *
 * 状态定义:
 * f[i][j]: 在当前阶段下, i 到 j 的最短距离
 * 循环不变量：外层循环处理完 k 以后，任意 i, j 的 f[i][j] 都表示只允许用 0..k 作为中转点的最短路
 * 
 * 状态转移:
 * 对于当前枚举的中转点 k, 考虑 i 到 j 的最短路是否经过 k
 * 1. 不经过 k: f[i][j] 保持不变
 * 2. 经过 k: 路径变成 i -> k -> j, 用 f[i][k] + f[k][j] 更新 f[i][j]：f[i][j] = min(f[i][j], f[i][k] + f[k][j])
 * 
 * 复杂度:
 * 时间 O(n^3), 空间 O(n^2)
 * 
 * 注意：
 * 计算结束后若存在 i 使得 f[i][i]<0，说明存在负环
 */
vector<vector<int>> floyd(int n, vector<vector<int>>& edges) {
    vector f(n, vector<int>(n, INF));
    for (int i = 0; i < n; ++i) f[i][i] = 0;
    for (auto& e : edges) {
        int u = e[0], v = e[1], wt = e[2];
        f[u][v] = min(f[u][v], wt);
        f[v][u] = min(f[v][u], wt);  // 若有向边，删除此行
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (f[i][k] == INF) continue;
            for (int j = 0; j < n; ++j) {
                if (f[k][j] == INF) continue;
                f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
            }
        }
    }
    return f;
}

/**
 * 对于 Floyd 算法，本质是动态规划，通过考虑转移点 k 来更新 i-j 的状态 f[i][j]
 * 因此除了求最短路，还可以有如下几种变体：
 * 1. 最小化最大边：f[i][j] 为 i-j 的所有简单路径中，路径最大边的最小值
 *    转移为 f[i][j] = min(f[i][j], max(f[i][k], f[k][j])); 初始化为 INF
 * 2. 最大化最小边：f[i][j] 为 i-j 的所有简单路径中，路径最小边的最大值
 *    转移为 f[i][j] = max(f[i][j], min(f[i][k], f[k][j])); 初始化为 -INF
 * 3. 传递闭包：两点间可达性
 *    转移为 f[i][j] |= f[i][k] && f[k][j]
 * 4. 无向图权值最小环：在 k 层更新前，先检查是否有环 i-k-j-i
 *    更新为 res = min(res, f[i][j] + g[i][k] + g[k][j])
 *    转移为 f[i][j] = min(f[i][j], f[i][k] + f[k][j])
 */