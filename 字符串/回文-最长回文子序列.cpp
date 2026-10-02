#include <bits/stdc++.h>
using namespace std;

/**
 * 求 s 的最长回文子序列
 * 本质是区间 DP
 * 
 * 复杂度 O(n^2)
 */
int longestPalindromeSubseq(string s) {
    int n = s.size();
    vector<vector<int>> f(n, vector<int>(n));
    for (int i = 0; i < n; ++i) f[i][i] = 1;
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len <= n; ++l) {
            int r = l + len - 1;
            if (s[l] == s[r]) f[l][r] = (len == 2 ? 2 : f[l + 1][r - 1] + 2);
            else f[l][r] = max(f[l + 1][r], f[l][r - 1]);
        }
    }
    return n ? f[0][n - 1] : 0;
}