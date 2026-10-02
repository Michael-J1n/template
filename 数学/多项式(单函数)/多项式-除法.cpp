#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 4e6;
const int mod = 998244353, g = 3;
ll w;
struct Complex {
    ll re, im;
    Complex(ll _re = 0, ll _im = 0) : re(_re), im(_im) {}
    Complex operator+(const Complex& c) const {
        return Complex((re + c.re) % mod, (im + c.im) % mod);
    }
    Complex operator*(const Complex& c) const {
        return Complex(
            (re * c.re % mod + im * c.im  % mod * w % mod) % mod,
            (re * c.im % mod + im * c.re % mod) % mod
        );
    }
};
Complex qpow(Complex a, ll b) {
	Complex res(1, 0);
	while (b) {
		if (b & 1) res = res * a;
		a = a * a, b >>= 1;
	}
	return res;
}
bool check_qr(ll n, int mod) {
	return qpow(Complex(n, 0), (mod - 1) / 2).re == 1;
}
ll get_qr(ll n, int mod) {
	if (n == 0) return 0;
	if (!check_qr(n, mod)) return -1;
	ll a;
	for (int i = 0; i < mod; ++i) {
		if (!check_qr(i * i - n, mod)) {
			a = i, w = ((a * a % mod - n) % mod + mod) % mod;
			break;
		}
	}
    int x = (qpow(Complex(a, 1), (mod + 1) / 2).re % mod + mod) % mod;
    if (x > mod - x) x= mod - x;  // 保留小的解
	return x;
}

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
 * 给定 n 次多项式 F 与 m 次多项式 G，求多项式 Q, R 使得 F = Q * G + R，其中 degQ = n - m, degR<degQ
 * 复杂度 O(nlogn)
 * */
void poly_div(const vector<ll>& F, const vector<ll>& G, vector<ll>& Q, vector<ll>& R) {
    int n = F.size() - 1, m = G.size() - 1;
    if (n < 0 || m < 0) {Q.assign(1, 0), R = F; return;}
    if (n < m) {Q.assign(1, 0), R = F; return;}
    int k = n - m;
    vector<ll> Fr(k + 1), Gr(m + 1);
    for (int i =  0; i <= k; ++i) Fr[i] = F[n - i];
    for (int i = 0; i <= m; ++i) Gr[i] = G[m - i];
    auto invGr = poly_inv(Gr, k + 1);
    auto Qr = conv(Fr, invGr, k + 1);
    Q.assign(k + 1, 0);
    for (int i = 0; i <= k; ++i) {
        ll v = Qr[k - i] % mod;
        Q[i] = v;
    }
    auto GQ = conv(G, Q, n + 1);
    R.assign(m, 0);
    for (int i = 0; i < m; ++i) {
        ll fi = (i <= n ? F[i] : 0), gi = (i < GQ.size() ? GQ[i] : 0);
        ll v = (fi - gi) % mod;
        if (v < 0) v += mod;
        R[i] = v;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<ll> F(n + 1), G(m + 1);
    for (int i = 0; i <= n; ++i) cin >> F[i];
    for (int i = 0; i <= m; ++i) cin >> G[i];
    vector<ll> Q, R;
    poly_div(F, G, Q, R);
    for (auto& x : Q) cout << x << " ";
    cout << endl;
    for (auto& x : R) cout << x << " ";
    return 0;
}