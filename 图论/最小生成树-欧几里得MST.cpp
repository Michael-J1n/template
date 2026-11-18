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
long double kruskalMST(vector<vector<pair<int, long double>>>& g) {
    int n = g.size();
    vector<tuple<long double, int, int>> edges;
    for (int u = 0; u < n; ++u) {
        for (auto& [v, w] : g[u]) if (u < v) edges.push_back({w, u, v});
    }
    sort(edges.begin(), edges.end());
    UFS s(n);
    long double wsum = 0;
    int ecnt = 0;
    for (auto& [w, u, v] : edges) {
        if (s.check(u, v)) continue;
        s.unite(u, v);
        wsum += w, ++ecnt;
    }
    return ecnt == n - 1 ? wsum : -1;
}

/** Delaunay 三角剖分，O(n log n) */
struct Delaunay {
    struct point { long double x, y; int id; };
    struct edge { int org, next; };
    vector<point> p;
    vector<edge> e;
    vector<int> free_edge;
    static int sign(long double x, long double scale) {
        long double eps = 32 * numeric_limits<long double>::epsilon() * max(1.0L, scale);
        return (x > eps) - (x < -eps);
    }
    static int cross(const point& a, const point& b, const point& c) {
        long double u = (b.x - a.x) * (c.y - a.y), v = (b.y - a.y) * (c.x - a.x);
        return sign(u - v, abs(u) + abs(v));
    }
    static bool in_circle(const point& a, const point& b, const point& c, const point& d) {
        long double ax = a.x - d.x, ay = a.y - d.y, bx = b.x - d.x, by = b.y - d.y, cx = c.x - d.x, cy = c.y - d.y;
        long double a2 = ax * ax + ay * ay, b2 = bx * bx + by * by, c2 = cx * cx + cy * cy;
        long double t1 = bx * cy - by * cx, t2 = cx * ay - cy * ax, t3 = ax * by - ay * bx;
        long double det = a2 * t1 + b2 * t2 + c2 * t3;
        long double scale = a2 * (abs(bx * cy) + abs(by * cx)) + b2 * (abs(cx * ay) + abs(cy * ax)) + c2 * (abs(ax * by) + abs(ay * bx));
        return sign(det, scale) > 0;
    }
    static int rot(int x) { return (x & ~3) | ((x + 1) & 3); }
    static int rev(int x) { return x ^ 2; }
    int org(int x) const { return e[x].org; }
    int dest(int x) const { return org(rev(x)); }
    int onext(int x) const { return e[x].next; }
    int oprev(int x) const { return rot(onext(rot(x))); }
    int lnext(int x) const { return rot(onext(rev(rot(x)))); }
    bool left_of(int v, int x) const { return cross(p[org(x)], p[dest(x)], p[v]) > 0; }
    bool right_of(int v, int x) const { return cross(p[org(x)], p[dest(x)], p[v]) < 0; }
    int make_edge(int u, int v) {
        int x;
        if (free_edge.empty()) {
            x = e.size();
            e.resize(e.size() + 4);
        } else {
            x = free_edge.back();
            free_edge.pop_back();
        }
        e[x] = {u, x}, e[x + 1] = {-1, x + 3}, e[x + 2] = {v, x + 2}, e[x + 3] = {-1, x + 1};
        return x;
    }
    void splice(int a, int b) {
        int x = rot(onext(a)), y = rot(onext(b));
        swap(e[a].next, e[b].next), swap(e[x].next, e[y].next);
    }
    void erase_edge(int x) {
        splice(x, oprev(x)), splice(rev(x), oprev(rev(x)));
        x &= ~3;
        e[x].org = e[x + 2].org = -1;
        free_edge.push_back(x);
    }
    int connect(int a, int b) {
        int x = make_edge(dest(a), org(b));
        splice(x, lnext(a)), splice(rev(x), b);
        return x;
    }
    pair<int, int> divide(int l, int r) {
        if (r - l == 2) {
            int a = make_edge(l, l + 1);
            return {a, rev(a)};
        }
        if (r - l == 3) {
            int a = make_edge(l, l + 1), b = make_edge(l + 1, l + 2);
            splice(rev(a), b);
            int t = cross(p[l], p[l + 1], p[l + 2]);
            if (t == 0) return {a, rev(b)};
            int c = connect(b, a);
            if (t > 0) return {a, rev(b)};
            return {rev(c), c};
        }
        int m = l + r >> 1;
        auto [ldo, ldi] = divide(l, m);
        auto [rdi, rdo] = divide(m, r);
        while (true) {
            if (left_of(org(rdi), ldi)) ldi = lnext(ldi);
            else if (right_of(org(ldi), rdi)) rdi = onext(rev(rdi));
            else break;
        }
        int base = connect(rev(rdi), ldi);
        if (org(ldi) == org(ldo)) ldo = rev(base);
        if (org(rdi) == org(rdo)) rdo = base;
        while (true) {
            int lcand = onext(rev(base));
            if (right_of(dest(lcand), base)) {
                while (in_circle(p[dest(base)], p[org(base)], p[dest(lcand)], p[dest(onext(lcand))])) {
                    int next = onext(lcand);
                    erase_edge(lcand);
                    lcand = next;
                }
            }
            int rcand = oprev(base);
            if (right_of(dest(rcand), base)) {
                while (in_circle(p[dest(base)], p[org(base)], p[dest(rcand)], p[dest(oprev(rcand))])) {
                    int prev = oprev(rcand);
                    erase_edge(rcand);
                    rcand = prev;
                }
            }
            bool lv = right_of(dest(lcand), base), rv = right_of(dest(rcand), base);
            if (!lv && !rv) break;
            if (!lv || (rv && in_circle(p[dest(lcand)], p[org(lcand)], p[org(rcand)], p[dest(rcand)]))) base = connect(rcand, rev(base));
            else base = connect(rev(base), rev(lcand));
        }
        return {ldo, rdo};
    }
    vector<pair<int, int>> build(vector<point> points) {
        p = move(points), e.clear(), free_edge.clear();
        sort(p.begin(), p.end(), [](const point& a, const point& b) {
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        });
        int n = p.size();
        if (n >= 2) divide(0, n);
        vector<pair<int, int>> res;
        int m = e.size();
        for (int i = 0; i < m; i += 4) if (org(i) != -1) res.push_back({p[org(i)].id, p[dest(i)].id});
        return res;
    }
};

/**
 * 欧几里得 MST 优化建图
 * 给定 n 个点，求一颗在这些点构成的完全图上的 MST
 * 点编号 0-based；points[i] = {x, y}；返回带权无向邻接表 g
 *
 * 欧几里得 MST 一定包含于 Delaunay 三角剖分的边集中，因此只需保留 Delaunay 边。
 *
 * 复杂度：O(n log n)
 */
vector<vector<pair<int, long double>>> euclideanMSTGraph(vector<pair<int, int>>& points) {
    int n = points.size();
    vector<int> ord(n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) {
        if (points[i].first != points[j].first) return points[i].first < points[j].first;
        if (points[i].second != points[j].second) return points[i].second < points[j].second;
        return i < j;
    });
    vector<Delaunay::point> p;
    vector<pair<int, int>> edges;
    for (int k = 0; k < n; ++k) {
        int i = ord[k];
        if (k == 0 || points[i] != points[ord[k - 1]]) p.push_back({1.0L * points[i].first, 1.0L * points[i].second, i});
        else edges.push_back({p.back().id, i});
    }
    if (p.size() >= 2) {
        Delaunay d;
        auto de = d.build(p);
        edges.insert(edges.end(), de.begin(), de.end());
    }
    for (auto& [u, v] : edges) if (u > v) swap(u, v);
    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());
    vector<vector<pair<int, long double>>> g(n);
    for (auto& [u, v] : edges) {
        long double dx = points[u].first, dy = points[u].second;
        dx -= points[v].first, dy -= points[v].second;
        long double w = sqrtl(dx * dx + dy * dy);
        g[u].push_back({v, w}), g[v].push_back({u, w});
    }
    return g;
}
long double euclideanMST(vector<pair<int, int>>& points) {
    auto g = euclideanMSTGraph(points);
    return kruskalMST(g);
}