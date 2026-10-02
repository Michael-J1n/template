#include <bits/stdc++.h>
using namespace std;

/**
 * 区间合并
 * 将区间按照重合部分合并
 * 
 * 按照区间左端点排序；每到一个新区间，判断应该留下合并还是新开一个
 * 
 * 复杂度：
 * O(nlogn)
 */
vector<vector<int>> intervalMerge(vector<vector<int>>& intervals) {
    ranges::sort(intervals);
    int n = intervals.size(), L = intervals[0][0], R = intervals[0][1];
    vector<vector<int>> res;
    for (int i = 1; i < n; ++i) {
        int l = intervals[i][0], r = intervals[i][1];
        if (l > R) res.push_back({L, R}), L = l, R = r;
        else R = max(r, R);
    }
    res.push_back({L, R});
    return res;
}