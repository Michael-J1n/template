#include <bits/stdc++.h>
using namespace std;

/**
 * 带修小波矩阵 / Dynamic Wavelet Matrix【单点修改、区间顺序统计、值域计数】
 * 维护数组 a，支持单点修改和区间值域查询，功能与 SegTreap 对齐。
 *
 * 数组下标 0-based，查询区间为闭区间 [l, r]，第 k 小的 k 从 1 开始。
 *
 * 功能：
 * DynamicWaveletMatrix<T> wm(a, vs): 使用数组 a，查询中可能会出现的值 vs 初始化
 * set(p, x)：令 a[p] = x
 * lt_cnt(l, r, x)：返回 [l, r] 中严格 <x 的元素个数
 * le_cnt(l, r, x)：返回 [l, r] 中 <=x 的元素个数
 * cnt_range(l, r, x, y)：返回 [l, r] 中值属于 [x, y] 的元素个数
 * rank(l, r, x)：返回 x 在 [l, r] 中的排名，1-based
 * pre_val(l, r, x)：返回 [l, r] 中最大 <x 的值
 * lower_bound(l, r, x)：返回 [l, r] 中最小 >=x 的值
 * upper_bound(l, r, x)：返回 [l, r] 中最小 >x 的值
 * nxt_val(l, r, x)：等价于 upper_bound
 * kth(l, r, k)：返回 [l, r] 中第 k 小，1-based
 *
 * 适用：带单点修改的区间顺序统计、值域计数、前驱后继查询。
 * 所有未来 set 出现的值必须预先加入 candidates 参与离散化。
 *
 * 复杂度：
 * 建树期望 O(n log n log m)，空间 O(n log m)
 * set / 所有查询期望 O(log n log m)
 * m 为离散化后的不同值个数。
 *
 * 前置条件：构造数组非空；修改下标与查询区间合法且区间非空；
 * kth 的 k 位于 [1, r-l+1]；set 的新值已参与离散化。
 */
template<class T>
struct DynamicWaveletMatrix {
    static constexpr T INF = numeric_limits<T>::max();
    static constexpr T NINF = numeric_limits<T>::lowest();
    struct DynamicBitVector {
        struct Node {
            int ls, rs;
            int sz, sum;
            int bit;
            unsigned pri;
        };
        int root;
        unsigned seed;
        vector<Node> tr;
        DynamicBitVector() : root(0), seed(712367821) {}
        int size(int u) const { return u ? tr[u].sz : 0; }
        int sum(int u) const { return u ? tr[u].sum : 0; }
        unsigned rng() {
            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;
            return seed;
        }
        void pushup(int u) {
            tr[u].sz = size(tr[u].ls) + size(tr[u].rs) + 1;
            tr[u].sum = sum(tr[u].ls) + sum(tr[u].rs) + tr[u].bit;
        }
        int merge(int a, int b) {
            if (!a || !b) return a | b;
            if (tr[a].pri > tr[b].pri) {
                tr[a].rs = merge(tr[a].rs, b);
                pushup(a);
                return a;
            }
            tr[b].ls = merge(a, tr[b].ls);
            pushup(b);
            return b;
        }
        void split(int u, int k, int& a, int& b) {
            if (!u) {
                a = b = 0;
                return;
            }
            if (size(tr[u].ls) >= k) {
                split(tr[u].ls, k, a, tr[u].ls);
                b = u;
                pushup(b);
            } else {
                split(tr[u].rs, k - size(tr[u].ls) - 1, tr[u].rs, b);
                a = u;
                pushup(a);
            }
        }
        void build(const vector<int>& a) {
            root = 0;
            tr.assign(a.size() + 1, Node());
            int n = a.size();
            for (int i = 0; i < n; ++i) {
                int u = i + 1;
                tr[u].ls = tr[u].rs = 0;
                tr[u].sz = 1;
                tr[u].sum = tr[u].bit = a[i];
                tr[u].pri = rng();
                root = merge(root, u);
            }
        }
        int prefix_sum(int k) const {
            int u = root, res = 0;
            while (u) {
                int left = size(tr[u].ls);
                if (k <= left) u = tr[u].ls;
                else {
                    res += sum(tr[u].ls);
                    if (k == left + 1) return res + tr[u].bit;
                    res += tr[u].bit;
                    k -= left + 1;
                    u = tr[u].rs;
                }
            }
            return res;
        }
        int rank0(int k) const { return k - prefix_sum(k); }
        int erase(int p) {
            int a, b, c, d;
            split(root, p, a, b);
            split(b, 1, c, d);
            root = merge(a, d);
            tr[c].ls = tr[c].rs = 0;
            tr[c].sz = 1;
            tr[c].sum = tr[c].bit;
            return c;
        }
        void insert(int p, int u, int bit) {
            int a, b;
            tr[u].ls = tr[u].rs = 0;
            tr[u].sz = 1;
            tr[u].sum = tr[u].bit = bit;
            split(root, p, a, b);
            root = merge(merge(a, u), b);
        }
    };
    int n, m, LOG;
    vector<T> vals;
    vector<int> a, mid;
    vector<DynamicBitVector> bit;
    DynamicWaveletMatrix() {}
    DynamicWaveletMatrix(int _n) { build(vector<T>(_n, T()), {}); }
    DynamicWaveletMatrix(int _n, const vector<T>& candidates) { build(vector<T>(_n, T()), candidates); }
    DynamicWaveletMatrix(const vector<T>& a) { build(a, a); }
    DynamicWaveletMatrix(const vector<T>& a, const vector<T>& candidates) { build(a, candidates); }
    void build(const vector<T>& src, const vector<T>& candidates) {
        n = src.size();
        vals = src;
        vals.insert(vals.end(), candidates.begin(), candidates.end());
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        m = vals.size();
        LOG = 1;
        while ((1LL << LOG) < m) ++LOG;
        a.resize(n);
        for (int i = 0; i < n; ++i)
            a[i] = std::lower_bound(vals.begin(), vals.end(), src[i]) - vals.begin();
        mid.assign(LOG, 0);
        bit.assign(LOG, DynamicBitVector());
        vector<int> cur = a, nxt(n);
        for (int d = 0; d < LOG; ++d) {
            int b = LOG - 1 - d;
            vector<int> bits(n);
            for (int i = 0; i < n; ++i) bits[i] = (cur[i] >> b) & 1;
            bit[d].build(bits);
            mid[d] = count(bits.begin(), bits.end(), 0);
            int p0 = 0, p1 = mid[d];
            for (int i = 0; i < n; ++i) {
                if (bits[i]) nxt[p1++] = cur[i];
                else nxt[p0++] = cur[i];
            }
            cur.swap(nxt);
        }
    }
    T kth(int l, int r, int k) const {
        int x = 0;
        for (int d = 0; d < LOG; ++d) {
            int zl = bit[d].rank0(l), zr = bit[d].rank0(r + 1);
            int cnt0 = zr - zl;
            if (k <= cnt0) l = zl, r = zr - 1;
            else k -= cnt0, x |= 1 << (LOG - 1 - d), l = mid[d] + l - zl, r = mid[d] + r - zr;
        }
        return vals[x];
    }
    int count_id_less(int l, int r, int x) const {
        if (x <= 0) return 0;
        if (x >= m) return r - l + 1;
        int res = 0;
        for (int d = 0; d < LOG; ++d) {
            int zl = bit[d].rank0(l), zr = bit[d].rank0(r + 1);
            if ((x >> (LOG - 1 - d)) & 1) res += zr - zl, l = mid[d] + l - zl, r = mid[d] + r - zr;
            else l = zl, r = zr - 1;
        }
        return res;
    }
    int lt_cnt(int l, int r, T x) const {
        return count_id_less(l, r, std::lower_bound(vals.begin(), vals.end(), x) - vals.begin());
    }
    int le_cnt(int l, int r, T x) const {
        return count_id_less(l, r, std::upper_bound(vals.begin(), vals.end(), x) - vals.begin());
    }
    int cnt_range(int l, int r, T x, T y) const {
        if (y < x) return 0;
        return le_cnt(l, r, y) - lt_cnt(l, r, x);
    }
    int rank(int l, int r, T x) const { return lt_cnt(l, r, x) + 1; }
    T pre_val(int l, int r, T x) const {
        int k = lt_cnt(l, r, x);
        if (k == 0) return NINF;
        return kth(l, r, k);
    }
    T lower_bound(int l, int r, T x) const {
        int k = lt_cnt(l, r, x) + 1;
        if (k > r - l + 1) return INF;
        return kth(l, r, k);
    }
    T upper_bound(int l, int r, T x) const {
        int k = le_cnt(l, r, x) + 1;
        if (k > r - l + 1) return INF;
        return kth(l, r, k);
    }
    T nxt_val(int l, int r, T x) const { return upper_bound(l, r, x); }
    void set(int p, T x) {
        int old_id = a[p];
        int new_id = std::lower_bound(vals.begin(), vals.end(), x) - vals.begin();
        if (old_id == new_id) return;
        int old_pos = p, new_pos = p;
        for (int d = 0; d < LOG; ++d) {
            int b = LOG - 1 - d;
            int old_bit = (old_id >> b) & 1;
            int new_bit = (new_id >> b) & 1;
            int old_z = bit[d].rank0(old_pos);
            int old_next = old_bit ? mid[d] + old_pos - old_z : old_z;
            int u = bit[d].erase(old_pos);
            if (!old_bit) --mid[d];
            bit[d].insert(new_pos, u, new_bit);
            if (!new_bit) ++mid[d];
            int new_z = bit[d].rank0(new_pos);
            int new_next = new_bit ? mid[d] + new_pos - new_z : new_z;
            old_pos = old_next;
            new_pos = new_next;
        }
        a[p] = new_id;
    }
};