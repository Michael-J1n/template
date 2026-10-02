/**
 * Miller Rabin 算法，考虑 d->d*2->...->d*2^s=n-1 modn 的链，只有里面出现了-1才通过这一轮检测
 * 复杂度约为 O(log^2n)
 * */

#include <bits/stdc++.h>
using namespace std;
#define int long long

const int prims[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

int qpow(int a, int b, int p) {
    int res = 1;
    while (b) {
        if (b & 1) res = (__int128_t)res * a % p;
        a = (__int128_t)a * a % p, b >>= 1;
    }
    return res;
}

bool check(int n) {
    if (n < 2) return false;
    int d = n - 1, s = 0;
    while (d % 2 == 0) ++s, d /= 2;
    for (auto& p : prims) {
        if (n == p) return true;
        if (p % n == 0) continue;
        int x = qpow(p, d, n);
        if (x == 1 || x == n - 1) continue;
        bool flag = false;
        for (int i = 1; i < s; ++i) {
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

signed main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        cout << (check(n) ? "YES" : "NO") << endl;
    }
    return 0;
}