#include <bits/stdc++.h>
using namespace std;

/**
 * 小波矩阵 / Wavelet Matrix【静态区间顺序统计、值域计数】
 * 维护静态数组 a，支持区间值域查询。
 * 数组下标为 0-based，查询区间为闭区间 [l, r]，第 k 小的 k 从 1 开始。
 *
 * 功能：
 * 构造：WaveletMatrix wm(std::move(a))
 * kth(l, r, k)：返回 [l, r] 中第 k 小，1-based
 * lt_cnt(l, r, x)：返回 [l, r] 中严格 <x 的元素个数
 * le_cnt(l, r, x)：返回 [l, r] 中 <=x 的元素个数
 * cnt_range(l, r, x, y)：返回 [l, r] 中值属于 [x, y] 的元素个数
 * rank(l, r, x)：返回 x 在 [l, r] 中的排名，1-based
 * pre_val(l, r, x)：返回 [l, r] 中最大 <x 的值
 * lower_bound(l, r, x)：返回 [l, r] 中最小 >=x 的值
 * upper_bound(l, r, x)：返回 [l, r] 中最小 >x 的值
 * nxt_val(l, r, x)：等价于 upper_bound
 *
 * 适用：静态数组上的区间顺序统计、值域计数、前驱后继查询。
 *
 * 复杂度：
 * 建树 O(n log n + n log m)，空间 O(n log m)
 * 单次查询 O(log m)，m 为不同值个数。
 *
 * 前置条件：构造数组非空；查询区间合法且非空；kth 的 k 位于 [1, r-l+1]。
 */
#pragma GCC target("popcnt")
template<class T>
struct WaveletMatrix {
    static constexpr T INF = numeric_limits<T>::max(), NINF = numeric_limits<T>::lowest();
    struct BitVector {
        vector<unsigned long long> bits;
        vector<int> pre;
        BitVector() {}
        BitVector(int n) {
            int m = (n + 63) >> 6;
            bits.assign(m + 1, 0);
            pre.assign(m + 1, 0);
        }
        void set(int i) { bits[i >> 6] |= 1ULL << (i & 63); }
        void build() {
            int m = pre.size() - 1;
            for (int i = 0; i < m; ++i) pre[i + 1] = pre[i] + __builtin_popcountll(bits[i]);
        }
        int rank0(int i) const {
            int b = i >> 6, k = i & 63;
            return i - pre[b] - __builtin_popcountll(bits[b] & ((1ULL << k) - 1));
        }
    };
    int n = 0, m = 0, LOG = 1;
    vector<T> vals;
    vector<int> mid;
    vector<BitVector> pre;
    WaveletMatrix() = default;
    WaveletMatrix(vector<T> a) { build(std::move(a)); }
    void build(vector<T> a) {
        n = a.size(); vals = a;
        sort(vals.begin(), vals.end()); vals.erase(unique(vals.begin(), vals.end()), vals.end());
        m = vals.size(); LOG = 0;
        while ((1LL << LOG) < m) ++LOG;
        pre.clear(); mid.clear();
        if (!LOG) return;
        vector<int> cur(n);
        for (int i = 0; i < n; ++i) cur[i] = std::lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin();
        vector<T>().swap(a);
        pre.resize(LOG); mid.resize(LOG);
        vector<int> nxt(n);
        for (int d = 0; d < LOG; ++d) {
            pre[d] = BitVector(n);
            int bit = LOG - 1 - d, cnt0 = 0;
            for (int i = 0; i < n; ++i) cnt0 += !((cur[i] >> bit) & 1);
            mid[d] = cnt0;
            int p0 = 0, p1 = cnt0;
            for (int i = 0; i < n; ++i) {
                if ((cur[i] >> bit) & 1) pre[d].set(i), nxt[p1++] = cur[i];
                else nxt[p0++] = cur[i];
            }
            pre[d].build();
            cur.swap(nxt);
        }
    }
    T kth(int l, int r, int k) const {
        int x = 0;
        for (int d = 0; d < LOG; ++d) {
            int zl = pre[d].rank0(l), zr = pre[d].rank0(r + 1), cnt0 = zr - zl;
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
            int zl = pre[d].rank0(l), zr = pre[d].rank0(r + 1);
            if ((x >> (LOG - 1 - d)) & 1) res += zr - zl, l = mid[d] + l - zl, r = mid[d] + r - zr;
            else l = zl, r = zr - 1;
        }
        return res;
    }
    int lt_cnt(int l, int r, T x) const { return count_id_less(l, r, std::lower_bound(vals.begin(), vals.end(), x) - vals.begin()); }
    int le_cnt(int l, int r, T x) const { return count_id_less(l, r, std::upper_bound(vals.begin(), vals.end(), x) - vals.begin()); }
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
};