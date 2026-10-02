#include <bits/stdc++.h>
using namespace std;

/**
 * GCD 容斥：给定数组 a，求满足 i!=j, gcd(a[i],a[j])=G 的无序对数量
 * 
 * 容斥：
 * 记 mul[g] = 数组中被 g 整除的数个数，cnt_gcd[g] = gcd 为 g 的无序数对个数
 * 有：cnt_gcd[g] = C(mul[g],2) - sum cnt_gcd[k*g]，倒序计算即可
 * 
 * 复杂度 O(V log V)，瓶颈在于调和级数枚举
 */
vector<long long> gcdPIE(vector<int>& a) {
    int mx = ranges::max(a);
    vector<long long> cnt_gcd(mx + 1);
    vector<int> cnt(mx + 1);
    for (auto& x : a) ++cnt[x];
    for (int g = mx; g >= 1; --g) {
        int c = cnt[g]; long long s = 0;
        for (int k = 2; k * g <= mx; ++k) {
            c += cnt[k * g];
            s += cnt_gcd[k * g];
        }
        cnt_gcd[g] = 1LL * c * (c - 1) / 2 - s; 
    }
    return cnt_gcd;
}