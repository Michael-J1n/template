#include <bits/stdc++.h>
using namespace std;

const int N = 4e6;
const double PI = acos(-1);

struct Complex {
	double x, y;
	Complex operator+(const Complex& t) const {
		return {x + t.x, y + t.y};
	}
	Complex operator-(const Complex& t) const {
		return {x - t.x, y - t.y};
	}
	Complex operator*(const Complex& t) const {
		return {x * t.x - y * t.y, x * t.y + y * t.x};
	}
};

void FFT(vector<Complex>& A, int n, int op) {
	for (int i = 1, j = 0; i < n; ++i) {
		int b = n >> 1;
		for (; j & b; b >>= 1) j ^= b;
		j ^= b;
		if (i < j) swap(A[i], A[j]);
	}
	for (int i = 2; i <= n; i <<= 1) {
		Complex w1({cos(2 * PI / i), sin(2 * PI / i) * op});
		for (int j = 0; j < n; j += i) {
			Complex wk({1, 0});
			for (int k = j; k < j + i / 2; ++k) {
				Complex x = A[k], y = A[k + i / 2] * wk;
				A[k] = x + y, A[k + i / 2] = x - y;
				wk = wk * w1;
			}
		}
	}
}

/**
 * 输入：多项式 P 与 Q（以系数方式保存，系数从左到右对应多项式次数从低到高）
 * 输出：多项式 P * Q 的乘积
 * 复杂度：O(nlogn)
 * */
vector<long long> conv(vector<double> P, vector<double> Q) {
	int n = P.size(), m = Q.size();
	if (n < m) swap(n, m), swap(P, Q);
	int N = 1, M = n + m - 1;
	while (N < M) N <<= 1;

	vector<Complex> A(N);
	for (int i = 0; i < n; ++i) {
		if (i < m) A[i] = {P[i], Q[i]};
		else A[i] = {P[i], 0};
	}
	FFT(A, N, 1);
	for (int i = 0; i < N; ++i) A[i] = A[i] * A[i];
	FFT(A, N, -1);
	
	vector<long long> res(M);
	for (int i = 0; i < M; ++i) res[i] = llround(A[i].y / (2.0 * N));
	return res;
}


double P[N], Q[N];

int main() {
	int n, m; cin >> n >> m;
	vector<double> P(n + 1), Q(m + 1);
	for (int i = 0; i <= n; ++i) cin >> P[i];
	for (int i = 0; i <= m; ++i) cin >> Q[i];
	auto res = conv(P, Q);
	for (int i = 0; i < res.size(); ++i) cout << res[i] << " ";
	return 0;
}