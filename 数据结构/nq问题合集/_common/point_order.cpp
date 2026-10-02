#include <bits/stdc++.h>
using namespace std;

/*
PointRangeOrder：单点修改 + 区间 kth / median / cnt[<=x] / 值域计数，0-based，闭区间 [l,r]。

线段树套手写 Treap，每个线段树节点维护 pair<value,index> 的平衡树。
不依赖 PBDS，适合 Codeforces / AtCoder / LeetCode / Luogu 这类环境直接改题使用。

接口：
1. set(p, v)：a[p] = v。
2. add(p, v)：a[p] += v。
3. count_le(l, r, x)：区间 <= x 个数。
4. count_between(l, r, vL, vR)：区间值域 [vL,vR] 个数。
5. kth_smallest / kth_largest / median。

复杂度：
1. set/add/count：O(log^2 n)。
2. kth：O(logV * log^2 n)。
*/

struct PointRangeOrder {
    using ll = long long;
    using Key = pair<ll, int>;

    struct Node {
        int l, r, pri, sz;
        Key v;
    };

    int n;
    vector<ll> a;
    vector<int> rt, trash;
    vector<Node> t;
    multiset<ll> allVals;
    uint64_t seed = 88172645463325252ull;

    PointRangeOrder() {}
    PointRangeOrder(const vector<ll>& nums) {
        init(nums);
    }
    inline uint32_t rng() {
        seed += 0x9e3779b97f4a7c15ull;
        uint64_t z = seed;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
        z ^= (z >> 31);
        return (uint32_t)z;
    }
    inline int sz(int u) const {
        return u ? t[u].sz : 0;
    }
    inline void pull(int u) {
        t[u].sz = sz(t[u].l) + sz(t[u].r) + 1;
    }
    int newNode(Key v) {
        if (!trash.empty()) {
            int u = trash.back();
            trash.pop_back();
            t[u] = {0, 0, (int)rng(), 1, v};
            return u;
        }
        t.push_back({0, 0, (int)rng(), 1, v});
        return (int)t.size() - 1;
    }
    int rotL(int u) {
        int p = t[u].r;
        t[u].r = t[p].l, t[p].l = u;
        pull(u), pull(p);
        return p;
    }
    int rotR(int u) {
        int p = t[u].l;
        t[u].l = t[p].r, t[p].r = u;
        pull(u), pull(p);
        return p;
    }
    int insert(int u, Key x) {
        if (!u) return newNode(x);
        if (x < t[u].v) {
            t[u].l = insert(t[u].l, x);
            if (t[t[u].l].pri < t[u].pri) u = rotR(u);
        } else {
            t[u].r = insert(t[u].r, x);
            if (t[t[u].r].pri < t[u].pri) u = rotL(u);
        }
        pull(u);
        return u;
    }
    int merge(int x, int y) {
        if (!x || !y) return x ? x : y;
        if (t[x].pri < t[y].pri) {
            t[x].r = merge(t[x].r, y);
            pull(x);
            return x;
        } else {
            t[y].l = merge(x, t[y].l);
            pull(y);
            return y;
        }
    }
    void eraseOne(int& u, Key x) {
        if (!u) return;
        if (t[u].v == x) {
            int old = u;
            u = merge(t[u].l, t[u].r);
            trash.emplace_back(old);
            return;
        }
        if (x < t[u].v) eraseOne(t[u].l, x);
        else eraseOne(t[u].r, x);
        if (u) pull(u);
    }
    int order_key(int u, Key x) const {
        int res = 0;
        while (u) {
            if (x <= t[u].v) u = t[u].l;
            else {
                res += sz(t[u].l) + 1;
                u = t[u].r;
            }
        }
        return res;
    }
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        rt.assign(n * 4 + 5, 0);
        trash.clear();
        t.clear();
        t.resize(1);
        if (n) t.reserve((long long)n * (32 - __builtin_clz(n)) + 5);
        allVals.clear();
        for (int i = 0; i < n; ++i) {
            insert_path(1, 0, n - 1, i, a[i]);
            allVals.insert(a[i]);
        }
    }
    void insert_path(int u, int l, int r, int p, ll v) {
        rt[u] = insert(rt[u], {v, p});
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (p <= mid) insert_path(u << 1, l, mid, p, v);
        else insert_path(u << 1 | 1, mid + 1, r, p, v);
    }
    void erase_path(int u, int l, int r, int p, ll v) {
        eraseOne(rt[u], {v, p});
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (p <= mid) erase_path(u << 1, l, mid, p, v);
        else erase_path(u << 1 | 1, mid + 1, r, p, v);
    }
    void set(int p, ll v) {
        if (p < 0 || p >= n) return;
        if (a[p] == v) return;
        erase_path(1, 0, n - 1, p, a[p]);
        allVals.erase(allVals.find(a[p]));
        a[p] = v;
        allVals.insert(a[p]);
        insert_path(1, 0, n - 1, p, a[p]);
    }
    void add(int p, ll v) {
        if (p < 0 || p >= n || !v) return;
        set(p, a[p] + v);
    }
    void collect_roots(int u, int l, int r, int ql, int qr, int* roots, int& rn) const {
        if (ql <= l && r <= qr) {
            roots[rn++] = rt[u];
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) collect_roots(u << 1, l, mid, ql, qr, roots, rn);
        if (qr > mid) collect_roots(u << 1 | 1, mid + 1, r, ql, qr, roots, rn);
    }
    int count_roots(const int* roots, int rn, Key x) const {
        int res = 0;
        for (int i = 0; i < rn; ++i) res += order_key(roots[i], x);
        return res;
    }
    int count_le(int u, int l, int r, int ql, int qr, ll x) const {
        if (ql <= l && r <= qr) return order_key(rt[u], {x, INT_MAX});
        int mid = (l + r) >> 1, res = 0;
        if (ql <= mid) res += count_le(u << 1, l, mid, ql, qr, x);
        if (qr > mid) res += count_le(u << 1 | 1, mid + 1, r, ql, qr, x);
        return res;
    }
    int count_less(int u, int l, int r, int ql, int qr, ll x) const {
        if (ql <= l && r <= qr) return order_key(rt[u], {x, -1});
        int mid = (l + r) >> 1, res = 0;
        if (ql <= mid) res += count_less(u << 1, l, mid, ql, qr, x);
        if (qr > mid) res += count_less(u << 1 | 1, mid + 1, r, ql, qr, x);
        return res;
    }
    int count_le(int l, int r, ll x) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int roots[128], rn = 0;
        collect_roots(1, 0, n - 1, l, r, roots, rn);
        return count_roots(roots, rn, {x, INT_MAX});
    }
    int count_less(int l, int r, ll x) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int roots[128], rn = 0;
        collect_roots(1, 0, n - 1, l, r, roots, rn);
        return count_roots(roots, rn, {x, -1});
    }
    int count_between(int l, int r, ll vL, ll vR) const {
        if (vL > vR) return 0;
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int roots[128], rn = 0;
        collect_roots(1, 0, n - 1, l, r, roots, rn);
        return count_roots(roots, rn, {vR, INT_MAX}) - count_roots(roots, rn, {vL, -1});
    }
    ll kth_smallest(int l, int r, int k) const {
        l = max(l, 0), r = min(r, n - 1);
        int len = r - l + 1;
        if (l > r || k < 1 || k > len || allVals.empty()) return 0;
        int roots[128], rn = 0;
        collect_roots(1, 0, n - 1, l, r, roots, rn);
        ll L = *allVals.begin(), R = *allVals.rbegin();
        while (L < R) {
            ll mid = L + (ll)((__int128)R - L) / 2;
            if (count_roots(roots, rn, {mid, INT_MAX}) >= k) R = mid;
            else L = mid + 1;
        }
        return L;
    }
    ll kth_largest(int l, int r, int k) const {
        l = max(l, 0), r = min(r, n - 1);
        int len = r - l + 1;
        if (l > r || k < 1 || k > len) return 0;
        return kth_smallest(l, r, len - k + 1);
    }
    ll median(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        return kth_smallest(l, r, (r - l + 2) / 2);
    }
};
