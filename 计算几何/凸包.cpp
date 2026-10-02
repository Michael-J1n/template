#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
struct Point {
    ll x, y;
    Point() : x(0), y(0) {}
    Point(ll _x, ll _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    bool operator<(const Point &o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
    bool operator==(const Point &o) const { return x == o.x && y == o.y; }
};
ld dis(const Point& a, const Point& b) {
    ll dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}
ll cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}
ll cross(const Point &a, const Point &b, const Point &c) {
    return cross(b - a, c - a);
}
ld triangleArea(const Point& a, const Point& b, const Point& c) {
    return fabsl((ld)cross(a, b, c)) / 2.0;
}
/**
 * 返回点集 p 的凸包 ch，保证凸包中的点按逆时针排序
 */
vector<Point> convexHull(vector<Point>& p) {
    sort(p.begin(), p.end()); p.erase(unique(p.begin(), p.end()), p.end());
    int n = p.size();
    if (n <= 1) return p;
    vector<Point> s; s.resize(n * 2);
    int top = -1;
    for (int i = 0; i < n; ++i) {  // 下凸包
        while (top > 0 && cross(s[top - 1], s[top], p[i]) <= 0) --top;
        s[++top] = p[i];
    }
    int t = top;
    for (int i = n - 2; ~i; --i) {  // 上凸包
        while (top > t && cross(s[top - 1], s[top], p[i]) <= 0) --top;
        s[++top] = p[i];
    }
    --top;
    s.resize(top + 1); return s;
}
/** 给定凸包，返回凸包周长 */
ld chP(vector<Point>& ch) {
    ld res = 0;
    int n = ch.size();
    for (int i = 0; i < n; ++i) res += dis(ch[i], ch[(i + 1) % n]);
    return res;
}
/** 给定凸包，返回凸包面积 */
ld chS(vector<Point>& ch) {
    ld res = 0;
    int n = ch.size();
    for (int i = 1; i + 1 < n; ++i) res += triangleArea(ch[0], ch[i], ch[i + 1]);
    return res;
}