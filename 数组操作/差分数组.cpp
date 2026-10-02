#include <bits/stdc++.h>
using namespace std;

/**
 * 对于一类问题，给定多个区间，区间有权值，求每个点的权值和
 * 这样的问题可以使用差分在区间两端打标记，最后做一次前缀和恢复信息，从而 O(n) 求出
 * 
 * 核心结构：重叠区间；连续子段的值相等
 */

const int N = 1e5 + 5;  // 区间端点的最大值
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    vector<int> d(N);
    for (auto& v : intervals) {
        int l = v[0], r = v[1], x = v[2];
        ++d[l], --d[r + 1];
    }
    for (int i = 0; i < N; ++i) {
        d[i] += d[i - 1];
    }
    return d;
}