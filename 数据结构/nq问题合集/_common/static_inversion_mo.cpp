#include <bits/stdc++.h>
using namespace std;

/*
StaticInversionMo：静态区间逆序对，0-based，闭区间 [l,r]，离线莫队。

add_query(l,r) 加入询问，solve_inversions() 返回答案。

复杂度：O((n+q)sqrt(n)logV)。
*/

struct StaticInversionMo {
    using ll = long long;
    struct Query {
        int l, r, id;
    };
    struct BIT {
        int n;
        vector<int> c;
        void init(int _n) {
            n = _n;
            c.assign(n + 1, 0);
        }
        void add(int p, int v) {
            for (++p; p <= n; p += p & -p) c[p] += v;
        }
        int sum(int p) const {
            int s = 0;
            for (++p; p > 0; p &= p - 1) s += c[p];
            return s;
        }
        int range_sum(int l, int r) const {
            if (l > r) return 0;
            return sum(r) - (l ? sum(l - 1) : 0);
        }
    };

    int n, B, V;
    vector<int> a;
    vector<ll> vals;
    vector<Query> qs;

    StaticInversionMo() {}
    StaticInversionMo(const vector<ll>& nums) {
        init(nums);
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
    void init(const vector<ll>& nums) {
        n = nums.size();
        B = max(1, isqrt(max(1, n)));
        vals = nums;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        V = vals.size();
        a.resize(n);
        for (int i = 0; i < n; ++i) a[i] = lower_bound(vals.begin(), vals.end(), nums[i]) - vals.begin();
        qs.clear();
    }
    int add_query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        int id = qs.size();
        if (l > r) qs.push_back({0, -1, id});
        else qs.push_back({l, r, id});
        return id;
    }
    vector<ll> solve_inversions() const {
        vector<Query> ord = qs;
        int BB = max(1, n / max(1, isqrt(max(1, (int)qs.size()))));
        sort(ord.begin(), ord.end(), [&](const Query& x, const Query& y) {
            int bx = x.l / BB, by = y.l / BB;
            if (bx != by) return bx < by;
            return (bx & 1) ? x.r > y.r : x.r < y.r;
        });
        BIT bit;
        bit.init(V);
        vector<ll> ans(qs.size(), 0);
        ll cur = 0;
        int l = 0, r = -1, len = 0;
        auto add_right = [&](int p) {
            cur += len - bit.sum(a[p]);
            bit.add(a[p], 1);
            ++len;
        };
        auto del_right = [&](int p) {
            bit.add(a[p], -1);
            --len;
            cur -= len - bit.sum(a[p]);
        };
        auto add_left = [&](int p) {
            cur += bit.range_sum(0, a[p] - 1);
            bit.add(a[p], 1);
            ++len;
        };
        auto del_left = [&](int p) {
            bit.add(a[p], -1);
            --len;
            cur -= bit.range_sum(0, a[p] - 1);
        };
        for (auto& q : ord) {
            while (r < q.r) add_right(++r);
            while (l > q.l) add_left(--l);
            while (r > q.r) del_right(r--);
            while (l < q.l) del_left(l++);
            ans[q.id] = cur;
        }
        return ans;
    }
};
