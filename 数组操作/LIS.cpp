#include <bits/stdc++.h>
using namespace std;

/**
 * 一维严格 LIS
 * 求最长满足 x1 < x2 < ... < xk 的子序列
 * 
 * g[j]=长度为j+1的严格上升子序列的最小结尾
 * 使用 lower_bound，使相等元素不能增加长度
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
 * 一维非严格 LIS
 * 求最长满足 x1 <= x2 <= ... <= xk 的子序列
 * 
 * g[j]=长度为j+1的非严格上升子序列的最小结尾
 * 使用 upper_bound，使相等元素可以增加长度
 * 
 * 复杂度 O(nlogn)
 */
int LNDS(vector<int>& a) {
    vector<int> g;
    for (auto& x : a) {
        auto it = ranges::upper_bound(g, x);
        if (it == g.end()) g.emplace_back(x);
        else *it = x;
    }
    return g.size();
}

/**
 * 二维严格 LIS
 * 求最长满足 x1 < x2 < ... < xk 且 y1 < y2 < ... < yk 的子序列
 * 
 * 先将第一维升序排序
 * 第一维相同时，将第二维降序排序，避免选择两个第一维相同的元素
 * 然后对第二维求严格 LIS
 * 
 * 复杂度 O(nlogn)
 */
int LIS2D_LT_LT(vector<pair<int, int>>& a) {
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

/**
 * 二维 LIS
 * 求最长满足 x1 <= x2 <= ... <= xk 且 y1 < y2 < ... < yk 的子序列
 * 
 * 先将第一维升序排序
 * 第一维相同时，将第二维升序排序，使同一第一维中的第二维可以继续递增
 * 然后对第二维求严格 LIS
 * 
 * 复杂度 O(nlogn)
 */
int LIS2D_LE_LT(vector<pair<int, int>>& a) {
    ranges::sort(a, [&](const auto& x, const auto& y) {
        if (x.first != y.first) return x.first < y.first;
        return x.second < y.second;
    });
    vector<int> g;
    for (auto& [_, x] : a) {
        auto it = ranges::lower_bound(g, x);
        if (it == g.end()) g.emplace_back(x);
        else *it = x;
    }
    return g.size();
}

/**
 * 二维 LIS
 * 求最长满足 x1 < x2 < ... < xk 且 y1 <= y2 <= ... <= yk 的子序列
 * 
 * 交换两个维度考虑，先将第二维升序排序
 * 第二维相同时，将第一维升序排序，使同一第二维中的第一维可以继续递增
 * 然后对第一维求严格 LIS
 * 
 * 复杂度 O(nlogn)
 */
int LIS2D_LT_LE(vector<pair<int, int>>& a) {
    ranges::sort(a, [&](const auto& x, const auto& y) {
        if (x.second != y.second) return x.second < y.second;
        return x.first < y.first;
    });
    vector<int> g;
    for (auto& [x, _] : a) {
        auto it = ranges::lower_bound(g, x);
        if (it == g.end()) g.emplace_back(x);
        else *it = x;
    }
    return g.size();
}

/**
 * 二维非严格 LIS
 * 求最长满足 x1 <= x2 <= ... <= xk 且 y1 <= y2 <= ... <= yk 的子序列
 * 
 * 先将第一维升序排序
 * 第一维相同时，将第二维升序排序
 * 然后对第二维求非严格 LIS
 * 
 * 复杂度 O(nlogn)
 */
int LIS2D_LE_LE(vector<pair<int, int>>& a) {
    ranges::sort(a, [&](const auto& x, const auto& y) {
        if (x.first != y.first) return x.first < y.first;
        return x.second < y.second;
    });
    vector<int> g;
    for (auto& [_, x] : a) {
        auto it = ranges::upper_bound(g, x);
        if (it == g.end()) g.emplace_back(x);
        else *it = x;
    }
    return g.size();
}