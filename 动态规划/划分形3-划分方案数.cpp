#include <bits/stdc++.h>
using namespace std;

/**
 * 计算数组按照要求进行划分的总方案数
 * 
 * 状态定义：
 * 定义 f[r] 表示前缀 a[0..r] 的合法划分方案数
 * 
 * 状态转移：
 * 枚举最后一段的左端点 l, f[r] = SUM_{l=0}^{r} (f[l-1] * [ok(l,r)])
 * 其中 ok(l,r) 表示最后一段子数组 a[l..r] 是否满足要求
 * 
 * 复杂度：
 * 记 ok 的复杂度为 C，则总体复杂度为 O(C * n^2)
 */

int numPartition(vector<int>& a) {
    int n = a.size();
    vector<int> f(n);
    auto ok = [&](int l, int r) -> int {
        return 1;
    };
    for (int r = 0; r < n; ++r) {
        for (int l = 0; l <= r; ++l) {
            if (!ok(l, r)) continue;
            int pre = l == 0 ? 1 : f[l - 1];
            f[r] += pre;
        }
    }
    return f[n - 1];
}