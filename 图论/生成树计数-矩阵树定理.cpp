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
long long det(vector<vector<long long>> a, long long mod) {
    int n = a.size();
    for (auto& row : a) {
        for (auto& x : row) x = (x % mod + mod) % mod;
    }
    long long res = 1;
    for (int i = 0; i < n; ++i) {
        int p = i;
        while (p < n && a[p][i] == 0) ++p;
        if (p == n) return 0;
        if (p != i) {
            swap(a[p], a[i]);
            res = (mod - res) % mod;
        }
        res = res * a[i][i] % mod;
        long long inv = qpow(a[i][i], mod - 2, mod);
        for (int j = i + 1; j < n; ++j) {
            long long k = a[j][i] * inv % mod;
            for (int t = i; t < n; ++t) a[j][t] = (a[j][t] - k * a[i][t] % mod + mod) % mod;
        }
    }
    return res;
}

/**
 * 无向图矩阵树定理 / Matrix-Tree Theorem【无权图生成树计数】
 * 点下标为 0-based，边格式为 {u, v}，mod 为质数。
 *
 * matrix_tree(n, edges, mod)：求无向图生成树数量。
 *
 * 复杂度：O(n^3 + m)。
 */
long long matrix_tree(int n, vector<pair<int, int>>& edges, long long mod) {
    vector<vector<long long>> a(n - 1, vector<long long>(n - 1));
    for (auto& [u, v] : edges) {
        if (u < n - 1) a[u][u] = (a[u][u] + 1) % mod;
        if (v < n - 1) a[v][v] = (a[v][v] + 1) % mod;
        if (u < n - 1 && v < n - 1) a[u][v] = (a[u][v] - 1 + mod) % mod, a[v][u] = (a[v][u] - 1 + mod) % mod;
    }
    return det(a, mod);
}

/**
 * 无向图带权矩阵树定理 / Weighted Matrix-Tree Theorem【带权生成树计数】
 * 点下标为 0-based，边格式为 {u, v, w}，mod 为质数。
 *
 * matrix_tree_weighted(n, edges, mod)：求所有生成树边权乘积之和。
 *
 * 复杂度：O(n^3 + m)。
 */
long long matrix_tree_weighted(int n, vector<tuple<int, int, long long>>& edges, long long mod) {
    vector<vector<long long>> a(n - 1, vector<long long>(n - 1));
    for (auto& [u, v, w] : edges) {
        w = (w % mod + mod) % mod;
        if (u < n - 1) a[u][u] = (a[u][u] + w) % mod;
        if (v < n - 1) a[v][v] = (a[v][v] + w) % mod;
        if (u < n - 1 && v < n - 1) a[u][v] = (a[u][v] - w + mod) % mod, a[v][u] = (a[v][u] - w + mod) % mod;
    }
    return det(a, mod);
}

/**
 * 有向图矩阵树定理 / Directed Matrix-Tree Theorem【内向生成树计数】
 * 点下标为 0-based，边格式为 {u, v}，表示 u -> v。
 *
 * matrix_tree_in(n, edges, r, mod)：求所有以 r 为根的内向生成树数量。
 *
 * 复杂度：O(n^3 + m)。
 */
long long matrix_tree_in(int n, vector<pair<int, int>>& edges, int r, long long mod) {
    vector<vector<long long>> a(n - 1, vector<long long>(n - 1));
    auto id = [&](int x) { return x < r ? x : x - 1; };
    for (auto& [u, v] : edges) {
        if (u != r) a[id(u)][id(u)] = (a[id(u)][id(u)] + 1) % mod;
        if (u != r && v != r) a[id(u)][id(v)] = (a[id(u)][id(v)] - 1 + mod) % mod;
    }
    return det(a, mod);
}

/**
 * 有向图矩阵树定理 / Directed Matrix-Tree Theorem【外向生成树计数】
 * 点下标为 0-based，边格式为 {u, v}，表示 u -> v。
 *
 * matrix_tree_out(n, edges, r, mod)：求所有以 r 为根的外向生成树数量。
 *
 * 复杂度：O(n^3 + m)。
 */
long long matrix_tree_out(int n, vector<pair<int, int>>& edges, int r, long long mod) {
    vector<vector<long long>> a(n - 1, vector<long long>(n - 1));
    auto id = [&](int x) { return x < r ? x : x - 1; };
    for (auto& [u, v] : edges) {
        if (v != r) a[id(v)][id(v)] = (a[id(v)][id(v)] + 1) % mod;
        if (u != r && v != r) a[id(v)][id(u)] = (a[id(v)][id(u)] - 1 + mod) % mod;
    }
    return det(a, mod);
}

/**
 * 有向图带权矩阵树定理 / Directed Matrix-Tree Theorem【内向生成树权值和】
 * 点下标为 0-based，边格式为 {u, v, w}，表示 u -> v，mod 为质数。
 *
 * matrix_tree_in_weighted(n, edges, r, mod)：求所有以 r 为根的内向生成树边权乘积之和。
 *
 * 复杂度：O(n^3 + m)。
 */
long long matrix_tree_in_weighted(int n, vector<tuple<int, int, long long>>& edges, int r, long long mod) {
    vector<vector<long long>> a(n - 1, vector<long long>(n - 1));
    auto id = [&](int x) {
        return x < r ? x : x - 1;
    };
    for (auto& [u, v, w] : edges) {
        w = (w % mod + mod) % mod;
        if (u != r) a[id(u)][id(u)] = (a[id(u)][id(u)] + w) % mod;
        if (u != r && v != r)
            a[id(u)][id(v)] = (a[id(u)][id(v)] - w + mod) % mod;
    }
    return det(a, mod);
}

/**
 * 有向图带权矩阵树定理 / Directed Matrix-Tree Theorem【外向生成树权值和】
 * 点下标为 0-based，边格式为 {u, v, w}，表示 u -> v，mod 为质数。
 *
 * matrix_tree_out_weighted(n, edges, r, mod)：求所有以 r 为根的外向生成树边权乘积之和。
 *
 * 复杂度：O(n^3 + m)。
 */
long long matrix_tree_out_weighted(int n, vector<tuple<int, int, long long>>& edges, int r, long long mod) {
    vector<vector<long long>> a(n - 1, vector<long long>(n - 1));
    auto id = [&](int x) {
        return x < r ? x : x - 1;
    };
    for (auto& [u, v, w] : edges) {
        w = (w % mod + mod) % mod;
        if (v != r) a[id(v)][id(v)] = (a[id(v)][id(v)] + w) % mod;
        if (u != r && v != r)
            a[id(v)][id(u)] = (a[id(v)][id(u)] - w + mod) % mod;
    }
    return det(a, mod);
}