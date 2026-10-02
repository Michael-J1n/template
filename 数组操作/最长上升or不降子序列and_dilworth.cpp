#include <bits/stdc++.h>
using namespace std;

/**
 * 求数组的最长上升子序列长度 
 * 
 * 复杂度：O(nlogn)
 */
int LIS(vector<int>& nums) {
    vector<int> g;
    for (auto& x : nums) {
        auto it = ranges::lower_bound(g, x);
        if (it == g.end()) g.push_back(x);
        else *it = x;
    }
    return g.size();
}

/**
 * 求数组的最长不降子序列长度 
 * 
 * 复杂度：O(nlogn)
 */
int LNDS(vector<int>& nums) {
    vector<int> g;
    for (auto& x : nums) {
        auto it = ranges::upper_bound(g, x);
        if (it == g.end()) g.push_back(x);
        else *it = x;
    }
    return g.size();
}

/**
 * Dilworth 定理
 *
 * 结论：
 * 有限偏序集的**最少链覆盖数 = 最大反链大小**
 *
 * 序列常用形式：
 * 1. 最少不降子序列覆盖数 = 最长严格下降子序列长度
 *    链：a[i] <= a[j]
 *    反链：a[i] > a[j]
 *
 * 2. 最少严格上升子序列覆盖数 = 最长非上升子序列长度
 *    链：a[i] < a[j]
 *    反链：a[i] >= a[j]
 *
 * 匹配求最少链覆盖：
 * 对每个点拆成左点和右点，若 i 可以接到 j 前面，则连 i -> j。
 * 设最大匹配为 match，则最少链覆盖数为 n - match。
 *
 * 注意：
 * 若给的是 DAG，链允许通过路径连接，需要先求可达性再建边。
 */