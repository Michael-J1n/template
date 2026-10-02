#include <bits/stdc++.h>
using namespace std;

/*
PointModeMo：单点修改 + 区间众数，离线带修莫队，0-based，闭区间 [l,r]。

用法：
1. PointModeMo mo(a);
2. mo.add_update(p, v)：按时间顺序加入 a[p]=v。
3. mo.add_query(l, r)：按时间顺序加入询问，返回 id。
4. solve_mode()：返回每个询问的众数。

约定：次数相同取值最小的众数。

复杂度：O((n+q) * n^(2/3) * logV) 量级，常数主要在 set 维护频率桶。
*/

struct PointModeMo {
    using ll = long long;
    struct Query {
        int l, r, t, id;
    };
    struct Update {
        int p;
        ll v;
    };
    struct Mod {
        int p, v;
    };

    int n;
    vector<ll> initA, vals;
    vector<Update> updates;
    vector<Query> queries;

    PointModeMo() {}
    PointModeMo(const vector<ll>& a) {
        init(a);
    }
    void init(const vector<ll>& a) {
        initA = a;
        n = a.size();
        updates.clear();
        queries.clear();
    }
    int add_update(int p, ll v) {
        updates.push_back({p, v});
        return (int)updates.size() - 1;
    }
    int add_query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        int id = queries.size();
        if (l > r) queries.push_back({0, -1, (int)updates.size(), id});
        else queries.push_back({l, r, (int)updates.size(), id});
        return id;
    }
    int block_len() const {
        long long lim = 1LL * max(1, n) * max(1, n);
        int l = 1, r = max(1, n), ans = 1;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if ((__int128)mid * mid * mid <= lim) ans = mid, l = mid + 1;
            else r = mid - 1;
        }
        return ans;
    }
    vector<ll> solve_mode() {
        vals = initA;
        for (auto& e : updates) vals.emplace_back(e.v);
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = lower_bound(vals.begin(), vals.end(), initA[i]) - vals.begin();
        vector<Mod> mods(updates.size());
        for (int i = 0; i < (int)updates.size(); ++i) {
            mods[i] = {updates[i].p, (int)(lower_bound(vals.begin(), vals.end(), updates[i].v) - vals.begin())};
        }

        int B = block_len();
        vector<Query> ord = queries;
        sort(ord.begin(), ord.end(), [&](const Query& x, const Query& y) {
            int ax = x.l / B, ay = y.l / B;
            if (ax != ay) return ax < ay;
            int bx = x.r / B, by = y.r / B;
            if (bx != by) return (ax & 1) ? bx > by : bx < by;
            return ((ax ^ bx) & 1) ? x.t > y.t : x.t < y.t;
        });

        int V = vals.size();
        vector<int> cnt(V, 0);
        vector<set<int>> bucket(n + 1);
        int best = 0;
        auto add_val = [&](int x) {
            int c = cnt[x];
            if (c) bucket[c].erase(x);
            ++cnt[x];
            bucket[c + 1].insert(x);
            if (c + 1 > best) best = c + 1;
        };
        auto del_val = [&](int x) {
            int c = cnt[x];
            bucket[c].erase(x);
            --cnt[x];
            if (c - 1) bucket[c - 1].insert(x);
            while (best && bucket[best].empty()) --best;
        };

        int L = 0, R = -1, T = 0;
        auto apply_mod = [&](int id) {
            int p = mods[id].p;
            if (L <= p && p <= R) del_val(a[p]);
            swap(a[p], mods[id].v);
            if (L <= p && p <= R) add_val(a[p]);
        };

        vector<ll> ans(queries.size(), 0);
        for (auto& q : ord) {
            while (T < q.t) apply_mod(T++);
            while (T > q.t) apply_mod(--T);
            while (R < q.r) add_val(a[++R]);
            while (L > q.l) add_val(a[--L]);
            while (R > q.r) del_val(a[R--]);
            while (L < q.l) del_val(a[L++]);
            if (best) ans[q.id] = vals[*bucket[best].begin()];
        }
        return ans;
    }
};
