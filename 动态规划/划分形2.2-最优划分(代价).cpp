#include <bits/stdc++.h>
using namespace std;

/**
 * 求数组按照要求划分得到的最小/最大代价/得分
 * 
 * 状态定义：
 * 定义 f[r] 表示前缀 a[0..r] 的最优代价/得分
 * 
 * 状态转移：
 * 枚举最后一段的左端点 l, f[r] = min/max_{0<=l<=r} (f[l-1] + cost(l,r))
 * 其中 cost(l,r) 表示最后一段子数组 a[l..r] 的划分代价/得分
 * 
 * 复杂度：
 * 记 cost 的复杂度为 C，则总体复杂度为 O(C * n^2)
 */

// 最小划分得分
int minPartitionScore(vector<int>& a) {
    int n = a.size(), INF = 1e9;
    vector<int> f(n, INF);
    auto cost = [&](int l, int r) -> int {
        return 0;
    };
    for (int r = 0; r < n; ++r) {
        for (int l = 0; l <= r; ++l) {
            int pre = l == 0 ? 0 : f[l - 1];
            f[r] = min(f[r], pre + cost(l, r));
        }
    }
    return f[n - 1];
}

// 最大划分得分
int maxPartitionScore(vector<int>& a) {
    int n = a.size(), INF = 1e9;
    vector<int> f(n, -INF);
    auto cost = [&](int l, int r) -> int {
        return 0;
    };
    for (int r = 0; r < n; ++r) {
        for (int l = 0; l <= r; ++l) {
            int pre = l == 0 ? 0 : f[l - 1];
            f[r] = max(f[r], pre + cost(l, r));
        }
    }
    return f[n - 1];
}