#include <bits/stdc++.h>
using namespace std;

/**
 * Booth 最小循环表示【循环同构 / canonical form】
 * 返回 s 的所有循环位移中字典序最小的字符串
 * 
 * 若两个字符串长度相同且最小循环表示相同，则它们互为循环位移
 * 
 * 复杂度：O(n)
 */
string minRotation(const string& s) {
    int n = s.size();
    string t = s + s;
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) {
            ++k;
            continue;
        }
        if (t[i + k] > t[j + k]) i += k + 1;
        else j += k + 1;
        if (i == j) ++j;
        k = 0;
    }
    int p = min(i, j);
    return t.substr(p, n);
}

/* 例：给定整数数组，求有多少对数字是互为循环移位；复杂度 O(nlogV) */
long long solution(vector<int> nums) {
    unordered_map<string, long long> cnt;
    long long ans = 0;
    for (int x : nums) {
        string s = minRotation(to_string(x));
        ans += cnt[s];
        ++cnt[s];
    }
    return ans;
}