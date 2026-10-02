/**
 * 求一个关于 x 的多项式，过 (x1,y1),..,(xn,yn) 在 x = k 的值
 * 拉格朗日插值：
 * 过 n 个点的多项式为 f(x) = ∑Π (x-xj)/(xi-xj)*yi
 * */

#include <bits/stdc++.h>
using namespace std;

const int mod = 998244353;

inline int qpow(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = 1LL * res * a % mod;
        a = 1LL * a * a % mod, b >>= 1;
    }
    return res;
}

inline int inv(int x) {return qpow(x, mod - 2);}

inline int Lag(vector<int>& x, vector<int>& y, int k) {
    int n = x.size();
    int res = 0;
    for (int i = 0; i < n; ++i) {
        long long tmp = 1, m = 1;
        for (int j = 0; j < n; ++j) if (j != i) {
            tmp = (tmp * (k - x[j]) % mod + mod) %mod;
            m = (m * (x[i] - x[j]) % mod + mod) % mod;
        }
        res = ((res + inv(m) * tmp % mod * y[i] % mod) % mod + mod) % mod;
    }
    return res;
}

int n, k;

int main() {
    cin >> n >> k;
    vector<int> x(n), y(n);
    for (int i =0; i < n; ++i) cin >> x[i] >> y[i];
    int res = Lag(x, y, k);
    cout << res << endl;
    return 0;
}