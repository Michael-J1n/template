#include <bits/stdc++.h>
using namespace std;
using ld = long double;

struct Point {
    ld x, y;
    Point() : x(0), y(0) {}
    Point(ld _x, ld _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
};

ld cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
ld cross(const Point &a, const Point &b, const Point &c) { return cross(b - a, c - a); }

/**
 * 返回上凸包
 */
vector<Point> upperHull(vector<Point> p) {
    sort(p.begin(), p.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y > b.y;
    });
    vector<Point> q;
    for (auto& pt : p) {
        if (!q.empty() && pt.x == q.back().x) continue;
        q.push_back(pt);
    }

    vector<Point> h;
    for (auto& pt : q) {
        while (h.size() >= 2 && cross(h[h.size() - 2], h.back(), pt) >= 0) h.pop_back();
        h.push_back(pt);
    }
    return h;
}

/**
 * 返回下凸包
 */
vector<Point> lowerHull(vector<Point> p) {
    sort(p.begin(), p.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });
    vector<Point> q;
    for (auto& pt : p) {
        if (!q.empty() && pt.x == q.back().x) continue;
        q.push_back(pt);
    }

    vector<Point> h;
    for (auto& pt : q) {
        while (h.size() >= 2 && cross(h[h.size() - 2], h.back(), pt) <= 0) h.pop_back();
        h.push_back(pt);
    }
    return h;
}