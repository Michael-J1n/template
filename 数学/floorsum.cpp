#include <bits/stdc++.h>
using namespace std;

/**
 * 求 sum_{i=0,1..,n-1} floor{(a*i+b)/m}，其中 n>=0,m>0,a>=0,b>=0 为整数
 * 几何意义：统计直线 y=a/m*x+b/m 下方，0<=x<n 且 y>=1 的整点数量
 */
long long floor_sum(long long n, long long m, long long a, long long b) {
    long long res = 0;
    while (1) {
        if (a >= m) { res += n * (n - 1) / 2 * (a / m); a %= m; }
        if (b >= m) { res += n * (b / m); b %= m; }
        long long y = a * n + b; if (y < m) break;
        n = y / m; b = y % m; swap(a, m);
    }
    return res;
}