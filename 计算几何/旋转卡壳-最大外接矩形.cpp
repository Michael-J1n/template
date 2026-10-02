#include <bits/stdc++.h>
using namespace std;
using ll = long double;

struct Point {
    ll x, y;
    Point() : x(0), y(0) {}
    Point(ll _x, ll _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
    Point operator*(ll k) const { return Point(x * k, y * k); }
    Point operator/(ll k) const { return Point(x / k, y / k); }
    bool operator<(const Point &o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
};
ostream &operator<<(ostream &os, const Point &p) {
    os << p.x << ' ' << p.y;
    return os;
}


ll cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

ll cross(const Point &a, const Point &b, const Point &c) {
    return cross(b - a, c - a);
}

ll dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

ll dot(const Point& a, const Point& b, const Point& c) {
    return dot(b - a, c - a);
}

ll norm(const Point &a) {
    return sqrtl(dot(a, a));
}

ll dis(const Point& a, const Point& b) {
    return sqrtl((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

ll projectLength(const Point& a, const Point& b, const Point& c) {
    return dot(a, b, c) / dis(a, b);
}

ll distanceToLine(const Point &p, const Point &a, const Point &b) {
    return fabs(cross(a, b, p) / dis(a, b));
}

Point lineNormal(const Point &a, const Point &b) {
    Point v = b - a;
    ll len = norm(v);
    return Point(-v.y / len, v.x / len);
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

const ll eps = 1e-6;
void zero(Point& p) {
    if (fabs(p.x) < eps) p.x = 0;
    if (fabs(p.y) < eps) p.y = 0;
}

Point p1, p2, p3, p4;

ll rotateCalliper(vector<Point>& ch) {
    int n = ch.size();
    ll area = INT_MAX;
    int a = 1, b = 1, c;
    // 旋转卡壳，逆时针枚举边，找到对踵点 a，再找 i+1->a 间的最长投影 b，再找 a->i 间的最长投影 c；逆时针旋转时 abc 的改变均满足单调性
    for (int i = 0; i < n; ++i) {
        while (cross(ch[i], ch[(i + 1) % n], ch[a]) < cross(ch[i], ch[(i + 1) % n], ch[(a + 1) % n])) a = (a + 1) % n;
        while (dot(ch[i], ch[(i + 1) % n], ch[b]) < dot(ch[i], ch[(i + 1) % n], ch[(b + 1) % n])) b = (b + 1) % n;
        if (i == 0) c = a;
        while (dot(ch[(i + 1) % n], ch[i], ch[c]) < dot(ch[(i + 1) % n], ch[i], ch[(c + 1) % n])) c = (c + 1) % n;
        int j = (i + 1) % n;
        ll d1 = dis(ch[i], ch[j]);
        ll pb = dot(ch[i], ch[j], ch[b]) / d1, pc = dot(ch[j], ch[i], ch[c]) / d1;
        ll h = distanceToLine(ch[a], ch[i], ch[j]);
        ll tmp = h * (pb + pc - d1);
        if (tmp < area) {
            area = tmp;
            Point dir1 = (ch[j] - ch[i]) / d1;
            p1 = ch[j] - dir1 * pc, p2 = ch[i] + dir1 * pb;
            Point dir2 = lineNormal(ch[i], ch[j]);
            p3 = p2 + dir2 * h, p4 = p1 + dir2 * h;
        }
    }
    zero(p1), zero(p2), zero(p3), zero(p4);
    return area;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        ll x, y; cin >> x >> y;
        pts[i].x = x, pts[i].y = y;
    }
    auto ch = convexHull(pts);
    auto res = rotateCalliper(ch);
    cout << fixed << setprecision(5) << res << '\n' << p1 << '\n' << p2 << '\n' << p3 << '\n' << p4 << '\n';
    return 0;
}