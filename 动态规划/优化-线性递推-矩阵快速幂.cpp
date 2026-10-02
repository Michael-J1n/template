#include <bits/stdc++.h>
using namespace std;

const int mod = 998244353;

/**
 * 矩阵快速幂求线性状态转移第 n 项
 *
 * 状态定义：
 * 给定初始状态 V[0] 和转移矩阵 M，V[i] = M * V[i - 1]
 * 则：V[n] = M^n * V[0]
 *
 * 参数：
 * V 表示初始状态 V[0]；M 表示状态转移矩阵；n 表示要求 V[n]
 *
 * 复杂度：O(k^3logn)，其中 k 为矩阵维数
 */
vector<long long> matrixRecurrence(vector<long long> V, vector<vector<long long>> M, long long n, int mod) {
    int k = V.size();
    auto mul_mat = [&](const vector<vector<long long>>& A, const vector<vector<long long>>& B) -> vector<vector<long long>> {
        vector<vector<long long>> C(k, vector<long long>(k));
        for (int i = 0; i < k; ++i) {
            for (int t = 0; t < k; ++t) {
                if (!A[i][t]) continue;
                long long x = A[i][t];
                for (int j = 0; j < k; ++j) {
                    C[i][j] = (C[i][j] + x * B[t][j]) % mod;
                }
            }
        }
        return C;
    };
    auto mul_vec = [&](const vector<vector<long long>>& A, const vector<long long>& B) -> vector<long long> {
        vector<long long> C(k);
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                C[i] = (C[i] + A[i][j] * B[j]) % mod;
            }
        }
        return C;
    };
    while (n) {
        if (n & 1) V = mul_vec(M, V);
        n >>= 1;
        if (n) M = mul_mat(M, M);
    }
    return V;
}

/**
 * 矩阵快速幂求线性递推第 n 项
 *
 * 状态定义：
 * 给定初值 a[0..k-1] 和递推系数 c[0..k-1]
 * f[i] = c[0] * f[i - 1] + c[1] * f[i - 2] + ... + c[k - 1] * f[i - k]
 *
 * 矩阵构造：
 * V[i] = [f[i], f[i - 1], ..., f[i - k + 1]]
 * V[i] = M * V[i - 1]
 * M 第一行是 c[0..k-1]，下面每行把上一维平移下来
 * 
 * 参数：
 * a[i] 表示 f[i]，即为从先到后的初值
 * c[i] 表示从近到远的递推系数
 * n 表示要求 f[n]
 *
 * 复杂度：
 * O(k^3logn)
 */
int linearRecurrence(vector<long long> a, vector<long long> c, long long n, int mod, bool need_norm = false) {
    int k = a.size();
    auto norm = [&](long long x) -> int {
        x %= mod;
        if (x < 0) x += mod;
        return x;
    };
    if (need_norm) {
        for (auto& x : a) x = norm(x);
        for (auto& x : c) x = norm(x);
    }
    if (n < k) return a[n];
    vector M(k, vector<int>(k));
    for (int i = 0; i < k; ++i) M[0][i] = c[i];
    for (int i = 1; i < k; ++i) M[i][i - 1] = 1;
    auto mul_mat = [&](const vector<vector<int>>& A, const vector<vector<int>>& B) -> vector<vector<int>> {
        vector C(k, vector<int>(k));
        for (int i = 0; i < k; ++i) {
            for (int t = 0; t < k; ++t) {
                if (!A[i][t]) continue;
                long long x = A[i][t];
                for (int j = 0; j < k; ++j) {
                    C[i][j] = (C[i][j] + x * B[t][j]) % mod;
                }
            }
        }
        return C;
    };
    auto mul_vec = [&](const vector<vector<int>>& A, const vector<int>& B) -> vector<int> {
        vector<int> C(k);
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                C[i] = (C[i] + 1LL * A[i][j] * B[j]) % mod;
            }
        }
        return C;
    };
    vector<int> res(k);
    for (int i = 0; i < k; ++i) res[i] = a[k - 1 - i];
    long long b = n - k + 1;
    while (b) {
        if (b & 1) res = mul_vec(M, res);
        b >>= 1;
        if (b) M = mul_mat(M, M);
    }
    return res[0];
}

/**
 * 例如斐波那契：
    vector<int> a = {0, 1};
    vector<int> c = {1, 1};
    cout << linearRecurrence(a, c, n) << '\n';
 */


/**
 * 若线性递推中有常数：假设递推常数为 d；
 * f[i] = c[0] * f[i - 1] + ... + c[k - 1] * f[i - k] + d
 * 则额外增加一维恒为 1：
 * V[i] = [f[i], f[i - 1], ..., f[i - k + 1], 1]
 * M 第一行最后加入 d，最后一行最后一个位置为 1
 */
int linearRecurrence2(vector<long long> a, vector<long long> c, long long d, long long n, int mod, bool need_norm = false) {
    int k = a.size();
    auto norm = [&](long long x) -> int {
        x %= mod;
        if (x < 0) x += mod;
        return x;
    };
    if (need_norm) {
        for (auto& x : a) x = norm(x);
        for (auto& x : c) x = norm(x);
        d = norm(d);
    }
    if (n < k) return a[n];
    int K = k + 1;
    vector M(K, vector<int>(K));
    for (int i = 0; i < k; ++i) M[0][i] = c[i];
    M[0][k] = d;
    for (int i = 1; i < k; ++i) M[i][i - 1] = 1;
    M[k][k] = 1;
    auto mul_mat = [&](const vector<vector<int>>& A, const vector<vector<int>>& B) -> vector<vector<int>> {
        vector C(K, vector<int>(K));
        for (int i = 0; i < K; ++i) {
            for (int t = 0; t < K; ++t) {
                if (!A[i][t]) continue;
                long long x = A[i][t];
                for (int j = 0; j < K; ++j) {
                    C[i][j] = (C[i][j] + x * B[t][j]) % mod;
                }
            }
        }
        return C;
    };
    auto mul_vec = [&](const vector<vector<int>>& A, const vector<int>& B) -> vector<int> {
        vector<int> C(K);
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < K; ++j) {
                C[i] = (C[i] + 1LL * A[i][j] * B[j]) % mod;
            }
        }
        return C;
    };
    vector<int> res(K);
    for (int i = 0; i < k; ++i) res[i] = a[k - 1 - i];
    res[k] = 1;
    long long b = n - k + 1;
    while (b) {
        if (b & 1) res = mul_vec(M, res);
        b >>= 1;
        if (b) M = mul_mat(M, M);
    }
    return res[0];
}