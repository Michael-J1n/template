#include <bits/stdc++.h>
using namespace std;

/**
* Polynomial / 多项式全家桶
*
* poly：vector<int>，A[i] 表示 x^i 的系数，模数固定为 998244353
* norm(A)：删除 A 末尾多余的 0
* poly_add(A, B)：返回 A+B
* poly_sub(A, B)：返回 A-B
* conv(A, B)：返回 A*B
* conv(A, B, n)：返回 A*B 的前 n 项
* poly_inv(A, n)：返回 A^{-1} mod x^n
* deriv(A)：返回 A 的导数
* integ(A)：返回 A 的积分，常数项为 0
* ln(A, n)：返回 ln(A) mod x^n
* exp(A, n)：返回 exp(A) mod x^n
* poly_pow(A, n, k)：返回 A^k mod x^n，k 为非负 long long
* mod_sqrt(x)：返回 x 的较小模平方根
* poly_sqrt(A, n)：返回满足 B^2=A mod x^n 的 B
* poly_div(F, G, Q, R)：计算 F=GQ+R，结果写入 Q、R
* poly_mod(F, G)：返回 F mod G
* eval(A, x)：返回 A(x)
* batch_inv(A)：返回每个 A[i] 的逆元

* ProductTree(x)：对非空点集 x 建立乘积树
* ProductTree::eval(F)：返回所有 F(x[i])
* ProductTree::interpolate(y)：返回满足 F(x[i])=y[i] 的次数小于 x.size() 的 F
* taylor_shift(A, c)：返回 A(x+c)
*
* conv、poly_inv、ln、exp、poly_pow、poly_sqrt、poly_div、poly_mod、taylor_shift 的复杂度 O(nlogn)
* ProductTree 建树、多点求值、插值的复杂度 O(nlog^2n)，空间 O(nlogn)
* deriv、integ、eval、batch_inv 的复杂度 O(n)
*
* 所有系数必须位于 [0, mod)，任意一次 NTT 长度必须不超过 2^23
* poly_inv 要求 A[0]!=0；ln 要求 A[0]==1；exp 要求 A[0]==0
* poly_sqrt 要求平方根存在；mod_sqrt 要求输入为二次剩余
* poly_div/poly_mod 要求 F、G 为规范多项式，且 G 非零
* batch_inv 要求所有 A[i]!=0
* ProductTree::interpolate 要求横坐标两两不同，且 x.size()==y.size()
* poly_pow/poly_sqrt 要求 A.size()>=n，其余 FPS 函数缺失的高次项视为 0
*/
namespace Polynomial {
	using ll = long long; using poly = vector<int>;
	const int PMOD = 998244353, PG = 3, NAIVE = 60;
	int qpow(int a, ll b) {
		int res = 1;
		while (b) { if (b & 1) res = 1LL * res * a % PMOD; a = 1LL * a * a % PMOD, b >>= 1; }
		return res;
	}
	int inv(int a) { return qpow(a, PMOD - 2); }
	void norm(poly& A) { while (A.size() > 1 && A.back() == 0) A.pop_back(); }
	poly poly_add(poly A, const poly& B) {
		if (A.size() < B.size()) A.resize(B.size()); int m = B.size();
		for (int i = 0; i < m; ++i) { A[i] += B[i]; if (A[i] >= PMOD) A[i] -= PMOD; }
		return A;
	}
	poly poly_sub(poly A, const poly& B) {
		if (A.size() < B.size()) A.resize(B.size()); int m = B.size();
		for (int i = 0; i < m; ++i) { A[i] -= B[i]; if (A[i] < 0) A[i] += PMOD; }
		return A;
	}
	void NTT(poly& A, int op) {
		int n = A.size(); if (n == 1) return;
		static vector<int> rev; static poly roots{0, 1};
		int rn = rev.size();
		if (rn != n) {
			int k = __builtin_ctz(n);
			rev.resize(n); rev[0] = 0; for (int i = 1; i < n; ++i) rev[i] = rev[i >> 1] >> 1 | (i & 1) << k - 1;
		}
		int rs = roots.size();
		if (rs < n) {
			int k = __builtin_ctz(rs); roots.resize(n);
			while (1 << k < n) {
				int e = qpow(PG, PMOD - 1 >> k + 1);
				for (int i = 1 << k - 1; i < 1 << k; ++i) roots[i << 1] = roots[i], roots[i << 1 | 1] = 1LL * roots[i] * e % PMOD;
				++k;
			}
		}
		for (int i = 0; i < n; ++i) if (i < rev[i]) swap(A[i], A[rev[i]]);
        for (int len = 1; len < n; len <<= 1) for (int i = 0; i < n; i += len << 1) for (int j = 0; j < len; ++j) {
            int x = A[i + j], y = 1LL * A[i + j + len] * roots[len + j] % PMOD;
            A[i + j] = x + y < PMOD ? x + y : x + y - PMOD; A[i + j + len] = x - y < 0 ? x - y + PMOD : x - y;
        }
		if (op == -1) { reverse(A.begin() + 1, A.end()); int ni = inv(n); for (auto& x : A) x = 1LL * x * ni % PMOD; }
	}
	poly conv(poly P, poly Q, int need = -1) {
		if (need == 0 || P.empty() || Q.empty()) return {};
		int n = P.size(), m = Q.size();
		if (need != -1) { if (n > need) P.resize(need), n = need; if (m > need) Q.resize(need), m = need; } if (n > m) swap(P, Q), swap(n, m);
		int M = n + m - 1; if (need != -1) M = min(M, need);
		if (n <= NAIVE) {
			poly R(M);
			for (int i = 0; i < n; ++i) { int lim = min(m, M - i); for (int j = 0; j < lim; ++j) R[i + j] = (R[i + j] + 1LL * P[i] * Q[j]) % PMOD; }
			return R;
		}
		int N = 1; while (N < n + m - 1) N <<= 1;
		P.resize(N), Q.resize(N); NTT(P, 1), NTT(Q, 1);
		for (int i = 0; i < N; ++i) P[i] = 1LL * P[i] * Q[i] % PMOD;
		NTT(P, -1); P.resize(M); return P;
	}
	poly poly_inv(const poly& A, int n) {
		assert(A[0] != 0);
		poly B{inv(A[0])};
		for (int m = 1; m < n; m <<= 1) {
			int k = min(n, m << 1), asz = A.size(), sz = min(k, asz);
			poly T(A.begin(), A.begin() + sz); T = conv(T, B, k); T.resize(k);
			for (auto& x : T) x = x ? PMOD - x : 0;
			T[0] += 2; if (T[0] >= PMOD) T[0] -= PMOD;
			B = conv(B, T, k); B.resize(k);
		}
		B.resize(n); return B;
	}
	poly deriv(const poly& A) {
		int n = A.size(); if (n <= 1) return {};
		poly B(n - 1); for (int i = 1; i < n; ++i) B[i - 1] = 1LL * A[i] * i % PMOD;
		return B;
	}
	poly integ(const poly& A) {
		int n = A.size();
		poly B(n + 1); static poly invs{0, 1};
		if (invs.size() <= n) {
			int old = invs.size(); invs.resize(n + 1);
			for (int i = old; i <= n; ++i) invs[i] = PMOD - 1LL * (PMOD / i) * invs[PMOD % i] % PMOD;
		}
		for (int i = 0; i < n; ++i) B[i + 1] = 1LL * A[i] * invs[i + 1] % PMOD;
		return B;
	}
	poly ln(const poly& A, int n) {
		assert(A[0] == 1);
		int asz = A.size(), sz = min(n, asz); poly T(A.begin(), A.begin() + sz);
		auto res = integ(conv(deriv(T), poly_inv(T, n), n - 1)); res.resize(n); return res;
	}
	poly exp(const poly& A, int n) {
		assert(A[0] == 0);
		poly B{1};
		for (int m = 1; m < n; m <<= 1) {
			int k = min(n, m << 1), asz = A.size(), sz = min(k, asz);
			poly T(k); for (int i = 0; i < sz; ++i) T[i] = A[i];
			auto L = ln(B, k);
			T[0]++; if (T[0] == PMOD) T[0] = 0;
			for (int i = 0; i < k; ++i) { T[i] -= L[i]; if (T[i] < 0) T[i] += PMOD; }
			B = conv(B, T, k); B.resize(k);
		}
		B.resize(n); return B;
	}
	void poly_div(const poly& F, const poly& G, poly& Q, poly& R) {
		int n = F.size() - 1, m = G.size() - 1;
		if (n < m) { Q = {0}, R = F; return; }
		int k = n - m;
		poly Fr(k + 1); for (int i = 0; i <= k; ++i) Fr[i] = F[n - i];
		poly Gr(min(m, k) + 1); for (int i = 0; i < Gr.size(); ++i) Gr[i] = G[m - i];
		auto Qr = conv(Fr, poly_inv(Gr, k + 1), k + 1);
		Q.resize(k + 1); for (int i = 0; i <= k; ++i) Q[i] = Qr[k - i];
		auto GQ = conv(G, Q, m); R.assign(max(1, m), 0);
		for (int i = 0; i < m; ++i) { R[i] = F[i] - GQ[i]; if (R[i] < 0) R[i] += PMOD; }
		norm(Q), norm(R);
	}
	poly poly_mod(const poly& F, const poly& G) {
		if (F.size() < G.size()) return F;
		poly Q, R; poly_div(F, G, Q, R); return R;
	}
	poly poly_pow1(const poly& A, int n, ll k) {
		auto T = ln(A, n); int km = k % PMOD;
		for (auto& x : T) x = 1LL * x * km % PMOD;
		return exp(T, n);
	}
	poly poly_pow(const poly& A, int n, ll k) {
		if (k == 0) { poly R(n); R[0] = 1; return R; }
		int t = 0; while (t < n && A[t] == 0) ++t;
		if (t == n) return poly(n); if (t && k >= (n + t - 1) / t) return poly(n);
		int shift = t * k, m = n - shift;
		poly F(m); for (int i = 0; i < m; ++i) F[i] = A[i + t];
		int c = F[0], ic = inv(c); for (auto& x : F) x = 1LL * x * ic % PMOD;
		auto R = poly_pow1(F, m, k); int ck = qpow(c, k);
		poly res(n); for (int i = 0; i < m; ++i) res[i + shift] = 1LL * R[i] * ck % PMOD;
		return res;
	}
	int mod_sqrt(int n) {
		if (n == 0) return 0;
		int q = PMOD - 1, s = 0; while (!(q & 1)) q >>= 1, ++s;
		int c = qpow(PG, q), x = qpow(n, (q + 1) / 2), t = qpow(n, q), m = s;
		while (t != 1) {
			int i = 1, v = 1LL * t * t % PMOD;
			while (v != 1) v = 1LL * v * v % PMOD, ++i;
			int b = qpow(c, 1LL << m - i - 1);
			x = 1LL * x * b % PMOD, c = 1LL * b * b % PMOD, t = 1LL * t * c % PMOD, m = i;
		}
		return min(x, PMOD - x);
	}
	poly poly_sqrt1(const poly& A, int n) {
		poly B{1}; const int inv2 = PMOD + 1 >> 1;
		for (int m = 1; m < n; m <<= 1) {
			int k = min(n, m << 1), asz = A.size(), sz = min(k, asz);
			poly C(A.begin(), A.begin() + sz); C = conv(C, poly_inv(B, k), k); C.resize(k);
			B.resize(k); for (int i = 0; i < k; ++i) B[i] = 1LL * (B[i] + C[i]) * inv2 % PMOD;
		}
		B.resize(n); return B;
	}
	poly poly_sqrt(const poly& A, int n) {
		int t = 0; while (t < n && A[t] == 0) ++t;
		if (t == n) return poly(n);
		int shift = t >> 1, len = n - shift, c = A[t], s = mod_sqrt(c), ic = inv(c);
		poly F(len); for (int i = 0; i < len && i + t < n; ++i) F[i] = 1LL * A[i + t] * ic % PMOD;
		auto S = poly_sqrt1(F, len); poly R(n);
		for (int i = 0; i < len; ++i) R[i + shift] = 1LL * S[i] * s % PMOD;
		return R;
	}
	int eval(const poly& A, int x) {
		int res = 0;
		for (int i = A.size() - 1; i >= 0; --i) res = (1LL * res * x + A[i]) % PMOD;
		return res;
	}
	poly batch_inv(const poly& A) {
		int n = A.size(); poly pre(n + 1, 1), R(n);
		for (int i = 0; i < n; ++i) pre[i + 1] = 1LL * pre[i] * A[i] % PMOD;
		int cur = inv(pre[n]);
		for (int i = n - 1; i >= 0; --i) R[i] = 1LL * pre[i] * cur % PMOD, cur = 1LL * cur * A[i] % PMOD;
		return R;
	}
	struct ProductTree {
		int n; poly x; vector<poly> tr;
		ProductTree(const poly& _x) : n(_x.size()), x(_x), tr(n << 2) { build(1, 0, n - 1); }
		void build(int p, int l, int r) {
			if (l == r) { tr[p] = {(PMOD - x[l]) % PMOD, 1}; return; }
			int mid = l + r >> 1; build(p << 1, l, mid), build(p << 1 | 1, mid + 1, r);
			tr[p] = conv(tr[p << 1], tr[p << 1 | 1]);
		}
		void eval_dfs(int p, int l, int r, const poly& F, poly& R) const {
			if (l == r) { R[l] = F[0]; return; }
			int mid = l + r >> 1;
			poly L = F.size() < tr[p << 1].size() ? F : poly_mod(F, tr[p << 1]), H = F.size() < tr[p << 1 | 1].size() ? F : poly_mod(F, tr[p << 1 | 1]);
			eval_dfs(p << 1, l, mid, L, R), eval_dfs(p << 1 | 1, mid + 1, r, H, R);
		}
		poly eval(const poly& F) const {
			poly R(n), T = F; norm(T);
			if (T.size() >= tr[1].size()) T = poly_mod(T, tr[1]);
			eval_dfs(1, 0, n - 1, T, R); return R;
		}
		poly interp_dfs(int p, int l, int r, const poly& w) const {
			if (l == r) return {w[l]};
			int mid = l + r >> 1;
			auto L = interp_dfs(p << 1, l, mid, w), R = interp_dfs(p << 1 | 1, mid + 1, r, w);
			return poly_add(conv(L, tr[p << 1 | 1]), conv(R, tr[p << 1]));
		}
		poly interpolate(const poly& y) const {
			auto d = Polynomial::deriv(tr[1]), v = eval(d), iv = batch_inv(v); poly w(n);
			for (int i = 0; i < n; ++i) w[i] = 1LL * y[i] * iv[i] % PMOD;
			auto R = interp_dfs(1, 0, n - 1, w); norm(R); return R;
		}
	};
	poly fac{1}, ifac{1};
	void init_fac(int n) {
		if (fac.size() > n) return;
		int old = fac.size(); fac.resize(n + 1), ifac.resize(n + 1);
		for (int i = old; i <= n; ++i) fac[i] = 1LL * fac[i - 1] * i % PMOD;
		ifac[n] = inv(fac[n]); for (int i = n; i >= old; --i) ifac[i - 1] = 1LL * ifac[i] * i % PMOD;
	}
	poly taylor_shift(const poly& A, int c) {
		int n = A.size(); if (n == 0) return {};
		init_fac(n - 1);
        poly P(n), Q(n); int pw = 1;
		for (int i = 0; i < n; ++i) P[n - 1 - i] = 1LL * A[i] * fac[i] % PMOD, Q[i] = 1LL * pw * ifac[i] % PMOD, pw = 1LL * pw * c % PMOD;
		auto T = conv(P, Q, n);
        poly R(n); for (int i = 0; i < n; ++i) R[i] = 1LL * T[n - 1 - i] * ifac[i] % PMOD;
		return R;
	}
} using namespace Polynomial;