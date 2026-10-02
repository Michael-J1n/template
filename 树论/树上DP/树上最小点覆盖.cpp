#include <bits/stdc++.h>
using namespace std;

/**
 * 求树上最小点覆盖：每条边都至少有一个顶点被选中
 * 
 * 状态定义：
 * dp[u][0] 为选了 u，u 子树中的最小点覆盖大小；init = 1
 * dp[u][1] 为不选 u，u 子树中的最小点覆盖大小；init = 0
 * 
 * 状态转移（按子树卷积语义）：
 * 1. 若选择 u，那么 u-v 被覆盖，儿子可以选或不选
 *      ndp[u][0] = dp[u][0] + min(dp[v][0], dp[v][1])
 * 2. 若不选 u，那么需要覆盖 u-v 必须选择儿子
 *      ndp[u][1] = dp[u][1] + dp[v][0]
 * 
 * 答案：根无限制，因此是 min(dp[0][0], dp[0][1])
 * 复杂度：遍历一次，O(n)
 * 
 * 注：这里的最小指的是选点个数；如果要求权值，把初始化的 1 换成 w[u] 即可
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
            dp[u][0] += min(dp[v][0], dp[v][1]);
            dp[u][1] += dp[v][0];
        }
    };
    dfs(0, -1);
    return min(dp[0][0], dp[0][1]);
}