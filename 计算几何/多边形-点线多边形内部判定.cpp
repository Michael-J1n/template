#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

const ld EPS = 1e-9;

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
    Point &operator+=(const Point &o) { x += o.x, y += o.y; return *this; }
    Point &operator-=(const Point &o) { x -= o.x, y -= o.y; return *this; }
    Point operator*(const ld v) const { return Point(x * v, y * v); }
    Point operator/(const ld v) const { return Point(x / v, y / v); }
    bool operator==(const Point &o) const {
        return fabsl(x - o.x) < EPS && fabsl(y - o.y) < EPS;
    }
    bool operator<(const Point &o) const {
        if (fabsl(x - o.x) > EPS) return x < o.x;
        return y < o.y - EPS;
    }
};
ld cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}
ld cross(const Point &a, const Point &b, const Point &c) {
    return cross(b - a, c - a);
}

/**
 * 判定点 p 是否在线段 ab 上（含端点）
 */
bool onSegment(const Point &a, const Point &b, const Point &p) {
    if (fabsl(cross(a, b, p)) > EPS) return false;
    ld minx = min(a.x, b.x), maxx = max(a.x, b.x);
    ld miny = min(a.y, b.y), maxy = max(a.y, b.y);
    if (p.x < minx - EPS || p.x > maxx + EPS) return false;
    if (p.y < miny - EPS || p.y > maxy + EPS) return false;
    return true;
}

/**
 * 点在三角形内判定（含边界）
 * 返回值： 在三角形外部 1 = 在三角形边界上（含顶点和边上) 2 = 在三角形严格内部
 * 前提：a, b, c 构成非退化三角形
 */
int pointInTriangle(const Point &a, const Point &b, const Point &c, const Point &p) {
    ld c1 = cross(a, b, p), c2 = cross(b, c, p), c3 = cross(c, a, p);
    int s1 = sgn(c1), s2 = sgn(c2), s3 = sgn(c3);
    if (s1 == 0 && onSegment(a, b, p) || s2 == 0 && onSegment(b, c, p) || s3 == 0 && onSegment(c, a, p)) return 1;
    if (s1 >= 0 && s2 >= 0 && s3 >= 0 || s1 <= 0 && s2 <= 0 && s3 <= 0) return 2;
    return 0;
}

/**
 * 点在简单多边形内判定（含边界）
 * 返回值：0 = 在多边形外部 1 = 在多边形边界上 2 = 在多边形严格内部
 * 前提：poly 为简单多边形顶点序列，按顺时针或逆时针给出，不包含重复的首尾点
 */
int pointInPolygon(const vector<Point> &poly, const Point &p) {
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        if (onSegment(poly[i], poly[(i + 1) % n], p)) return 1;
    }
    bool inside = false;
    for (int i = 0; i < n; ++i) {
        Point a = poly[i], b = poly[(i + 1) % n];
        bool up1 = a.y > p.y, up2 = b.y > p.y;
        if (up1 == up2) continue;
        ld x = (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x;
        if (x > p.x + EPS) inside = !inside;
    }
    return inside ? 2 : 0;
}

/**
 * 计算点 p 在线段 ab 上的参数 t，使得 p = a + t*(b-a)
 */
ld getParamOnSeg(const Point &a, const Point &b, const Point &p) {
    ld dx = b.x - a.x, dy = b.y - a.y;
    if (fabsl(dx) >= fabsl(dy)) {
        if (fabsl(dx) < EPS) return 0;
        return (p.x - a.x) / dx;
    } else {
        if (fabsl(dy) < EPS) return 0;
        return (p.y - a.y) / dy;
    }
}

/**
 * 判定两线段是否严格相交（交点在两段内部，排除共线和端点接触）
*/
bool properIntersect(const Point &a, const Point &b, const Point &c, const Point &d) {
    ld c1 = cross(a, b, c), c2 = cross(a, b, d), c3 = cross(c, d, a), c4 = cross(c, d, b);
    int s1 = sgn(c1), s2 = sgn(c2), s3 = sgn(c3), s4 = sgn(c4);
    return s1 * s2 < 0 && s3 * s4 < 0;
}

/**
 * 计算线段 ab 与 cd 严格相交时，交点在 ab 上的参数 t，假定两线段严格相交且不平行
 */
ld intersectParam(const Point &a, const Point &b, const Point &c, const Point &d) {
    ld vx = b.x - a.x, vy = b.y - a.y, wx = d.x - c.x, wy = d.y - c.y;
    ld sx = c.x - a.x, sy = c.y - a.y;
    ld den = vx * wy - vy * wx, num = sx * wy - sy * wx;
    return num / den;
}

/**
 * 线段是否完全在简单多边形内（含边界、允许与边重合）
 * 前提： poly 为简单多边形顶点序列，按顺/逆时针给出，不包含重复首尾点
 */
bool segmentInPolygon(const vector<Point> &poly, const Point &a, const Point &b) {
    if (a == b) return pointInPolygon(poly, a) != 0;
    int t1 = pointInPolygon(poly, a), t2 = pointInPolygon(poly, b);
    if (t1 == 0 || t2 == 0) return false;

    vector<ld> ts;
    ts.push_back(0), ts.push_back(1);
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        if (onSegment(a, b, poly[i])) {
            ld t = getParamOnSeg(a, b, poly[i]);
            if (t >= -EPS && t <= 1 + EPS) ts.push_back(t);
        }
    }
    for (int i = 0; i < n; ++i) {
        Point c = poly[i], d = poly[(i + 1) % n];
        if (!properIntersect(a, b, c, d)) continue;
        ld t = intersectParam(a, b, c, d);
        if (t > EPS && t < 1 - EPS) ts.push_back(t);
    }

    sort(ts.begin(), ts.end());
    vector<ld> uniq;
    for (int i = 0; i < ts.size(); ++i) {
        if (i == 0 || fabsl(ts[i] - ts[i - 1]) > (ld)1e-10) uniq.push_back(ts[i]);
    }
    for (int i = 0; i + 1 < uniq.size(); ++i) {
        ld L = uniq[i], R = uniq[i + 1];
        ld mid = (L + R) * (ld)0.5;
        Point m(a.x + (b.x - a.x) * mid, a.y + (b.y - a.y) * mid);
        int t = pointInPolygon(poly, m);
        if (t == 0) return false;
    }
    return true;
}
