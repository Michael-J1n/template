#include <bits/stdc++.h>
using namespace std;

/**
 * 带修小波矩阵 / Dynamic Wavelet Matrix【单点修改、区间第 k 小、排名、值域计数】
 * 数组下标为 0-based，所有查询区间均为闭区间 [l, r]，第 k 小的 k 从 1 开始。
 *
 * 支持操作：
 * update(p,x)：将 a[p] 修改为 x
 * kth(l,r,k)：区间第 k 小
 * rank(l,r,x)：x 的排名，即 <x 的个数 + 1
 * count(l,r,x)：x 的出现次数；range_freq(l,r,low,high)：值属于 [low,high] 的个数
 * prev_value / next_value：查询前驱后继，不存在时分别返回 T 的最小值 / 最大值
 *
 * 适用：带单点修改的区间顺序统计、值域计数、前驱后继查询。
 * 所有未来 update 出现的值必须预先加入 candidates 参与离散化。
 *
 * 复杂度：建树 O(n log n log V)，修改/查询 O(log n log V)，空间 O(n log V)。
 */
template<class T>
struct DynamicWaveletMatrix {
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
            for (int i = 0; i < (int)a.size(); ++i) {
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
        for (int i = 0; i < n; ++i) a[i] = lower_bound(vals.begin(), vals.end(), src[i]) - vals.begin();
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
    int count_less(int l, int r, T x) const { return count_id_less(l, r, lower_bound(vals.begin(), vals.end(), x) - vals.begin()); }
    int count_leq(int l, int r, T x) const { return count_id_less(l, r, upper_bound(vals.begin(), vals.end(), x) - vals.begin()); }
    int rank(int l, int r, T x) const { return count_less(l, r, x) + 1; }
    int count(int l, int r, T x) const { return count_leq(l, r, x) - count_less(l, r, x); }
    int range_freq(int l, int r, T low, T high) const { return count_leq(l, r, high) - count_less(l, r, low); }
    T prev_value(int l, int r, T x) const {
        int k = count_less(l, r, x);
        if (k == 0) return numeric_limits<T>::lowest();
        return kth(l, r, k);
    }
    T next_value(int l, int r, T x) const {
        int k = count_less(l, r, x) + 1;
        if (k > r - l + 1) return numeric_limits<T>::max();
        return kth(l, r, k);
    }
    void update(int p, T x) {
        int old_id = a[p];
        int new_id = lower_bound(vals.begin(), vals.end(), x) - vals.begin();
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