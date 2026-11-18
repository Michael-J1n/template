#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    Point() : x(0), y(0) {}
    Point(long long _x, long long _y) : x(_x), y(_y) {}
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    bool operator<(const Point &o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
    bool operator==(const Point &o) const { return x == o.x && y == o.y; }
};
long long cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
long long cross(const Point &a, const Point &b, const Point &c) { return cross(b - a, c - a); }
vector<Point> convexHull(vector<Point>& p) {
    sort(p.begin(), p.end()); p.erase(unique(p.begin(), p.end()), p.end());
    int n = p.size();
    if (n <= 1) return p;
    vector<Point> s; s.resize(n * 2);
    int top = -1;
    for (int i = 0; i < n; ++i) {
        while (top > 0 && cross(s[top - 1], s[top], p[i]) <= 0) --top;
        s[++top] = p[i];
    }
    int t = top;
    for (int i = n - 2; ~i; --i) {
        while (top > t && cross(s[top - 1], s[top], p[i]) <= 0) --top;
        s[++top] = p[i];
    }
    --top;
    s.resize(top + 1); return s;
}

struct FPoint { long double x, y; };
int half(const Point& p) {
    if (p.y > 0) return 0;
    if (p.y < 0) return 1;
    return p.x >= 0 ? 0 : 1;
}
// 沿着 a->b 走，保留左侧半平面，再将直线向左平移 r
struct Line {
    FPoint a;
    Point dir;
    Line() : a{0, 0}, dir(Point()) {}
    Line(const Point& _a, const Point& _b, long double r) : a{(long double)_a.x, (long double)_a.y}, dir(_b - _a) {
        long double len = hypotl(dir.x, dir.y);
        a.x -= dir.y * r / len; a.y += dir.x * r / len;
    }
    bool operator<(const Line& l) const {
        int ha = half(dir), hb = half(l.dir);
        if (ha != hb) return ha < hb;
        long long c = cross(dir, l.dir);
        if (c != 0) return c > 0;
        return dir.x * (l.a.y - a.y) - dir.y * (l.a.x - a.x) < 0;
    }
};
FPoint intersect_point(const Line& m, const Line& n) {
    long double t = ((n.a.x - m.a.x) * n.dir.y - (n.a.y - m.a.y) * n.dir.x) / cross(m.dir, n.dir);
    return {m.a.x + m.dir.x * t, m.a.y + m.dir.y * t};
}
bool on_right(const FPoint& p, const Line& l) { return l.dir.x * (p.y - l.a.y) - l.dir.y * (p.x - l.a.x) <= 0; }

/** 返回半平面交的边界直线，按极角逆时针排序；无正面积交集时返回空 */
vector<Line> half_plane_intersect(vector<Line>& ls) {
    sort(ls.begin(), ls.end());
    int n = ls.size(), h = 0, t = -1;
    vector<Line> dq; dq.resize(n);
    for (int i = 0; i < n; ++i) {
        if (i > 0 && half(ls[i].dir) == half(ls[i - 1].dir) && cross(ls[i].dir, ls[i - 1].dir) == 0) continue;
        while (h < t && on_right(intersect_point(dq[t - 1], dq[t]), ls[i])) --t;
        while (h < t && on_right(intersect_point(dq[h], dq[h + 1]), ls[i])) ++h;
        if (h <= t && cross(dq[t].dir, ls[i].dir) == 0) return {};
        dq[++t] = ls[i];
    }
    while (h < t && on_right(intersect_point(dq[t - 1], dq[t]), dq[h])) --t;
    while (h < t && on_right(intersect_point(dq[h], dq[h + 1]), dq[t])) ++h;
    if (t - h + 1 < 3) return {};
    for (int i = h; i <= t; ++i) {
        if (cross(dq[i].dir, dq[i == t ? h : i + 1].dir) <= 0) return {};
    }
    return vector<Line>(dq.begin() + h, dq.begin() + t + 1);
}

/**
 * 返回点集 pts 的凸包的最大内接圆 {圆心, 半径}
 * 
 * 二分半径，将凸包各边向内平移后求半平面交为固定半径时圆心的可行域
 * 
 * 复杂度：O(n log n)
 */
pair<FPoint, long double> maximumInscribedCircle(vector<Point>& pts) {
    vector<Point> ch = convexHull(pts);
    int n = ch.size();
    if (n == 0) return {{0, 0}, 0};
    FPoint o{(long double)ch[0].x, (long double)ch[0].y};
    if (n < 3) return {o, 0};
    Point origin = ch[0];
    for (Point& p : ch) p = p - origin;
    o = {0, 0};
    Point low = ch[0], high = ch[0];
    for (const Point& p : ch) {
        low.x = min(low.x, p.x); low.y = min(low.y, p.y);
        high.x = max(high.x, p.x); high.y = max(high.y, p.y);
    }
    long double l = 0, r = min((long double)high.x - low.x, (long double)high.y - low.y) / 2;
    for (int it = 0; it < 80; ++it) {
        long double mid = (l + r) / 2;
        vector<Line> ls; ls.reserve(n);
        for (int i = 0; i < n; ++i) ls.emplace_back(ch[i], ch[(i + 1) % n], mid);
        vector<Line> res = half_plane_intersect(ls);
        if (res.empty()) { r = mid; continue; }
        l = mid; o = {0, 0};
        int m = res.size();
        for (int i = 0; i < m; ++i) {
            FPoint p = intersect_point(res[i], res[(i + 1) % m]);
            o.x += p.x / m; o.y += p.y / m;
        }
    }
    return {{o.x + origin.x, o.y + origin.y}, l};
}
