#include <bits/stdc++.h>
using namespace std;

/**
 * 计算数组按照要求进行划分，且约束了划分个数（恰好/最多/最少划分为 k 段）的最优值/方案数
 * 
 * 状态定义：
 * 定义 f[t][r] 表示把前缀 a[0..r] 恰好划分为 t 段的最优值
 * 
 * 状态转移：
 * 初始化 f[1][r] = cost(0,r)
 * 枚举最后一段的左端点 l, f[t][r] = min/max_{l=t-1}^{r} (f[t-1][l-1] + cost(l,r))
 * 其中 ok(l,r) 表示最后一段子数组 a[l..r] 是否满足要求；l>=t-1 是由于前面至少要有 t-1 个元素去划分为 t-1 段
 * 
 * 复杂度：
 * 记 cost 的复杂度为 C，则总体复杂度为 O(C * k * n^2)
 * 
 * 注意：
 * 至多 k 段可以转化为 min/max {f[t][n - 1]: t<=k}; 至少 k 段可以转化为 min/max {f[t][n - 1]: t >= k}
 */

// 恰好 k 段的最小代价
int minPartitionK(vector<int>& a, int k) {
    int n = a.size(), INF = 1e9;
    vector<vector<int>> f(k + 1, vector<int>(n, INF));
    auto cost = [&](int l, int r) -> int {
        return 0;
    };
    for (int r = 0; r < n; ++r) f[1][r] = cost(0, r);
    for (int t = 2; t <= k; ++t) {
        for (int r = t - 1; r < n; ++r) {
            for (int l = t - 1; l <= r; ++l) {
                f[t][r] = min(f[t][r], f[t - 1][l - 1] + cost(l, r));
            }
        }
    }
    return f[k][n - 1];
}

// 恰好 k 段的最大代价
int maxPartitionK(vector<int>& a, int k) {
    int n = a.size(), INF = 1e9;
    vector<vector<int>> f(k + 1, vector<int>(n, -INF));
    auto cost = [&](int l, int r) -> int {
        return 0;
    };
    for (int r = 0; r < n; ++r) f[1][r] = cost(0, r);
    for (int t = 2; t <= k; ++t) {
        for (int r = t - 1; r < n; ++r) {
            for (int l = t - 1; l <= r; ++l) {
                f[t][r] = max(f[t][r], f[t - 1][l - 1] + cost(l, r));
            }
        }
    }
    return f[k][n - 1];
}