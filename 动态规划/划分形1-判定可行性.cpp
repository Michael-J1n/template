#include <bits/stdc++.h>
using namespace std;

/**
 * 判定数组能否按照要求划分
 * 
 * 状态定义：
 * 定义 f[r] 表示前缀 a[0..r] 能否被合法划分
 * 
 * 状态转移：
 * 枚举最后一段的左端点 l, f[r] = OR_{l=0}^{r} (f[l-1] AND ok(l,r))
 * 其中 ok(l,r) 表示最后一段子数组 a[l..r] 是否满足要求
 * 
 * 复杂度：
 * 记 ok 的复杂度为 C，则总体复杂度为 O(C * n^2)
 */

int canPartition(vector<int>& a) {
    int n = a.size();
    vector<int> f(n);
    auto ok = [&](int l, int r) -> int {
        return 1;
    };
    for (int r = 0; r < n; ++r) {
        for (int l = 0; l <= r; ++l) {
            if ((l == 0 || f[l - 1]) && ok(l, r)) {
                f[r] = 1;
                break;
            }
        }
    }
    return f[n - 1];
}