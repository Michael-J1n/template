#include <bits/stdc++.h>
using namespace std;

/**
 * Water Filling【相同离散凸函数求和最小化】
 *
 * 问题：每次选择一个正元素减 1，恰好执行 k 次，最小化 sum(f(a[i]))，其中 f 在非负整数上离散凸（即 f(x+2)-f(x+1)>=f(x+1)-f(x)）。
 *
 * 思路：离散凸性保证 delta(x) = f(x-1)-f(x) 随 x 单调不增，因此优先减少最大的元素。
 * 二分最小可行水位 h，使得将所有元素降低到 <= h 所需的操作次数 <= k；剩余 rem 次操作分别作用于高度为 h 的元素，每次代价为 f(h-1)-f(h)。
 *
 * 复杂度：O(n log V)
 */
long long f(long long x) {
    return x * x; // 根据题目修改，要求 f 为离散凸函数
}
long long calc(const vector<int>& a, long long k) {
    auto check = [&](int h)->bool {
        long long cnt = 0;
        for (auto& x : a) if (x > h) cnt += x - h;
        return cnt <= k;
    };
    int l = 0, r = ranges::max(a);
    while (l < r) {
        int mid = l + r >> 1;
        if (check(mid)) r = mid;
        else l = mid + 1;
    }
    long long rem = k, res = 0;
    for (auto& x : a) {
        long long y = min(x, l);
        res += f(y);
        rem -= x - y;
    }
    if (rem) res += rem * (f(l - 1) - f(l));
    return res;
}