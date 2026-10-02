#include <bits/stdc++.h>
using namespace std;

/**
 * 返回字符串 s 的 LCP 数组
 * 定义 lcp[i][j] = LCP(s[i:], s[j:])
 * 
 * 复杂度：O(n^2)
 */
vector<vector<int>> LCP(const string& s) {
    int n = s.size();
    vector lcp(n + 1, vector<int>(n + 1));
    for (int i = n - 1; ~i; --i) {
        for (int j = n - 1; ~j; --j) {
            if (s[i] == s[j]) lcp[i][j] = lcp[i + 1][j + 1] + 1;
            else lcp[i][j] = 0;
        }
    }
    return lcp;
}