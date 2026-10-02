#include "point_order.cpp"

/*
PointDistinct：单点修改 + 区间不同值个数，0-based，闭区间 [l,r]。

维护每个值出现位置集合，以及每个位置 i 的 pre[i] = 同值上一次出现位置。
distinct(l,r) = count of i in [l,r] with pre[i] < l。
pre 数组用 PointRangeOrder 维护，即动态二维数点。

复杂度：
1. set(p, v)：O(log^2 n)。
2. distinct(l, r)：O(log^2 n)。
*/

struct PointDistinct {
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
    unordered_map<ll, set<int>, Hash> pos;
    PointRangeOrder ord;

    PointDistinct() {}
    PointDistinct(const vector<ll>& nums) {
        init(nums);
    }
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        pos.clear();
        pos.reserve(n * 2 + 5);
        pos.max_load_factor(0.7);
        vector<ll> pre(n, -1);
        unordered_map<ll, int, Hash> last;
        last.reserve(n * 2 + 5);
        last.max_load_factor(0.7);
        for (int i = 0; i < n; ++i) {
            auto it = last.find(a[i]);
            if (it != last.end()) pre[i] = it->second;
            last[a[i]] = i;
            pos[a[i]].insert(i);
        }
        ord.init(pre);
    }
    void set(int p, ll v) {
        if (p < 0 || p >= n || a[p] == v) return;
        ll old = a[p];
        auto& so = pos[old];
        auto it = so.find(p);
        int pre = it == so.begin() ? -1 : *prev(it);
        int nxt = next(it) == so.end() ? -1 : *next(it);
        so.erase(it);
        if (nxt != -1) ord.set(nxt, pre);

        auto& sn = pos[v];
        auto jt = sn.lower_bound(p);
        pre = jt == sn.begin() ? -1 : *prev(jt);
        nxt = jt == sn.end() ? -1 : *jt;
        sn.insert(p);
        ord.set(p, pre);
        if (nxt != -1) ord.set(nxt, p);
        a[p] = v;
    }
    void add(int p, ll v) {
        if (p < 0 || p >= n) return;
        set(p, a[p] + v);
    }
    int distinct(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        return ord.count_le(l, r, l - 1);
    }
};
