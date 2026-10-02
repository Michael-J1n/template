#include <bits/stdc++.h>
using namespace std;

/**
 * 并查集
 * 
 * 功能：
 * unite(x, y)：合并 x 与 y 所在连通块
 * check(x, y)：查询 x 与 y 是否在相同块
 * size(x)：查询 x 所在连通块大小
 * cc：连通块个数
 * mx：最大连通块大小
 * 
 * 复杂度：
 * 初始化 O(n)，查询均摊 O(1)
 */
struct UFS {
	int n, cc;
    int mx;
	vector<int> pa, sz;
	UFS(int _n) : n(_n), cc(_n), mx(1) {
		pa.resize(n), sz.resize(n);
		for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1;
	}
	int find(int x) {
        if (pa[x] != x) pa[x] = find(pa[x]);
        return pa[x];
    }
    void unite(int x, int y) {
    	int px = find(x), py = find(y);
    	if (px == py) return;
    	if (sz[px] < sz[py]) swap(px, py);
    	pa[py] = px;
    	sz[px] += sz[py], sz[py] = 0;
        --cc, mx = max(mx, sz[px]);
    }
    bool check(int x, int y) { return find(x) == find(y); }
    int size(int x) { return sz[find(x)]; }
};