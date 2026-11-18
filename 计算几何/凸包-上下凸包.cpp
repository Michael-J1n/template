#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    Point() : x(0), y(0) {}
    Point(long long _x, long long _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    bool operator<(const Point &o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
};
long long cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
long long cross(const Point &a, const Point &b, const Point &c) { return cross(b - a, c - a); }
/** 返回上凸包 */
vector<Point> upperHull(vector<Point> p) {
    sort(p.begin(), p.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y > b.y;
    });
    int n = p.size();
    vector<Point> s; s.resize(n);
    int top = -1;
    for (int i = 0; i < n; ++i) {
        if (i > 0 && p[i].x == p[i - 1].x) continue;
        while (top > 0 && cross(s[top - 1], s[top], p[i]) >= 0) --top;
        s[++top] = p[i];
    }
    s.resize(top + 1); return s;
}
/** 返回下凸包 */
vector<Point> lowerHull(vector<Point> p) {
    sort(p.begin(), p.end());
    int n = p.size();
    vector<Point> s; s.resize(n);
    int top = -1;
    for (int i = 0; i < n; ++i) {
        if (i > 0 && p[i].x == p[i - 1].x) continue;
        while (top > 0 && cross(s[top - 1], s[top], p[i]) <= 0) --top;
        s[++top] = p[i];
    }
    s.resize(top + 1); return s;
}
