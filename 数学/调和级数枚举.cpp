#include <bits/stdc++.h>
using namespace std;

/**
 * 调和级数枚举
 * 给定数组，查询每个数在数组中有多少个倍数，有多少个因数
 * 
 * 调和级数枚举每个数的因数；返回（注意都包含了自己）：
 * fac[nums[i]] = #{x: x∈a, x|nums[i]}
 * mul[nums[i]] = #{x: x∈a, nums[i]|x}
 * 
 * 复杂度 O(V log V)，其中 V 为值域
 */
void count(const vector<int>& a, vector<int>& fac, vector<int>& mul) {
    int n = a.size(), M = ranges::max(a);
    vector<int> cnt(M + 1);
    fac.assign(M + 1, 0), mul.assign(M + 1, 0);
    for (auto& x : a) ++cnt[x];
    for (int d = 1; d <= M; ++d) {
        for (int x = d; x <= M; x += d) {
            // 枚举 d 的每个倍数 x 
            fac[x] += cnt[d];
            mul[d] += cnt[x];
        }
    }
}