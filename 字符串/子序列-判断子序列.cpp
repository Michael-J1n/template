#include <bits/stdc++.h>
using namespace std;

// 判断 s 是否为 t 的子序列
bool checkSubstring(const string& t, const string& s) {
    int j = 0, n = s.size(), m = t.size();
    for (int i = 0; i < n && j < m; ++i) {
        if (s[i] == t[j]) ++j;
    }
    return j == m;
}