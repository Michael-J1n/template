#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int mod;

const int MX = 1e6 + 5;
int cnt = 0, prim[MX]; bool vis[MX];

void init() {
    vis[1] = false;
    for (int i = 2; i < MX; ++i) {
        if (!vis[i]) prim[++cnt] = i;
        for (int j = 1; i * prim[j] < MX; ++j) {
            vis[i * prim[j]] = true;
            if (i % prim[j] == 0) break;
        }
    }
}

int qpow(int a, int b) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = 1LL * a * a % mod, b>>= 1;
    }
    return res;
}

// 求 v_p(n!) = sum{[n/p^i]}，复杂度 O(logn)
int vp(int n, int p) {
    int s = 0;
    while (n) n /= p, s += n;
    return s;
}

// 应用：nCm 中素数 p 的次数
int vpC(int n, int m, int p) {
	if (m < 0 || m > n) return 0;
	return vp(n, p) - vp(m, p) - vp(n - m, p);
}

// 求 C(n, m) % mod
int C(int n, int m) {
    int res = 1;
    for (int i = 1; i <= cnt; ++i) {
        int p = prim[i];
        if (p > n) return res;
        int e = vpC(n, m, p);
        res = (__int128)res * qpow(p, e) % mod;
    }
    return res;
}