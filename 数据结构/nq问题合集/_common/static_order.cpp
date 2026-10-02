#include <bits/stdc++.h>
using namespace std;

/*
StaticRangeOrder：静态区间 kth / median / cnt[<=x] / 值域计数，0-based，闭区间 [l,r]。

先离散化，再建 Wavelet Matrix。构造器的 sigma 填写值域大小
比按原始值域建 Wavelet Tree 更稳：层数是 log(不同值个数)，没有指针节点。

接口：
1. kth_smallest(l, r, k)：区间第 k 小，k 从 1 开始。
2. kth_largest(l, r, k)：区间第 k 大，k 从 1 开始。
3. median(l, r)：下中位数，即第 (len+1)/2 小。
4. count_le(l, r, x)：区间内 <= x 的个数。
5. count_between(l, r, vL, vR)：区间内值域在 [vL,vR] 的个数。

复杂度：build O(nlogV)，单次查询 O(logV)，V 为不同值个数。
*/

struct StaticRangeOrder {
    using ll = long long;

    struct WaveletMatrix {
        int n, K, stride;
        vector<int> z;
        vector<int> pre;

        WaveletMatrix() {}
        WaveletMatrix(const vector<int>& a, int sigma) {
            init(a, sigma);
        }
        void init(const vector<int>& a, int sigma) {
            n = a.size();
            K = sigma <= 1 ? 0 : 32 - __builtin_clz(sigma - 1);
            stride = n + 1;
            z.assign(K, 0);
            pre.assign((long long)K * stride, 0);
            vector<int> cur = a, nxt(n);
            for (int dep = 0; dep < K; ++dep) {
                int bit = K - 1 - dep;
                int* row = pre.data() + (long long)dep * stride;
                for (int i = 0; i < n; ++i) row[i + 1] = row[i] + ((cur[i] >> bit) & 1);
                int p0 = 0, p1 = 0;
                for (int x : cur) p1 += !((x >> bit) & 1);
                z[dep] = p1;
                for (int x : cur) {
                    if ((x >> bit) & 1) nxt[p1++] = x;
                    else nxt[p0++] = x;
                }
                cur.swap(nxt);
            }
        }
        int kth(int l, int r, int k) const {
            if (!K) return 0;
            int res = 0;
            for (int dep = 0; dep < K; ++dep) {
                int bit = K - 1 - dep;
                const int* row = pre.data() + (long long)dep * stride;
                int l1 = row[l], r1 = row[r];
                int l0 = l - l1, r0 = r - r1;
                int cnt0 = r0 - l0;
                if (k <= cnt0) {
                    l = l0, r = r0;
                } else {
                    k -= cnt0;
                    res |= 1 << bit;
                    l = z[dep] + l1;
                    r = z[dep] + r1;
                }
            }
            return res;
        }
        int count_less_id(int l, int r, int x) const {
            if (l >= r || x <= 0) return 0;
            if (!K) return x > 0 ? r - l : 0;
            if (x >= (1LL << K)) return r - l;
            int res = 0;
            for (int dep = 0; dep < K; ++dep) {
                int bit = K - 1 - dep;
                const int* row = pre.data() + (long long)dep * stride;
                int l1 = row[l], r1 = row[r];
                int l0 = l - l1, r0 = r - r1;
                if ((x >> bit) & 1) {
                    res += r0 - l0;
                    l = z[dep] + l1;
                    r = z[dep] + r1;
                } else {
                    l = l0, r = r0;
                }
            }
            return res;
        }
    };

    int n;
    vector<ll> vals;
    WaveletMatrix wm;

    StaticRangeOrder() {}
    StaticRangeOrder(const vector<ll>& nums) {
        init(nums);
    }
    void init(const vector<ll>& nums) {
        n = nums.size();
        vals = nums;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = lower_bound(vals.begin(), vals.end(), nums[i]) - vals.begin();
        wm.init(a, vals.size());
    }
    bool norm(int& l, int& r) const {
        l = max(l, 0), r = min(r, n - 1);
        return l <= r;
    }
    ll kth_smallest(int l, int r, int k) const {
        if (!norm(l, r)) return 0;
        int len = r - l + 1;
        if (k < 1 || k > len) return 0;
        return vals[wm.kth(l, r + 1, k)];
    }
    ll kth_largest(int l, int r, int k) const {
        if (!norm(l, r)) return 0;
        int len = r - l + 1;
        if (k < 1 || k > len) return 0;
        return kth_smallest(l, r, len - k + 1);
    }
    ll median(int l, int r) const {
        if (!norm(l, r)) return 0;
        return kth_smallest(l, r, (r - l + 2) / 2);
    }
    int count_le(int l, int r, ll x) const {
        if (!norm(l, r)) return 0;
        int id = upper_bound(vals.begin(), vals.end(), x) - vals.begin();
        return wm.count_less_id(l, r + 1, id);
    }
    int count_between(int l, int r, ll vL, ll vR) const {
        if (vL > vR) return 0;
        if (!norm(l, r)) return 0;
        int L = lower_bound(vals.begin(), vals.end(), vL) - vals.begin();
        int R = upper_bound(vals.begin(), vals.end(), vR) - vals.begin();
        return wm.count_less_id(l, r + 1, R) - wm.count_less_id(l, r + 1, L);
    }
};
