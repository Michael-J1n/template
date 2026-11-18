#include <bits/stdc++.h>
using namespace std;

/**
 * 倍数容斥
 * 给定数组 a，求 1..N 中至少被一个 a[i] 整除的数个数
 * 
 * 容斥：
 * 如果只有一个数，答案显然为 N/a[i]，现在考虑所有数，如果被 a[i],a[j] 整除，那么也被 lcm(a[i],a[j]) 整除，因此贡献扣掉 N/lcm(a[i],a[j])，如此下去
 * 有 cnt = N/a[0]+...+N/a[n-1] - N/lcm(a[0],a[1])-...-N/lcm(a[n-2],a[n-1]) + N/lcm(a[0],a[1],a[2])+... - ...
 * 
 * 复杂度 O(2^n * log V)，瓶颈在于子集枚举，log 来自计算 LCM（但注意由于剪枝，实际复杂度比这个低很多）
 * 在有多次查询时（例如二分答案时多次调用），可以预处理出子集的 LCM 并计算，见第二个代码。
 */
long long multiplePIE(long long N, vector<int> a) {
    long long res = 0;
    ranges::sort(a);
    vector<int> tmp;
    for (auto& x : a) {
        bool ok = true;
        for (auto& y : tmp) {
            if (x % y == 0) {
                ok = false;
                break;
            }
        }
        if (ok) tmp.emplace_back(x);
    }
    swap(a, tmp); ranges::reverse(a);
    int n = a.size();
    auto dfs = [&](this auto&& dfs, int p, long long cur, int cnt) -> void {
        for (int i = p; i < n; ++i) {
            long long g = gcd(cur, 1LL * a[i]);
            if (cur / g > N / a[i]) continue;
            long long nc = cur / g * a[i];
            if (cnt & 1) res += N / nc;
            else res -= N / nc;
            dfs(i + 1, nc, cnt + 1);
        }
    };
    dfs(0, 1, 1);
    return res;
}

/**
 * 多次查询版本，预处理所有子集的 LCM
 * 预处理复杂度 O(n^2 + 2^n * log V)，单次查询复杂度 O(2^n)
 */
struct MultiplePIE {
    int n;
    vector<long long> lcm;
    MultiplePIE(vector<int> a) {
        ranges::sort(a);
        vector<int> tmp;
        for (auto& x : a) {
            bool ok = true;
            for (auto& y : tmp) {
                if (x % y == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) tmp.emplace_back(x);
        }
        swap(a, tmp);
        n = a.size();
        lcm.resize(1 << n);
        lcm[0] = 1;
        for (int mask = 1; mask < (1 << n); ++mask) {
            int i = __lg(mask & -mask);
            int pre = mask ^ (1 << i);
            long long g = gcd(lcm[pre], 1LL * a[i]);
            lcm[mask] = lcm[pre] / g * a[i];
        }
    }
    long long query(long long N) {
        long long res = 0;
        for (int mask = 1; mask < (1 << n); ++mask) {
            if (__builtin_popcount(mask) & 1) res += N / lcm[mask];
            else res -= N / lcm[mask];
        }
        return res;
    }
};