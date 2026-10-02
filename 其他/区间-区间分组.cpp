#include <bits/stdc++.h>
using namespace std;

/**
 * 区间分组
 * 给定一些区间，把区间分成最少的组，使得每组内部区间互不相交。例如：任务调度 / 会议室
 * 
 * 贪心：
 * 按左端点 l 排序，维护一个每组当前右端点的 multiset；每次二分查可以接入的组
 * 
 * 复杂度：
 * O(nlogn)
 * 
 * 注意允许端点重合时二分改为 upper_bound
 */
int minGroups(vector<vector<int>>& seg) {
    ranges::sort(seg);
    multiset<int> ends;
    for (auto& x : seg) {
        int l = x[0], r = x[1];
        auto it = ends.lower_bound(l);     // 接入这一组
        // auto it = ends.upper_bound(l);  // 允许端点重合时
        if (it != ends.begin()) ends.erase(prev(it));
        ends.insert(r);
    }
    return ends.size();
}