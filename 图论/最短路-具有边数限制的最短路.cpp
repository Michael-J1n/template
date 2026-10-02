#include <bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;
const long long LINF = LLONG_MAX;

/**
 * 求从 S 到 E，恰好走 k 条边的最短路
 *
 * 做法1：
 * 分层图最短路；定义 dp[u][t] 为到节点u，经过t条边的最短路；
 * 有 dp[u][t]=min{dp[v][t-1]+w(v,u)}，第二层可滚动，按此做DP即可
 *
 * 复杂度 O(mk)
 */
int minCostKEdges(vector<vector<pair<int, int>>>& g, int S, int E, int k) {
    int n = g.size();
    vector<int> dp(n, INF); dp[S] = 0;
    for (int t = 0; t < k; ++t) {
        vector<int> ndp(n, INF);
        for (int u = 0; u < n; ++u) {
            if (dp[u] == INF) continue;
            for (auto& [v, w] : g[u]) {
                ndp[v] = min(ndp[v], dp[u] + w);
            }
        }
        dp = move(ndp);
    }
    return dp[E];
}

/**
 * 做法2：
 * 当 k 很大时，上述做法每层转移，太慢了。考虑优化：
 * 
 * 仍然定义 dp[u][t] 为从 S 到 u，走了t条边的最短路。那么 dp[u][t]=min(dp[0][t-1]+w(0,u),dp[1][t-1]+w(1,u),...,dp[n-1][t-1]+w(n-1,u)} 是 min-plus 转移形式：
 * col(dp[0][t],dp[1][t],..,dp[n-1][t]) = ([a[0][0],a[1][0],..,a[n-1][0]],[a[0][1],a[1][1],..,a[n-1][1]],...,[a[0][n-1],a[1][n-1],..,a[n-1][n-1]]) ⊗ col(dp[0][t-1],dp[1][t-1],..,dp[n-1][t-1])
 * 若把 col(dp[0][t],..,dp[n-1][t]) 合成为列向量状态 V[t]，有：V[t] = M ⊗ V[t-1]，其中 M 为邻接矩阵 A 的转置, M=A^T.
 * 按照矩阵快速幂，V[k] = M^{k} ⊗ V[0]，初始状态 V[0]=col(dp[0][0],..,dp[n-1][0])=col(inf,inf,..,0,inf,..,inf)，其中只有第S行为0.
 * 答案即为 V[k] 的第 E 行。
 * 
 * 复杂度 O(n^3 log k)
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
long long minCostKEdges(vector<vector<int>>& A, int S, int E, int k) {
    int n = A.size();
    Matrix M(n, vector<long long>(n, LINF));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (A[i][j] != INF) M[j][i] = A[i][j];
        }
    }
    vector<long long> V(n, LINF); V[S] = 0;
    M = qpow(M, k);
    V = mul(M, V);
    return V[E];
}

/**
 * 如果是固定 k，但多次询问求 S 到 E 经过 k 条边的最短路呢？
 * 
 * 1. 对于每个起点做单源分层图，复杂度 O(nmk)
 * 
 * 2. k 很大时，仍然有 O(n^3 log k) 的快速幂做法，见下文：
 * 注意到，从每个起点开始，M^k 都是一样的，只有初始列向量 V[0] 不一样。因此考虑将所有 V[i] 堆叠成一个方阵：S = concat(V_0[0],V_1[0],..,V_{n-1}[0]) = diag(0)，其余是 inf.
 * 那么做 M^{k} ⊗ S 得到的结果即为 RES=concat(V_0[k],V_1[k],..,V_{n-1}[k])；查询 (S,E) 即为 RES[E][S]（注意这里起点维度是列，终点维度是行）
 * 实现中注意 S 就是 min-plus 的单位方阵 I，可以省去这次计算，做一次快速幂即可
 */
Matrix minCostKEdgesAll(vector<vector<int>>& A, long long k) {
    int n = A.size();
    Matrix M(n, vector<long long>(n, LINF));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (A[i][j] != INF) M[j][i] = A[i][j];
        }
    }
    return qpow(M, k);  // 查询 (S,E): res[E][S]
}