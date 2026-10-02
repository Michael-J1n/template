#include <bits/stdc++.h>
using namespace std;

/**
 * 恰好型滑动窗口；当前模板以“恰好 k 个奇数”的子数组个数为例
 * exactly(k) 有两种写法：
 * 
 * 1. exactly(k) = atMost(k) - atMost(k - 1)
 * atMost(k) 对应越短越合法式滑窗，大部分选用这个即可；即：恰好 k 个合法元素的子数组个数等于至多 k 个合法元素的子数组个数减去至多 k-1 个合法元素的子数组个数
 * ok 函数填写的是：是否满足最多只有 k 个
 * 
 * 2. exactly(k) = atLeast(k) - atLeast(k + 1)
 * atLeast(k) 对应越长越合法式滑窗，特殊情况下选用
 * 
 * 复杂度：O(n)
 */
long long atMost(vector<int>& a, long long k) {
    if (k < 0) return 0;
    int n = a.size(), l = 0;
    long long res = 0, cnt = 0;
    auto add = [&](int x) {  // 进入窗口
        cnt += x & 1;
    };
    auto del = [&](int x) {  // 离开窗口
        cnt -= x & 1;
    };
    auto ok = [&]() {        // 当前窗口 [l,r] 的计数是否不超过 k
        return cnt <= k;
    };
    for (int r = 0; r < n; ++r) {
        add(a[r]);
        while (l <= r && !ok()) del(a[l++]);
        res += r - l + 1;
    }
    return res;
}
long long exactlyCount(vector<int>& a, long long k) {
    return atMost(a, k) - atMost(a, k - 1);
    // return atLeast(a, k) - atLeast(a, k + 1);
}