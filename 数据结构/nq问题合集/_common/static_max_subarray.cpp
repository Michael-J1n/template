#include <bits/stdc++.h>
using namespace std;

/*
StaticMaxSubarray：静态区间最大子段和，0-based，闭区间 [l,r]。

用 Disjoint Sparse Table 维护最大子段和这个结合信息。
比普通线段树更适合纯静态：build O(nlogn)，query O(1)。

约定：最大子段和为非空子段。
*/

struct StaticMaxSubarray {
    using ll = long long;
    static constexpr ll NEG = LLONG_MIN / 4;
    struct Node {
        ll sum, pref, suff, best;
        int len;
    };
    int n, K;
    vector<Node> st;

    StaticMaxSubarray() {}
    StaticMaxSubarray(const vector<ll>& a) {
        init(a);
    }
    Node empty() const {
        return {0, NEG, NEG, NEG, 0};
    }
    Node single(ll v) const {
        return {v, v, v, v, 1};
    }
    Node merge(const Node& a, const Node& b) const {
        if (!a.len) return b;
        if (!b.len) return a;
        Node c;
        c.len = a.len + b.len;
        c.sum = a.sum + b.sum;
        c.pref = max(a.pref, a.sum + b.pref);
        c.suff = max(b.suff, b.sum + a.suff);
        c.best = max(max(a.best, b.best), a.suff + b.pref);
        return c;
    }
    Node& at(int k, int i) {
        return st[(long long)k * n + i];
    }
    const Node& at(int k, int i) const {
        return st[(long long)k * n + i];
    }
    void init(const vector<ll>& a) {
        n = a.size();
        if (!n) {
            K = 0;
            st.clear();
            return;
        }
        K = n <= 1 ? 0 : 32 - __builtin_clz(n - 1);
        st.assign((long long)(K + 1) * n, empty());
        for (int i = 0; i < n; ++i) at(0, i) = single(a[i]);
        for (int k = 1; k <= K; ++k) {
            int len = 1 << k, half = len >> 1;
            for (int l = 0; l < n; l += len) {
                int mid = min(l + half, n), r = min(l + len, n);
                if (mid == l) continue;
                at(k, mid - 1) = at(0, mid - 1);
                for (int i = mid - 2; i >= l; --i) at(k, i) = merge(at(0, i), at(k, i + 1));
                if (mid < r) {
                    at(k, mid) = at(0, mid);
                    for (int i = mid + 1; i < r; ++i) at(k, i) = merge(at(k, i - 1), at(0, i));
                }
            }
        }
    }
    Node query(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return empty();
        if (l == r) return at(0, l);
        int k = 32 - __builtin_clz(l ^ r);
        return merge(at(k, l), at(k, r));
    }
    ll range_max_subarray(int l, int r) const {
        return query(l, r).best;
    }
};
