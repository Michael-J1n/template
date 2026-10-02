#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int mod = 1e9 + 7;

// 求 a^b，复杂度 O(logb)
int qpow(int a, int b) {
	ll res = 1;
	while (b) {
		if (b & 1) res = res * a % mod;
		a = 1LL * a * a % mod, b >>= 1;
	}
	return (int)((res % mod + mod) % mod);
}