#include <bits/stdc++.h>
using namespace std;

/**
 * 求一颗带权树，树上选出恰好 k 个点的最大权值和
 * 
 * 状态定义：
 * dp[u][s] 为在子树 u 中选择了恰好 s 个点的最大权值和；
 * 
 * 初始化：
 * 不选点，没有权值 dp[u][0] = 0
 * 在卷孩子前，恰好只选一个，因此只能选自己 dp[u][1] = w[u]
 * 
 * 状态转移（按子树卷积语义）：
 * 考虑合并子树 v 之前，u 上选了 i 个点，这时若子树 v 贡献 j 个点，总共可以贡献 i + j 个点
 *      ndp[u][i + j] = max(ndp[u][i + j], dp[u][i] + dp[v][j])
 * 
 * 答案：dp[0][k]
 * 复杂度：遍历一次，每次遍历 k^2 个状态，因此 O(k^2 * n)
 */
int treeDP(vector<vector<int>>& g, vector<int>& w, int k) {
    const int NINF = -0x3f3f3f3f;
    int n = g.size();
    vector dp(n, vector<int>(k + 1, NINF));
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        dp[u][0] = 0;
        dp[u][1] = w[u];
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            vector<int> ndp(k + 1, NINF);
            for (int i = 0; i <= k; ++i) {
                if (dp[u][i] == NINF) continue;
                for (int j = 0; j + i <= k; ++j) {
                    if (dp[v][j] == NINF) continue;
                    ndp[i + j] = max(ndp[i + j], dp[u][i] + dp[v][j]);
                }
            }
            dp[u] = ndp;
        }
    };
    dfs(0, -1);
    return dp[0][k];
}