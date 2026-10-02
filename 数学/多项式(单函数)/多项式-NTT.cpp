#include <bits/stdc++.h>
using namespace std;

const int ntt_mod = 998244353;
const int g = 3, gi = 332748118;
int qpow(int a, int b) {
	int res = 1;
	while (b) {
		if (b & 1) res = 1LL * res * a % ntt_mod;
		a = 1LL * a * a % ntt_mod, b >>= 1;
	}
	return res;
}
int inv(int a) { return qpow(a, ntt_mod - 2); }
void NTT(vector<long long>& A, int n, int op) {
	for (int i = 1, j = 0; i < n; ++i) {
		int b = n >> 1;
		for (; j & b; b >>= 1) j ^= b;
		j ^= b;
		if (i < j) swap(A[i], A[j]);
	}
	for (int i = 2; i <= n; i <<= 1) {
		long long g1 = qpow(op == 1 ? g : gi, (ntt_mod - 1) / i);
		for (int j = 0; j < n; j += i) {
			long long gk = 1;
			for (int k = j; k < j + i / 2; ++k) {
				long long x = A[k], y = A[k + i / 2] * gk % ntt_mod;
				A[k] = (x + y) % ntt_mod, A[k + i / 2] = (x - y + ntt_mod) % ntt_mod;
				gk = gk * g1 % ntt_mod;
			}
		}
	}
}
/**
 * 多项式卷积，NTT
 * 
 * 输入：多项式 P 与 Q（以系数方式保存，系数从左到右对应多项式次数从低到高）；need：在 mod x^need 意义下做运算
 * 输出：多项式 P * Q 的乘积
 * 保证：need!=0, P,Q 不空
 * 复杂度：O(nlogn)
 * 
 * i.e., 次数 n - 1 的多项式 P，次数 m - 1 的多项式 Q，卷出次数 n + m - 2 的多项式 P*Q；在 mod x^need 下考虑
 * */
vector<long long> conv(vector<long long> P, vector<long long> Q, int need = -1) {
	int n = P.size(), m = Q.size();
    if (need != -1) {
        if (n > need) P.resize(need), n = need;
        if (m > need) Q.resize(need), m = need;
    }
	int N = 1, M = n + m - 1;
	while (N < M) N <<= 1;
    P.resize(N), Q.resize(N);
	NTT(P, N, 1), NTT(Q, N, 1);
	for (int i = 0; i < N; ++i) P[i] = P[i] * Q[i] % ntt_mod;
	NTT(P, N, -1);
	int ni = inv(N);
	for (int i = 0; i < M; ++i) P[i] = P[i] * ni % ntt_mod;
    if (need != -1) M = min(M, need);
    P.resize(M);
	return P;
}


// 示例用法：P3803
int main() {
	int n, m; cin >> n >> m;
	vector<long long> P(n + 1), Q(m + 1);
	for (int i = 0; i <= n; ++i) cin >> P[i];
	for (int i = 0; i <= m; ++i) cin >> Q[i];
	auto res = conv(P, Q);
	for (int i = 0; i < res.size(); ++i) cout << res[i] << " ";
	return 0;
}