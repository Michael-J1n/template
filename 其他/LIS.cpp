#include <bits/stdc++.h>
using namespace std;

/**
 * 一维 LIS
 * 
 * 复杂度 O(nlogn)
 */
int LIS(vector<int>& a) {
    vector<int> g;
    for (auto& x : a) {
        auto it = ranges::lower_bound(g, x);
        if (it == g.end()) g.emplace_back(x);
        else *it = x;
    }
    return g.size();
}

/**
 * 二维 LIS
 * 先将第一维度升序排序；那么任意子序列第一维度都被确定
 * 考虑第二维度要升序，求 LIS 即可。为了避免同一个第一维度被选择，第二维度在 tie 的时候从大到小排列即可
 * 
 * 复杂度 O(nlogn)
 */
int LIS2D(vector<pair<int, int>>& a) {
    ranges::sort(a, [&](const auto& x, const auto& y) {
        if (x.first != y.first) return x.first < y.first;
        return x.second > y.second;
    });
    vector<int> g;
    for (auto& [_, x] : a) {
        auto it = ranges::lower_bound(g, x);
        if (it == g.end()) g.emplace_back(x);
        else *it = x;
    }
    return g.size();
}