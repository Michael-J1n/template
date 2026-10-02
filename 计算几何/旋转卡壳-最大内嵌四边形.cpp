#include <bits/stdc++.h>
using namespace std;
using ll = long double;

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
    return cross(b - a, c - a); // (b-a) x (c-a)
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

// 凸包点数 >= 4 时的旋转卡壳
ll rotateCalliper(vector<Point>& ch) {
    int n = ch.size();
    ll area = 0;
    // 逆时针枚举对角线 i-j
    for (int i = 0; i < n; ++i) {
        int k1 = i + 1, k2 = i;  // k1 为 j->i 间的点，k2 为 i->j 间的点，两个区域分别指针找最大面积点
        for (int j = i + 1; j < n; ++j) {
            while (cross(ch[i], ch[j], ch[k1]) < cross(ch[i], ch[j], ch[(k1 + 1) % n])) k1 = (k1 + 1) % n;
            while (cross(ch[j], ch[i], ch[k2]) < cross(ch[j], ch[i], ch[(k2 + 1) % n])) k2 = (k2 + 1) % n;
            area = max(area, cross(ch[i], ch[j], ch[k1]) + cross(ch[j], ch[i], ch[k2]));
        }
    }
    return area / 2.0L;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        ll x, y; 
        cin >> x >> y;
        pts[i].x = x, pts[i].y = y;
    }

    auto ch = convexHull(pts);
    int m = ch.size();
    const ll EPS = 1e-10L;
    ll ans = 0.0L;
    if (m <= 2) {
        ans = 0.0L;
    } else if (m == 3) {
        // 凸包是三角形 ABC，需要求 最大凹四边形：S_ABC - min(三个小三角形)
        Point A = ch[0], B = ch[1], C = ch[2];
        ll S_ABC = fabsl(cross(A, B, C)) / 2.0L;
        for (int i = 0; i < n; ++i) {
            Point P = pts[i];
            bool isHull = false;
            if (fabsl(P.x - A.x) < EPS && fabsl(P.y - A.y) < EPS) isHull = true;
            if (fabsl(P.x - B.x) < EPS && fabsl(P.y - B.y) < EPS) isHull = true;
            if (fabsl(P.x - C.x) < EPS && fabsl(P.y - C.y) < EPS) isHull = true;
            if (isHull) continue;
            ll S1 = fabsl(cross(A, B, P)) / 2.0L, S2 = fabsl(cross(A, C, P)) / 2.0L, S3 = fabsl(cross(B, C, P)) / 2.0L;
            ll cut = min(S1, min(S2, S3));
            ans = max(ans, S_ABC - cut);
        }
    } else {
        ans = rotateCalliper(ch);
    }
    cout << fixed << setprecision(3) << (double)(ans + 1e-8L) << '\n';
    return 0;
}
