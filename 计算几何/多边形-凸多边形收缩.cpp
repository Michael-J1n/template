/** 在许多题目中要找到在多边形内嵌入的圆，圆心最外层的可行轨迹即为多边形向内收缩 R 的结果 */

#include <bits/stdc++.h>
using namespace std;
#define eb emplace_back

using ll = long long;
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
ld len(const Point& a) { return hypotl(a.x, a.y); }

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

vector<Point> halfplaneIntersection(vector<Line>& ls) {
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
    vector<Point> res;
    for (int i = h; i <= t; ++i) res.eb(dq[i].a);
    return res;
}

/** shiftInside: 任意 simple polygon poly 向内距离 R，返回圆心可行域（所有距边界>=R且在多边形内的点集）
 * 结果为凸多边形（CCW），可能为空；
 * 前提：poly 为简单**凸**多边形
 * 复杂度 O(nlogn)
 * */
vector<Point> shiftInside(vector<Point> poly, ld R) {
    int n = poly.size();
    if (n < 3) return {};
    if (polygonArea(poly) < 0) reverse(poly.begin(), poly.end());

    vector<Line> H;
    H.reserve(n);
    for (int i = 0; i < n; ++i) {
        Point a = poly[i], b = poly[i + 1 < n ? i + 1 : 0];
        Point v = b - a;
        ld L = len(v);
        if (L < EPS) continue;
        Point nrm(-v.y / L, v.x / L);
        Point p1 = a + nrm * R, p2 = b + nrm * R;
        H.eb(Line(p1, p2 - p1));
    }
    if (H.size() < 3) return {};
    return halfplaneIntersection(H);
}

vector<Point> shiftOutside(vector<Point> poly, ld R) {
    int n = poly.size();
    if (n < 3) return {};
    if (polygonArea(poly) < 0) reverse(poly.begin(), poly.end());

    vector<Line> H;
    H.reserve(n);
    for (int i = 0; i < n; ++i) {
        Point a = poly[i], b = poly[i + 1 < n ? i + 1 : 0];
        Point v = b - a;
        ld L = len(v);
        if (L < EPS) continue;
        Point nrm(v.y / L, -v.x / L);
        Point p1 = a + nrm * R, p2 = b + nrm * R;
        H.eb(Line(p1, p2 - p1));
    }
    if (H.size() < 3) return {};
    return halfplaneIntersection(H);
}