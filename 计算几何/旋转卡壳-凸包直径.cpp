#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
};

ll cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

ll cross(const Point &a, const Point &b, const Point &c) {
    return cross(b - a, c - a);
}

ll dis2(const Point& a, const Point& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

vector<Point> convexHull(vector<Point>& p) {
    sort(p.begin(), p.end());
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
    s.resize(top + 1);
    return s;
}

/**
 * 旋转卡壳求凸包直径，结果同时也是平面最远点对的距离
 */
ll rotateCalliper(vector<Point>& ch) {
    int n = ch.size();
    if (n == 2) return dis2(ch[0], ch[1]);
    ll diameter = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        while (cross(ch[i], ch[(i + 1) % n], ch[j]) < cross(ch[i], ch[(i + 1) % n], ch[(j + 1) % n])) j = (j + 1) % n;
        diameter = max(diameter, max(dis2(ch[i], ch[j]), dis2(ch[(i + 1) % n], ch[j])));
    }
    return diameter;  // 注意这里是距离平方，若需要精确距离改成 ld 并使用 sqrtl
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        int x, y; cin >> x >> y;
        pts[i].x = x, pts[i].y = y;
    }
    auto ch = convexHull(pts);
    auto d = rotateCalliper(ch);
    cout << d << '\n';
    return 0;
}