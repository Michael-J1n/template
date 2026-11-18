#include <bits/stdc++.h>
using namespace std;

/**
 * 曼哈顿距离与切比雪夫距离双向转化
 * 曼哈顿距离的等距线是菱形，切比雪夫距离的等距线是正方形
 * 1. 曼哈顿转切比雪夫：(x, y) --> (x + y, x - y)     ==> M(x,y) = C(x',y')
 * 2. 切比雪夫转曼哈顿：(x, y) --> ((x+y)/2, (x-y)/2) ==> C(x,y) = M(x',y')
 * 
 * 相关 tricks/母题（通过距离互相转化，把 O(n^2) 的统计变为 O(n) 的统计）：
 * 1. 给定 n 个点，求最大曼哈顿距离：转化到切比雪夫，那么只需要 max(maxU-minU,maxV-minV)
 * 2. 给定 n 个点，求切比雪夫距离之和：转化到曼哈顿，那么只需要求 sum(|xi-xj|)+sum(|yi-yj|)【注意 C->M 时 1/2 可以先忽略，最后答案整体乘以 1/2 即可】
 */
pair<long long, long long> M2C(long long x, long long y) { return {x + y, x - y}; }
pair<long long, long long> C2M(long long u, long long v) { return {(u + v) / 2, (u - v) / 2}; }
long long disM(long long x1, long long y1, long long x2, long long y2) { return abs(x1 - x2) + abs(y1 - y2); }
long long disC(long long x1, long long y1, long long x2, long long y2) { return max(abs(x1 - x2), abs(y1 - y2)); }


/** 
 * 一些常见距离最小化的模板；复杂度均为 O(n logn)
 */
struct Point { long long x, y; };
long long manhattan(const Point& a, const Point& b) { return abs(a.x - b.x) + abs(a.y - b.y); }
long long chebyshev(const Point& a, const Point& b) { return max(abs(a.x - b.x), abs(a.y - b.y)); }
struct HalfResult { long long x2, y2, dist2; };
// 返回 sum |a[i]-x|
long long abs_sum(const vector<long long>& a, long long x) {
    long long res = 0;
    for (auto y : a) res += abs(y - x);
    return res;
}
// 返回最小化 sum |a[i]-x| 的 {最优位置，最小距离和}，位置需要是原数组值
pair<long long, long long> line_min(vector<long long> a) {
    sort(a.begin(), a.end());
    int n = a.size();
    long long x = a[n >> 1];
    return {x, abs_sum(a, x)};
}
// 返回最小化 sum |a[i]-x| 的 {奇偶性为 p 的最优位置，最小距离和}
array<pair<long long, long long>, 2> line_min_parity(vector<long long> a) {
    sort(a.begin(), a.end());
    int n = a.size();
    long long m = a[n >> 1];
    array<pair<long long, long long>, 2> res;
    for (int p = 0; p < 2; ++p) {
        if ((m & 1) == p) {
            res[p] = {m, abs_sum(a, m)};
            continue;
        }
        long long x1 = m - 1, x2 = m + 1;
        long long s1 = abs_sum(a, x1), s2 = abs_sum(a, x2);
        if (s1 <= s2) res[p] = {x1, s1};
        else res[p] = {x2, s2};
    }
    return res;
}
// 返回 res[i] = sum_j |a[i] - a[j]|
vector<long long> all_abs_sum(const vector<long long>& a) {
    int n = a.size();
    vector<pair<long long, int>> ord(n);
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        ord[i] = {a[i], i};
        total += a[i];
    }
    sort(ord.begin(), ord.end());
    vector<long long> res(n);
    long long pre = 0;
    for (int i = 0; i < n; ++i) {
        auto [x, id] = ord[i];
        long long left = 1LL * i * x - pre, right = total - pre - x - 1LL * (n - i - 1) * x;
        res[id] = left + right;
        pre += x;
    }
    return res;
}
// 最小化候选点为任意点时（一定可以是整数点），它到数组中所有点的曼哈顿距离之和；返回 {最优点, 最小距离和}
pair<Point, long long> min_manhattan_any(const vector<Point>& p) {
    int n = p.size();
    vector<long long> xs(n), ys(n);
    for (int i = 0; i < n; ++i) xs[i] = p[i].x, ys[i] = p[i].y;
    auto [x, sx] = line_min(xs);
    auto [y, sy] = line_min(ys);
    return {{x, y}, sx + sy};
}
// 最小化候选点必须选择原来的某个点时，它到数组中所有点的曼哈顿距离之和；返回 {最优点，最小距离和}
pair<Point, long long> min_manhattan_original(const vector<Point>& p) {
    int n = p.size();
    vector<long long> xs(n), ys(n);
    for (int i = 0; i < n; ++i) xs[i] = p[i].x, ys[i] = p[i].y;
    auto sx = all_abs_sum(xs);
    auto sy = all_abs_sum(ys);
    int best = 0;
    for (int i = 1; i < n; ++i) if (sx[i] + sy[i] < sx[best] + sy[best]) best = i;
    return {p[best], sx[best] + sy[best]};
}
// 最小化候选点为任意实数点时，它到数组中所有点的切比雪夫距离之和；返回 {x2=2x，y2=2y，dist2=2*距离和}
HalfResult min_chebyshev_any(const vector<Point>& p) {
    int n = p.size();
    vector<long long> us(n), vs(n);
    for (int i = 0; i < n; ++i) us[i] = p[i].x + p[i].y, vs[i] = p[i].x - p[i].y;
    auto [u, su] = line_min(us);
    auto [v, sv] = line_min(vs);
    return {u + v, u - v, su + sv};
}
// 最小化候选点为任意整数点时，它到数组中所有点的切比雪夫距离之和；返回 {x2=2x，y2=2y，dist2=2*距离和}
pair<Point, long long> min_chebyshev_integer(const vector<Point>& p) {
    int n = p.size();
    vector<long long> us(n), vs(n);
    for (int i = 0; i < n; ++i) us[i] = p[i].x + p[i].y, vs[i] = p[i].x - p[i].y;
    auto ru = line_min_parity(us);
    auto rv = line_min_parity(vs);
    int parity = 0;
    if (ru[1].second + rv[1].second < ru[0].second + rv[0].second) parity = 1;
    long long u = ru[parity].first, v = rv[parity].first;
    long long dist = (ru[parity].second + rv[parity].second) >> 1;
    return {{(u + v) >> 1, (u - v) >> 1}, dist};
}
// 最小化候选点为必须选择原来的某个点时，它到数组中所有点的切比雪夫距离之和；返回 {x2=2x，y2=2y，dist2=2*距离和}
pair<Point, long long> min_chebyshev_original(const vector<Point>& p) {
    int n = p.size();
    vector<long long> us(n), vs(n);
    for (int i = 0; i < n; ++i) us[i] = p[i].x + p[i].y, vs[i] = p[i].x - p[i].y;
    auto su = all_abs_sum(us);
    auto sv = all_abs_sum(vs);
    int best = 0;
    for (int i = 1; i < n; ++i) if (su[i] + sv[i] < su[best] + sv[best]) best = i;
    return {p[best], (su[best] + sv[best]) >> 1};
}