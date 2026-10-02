#include <bits/stdc++.h>
using namespace std;

/**
 * Convex Hull Trick（一般形，上凸壳）优化 DP：dp[x] = max{mi * x + bi}，其中 x 为关于 i 的函数，mi 与 bi 为关于 j 的函数
 *
 * 维护一组直线 y = m * x + b，支持：
 *   - add(m, b): 在 O(log n) 时间插入一条直线
 *   - query(x):  在 O(log n) 时间查询 max(m * x + b)
 *
 * 注意：
 * 1. 斜率、查询顺序都不需要单调；允许负值
 * 2. 若要求 min，可在插入时用 add(-m, -b)，查询后再取相反数
 * 3. 如果一开始没有，也要先插入一条(0,0)线
 */
struct Line {
    using ll = long long;
    ll m, b;
    mutable long double x;
    bool operator<(const Line &o) const { return m < o.m; }
    bool operator<(long double v) const { return x < v; }
};
struct LineContainer : multiset<Line, less<>> {
    using ll = long long;
    static constexpr long double INF = 1e30L;
    static long double intersect(const Line &a, const Line &b) {
        if (a.m == b.m) return a.b > b.b ? INF : -INF;
        return (long double)(b.b - a.b) / (long double)(a.m - b.m);
    }
    bool isect(iterator x, iterator y) {
        if (y == end()) {
            x->x = INF;
            return false;
        }
        if (x->m == (*y).m) x->x = x->b > (*y).b ? INF : -INF;
        else x->x = intersect(*x, *y);
        return x->x >= (*y).x;
    }
    void add(ll m, ll b) {
        auto z = insert({m, b, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, erase(y));
        while ((y = x) != begin() && (--x)->x >= y->x) isect(x, erase(y));
    }
    ll query(ll x) const {
        assert(!empty());
        auto it = lower_bound((long double)x);
        if (it == end()) --it;
        return it->m * x + it->b;
    }
};

/**
 * 使用：对于形如 dp[i] = max_{1<=j<=i} C(i) + k(j) * x(i) + b(j) 的式子
 * 其中 C(i) 为和 i 有关的常数，k(j) 和 b(j) 为和 j 有关的直线斜率/截距，x(i) 是查询处的值，代码模板为：
 * 
 * vll dp(n + 1, -LINF); dp[0] = 0;
 * LineContainer lc; lc.add(k(1), b(1));
 * for (int i = 1; i <= n; ++i) {
 *     dp[i] = query(x(i)) + C(i);
 *     add(k(i + 1), b(i + 1));
 * }
 * 
 * 加入的是 k(i + 1) 与 b(i + 1) 是由于 j<=i，那么在 i 的时候，希望可以查到 k(i) 和 b(i) 的信息，因此要在 i-1 处添加这两条线
 * 如果 j<i，就加入 k(i) 与 b(i) 即可。
 * 
 * 注意，在求 min 的时候，需要变为：add(-k(i+1), -b(i+1))，-query(x(i))
 * 
 * 例题：【P3195】给定长为 n 的数组，划分 j..i 的代价是 (i-j+sum[i..j]-L)^2，其中 L 是常数，求最小划分代价。
 * 在计算 dp[i] 的时候，枚举这一段的开头 j，有：dp[i] = min_{1<=j<=i} (dp[j - 1] + (i-j+sum[i..j]-L)^2)
 * 记 S 为数组值的前缀和，那么 dp[i] = min_{1<=j<=i} (dp[j-1] + (i+S[i]-j-S[j-1]-L)^2)
 * 不要暴力拆式子，关键在于识别 C(i),k(j),x(i),b(j):
 * dp[i] = min (i+S[i])^2 - 2(L+j+S[j-1])*(i+S[i]) + dp[j-1]+(L+j+S[j-1])^2
 * 即 C(i)=(i+S[i])^2, k(j)=-2(L+j+S[j-1]), x(i)=i+S[i], b(j)=dp[j-1]+(L+j+S[j-1])^2
 * 最开始需要加入 k(1)=-2(L+1), b(1)=(L+1)^2
 * 此后加入 k(i+1)=-2(L+i+1+S[i]), b=dp[i]+(L+i+1+S[i])^2
 * 
 * 代码（这里是 min，注意取反）：
 * vll dp(n + 1, LINF); dp[0] = 0;
 * LineContainer lc; lc.add((L + 1) * 2, -(L + 1) * (L + 1));
 * for (int i = 1; i <= n; ++i) {
 *     dp[i] = -lc.query(S[i] + i) + (S[i] + i) * (S[i] + i);
 *     lc.add(-2LL * (S[i] + i + 1 + L), -dp[i] - (S[i] + i + 1 + L) * (S[i] + i + 1 + L));
 * }
 * cout << dp[n] << endl;
 */