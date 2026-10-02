#include <bits/stdc++.h>
using namespace std;

/**
 * 二分图最大独立集：最大的顶点子集使得任何两个顶点之间都没有边相连
 * 考虑所有顶点去掉最小点覆盖后，剩下任意两点没有边（否则这条边没有被点覆盖所包含，矛盾）
 * 因此最大独立集数量 = n + m - 最小点覆盖数 = n + m - 最大匹配数
 *
 * 图定义：
 * 左部点编号 0..n-1，右部点编号 0..m-1；g[u] 表示左部点 u 连向哪些右部点
 *
 * 返回：
 * 最大独立集大小，matchL/R 为最大匹配的匹配边，independentL/R 为独立集元素
 *
 * 复杂度：
 * O(E sqrt(V))
 */
int bipartiteMaxIndependentSet(vector<vector<int>>& g, int m, vector<int>& matchL, vector<int>& matchR, vector<int>& independentL, vector<int>& independentR) {
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
    independentL.clear();
    independentR.clear();
    for (int i = 0; i < n; ++i) {
        if (visL[i]) independentL.emplace_back(i);
    }
    for (int i = 0; i < m; ++i) {
        if (!visR[i]) independentR.emplace_back(i);
    }
    return n + m - res;
}

/**
 * 二分图最大独立集常见建模：
 * 给定一些互斥关系，并且互斥两端属于不同集合，且一共能划分出两个集合
 * 那么求最多能放多少个不互斥的元素即为求出最大独立集
 * 
 * 例：
 * 给一个棋盘，有一些位置被 ban。求最多可以放置多少个互不攻击的马棋（马可以攻击八个方向的日字）
 * 首先发现日字两端棋盘颜色必一黑一白，即为互斥关系两端属于不同集合
 * 因此给一对可以互相攻击的关系两端连边，求出最后这个二分图的最大独立集即为最多放置的棋子数量
 */