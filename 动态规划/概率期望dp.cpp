#include <bits/stdc++.h>
using namespace std;

/**
 * 概率 DP
 * 
 * 定义 dp[state] 为当前处于 state 的概率；初始 dp[start] = 1
 * 
 * 常见转移：刷表法，dp[nstate] += dp[state] * p
 * 
 * 假设状态转移可以建模为拓扑图（i 到 j 有 p 边代表状态 i 到状态 j 的概率为 p），则模板如下：
 */
vector<double> pdp(const vector<vector<pair<int, double>>>& g) {
    int n = g.size();
    vector<double> dp(n); dp[0] = 1;
    for (int i = 0; i < n; ++i) {
        for (auto& [j, p] : g[i]) dp[j] += dp[i] * p;
    }
    return dp;
}

/**
 * 期望 DP
 * 
 * 定义 memo[u] 为从 u 开始到终止状态的期望步数；基本情况 memo[end] = 0
 * 一般采用记忆化搜索，有 memo[u] = sum_{u->v} (w(u,v) + memo[v]) * p(u,v)
 * 
 * 常见转移：画状态机图 + 记忆化
 * 以操作次数为例：E[state] = 1 + sum_i p[i] * E[nstate_i]
 * 更 General 的：E[u] = sum P(u->v) * (E[v] + w(u->v))
 *               其中 E[u] 为在状态节点 u 到结束状态过程的期望；P(u->v) 为状态节点转移到状态节点 v 的概率；w(u->v) 为状态转移时对于最终期望的贡献（例如多一次操作，或多 x 代价）。
 *               需要注意的是有时 w(u->v) 也是所有 u->v 的可能转移的贡献的期望
 * 
 * 假设状态转移可以建模为拓扑图（i 到 j 有 {w,p} 边代表状态 i 可以以概率 p 经历 w 步到达状态 j）
 * 但也可能有有自环的情况，即 E[nstate_i] = E[state]
 */
double edp(const vector<vector<tuple<int, int, double>>>& g) {
    int n = g.size();
    vector<double> memo(n, -1);
    auto dfs = [&](this auto&& dfs, int u) -> double {
        if (memo[u] != -1) return memo[u];
        if (g[u].empty()) return memo[u] = 0;
        double res = 0;
        for (auto& [v, w, p] : g[u]) res += p * (w + dfs(v));
        return memo[u] = res;
    };
    double ans = dfs(0);
}