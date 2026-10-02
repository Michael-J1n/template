#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;
const int MX = 1e5 + 5;

/**
 * 预处理乘法逆元，保证模数为素数
 * 复杂度 O(N)
 */
int inv[MX];
void init() {
	inv[1] = 1;
	for (int i = 2; i < MX; ++i) {
		inv[i] = (mod - mod / i) * inv[mod % i] % mod;
	}
}