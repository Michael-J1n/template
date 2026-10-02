#include <bits/stdc++.h>
using namespace std;

/**
 * 二维数论分块
 *
 * 对于形如 sum_{1<=i<=min(n,m)} G(i) * F(floor(n / i), floor(m / i)) 的题目，分块计算使得 floor(n / i) 和 floor(m / i) 都相同的极大连续段 [l, r] 对于答案的贡献
 * 公式：固定 l，r = min(n // (n // l), m // (m // l))
 * 
 * 假设对于 [l, r]，当前 floor(n / i) 值都为 q1，floor(m / i) 值都为 q2；则此块贡献为 F(q1, q2) * sum_{l<=i<=r} G(i) = F(q1, q2) * (Gsum(r) - Gsum(l-1))
 * 模板中 F 函数代表 F(q1, q2) 的值；Gsum 函数代表 [l, r] 间 G(i) 的总和
 *
 * 复杂度：
 * O(sqrt(n) + sqrt(m))
 */
long long F(long long q1, long long q2) {
    return q1 * q2;
}
long long Gsum(long long id) {
    // return pre[id];
    return id;
}
long long calc(long long n, long long m) {
    long long res = 0, lim = min(n, m);
    for (long long l = 1, r; l <= lim; l = r + 1) {
        long long q1 = n / l, q2 = m / l; r = min(n / q1, m / q2);
        // [l, r] 内 floor(n / i) 都等于 q1，floor(m / i) 都等于 q2
        res += (Gsum(r) - Gsum(l - 1)) * F(q1, q2);
    }
    return res;
}