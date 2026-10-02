#include <bits/stdc++.h>
using namespace std;
#define eb emplace_back

using ll = long long;
using ld = long double;

const ld EPS = 1e-12;

int sgn(ld x) {
    if (x < -EPS) return -1;
    if (x > EPS) return 1;
    return 0;
}

struct Point {
    ld x, y;
    Point() : x(0), y(0) {}
    Point(ld _x, ld _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator*(ld k) const { return Point(x * k, y * k); }
    Point operator/(ld k) const { return Point(x / k, y / k); }
};
ld dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}
ld cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}
ld cross(const Point &a, const Point &b, const Point &c) {
    return cross(b - a, c - a);
}
ld len(const Point &a) {
    return sqrtl(dot(a, a));
}

ld polygonArea(const vector<Point> &p) {
    int n = p.size();
    ld s = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1 < n ? i + 1 : 0);
        s += cross(p[i], p[j]);
    }
    return s / 2;
}

bool onSegment(const Point &a, const Point &b, const Point &p) {
    if (fabsl(cross(a, b, p)) > 1e-10) return false;
    if (p.x < min(a.x, b.x) - 1e-10 || p.x > max(a.x, b.x) + 1e-10) return false;
    if (p.y < min(a.y, b.y) - 1e-10 || p.y > max(a.y, b.y) + 1e-10) return false;
    return true;
}

/** 点在简单多边形内（含边界），poly 必须是简单多边形，顺/逆时针都可 */
bool inPolygon(const vector<Point> &poly, const Point &q) {
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if (onSegment(a, b, q)) return true;
    }
    bool in = false;
    for (int i = 0; i < n; ++i) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if (a.y > b.y) swap(a, b);
        if (q.y <= a.y + 1e-18 || q.y > b.y + 1e-18) continue;
        ld x = a.x + (b.x - a.x) * (q.y - a.y) / (b.y - a.y);
        if (x > q.x) in = !in;
    }
    return in;
}

ld distPointSeg(const Point &p, const Point &a, const Point &b) {
    Point ab = b - a;
    ld vv = dot(ab, ab);
    if (vv < EPS) return len(p - a);
    ld t = dot(p - a, ab) / vv;
    if (t < 0) return len(p - a);
    if (t > 1) return len(p - b);
    Point proj = a + ab * t;
    return len(p - proj);
}

/** 直线：点 p + 方向 v（无限长） */
struct Line {
    Point p, v;
    Line() {}
    Line(const Point &_p, const Point &_v) : p(_p), v(_v) {}
};

/** 两直线交点（不平行）。返回 (ok, inter) */
pair<int, Point> lineInter(const Line &a, const Line &b) {
    ld d = cross(a.v, b.v);
    if (fabsl(d) < 1e-18) return {0, Point()};
    Point u = a.p - b.p;
    ld t = cross(u, b.v) / d;
    return {1, a.p + a.v * t};
}

/** CCW 多边形的一条边 a->b 的“内移 R 平行线” */
Line shiftEdgeInsideCCW(const Point &a, const Point &b, ld R) {
    Point v = b - a;
    ld L = len(v);
    Point nrm(-v.y / L, v.x / L);      // 左法向
    Point p1 = a + nrm * R;
    return Line(p1, v);
}

/**
 * 任意 simple polygon 的 erosion 候选点（只取平移边交点）：
 * - poly 可以非凸
 * - 返回所有相邻内移边的交点，且通过：
 *   1) 在原多边形内
 *   2) 到所有边距离 >= R - eps
 * 复杂度 O(n^2)
 */
vector<Point> erosionShiftVertices(vector<Point> poly, ld R) {
    int n = poly.size();
    if (n < 3) return {};
    if (polygonArea(poly) < 0) reverse(poly.begin(), poly.end()); // CCW
    vector<Line> L(n);
    for (int i = 0; i < n; ++i) {
        Point a = poly[i], b = poly[(i + 1) % n];
        L[i] = shiftEdgeInsideCCW(a, b, R);
    }

    vector<Point> res;
    res.reserve(n);
    for (int i = 0; i < n; ++i) {
        Line l1 = L[(i - 1 + n) % n], l2 = L[i];

        auto it = lineInter(l1, l2);
        if (!it.first) continue;
        Point q = it.second;
        if (!inPolygon(poly, q)) continue;
        bool ok = true;
        for (int j = 0; j < n; ++j) {
            Point a = poly[j], b = poly[(j + 1) % n];
            if (distPointSeg(q, a, b) + 1e-9 < R) { ok = false; break; }
        }
        if (!ok) continue;
        res.eb(q);
    }

    vector<Point> out;
    for (auto &p : res) {
        bool dup = false;
        for (auto &q : out) {
            if (len(p - q) < 1e-7) { dup = true; break; }
        }
        if (!dup) out.eb(p);
    }
    return out;
}
