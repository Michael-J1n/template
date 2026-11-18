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
    bool check(int x, int y) { return find(x) == find(y); }
};
long long kruskalMST(vector<vector<pair<int, long long>>>& g) {
    int n = g.size();
    vector<tuple<long long, int, int>> edges;
    for (int u = 0; u < n; ++u) {
        for (auto& [v, w] : g[u]) {
            if (u < v) edges.push_back({w, u, v});
        }
    }
    sort(edges.begin(), edges.end());
    UFS s(n);
    long long wsum = 0;
    int ecnt = 0;
    for (auto& [w, u, v] : edges) {
        if (s.check(u, v)) continue;
        s.unite(u, v);
        wsum += w, ++ecnt;
    }
    return ecnt == n - 1 ? wsum : -1;
}

/**
 * 曼哈顿 MST 优化建图
 * 给定 n 个点，求一颗在这些点构成的完全图上的 MST
 * 点编号 0-based；points[i] = {x, y}；返回带权无向邻接表 g
 *
 * 根据 MST 性质可以证明，最终图上每个点的候选邻点只有 8 个 45° 角扇形区的最近点。
 * 对一个固定方向按照 x+y 扫描，并利用 active 中 x 与 x-y 均单调递增的性质寻找候选边；
 * 通过坐标变换覆盖所有方向，最后在优化图上求 MST 即可。
 *
 * 复杂度：O(n log n)
 */
vector<vector<pair<int, long long>>> manhattanMSTGraph(vector<pair<int, int>>& points) {
    int n = points.size();
    vector<pair<long long, long long>> p(n);
    for (int i = 0; i < n; ++i) p[i] = points[i];
    vector<int> ord(n); iota(ord.begin(), ord.end(), 0);
    vector<pair<int, int>> edges;
    for (int dir = 0; dir < 4; ++dir) {
        sort(ord.begin(), ord.end(), [&](int i, int j) {
            auto& [xi, yi] = p[i];
            auto& [xj, yj] = p[j];
            if (xi + yi != xj + yj) return xi + yi < xj + yj;
            if (xi != xj) return xi < xj;
            return yi < yj;
        });
        set<pair<long long, int>> active;
        // 按照 x+y 从小到大扫描；可以证明 active 中点按照 x 与 x-y 都单调递增
        for (auto i : ord) {
            auto& [x, y] = p[i];
            auto it = active.upper_bound({x, numeric_limits<int>::max()});
            while (it != active.begin()) {
                --it;
                int j = it->second;
                auto& [xx, yy] = p[j];
                if (xx - yy < x - y) break;
                edges.push_back({min(i, j), max(i, j)});
                it = active.erase(it);
            }
            active.insert({x, i});
        }
        if (dir & 1) {
            for (auto& [x, y] : p) x = -x;
        } else {
            for (auto& [x, y] : p) swap(x, y);
        }
    }
    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());
    vector<vector<pair<int, long long>>> g(n);
    for (auto& [u, v] : edges) {
        long long dx = 1LL * points[u].first - points[v].first;
        long long dy = 1LL * points[u].second - points[v].second;
        long long w = abs(dx) + abs(dy);
        g[u].push_back({v, w}), g[v].push_back({u, w});
    }
    return g;
}
long long manhattanMST(vector<pair<int, int>>& points) {
    auto g = manhattanMSTGraph(points);
    return kruskalMST(g);
}