#include <bits/stdc++.h>
using namespace std;

/**
 * 传递闭包：图上二元关系 a * b, b * c 可以传递出 a * c，求每个顶点对是否满足二元关系；例如：是否存在一条路径到达，两数的大于小于关系
 * 
 * 定义 can[i][j] 为 i 是否可达 j，枚举中转点有 can[i][j] |= can[i][k] && can[k][j]
 * 
 * 注意：如果是有向闭包，那么如果存在一对 (i,j) 满足 can[i][j]&&can[j][i]，那么说明发现了矛盾
 */

 // 朴素版
vector<vector<int>> transitiveClosure(int n, vector<vector<int>>& edges) {
    vector can(n, vector<int>(n));
    for (int i = 0; i < n; ++i) can[i][i] = 1;
    for (auto& e : edges) {
        int u = e[0], v = e[1];
        can[u][v] = 1;
        can[v][u] = 1;  // 若关系有向删除此行
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                can[i][j] = can[i][j] | (can[i][k] & can[k][j]);
            }
        }
    }
    return can;
}

// bitset 优化版；can[i] 表示 i 的可达集合
const int N = 2005;
bitset<N> can[N];
void bitsetFloyd(int n, vector<vector<int>>& edges) {
    for (int i = 0; i < n; ++i) can[i].reset();
    for (int i = 0; i < n; ++i) can[i][i] = 1;
    for (auto& e : edges) {
        int u = e[0], v = e[1];
        can[u][v] = 1;
        can[v][u] = 1;  // 若关系有向删除此行
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (can[i][k]) can[i] |= can[k];
        }
    }
}