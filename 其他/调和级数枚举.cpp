#include <bits/stdc++.h>
using namespace std;

/**
 * 给定数组，查询每个数在数组中有多少个倍数，有多少个因数
 * 
 * 假设值域范围为 V，那么可以进行调和级数枚举每个数的因数
 * 
 * 复杂度 O(V log V)
 */
void count(const vector<int>& a, vector<int>& fac, vector<int>& mul) {
    int n = a.size(), M = ranges::max(a);
    vector<int> cnt(M + 1);
    fac.assign(M + 1, 0), mul.assign(M + 1, 0);
    for (auto& x : a) ++cnt[x];
    for (int d = 1; d <= M; ++d) {
        for (int x = d; x <= M; x += d) {
            // d | x
            fac[x] += cnt[d];
            mul[d] += cnt[x];
        }
    }
    // #{x: x∈a, x|nums[i]} = fac[nums[i]]
    // #{x: x∈a, nums[i]|x} = mul[nums[i]]
    // 若不包含自己，减去 1
}