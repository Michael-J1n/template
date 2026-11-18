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

/**
 * 旋转卡壳
 * 
 * 本质上是双指针；此处为遍历框架，需要根据题目在循环中更新 res
 * 不变量：
 *      i 枚举有向边 ch[i] -> ch[(i+1)%n]；
 *      每轮 while 结束后，j 是使该有向边与顶点构成的叉积最大的点，即离这条支撑线最远的顶点。
 * 复杂度：O(n)
 */
long long rotateCalliper(vector<Point>& ch) {
    int n = ch.size();
    if (n <= 2) {  // 注意处理退化的情况
        return 0;
    }
    long long res = 0;
    int j = 1;     // 旋转卡壳中注意 j 在第二个点为起点
    for (int i = 0; i < n; ++i) {
        while (/** 这里写 j 指针单调性移动的条件；注意 dis 不是单峰的！要用 cross */
            cross(ch[i], ch[(i + 1) % n], ch[j]) < cross(ch[i], ch[(i + 1) % n], ch[(j + 1) % n])) {
            j = (j + 1) % n;
        }
    }
    return res;
}

/**
 * 在凸包的边上需要保留共线点时，j 指针移动的条件要从 < 变为 <= 以防止平台阻止更大值更新
 */
long long rotateCalliper2(vector<Point>& ch) {
    vector<Point> p;
    for (auto &pt : ch) {
        if (p.empty() || !(p.back() == pt)) p.push_back(pt);
    }
    if (p.size() > 1 && p.front() == p.back()) p.pop_back();  // 去掉首尾重复点，保证边长非零
    int n = p.size();
    if (n <= 2) {  // 根据题目处理空集、单点、线段的答案
        return 0;
    }
    bool collinear = true;
    for (int i = 2; i < n; ++i) {
        if (cross(p[0], p[1], p[i]) != 0) {
            collinear = false; break;
        }
    }
    if (collinear) {  // 根据题目处理全共线的答案，否则 <= 会一直绕圈
        return 0;
    }
    long long res = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        while (cross(p[i], p[(i + 1) % n], p[j]) <= cross(p[i], p[(i + 1) % n], p[(j + 1) % n])) {
            j = (j + 1) % n;
        }
        // 在这里根据题目更新 res；p[j] 是当前边对应的最后一个最远点
    }
    return res;
}
