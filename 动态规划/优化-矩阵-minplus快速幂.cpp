#include <bits/stdc++.h>
using namespace std;
const long long LINF = 4e18;

/**
 * Min-Plus 矩阵快速幂；加速“恰好走 k 次转移”的 DP
 * 对于类似 dp(a+b,i,j) = min_k (dp(a,i,k) + dp(b,k,j)) 或 dp(i,x) = min_v (dp(i-1,v) + cost(v,u)) 的转移，使用快速幂将 n^3k 优化为 n^3logk
 * 关键在于结构 F_(a+b) = F_a ⊗ F_b，其中 ⊗ 为 min_plus 卷积。
 * 若需要 max-plus，将取 min 改为取 max 即可
 * 
 * 基础转移矩阵 A：
 * A[i][j] = 状态 i 经过 1 次转移到状态 j 的最小代价
 * Min-Plus 乘法：C[i][j] = min_k(A[i][k] + B[k][j])；表示把“i -> k 的一段转移”和“k -> j 的一段转移”拼接
 * 因此 A^k[i][j] = 状态 i 恰好经过 k 次转移到状态 j 的最小代价
 *
 * 复杂度：O(n^3 log k)
 * 
 * 注意：min-plus 矩阵的单位元为 {{0,LINF},{LINF,0}};
 */
using Matrix = vector<vector<long long>>;
Matrix mul(const Matrix& a, const Matrix& b) {
    int n = a.size();
    vector c(n, vector<long long>(n, LINF));
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            if (a[i][k] == LINF) continue;
            for (int j = 0; j < n; ++j) {
                if (b[k][j] == LINF) continue;
                c[i][j] = min(c[i][j], a[i][k] + b[k][j]);
            }
        }
    }
    return c;
}
vector<long long> mul(const Matrix& a, const vector<long long>& b) {
    int n = a.size();
    vector<long long> c(n, LINF);
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            if (a[i][k] == LINF || b[k] == LINF) continue;
            c[i] = min(c[i], a[i][k] + b[k]);
        }
    }
    return c;
}
Matrix qpow(Matrix a, long long k) {
    int n = a.size();
    vector res(n, vector<long long>(n, LINF));
    for (int i = 0; i < n; ++i) res[i][i] = 0;
    while (k) {
        if (k & 1) res = mul(res, a);
        a = mul(a, a), k >>= 1;
    }
    return res;
}

/**
* 例题：
* 在无向有权图中，求从起点 S 到终点 E 经过恰好 K 条边的最短路长度
*
* 识别到特殊操作限制，首先可以联想到分层图最短路。定义 dis[u][t] 为到点 u，经过恰好 t 条边的最短路，做 Dijkstra 可以达到 O(mk log(nk)) 的复杂度
* （事实上，由于边只会从第 t 层走到第 t+1 层，也可以直接按层 DP，做到 O(mk)）
*
* 如果 k 的值域非常大，这种做法就无法通过了。考虑如下 DP：
* 定义 dp[i][u] 为：从起点 S 出发，经过恰好 i 条边到达点 u 的最短路长度。
* 转移有：dp[i+1][v] = min_u(dp[i][u] + cost(u,v))（如果不可达，cost=INF）
*
* 定义矩阵 A[u][v] = cost(u,v)，并把 dp[i] 看作一个行向量。
* 定义 min-plus 矩阵乘法：C[i][j] = min_k(A[i][k] + B[k][j])
* 此时 dp[i+1] = dp[i] * A，因此 dp[K] = dp[0] * A^K；其中 dp[0][S] = 0，其余位置为 INF，所以 dp[K][E] 即答案。
*
* 也可以直接理解为：A^k[u][v] 表示从 u 到 v 恰好经过 k 条边的最短路，
* 因为一条恰好经过 x+y 条边的路径，可以在第 x 条边后枚举中间点 p，
* 所以 A^(x+y)[u][v] = min_p(A^x[u][p] + A^y[p][v])，正好满足 min-plus 矩阵乘法。
*
* 因此可以用矩阵快速幂把 O(km) 优化为 O(n^3 log k)。
*
* 识别特征：
* 有限状态 + 每一步使用相同转移 + dp 转移形如 min(old + cost) + 步数 k 极大
* => 考虑 min-plus 矩阵快速幂。
*/
