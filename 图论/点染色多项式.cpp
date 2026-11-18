#include <bits/stdc++.h>
using namespace std;

const int mod = 998244353;

struct UFS {
    int n, cc;
    vector<int> pa, sz;
    vector<pair<int, int>> stk;
    UFS(int _n) : n(_n), cc(_n) {
        pa.resize(n), sz.resize(n);
        for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1;
    }
    int find(int x) {
        while (pa[x] != x) x = pa[x];
        return x;
    }
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) {
            stk.push_back({-1, -1});
            return false;
        }
        if (sz[px] < sz[py]) swap(px, py);
        stk.push_back({px, py});
        pa[py] = px;
        sz[px] += sz[py], --cc;
        return true;
    }
    bool check(int x, int y) { return find(x) == find(y); }
    int size(int x) { return sz[find(x)]; }
    int snapshot() { return stk.size(); }
    void undo() {
        auto [px, py] = stk.back();
        stk.pop_back();
        if (px == -1) return;
        sz[px] -= sz[py];
        pa[py] = py;
        ++cc;
    }
    void rollback(int t) {
        while (stk.size() > t) undo();
    }
};

long long qpow(int a, int b) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = 1LL * a * a % mod, b >>= 1;
    }
    return res;
}

/**
 * 计算无向图的染色多项式 P(G,k)，即使用 k 种颜色对点进行合法染色的方案数。
 * 
 * 参数：n 为点数，m 为边数，g 为邻接表，qs 为查询的颜色数量。
 * 返回 ans[i] = P(G,qs[i])
 * 
 * 适用：无向简单图，满足以下条件之一：
 *   1. n <= 15：独立集状压 DP。
 *   2. m <= 24：边子集容斥 + 可撤销并查集。
 *   3. 所有点度数均为 2：图为若干个不相交的环。
 * 
 * 复杂度（假设有 Q 个询问）：
 *   1. O(n * 3^n)
 *   2. O(2^m * log n)
 *   3. O(m * log n)
 */
vector<int> chromatic(int n, int m, vector<vector<int>>& g, vector<int>& qs) {
    vector<pair<int, int>> edges; edges.reserve(m);
    for (int u = 0; u < n; ++u) {
        for (int v : g[u]) {
            if (u < v) edges.emplace_back(u, v);
        }
    }
    vector<int> ans;
    ans.reserve(qs.size());
    if (n <= 15) {
        // 独立集状压；f[mask][t] 为将 mask 中的点划分为 t 个非空独立集的方案数
        // P(G,k) = sum_t f[N - 1][t] * k * (k - 1) * ... * (k - t + 1)
        int N = 1 << n;
        vector<int> adj(n);
        for (int i = 0; i < n; ++i) {
            for (int j : g[i]) adj[i] |= 1 << j;
        }
        vector<int> ind(N, 1);
        for (int mask = 1; mask < N; ++mask) {
            int i = __builtin_ctz(mask);
            ind[mask] = ind[mask ^ (1 << i)] && !(adj[i] & mask);
        }
        vector<vector<long long>> f(N, vector<long long>(n + 1));
        f[0][0] = 1;
        for (int mask = 1; mask < N; ++mask) {
            int i = __builtin_ctz(mask);
            for (int sub = mask; sub; sub = (sub - 1) & mask) {
                if (!(sub >> i & 1) || !ind[sub]) continue;
                for (int t = 1; t <= n; ++t) {
                    (f[mask][t] += f[mask ^ sub][t - 1]) %= mod;
                }
            }
        }
        for (int k : qs) {
            long long res = 0, prod = 1;
            if (n == 0) res = 1;
            for (int t = 1; t <= n; ++t) {
                prod = prod * ((1LL * k - t + 1) % mod + mod) % mod;
                (res += f[N - 1][t] * prod % mod) %= mod;
            }
            ans.emplace_back(res);
        }
    } else if (m <= 24) {
        // 对边容斥；P(G,k) = sum_S (-1)^|S| * k^cc(S)
        // DFS 枚举所有边子集，用可撤销并查集维护连通块
        vector<long long> a(n + 1); a[n] = 1;
        UFS s(n);
        auto dfs = [&](this auto&& dfs, int i, int sgn) -> void {
            for (int j = i; j < m; ++j) {
                s.unite(edges[j].first, edges[j].second);
                a[s.cc] -= sgn;
                dfs(j + 1, -sgn);
                s.undo();
            }
        };
        dfs(0, 1);
        for (int i = 0; i <= n; ++i) a[i] = (a[i] % mod + mod) % mod;
        for (int k : qs) {
            long long res = 0;
            long long x = (1LL * k % mod + mod) % mod;
            for (int i = n; ~i; --i) res = (res * x + a[i]) % mod;
            ans.emplace_back(res);
        }
    } else {
        // 每个连通块都是环；P(Cx,k) = (k - 1)^x + (-1)^x * (k - 1)
        // 不同连通块的染色多项式相乘
        UFS s(n);
        for (int i = 0; i < m; ++i) {
            s.unite(edges[i].first, edges[i].second);
        }
        vector<int> vec;
        vec.reserve(n);
        for (int i = 0; i < n; ++i) {
            if (i == s.find(i)) vec.emplace_back(s.size(i));
        }
        for (int k : qs) {
            long long res = 1, b = (1LL * k - 1) % mod;
            if (b < 0) b += mod;
            for (int x : vec) {
                long long sgn = (x & 1) ? -1 : 1;
                long long cur = (qpow(b, x) + sgn * b) % mod;
                if (cur < 0) cur += mod;
                res = res * cur % mod;
            }
            ans.emplace_back(res);
        }
    }
    return ans;
}