#include <bits/stdc++.h>
using namespace std;

/**
 * 模素数 p 意义下的整数与有理数
 */
template<int mod>
struct mint{
    int v;
    mint(long long x = 0) { x %= mod; if (x < 0) x += mod; v = x; }
    mint& operator+=(const mint& o) { v += o.v; if (v >= mod) v -= mod; return *this; }
    mint& operator-=(const mint& o) { v -= o.v; if (v < 0) v += mod; return *this; }
    mint& operator*=(const mint& o) { v = 1LL * v * o.v % mod; return *this; }
    mint qpow(long long k) const { mint a = *this, res = 1; while (k) { if (k & 1) res *= a; a *= a; k >>= 1; } return res; }
    mint inv() const { return qpow(mod - 2); }
    mint& operator/=(const mint& o) { return *this *= o.inv(); }
    friend mint operator+(mint a, const mint& b) { return a += b; }
    friend mint operator-(mint a, const mint& b) { return a -= b; }
    friend mint operator*(mint a, const mint& b) { return a *= b; }
    friend mint operator/(mint a, const mint& b) { return a /= b; }
    mint operator-() const { return mint(-v); }
    bool operator==(const mint& o) const { return v == o.v; }
    bool operator!=(const mint& o) const { return v != o.v; }
    friend ostream& operator<<(ostream& os, const mint& x) { return os << x.v; }
    friend istream& operator>>(istream& is, mint& x) { long long v; is >> v; x = mint(v); return is; }
};
template<int mod>
struct mfrac{
    mint<mod> p, q;
    mfrac(long long p = 0, long long q = 1) : p(p), q(q) {}
    mfrac(mint<mod> p, mint<mod> q = 1) : p(p), q(q) {}
    mint<mod> val() const { return p / q; }
    mfrac& operator+=(const mfrac& o) { p = p * o.q + o.p * q; q *= o.q; return *this; }
    mfrac& operator-=(const mfrac& o) { p = p * o.q - o.p * q; q *= o.q; return *this; }
    mfrac& operator*=(const mfrac& o) { p *= o.p; q *= o.q; return *this; }
    mfrac& operator/=(const mfrac& o) { p *= o.q; q *= o.p; return *this; }
    friend mfrac operator+(mfrac a, const mfrac& b) { return a += b; }
    friend mfrac operator-(mfrac a, const mfrac& b) { return a -= b; }
    friend mfrac operator*(mfrac a, const mfrac& b) { return a *= b; }
    friend mfrac operator/(mfrac a, const mfrac& b) { return a /= b; }
    mfrac operator-() const { return mfrac(-p, q); }
    bool operator==(const mfrac& o) const { return p * o.q == o.p * q; }
    bool operator!=(const mfrac& o) const { return !(*this == o); }
    friend ostream& operator<<(ostream& os, const mfrac& x) { return os << x.val(); }
};

// 示例用法：
const int mod = 998244353;
using Z = mint<mod>;
using F = mfrac<mod>;