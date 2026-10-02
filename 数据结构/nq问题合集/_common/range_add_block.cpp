#include <bits/stdc++.h>
using namespace std;

/*
RangeAddBlock：区间加 + 频率表 / 众数 / kth / median / distinct / cnt / 值域计数。

0-based，闭区间 [l,r]。
每块维护：
1. a：不含整块 tag 的块内值。
2. tag：整块加懒标记。
3. sorted array：回答 cnt[<=x] / 值域计数 / kth 二分。
4. freq map：回答频率表 / 众数 / distinct。

复杂度：
1. add：O(BlogB + sqrt n)。
2. count_le/count_between：O(B + sqrt n logB)。
3. kth：O(logV * (B + sqrt n logB))。
4. freq/mode/distinct：O(B + 被扫到整块的不同值总数)。
*/

struct RangeAddBlock {
    using ll = long long;
    static constexpr ll INF = LLONG_MAX / 2;
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
    int n, B, m;
    vector<ll> a, tag, sum;
    vector<vector<ll>> s;
    vector<unordered_map<ll, int, Hash>> frq;
    mutable vector<int> seenMex;
    mutable vector<ll> invBuf, invTmp;
    mutable unordered_map<ll, int, Hash> work;
    mutable int mexTimer = 0;

    RangeAddBlock() {}
    RangeAddBlock(const vector<ll>& nums) {
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
        a = nums;
        n = a.size();
        B = isqrt(n) + 1;
        m = (n + B - 1) / B;
        tag.assign(m, 0);
        sum.assign(m, 0);
        s.assign(m, {});
        frq.assign(m, {});
        seenMex.assign(n + 2, 0);
        invBuf.reserve(n);
        invTmp.reserve(n);
        work.clear();
        work.max_load_factor(0.7);
        mexTimer = 0;
        for (int b = 0; b < m; ++b) rebuild(b);
    }
    inline int id(int i) const { return i / B; }
    inline int L(int b) const { return b * B; }
    inline int R(int b) const { return min(n, (b + 1) * B) - 1; }
    inline int len(int b) const { return R(b) - L(b) + 1; }
    void rebuild(int b) {
        sum[b] = 0;
        s[b].clear();
        frq[b].clear();
        s[b].reserve(len(b));
        frq[b].max_load_factor(0.7);
        frq[b].reserve(len(b) + 5);
        for (int i = L(b); i <= R(b); ++i) {
            sum[b] += a[i];
            s[b].emplace_back(a[i]);
            ++frq[b][a[i]];
        }
        sort(s[b].begin(), s[b].end());
    }
    void add(int l, int r, ll v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r || !v) return;
        int bl = id(l), br = id(r);
        if (bl == br) {
            for (int i = l; i <= r; ++i) a[i] += v;
            rebuild(bl);
            return;
        }
        for (int i = l; i <= R(bl); ++i) a[i] += v;
        rebuild(bl);
        for (int b = bl + 1; b < br; ++b) tag[b] += v;
        for (int i = L(br); i <= r; ++i) a[i] += v;
        rebuild(br);
    }
    int count_eq(int l, int r, ll x) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int bl = id(l), br = id(r), res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) res += (a[i] + tag[bl] == x);
            return res;
        }
        for (int i = l; i <= R(bl); ++i) res += (a[i] + tag[bl] == x);
        for (int b = bl + 1; b < br; ++b) {
            auto it = frq[b].find(x - tag[b]);
            if (it != frq[b].end()) res += it->second;
        }
        for (int i = L(br); i <= r; ++i) res += (a[i] + tag[br] == x);
        return res;
    }
    int freq_hint(int l, int r) const {
        int bl = id(l), br = id(r);
        if (bl == br) return r - l + 1;
        long long res = R(bl) - l + 1 + r - L(br) + 1;
        for (int b = bl + 1; b < br; ++b) res += frq[b].size();
        return (int)min<long long>(res, r - l + 1);
    }
    void fill_freq(int l, int r, unordered_map<ll, int, Hash>& res) const {
        int bl = id(l), br = id(r);
        if (bl == br) {
            ll t = tag[bl];
            for (int i = l; i <= r; ++i) ++res[a[i] + t];
            return;
        }
        ll lt = tag[bl];
        for (int i = l; i <= R(bl); ++i) ++res[a[i] + lt];
        for (int b = bl + 1; b < br; ++b) {
            ll t = tag[b];
            for (auto& p : frq[b]) res[p.first + t] += p.second;
        }
        ll rt = tag[br];
        for (int i = L(br); i <= r; ++i) ++res[a[i] + rt];
    }
    unordered_map<ll, int, Hash> freq(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        unordered_map<ll, int, Hash> res;
        if (l > r) return res;
        res.max_load_factor(0.7);
        res.reserve(freq_hint(l, r) + 5);
        fill_freq(l, r, res);
        return res;
    }
    int distinct(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        work.clear();
        work.reserve(freq_hint(l, r) + 5);
        fill_freq(l, r, work);
        return work.size();
    }
    pair<ll, int> mode_pair(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return {0, 0};
        work.clear();
        work.reserve(freq_hint(l, r) + 5);
        fill_freq(l, r, work);
        pair<ll, int> res = {0, 0};
        for (auto& [x, c] : work) {
            if (c > res.second || (c == res.second && x < res.first)) res = {x, c};
        }
        return res;
    }
    ll mode(int l, int r) const {
        return mode_pair(l, r).first;
    }
    int count_le(int l, int r, ll x) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int bl = id(l), br = id(r), res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) res += (a[i] + tag[bl] <= x);
            return res;
        }
        for (int i = l; i <= R(bl); ++i) res += (a[i] + tag[bl] <= x);
        for (int b = bl + 1; b < br; ++b) {
            res += upper_bound(s[b].begin(), s[b].end(), x - tag[b]) - s[b].begin();
        }
        for (int i = L(br); i <= r; ++i) res += (a[i] + tag[br] <= x);
        return res;
    }
    int count_between(int l, int r, ll vL, ll vR) const {
        if (vL > vR) return 0;
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int bl = id(l), br = id(r), res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) {
                ll cur = a[i] + tag[bl];
                res += (vL <= cur && cur <= vR);
            }
            return res;
        }
        for (int i = l; i <= R(bl); ++i) {
            ll cur = a[i] + tag[bl];
            res += (vL <= cur && cur <= vR);
        }
        for (int b = bl + 1; b < br; ++b) {
            auto L = lower_bound(s[b].begin(), s[b].end(), vL - tag[b]);
            auto R = upper_bound(s[b].begin(), s[b].end(), vR - tag[b]);
            res += R - L;
        }
        for (int i = L(br); i <= r; ++i) {
            ll cur = a[i] + tag[br];
            res += (vL <= cur && cur <= vR);
        }
        return res;
    }
    pair<ll, ll> range_minmax(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return {INF, -INF};
        int bl = id(l), br = id(r);
        ll mn = INF, mx = -INF;
        auto touch = [&](ll x) {
            mn = min(mn, x);
            mx = max(mx, x);
        };
        if (bl == br) {
            for (int i = l; i <= r; ++i) touch(a[i] + tag[bl]);
            return {mn, mx};
        }
        for (int i = l; i <= R(bl); ++i) touch(a[i] + tag[bl]);
        for (int b = bl + 1; b < br; ++b) {
            touch(s[b].front() + tag[b]);
            touch(s[b].back() + tag[b]);
        }
        for (int i = L(br); i <= r; ++i) touch(a[i] + tag[br]);
        return {mn, mx};
    }
    ll kth_smallest(int l, int r, int k) const {
        l = max(l, 0), r = min(r, n - 1);
        int len = r - l + 1;
        if (l > r || k < 1 || k > len) return 0;
        auto [Lval, Rval] = range_minmax(l, r);
        while (Lval < Rval) {
            ll mid = Lval + (ll)((__int128)Rval - Lval) / 2;
            if (count_le(l, r, mid) >= k) Rval = mid;
            else Lval = mid + 1;
        }
        return Lval;
    }
    ll kth_largest(int l, int r, int k) const {
        l = max(l, 0), r = min(r, n - 1);
        int len = r - l + 1;
        if (l > r || k < 1 || k > len) return 0;
        return kth_smallest(l, r, len - k + 1);
    }
    ll median(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        return kth_smallest(l, r, (r - l + 2) / 2);
    }
    ll range_sum(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int bl = id(l), br = id(r);
        ll res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) res += a[i] + tag[bl];
            return res;
        }
        for (int i = l; i <= R(bl); ++i) res += a[i] + tag[bl];
        for (int b = bl + 1; b < br; ++b) res += sum[b] + tag[b] * len(b);
        for (int i = L(br); i <= r; ++i) res += a[i] + tag[br];
        return res;
    }
    int mex(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int lim = r - l + 1;
        if (++mexTimer == INT_MAX) {
            fill(seenMex.begin(), seenMex.end(), 0);
            mexTimer = 1;
        }
        int bl = id(l), br = id(r);
        auto mark = [&](ll x) {
            if (0 <= x && x <= lim) seenMex[(int)x] = mexTimer;
        };
        if (bl == br) {
            ll t = tag[bl];
            for (int i = l; i <= r; ++i) mark(a[i] + t);
        } else {
            ll lt = tag[bl];
            for (int i = l; i <= R(bl); ++i) mark(a[i] + lt);
            for (int b = bl + 1; b < br; ++b) {
                ll t = tag[b];
                for (auto& p : frq[b]) mark(p.first + t);
            }
            ll rt = tag[br];
            for (int i = L(br); i <= r; ++i) mark(a[i] + rt);
        }
        for (int x = 0; x <= lim + 1; ++x) if (seenMex[x] != mexTimer) return x;
        return lim + 1;
    }
    void collect_vals(int l, int r, vector<ll>& v) const {
        int bl = id(l), br = id(r);
        if (bl == br) {
            ll t = tag[bl];
            for (int i = l; i <= r; ++i) v.emplace_back(a[i] + t);
            return;
        }
        ll lt = tag[bl];
        for (int i = l; i <= R(bl); ++i) v.emplace_back(a[i] + lt);
        for (int b = bl + 1; b < br; ++b) {
            ll t = tag[b];
            for (int i = L(b); i <= R(b); ++i) v.emplace_back(a[i] + t);
        }
        ll rt = tag[br];
        for (int i = L(br); i <= r; ++i) v.emplace_back(a[i] + rt);
    }
    long long cdq_inv(vector<ll>& v, vector<ll>& tmp, int L, int R) const {
        static constexpr int S = 32;
        if (R - L <= S) {
            long long res = 0;
            for (int i = L; i < R; ++i) {
                for (int j = i + 1; j < R; ++j) res += v[i] > v[j];
            }
            for (int i = L + 1; i < R; ++i) {
                ll x = v[i];
                int j = i - 1;
                while (j >= L && v[j] > x) v[j + 1] = v[j], --j;
                v[j + 1] = x;
            }
            return res;
        }
        int M = (L + R) >> 1;
        long long res = cdq_inv(v, tmp, L, M) + cdq_inv(v, tmp, M, R);
        int i = L, j = M, k = L;
        while (i < M && j < R) {
            if (v[i] <= v[j]) tmp[k++] = v[i++];
            else res += M - i, tmp[k++] = v[j++];
        }
        while (i < M) tmp[k++] = v[i++];
        while (j < R) tmp[k++] = v[j++];
        copy(tmp.begin() + L, tmp.begin() + R, v.begin() + L);
        return res;
    }
    long long inversion_count(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        invBuf.clear();
        invBuf.reserve(r - l + 1);
        collect_vals(l, r, invBuf);
        invTmp.resize(invBuf.size());
        return cdq_inv(invBuf, invTmp, 0, invBuf.size());
    }
};
