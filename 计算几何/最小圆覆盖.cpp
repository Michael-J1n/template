#include <bits/stdc++.h>
using namespace std;

using ld = long double;

struct Point {
    ld x, y;
};

struct Circle {
    Point o;
    ld r;
};

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

ld dis(Point a, Point b) {
    return hypot(a.x - b.x, a.y - b.y);
}

bool inCircle(Point p, Circle c) {
    return dis(p, c.o) <= c.r + 1e-12;
}

Point getMid(Point a, Point b) {
    return {(a.x + b.x) / 2, (a.y + b.y) / 2};
}

Point getCenter(Point a, Point b, Point c) {
    ld A = b.x - a.x, B = b.y - a.y;
    ld C = c.x - a.x, D = c.y - a.y;
    ld E = A * (a.x + b.x) + B * (a.y + b.y);
    ld F = C * (a.x + c.x) + D * (a.y + c.y);
    ld G = 2 * (A * (c.y - b.y) - B * (c.x - b.x));
    return {(D * E - B * F) / G, (A * F - C * E) / G};
}

Circle minimumEnclosingCircle(vector<Point> p) {
    shuffle(p.begin(), p.end(), rng);
    Circle c{{0, 0}, -1};
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        if (c.r >= 0 && inCircle(p[i], c)) continue;
        c = {p[i], 0};
        for (int j = 0; j < i; ++j) {
            if (inCircle(p[j], c)) continue;
            c = {getMid(p[i], p[j]), dis(p[i], p[j]) / 2};
            for (int k = 0; k < j; ++k) {
                if (inCircle(p[k], c)) continue;
                Point o = getCenter(p[i], p[j], p[k]);
                c = {o, dis(o, p[i])};
            }
        }
    }
    return c;
}