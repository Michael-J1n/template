#include <bits/stdc++.h>
using namespace std;

/**
 * 不相交区间
 * 给定若干区间 [l, r]，选择最多个互不相交区间。例如：单机器调度 / 活动安排
 * 
 * 贪心：
 * 按右端点 r 升序排序，每次选能接上的区间。
 * 
 * 复杂度：
 * O(nlogn)
 * 
 * 注意允许端点重合时将判断加上取等
 */
int intervalSchedule(vector<vector<int>>& seg) {
    sort(seg.begin(), seg.end(), [](auto& a, auto& b) {
        if (a[1] != b[1]) return a[1] < b[1];
        return a[0] < b[0];
    });
    int res = 0, last = INT_MIN;
    for (auto& x : seg) {
        int l = x[0], r = x[1];
        if (last < l) ++res, last = r;      // 选择这个区间
        // if (last <= l) ++res, last = r;  // 允许端点重合时
    }
    return res;
}