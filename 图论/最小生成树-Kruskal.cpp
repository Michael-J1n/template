#include <bits/stdc++.h>
using namespace std;

/**
 * Kruskal 算法求最小生成树
 * 返回MST的边权值和，若为-1代表图不连通
 * 
 * 复杂度：O(mlogm)
 */
struct UFS {
	int n, cc;
	vector<int> pa, sz;
	UFS(int _n) : n(_n), cc(_n) {
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
    	sz[px] += sz[py], sz[py] = 0, --cc;
    }
    bool check(int x, int y) {
    	return find(x) == find(y);
    }
};
long long kruskalMST(int n, vector<array<int, 3>>& edges) {
    sort(edges.begin(), edges.end(), [](const auto& e1, const auto& e2) {
        return e1[2] < e2[2];
    });
    UFS s(n);
    long long wsum = 0;
    int ecnt = 0;
    for (auto& e : edges) {
        int u = e[0], v = e[1], w = e[2];
        if (s.check(u, v)) continue;
        s.unite(u, v);
        wsum += w, ++ecnt;
    }
    return ecnt == n - 1 ? wsum : -1;
}