#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;

/**
 * 树形 DP：状态卷积合并
 *
 * 状态定义：
 * dp[u][s] 表示 u 子树处理完若干儿子后，处于状态 s 的最优值
 *
 * 状态转移：
 * 每处理一个儿子 v，用旧 dp[u] 和 dp[v] 做状态卷积：
 *      ndp[ns] = max/min(ndp[ns], dp[u][i] + dp[v][j] + extra)
 * 这即是说，在返回 v 后，使用 dp[v] 中的状态信息，和 u 与前几个儿子得到的中间信息 dp[u] 结合，得到 ndp[u]；其中：i 是 u 当前状态，j 是 v 子树状态，ns = trans(i, j, u, v) 是合并后的新状态（例如树上背包的 i+j）
 *
 * 复杂度：O(n * S^2)，S 为状态数
 * 注意：状态要初始化状态值为 -INF，代表不可达
 */
vector<vector<int>> treeDP(vector<vector<int>>& g, int S) {
    int n = g.size();
    vector<vector<int>> dp(n, vector<int>(S + 1, -INF));
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        // init dp[u]
        dp[u][0] = 0;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            auto ndp = dp[u];  // 如果这个子树强制要做出贡献，用 vector<int> ndp(S, -INF);
            for (int i = 0; i <= S; ++i) {
                if (dp[u][i] == -INF) continue;
                for (int j = 0; j <= S; ++j) {
                    if (dp[v][j] == -INF) continue;
                    // ns <-- i, j
                    int ns = i + j;
                    if (ns < 0 || ns > S) continue;
                    // ndp[ns] <-- dp[u][i], dp[v][j]
                    ndp[ns] = max(ndp[ns], dp[u][i] + dp[v][j]);
                }
            }
            dp[u] = ndp;
        }
    }; dfs(0, -1);
    return dp;
}