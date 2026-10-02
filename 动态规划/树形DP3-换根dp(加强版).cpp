#include <bits/stdc++.h>
using namespace std;

/**
 * 换根 DP
 * 解决树上不定根统计问题，且贡献不可撤销（如果排除儿子是可撤销贡献，建议用前一份换根dp模板，常数小很多）
 * 此模板以求树上每个点偏心距为例（到最远点的边数）。注意 max 是不可撤销贡献。
 * 易知 res[u] = 1+max(dp[v],up[u])；注意这里 mx 的单位元是 -1：如果四周都没有点，1+(-1)=0 没有继续扩展的边了
 * 
 * 状态定义：
 * dp[u] = 以 0 为总树根时，从 u 看 u 子树这一块的状态（包含 u 的贡献）
 * up[u] = 从 fa[u] 看 u 子树外这一块的状态         （不包含 u 的贡献）
 * res[u] = 以 u 作为总树根时的答案
 * 
 * 第二次 dp 时，对于 u 的每个邻居 g[u][i]：
 *      val[i] = up[u] or dp[v]
 *      若 g[u][i] == fa，则 val[i] 表示从 fa 看 u 子树外这一块的状态
 *      若 g[u][i] == v，则 val[i] 表示从 v 看 v 子树这一块的状态
 * 
 * 前后缀分解：pre[i] = val[0..i]；suf[i] = val[i..m-1]
 *
 * 复杂度：O(n)（但时空常数大）
 * 
 * 注意：
 * 1. up[u] 和 dp[v] 是同一层语义，都是从邻居那一侧看某个连通块的状态；n=1 时需要特判
 * 2. 计算 up[u] 的时候，有时需要特判当 father[u]=-1 的情况
 */
vector<int> rerootDP(vector<vector<int>>& g) {
    int n = g.size();
    if (n == 1) {
        return {0};
    }
    vector<int> dp(n);
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        // init dp[u]
        int mx = -1;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            // dp[u] <-- dp[v]
            mx = max(mx, dp[v]);
        }
        dp[u] = 1 + mx;
    }; dfs(0, -1);
    vector<int> up(n), res(n);
    auto reroot = [&](this auto&& reroot, int u, int fa) -> void {
        int m = g[u].size();
        // val[i] = i 方向邻居的贡献；pre[i] = val[0..i], suf[i] = val[i..m-1] 按照题目要求进行前后缀分解计算
        vector<int> val(m), pre(m), suf(m);
        for (int i = 0; i < m; ++i) {
            int v = g[u][i];
            if (v == fa) val[i] = up[u];
            else val[i] = dp[v];
        }
        // 前后缀分解
        pre[0] = val[0];
        for (int i = 1; i < m; ++i) pre[i] = max(pre[i - 1], val[i]);
        suf[m - 1] = val[m - 1];
        for (int i = m - 2; ~i; --i) suf[i] = max(suf[i + 1], val[i]);
        // res[u] <-- val[0],...,val[m-1] 所有邻点的贡献
        res[u] = 1 + pre[m - 1];
        // up[v] <-- 所有非 v 的邻点贡献
        for (int i = 0; i < m; ++i) {
            int v = g[u][i];
            if (v == fa) continue;
            up[v] = 1 + max(i ? pre[i - 1] : -1, i + 1 < m ? suf[i + 1] : -1);
            reroot(v, u);
        }
    }; reroot(0, -1);
    return res;
}