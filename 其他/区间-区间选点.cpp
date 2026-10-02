#include <bits/stdc++.h>
using namespace std;

/**
 * 区间选点
 * 给定若干区间，放置尽量少的点，使每个区间都包含一个点。例如：射气球 / Interval Stabbing
 * 
 * 贪心：
 * 按右端点 r 升序排序，维护上一个点位置 last；若当前区间不包含 last，就在当前右端点放置一个点
 * 
 * 复杂度：
 * O(nlogn)
 * 
 * 注意本质上与不相交区间相同
 */
int stabPoints(vector<vector<int>>& seg) {
    sort(seg.begin(), seg.end(), [](auto& a, auto& b) {
        if (a[1] != b[1]) return a[1] < b[1];
        return a[0] < b[0];
    });
    int res = 0, last = INT_MIN;
    for (auto& x : seg) {
        int l = x[0], r = x[1];
        if (last < l) ++res, last = r;
    }
    return res;
}