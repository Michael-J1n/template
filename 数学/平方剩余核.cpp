#include <bits/stdc++.h>
using namespace std;

/**
 * 平方剩余核
 * 定义：一个数 x 的平方剩余核 core[x] 为他所有质因数指数 %=2 后乘得得数，即为去除最大平方因子后得结果
 * 性质：
 * 1. x * y 为完全平方数 <==> core[x] == core[y]【重要！】
 * 2. 记 core[x]=a, core[y]=b，则 core[x * y] = a*b / gcd(a,b)^2
 * 3. 使 x * y 成为完全平方数的最小正整数 y 为 core[x]，所有满足条件的 y 为 {core[x] * k^2}
 * 
 * core(x) 作为集合的视角：令 S(x) 为保存了 x 的奇数幂次的素因子的集合，则 core(x) 即为 S(x) 中元素的乘积。
 * 区间 [l,r] 的乘积为完全平方数 <==> S(P[r])==S(P[l-1])，其中 P[i] 为前缀 a[0..i] 上的积。
 */
// 单点，O(sqrt(x))
int kernel(int x) {
    int res = 1;
    for (int p = 2; 1LL * p * p <= x; ++p) {
        int cnt = 0;
        while (x % p == 0) x /= p, cnt ^= 1;
        if (cnt) res *= p;
    }
    if (x > 1) res *= x;
    return res;
}
// 预处理，O(MX)，其中 MX 为最大值域
const int MX = 1e6 + 5;
int core[MX];
void init() {
    memset(core, 0, sizeof core);
    for (int i = 1; i < MX; ++i) {
        if (core[i]) continue;
        for (int j = 1; 1LL * i * j * j < MX; ++j) core[i * j * j] = i;
    }
}