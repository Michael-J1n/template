#include <bits/stdc++.h>
using namespace std;

/**
 * 区间合并
 * 将区间按照重合部分合并
 * 
 * 按照区间左端点排序；每到一个新区间，判断应该留下合并还是新开一个
 * 
 * 复杂度：O(n log n)
 */
vector<pair<int, int>> intervalMerge(vector<pair<int, int>>& segs) {
    ranges::sort(segs);
    int n = segs.size(), L = segs[0].first, R = segs[0].second;
    vector<pair<int, int>> res;
    for (int i = 1; i < n; ++i) {
        auto& [l, r] = segs[i];
        if (l > R) res.push_back({L, R}), L = l, R = r;
        else R = max(r, R);
    }
    res.push_back({L, R});
    return res;
}
