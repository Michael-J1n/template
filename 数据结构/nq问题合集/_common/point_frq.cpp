#include <bits/stdc++.h>
using namespace std;

/*
PointRangeFrq：单点修改 + 区间频率表 / 众数 / 不同值个数，0-based，闭区间 [l,r]。

接口：
1. set(p, v)：a[p] = v。
2. add(p, v)：a[p] += v。
3. freq(l, r)：返回区间频率表。
4. count_eq(l, r, x)：区间内 x 的出现次数。
5. distinct(l, r)：不同值个数。
6. mode(l, r)：众数；次数相同时取值最小的那个。

复杂度：
1. set/add/count_eq：O(logn) 均摊。
2. freq/distinct/mode：O(logn + 答案不同值个数)。
*/

struct PointRangeFrq {
    using ll = long long;
    struct Hash {
        static uint64_t splitmix64(uint64_t x) {
            x += 0x9e3779b97f4a7c15ull;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
            x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
            return x ^ (x >> 31);
        }
        size_t operator()(ll x) const {
            static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64((uint64_t)x + FIXED_RANDOM);
        }
    };
    int n;
    vector<ll> a;
    vector<unordered_map<ll, int, Hash>> tr;
    mutable unordered_map<ll, int, Hash> work;

    PointRangeFrq() {}
    PointRangeFrq(const vector<ll>& nums) {
        init(nums);
    }
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        tr.assign(n * 4 + 5, {});
        work.clear();
        work.max_load_factor(0.7);
        if (!n) return;
        build(1, 0, n - 1);
    }
    void build(int u, int l, int r) {
        auto& mp = tr[u];
        mp.max_load_factor(0.7);
        if (l == r) {
            mp.reserve(1);
            mp[a[l]] = 1;
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid + 1, r);
        auto& L = tr[u << 1];
        auto& R = tr[u << 1 | 1];
        mp.reserve(L.size() + R.size() + 5);
        if (L.size() >= R.size()) {
            mp.insert(L.begin(), L.end());
            for (auto& p : R) mp[p.first] += p.second;
        } else {
            mp.insert(R.begin(), R.end());
            for (auto& p : L) mp[p.first] += p.second;
        }
    }
    void put(unordered_map<ll, int, Hash>& mp, ll x, int v) {
        auto it = mp.find(x);
        if (it == mp.end()) {
            if (v > 0) mp[x] = v;
            return;
        }
        it->second += v;
        if (it->second == 0) mp.erase(it);
    }
    void add_key(int u, int l, int r, int p, ll x, int v) {
        put(tr[u], x, v);
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (p <= mid) add_key(u << 1, l, mid, p, x, v);
        else add_key(u << 1 | 1, mid + 1, r, p, x, v);
    }
    void set(int p, ll v) {
        if (p < 0 || p >= n) return;
        if (a[p] == v) return;
        add_key(1, 0, n - 1, p, a[p], -1);
        a[p] = v;
        add_key(1, 0, n - 1, p, a[p], 1);
    }
    void add(int p, ll v) {
        if (p < 0 || p >= n || !v) return;
        set(p, a[p] + v);
    }
    void collect_nodes(int u, int l, int r, int ql, int qr, int* nodes, int& cnt) const {
        if (ql <= l && r <= qr) {
            nodes[cnt++] = u;
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) collect_nodes(u << 1, l, mid, ql, qr, nodes, cnt);
        if (qr > mid) collect_nodes(u << 1 | 1, mid + 1, r, ql, qr, nodes, cnt);
    }
    int count_eq(int l, int r, ll x) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int nodes[128], cnt = 0, res = 0;
        collect_nodes(1, 0, n - 1, l, r, nodes, cnt);
        for (int i = 0; i < cnt; ++i) {
            auto it = tr[nodes[i]].find(x);
            if (it != tr[nodes[i]].end()) res += it->second;
        }
        return res;
    }
    int reserve_hint(const int* nodes, int cnt, int len) const {
        long long res = 0;
        for (int i = 0; i < cnt; ++i) res += tr[nodes[i]].size();
        return (int)min<long long>(res, len);
    }
    void fill_freq(const int* nodes, int cnt, unordered_map<ll, int, Hash>& res) const {
        for (int i = 0; i < cnt; ++i) {
            for (auto& [x, c] : tr[nodes[i]]) res[x] += c;
        }
    }
    unordered_map<ll, int, Hash> freq(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        unordered_map<ll, int, Hash> res;
        if (l > r) return res;
        int nodes[128], cnt = 0;
        collect_nodes(1, 0, n - 1, l, r, nodes, cnt);
        res.max_load_factor(0.7);
        res.reserve(reserve_hint(nodes, cnt, r - l + 1) + 5);
        fill_freq(nodes, cnt, res);
        return res;
    }
    int distinct(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int nodes[128], cnt = 0;
        collect_nodes(1, 0, n - 1, l, r, nodes, cnt);
        work.clear();
        work.reserve(reserve_hint(nodes, cnt, r - l + 1) + 5);
        fill_freq(nodes, cnt, work);
        return work.size();
    }
    pair<ll, int> mode_pair(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return {0, 0};
        int nodes[128], cnt = 0;
        collect_nodes(1, 0, n - 1, l, r, nodes, cnt);
        work.clear();
        work.reserve(reserve_hint(nodes, cnt, r - l + 1) + 5);
        fill_freq(nodes, cnt, work);
        pair<ll, int> res = {0, 0};
        for (auto& [x, c] : work) {
            if (c > res.second || (c == res.second && x < res.first)) res = {x, c};
        }
        return res;
    }
    ll mode(int l, int r) const {
        return mode_pair(l, r).first;
    }
};
