#include <bits/stdc++.h>
using namespace std;

/**
 * 多项式哈希(双模随机基数，支持区间仿射变换)
 *
 * 支持操作：
 * get(l, r)：获取区间 [l,r] 打包后的 u64 Hash
 * affine(l, r, mul, add)：获取 mul * a[i] + add 后的区间 Hash
 * equal_affine(other, l1, r1, l2, r2, mul, add)：判断变换后的区间是否相等
 *
 * 适用：静态字符串、整数数组、区间线性变换比较
 * 复杂度：构造 O(n)，单次查询 O(1)，空间 O(n)
 */
using u64 = uint64_t;
struct PolyHash {
    static constexpr int mod1 = 1000000007, mod2 = 1000000009;
    inline static vector<int> p1 = {1}, p2 = {1}, s1 = {0}, s2 = {0};
    static int base() {
        static int b = []() {
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            return uniform_int_distribution<int>(256, mod1 - 1)(rng);
        }();
        return b;
    }
    static void ensure(int n) {
        int b = base();
        while (p1.size() <= n) {
            p1.emplace_back(1LL * p1.back() * b % mod1), p2.emplace_back(1LL * p2.back() * b % mod2);
            s1.emplace_back((1LL * s1.back() * b + 1) % mod1), s2.emplace_back((1LL * s2.back() * b + 1) % mod2);
        }
    }
    vector<int> h1, h2;
    void build(int n, auto value) {
        ensure(n);
        h1.resize(n + 1); h2.resize(n + 1);
        int b = base();
        for (int i = 0; i < n; ++i) {
            long long x = value(i);
            long long x1 = (x % mod1 + mod1 + 1) % mod1, x2 = (x % mod2 + mod2 + 1) % mod2;
            h1[i + 1] = (1LL * h1[i] * b + x1) % mod1;
            h2[i + 1] = (1LL * h2[i] * b + x2) % mod2;
        }
    }
    PolyHash(const vector<int>& a) { build(a.size(), [&](int i) { return (long long)a[i]; }); }
    PolyHash(const string& s) { build(s.size(), [&](int i) { return (int)(unsigned char)s[i]; }); }
    pair<int, int> getp(int l, int r) const {
        int len = r - l + 1;
        int x1 = (h1[r + 1] - 1LL * h1[l] * p1[len] % mod1 + mod1) % mod1, x2 = (h2[r + 1] - 1LL * h2[l] * p2[len] % mod2 + mod2) % mod2;
        return {x1, x2};
    }
    u64 get(int l, int r) const {
        auto [x1, x2] = getp(l, r);
        return (u64)(uint32_t)x1 << 32 | (uint32_t)x2;
    }
    pair<int, int> affine(int l, int r, long long mul, long long add) const {
        auto [x1, x2] = getp(l, r);
        int len = r - l + 1;
        long long a1 = (mul % mod1 + mod1) % mod1, a2 = (mul % mod2 + mod2) % mod2;
        long long b1 = (add % mod1 + 2LL * mod1 + 1 - a1) % mod1, b2 = (add % mod2 + 2LL * mod2 + 1 - a2) % mod2;
        return {(int)((a1 * x1 + b1 * s1[len]) % mod1), (int)((a2 * x2 + b2 * s2[len]) % mod2)};
    }
    bool equal_affine(const PolyHash& other, int l1, int r1, int l2, int r2, long long mul, long long add) const { return r1 - l1 == r2 - l2 && affine(l1, r1, mul, add) == other.getp(l2, r2); }
};