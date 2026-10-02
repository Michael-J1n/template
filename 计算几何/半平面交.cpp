#include <bits/stdc++.h>
using namespace std;
#define eb emplace_back

using ld = long double;
const ld EPS = 1e-12;

struct Point {
    ld x, y;
    Point() : x(0), y(0) {}
    Point(ld _x, ld _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator*(ld k) const { return Point(x * k, y * k); }
};
ld cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}
ld cross(const Point &a, const Point &b, const Point &c) {
    return cross(b - a, c - a);
}

struct Line {
    Point a, b, dir;
    ld ang;
    Line() : a(Point()), b(Point()) {}
    Line(const Point& _a, const Point& _b) : a(_a), b(_b) {
        ang = atan2l((b.y - a.y), (b.x - a.x));
        dir = b - a;
    }
    bool operator<(const Line& l) const {
        if (fabsl(ang - l.ang) > EPS) return ang < l.ang;
        return cross(dir, l.b - a) < 0;
    }
};
Point intersectPoint(const Line& m, const Line& n) {
    Point u = m.a - n.a;
    ld t = cross(u, n.dir) / cross(n.dir, m.dir);
    return m.a + m.dir * t;
}
bool onRight(const Point& p, const Line& l) {
    return cross(l.a, l.b, p) <= 0;
}

ld polygonArea(vector<Point>& ch) {
    ld res = 0;
    int n = ch.size();
    for (int i = 1; i < n; ++i) res += fabs(cross(ch[0], ch[i], ch[(i + 1) % n]));
    return res / 2;
}

vector<Line> halfPlaneIntersect(vector<Line>& ls) {
    sort(ls.begin(), ls.end());
    int n = ls.size(), h = 0, t = 0;
    vector<Line> dq; dq.resize(n);
    dq[0] = ls[0];
    for (int i = 1; i < n; ++i) {
        if ((ls[i].ang - ls[i - 1].ang) < EPS) continue;
        while (h < t && onRight(intersectPoint(dq[t], dq[t - 1]), ls[i])) --t;
        while (h < t && onRight(intersectPoint(dq[h], dq[h + 1]), ls[i])) ++h;
        dq[++t] = ls[i];
    }
    while (h < t && onRight(intersectPoint(dq[t], dq[t - 1]), dq[h])) --t; 
    vector<Line> res; res.reserve(dq.size());
    for (int i = h; i <= t; ++i) res.eb(dq[i]);
    return res;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<Line> ls; ls.reserve(n * 50);
    for (int i = 0; i < n; ++i) {
        int m; cin >> m;
        Point prev, cur, init;
        for (int i = 0; i < m; ++i) {
            ld x, y; cin >> x >> y, cur = {x, y};
            if (i > 0) ls.eb(prev, cur);
            else init = cur;
            prev = {x, y};
        }
        ls.eb(cur, init);
    }
    auto res = halfPlaneIntersect(ls);
    int sz = res.size();
    vector<Point> ch(sz);
    for (int i = 0; i < sz; ++i) ch[i] = intersectPoint(res[i], res[(i + 1) % sz]);
    auto area = polygonArea(ch);
    cout << fixed << setprecision(3) << area << '\n';
    return 0;
}