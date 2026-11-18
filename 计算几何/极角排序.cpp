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
};
long long cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
long long cross(const Point& a, const Point& b, const Point& c) { return cross(b - a, c - a); }
long long norm2(const Point &a) { return a.x * a.x + a.y * a.y; }

int half(const Point& p) {
    if (p.y > 0) return 0;
    if (p.y < 0) return 1;
    return p.x >= 0 ? 0 : 1;
}
/** 围绕原点的极角排序比较器（逆时针）*/
bool polar_cmp(const Point &a, const Point &b) {
    int ha = half(a), hb = half(b);
    if (ha != hb) return ha < hb;
    long long c = cross(a, b);
    if (c != 0) return c > 0;
    return norm2(a) < norm2(b);
}
/** 对点集 pts 以原点为极点按极角逆时针排序（原地排序） */
void polar_sort(vector<Point>& pts) { sort(pts.begin(), pts.end(), polar_cmp); }

/** 
 * 固定点 s，将剩余点按照相对于 s 的极角进行排序，即右下方向右上方的逆时针顺序！
 * 前提条件：所有点正常按照 Point 的 < sort 过，并且 pts 中点只出现在 s 的右半平面（含正上方）
 * 性质：i<j ==> cross(s,v[i],v[j])>=0，即满足凸链
 */
void polar_sort(Point s, vector<Point>& pts) {
    sort(pts.begin(), pts.end(), [&](const Point& p1, const Point& p2) {
        long long c = cross(s, p1, p2);
        if (c) return c > 0;
        if (p1.x != p2.x) return p1.x < p2.x;
        return p1.y < p2.y;
    });
}