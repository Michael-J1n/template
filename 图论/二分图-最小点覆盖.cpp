#include <bits/stdc++.h>
using namespace std;

/**
 * 二分图最小点覆盖：图中最小点集 S 使得图中任意一条边都有至少一个端点属于 S
 * Kőnig 定理: 二分图最小点覆盖大小 = 最大匹配大小
 * 
 * 图定义：
 * 左部点编号 0..n-1，右部点编号 0..m-1；g[u] 表示左部点 u 连向哪些右部点
 *
 * 返回：
 * 最大匹配大小；matchL/R 是最大匹配的端点，coverL/R 是最小点覆盖的端点
 *
 * 复杂度：瓶颈在于求最大匹配， O(E sqrt(V))
 */
int bipartiteMinVertexCover(vector<vector<int>>& g, int m, vector<int>& matchL, vector<int>& matchR, vector<int>& coverL, vector<int>& coverR) {
    int n = g.size(), res = 0;
    matchL.assign(n, -1);
    matchR.assign(m, -1);
    vector<int> dep(n);
    auto bfs = [&]() -> bool {
        queue<int> q;
        fill(dep.begin(), dep.end(), -1);
        for (int i = 0; i < n; ++i) {
            if (matchL[i] == -1) {
                dep[i] = 0;
                q.push(i);
            }
        }
        bool ok = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& v : g[u]) {
                int x = matchR[v];
                if (x == -1) {
                    ok = true;
                } else if (dep[x] == -1) {
                    dep[x] = dep[u] + 1;
                    q.push(x);
                }
            }
        }
        return ok;
    };
    auto dfs = [&](this auto&& dfs, int u) -> bool {
        for (auto& v : g[u]) {
            int x = matchR[v];
            if (x == -1 || dep[x] == dep[u] + 1 && dfs(x)) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        dep[u] = -1;
        return false;
    };
    while (bfs()) {
        for (int i = 0; i < n; ++i) {
            if (matchL[i] == -1 && dfs(i)) ++res;
        }
    }
    vector<int> visL(n), visR(m);
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (matchL[i] == -1) {
            visL[i] = 1;
            q.push(i);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& v : g[u]) {
            if (matchL[u] == v || visR[v]) continue;
            visR[v] = 1;
            int x = matchR[v];
            if (x != -1 && !visL[x]) {
                visL[x] = 1;
                q.push(x);
            }
        }
    }
    coverL.clear();
    coverR.clear();
    for (int i = 0; i < n; ++i) {
        if (!visL[i]) coverL.emplace_back(i);
    }
    for (int i = 0; i < m; ++i) {
        if (visR[i]) coverR.emplace_back(i);
    }
    return res;
}

/**
 * 二分图最小点覆盖“2”要素：
 * 每条边有两个端点，二者**至少且只能选择一个**
 * 即：每条边要么选择它的左端点，要么选择右端点
 * 
 * 问题常见模型：每件事必须在两个选择中做决策：要么选 a，要么选 b（ab连边）；
 * 一种决策可以同时对别的事件产生影响（一条边允许覆盖多个顶点），求最小的不同点数
 * 
 * 例1：
 * 给定一个棋盘，上面有些位置是敌人；每次可以选择一行/一列消除，求最小多少次可以消除掉
 * 每个敌人要么选择消除它所在的行，要么选择它所在的列，这启发用二分图建模：
 * 左部点为行，右部点位列，敌人所在 (i,j) 就将 i-j 连边；那么每条边要么选择消除行（选择左端点）要么选择消除列（选择右端点）
 * 因此求二分图最小点覆盖即为答案
 * 
 * 例2：
 * 给定两个机器和 n 个任务，每个机器有 m 种模式
 * 对于每个任务，给定 a[i] 和 b[i] 代表在机器上的模式，任务顺序可以任意，求最小切换模式的次数
 * 考虑每个任务要么用 A 机器的 a[i] 模式，要么用 B 机器的 b[i] 模式
 * 左部放 m 个 A 机器的模式，右部放 m 个 B 机器的模式，对于任务 i 就在 a[i] 和 b[i] 连边（必须选择一个）
 * 求出最小点覆盖，就是最小切换次数
 */