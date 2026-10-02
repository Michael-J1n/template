#include <bits/stdc++.h>
using namespace std;

/**
 * 严格次小生成树：边权严格大于 MST 的一颗最小生成树
 * 
 * 算法：
 * 首先使用 Kruskal 求出原图的生成树
 * 枚举每个非树边 {u,v,w}，记 MST 中 u--v 路径中的最大权值为 mx，严格次大权值为 se
 * 为了让新生成树也足够小，因此如果 w>mx 时，使用 w 替换 mx；否则如果 w=mx，用 w 替换 se（注意 MST 的性质保证了 w>=mx）
 * 在 MST 树中求路径的最大/次大权值可以使用树上倍增的算法进行计算
 * 
 * 传入 edges 数组为 {u,v,w,0}，第四维设置为 0 即可，是为了让 Kruskal 进行标记树边
 * 
 * 复杂度为 O(mlogm)
 */

pair<int, int> merge(pair<int, int> a, pair<int, int> b) {
    if (a.first < b.first) return {b.first, max(a.first, b.second)};
    if (a.first > b.first) return {a.first, max(a.second, b.first)};
    return {a.first, max(a.second, b.second)};
}
struct TreeLift {
    const int INF = 0x3f3f3f3f;
    int n, m;
    vector<int> dep;
    vector<vector<int>> pa, mx, se;
    TreeLift(vector<vector<pair<int, int>>>& g, int root = 0) {
        n = g.size(), m = __lg(max(1, n)) + 1;
        dep.assign(n, 0);
        pa.assign(n, vector<int>(m));
        mx.assign(n, vector<int>(m, -INF));
        se.assign(n, vector<int>(m, -INF));
        dfs(g, root, root, 0);
    }
    void dfs(vector<vector<pair<int, int>>>& g, int u, int p, int w) {
        pa[u][0] = p;
        if (u != p) mx[u][0] = w, se[u][0] = -INF;
        for (int j = 1; j < m; ++j) {
            int x = pa[u][j - 1];
            pa[u][j] = pa[x][j - 1];
            pair<int, int> p1 = {mx[u][j - 1], se[u][j - 1]};
            pair<int, int> p2 = {mx[x][j - 1], se[x][j - 1]};
            pair<int, int> p = merge(p1, p2);
            mx[u][j] = p.first;
            se[u][j] = p.second;
        }
        for (auto& [v, nw] : g[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1;
            dfs(g, v, u, nw);
        }
    }
    pair<int, int> query(int u, int v) {
        pair<int, int> pu = {-INF, -INF}, pv = {-INF, -INF};
        if (dep[u] > dep[v]) swap(u, v);
        int d = dep[v] - dep[u];
        for (int j = 0; j < m; ++j) {
            if (d >> j & 1) {
                pv = merge(pv, {mx[v][j], se[v][j]});
                v = pa[v][j];
            }
        }
        if (u == v) return pv;
        for (int j = m - 1; j >= 0; --j) {
            if (pa[u][j] != pa[v][j]) {
                pu = merge(pu, {mx[u][j], se[u][j]});
                pv = merge(pv, {mx[v][j], se[v][j]});
                u = pa[u][j], v = pa[v][j];
            }
        }
        pu = merge(pu, {mx[u][0], se[u][0]});
        pv = merge(pv, {mx[v][0], se[v][0]});
        return merge(pu, pv);
    }
};

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
int kruskalMST(int n, vector<array<int, 4>>& edges) {
    sort(edges.begin(), edges.end(), [](const auto& e1, const auto& e2) {
        return e1[2] < e2[2];
    });
    UFS s(n);
    int cnt = 0, sum = 0;
    for (auto& e : edges) {
        int u = e[0], v = e[1], w = e[2];
        int& id = e[3];
        if (s.check(u, v)) continue;
        s.unite(u, v);
        id = 1;
        ++cnt, sum += w;
    }
    return sum;
}

int secMST(vector<array<int, 4>>& edges) {
    int sum = kruskalMST(n, edges);
    vector<vector<pair<int, int>>> g(n);
    for (auto& e : edges) {
        if (!e[3]) continue;
        int u = e[0], v = e[1], w = e[2];
        g[u].emplace_back(v, w), g[v].emplace_back(u, w);
    }
    int res = INT_MAX;
    TreeLift tr(g);
    for (auto& e : edges) {
        if (e[3]) continue;
        int u = e[0], v = e[1], w = e[2];
        auto [mx, se] = tr.query(u, v);
        if (w > mx) res = min(res, sum - mx + w);
        else if (w == mx && se != -0x3f3f3f3f) res = min(res, sum - se + w);
    }
    return res;
}