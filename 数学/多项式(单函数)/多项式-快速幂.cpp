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

vector<ll> deriv(const vector<ll>& A) {
	int n = A.size();
	if (n == 0) return {};
	vector<ll> B(n - 1);
	for (int i = 1; i < n; ++i) B[i - 1] = A[i] * i % mod;
	return B;
}

vector<ll> integ(const vector<ll>& A) {
	int n = A.size();
	vector<ll> B(n + 1);

	static vector<ll> invs{0, 1};
	if (invs.size() <= n) {
		int old = invs.size();
		invs.resize(n + 1);
		for (int i = old; i <= n; ++i) invs[i] = mod - 1LL * (mod / i) * invs[mod % i] % mod;
	}

	B[0] = 0;
	for (int i = 0; i < n; ++i) B[i + 1] = A[i] * invs[i + 1] % mod;
	return B;
}

vector<ll> ln(const vector<ll>& A, int n) {
    auto tmp = conv(deriv(A), poly_inv(A, n), n - 1);
    auto res = integ(tmp);
    res.resize(n);
    return res;
}

vector<ll> exp(const vector<ll>& A, int n) {
    vector<ll> B(1, 1);
    for (int m = 1; m < n; m <<= 1) {
        int k = min(n, m << 1);
        vector<ll> Bk = B;
        Bk.resize(k);
        auto lnB = ln(Bk, k);
        vector<ll> T(k); T[0] = 1;
        for (int i = 0; i < k; ++i) {
            ll ai = (i < A.size() ? A[i] : 0);
            T[i] = (T[i] + ai - lnB[i]) % mod;
            if (T[i] < 0) T[i] += mod;
        }
        B = conv(Bk, T, k);
        B.resize(k);
    }
    B.resize(n);
    return B;
}

/**
 * 输入：多项式 A，次数 n - 1，保证 A[0] = 1；次数 k
 * 输出：多项式 B，满足 B = A^k (mod x^n)
 * */
vector<ll> poly_pow1(const vector<ll>& A, int n, int k) {
   auto tmp = ln(A, n);
    for (auto& x : tmp) (x *= k) %= mod;
    auto res = exp(tmp, n);
    return res;
}

int main() {
    int n; string K; cin >> n >> K;
    ll k = 0;
    for (int i = 0; i < K.size(); ++i) {
        k = (k * 10 + (K[i] - '0')) % mod;
    }
    vector<ll> A(n);
    for (int i = 0; i < n; ++i) cin >> A[i];
    auto res = poly_pow1(A, n, k);
    for (auto& x : res) cout << x << " ";
    return 0;
}