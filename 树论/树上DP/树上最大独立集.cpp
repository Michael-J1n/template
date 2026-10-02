#include <bits/stdc++.h>
using namespace std;

/**
 * 求树上最大独立集大小：选点后，不能再选邻点
 * 
 * 状态定义：
 * dp[u][0] 为选了 u，u 子树中最大独立集大小；init = 1
 * dp[u][1] 为不选 u，u 子树中最大独立集大小；init = 0
 * 
 * 状态转移（按子树卷积语义）：
 * 1. 选了 u，儿子一个都不能选：
 *      ndp[u][0] = dp[u][0] + dp[v][1]
 * 2. 不选 u，儿子可以选或不选：
 *      ndp[u][1] = dp[u][1] + max(dp[v][0], dp[v][1])
 * 
 * 答案：根无限制，因此 max(dp[0][0], dp[0][1])
 * 复杂度：遍历一遍，O(n)
 * 
 * 注：这里的最大指的是选点个数；如果要求权值，把初始化的 1 换成 w[u] 即可
 */
int treeDP(vector<vector<int>>& g) {
    int n = g.size();
    vector dp(n, array<int, 2>());
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        dp[u][0] = 1;
        dp[u][1] = 0;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            dp[u][0] += dp[v][1];
            dp[u][1] += max(dp[v][0], dp[v][1]);
        }
    };
    dfs(0, -1);
    return max(dp[0][0], dp[0][1]);
}