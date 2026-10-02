#include <bits/stdc++.h>
using namespace std;

/*
StaticMoFrq：静态区间频率表 / 众数 / 不同值个数，0-based，闭区间 [l,r]。

用法：
1. add_query(l, r) 加入询问，返回 id。
2. solve_tables() 返回每个询问的频率表 unordered_map<long long,int>。
3. solve_distinct() 返回不同值个数。
4. solve_mode() 返回众数；次数相同时取值最小的那个。

复杂度：莫队 O((n+q)sqrt n)，频率表输出还要额外加上答案表大小。
*/

struct StaticMoFrq {
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
    struct Query {
        int l, r, id;
    };
    int n, B;
    vector<ll> a, vals;
    vector<int> ca;
    vector<Query> qs;
    mutable vector<Query> ordCache;
    mutable bool ordDirty = true;

    StaticMoFrq() {}
    StaticMoFrq(const vector<ll>& nums) {
        init(nums);
    }
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        B = max(1, isqrt(max(1, n)));
        vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        ca.resize(n);
        for (int i = 0; i < n; ++i) ca[i] = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin();
        qs.clear();
        ordCache.clear();
        ordDirty = true;
    }
    int isqrt(int x) const {
        int l = 0, r = min(x, 46340), ans = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (1LL * mid * mid <= x) ans = mid, l = mid + 1;
            else r = mid - 1;
        }
        return ans;
    }
    int add_query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        int id = qs.size();
        if (l > r) qs.push_back({0, -1, id});
        else qs.push_back({l, r, id});
        ordDirty = true;
        return id;
    }
    const vector<Query>& order() const {
        if (!ordDirty) return ordCache;
        ordCache = qs;
        int BB = max(1, n / max(1, isqrt(max(1, (int)qs.size()))));
        sort(ordCache.begin(), ordCache.end(), [&](const Query& x, const Query& y) {
            int bx = x.l / BB, by = y.l / BB;
            if (bx != by) return bx < by;
            if (bx & 1) return x.r > y.r;
            return x.r < y.r;
        });
        ordDirty = false;
        return ordCache;
    }
    vector<int> solve_distinct() const {
        vector<int> res(qs.size());
        vector<int> cnt(vals.size(), 0);
        int l = 0, r = -1, cur = 0;
        auto add = [&](int p) {
            if (cnt[ca[p]]++ == 0) ++cur;
        };
        auto del = [&](int p) {
            if (--cnt[ca[p]] == 0) --cur;
        };
        for (auto& q : order()) {
            while (r < q.r) add(++r);
            while (l > q.l) add(--l);
            while (r > q.r) del(r--);
            while (l < q.l) del(l++);
            res[q.id] = cur;
        }
        return res;
    }
    vector<unordered_map<ll, int, Hash>> solve_tables() const {
        vector<unordered_map<ll, int, Hash>> res(qs.size());
        vector<int> cnt(vals.size(), 0);
        vector<int> active, pos(vals.size(), -1);
        active.reserve(vals.size());
        int l = 0, r = -1;
        auto add = [&](int p) {
            int x = ca[p];
            if (cnt[x]++ == 0) {
                pos[x] = active.size();
                active.emplace_back(x);
            }
        };
        auto del = [&](int p) {
            int x = ca[p];
            if (--cnt[x] == 0) {
                int p = pos[x], y = active.back();
                active[p] = y;
                pos[y] = p;
                active.pop_back();
                pos[x] = -1;
            }
        };
        for (auto& q : order()) {
            while (r < q.r) add(++r);
            while (l > q.l) add(--l);
            while (r > q.r) del(r--);
            while (l < q.l) del(l++);
            auto& mp = res[q.id];
            mp.max_load_factor(0.7);
            mp.reserve(active.size() + 5);
            for (int x : active) mp[vals[x]] = cnt[x];
        }
        return res;
    }
    vector<ll> solve_mode() const {
        vector<ll> res(qs.size(), 0);
        vector<int> cnt(vals.size(), 0);
        vector<set<int>> bucket(n + 1);
        int l = 0, r = -1, best = 0;
        auto add = [&](int p) {
            int x = ca[p], c = cnt[x];
            if (c) bucket[c].erase(x);
            ++cnt[x];
            bucket[c + 1].insert(x);
            best = max(best, c + 1);
        };
        auto del = [&](int p) {
            int x = ca[p], c = cnt[x];
            bucket[c].erase(x);
            --cnt[x];
            if (c - 1) bucket[c - 1].insert(x);
            while (best && bucket[best].empty()) --best;
        };
        for (auto& q : order()) {
            while (r < q.r) add(++r);
            while (l > q.l) add(--l);
            while (r > q.r) del(r--);
            while (l < q.l) del(l++);
            if (best) res[q.id] = vals[*bucket[best].begin()];
        }
        return res;
    }
};
