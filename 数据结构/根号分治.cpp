#include <bits/stdc++.h>
using namespace std;

/**
 * 根号分治
 *
 * 根号分治的基本思想：当对象具有“大小、出现次数、度数”等规模属性，且所有对象的总规模为 N 时，取阈值 B = sqrt(N)，将对象分成小对象和大对象。
 * 小对象规模 < B，因此可以直接暴力处理；
 * 大对象规模 >= B，因此数量最多只有 N / B = O(sqrt(N)) 个，可以对它们进行额外预处理。
 * 例如涉及到与步长 x 相关时，>B 的步长询问访问位置只有 n/B 个，而小步长可以暴力预处理维护每个步长对应的答案。
 * 
 * 基本处理手法：小对象利用“单次处理代价小”，大对象利用“总数量少”。通常对大对象预处理答案、建立 bitset 或维护额外信息，而小对象在查询时直接枚举。取 B = sqrt(N) 后，可以同时控制小对象的暴力规模和大对象的数量。
 *
 * 本例：查询两个静态集合 |S_x ∩ S_y|，只给大集合建立 bitset。
 * 大-大用 bitset 按位与求交；大-小枚举小集合并查询 bitset；小-小直接双指针求交。
 *
 * 复杂度：预处理 O(N)，大-大查询 O(U / 64)，其余查询 O(sqrt(N))。
 * 空间复杂度：O(N + U sqrt(N) / 64) 个 64 位整数。
 *
 * 前置条件：集合静态不修改，元素编号在 [0, MXV)，每个集合内部元素无重复。
 */
const int MXV = 200005;
vector<int> set_intersection(vector<vector<int>>& a, vector<pair<int, int>>& qs) {
    int n = a.size(), N = 0;
    for (auto& v : a) {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
        N += v.size();
    }
    int B = sqrt(N) + 1;
    vector<int> id(n, -1), heavy;
    for (int i = 0; i < n; ++i) {
        if (a[i].size() >= B) id[i] = heavy.size(), heavy.emplace_back(i);
    }
    vector<bitset<MXV>> bs(heavy.size());
    for (int i = 0; i < heavy.size(); ++i) {
        for (int x : a[heavy[i]]) bs[i][x] = 1;
    }
    vector<int> res;
    for (auto [x, y] : qs) {
        // 均为重集合，调用比特集求交；O(MXV / w)
        if (id[x] != -1 && id[y] != -1) {
            res.emplace_back((bs[id[x]] & bs[id[y]]).count());
            continue;
        }
        // 重集合(x)+轻集合(y)，暴力枚举轻集+比特集查存在性；O(B) = O(sqrt(N))
        if (id[x] != -1 || id[y] != -1) {
            if (id[x] == -1) swap(x, y);
            int cur = 0;
            for (int v : a[y]) cur += bs[id[x]][v];
            res.emplace_back(cur);
            continue;
        }
        // 均为轻集合，由于集合预先排序，暴力双指针求交；O(B) = O(sqrt(N))
        int i = 0, j = 0, cur = 0;
        while (i < a[x].size() && j < a[y].size()) {
            if (a[x][i] == a[y][j]) ++cur, ++i, ++j;
            else if (a[x][i] < a[y][j]) ++i;
            else ++j;
        }
        res.emplace_back(cur);
    }
    return res;
}

/**
 * 例2：无向图中支持点权增加，以及查询一个点所有邻居的点权和。
 * 度数 < B 的点为轻点，查询时直接枚举邻居；度数 >= B 的点为重点，提前维护其所有邻居的点权和。由于总度数 2m，因此重点只有 O(m/B) 个。取 B=sqrt(M)
 *
 * 复杂度：预处理 O(n + m)，单次修改 O(sqrt(m))，单次查询 O(sqrt(m))。
 * 重点查询实际为 O(1)，轻点查询为 O(deg(u)) < O(sqrt(m))。
 *
 * 前置条件：无向简单图，点编号为 [0, n)。
 * qs 中 (1, u, x) 表示 val[u] += x，(2, u, 0) 表示查询 u 的邻居点权和。
 */
vector<long long> degree_sqrt(vector<long long> val, vector<vector<int>>& g, int m, vector<tuple<int, int, long long>>& qs) {
    int n = g.size();
    int B = sqrt(2 * m) + 1;
    vector<int> id(n, -1), heavy;
    for (int u = 0; u < n; ++u) {
        if (g[u].size() >= B) id[u] = heavy.size(), heavy.emplace_back(u);
    }
    vector<vector<int>> heavy_nei(n);
    for (int u = 0; u < n; ++u) {
        for (int v : g[u]) {
            if (id[v] != -1) heavy_nei[u].emplace_back(id[v]);
        }
    }
    vector<long long> sum(heavy.size());
    for (int i = 0; i < heavy.size(); ++i) {
        for (int v : g[heavy[i]]) sum[i] += val[v];
    }
    vector<long long> res;
    for (auto [op, u, x] : qs) {
        if (op == 1) {
            val[u] += x;
            for (int i : heavy_nei[u]) sum[i] += x;
        } else {
            if (id[u] != -1) {
                res.emplace_back(sum[id[u]]);
            } else {
                long long cur = 0;
                for (int v : g[u]) cur += val[v];
                res.emplace_back(cur);
            }
        }
    }
    return res;
}