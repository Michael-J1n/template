#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x,y;
    Point operator-(const Point& p) const { return { x - p.x, y - p.y }; }
};

/**
 * 两向量 a, b 叉积：
 * >0: 从 a 转向 b 是逆时针
 * <0: 从 a 转向 b 是顺时针
 * =0: 两向量共线（平行或有零向量）
 * 几何意义：a, b 为邻边构成的平行四边形的有向面积
 */
long long cross(Point a,Point b) { return a.x * b.y - a.y * b.x; }

/**
 * 三点 a, b, c 叉积，等价于向量 b - a 与向量 c - a 的叉积
 * 
 * >0: c 在有向直线 ab 左侧
 * <0: c 在有向直线 ab 右侧
 * =0: a, b, c 三点共线
 * 几何意义：三角形 abc 的两倍有向面积
 */
long long cross(Point a,Point b,Point c) { return cross(b - a, c - a); }