#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'

/**
 * 适用于 mod 数为一些小素数乘积，且为 square-free 的形式
 * 使用 Lucas 定理处理每一种 mod 数的情况，再使用 CRT 进行合并
 */
const int mod = 10000003233;
vector<int> facts = {3, 11, 101, 3000301};
// CRT
int exgcd(int a, int b, int& x, int& y) {
    if (b == 0) { x = 1, y = 0; return a; }
    int x1, y1;
    int g = exgcd(b, a % b, x1, y1);
    x = y1, y = x1 - (a / b) * y1;
    return g;
}
int inv(int a, int m) {
    int x, y;
    int g = exgcd(a, m, x, y);
    if (g != 1) return INT_MAX;
    x = (x % m + m) % m;
    return x;
}
int solve(vector<int>& r, vector<int>& m) {
    assert(r.size() == m.size());
    int n = r.size(), M = 1;
    for (int i = 0; i < n; ++i) M = M * m[i];
    int res = 0;
    for (int i = 0; i < n; ++i) {
        int ci = M / m[i], ivc = inv(ci % m[i], m[i]);
        (res += (__int128_t)r[i] * ci % M * ivc) %= M;
    }
    return res;
}
// 对于单个素数建立卢卡斯
struct LucasPrime {
    int p;
    vector<int> fac, ifac;
    LucasPrime(int _p = 2) { init(_p); }
    int qpow(int a, int b) {
        long long res = 1;
        while (b) {
            if (b & 1) res = res * a % p;
            a = 1LL * a * a % p, b >>= 1;
        }
        return (int)res;
    }
    void init(int _p) {
        p = _p;
        fac.assign(p, 1);
        ifac.assign(p, 1);
        for (int i = 1; i < p; ++i) fac[i] = 1LL * fac[i - 1] * i % p;
        ifac[p - 1] = qpow(fac[p - 1], p - 2);
        for (int i = p - 2; ~i; --i) ifac[i] = 1LL * ifac[i + 1] * (i + 1) % p;
    }
    int Csmall(int n, int k) {
        if (k < 0 || k > n) return 0;
        return 1LL * fac[n] * ifac[k] % p * ifac[n - k] % p;
    }
    int Lucas(long long n, long long k) {
        if (k < 0 || k > n) return 0;
        if (k == 0) return 1;
        return 1LL * Csmall((int)(n % p), (int)(k % p)) % p * Lucas(n / p, k / p) % p;
    }
};
vector<LucasPrime> luc;  // 对于每个素数，缓存结果
int C(long long n, long long k) {
    if (k < 0 || k > n) return 0;
    vector<int> r, m;
    r.reserve(facts.size());
    m.reserve(facts.size());
    for (int i = 0; i < (int)facts.size(); ++i) {
        r.emplace_back(luc[i].Lucas(n, k));
        m.emplace_back(facts[i]);
    }
    return solve(r, m);
}

// 示例用法：
signed main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    // 对于每个素数，缓存进 luc
    luc.reserve(facts.size());
    for (int p : facts) luc.emplace_back(LucasPrime(p));
    
    int T; cin >> T;
    while (T--) {
        int n, k, c; cin >> n >> k >> c;
        if (c < 1 || c > n) { cout << 0 << endl; continue; }
        cout << 2LL * C(n - 1, c - 1) % mod << endl;
    }
    return 0;
}
