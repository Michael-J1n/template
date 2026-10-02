#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    using ll = long long;
    ll x, y;
    Point() : x(0), y(0) {}
    Point(ll _x, ll _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
};

/** @brief 叉积 */
ll cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

/** @brief 向量长度平方 */
ll norm2(const Point &a) {
    return a.x * a.x + a.y * a.y;
}

/** @brief 极角排序用半平面分类 */
int half(const Point &p) {
    if (p.y > 0) return 0;
    if (p.y < 0) return 1;
    return p.x >= 0 ? 0 : 1;
}

/** @brief 围绕原点的极角排序比较器（逆时针）*/
bool polarCmpOriginCCW(const Point &a, const Point &b) {
    int ha = half(a), hb = half(b);
    if (ha != hb) return ha < hb;
    ll c = cross(a, b);
    if (c != 0) return c > 0;
    return norm2(a) < norm2(b);
}

/** @brief 对点集 pts 以原点为极点按极角逆时针排序（原地排序） */
void polarSortOrigin(vector<Point> &pts) {
    sort(pts.begin(), pts.end(), polarCmpOriginCCW);
}
