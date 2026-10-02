#include <bits/stdc++.h>
using namespace std;

/**
 * 给定一个数组，判断是否能够重新排列使得元素间两两不相邻
 * 
 * 贪心，把出现频率最多的拿出来，间隔排序，并对于别的值也不断重复
 */
vector<int> rearrange(vector<int>& a) {
    int n = a.size();
    unordered_map<int, int> cnt;
    for (auto& x : a) ++cnt[x];
    int mv = 0, mf = 0;
    for (auto& [v, f] : cnt) {
        if (f > mf) mf = f, mv = v;
    }
    if (mf > (n + 1) / 2) return {}; // 不可能
    vector<int> res(n);
    int cur = 0;
    while (mf--) res[cur] = mv, cur += 2;
    cnt[mv] = 0;
    if (cur >= n) cur = 1;
    for (auto& [v, f] : cnt) {
        while (f--) {
            res[cur] = v, cur += 2;
            if (cur >= n) cur = 1;
        }
    }
    return res;
}