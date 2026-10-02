#include <bits/stdc++.h>
using namespace std;

/**
 * bitCnt：统计数组中每一位为 1 的元素个数，默认处理非负 int
 * cnt[b] 表示第 b 位为 1 的数有多少个
 * 复杂度 O(31n)
 */
vector<int> bitCnt(const vector<int>& a) {
    vector<int> cnt(31);
    for (auto& x : a) {
        for (int b = 0; b <= 30; ++b) {
            if (x >> b & 1) ++cnt[b];
        }
    }
    return cnt;
}