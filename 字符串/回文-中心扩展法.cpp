#include <bits/stdc++.h>
using namespace std;

/**
 * 中心扩展法求最长回文子串
 * 
 * 考虑高效枚举所有回文对判断；如果先枚举左右端点再内部扫描，是 O(n^3) 的。借助中心扩展法，可以在 O(n^2) 扫描到所有信息
 * 
 * 复杂度 O(n^2)
 */
string longestPalindrome(string s) {
    string res;
    int len = -1, n = s.size();
    for (int i = 0; i < 2 * n - 1; ++i) {
        int l = i / 2, r = (i + 1) / 2;
        // 扩展完成后，s[l+1]~s[r-1] 为回文；回文中心为 l|r（当l==r时为奇回文，否则为偶回文）
        while (l >= 0 && r < n && s[l] == s[r]) --l, ++r;
        if (r - l - 1 > len) len = r - l - 1, res = s.substr(l + 1, r - l - 1);
    }
    // 此时 len 为最大长度
    return res;
}