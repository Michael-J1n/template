#include <bits/stdc++.h>
using namespace std;

// 前置条件：输入值与 threshold 均非负；最大值必须小到足以分配并筛选 O(max(a)) 的数组

struct UFS {
    int n, cc;
    vector<int> pa, sz;
    UFS(int _n) : n(_n), cc(_n) {
        pa.resize(n), sz.resize(n);
        for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1;
    }
    int find(int x) {
        if (pa[x] != x) pa[x] = find(pa[x]);
        return pa[x];
    }
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (sz[px] < sz[py]) swap(px, py);
        pa[py] = px;
        sz[px] += sz[py], sz[py] = 0, --cc;
    }
    bool check(int x, int y) {
        return find(x) == find(y);
    }
    int size(int x) {
        return sz[find(x)];
    }
};

/**
 * 最小质因子筛
 * spf[x] 表示 x 的最小质因子
 *
 * 复杂度：
 * O(V log log V)
 */
vector<int> getSPF(int x) {
    vector<int> spf(x + 1);
    for (int i = 0; i <= x; ++i) spf[i] = i;
    for (int i = 2; i * i <= x; ++i) {
        if (spf[i] != i) continue;
        for (int j = i * i; j <= x; j += i) {
            if (spf[j] == j) spf[j] = i;
        }
    }
    return spf;
}

/**
 * gcd 下标并查集；按公共质因子连通
 *
 * 点是数组下标 i；若 gcd(a[i], a[j]) > 1，则 i 和 j 连通
 *
 * 复杂度：
 * O(V log log V + n log V)
 */
UFS gcdIndexUFS(vector<int>& a) {
    int n = a.size(), mx = 0;
    for (auto& x : a) mx = max(mx, x);
    vector<int> spf = getSPF(mx);
    vector<int> first(mx + 1, -1);
    UFS s(n);
    for (int i = 0; i < n; ++i) {
        int x = a[i];
        while (x > 1) {    // 每次取出最小质因子合并
            int p = spf[x];
            if (first[p] == -1) first[p] = i;
            else s.unite(i, first[p]);
            while (x % p == 0) x /= p;
        }
    }
    return s;
}

/**
 * gcd 阈值下标并查集；按公共因子 > threshold 连通
 *
 * 点是数组下标 i；若 gcd(a[i], a[j]) > threshold，则 i 和 j 联通
 *
 * 复杂度：
 * O(V log V + n)
 */
UFS gcdIndexThresholdUFS(vector<int>& a, int threshold) {
    int n = a.size(), mx = 0;
    for (auto& x : a) mx = max(mx, x);
    UFS s(n);
    if (threshold >= mx) return s;
    vector<int> id(mx + 1, -1);
    for (int i = 0; i < n; ++i) {
        if (a[i] <= threshold) continue;
        if (id[a[i]] == -1) id[a[i]] = i;
        else s.unite(i, id[a[i]]);
    }
    for (int d = threshold + 1; d <= mx; ++d) {  // 枚举大于阈值的 d | a[i], d | a[j]
        int first = -1;
        for (int x = d; x <= mx; x += d) {
            if (id[x] == -1) continue;
            if (first == -1) first = id[x];
            else s.unite(first, id[x]);
        }
    }
    return s;
}

/**
 * gcd 值域并查集；按 gcd(x, y) > threshold 连通
 *
 * 点是原值 x；若 gcd(x, y) > threshold，则 x 和 y 连通
 *
 * 复杂度：
 * O(V log V)
 */
UFS gcdValueUFS(int mx, int threshold) {
    UFS s(mx + 1);
    for (int d = threshold + 1; d <= mx; ++d) {
        for (int x = d + d; x <= mx; x += d) s.unite(d, x);
    }
    return s;
}
