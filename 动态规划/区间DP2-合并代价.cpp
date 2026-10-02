#include <bits/stdc++.h>
using namespace std;

/**
 * 区间 DP
 * 模板以最小区间合并得分（最大改为 -INF，转移改为 max）为例
 * 
 * 状态定义：
 * 定义 f[l][r] 表示子数组 a[l..r] 的最优代价/得分
 *
 * 状态转移：
 * 初始化 f[i][i]=0，若单点选择有代价需要修改
 * 先枚举长度 r-l+1，再枚举左端点 l，最后枚举合并/划分的位置 k, f[l][r] = min/max_{l<=k<r} (f[l][k] + f[k+1][r] + cost(l,r))
 * 其中 cost(l,r) 表示把区间 a[l..r] 合并成一个整体时额外产生的代价/得分
 *
 * 复杂度：
 * 记 cost 的复杂度为 C，则总体复杂度为 O(C * n^3)
 */
const long long LINF = 4e18;
long long minIntervalScore(vector<int>& a) {
    int n = a.size();
    vector f(n, vector<long long>(n, LINF));
    auto cost = [&](int l, int r) -> long long {
        return r - l + 1;
    };
    for (int i = 0; i < n; ++i) f[i][i] = 0;
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            for (int k = l; k < r; ++k) {
                f[l][r] = min(f[l][r], f[l][k] + f[k + 1][r] + cost(l, r));
            }
        }
    }
    return f[0][n - 1];
}

/**
 * 常见题型：
 * 相邻合并；消除游戏；括号匹配；回文删除
 * 注意数据范围，在 200 左右要想到区间dp（由于这是立方复杂度的算法）
 */