/** 
 * Luogu P4213
 * 杜教筛：O(n^{2/3}) 时间内求出一类特殊函数的前缀和
 * 优化：1. 预处理剪枝  2. 记忆化搜索减枝
 * */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e7 + 5;
int prim[N];
int cnt = 0;
int vis[N];
ll phi[N];
unordered_map<int, ll> memo_phi;
ll mu[N];
unordered_map<int, ll> memo_mu;

void init() {
	vis[1] = true, phi[1] = 1, mu[1] = 1;
	for (int i = 2; i < N; ++i) {
		if (!vis[i]) prim[++cnt] = i, phi[i] = i - 1, mu[i] = -1;
		for (int j = 1; j <= cnt && i * prim[j] < N; ++j) {
			int p = prim[j];
			vis[i * p] = true;
			if (i % p == 0) {
				phi[i * p] = phi[i] * p;
				mu[i * p] = 0;
				break;
			}
			phi[i * p] = phi[i] * (p - 1);
			mu[i * p] = -mu[i];
		}
	}
    for (int i = 1; i < N; ++i) phi[i] += phi[i - 1], mu[i] += mu[i - 1];
}

ll Sphi(ll n) {
    if (n < N) return phi[n];
    if (memo_phi.count(n)) return memo_phi[n];
    long long res = 1LL * n * (n + 1) / 2;
    for (ll l = 2, r; l <= n; l = r + 1) {
        int q = n / l;
        r = n / q;
        res -= 1LL * (r - l + 1) * Sphi(q);
    }
    memo_phi[n] = res;
    return res;
}

ll Smu(ll n) {
    if (n < N) return mu[n];
    if (memo_mu.count(n)) return memo_mu[n];
    long long res = 1LL;
    for (ll l = 2, r; l <= n; l = r + 1) {
        int q = n / l;
        r = n / q;
        res -= 1LL * (r - l + 1) * Smu(q);
    }
    memo_mu[n] = res;
    return res;
}

int t;
ll n;

void solve() {
    cin >> n;
    cout << Sphi(n) << " " << Smu(n) << endl;
}

int main() {
    init();
    cin >> t;
    while (t--) solve();
    return 0;
}