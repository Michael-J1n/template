#include <bits/stdc++.h>
using namespace std;

/**
 * 带权区间选择
 * 选择若干个不相交的区间，使得权值和最大
 * 
 * 状态定义：
 * 先按右端点 r 升序排序后，定义 f[i] 表示只考虑排序后区间 seg[0..i] 中选的最大权值和
 * 
 * 状态转移（选或不选）：
 * 对当前区间 [l, r, w]，二分找到最后一个满足 end < l 的区间 p，按照选或不选考虑有 f[i] = max(f[i - 1], w + f[p])
 * 
 * 复杂度：
 * O(nlogn)，其中 n 为区间数量。
 * 
 * 注意：如果允许两个区间的端点成为一个点，那么二分改成 upper_bound
 */
long long weightedIntervalSchedule(vector<vector<int>>& seg) {
    int n = seg.size();
    sort(seg.begin(), seg.end(), [](auto& a, auto& b) {
        if (a[1] != b[1]) return a[1] < b[1];
        return a[0] < b[0];
    });
    vector<int> ends(n);
    for (int i = 0; i < n; ++i) ends[i] = seg[i][1];
    vector<long long> f(n);
    for (int i = 0; i < n; ++i) {
        int l = seg[i][0], w = seg[i][2];
        int p = lower_bound(ends.begin(), ends.begin() + i, l) - ends.begin() - 1;      // 最后一个不相交的区间编号
        // int p = upper_bound(ends.begin(), ends.begin() + i, l) - ends.begin() - 1;   // 允许端点重合时
        long long cur = 1LL * w + (p >= 0 ? f[p] : 0);
        f[i] = max((i ? f[i - 1] : 0LL), cur);
    }
    return f[n - 1];
}