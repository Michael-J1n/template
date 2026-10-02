#include <bits/stdc++.h>
using namespace std;

/**
 * 区间 DP 求有多少个合法的子序列
 * 假设 ok 代价为 C，复杂度 O(C * N^2) 
 * 
 * 这里以计算回文子串个数为例，f[l][r] 代表 s[l..r] 是否为回文子串
 */

int intervalDP(string s) {
    int n = s.size(), res = 0;
    vector<vector<int>> f(n, vector<int>(n));
    auto ok = [&](int l, int r) -> bool {
        return f[l][r];
    };
    for (int len = 1; len <= n; ++len) {
        for (int l = 0; l + len <= n; ++l) {
            int r = l + len - 1;
            f[l][r] = (s[l] == s[r]) && (len <= 2 || f[l + 1][r - 1]);
            if (ok(l, r)) ++res;
        }
    }
    return res;
}