#include <bits/stdc++.h>
using namespace std;
const long double PI = acosl(-1.0L);

struct Complex {
    long double x, y;
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
        Complex w1({cosl(2 * PI / i), sinl(2 * PI / i) * op});
        for (int j = 0; j < n; j += i) {
            Complex wk({1, 0});
            for (int k = j; k < j + i / 2; ++k) {
                Complex x = A[k], y = A[k + i / 2] * wk;
                A[k] = x + y, A[k + i / 2] = x - y;
                wk = wk * w1;
            }
        }
    }
    if (op == -1) {
        for (int i = 0; i < n; ++i) A[i].x /= n, A[i].y /= n;
    }
}

vector<long double> conv(vector<long double> P, vector<long double> Q) {
    int n = P.size(), m = Q.size();
    int N = 1, M = n + m - 1;
    while (N < M) N <<= 1;

    vector<Complex> A(N), B(N);
    for (int i = 0; i < n; ++i) A[i].x = P[i];
    for (int i = 0; i < m; ++i) B[i].x = Q[i];

    FFT(A, N, 1);
    FFT(B, N, 1);
    for (int i = 0; i < N; ++i) A[i] = A[i] * B[i];
    FFT(A, N, -1);

    vector<long double> res(M);
    for (int i = 0; i < M; ++i) res[i] = A[i].x;
    return res;
}

using poly = vector<long double>;