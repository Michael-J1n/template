#include <bits/stdc++.h>
using namespace std;

/**
 * 求树上最小支配集：每个点要么自己被选，要么至少有一个邻点被选
 * 
 * 状态定义（这里多一状态是由于从底向上时无法做出完整决策，一个点被支配不仅和子树有关，还和父亲有关）：
 * dp[u][0] 为选择 u，u 子树的最小支配集大小；init = 1
 * dp[u][1] 为不选 u，且 u 被至少一个儿子支配，在 u 子树中的最小支配集大小；init = INF（初始没有卷儿子状态时，无任何一个孩子支配）
 * dp[u][2] 为不选 u，且 u 不被任何儿子支配，但被父亲支配，在 u 子树中的最小支配集大小；init = 0
 * 
 * 状态转移（按子树卷积语义）：
 * 1. 选择 u，那么 v 一定能被支配，可以任意选择状态
 *      ndp[u][0] = dp[u][0] + min(dp[v][0], dp[v][1], dp[v][2])
 * 2. 不选 u，且 u 被至少一个孩子支配
 *          - 合并 v 子树前，已经被至少一个孩子支配，这时 v 子树可以选或不选（但 v 不能选择被父亲支配的方案，否则 v 没有孩子，u 也不被选中）
 *          - 合并 v 子树前，v 没有被任何孩子支配，这时 v 被选择作为支配它的孩子
 *      ndp[u][1] = min(dp[u][1] + min(dp[v][0], dp[v][1]), dp[u][2] + dp[v][0])
 * 3. 不选 u，且 u 不被任何孩子支配（但未来被 u 父亲支配），这时 v 不能选，u 也无法成为支配他的父亲
 *      ndp[u][2] = dp[u][2] + dp[v][1]
 * 
 * 答案：根没有父亲了，所以要排除第三个状态，为 min(dp[0][0], dp[0][1])
 * 复杂度：遍历一次，O(n)
 * 
 * 注：这里的最小指的是选点个数；如果要求权值，把初始化的 1 换成 w[u] 即可
 */
int treeDP(vector<vector<int>>& g) {
    const int INF = 1e8;
    int n = g.size();
    vector dp(n, array<int, 3>());
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        dp[u][0] = 1;
        dp[u][1] = INF;
        dp[u][2] = 0;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            array<int, 3> ndp;
            ndp[0] = dp[u][0] + min({dp[v][0], dp[v][1], dp[v][2]});
            ndp[1] = min(dp[u][1] + min(dp[v][0], dp[v][1]), dp[u][2] + dp[v][0]);
            ndp[2] = dp[u][2] + dp[v][1];
            dp[u] = ndp;
        }
    };
    dfs(0, -1);
    return min(dp[0][0], dp[0][1]);
}