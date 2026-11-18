#include <bits/stdc++.h>
using namespace std;
using ld = long double;
const ld INF = 1e30;

/**
 * 在 O(nlogn) 求出平面中最近点对的距离
 * 最远点对可以通过凸包直径求出，最近点对需要使用分治的思路
 */
struct Point {
    ld x, y;
};
ld dis(const Point& a, const Point& b) {
    ld dx = a.x - b.x, dy = a.y - b.y;
    return sqrtl(dx * dx + dy * dy);
}

ld solve(int l, int r, vector<Point>& a, vector<Point>& t) {
    if (l == r) return INF;
    int m = l + r >> 1;
    ld midx = a[m].x, d = min(solve(l, m, a, t), solve(m + 1, r, a, t));
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (a[i].y < a[j].y) t[k++] = a[i++];
        else t[k++] = a[j++];
    }
    while (i <= m) t[k++] = a[i++];
    while (j <= r) t[k++] = a[j++];
    for (int p = l; p <= r; ++p) a[p] = t[p];
    
    vector<Point> q;
    for (int p = l; p <= r; ++p) {
        if (fabs(a[p].x - midx) < d) q.emplace_back(a[p]);
    }
    int sz = q.size();
    for (int i = 0; i < sz; ++i) {
        for (int j = i + 1; j < sz && q[j].y - q[i].y < d; ++j) {
            d = min(d, dis(q[i], q[j]));
        }
    }
    return d;
}

ld minDis(vector<Point>& pts) {
    int n = pts.size();
    if (n <= 1) return 0;
    vector<Point> a = pts, t(n);
    sort(a.begin(), a.end(), [](const Point& a, const Point& b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });
    return solve(0, n - 1, a, t);
}