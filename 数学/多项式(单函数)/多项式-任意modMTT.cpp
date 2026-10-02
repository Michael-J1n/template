#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const long double PI = acos(-1);

struct Complex {
    long double x, y;
    Complex operator+(const Complex& t) const { return {x + t.x, y + t.y}; }
    Complex operator-(const Complex& t) const { return {x - t.x, y - t.y}; }
    Complex operator*(const Complex& t) const { return {x * t.x - y * t.y, x * t.y + y * t.x}; }
};

void FFT(vector<Complex>& A, int n, int op) {
    for (int i = 1, j = 0; i < n; ++i) {
        int b = n >> 1;
        for (; j & b; b >>= 1) j ^= b;
        j ^= b;
        if (i < j) swap(A[i], A[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        Complex w1({cos(2 * PI / len), sin(2 * PI / len) * op});
        for (int i = 0; i < n; i += len) {
            Complex wk({1, 0});
            for (int k = i; k < i + (len >> 1); ++k) {
                Complex x = A[k], y = A[k + (len >> 1)] * wk;
                A[k] = x + y, A[k + (len >> 1)] = x - y;
                wk = wk * w1;
            }
        }
    }
}

// 任意模数卷积（4-FFT MTT） A * B，系数 % mod，多项式在 mod x^need 里做
vector<ll> conv(vector<ll> A, vector<ll> B, ll mod, int need = -1) {
    int n = A.size(), m = B.size();
    if (need != -1) {
        if (n > need) A.resize(need), n = need;
        if (m > need) B.resize(need), m = need;
    }
    int N = 1, M = n + m - 1;
    while (N < M) N <<= 1;

    const ll BASE = 1LL << 15;
    vector<Complex> FA(N), FB(N);

    for (int i = 0; i < n; ++i) {
        ll v = A[i] % mod; if (v < 0) v += mod;
        ll a0 = v % BASE, a1 = v / BASE;
        FA[i] = {(long double)a0, (long double)a1};
    }
    for (int i = 0; i < m; ++i) {
        ll v = B[i] % mod; if (v < 0) v += mod;
        ll b0 = v % BASE, b1 = v / BASE;
        FB[i] = {(long double)b0, (long double)b1};
    }

    FFT(FA, N, 1), FFT(FB, N, 1);

    vector<Complex> A0(N), A1(N), B0(N), B1(N);
    for (int i = 0; i < N; ++i) {
        int j = (N - i) & (N - 1);
        Complex xi = FA[i], xj = FA[j], yi = FB[i], yj = FB[j], xj_conj = {xj.x, -xj.y}, yj_conj = {yj.x, -yj.y};
        Complex sA = xi + xj_conj, dA = xi - xj_conj, sB = yi + yj_conj, dB = yi - yj_conj;
        A0[i] = {sA.x * 0.5, sA.y * 0.5}, A1[i] = {dA.y * 0.5, -dA.x * 0.5},
        B0[i] = {sB.x * 0.5, sB.y * 0.5}, B1[i] = {dB.y * 0.5, -dB.x * 0.5};
    }

    vector<Complex> P(N), Q(N);
    for (int i = 0; i < N; ++i) {
        Complex f00 = A0[i] * B0[i], f10 = A1[i] * B0[i], f01 = A0[i] * B1[i], f11 = A1[i] * B1[i];
        P[i] = {f00.x - f10.y, f00.y + f10.x};
        Q[i] = {f01.x - f11.y, f01.y + f11.x};
    }

    FFT(P, N, -1), FFT(Q, N, -1);
    for (int i = 0; i < N; ++i) P[i].x /= N, P[i].y /= N, Q[i].x /= N, Q[i].y /= N;

    vector<ll> C(M);
    ll BASEmod = BASE % mod, BASE2 = BASEmod * BASEmod % mod;
    for (int i = 0; i < M; ++i) {
        ll c00 = (ll)llround(P[i].x) % mod, c10 = (ll)llround(P[i].y) % mod, c01 = (ll)llround(Q[i].x) % mod, c11 = (ll)llround(Q[i].y) % mod;
        if (c00 < 0) c00 += mod;
        if (c10 < 0) c10 += mod;
        if (c01 < 0) c01 += mod;
        if (c11 < 0) c11 += mod;
        ll mid = (c01 + c10) % mod;
        ll res = (c00 + mid * BASEmod) % mod;
        res = (res + c11 * BASE2) % mod;
        C[i] = res;
    }
    return C;
}

ll n, m, p;

int main() {
    cin >> n >> m >> p;
    vector<ll> F(n + 1), G(m + 1);
    for (int i = 0; i <= n; ++i) cin >> F[i];
    for (int i = 0; i <= m; ++i) cin >> G[i];
    auto res = conv(F, G, p);
    for (auto& x : res) cout << x << " ";
    return 0;
}