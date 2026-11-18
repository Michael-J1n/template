#include <bits/stdc++.h>
using namespace std;

/**
 * 区间覆盖
 * 给定若干区间，选最少个区间覆盖目标区间 [s, t]
 * 
 * 贪心：
 * 按左端点 l 升序排序，每次在所有能覆盖当前最左点时选择右端点最大的
 * 
 * 复杂度：O(n log n)
 * 
 * 注意如果无法覆盖，返回 -1
 */
int intervalCover(vector<pair<int, int>>& segs, int s, int t) {
    ranges::sort(segs);
    int n = segs.size(), res = 0;
    int cur = s, i = 0;
    while (cur < t) {
        int mx = cur;
        while (i < n && segs[i].first <= cur) {
            auto& [l, r] = segs[i];
            mx = max(mx, r), ++i;
        }
        if (mx == cur) return -1;
        ++res, cur = mx;
    }
    return res;
}
