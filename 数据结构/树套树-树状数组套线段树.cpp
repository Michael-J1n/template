#include <bits/stdc++.h>
using namespace std;

/**
 * 树状数组套权值线段树 / BIT of Segment Trees【单点修改、区间顺序统计、加权求和】
 * 维护数组 a 及位置权重 w，支持单点修改和区间值域查询；数组下标 0-based，查询区间为闭区间 [l, r]，第 k 小的 k 从 1 开始。
 * 关键在于可以求出**涉及区间的加权求和**。
 * 
 * 功能：
 * BITSegTree<T> st(a, vs, w)：初始化，a 为原数组，vs 为后续操作带来的值，w 为数组对应位置的权值
 * set(p, x)：令 a[p] = x，保留 w[p]
 * set_weight(p, v)：令 w[p] = v
 * set(p, x, v)：同时修改 a[p] 和 w[p]
 * lt_cnt(l, r, x)：返回 [l, r] 中严格 <x 的元素个数
 * le_cnt(l, r, x)：返回 [l, r] 中 <=x 的元素个数
 * cnt_range(l, r, x, y)：返回 [l, r] 中值属于 [x, y] 的元素个数
 * rank(l, r, x)：返回 x 在 [l, r] 中的排名，1-based
 * pre_val(l, r, x)：返回 [l, r] 中最大 <x 的值
 * lower_bound(l, r, x)：返回 [l, r] 中最小 >=x 的值
 * upper_bound(l, r, x)：返回 [l, r] 中最小 >x 的值
 * nxt_val(l, r, x)：等价于 upper_bound
 * kth(l, r, k)：返回 [l, r] 中第 k 小，1-based
 * sum_lt / sum_le / sum_range：按值域筛选，求 a[i] 之和
 *【特殊操作：】
 * weight_lt / weight_le / weight_range：按 a[i] 筛选，求 w[i] 之和
 * sum_k_smallest(l, r, k)：返回 [l, r] 中最小 k 个元素值之和
 *
 * 适用：带单点修改的区间顺序统计、值域计数、元素和及独立权重和。所有未来 set 出现的值必须提前加入 candidates 离散化，默认 w[i] = 1。
 *
 * 复杂度：
 * 离散化排序 O((n+c) log(n+c))，建树插入 O(n log n log m)
 * set / 查询最坏 O(log n log m)
 * 空间最坏 O(n log n log m + m)，动态节点支持回收。
 * m 为离散化后不同值个数，c 为候选值数量。
 *
 * 前置条件：数组非空；修改下标与查询区间合法且区间非空；
 * kth 的 k 位于 [1, r-l+1]，sum_k_smallest 的 k 位于 [0, r-l+1]；
 * T 为 int 或 long long，所有求和及中间结果不溢出 long long。
 */
template<class T>
struct BITSegTree {
    static constexpr T INF = sizeof(T) == 4 ? INT_MAX : LLONG_MAX;
    static constexpr T NINF = sizeof(T) == 4 ? INT_MIN : LLONG_MIN;
    struct Node {
        int ls, rs, cnt;
        long long sum, wsum;
    };
    struct Stat {
        int cnt = 0;
        long long sum = 0, wsum = 0;
    };
    int n, m;
    vector<T> vals;
    vector<int> a, root, free_nodes;
    vector<long long> w;
    vector<Node> tr;
    BITSegTree() {}
    BITSegTree(int _n) { build(vector<T>(_n, T()), {}); }
    BITSegTree(int _n, const vector<T>& candidates) { build(vector<T>(_n, T()), candidates); }
    BITSegTree(const vector<T>& _a) { build(_a, {}); }
    BITSegTree(const vector<T>& _a, const vector<T>& candidates) { build(_a, candidates); }
    BITSegTree(const vector<T>& _a, const vector<T>& candidates, const vector<long long>& weights) { build(_a, candidates, weights); }
    int new_node() {
        if (!free_nodes.empty()) {
            int u = free_nodes.back();
            free_nodes.pop_back();
            tr[u] = {};
            return u;
        }
        int u = tr.size();
        tr.push_back({});
        return u;
    }
    int add(int u, int l, int r, int p, int delta, long long weight) {
        if (!u) u = new_node();
        tr[u].cnt += delta;
        if (delta > 0) tr[u].sum += vals[p], tr[u].wsum += weight;
        else tr[u].sum -= vals[p], tr[u].wsum -= weight;
        if (l != r) {
            int mid = l + r >> 1;
            if (p <= mid) tr[u].ls = add(tr[u].ls, l, mid, p, delta, weight);
            else tr[u].rs = add(tr[u].rs, mid + 1, r, p, delta, weight);
        }
        if (!tr[u].cnt) {
            free_nodes.push_back(u);
            return 0;
        }
        return u;
    }
    void build(const vector<T>& src, const vector<T>& candidates, const vector<long long>& weights = {}) {
        n = src.size();
        vals = src;
        vals.insert(vals.end(), candidates.begin(), candidates.end());
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        m = vals.size();
        a.resize(n);
        w = weights.empty() ? vector<long long>(n, 1) : weights;
        root.assign(n + 1, 0);
        tr.assign(1, {});
        free_nodes.clear();
        for (int p = 0; p < n; ++p) {
            a[p] = std::lower_bound(vals.begin(), vals.end(), src[p]) - vals.begin();
            for (int i = p + 1; i <= n; i += i & -i)
                root[i] = add(root[i], 0, m - 1, a[p], 1, w[p]);
        }
    }
    Stat query_less(int u, int l, int r, int x) const {
        if (!u || x <= l) return {};
        if (r < x) return {tr[u].cnt, tr[u].sum, tr[u].wsum};
        int mid = l + r >> 1;
        if (x <= mid + 1) return query_less(tr[u].ls, l, mid, x);
        Stat res = query_less(tr[u].rs, mid + 1, r, x);
        int v = tr[u].ls;
        res.cnt += tr[v].cnt;
        res.sum += tr[v].sum;
        res.wsum += tr[v].wsum;
        return res;
    }
    Stat get_less(int l, int r, int x) const {
        Stat res;
        for (int i = r + 1; i; i -= i & -i) {
            Stat cur = query_less(root[i], 0, m - 1, x);
            res.cnt += cur.cnt;
            res.sum += cur.sum;
            res.wsum += cur.wsum;
        }
        for (int i = l; i; i -= i & -i) {
            Stat cur = query_less(root[i], 0, m - 1, x);
            res.cnt -= cur.cnt;
            res.sum -= cur.sum;
            res.wsum -= cur.wsum;
        }
        return res;
    }
    int get_roots(int p, array<int, 64>& ids) const {
        int k = 0;
        for (; p; p -= p & -p) ids[k++] = root[p];
        return k;
    }
    int lt_cnt(int l, int r, T x) const {
        int p = std::lower_bound(vals.begin(), vals.end(), x) - vals.begin();
        return get_less(l, r, p).cnt;
    }
    int le_cnt(int l, int r, T x) const {
        int p = std::upper_bound(vals.begin(), vals.end(), x) - vals.begin();
        return get_less(l, r, p).cnt;
    }
    int cnt_range(int l, int r, T x, T y) const {
        if (y < x) return 0;
        return le_cnt(l, r, y) - lt_cnt(l, r, x);
    }
    int rank(int l, int r, T x) const { return lt_cnt(l, r, x) + 1; }
    T kth(int l, int r, int k) const {
        array<int, 64> ls, rs;
        int ln = get_roots(l, ls), rn = get_roots(r + 1, rs);
        int lo = 0, hi = m - 1;
        while (lo < hi) {
            int cnt = 0, mid = lo + hi >> 1;
            for (int i = 0; i < rn; ++i) cnt += tr[tr[rs[i]].ls].cnt;
            for (int i = 0; i < ln; ++i) cnt -= tr[tr[ls[i]].ls].cnt;
            if (k <= cnt) {
                for (int i = 0; i < rn; ++i) rs[i] = tr[rs[i]].ls;
                for (int i = 0; i < ln; ++i) ls[i] = tr[ls[i]].ls;
                hi = mid;
            } else {
                k -= cnt;
                for (int i = 0; i < rn; ++i) rs[i] = tr[rs[i]].rs;
                for (int i = 0; i < ln; ++i) ls[i] = tr[ls[i]].rs;
                lo = mid + 1;
            }
        }
        return vals[lo];
    }
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
    long long sum_lt(int l, int r, T x) const {
        int p = std::lower_bound(vals.begin(), vals.end(), x) - vals.begin();
        return get_less(l, r, p).sum;
    }
    long long sum_le(int l, int r, T x) const {
        int p = std::upper_bound(vals.begin(), vals.end(), x) - vals.begin();
        return get_less(l, r, p).sum;
    }
    long long sum_range(int l, int r, T x, T y) const {
        if (y < x) return 0;
        return sum_le(l, r, y) - sum_lt(l, r, x);
    }
    long long weight_lt(int l, int r, T x) const {
        int p = std::lower_bound(vals.begin(), vals.end(), x) - vals.begin();
        return get_less(l, r, p).wsum;
    }
    long long weight_le(int l, int r, T x) const {
        int p = std::upper_bound(vals.begin(), vals.end(), x) - vals.begin();
        return get_less(l, r, p).wsum;
    }
    long long weight_range(int l, int r, T x, T y) const {
        if (y < x) return 0;
        return weight_le(l, r, y) - weight_lt(l, r, x);
    }
    long long sum_k_smallest(int l, int r, int k) const {
        if (k == 0) return 0;
        array<int, 64> ls, rs;
        int ln = get_roots(l, ls), rn = get_roots(r + 1, rs);
        long long res = 0;
        int lo = 0, hi = m - 1;
        while (lo < hi) {
            int cnt = 0, mid = lo + hi >> 1;
            long long sum = 0;
            for (int i = 0; i < rn; ++i) {
                int u = tr[rs[i]].ls;
                cnt += tr[u].cnt;
                sum += tr[u].sum;
            }
            for (int i = 0; i < ln; ++i) {
                int u = tr[ls[i]].ls;
                cnt -= tr[u].cnt;
                sum -= tr[u].sum;
            }
            if (k <= cnt) {
                for (int i = 0; i < rn; ++i) rs[i] = tr[rs[i]].ls;
                for (int i = 0; i < ln; ++i) ls[i] = tr[ls[i]].ls;
                hi = mid;
            } else {
                k -= cnt;
                res += sum;
                for (int i = 0; i < rn; ++i) rs[i] = tr[rs[i]].rs;
                for (int i = 0; i < ln; ++i) ls[i] = tr[ls[i]].rs;
                lo = mid + 1;
            }
        }
        return res + 1LL * k * vals[lo];
    }
    void set(int p, T x, long long weight) {
        int id = std::lower_bound(vals.begin(), vals.end(), x) - vals.begin();
        if (a[p] == id && w[p] == weight) return;
        for (int i = p + 1; i <= n; i += i & -i) {
            root[i] = add(root[i], 0, m - 1, a[p], -1, w[p]);
            root[i] = add(root[i], 0, m - 1, id, 1, weight);
        }
        a[p] = id;
        w[p] = weight;
    }
    void set(int p, T x) { set(p, x, w[p]); }
    void set_weight(int p, long long weight) { set(p, vals[a[p]], weight); }
};
