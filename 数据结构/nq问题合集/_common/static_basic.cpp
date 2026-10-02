#include <bits/stdc++.h>
using namespace std;

/*
StaticRangeBasic：静态数组 + 区间 sum/max/min/gcd，0-based，闭区间 [l,r]。

复杂度：
1. build：O(nlogn)。
2. range_sum：O(1)。
3. range_max/range_min/range_gcd：O(1)。

注意：
1. gcd 用 abs(value)，空区间返回 0。
2. max/min 的空区间返回 -INF/INF。
*/

struct StaticRangeBasic {
    using ll = long long;
    static constexpr ll INF = LLONG_MAX / 2;
    struct Node {
        ll sum, mx, mn, g;
    };
    template<int OP>
    struct ST {
        int n, K;
        vector<ll> st;
        ST() {}
        ST(const vector<ll>& a) {
            init(a);
        }
        ll e() const {
            if constexpr (OP == 0) return -INF;
            else if constexpr (OP == 1) return INF;
            else return 0;
        }
        ll f(ll x, ll y) const {
            if constexpr (OP == 0) return max(x, y);
            else if constexpr (OP == 1) return min(x, y);
            else return std::gcd(llabs(x), llabs(y));
        }
        void init(const vector<ll>& a) {
            n = a.size();
            if (!n) {
                K = 0;
                st.clear();
                return;
            }
            K = 31 - __builtin_clz(n);
            st.assign((long long)(K + 1) * n, e());
            for (int i = 0; i < n; ++i) st[i] = a[i];
            for (int k = 1; k <= K; ++k) {
                int len = 1 << k, half = len >> 1;
                ll* cur = st.data() + (long long)k * n;
                ll* pre = st.data() + (long long)(k - 1) * n;
                for (int i = 0; i + len <= n; ++i) cur[i] = f(pre[i], pre[i + half]);
            }
        }
        ll get(int l, int r) const {
            l = max(l, 0), r = min(r, n - 1);
            if (l > r) return e();
            int k = 31 - __builtin_clz(r - l + 1);
            const ll* row = st.data() + (long long)k * n;
            return f(row[l], row[r - (1 << k) + 1]);
        }
    };

    int n;
    vector<ll> pre;
    ST<0> stMx;
    ST<1> stMn;
    ST<2> stGcd;

    StaticRangeBasic() {}
    StaticRangeBasic(const vector<ll>& a) {
        init(a);
    }
    void init(const vector<ll>& a) {
        n = a.size();
        pre.assign(n + 1, 0);
        for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] + a[i];
        stMx.init(a);
        stMn.init(a);
        stGcd.init(a);
    }
    bool norm(int& l, int& r) const {
        l = max(l, 0), r = min(r, n - 1);
        return l <= r;
    }
    ll range_sum(int l, int r) const {
        if (!norm(l, r)) return 0;
        return pre[r + 1] - pre[l];
    }
    ll range_max(int l, int r) const {
        return stMx.get(l, r);
    }
    ll range_min(int l, int r) const {
        return stMn.get(l, r);
    }
    ll range_gcd(int l, int r) const {
        return stGcd.get(l, r);
    }
    Node query(int l, int r) const {
        return {range_sum(l, r), range_max(l, r), range_min(l, r), range_gcd(l, r)};
    }
};
