#include <bits/stdc++.h>
using namespace std;

/**
 * 给定一个非严格递增的数组，找到 range [vL, vR] 出现的下标区间 [L, R]，如果不存在一个可能的或合法区间，返回空
 * a 为 0-based 下标；在权值线段树中为排序去重后的值域列表
 *  
 * 应用：
 * 1. 不带修改的区间频率查询
 * 2. 在权值线段树优化dp中较常用，其下标为数组值域值nums[i]，其对应的树值为nums[i]对应的f值
 * 在dp中转移涉及到一个连续的值域区间，就可以在tr中使用getRange来查询对应连续值域区间对应的树中下标范围
 */
template<class T>
vector<int> getRange(const vector<T>& a, T vL, T vR) {
    if (vL > vR) return {};
    int L = lower_bound(a.begin(), a.end(), vL) - a.begin();
    int R = upper_bound(a.begin(), a.end(), vR) - a.begin() - 1;
    if (L > R) return {};
    return {L, R};
}


// 例题：LC 2080
/**
 * 题意：给定数组，每次查询 [l, r] 中值 v 的出现次数
 * 此即为区间不带修频率查询，n<=1e5, v<=1e4
 * 做法：
 * 对于每个值都开一个<int, vector<int>> 的哈希表，mp[v] 代表 v 出现的所有下标（天然有序）
 * 那么对于 [l, r] 上对于 v 的查询，在 mp[v] 上做 [l, r] 出现的下标区间，这个区间的区间长度即为所需频率值
 * 复杂度 O(n + qlogn)
 */
class RangeFreqQuery {
public:
    RangeFreqQuery(vector<int>& arr) {
        int n = arr.size();
        for (int i = 0; i < n; ++i) mp[arr[i]].emplace_back(i);
    } 
    int query(int left, int right, int value) {
        auto& v = mp[value];
        return ranges::upper_bound(v, right) - ranges::lower_bound(v, left);
    }
    unordered_map<int, vector<int>> mp;
};