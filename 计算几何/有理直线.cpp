#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;

/** 整数点 */
struct Point {
    ll x, y;
    Point() : x(0), y(0) {}
    Point(ll _x, ll _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    Point &operator+=(const Point &o) { x += o.x, y += o.y; return *this; }
    Point &operator-=(const Point &o) { x -= o.x, y -= o.y; return *this; }
    bool operator==(const Point &o) const { return x == o.x && y == o.y; }
    bool operator<(const Point &o) const { if (x != o.x) return x < o.x; return y < o.y; }
};

/** 向量点积 */
ll dot(const Point &a, const Point &b) { return a.x * b.x + a.y * b.y; }

/** 向量叉积 a x b */
ll cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }

/** 叉积 (b - a) x (c - a) */
ll cross(const Point &a, const Point &b, const Point &c) { return cross(b - a, c - a); }

/** 有理数 n / d，自动约分，den<0 统一成正，den==0 视为无穷 */
struct Frac {
    ll num, den;
    Frac(ll n = 0, ll d = 1) {
        if (d < 0) n = -n, d = -d;
        if (d == 0) num = 1, den = 0;
        else {
            ll g = std::gcd(n >= 0 ? n : -n, d);
            num = n / g, den = d / g;
        }
    }
    bool isInf() const { return den == 0; }
    bool operator==(const Frac &o) const { return num == o.num && den == o.den; }
    bool operator<(const Frac &o) const {
        if (den == 0 || o.den == 0) return den > o.den;
        return (i128)num * o.den < (i128)o.num * den;
    }
};

/** 直线 Ax + By = C，(A,B,C) 归一化：A>0 或 A==0,B>0，整体约分 */
struct Line {
    ll A, B, C;
    Line() : A(0), B(0), C(0) {}
    Line(ll _A, ll _B, ll _C) : A(_A), B(_B), C(_C) { normalize(); }

    /** 将 (A,B,C) 归一化 */
    void normalize() {
        if (A == 0 && B == 0) { C = 0; return; }
        if (A < 0 || (A == 0 && B < 0)) A = -A, B = -B, C = -C;
        ll g = std::gcd(A >= 0 ? A : -A, B >= 0 ? B : -B);
        g = std::gcd(g, C >= 0 ? C : -C);
        if (g > 1) A /= g, B /= g, C /= g;
    }

    /** 从两点构造直线，p==q 则为非法直线 */
    static Line fromPoints(const Point &p, const Point &q) {
        ll dx = q.x - p.x, dy = q.y - p.y;
        if (dx == 0 && dy == 0) return Line(0,0,0);
        ll A = dy, B = -dx, C = A * p.x + B * p.y;
        Line L(A,B,C);
        return L;
    }

    /** 从点和方向向量构造直线，方向 v=(vx,vy) */
    static Line fromPointDir(const Point &p, const Point &v) {
        ll A = v.y, B = -v.x, C = A * p.x + B * p.y;
        Line L(A,B,C);
        return L;
    }

    /** 是否非法直线 */
    bool isInvalid() const { return A == 0 && B == 0; }

    /** 是否竖直线 x = 常数 */
    bool isVertical() const { return B == 0 && A != 0; }

    /** 是否水平线 y = 常数 */
    bool isHorizontal() const { return A == 0 && B != 0; }

    /** 方向向量 */
    Point dir() const { return Point(-B, A); }

    /** 法向量 */
    Point normal() const { return Point(A, B); }

    /** 与另一条直线是否平行 */
    bool parallel(const Line &o) const { return (i128)A * o.B == (i128)B * o.A; }

    /** 是否同一条直线 */
    bool same(const Line &o) const { return A == o.A && B == o.B && C == o.C; }

    bool operator==(const Line &o) const { return same(o); }

    /** n·p = A x + B y */
    ll eval(const Point &p) const { return A * p.x + B * p.y; }

    /** 斜率 k = -A/B，竖直线返回 ±∞ */
    Frac slope() const { if (isVertical()) return Frac(1,0); return Frac(-A, B); }

    /** y 截距 y = C/B，竖直线返回 ±∞ */
    Frac yIntercept() const { if (isVertical()) return Frac(1,0); return Frac(C, B); }

    /** x 截距 x = C/A，水平线返回 ±∞ */
    Frac xIntercept() const { if (isHorizontal()) return Frac(1,0); return Frac(C, A); }
};

/** 求两直线交点，返回是否相交（平行/重合返回 false），若相交 x,y 为有理数坐标 */
bool lineIntersection(const Line &l1, const Line &l2, Frac &x, Frac &y) {
    i128 A1 = l1.A, B1 = l1.B, C1 = l1.C, A2 = l2.A, B2 = l2.B, C2 = l2.C;
    i128 D = A1 * B2 - A2 * B1;
    if (D == 0) return false;
    i128 Dx = C1 * B2 - C2 * B1, Dy = A1 * C2 - A2 * C1;
    ll d = (ll)D, nx = (ll)Dx, ny = (ll)Dy;
    x = Frac(nx, d), y = Frac(ny, d);
    return true;
}

/** 点到直线的距离平方 dist^2 = (A x0 + B y0 - C)^2 / (A^2 + B^2) */
Frac pointLineDist2(const Point &p, const Line &l) {
    if (l.isInvalid()) return Frac(0,1);
    i128 val = (i128)l.A * p.x + (i128)l.B * p.y - (i128)l.C;
    i128 num = val * val, den = (i128)l.A * l.A + (i128)l.B * l.B;
    return Frac((ll)num, (ll)den);
}

/** 两直线距离平方，若平行且不同 dist^2 = (C1 - C2)^2 / (A^2 + B^2)，否则为 0 */
Frac lineLineDist2(const Line &l1, const Line &l2) {
    if (l1.isInvalid() || l2.isInvalid()) return Frac(0,1);
    if (!l1.parallel(l2) || l1.same(l2)) return Frac(0,1);
    i128 dc = (i128)l1.C - (i128)l2.C, num = dc * dc, den = (i128)l1.A * l1.A + (i128)l1.B * l1.B;
    return Frac((ll)num, (ll)den);
}
