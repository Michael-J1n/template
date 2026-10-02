#include <bits/stdc++.h>
using namespace std;

/**
 * 求两个序列的公共最长上升子序列
 * 
 * 定义 f[i][j] 为考虑 a[0..i]，以 b[j] 结尾的最长公共上升子序列长度（注意到这个状态设计是 LIS 与 LCS 结合而来）
 * - 若 a[i] != b[j]，那么 f[i][j] = f[i - 1][j]
 * - 若 a[i] == b[j]，枚举上一位填了谁，有 f[i][j] = max_{k < j, b[k] < b[j] = a[i]} f[i - 1][k] + 1
 *   转移时固定 i，顺序枚举 j，k < j 维度天然满足，只需要维护前缀 max_{b[k] < a[i]} f[i - 1][k] 即可
 * 实现时可以滚动数组优化；答案即为 max_{j}(f[n - 1][j])
 */
int LCIS(vector<int>& a, vector<int>& b) {
    int n = a.size(), m = b.size();
    vector<int> f(n, 0);
    for (int i = 0; i < n; ++i) {
        int cur = 0;
        for (int j = 0; j < m; ++j) {
            if (a[i] == b[j]) f[j] = max(f[j], cur + 1);
            if (b[j] < a[i]) cur = max(cur, f[j]);
        }
    }
    return ranges::max(f);
}