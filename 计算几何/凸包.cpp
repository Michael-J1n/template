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

/**
 * 返回点集 p 的凸包 ch，保证凸包中的点按逆时针排序
 * 
 * 核心算法：Andrew 算法；按照 x 坐标排序，使用单调栈维护凸包的下边界和上边界。关键是 cross>0 代表左转，cross<0 代表右转
 * 下文中 a=stk[top-1], b=stk[top], c=pts[i].
 * 下凸包：从左到右扫描，维护一条折线，使得所有已处理的点都在折线的上方或边界上；折线始终保持向左转，即相邻两条边的 cross > 0。
 *         对于栈中连续两点 a, b 与新点 c，若：
 *           cross > 0：左转，符合要求，保留 b
 *           cross <= 0：右转或共线，b 在 ac 上方或之间，弹出 b
 * 上凸包：从右到左扫描，维护一条折线，使得所有已处理的点都在折线的下方或边界上；折线始终保持向左转，即相邻两条边的 cross > 0。
 *         对于栈中连续两点 a, b 与新点 c，若：
 *           cross > 0：左转，符合要求，保留 b
 *           cross <= 0：右转或共线，b 在 ac 下方或之间，弹出 b
 * 
 * 复杂度：O(n log n)，瓶颈在于排序
 */
vector<Point> convexHull(vector<Point>& p) {
    sort(p.begin(), p.end()); p.erase(unique(p.begin(), p.end()), p.end());
    int n = p.size();
    if (n <= 1) return p;
    vector<Point> s; s.resize(n * 2);
    // 求下凸包
    int top = -1;
    for (int i = 0; i < n; ++i) {
        while (top > 0 && cross(s[top - 1], s[top], p[i]) <= 0) --top;
        s[++top] = p[i];
    }
    // 求上凸包
    int t = top;
    for (int i = n - 2; ~i; --i) {
        while (top > t && cross(s[top - 1], s[top], p[i]) <= 0) --top;
        s[++top] = p[i];
    }
    --top;
    s.resize(top + 1); return s;
}

/** 给定凸包，返回凸包周长 */
long long dis2(const Point& a, const Point& b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); }
long double dis(const Point& a, const Point& b) { return sqrtl(dis(a, b)); }
long double chP(vector<Point>& ch) {
    long double res = 0;
    int n = ch.size();
    for (int i = 0; i < n; ++i) res += dis(ch[i], ch[(i + 1) % n]);
    return res;
}
/** 给定凸包，返回凸包面积 */
long double triangleArea(const Point& a, const Point& b, const Point& c) { return fabsl((long double)cross(a, b, c)) / 2.0; }
long double chS(vector<Point>& ch) {
    long double res = 0;
    int n = ch.size();
    for (int i = 1; i + 1 < n; ++i) res += triangleArea(ch[0], ch[i], ch[i + 1]);
    return res;
}