#include <bits/stdc++.h>
using namespace std;

/**
 * 给定一个非严格递增的数组，找到 range [vL, vR] 出现的下标区间 [L, R]，如果不存在一个可能的或合法区间，返回空
 * a 为 0-based 下标；在权值线段树中为排序去重后的值域列表
 *  
 * 应用：
 * 1. 不带修改的区间频率查询
 * 2. 在权值线段树优化 dp 中较常用，其下标为数组值域值 nums[i]，其对应的树值为 nums[i] 对应的f值
 * 在 dp 中转移涉及到一个连续的值域区间，就可以在 tr 中使用 get_range 来查询对应连续值域区间对应的树中下标范围
 */
pair<int, int> get_range(const vector<long long>& a, long long vL, long long vR) {
    if (vL > vR) return {};
    int L = lower_bound(a.begin(), a.end(), vL) - a.begin();
    int R = upper_bound(a.begin(), a.end(), vR) - a.begin() - 1;
    if (L > R) return {};
    return {L, R};
}