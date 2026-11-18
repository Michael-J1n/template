#include <bits/stdc++.h>
using namespace std;

/**
 * 单点分解质因数；返回 {pi,ci} 的数组
 * 
 * 复杂度：O(sqrtn)
 */
vector<pair<int, int>> fact(int n) {
	vector<pair<int, int>> res;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			int c = 0;
			while (n % i == 0) n /= i, ++c;
			res.emplace_back(i, c);
		}
	}
	if (n > 1) res.emplace_back(n, 1);
	return res;
}

/**
 * 预处理 lpf 快速分解质因数，适合需要批量处理的情况
 * 
 * 复杂度：预处理 O(MX)，查询最坏 O(log n)
 */
const int MX = 1e6 + 5;
int cnt = 0, prim[MX]; bool vis[MX];
int lpf[MX];
void init() {
    vis[1] = false;
    for (int i = 2; i < MX; ++i) {
        if (!vis[i]) prim[++cnt] = i, lpf[i] = i;
        for (int j = 1; i * prim[j] < MX; ++j) {
            vis[i * prim[j]] = true, lpf[i * prim[j]] = prim[j];
            if (i % prim[j] == 0) break;
        }
    }
}
/** 生成质因子，O(d) */
vector<pair<int, int>> fact(int n) {
	vector<pair<int, int>> res;
	while (n > 1) {
		int p = lpf[n], c = 0;
		while (n % p == 0) n /= p, ++c;
		res.emplace_back(p, c);
	}
	return res;
}
/** 生成因子，O(d) */
vector<int> getdivs(int x) {
    vector<int> ds{1};
    while (x > 1) {
        int p = lpf[x], sz = ds.size(), pw = 1;
        do {
            x /= p, pw *= p;
            for (int i = 0; i < sz; ++i) ds.emplace_back(ds[i] * pw);
        } while (x > 1 && lpf[x] == p);
    }
    // sort(ds.begin(), ds.end());
    return ds;
}