#include <bits/stdc++.h>
using namespace std;
#define int long long
using pii = pair<int, int>;

/**
 * 皮萨诺周期：
 * 对于斐波那契数 Fn % p，对于固定的 p，存在 pi(p) 使得 F_{n} = F_{n % pi(p)} (mod p)
 * 因此对于大下标的 fib 数可以用周期归约到小情况
 */

/** miller rabin 以及 pollard-rho */
const int prims[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int rnd(int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    return dist(rng);
}
int qpow(int a, int b, int p) {
    int res = 1 % p;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p, b >>= 1;
    }
    return res;
}
bool check(int n) {
    if (n < 2) return false;
    int d = n - 1, s = 0;
    while ((d & 1) == 0) ++s, d >>= 1;
    for (auto &p : prims) {
        if (n == p) return true;
        if (p % n == 0) continue;
        int x = qpow(p, d, n);
        if (x == 1 || x == n - 1) continue;
        bool flag = false;
        for (int i = 1; i < s; ++i) {
            x = x * x % n;
            if (x == n - 1) { flag = true; break; }
            if (x == 1) return false;
        }
        if (!flag && x != 1) return false;
    }
    return true;
}
int f(int x, int c, int mod) {
    return ((x * x % mod - c) % mod + mod) % mod;
}
int pr(int n) {
    if ((n & 1) == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        int c = rnd(1, n - 1), x = rnd(2, n - 2), y = x, d = 1;
        while (d == 1) {
            x = f(x, c, n), y = f(f(y, c, n), c, n);
            d = gcd(llabs(x - y), n);
        }
        if (d != n) return d;
    }
}
void factorDfs(int n, vector<int> &fac) {
    if (n == 1) return;
    if (check(n)) { fac.push_back(n); return; }
    int d = pr(n);
    factorDfs(d, fac), factorDfs(n / d, fac);
}
vector<pii> factorize(int n) {
    vector<int> fac;
    factorDfs(n, fac);
    sort(fac.begin(), fac.end());
    vector<pii> res;
    for (int i = 0; i < (int)fac.size();) {
        int j = i;
        while (j < (int)fac.size() && fac[j] == fac[i]) ++j;
        res.push_back({fac[i], j - i});
        i = j;
    }
    return res;
}

// 求 fib 数
pii fib2(int n, int mod) {
    if (mod == 1) return {0, 0};
    if (n == 0) return {0, 1};
    auto [a, b] = fib2(n >> 1, mod);
    int t = (2 * b % mod - a + mod) % mod, c = a * t % mod, d = (a * a % mod + b * b % mod) % mod;
    if ((n & 1) == 0) return {c, d};
    return {d, (c + d) % mod};
}
int fib(int n, int mod) { return fib2(n, mod).first; }
int ipow(int a, int e) {
    int r = 1;
    while (e) {
        if (e & 1) r = r * a;
        a = a * a, e >>= 1;
    }
    return r;
}

/** pi(p) for prime p */
int pisanoPrime(int p) {
    if (p == 2) return 3;
    if (p == 3) return 8;
    if (p == 5) return 20;
    int leg = qpow(5 % p, (p - 1) / 2, p), upper = (leg == 1 ? p - 1 : 2 * (p + 1));
    auto fs = factorize(upper);
    vector<int> divs;
    auto gen = [&](auto &&gen, int i, int cur) -> void {
        if (i == (int)fs.size()) return divs.push_back(cur);
        int pr = fs[i].first, e = fs[i].second, x = 1;
        for (int k = 0; k <= e; ++k) gen(gen, i + 1, cur * x), x *= pr;
    };
    gen(gen, 0, 1);
    sort(divs.begin(), divs.end());
    for (int d : divs) {
        auto [fd, fd1] = fib2(d, p);
        if (fd == 0 && fd1 == 1) return d;
    }
    return upper;
}
int pisanoPrimePower(int p, int e) {
    if (p == 2) {
        if (e == 1) return 3;
        if (e == 2) return 6;
        return 3 * (1LL << (e - 1));
    }
    if (p == 5) return ipow(5, e - 1) * 20;
    int pi = pisanoPrime(p);
    return ipow(p, e - 1) * pi;
}
/** 求 m 的皮萨诺周期，期望复杂度 O(m^0.25) */
int pisano(int m) {
    if (m == 1) return 1;
    auto fs = factorize(m);
    int res = 1;
    for (auto [p, e] : fs) res = lcm(res, pisanoPrimePower(p, e));
    return res;
}