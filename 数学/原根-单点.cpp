#include <bits/stdc++.h>
using namespace std;

/**
 * 单点求 mod n 的最小原根 g（若不存在返回 -1）
 */

long long qpow(long long a, long long b, long long mod) {
    a %= mod;
    long long res = 1 % mod;
    while (b) {
        if (b & 1) res = ((__int128)res * a) % mod;
        a = ((__int128)a * a) % mod, b >>= 1;
    }
    return res;
}

bool check_exist(long long n) {
    if (n == 2 || n == 4) return true;
    if (n % 2 == 0) n /= 2;
    if (n % 2 == 0) return false;
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            return (n == 1);
        }
    }
    return true;
}

long long phi(long long n) {
    long long res = n;
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            res -= res / i;
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) res -= res / n;
    return res;
}

vector<long long> pfacs;
void get(long long x) {
    pfacs.clear();
    for (long long i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            pfacs.emplace_back(i);
            while (x % i == 0) x /= i;
        }
    }
    if (x > 1) pfacs.emplace_back(x);
}

bool check_isroot(long long g, long long n, long long pn) {
    if (gcd(g, n) != 1) return false;
    for (auto &p : pfacs) {
        if (qpow(g, pn / p, n) == 1) return false;
    }
    return true;
}

long long get_root(long long n) {
    if (!check_exist(n)) return -1;
    long long pn = phi(n);
    get(pn);
    for (long long g = 1; g <= n; ++g) {
        if (check_isroot(g, n, pn)) return g;
    }
    return -1;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    cin >> n;
    cout << get_root(n) << '\n';
    return 0;
}
