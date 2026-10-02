#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 4e6;
const int mod = 998244353, g = 3;

int qpow(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % mod;
		a = 1LL * a * a % mod, b >>= 1;
	}
	return res;
}

int inv(int a) {
	return qpow(a, mod - 2);
}

void NTT(vector<ll>& A, int n, int op) {
	for (int i = 1, j = 0; i < n; ++i) {
		int b = n >> 1;
		for (; j & b; b >>= 1) j ^= b;
		j ^= b;
		if (i < j) swap(A[i], A[j]);
	}
	for (int i = 2; i <= n; i <<= 1) {
		ll g1 = qpow(g, (mod - 1) / i);
        if (op == -1) g1 = inv(g1);
		for (int j = 0; j < n; j += i) {
			ll gk = 1;
			for (int k = j; k < j + i / 2; ++k) {
				ll x = A[k], y = A[k + i / 2] * gk % mod;
				A[k] = (x + y) % mod, A[k + i / 2] = (x - y + mod) % mod;
				gk = gk * g1 % mod;
			}
		}
	}
}

vector<ll> conv(vector<ll> P, vector<ll> Q, int need = -1) {
	int n = P.size(), m = Q.size();
    if (need != -1) {
        if (n > need) P.resize(need), n = need;
        if (m > need) Q.resize(need), m = need;
    }
	int N = 1, M = n + m - 1;
	while (N < M) N <<= 1;

    P.resize(N), Q.resize(N);
	NTT(P, N, 1), NTT(Q, N, 1);
	for (int i = 0; i < N; ++i) P[i] = P[i] * Q[i] % mod;
	NTT(P, N, -1);
	
	int ni = inv(N);
	for (int i = 0; i < M; ++i) P[i] = P[i] * ni % mod;
    if (need != -1) M = min(M, need);
    P.resize(M);
	return P;
}

vector<ll> poly_inv(const vector<ll>& A, int n) {
    vector<ll> B(1, inv(A[0]));
    for (int m = 1; m < n; m <<= 1) {
        int k = min(n, m << 1);
        vector<ll> tmp(k);
        for (int i = 0; i < k && i < A.size(); ++i) tmp[i] = A[i];
        auto ab = conv(tmp, B, k);
        for (int i = 0; i < k; ++i) ab[i] = (mod - ab[i]) % mod;
        ab[0] = (ab[0] + 2) % mod;
        B = conv(B, ab, k);
        B.resize(k);
    }
    B.resize(n);
    return B;
}

/**
 * 输入：多项式 A，次数 n - 1，保证 A[0] = 1
 * 输出：多项式 B，满足 B^2 = A (mod x^n)
 * 复杂度 O(nlogn)
 * */
vector<ll> poly_sqrt1(const vector<ll>& A, int n) {
    vector<ll> B(1, 1);
    const ll inv2 = (mod + 1) / 2;
    for (int m = 1; m < n; m <<= 1) {
        int k = min(n, m << 1);
        vector<ll> Bk = B; Bk.resize(k);
        vector<ll> Ak(k);
        for (int i = 0; i < k; ++i) Ak[i] = (i < A.size() ? (A[i] % mod + mod) % mod : 0);
        auto C = conv(Ak, poly_inv(Bk, k), k);
        B.resize(k);
        for (int i = 0; i < k; ++i) B[i] = (Bk[i] + C[i]) % mod * inv2 % mod;
    }
    B.resize(n);
    return B;
}

int main() {
    int n; cin >> n;
    vector<ll> A(n);
    for (int i = 0; i < n; ++i) cin >> A[i];
    auto res = poly_sqrt1(A, n);
    for (auto& x : res) cout << x << " ";
    return 0;
}