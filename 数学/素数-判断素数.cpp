// 单点判断素数，复杂度 O(sqrt(n))
// 例题：LC3044

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

bool isPrime(ll n) {
	if (n < 2) return false;
	if (n % 2 == 0) return n == 2;
	for (ll d = 3; d * d <= n; d += 2) {
		if (n % d == 0) return false;
	}
	return true;
}