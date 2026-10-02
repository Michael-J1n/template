#include <bits/stdc++.h>
using namespace std;

/**
 * 区间覆盖
 * 给定若干区间，选最少个区间覆盖目标区间 [s, t]
 * 
 * 贪心：
 * 按左端点 l 升序排序，每次在所有能覆盖当前最左点时选择右端点最大的
 * 
 * 复杂度：
 * O(nlogn)
 * 
 * 注意如果无法覆盖，返回 -1
 */
int intervalCover(vector<vector<int>>& seg, int s, int t) {
    ranges::sort(seg);
    int n = seg.size(), res = 0;
    int cur = s, i = 0;
    while (cur < t) {
        int mx = cur;
        while (i < n && seg[i][0] <= cur) mx = max(mx, seg[i][1]), ++i;
        if (mx == cur) return -1;
        ++res, cur = mx;
    }
    return res;
}