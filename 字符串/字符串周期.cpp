/**
 * 作用：求字符串最大重复次数 power(s)=max n 使得 s=a^n
 * 复杂度：O(n)
 */

#include <bits/stdc++.h>
using namespace std;

vector<int> buildPi(const string& s) {
    int n = s.size();
    vector<int> pi(n);
    for (int i = 1, j = 0; i < n; ++i) {
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

int powerString(const string& s) {
    int n = s.size();
    if (n == 0) return 0;
    vector<int> pi = buildPi(s);
    int per = n - pi[n - 1];
    return (n % per == 0 ? n / per : 1);
}