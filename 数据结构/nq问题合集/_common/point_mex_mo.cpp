#include <bits/stdc++.h>
using namespace std;

/*
PointMexMo：单点修改 + 区间 mex，离线带修莫队，0-based，闭区间 [l,r]。

用法：
1. PointMexMo mo(a);
2. mo.add_update(p, v)：按时间顺序加入 a[p]=v。
3. mo.add_query(l, r)：按时间顺序加入询问，返回 id。
4. solve_mex()：返回每个询问的 mex。

复杂度：O((n+q) * n^(2/3)) 量级。
*/

struct PointMexMo {
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
    vector<ll> initA;
    vector<Update> updates;
    vector<Query> queries;

    PointMexMo() {}
    PointMexMo(const vector<ll>& a) {
        init(a);
    }
    void init(const vector<ll>& a) {
        initA = a;
        n = a.size();
        updates.clear();
        queries.clear();
    }
    int norm_val(ll x) const {
        return (0 <= x && x <= n) ? (int)x : n + 1;
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
    int isqrt(int x) const {
        int l = 0, r = min(x, 46340), ans = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (1LL * mid * mid <= x) ans = mid, l = mid + 1;
            else r = mid - 1;
        }
        return ans;
    }
    vector<int> solve_mex() const {
        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = norm_val(initA[i]);
        vector<Mod> mods(updates.size());
        for (int i = 0; i < (int)updates.size(); ++i) mods[i] = {updates[i].p, norm_val(updates[i].v)};

        int B = block_len();
        vector<Query> ord = queries;
        sort(ord.begin(), ord.end(), [&](const Query& x, const Query& y) {
            int ax = x.l / B, ay = y.l / B;
            if (ax != ay) return ax < ay;
            int bx = x.r / B, by = y.r / B;
            if (bx != by) return (ax & 1) ? bx > by : bx < by;
            return ((ax ^ bx) & 1) ? x.t > y.t : x.t < y.t;
        });

        int S = isqrt(n + 1) + 1, M = (n + S) / S + 1;
        vector<int> cnt(n + 2, 0), miss(M, 0);
        for (int b = 0; b < M; ++b) {
            int l = b * S, r = min(n, (b + 1) * S - 1);
            if (l <= r) miss[b] = r - l + 1;
        }
        auto add_val = [&](int x) {
            if (x > n) return;
            if (cnt[x]++ == 0) --miss[x / S];
        };
        auto del_val = [&](int x) {
            if (x > n) return;
            if (--cnt[x] == 0) ++miss[x / S];
        };
        auto get_mex = [&]() {
            for (int b = 0; b < M; ++b) {
                if (!miss[b]) continue;
                int l = b * S, r = min(n, (b + 1) * S - 1);
                for (int x = l; x <= r; ++x) if (!cnt[x]) return x;
            }
            return n + 1;
        };

        int L = 0, R = -1, T = 0;
        auto apply_mod = [&](int id) {
            int p = mods[id].p;
            if (L <= p && p <= R) del_val(a[p]);
            swap(a[p], mods[id].v);
            if (L <= p && p <= R) add_val(a[p]);
        };

        vector<int> ans(queries.size(), 0);
        for (auto& q : ord) {
            while (T < q.t) apply_mod(T++);
            while (T > q.t) apply_mod(--T);
            while (R < q.r) add_val(a[++R]);
            while (L > q.l) add_val(a[--L]);
            while (R > q.r) del_val(a[R--]);
            while (L < q.l) del_val(a[L++]);
            ans[q.id] = get_mex();
        }
        return ans;
    }
};
