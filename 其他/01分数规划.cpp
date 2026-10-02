#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-10;

/**
 * 0/1分数规划
 *
 * 对于形如 max sum_{i∈S}f[i] / sum_{i∈S}g[i] 的问题，二分答案；转化为判定 sum f[i] / sum g[i] >= mid
 * <==> sum_{i∈S} (f[i] - mid * g[i]) >= 0；因此如果存在集合 S 满足此式，更新 l = mid，否则更新 r = mid
 * 
 * 如果需要最小值，将符号反向即可
 *
 * 复杂度：
 *   O(T * n log n)，T 为二分次数
 */
double frac01(vector<int>& a, vector<int>& b, int k) {
    int n = a.size();
    vector<double> c(n);
    // 检查是否存在集合 S 使得 sum_{i∈S}(f[i] - mid * g[i]) >= 0
    auto check = [&](double mid) -> bool {
        for (int i = 0; i < n; ++i) c[i] = a[i] - mid * b[i];
        ranges::sort(c, greater<>());
        double sum = 0;
        for (int i = 0; i < k; ++i) sum += c[i];
        return sum >= -eps;
    };
    double l = 0, r = 1e9;
    for (int t = 0; t < 60; ++t) {
        double mid = (l + r) / 2;
        if (check(mid)) l = mid;
        else r = mid;
    }
    return l;
}