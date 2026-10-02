#include <bits/stdc++.h>
using namespace std;

/**
 * 中心扩展法：一个子串的中心要么是单个，要么是两个，分别以中心扩展，扩展途中更新答案
 * 模板以求每个长度的最小 |h[l+i]-h[r-i]| 为例
 */
vector<long long> calc(vector<int>& h) {
    int n = h.size();
    vector<long long> res(n + 1, 4e18);
    auto expand = [&](int l, int r) -> void {
        long long cur = 0;
        while (l >= 0 && r < n) {
            cur += abs(h[l] - h[r]);
            res[r - l + 1] = min(res[r - l + 1], cur);
            --l, ++r;
        }
    };
    for (int i = 0; i < n; ++i) expand(i, i), expand(i, i + 1);
    return res;
}

/**
 * 在需要枚举子串的时候，想中心扩展法，有时会大幅减少重复计算（复用之前的状态）
 */