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

int half(const Point& p) {
    if (p.y > 0) return 0;
    if (p.y < 0) return 1;
    return p.x >= 0 ? 0 : 1;
}
// 构造直线 l(a,b)：沿着 a->b 走，要保留的区域必须在左手边
struct Line {
    Point a, b, dir;
    Line() : a(Point()), b(Point()) {}
    Line(const Point& _a, const Point& _b) : a(_a), b(_b), dir(b - a) {}
    bool operator<(const Line& l) const {
        int ha = half(dir), hb = half(l.dir);
        if (ha != hb) return ha < hb;
        long long c = cross(dir, l.dir);
        if (c != 0) return c > 0;
        return cross(dir, l.a - a) < 0;
    }
};
bool on_right(const Line& a, const Line& b, const Line& l) {
    long long d = cross(a.dir, b.dir);
    long long s = cross(l.dir, a.a - l.a) * d + cross(b.a - a.a, b.dir) * cross(l.dir, a.dir);
    return d > 0 ? s < 0 : s > 0;
}

/**
 * 半平面交
 * 返回有界闭半平面交的边界直线，按极角逆时针排序
 *
 * 交点为 intersect_point(res[i], res[(i+1)%res.size()])，使用 FPoint
 * 
 * 复杂度：O(n log n)
 */
vector<Line> half_plane_intersect(vector<Line>& ls) {
    sort(ls.begin(), ls.end());
    int n = ls.size(), h = 0, t = -1;
    vector<Line> dq; dq.resize(n);
    for (int i = 0; i < n; ++i) {
        if (i > 0 && half(ls[i].dir) == half(ls[i - 1].dir) && cross(ls[i].dir, ls[i - 1].dir) == 0) continue;
        while (h < t && on_right(dq[t - 1], dq[t], ls[i])) --t;
        while (h < t && on_right(dq[h], dq[h + 1], ls[i])) ++h;
        if (h <= t && cross(dq[t].dir, ls[i].dir) == 0) return {};
        dq[++t] = ls[i];
    }
    while (h < t && on_right(dq[t - 1], dq[t], dq[h])) --t;
    while (h < t && on_right(dq[h], dq[h + 1], dq[t])) ++h;
    if (t - h + 1 < 3) return {};
    for (int i = h; i <= t; ++i) {
        if (cross(dq[i].dir, dq[i == t ? h : i + 1].dir) <= 0) return {};
    }
    return vector<Line>(dq.begin() + h, dq.begin() + t + 1);
}

// 附：求交点
struct FPoint { long double x, y; };
FPoint intersect_point(const Line& m, const Line& n) {
    long double t = (long double)cross(n.a - m.a, n.dir) / cross(m.dir, n.dir);
    return {(long double)m.a.x + m.dir.x * t, (long double)m.a.y + m.dir.y * t};
}