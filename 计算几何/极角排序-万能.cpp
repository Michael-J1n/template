#include <bits/stdc++.h>
using namespace std;

using db = long double;
const db EPS = 1e-9;

/** @brief 返回带容差的符号：x < -EPS 返回 -1，|x| <= EPS 返回 0，x > EPS 返回 1 */
int sgn(db x) {
    if (x < -EPS) return -1;
    if (x > EPS) return 1;
    return 0;
}

/** @brief 平面点 / 向量，坐标为 double */
struct Point {
    db x, y;

    Point() : x(0), y(0) {}
    Point(db _x, db _y) : x(_x), y(_y) {}

    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator*(db k) const { return Point(x * k, y * k); }
    Point operator/(db k) const { return Point(x / k, y / k); }

    Point &operator+=(const Point &o) { x += o.x, y += o.y; return *this; }
    Point &operator-=(const Point &o) { x -= o.x, y -= o.y; return *this; }
    Point &operator*=(db k) { x *= k, y *= k; return *this; }
    Point &operator/=(db k) { x /= k; y /= k; return *this; }
};

/** @brief 点积：a·b */
db dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

/** @brief 叉积：a×b = a.x*b.y - a.y*b.x（标量） */
db cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

/** @brief 向量长度平方 |a|^2，避免频繁 sqrt */
db norm2(const Point &a) {
    return dot(a, a);
}

/** @brief 极角排序用半平面分类函数
 *  返回 0: 上半平面（y > 0 或 y≈0 且 x >= 0）
 *  返回 1: 下半平面（y < 0 或 y≈0 且 x < 0）
 *  极角排序时先按 half 升序，再按叉积保证极角单调
 */
int half(const Point &p) {
    int sy = sgn(p.y);
    if (sy > 0) return 0;
    if (sy < 0) return 1;
    return sgn(p.x) >= 0 ? 0 : 1;
}

/** @brief 以原点为极点的极角排序比较器（逆时针）
 *  排序顺序：
 *    1. half(a) 从小到大（上半平面在前）
 *    2. 同一半平面内，cross(a,b) > 0 则 a 在 b 前（逆时针）
 *    3. 若极角相同（共线），按距离从近到远排序
 */
bool polarCmpOriginCCW(const Point &a, const Point &b) {
    int ha = half(a), hb = half(b);
    if (ha != hb) return ha < hb;
    int c = sgn(cross(a, b));
    if (c != 0) return c > 0;
    return norm2(a) < norm2(b);
}

/** @brief 以给定点 o 为极点的极角排序比较器（逆时针）
 *  将 a,b 平移到以 o 为原点的向量，再套用 half+cross 规则。
 */
bool polarCmpAroundCCW(const Point &a, const Point &b, const Point &o) {
    Point A = a - o, B = b - o;
    int ha = half(A), hb = half(B);
    if (ha != hb) return ha < hb;
    int c = sgn(cross(A, B));
    if (c != 0) return c > 0;
    return norm2(A) < norm2(B);
}

/** @brief 以原点为极点对点集做极角排序
 *  @param pts     点集（会被原地排序）
 *  @param clockwise 为 false 时按逆时针排序；为 true 时按顺时针排序
 *  说明：
 *    1. 逆时针：从 x 轴正方向开始，沿逆时针方向极角单调。
 *    2. 顺时针：在逆时针结果的基础上整体反转，起点方向可能不同，
 *       但点的相对顺序满足顺时针极角单调。
 */
void polarSortOrigin(vector<Point> &pts, bool clockwise = false) {
    sort(pts.begin(), pts.end(), polarCmpOriginCCW);
    if (clockwise) reverse(pts.begin(), pts.end());
}

/** @brief 以给定点 o 为极点对点集做极角排序
 *  @param pts     点集（会被原地排序）
 *  @param o       极点（基准点）
 *  @param clockwise 为 false 时按逆时针排序；为 true 时按顺时针排序
 *  说明：
 *    1. 排序依据是向量 (p - o) 的极角。
 *    2. 顺逆时针规则与 polarSortOrigin 相同。
 */
void polarSortAround(vector<Point> &pts, const Point &o, bool clockwise = false) {
    sort(pts.begin(), pts.end(), [&](const Point &a, const Point &b) {
        return polarCmpAroundCCW(a, b, o);
    });
    if (clockwise) reverse(pts.begin(), pts.end());
}
