#include <bits/stdc++.h>
using namespace std;

/**
 * Hopcroft-Karp 求二分图最大匹配
 * 给定一个二分图，求出二分图最多可以选中多少条没有公共端点的边
 *
 * 图的定义：
 * 左部点编号为 0..n-1，右部点编号为 0..m-1；g[u] 存左部点 u 能连到的所有右部点 v
 *
 * 返回：
 * 二分图的最大匹配数，matchL / matchR 会被写成最终匹配方案；若 matchL/R[u]=-1 代表未匹配
 * 
 * 复杂度：
 * 设 V = n + m，E = 所有边数
 * 时间复杂度 O(E * sqrt(V))
 */
int bipartiteMatching(vector<vector<int>>& g, int m, vector<int>& matchL, vector<int>& matchR) {
    int n = g.size(), res = 0, lim = -1;
    matchL.assign(n, -1), matchR.assign(m, -1);
    vector<int> dep(n);
    auto bfs = [&]() -> bool {
        queue<int> q;
        fill(dep.begin(), dep.end(), -1);
        lim = -1;
        for (int i = 0; i < n; ++i) {
            if (matchL[i] == -1) dep[i] = 0, q.push(i);
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (lim != -1 && dep[u] >= lim) continue;
            for (auto& v : g[u]) {
                int x = matchR[v];
                if (x == -1) lim = dep[u];
                else if (dep[x] == -1) dep[x] = dep[u] + 1, q.push(x);
            }
        }
        return lim != -1;
    };
    auto dfs = [&](this auto&& dfs, int u) -> bool {
        for (auto& v : g[u]) {
            int x = matchR[v];
            if (x == -1 || (dep[x] == dep[u] + 1 && dep[x] <= lim && dfs(x))) {
                matchL[u] = v, matchR[v] = u;
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
    return res;
}

/**
 * 二分图最大匹配模型的要素：
 * 1. “0”要素：节点能分成两个独立的集合，每个集合内部有 0 条边
 * 2. “1”要素：每个节点只能最多与 1 条匹配边相连
 * 即：每条边被选中后，它的两顶点就不能参与任何其他决定了
 * 
 * 常见题目模型：有多个可以“互相牵制”的决策，如果选了某个物品，就有对于别的物品的限制或牵制，有”牵一发而动全身“的感觉
 * 常见模型：网格图相邻点/行-列为左部和右部
 * 
 * 例：
 * 给定网格图和一些被 ban 掉的格子，求最多放下多少个车棋使他们互不攻击（车棋可以攻击同行/列）
 * 选择一个棋子后，它所在的行与列就不能再放任何棋子，因此启发进行二分图建模：
 * 左部以行做点，右部以列做点，格子 (i,j) 如果没有被 ban 就把 i-j 连边；即一个合法放置位置代表一条边
 * “0”要素：行与行，列与列之间无边
 * “1”要素：每个行，最多选中一个列，即最多一条边作为匹配
 * 因此求出最大匹配即为最多放置的个数
 */


/**
 * 另：匈牙利朴素算法求最大匹配
 * 
 * g[u]：左部点能连接到的所有右部点；match[v]：当前与右部点 v 匹配的左部点，-1 表示未匹配
 * 增广路定义：起点重点都为为匹配点，边按照 非匹配，匹配 交替，因此第一条与最后一条也都是非匹配边
 * 
 * 复杂度 O(VE)
 */
int bipartiteMatching(vector<vector<int>>& g, int m, vector<int>& match) {
    int n = g.size(), res = 0, timer = 0;
    match.assign(m, -1);
    vector<int> vis(m);
    // dfs(u)：左部点是否能找到一条增广路；vis[v]：右部点在哪一轮增广中被访问过
    auto dfs = [&](this auto&& dfs, int u) -> bool {
        for (auto& v : g[u]) {
            if (vis[v] == timer) continue;
            vis[v] = timer;
            if (match[v] == -1 || dfs(match[v])) {
                // v 没有匹配，或发现了从 match[v] 开始的增广路，将路径上的匹配属性对换
                match[v] = u;
                return true;
            }
        }
        return false;
    };
    for (int i = 0; i < n; ++i) {
        ++timer;
        if (dfs(i)) ++res;
    }
    return res;
}