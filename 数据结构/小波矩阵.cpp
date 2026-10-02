#include <bits/stdc++.h>
using namespace std;

/**
 * 小波矩阵 / Wavelet Matrix【静态区间第 k 小、排名、值域计数】
 * 数组下标为 0-based，所有查询区间均为闭区间 [l,r]。
 * 
 * 支持操作：
 * kth(l,r,k)：区间第 k 小，k 从 1 开始
 * rank(l,r,x)：x 的排名，即 <x 的个数 +1
 * count(l,r,x)：x 的出现次数；range_freq(l,r,low,high)：值属于 [low,high] 的个数
 * prev_value / next_value：查询前驱后继，不存在时分别返回 T 的最小值 / 最大值
 *
 * 适用：静态数组上的区间顺序统计、值域计数、前驱后继查询。
 *
 * 复杂度：建树 O(n log V)，单次查询 O(log V)，空间 O(n log V)，V 为不同值个数。
 */
template<class T>
struct WaveletMatrix {
    int n, m, LOG;
    vector<T> vals;
    vector<int> mid;
    vector<vector<int>> pre;
    WaveletMatrix() {}
    WaveletMatrix(const vector<T>& a) { build(a); }
    void build(const vector<T>& a) {
        n = a.size();
        vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        m = vals.size();
        LOG = 1;
        while ((1LL << LOG) < m) ++LOG;
        pre.assign(LOG, vector<int>(n + 1));
        mid.assign(LOG, 0);
        vector<int> cur(n), nxt(n);
        for (int i = 0; i < n; ++i) cur[i] = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin();
        for (int d = 0; d < LOG; ++d) {
            int bit = LOG - 1 - d;
            for (int i = 0; i < n; ++i) pre[d][i + 1] = pre[d][i] + (((cur[i] >> bit) & 1) == 0);
            mid[d] = pre[d][n];
            int p0 = 0, p1 = mid[d];
            for (int i = 0; i < n; ++i) {
                if ((cur[i] >> bit) & 1) nxt[p1++] = cur[i];
                else nxt[p0++] = cur[i];
            }
            cur.swap(nxt);
        }
    }
    T kth(int l, int r, int k) const {
        int x = 0;
        for (int d = 0; d < LOG; ++d) {
            int zl = pre[d][l], zr = pre[d][r + 1];
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
            int zl = pre[d][l], zr = pre[d][r + 1];
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
        // int k = count_leq(l, r, x) + 1;  // 若需要的是严格后继
        if (k > r - l + 1) return numeric_limits<T>::max();
        return kth(l, r, k);
    }
};