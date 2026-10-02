#include <bits/stdc++.h>
using namespace std;

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
    int size(int x) {
        return sz[find(x)];
    }
};

/**
 * 时间倒流+并查集回答连通性问题【P1197】
 * 
 * 只有删点操作与询问，每次询问当前连通块数量；逆序回答，把删点变成加点。注意不能用并查集中的 cc（有死点），要额外维护
 * 注意：先删点，再询问，在倒序中为**先询问，再加点**
 */
void answerQs() {
    int n, m; cin >> n >> m;
    vector<char> alive(n, 1);
    vector<pair<int, int>> edges(m);
    vector<vector<int>> g(n);
    for (int i = 0; i < m; ++i) {
    	int u, v; cin >> u >> v;
    	edges[i] = {u, v};
    	g[u].emplace_back(v), g[v].emplace_back(u);
    }
    int k; cin >> k;
    vector<int> qs(k), res(k);
    for (int i = 0; i < k; ++i) {
    	int u; cin >> u;
    	alive[u] = 0;
    	qs[i] = u;
    }
    int cc = 0;
    for (int i = 0; i < n; ++i) {
        if (alive[i]) ++cc;
    }
    UFS s(n);
    for (int i = 0; i < m; ++i) {
    	auto [u, v] = edges[i];
    	if (alive[u] && alive[v] && !s.check(u, v)) s.unite(u, v), --cc;
    }
    for (int i = k - 1; ~i; --i) {
    	res[i] = cc;
    	int u = qs[i];
    	alive[u] = 1;
    	++cc;
        for (auto& v : g[u]) {
    		if (alive[v] && !s.check(u, v)) s.unite(u, v), --cc;
    	}
    }
    cout << cc << endl;
    for (auto& x : res) cout << x << endl;
}