#include <bits/stdc++.h>
using namespace std;

/**
 * 求数组按照要求最少/最多能够划分多少段
 * 
 * 状态定义：
 * 定义 f[r] 表示前缀 a[0..r] 的最优划分数
 * 
 * 状态转移：
 * 枚举最后一段的左端点 l, f[r] = min/max_{0<=l<=r AND ok(l,r)} (f[l-1] + 1)
 * 其中 ok(l,r) 表示最后一段子数组 a[l..r] 是否满足要求
 * 
 * 复杂度：
 * 记 ok 的复杂度为 C，则总体复杂度为 O(C * n^2)
 */

// 最少划分为几段
int minPartition(vector<int>& a) {
    int n = a.size(), INF = 1e9;
    vector<int> f(n, INF);
    auto ok = [&](int l, int r) -> int {
        return 1;
    };
    for (int r = 0; r < n; ++r) {
        for (int l = 0; l <= r; ++l) {
            if (!ok(l, r)) continue;
            int pre = l == 0 ? 0 : f[l - 1];
            f[r] = min(f[r], pre + 1);
        }
    }
    return f[n - 1];
}

// 最多划分为几段
int maxPartition(vector<int>& a) {
    int n = a.size(), INF = 1e9;
    vector<int> f(n, -INF);
    auto ok = [&](int l, int r) -> int {
        return 1;
    };
    for (int r = 0; r < n; ++r) {
        for (int l = 0; l <= r; ++l) {
            if (!ok(l, r)) continue;
            int pre = l == 0 ? 0 : f[l - 1];
            f[r] = max(f[r], pre + 1);
        }
    }
    return f[n - 1];
}