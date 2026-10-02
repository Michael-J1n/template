#include <bits/stdc++.h>
using namespace std;

/*
RangeAssignBlock：区间赋值 + 频率表 / 众数 / kth / median / distinct / cnt / 值域计数。

0-based，闭区间 [l,r]。
这是 ODT 的确定性替代：不会被构造数据卡成很多段。

复杂度：
1. set：O(BlogB + sqrt n)，整块赋值 O(1) 打 tag。
2. count_le/count_between：O(B + sqrt n logB)。
3. kth：O(logV * (B + sqrt n logB))。
4. freq/mode/distinct：O(B + 被扫到整块的不同值总数)。
*/

struct RangeAssignBlock {
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
    vector<ll> a, setTag;
    vector<char> hasSet;
    vector<vector<ll>> s;
    vector<unordered_map<ll, int, Hash>> frq;
    mutable vector<int> seenMex;
    mutable vector<ll> invBuf, invTmp;
    mutable unordered_map<ll, int, Hash> work;
    mutable int mexTimer = 0;

    RangeAssignBlock() {}
    RangeAssignBlock(const vector<ll>& nums) {
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
        setTag.assign(m, 0);
        hasSet.assign(m, 0);
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
    void materialize(int b) {
        if (!hasSet[b]) return;
        for (int i = L(b); i <= R(b); ++i) a[i] = setTag[b];
        hasSet[b] = 0;
    }
    void apply_block(int b, ll v) {
        hasSet[b] = 1;
        setTag[b] = v;
        frq[b].clear();
        frq[b][v] = len(b);
    }
    void rebuild(int b) {
        hasSet[b] = 0;
        s[b].clear();
        frq[b].clear();
        s[b].reserve(len(b));
        frq[b].max_load_factor(0.7);
        frq[b].reserve(len(b) + 5);
        for (int i = L(b); i <= R(b); ++i) {
            s[b].emplace_back(a[i]);
            ++frq[b][a[i]];
        }
        sort(s[b].begin(), s[b].end());
    }
    void set(int l, int r, ll v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        int bl = id(l), br = id(r);
        if (bl == br) {
            materialize(bl);
            for (int i = l; i <= r; ++i) a[i] = v;
            rebuild(bl);
            return;
        }
        materialize(bl);
        for (int i = l; i <= R(bl); ++i) a[i] = v;
        rebuild(bl);
        for (int b = bl + 1; b < br; ++b) apply_block(b, v);
        materialize(br);
        for (int i = L(br); i <= r; ++i) a[i] = v;
        rebuild(br);
    }
    int count_eq(int l, int r, ll x) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int bl = id(l), br = id(r), res = 0;
        if (bl == br) {
            if (hasSet[bl]) return setTag[bl] == x ? r - l + 1 : 0;
            for (int i = l; i <= r; ++i) res += (a[i] == x);
            return res;
        }
        if (hasSet[bl]) res += setTag[bl] == x ? R(bl) - l + 1 : 0;
        else for (int i = l; i <= R(bl); ++i) res += (a[i] == x);
        for (int b = bl + 1; b < br; ++b) {
            if (hasSet[b]) res += setTag[b] == x ? len(b) : 0;
            else {
                auto it = frq[b].find(x);
                if (it != frq[b].end()) res += it->second;
            }
        }
        if (hasSet[br]) res += setTag[br] == x ? r - L(br) + 1 : 0;
        else for (int i = L(br); i <= r; ++i) res += (a[i] == x);
        return res;
    }
    int freq_hint(int l, int r) const {
        int bl = id(l), br = id(r);
        auto part = [&](int b, int len) {
            return hasSet[b] ? 1 : len;
        };
        if (bl == br) return part(bl, r - l + 1);
        long long res = part(bl, R(bl) - l + 1) + part(br, r - L(br) + 1);
        for (int b = bl + 1; b < br; ++b) res += hasSet[b] ? 1 : (int)frq[b].size();
        return (int)min<long long>(res, r - l + 1);
    }
    void fill_freq(int l, int r, unordered_map<ll, int, Hash>& res) const {
        int bl = id(l), br = id(r);
        if (bl == br) {
            if (hasSet[bl]) res[setTag[bl]] = r - l + 1;
            else for (int i = l; i <= r; ++i) ++res[a[i]];
            return;
        }
        if (hasSet[bl]) res[setTag[bl]] += R(bl) - l + 1;
        else for (int i = l; i <= R(bl); ++i) ++res[a[i]];
        for (int b = bl + 1; b < br; ++b) {
            if (hasSet[b]) res[setTag[b]] += len(b);
            else for (auto& p : frq[b]) res[p.first] += p.second;
        }
        if (hasSet[br]) res[setTag[br]] += r - L(br) + 1;
        else for (int i = L(br); i <= r; ++i) ++res[a[i]];
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
            if (hasSet[bl]) return setTag[bl] <= x ? r - l + 1 : 0;
            for (int i = l; i <= r; ++i) res += (a[i] <= x);
            return res;
        }
        if (hasSet[bl]) res += setTag[bl] <= x ? R(bl) - l + 1 : 0;
        else for (int i = l; i <= R(bl); ++i) res += (a[i] <= x);
        for (int b = bl + 1; b < br; ++b) {
            if (hasSet[b]) res += setTag[b] <= x ? len(b) : 0;
            else res += upper_bound(s[b].begin(), s[b].end(), x) - s[b].begin();
        }
        if (hasSet[br]) res += setTag[br] <= x ? r - L(br) + 1 : 0;
        else for (int i = L(br); i <= r; ++i) res += (a[i] <= x);
        return res;
    }
    int count_between(int l, int r, ll vL, ll vR) const {
        if (vL > vR) return 0;
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int bl = id(l), br = id(r), res = 0;
        auto inside = [&](ll x) { return vL <= x && x <= vR; };
        if (bl == br) {
            if (hasSet[bl]) return inside(setTag[bl]) ? r - l + 1 : 0;
            for (int i = l; i <= r; ++i) res += inside(a[i]);
            return res;
        }
        if (hasSet[bl]) res += inside(setTag[bl]) ? R(bl) - l + 1 : 0;
        else for (int i = l; i <= R(bl); ++i) res += inside(a[i]);
        for (int b = bl + 1; b < br; ++b) {
            if (hasSet[b]) res += inside(setTag[b]) ? len(b) : 0;
            else {
                auto L = lower_bound(s[b].begin(), s[b].end(), vL);
                auto R = upper_bound(s[b].begin(), s[b].end(), vR);
                res += R - L;
            }
        }
        if (hasSet[br]) res += inside(setTag[br]) ? r - L(br) + 1 : 0;
        else for (int i = L(br); i <= r; ++i) res += inside(a[i]);
        return res;
    }
    pair<ll, ll> range_minmax(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return {INF, -INF};
        int bl = id(l), br = id(r);
        ll mn = INF, mx = -INF;
        auto touch = [&](ll x) { mn = min(mn, x), mx = max(mx, x); };
        if (bl == br) {
            if (hasSet[bl]) return {setTag[bl], setTag[bl]};
            for (int i = l; i <= r; ++i) touch(a[i]);
            return {mn, mx};
        }
        if (hasSet[bl]) touch(setTag[bl]);
        else for (int i = l; i <= R(bl); ++i) touch(a[i]);
        for (int b = bl + 1; b < br; ++b) {
            if (hasSet[b]) touch(setTag[b]);
            else touch(s[b].front()), touch(s[b].back());
        }
        if (hasSet[br]) touch(setTag[br]);
        else for (int i = L(br); i <= r; ++i) touch(a[i]);
        return {mn, mx};
    }
    ll kth_smallest(int l, int r, int k) const {
        l = max(l, 0), r = min(r, n - 1);
        int sz = r - l + 1;
        if (l > r || k < 1 || k > sz) return 0;
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
        int sz = r - l + 1;
        if (l > r || k < 1 || k > sz) return 0;
        return kth_smallest(l, r, sz - k + 1);
    }
    ll median(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        return kth_smallest(l, r, (r - l + 2) / 2);
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
            if (hasSet[bl]) mark(setTag[bl]);
            else for (int i = l; i <= r; ++i) mark(a[i]);
        } else {
            if (hasSet[bl]) mark(setTag[bl]);
            else for (int i = l; i <= R(bl); ++i) mark(a[i]);
            for (int b = bl + 1; b < br; ++b) {
                if (hasSet[b]) mark(setTag[b]);
                else for (auto& p : frq[b]) mark(p.first);
            }
            if (hasSet[br]) mark(setTag[br]);
            else for (int i = L(br); i <= r; ++i) mark(a[i]);
        }
        for (int x = 0; x <= lim + 1; ++x) if (seenMex[x] != mexTimer) return x;
        return lim + 1;
    }
    void collect_vals(int l, int r, vector<ll>& v) const {
        int bl = id(l), br = id(r);
        if (bl == br) {
            if (hasSet[bl]) {
                for (int i = l; i <= r; ++i) v.emplace_back(setTag[bl]);
            } else {
                for (int i = l; i <= r; ++i) v.emplace_back(a[i]);
            }
            return;
        }
        if (hasSet[bl]) for (int i = l; i <= R(bl); ++i) v.emplace_back(setTag[bl]);
        else for (int i = l; i <= R(bl); ++i) v.emplace_back(a[i]);
        for (int b = bl + 1; b < br; ++b) {
            if (hasSet[b]) for (int i = L(b); i <= R(b); ++i) v.emplace_back(setTag[b]);
            else for (int i = L(b); i <= R(b); ++i) v.emplace_back(a[i]);
        }
        if (hasSet[br]) for (int i = L(br); i <= r; ++i) v.emplace_back(setTag[br]);
        else for (int i = L(br); i <= r; ++i) v.emplace_back(a[i]);
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
