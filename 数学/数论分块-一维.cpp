#include <bits/stdc++.h>
using namespace std;

/**
 * 一维数论分块
 *
 * 对于形如 sum_{1<=i<=n} G(i) * F(floor(m / i)) 的题目，分块计算使得 floor(m / i) 相同的极大连续段 [l, r] 对于答案的贡献
 * 
 * 假设对于 [l, r]，当前 floor(m / i) 值都为 q；则此块贡献为 F(q) * sum_{l<=i<=r} G(i) = F(q) * (Gsum(r) - Gsum(l-1))
 * 模板中 F 函数代表 F(q) 的值；Gsum 函数代表 [l, r] 间 G(i) 的总和
 *
 * 复杂度：O(sqrt(n))
 * 
 * 变式：
 * 1. 上取整：sum_{1<=i<=n} G(i) * F(upfloor(n / i)) = G(n) * F(1) + sum_{1<=i<=n-1} G(i) * F(floor((n-1) / i) + 1)
 * 2. 二维取整 F(floor(n/i), floor(m/i))： 见数论分块-二维
 * 3. 反演形式：对于求和式 sum_{j=i,..,n} floor(j / i) 有闭式 (n + 1 - i/2) q - i/2 q^2，其中 q = floor(n / i)
 * 
 * 注意：有类似形式的，见“倍数差分前缀和”
 */
long long F(long long q) {
    return q;
}
long long Gsum(long long id) {
    // 填写 G[1] + G[2] + ... + G[id]，推式子或预处理前缀和
    return id;
}
long long calc(long long n, long long m) {
    long long res = 0;
    for (long long l = 1, r; l <= n; l = r + 1) {
        long long q = m / l;
        if (q == 0) r = n;
        else r = min(n, m / q);
        // [l, r] 内 floor(m / i) 都等于 q
        res += (Gsum(r) - Gsum(l - 1)) * F(q);
    }
    return res;
}

/**
 * （重要！）常见数论求和推式子技巧：
 * 
 * 0. 分离变量：sum_i sum_j A(i)B(j) = sum_i A(i) * sum_j B(j)
 * 
 * 1. 求和换位：sum_i sum_j A(i)B(j) = sum_j sum_i A(i)B(j)
 * 
 * 2. 替换条件式：sum_{d|gcd(i,j)} d = sum_{1<=d<=n} [d|i] * [d|j] * d
 * 
 * 3. 换元归一：sum_{1<=i<=n} sum_{1<=j<=m} [gcd(i,j) == k] ---{换元 i=ki',j=kj'}--->
 *             = sum_{1<=i'<=floor(n/k)} sum_{1<=j'<=floor(m/k)} [gcd(i',j') == 1]
 * 
 * 4. 使用指标变量：sum_{i|x} f(i) = sum_{1<=i<=n} f(i) * [i|x]
 * 
 * 5. 指标式转化：[n == 1] = sum_{d|n} mu(d); 注意当指标中不是 1 而是 k 时，用 3 换元归一先处理；转化好后可以利用 4 将 sum_{d|n} d 变为 sum_{1<=d<=n} mu(d) * [d|n]
 * 
 * 6. 互素指标式转化： [gcd(i,j) == 1] = sum_{d|gcd(i,j)} mu(d) = sum_{1<=d<=n} mu(d) * [d|i] * [d|j]
 * 
 * 7. GCD 整除关系转化：d | gcd(i,j) <==> d|i && d|j；因此对于 [d|gcd(i,j)] 指标的求和就是对于 [d|i]*[d|j] 的求和；这一步做完后常跟着对 i,j 分离变量分别求和
 * 
 * 8. 欧拉函数定义：phi(n) = sum_{1<=i<=n} [gcd(i,n) == 1]
 * 
 * 9. 卷积 phi * 1 = id： sum_{d|n} phi(d) = n
 * 
 * 10. 卷积 mu * id = phi：sum_{d|n} mu(d) * (n / d) = phi(n)
 * 
 * 11. 反演：sum_{j=i..n} floor(j / i) = (n + 1 - i/2) q - i/2 q^2，其中 q = floor(n/i)；如果外层再套求和，就可以使用数论分块。例如：
 *     推式子  sum_{i=1..n} sum_{j=1..i} floor(i / j) * (-1)^j，首先使用“横看成岭侧成峰”的 trick 转化为
 *          = sum_{i=1..n} (-1)^i sum_{j=i..n} floor(j / i)，使用反演得到公式并进行数论分块计算即可
 * 
 * 12. GCD 的求和： sum_{1<=i<=n} sum_{1<=j<=n} gcd(i, j) 
 *              = sum_{1<=i<=n} sum_{1<=j<=n} sum_{d|gcd(i,j)} phi(d) 
 *              = sum_{1<=i<=n} sum_{1<=j<=n} sum_{1<=d<=n} phi(d) * [d|i] * [d|j] 
 *              = sum_{1<=d<=n} phi(d) * sum_{1<=i<=n} [d|i] * sum_{1<=j<=n} [d|j]
 *              = sum_{1<=d<=n} phi(d) * (n//d)^2
 * 
 * 13. GCD 指标的求和：sum_{1<=i<=n} sum_{1<=j<=m} [gcd(i,j) == k]
 *                 = sum_{1<=i<=n//i} sum_{1<=j<=m//k} [gcd(i,j) == 1]
 *                 = sum_{1<=i<=n//i} sum_{1<=j<=m//k} sum_{d|gcd(i,j)} mu(d)
 *                 = sum_{1<=i<=n//i} sum_{1<=j<=m//k} sum_{1<=d<=lim} mu(d) * [d|i] * [d|j], lim = min(n//k,m//k)
 *                 = sum_{1<=d<=lim} mu(d) * sum_{1<=i<=n//k} [d|i] * sum_{1<=j<=m//k} [d|j]
 *                 = sum_{1<=d<=lim} mu(d) * (n//k)//d * (m//k)//d
 *                 = sum_{1<=d<=lim} mu(d) * (n // (kd)) * (m // (kd))
 *    改编：若为 [gcd(i,j) ∈ prime]，则再加一个指示变量 [k ∈ prime] 乘在后面，最后可转化到 sum_{1<=T<=n} sum_{k ∈ prime} mu(T/k) * (n//T) * (m//T)
 * 
 * 14. 乘积的约数个数恒等式：d(i * j) = sum_{x|i} sum_{y|j} [gcd(x,y) == 1]
 * 例：
 *       sum_{1<=i<=n} sum_{1<=j<=m} d(i * j)
 *     = sum_{1<=i<=n} sum_{1<=j<=m} sum_{x|i} sum_{y|j} [gcd(x,y) == 1]
 *     = sum_{1<=i<=n} sum_{1<=j<=m} sum_{1<=x<=n} sum_{1<=y<=m} [x|i] * [y|j] * [gcd(x,y) == 1]
 *     = sum_{1<=x<=n} sum_{1<=y<=m} (sum_{1<=i<=n} [x|i]) * (sum_{1<=j<=m} [y|j]) * [gcd(x,y) == 1]
 *     = sum_{1<=x<=n} sum_{1<=y<=m} n//x * m//y * sum_{1<=d<=lim} [d|x] * [d|y] * mu(d)，lim=min(n,m)
 *     = sum_{1<=d<=lim} mu(d) * sum_{1<=x<=n} [d|x] n//x * sum_{1<=y<=m} [d|y] m//y，使用换元归一，令 x=x'd, y=y'd
 *     = sum_{1<=d<=lim} mu(d) * sum_{1<=x'<=n//d} n//(dx') * sum_{1<=y'<=m/d} m//(dy')
 * 
 * 15. LCM 的求和： sum_{1<=i<=n} sum_{1<=j<=m} lcm(i,j)
 *              = sum_{1<=i<=n} sum_{1<=j<=m} i*j / gcd(i,j)
 *              = sum_{1<=i<=n} sum_{1<=j<=m} i*j * sum_{1<=d<=lim} [gcd(i,j)==d]/d，lim=min(n,m) 【注意这里对于除法的操作】使用归一化技巧做 i=i'd, j=j'd
 *              = sum_{1<=d<=lim} sum_{1<=i'<=n//d} sum_{1<=j'<=m//d} i*j*d * [gcd(i,j)==1]
 *              = sum_{1<=d<=lim} sum_{1<=i'<=n//d} sum_{1<=j'<=m//d} i'*j'*d * sum_{1<=k<=lim//d} mu(k) * [k|i'] * [k|j']
 *              = sum_{1<=d<=lim} d * sum_{1<=k<=lim//d} mu(k) * sum_{1<=i'<=n//d} i' * [k|i'] * sum_{1<=j'<=m//d} j' * [k|j'] 再使用归一化技巧，令 i'=i''k, j'=j''k
 *              = sum_{1<=d<=lim} d * sum_{1<=k<=lim//d} mu(k) * sum_{1<=i''<=n//dk} i''k * sum_{1<=j''<=m//dk} j''k
 *              = sum_{1<=d<=lim} d * sum_{1<=k<=lim//d} mu(k)*k^2 * sum_{1<=i''<=n//dk}i'' * sum_{1<=j''<=m//dk}j''
 */