#include <bits/stdc++.h>
using namespace std;

long long qpow(long long a, long long b, long long mod) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}
pair<long long, long long> mul_linear(pair<long long, long long> a, pair<long long, long long> b, long long mod) { return {a.first * b.first % mod, (a.first * b.second + a.second * b.first) % mod}; }
/** det_linear(a, mod)：返回 { [x^0]det(a), [x^1]det(a) } */
pair<long long, long long> det_linear(vector<vector<pair<long long, long long>>> a, long long mod) {
    int n = a.size();
    pair<long long, long long> res = {1, 0};
    for (int i = 0; i < n; ++i) {
        int p = -1, q = -1;
        for (int j = i; j < n && p == -1; ++j) {
            for (int k = i; k < n; ++k) {
                if (a[j][k].first) {
                    p = j, q = k;
                    break;
                }
            }
        }
        if (p == -1) {
            if (n - i >= 2) return {0, 0};
            return mul_linear(res, a[i][i], mod);
        }
        if (p != i) {
            swap(a[p], a[i]);
            res.first = (mod - res.first) % mod, res.second = (mod - res.second) % mod;
        }
        if (q != i) {
            for (int j = 0; j < n; ++j) swap(a[j][q], a[j][i]);
            res.first = (mod - res.first) % mod, res.second = (mod - res.second) % mod;
        }
        long long inv0 = qpow(a[i][i].first, mod - 2, mod);
        pair<long long, long long> inv = {inv0, (mod - a[i][i].second * inv0 % mod * inv0 % mod) % mod };
        for (int j = i + 1; j < n; ++j) {
            pair<long long, long long> k = mul_linear(a[j][i], inv, mod);
            for (int t = i; t < n; ++t) {
                pair<long long, long long> v = mul_linear(k, a[i][t], mod);
                a[j][t].first = (a[j][t].first - v.first + mod) % mod;
                a[j][t].second = (a[j][t].second - v.second + mod) % mod;
            }
        }
        res = mul_linear(res, a[i][i], mod);
    }
    return res;
}

/**
 *【矩阵树生成函数】
 * 生成树边权和 / Spanning Tree Weight Sum【所有生成树边权和之和】
 * 点下标为 0-based，边格式为 {u, v, w}，mod 为质数。
 * 每棵生成树 T 的权值定义为 Σ_{e∈T} w_e，返回所有生成树权值之和。
 *
 * 普通带权矩阵树定理中，若边权为 w_e，则 det = Σ_T ∏_{e∈T} w_e；将每条边权替换为 1 + w_e x，得到生成函数 F(x) = Σ_T ∏_{e∈T}(1 + w_e x)。
 * 对一棵生成树展开 ∏(1 + w_e x)，其中 [x^0] = 1，[x^1] = Σ w_e，因此 [x^0]F(x) 为生成树数量，[x^1]F(x) 为所有生成树边权和之和。
 *
 * 复杂度：O(n^3 + m)。
 */
long long spanning_tree_sum(int n, vector<tuple<int, int, long long>>& edges, long long mod) {
    vector<vector<pair<long long, long long>>> a(n - 1, vector<pair<long long, long long>>(n - 1));
    for (auto& [u, v, w] : edges) {
        w = (w % mod + mod) % mod;
        if (u < n - 1) a[u][u].first = (a[u][u].first + 1) % mod, a[u][u].second = (a[u][u].second + w) % mod;
        if (v < n - 1) a[v][v].first = (a[v][v].first + 1) % mod, a[v][v].second = (a[v][v].second + w) % mod;
        if (u < n - 1 && v < n - 1) {
            a[u][v].first = (a[u][v].first - 1 + mod) % mod, a[u][v].second = (a[u][v].second - w + mod) % mod;
            a[v][u].first = (a[v][u].first - 1 + mod) % mod, a[v][u].second = (a[v][u].second - w + mod) % mod;
        }
    }
    return det_linear(a, mod).second;
}