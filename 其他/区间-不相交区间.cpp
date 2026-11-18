#include <bits/stdc++.h>
using namespace std;

/**
 * 不相交区间
 * 给定若干区间 [l, r]，选择最多个互不相交区间。例如：单机器调度 / 活动安排
 * 
 * 贪心：
 * 按右端点 r 升序排序，每次选能接上的区间。
 * 
 * 复杂度：O(n log n)
 * 
 * 注意允许端点重合时将判断加上取等
 */
int intervalSchedule(vector<pair<int, int>>& segs) {
    sort(segs.begin(), segs.end(), [](auto& a, auto& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });
    int res = 0, last = INT_MIN;
    for (auto& [l, r] : segs) {
        if (last < l) ++res, last = r;      // 选择这个区间
        // if (last <= l) ++res, last = r;  // 允许端点重合时
    }
    return res;
}
