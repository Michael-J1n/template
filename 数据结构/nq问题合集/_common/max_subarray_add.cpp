#include <bits/stdc++.h>
using namespace std;

/*
RangeAddMaxSubarray：区间加 + 区间最大子段和，0-based，闭区间 [l,r]。

做法：
1. 根号分块。
2. 整块加只改 tag。
3. 每个块预处理三组直线：
   pref：所有前缀，sum + len * tag。
   suff：所有后缀，sum + len * tag。
   best：所有子段，sum + len * tag。

约定：最大子段和为非空子段。
复杂度：
1. rebuild 单块 O(B^2)。
2. add：O(B^2 + n/B)。
3. query：O(B + n/B * logB)。

B 可在构造时手动指定。默认取 n^0.4 左右，比 sqrt(n) 更适合这里的 O(B^2) rebuild。
*/

struct RangeAddMaxSubarray {
    using ll = long long;
    static constexpr ll NEG = LLONG_MIN / 4;
    struct Node {
        ll sum, pref, suff, best;
        int len;
    };
    struct Line {
        ll k, b;
        ll get(ll x) const {
            return k * x + b;
        }
    };
    struct LineHull {
        vector<Line> h;
        static long double inter(const Line& a, const Line& b) {
            return (long double)(b.b - a.b) / (a.k - b.k);
        }
        void build(const vector<Line>& lines) {
            h.clear();
            if (lines.empty()) return;
            h.reserve(lines.size());
            for (auto& line : lines) {
                while (h.size() >= 2 && inter(h[h.size() - 2], h.back()) >= inter(h.back(), line)) h.pop_back();
                h.emplace_back(line);
            }
        }
        ll get(ll x) const {
            if (h.empty()) return NEG;
            int l = 0, r = (int)h.size() - 1;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (h[mid].get(x) <= h[mid + 1].get(x)) l = mid + 1;
                else r = mid;
            }
            return h[l].get(x);
        }
    };

    int n, B, m;
    vector<ll> a, tag, sum;
    vector<LineHull> prefHull, suffHull, bestHull;
    vector<Line> tmpPref, tmpSuff, tmpBest;
    vector<ll> tmpBestByLen;

    RangeAddMaxSubarray() {}
    RangeAddMaxSubarray(const vector<ll>& nums, int _B = 0) {
        init(nums, _B);
    }
    int default_block(int n) const {
        long long lim = 1LL * max(1, n) * max(1, n);
        int x = 1;
        while (1LL * (x + 1) * (x + 1) * (x + 1) * (x + 1) * (x + 1) <= lim) ++x;
        return max(32, x + 1);
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
    void init(const vector<ll>& nums, int _B = 0) {
        n = nums.size();
        B = _B ? _B : default_block(n);
        B = min(B, max(1, n));
        m = (n + B - 1) / B;
        a = nums;
        tag.assign(m, 0);
        sum.assign(m, 0);
        prefHull.resize(m);
        suffHull.resize(m);
        bestHull.resize(m);
        for (int b = 0; b < m; ++b) rebuild(b);
    }
    inline int id(int i) const { return i / B; }
    inline int L(int b) const { return b * B; }
    inline int R(int b) const { return min(n, (b + 1) * B) - 1; }
    inline int len(int b) const { return R(b) - L(b) + 1; }
    void materialize(int b) {
        if (!tag[b]) return;
        for (int i = L(b); i <= R(b); ++i) a[i] += tag[b];
        tag[b] = 0;
    }
    void rebuild(int b) {
        int l = L(b), r = R(b);
        sum[b] = 0;
        int sz = r - l + 1;
        tmpPref.clear();
        tmpSuff.clear();
        tmpBest.clear();
        tmpPref.reserve(sz);
        tmpSuff.reserve(sz);
        tmpBest.reserve(sz);
        ll cur = 0;
        for (int i = l; i <= r; ++i) {
            sum[b] += a[i];
            cur += a[i];
            tmpPref.push_back({i - l + 1, cur});
        }
        cur = 0;
        for (int i = r; i >= l; --i) {
            cur += a[i];
            tmpSuff.push_back({r - i + 1, cur});
        }
        tmpBestByLen.assign(sz + 1, NEG);
        for (int i = l; i <= r; ++i) {
            cur = 0;
            for (int j = i; j <= r; ++j) {
                cur += a[j];
                int len = j - i + 1;
                tmpBestByLen[len] = max(tmpBestByLen[len], cur);
            }
        }
        for (int len = 1; len <= sz; ++len) {
            tmpBest.push_back({len, tmpBestByLen[len]});
        }
        prefHull[b].build(tmpPref);
        suffHull[b].build(tmpSuff);
        bestHull[b].build(tmpBest);
    }
    Node block_node(int b) const {
        int sz = len(b);
        ll t = tag[b];
        return {sum[b] + t * sz, prefHull[b].get(t), suffHull[b].get(t), bestHull[b].get(t), sz};
    }
    Node brute_node(int l, int r, int b) const {
        Node res = empty();
        ll t = tag[b];
        for (int i = l; i <= r; ++i) res = merge(res, single(a[i] + t));
        return res;
    }
    void add(int l, int r, ll v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r || !v) return;
        int bl = id(l), br = id(r);
        if (bl == br) {
            materialize(bl);
            for (int i = l; i <= r; ++i) a[i] += v;
            rebuild(bl);
            return;
        }
        materialize(bl);
        for (int i = l; i <= R(bl); ++i) a[i] += v;
        rebuild(bl);
        for (int b = bl + 1; b < br; ++b) tag[b] += v;
        materialize(br);
        for (int i = L(br); i <= r; ++i) a[i] += v;
        rebuild(br);
    }
    Node query(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return empty();
        int bl = id(l), br = id(r);
        if (bl == br) return brute_node(l, r, bl);
        Node res = brute_node(l, R(bl), bl);
        for (int b = bl + 1; b < br; ++b) res = merge(res, block_node(b));
        return merge(res, brute_node(L(br), r, br));
    }
    ll range_max_subarray(int l, int r) const {
        return query(l, r).best;
    }
};
