#include <bits/stdc++.h>
using namespace std;

/**
 * 换根 DP 处理树上问题（一般问题需要对于树上每个点作为根的情况下答案的查询）
 * 
 * 分为两次树上 DP 解决：
 * 1. 第一次 dp：
 *      定义 dp[u] 为以 0 作为树根时，子树 u 的状态值
 *      有: dp[u] = merge(f(v))
 *      其中这里的 merge 是可撤销贡献，例如求和；f(v) 是有关例如 dp[v],sz[v] 的函数
 * 2. 第二次 dp，定义 res[u] 为以 u 作为总树根时的答案，初始有 res[0] = dp[0]
 *    现在考虑将 u 通过边 u--v 换根到 v：
 *      2.1. 之前 v 作为 u 的子树，定义 tmp 为在 res[u] 中将 v 子树完全忽略后的新答案（这一步可以无脑的将第一次 dp 时 v 的贡献逆向撤销掉）
 *           一般有: tmp = res[u] - f(v)
 *      2.2. 现在 u 作为 v 的子树，res[v] 分为两部分：原来的 dp[v]，和另一个方向上 tmp 通过 u--v 边加入的贡献
 *           一般有: res[v] = dp[v] + tmp + {cost of using edge u--v}
 * 
 * 注意：在换根时，对于 u 慎用第一次 DFS 的统计信息，例如 cnt,sz,dep 等！！u 此时是根，融合了之前u子树与u父亲那侧的信息，要重新考虑！
 * 例如：应该用 n-sz[v] 而不是 sz[u]，应该用 g[u].size()-1 而不是 cnt[u]
 */
vector<int> rerootDP(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> dp(n);
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        // init dp[u]
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            // dp[u] <-- dp[v]
        }
    }; dfs(0, -1);
    vector<int> res(n); res[0] = dp[0];
    auto reroot = [&](this auto&& reroot, int u, int fa) -> void {
        for (auto& v : g[u]) {
            if (v == fa) continue;
            // tmp    <-- res[u] w/o dp[v]
            // res[v] <-- dp[v]  w/ tmp
            reroot(v, u);
        }
    }; reroot(0, -1);
    return res;
}