#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    ll x, y;
    Point() : x(0), y(0) {}
    Point(ll _x, ll _y) : x(_x), y(_y) {}
    Point operator+(const Point &o) const { return Point(x + o.x, y + o.y); }
    Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
};

const long double PI = acosl(-1.0L);
const long double TAU = PI * 2.0L;

/** @brief 极角统计结构（围绕原点）
 *
 *  - 输入：一批点
 *  - 处理：把所有“非原点”的点按极角 θ ∈ [0, 2π) 排序存入 ang
 *  - 查询：给定两条射线 theta1, theta2，统计从 theta1 逆时针扫到 theta2 这段扇形内的点数（包含边界），自动加入 origin_cnt
 */
struct AnglePrefix {
    using ld = long double;
    static constexpr ld EPS = (ld)1e-12L;

    int n;              // 非原点点数量
    int origin_cnt;     // 在原点的点数量
    vector<ld> ang;     // 非原点的极角数组（排序后）

    AnglePrefix() : n(0), origin_cnt(0) {}
    AnglePrefix(const vector<Point> &pts) { init(pts); }

    /** 将任意弧度规范到 [0, 2π) */
    static ld normAngle(ld t) {
        t = fmodl(t, TAU);
        if (t < 0) t += TAU;
        return t;
    }

    /** 用点集初始化极角结构（以原点为极点） */
    void init(const vector<Point> &pts) {
        origin_cnt = 0;
        ang.clear();
        ang.reserve(pts.size());
        for (auto &p : pts) {
            if (p.x == 0 && p.y == 0) {
                ++origin_cnt;
                continue;
            }
            ld t = atan2l((ld)p.y, (ld)p.x);
            ang.push_back(normAngle(t));
        }
        sort(ang.begin(), ang.end());
        n = (int)ang.size();
    }

    /** @brief 统计：逆时针从 theta1 到 theta2 的闭区间内有多少个点
     *  - 若扫角 >= 2π，则视为整圆，返回所有点（非原点 + 原点）。
     *  - 一般情况：
     *      theta1 <= theta2：统计 [theta1 - EPS, theta2 + EPS]；
     *      theta1 >  theta2：统计 [theta1 - EPS, 2π) ∪ [0, theta2 + EPS]。
     *  - 结果始终自动加上 origin_cnt。
     */
    int countCCW(ld theta1, ld theta2) const {
        if (n == 0) return origin_cnt;
        ld rawDiff = theta2 - theta1;
        if (rawDiff >= TAU - EPS) {
            return n + origin_cnt;
        }

        theta1 = normAngle(theta1);
        theta2 = normAngle(theta2);
        int res = 0;
        if (theta1 <= theta2) {
            auto itL = lower_bound(ang.begin(), ang.end(), theta1 - EPS);
            auto itR = upper_bound(ang.begin(), ang.end(), theta2 + EPS);
            res = (int)(itR - itL);
        } else {
            auto itL1 = lower_bound(ang.begin(), ang.end(), theta1 - EPS);
            auto itR1 = ang.end();
            auto itL2 = ang.begin();
            auto itR2 = upper_bound(ang.begin(), ang.end(), theta2 + EPS);
            res = (int)(itR1 - itL1) + (int)(itR2 - itL2);
        }
        return res + origin_cnt;
    }

    /** @brief 顺时针统计：从 theta1 顺时针扫到 theta2 的点数 */
    int countCW(ld theta1, ld theta2) const {
        int tot = n + origin_cnt;
        if (tot == 0) return 0;
        theta1 = normAngle(theta1);
        theta2 = normAngle(theta2);
        if (theta1 >= theta2) return countCCW(theta2, theta1);
        int insideCCW = countCCW(theta1, theta2);
        return tot - insideCCW;
    }
};