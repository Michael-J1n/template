#include <bits/stdc++.h>
using namespace std;

/**
 * 树形 DP 求带权树直径，允许负权边。
 * 定义：直径为至少包含一条边的简单路径最大权值和，要求 n >= 2。
 *
 * d[x]：从 x 往下走的最大非负链，0 表示不接；cur = w + max(0, d[y])：经过边 x-y 的链。
 *
 * 注意：res 初始化为 -INF，不能为 0；否则全负边会错。
 * 若允许空路径，答案取 max(0, res)。
 * 复杂度：O(n)
 */
int treeDiameter(vector<vector<pair<int, int>>>& g) {
    const int INF = 0x3f3f3f3f;
    int n = g.size(), res = -INF;
    vector<int> d(n);
    auto dfs = [&](this auto&& dfs, int x, int fa) -> void {
        for (auto& [y, w] : g[x]) {
            if (y == fa) continue;
            dfs(y, x);
            int cur = w + max(0, d[y]);
            res = max(res, cur);
            res = max(res, d[x] + cur);
            d[x] = max(d[x], cur);
        }
    };
    if (n == 1) return 0;
    dfs(0, -1);
    return res;
}