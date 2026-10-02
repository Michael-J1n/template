#include <bits/stdc++.h>
using namespace std;

/**
 * Pollar-Rho 算法，在期望 O(n^0.25) 时间内找到 n 的一个素因子
 * */
using pll = pair<long long, long long>;
const long long prims[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
long long rnd(long long l, long long r) {   // [l, r]
    uniform_int_distribution<long long> dist(l, r);
    return dist(rng);
}
long long qpow(long long a, long long b, long long p) {
    long long res = 1;
    while (b) {
        if (b & 1) res = (__int128_t)res * a % p;
        a = (__int128_t)a * a % p, b >>= 1;
    }
    return res;
}
/** miller-rabin 素性检验，从a^d开始不断平方找-1 */
bool check(long long n) {
    if (n < 2) return false;
    long long d = n - 1, s = 0;
    while (d % 2 == 0) ++s, d /= 2;
    for (auto& p : prims) {
        if (n == p) return true;
        if (p % n == 0) continue;
        long long x = qpow(p, d, n);
        if (x == 1 || x == n - 1) continue;
        bool flag = false;
        for (long long i = 1; i < s; ++i) {
             x = (__int128_t)x * x % n;
            if (x == n - 1) {
                flag = true;
                break;
            }
            if (x == 1) return false;
        }
        if (!flag && x != 1) return false;
    }
    return true;
}
long long f(long long x, long long c, long long mod) {
    return (((__int128_t)x * x % mod - c) % mod + mod) % mod;
}
long long pr(long long n) {
    if ((n & 1) == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        long long c = rnd(1, n - 1), x = rnd(2, n - 2), y = x, d = 1;
        while (d == 1) {
            x = f(x, c, n), y = f(f(y, c, n), c, n);
            d = gcd(llabs(x - y), n);
        }
        if (d != n) return d;
    }
}
void factorDfs(long long n, vector<long long>& fac) {
    if (n == 1) return;
    if (check(n)) { fac.push_back(n); return; }
    long long d = pr(n);
    factorDfs(d, fac);
    factorDfs(n / d, fac);
}
/** O(n^0.25) 分解质因数；返回 {质因子，指数} 列表 */
vector<pll> factorize(long long n) {
    vector<long long> fac;
    factorDfs(n, fac);
    sort(fac.begin(), fac.end());
    vector<pll> res;
    for (long long i = 0; i < fac.size();) {
        long long j = i;
        while (j < fac.size() && fac[j] == fac[i]) ++j;
        res.push_back({fac[i], j - i});
        i = j;
    }
    return res;
}
/** 生成因子列表 */
vector<long long> divisors(const vector<pll>& fs) {
    vector<long long> res = {1};
    for (auto [p, e] : fs) {
        int m = res.size();
        long long pw = 1;
        for (int i = 1; i <= e; ++i) {
            pw *= p;
            for (int j = 0; j < m; ++j) res.push_back(res[j] * pw);
        }
    }
    sort(res.begin(), res.end());
    return res;
}