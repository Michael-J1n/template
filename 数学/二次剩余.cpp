#include <bits/stdc++.h>
using namespace std;
#define int long long

/**
 * Cipolla 算法求 n 的二次剩余 x^2=n(mod p)，复杂度 O(logp)
 * 核心思想：找到a使得i^2=w=a^2-n为非二次剩余，结果即为(a+i)^{(p+1)/2}
 * 复杂度 O(Logp)
 * */

int mod, w;

// 复数类，这里阔域的i有i^2=w=a^2-n
struct Complex {
    int re, im;
    Complex(int _re = 0, int _im = 0) : re(_re), im(_im) {}
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

// 复数快速幂
Complex qpow(Complex a, int b) {
	Complex res(1, 0);
	while (b) {
		if (b & 1) res = res * a;
		a = a * a, b >>= 1;
	}
	return res;
}

// 欧拉准则：n^{(p-1)/2} <=> n为二次剩余
bool check_qr(int n, int mod) {
	return qpow(Complex(n, 0), (mod - 1) / 2).re == 1;
}

// 求n的根：找到a使得a^2-n为非二次剩余，那么(a+i)^{(p+1)/2}即为根
int get_qr(int n, int mod) {
	if (n == 0) return 0;
	if (!check_qr(n, mod)) return -1;
	int a;
	for (int i = 0; i < mod; ++i) {
		if (!check_qr(i * i - n, mod)) {
			a = i, w = ((a * a % mod - n) % mod + mod) % mod;
			break;
		}
	}
	int res = (qpow(Complex(a, 1), (mod + 1) / 2).re % mod + mod) % mod;
	int res2 = mod - res;
	return min(res, res2);  // 返回其中更小的那个根
}

int t, n;

void solve() {
	cin >> n >> mod;
    n %= mod;
	int res = get_qr(n, mod);
	if (res == -1) cout << "Hola!" << endl;
	else if (res == 0) cout << res << endl;
	else {
		int a = res, b = mod - res;
		if (a > b) swap(a, b);
		cout << a << " " << b << endl;
	}
}

signed main() {
	ios::sync_with_stdio(false); cin.tie(nullptr);
	cin >> t;
	while (t--) solve();
	return 0;
}