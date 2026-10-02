#include <bits/stdc++.h>
using namespace std;

/*
StaticDistinct：静态区间不同值个数，0-based，闭区间 [l,r]。

HH 的项链 trick：
按右端点离线排序，树状数组只在每个值最后一次出现的位置放 1。

复杂度：O((n+q)logn)，比莫队更适合单纯 distinct count。
*/

struct StaticDistinct {
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
    struct BIT {
        int n;
        vector<int> c;
        BIT() {}
        BIT(int _n) { init(_n); }
        void init(int _n) {
            n = _n;
            c.assign(n + 1, 0);
        }
        void add(int p, int v) {
            for (++p; p <= n; p += p & -p) c[p] += v;
        }
        int pres(int p) const {
            int s = 0;
            for (++p; p > 0; p &= p - 1) s += c[p];
            return s;
        }
        int query(int l, int r) const {
            return pres(r) - (l ? pres(l - 1) : 0);
        }
    };

    int n;
    vector<ll> a, vals;
    vector<int> ca;
    vector<Query> qs;

    StaticDistinct() {}
    StaticDistinct(const vector<ll>& nums) {
        init(nums);
    }
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        ca.resize(n);
        for (int i = 0; i < n; ++i) ca[i] = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin();
        qs.clear();
    }
    int add_query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        int id = qs.size();
        if (l > r) qs.push_back({0, -1, id});
        else qs.push_back({l, r, id});
        return id;
    }
    vector<int> solve() const {
        vector<Query> ord = qs;
        sort(ord.begin(), ord.end(), [](const Query& x, const Query& y) {
            return x.r < y.r;
        });
        vector<int> ans(qs.size(), 0);
        vector<int> last(vals.size(), -1);
        BIT bit(n);
        int cur = 0;
        while (cur < (int)ord.size() && ord[cur].r < 0) ++cur;
        for (int i = 0; i < n; ++i) {
            int x = ca[i];
            if (last[x] != -1) bit.add(last[x], -1);
            bit.add(i, 1);
            last[x] = i;
            while (cur < (int)ord.size() && ord[cur].r == i) {
                if (ord[cur].l <= ord[cur].r) ans[ord[cur].id] = bit.query(ord[cur].l, ord[cur].r);
                ++cur;
            }
        }
        return ans;
    }
    vector<int> solve_distinct() const {
        return solve();
    }
};
