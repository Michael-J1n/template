#include <bits/stdc++.h>
using namespace std;

/**
 * prim[] : 质数表
 * cnt    : 当前质数个数
 * vis[]  : 是否为合数
 * phi[]  : 欧拉函数 φ(n)          φ(n)=∣{1≤x≤n:gcd(x,n)=1}∣
 * mu[]   : 莫比乌斯函数 μ(n)       μ(n)=(-1)^k, n有k个不同因数；若n不是square-free，则 μ(n)=0.
 * d[]    : 约数个数函数 τ(n)       d(n)=τ(n)=#{x:x∣n}
 * f[]    : 约数和函数 σ(n)         f(n)=σ(n)=x∣n∑​x
 */
const int N = 1e6 + 5;
int prim[N], cnt = 0; char vis[N];
// ---------------------------------
int phi[N];
int mu[N];
int a[N], d[N];
long long g[N], f[N];
// ---------------------------------
void init() {
	vis[1] = true;
    phi[1] = 1;
    mu[1] = 1;
    a[1] = 0, d[1] = 1;
    g[1] = 0, f[1] = 1;
	for (int i = 2; i < N; ++i) {
		if (!vis[i]) {
            prim[++cnt] = i;
            phi[i] = i - 1;
            a[i] = 1, d[i] = 2;
            g[i] = 1 + i, f[i] = 1 + i;
            mu[i] = -1;
        }
		for (int j = 1; i * prim[j] < N; ++j) {
			int p = prim[j];
			vis[i * p] = 1;
			if (i % p == 0) {
				phi[i * p] = phi[i] * p;
				a[i * p] = a[i] + 1, d[i * p] = d[i] / a[i * p] * (a[i * p] + 1);
				g[i * p] = g[i] * p + 1, f[i * p] = f[i] / g[i] * g[i * p];
				mu[i * p] = 0;
				break;
			}
			phi[i * p] = phi[i] * (p - 1);
			a[i * p] = 1, d[i * p] = d[i] * 2;
			g[i * p] = 1 + p, f[i * p] = f[i] * g[p];
			mu[i * p] = -mu[i];
		}
	}
}