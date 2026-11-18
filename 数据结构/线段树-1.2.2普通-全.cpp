#include <bits/stdc++.h>
using namespace std;

/**
 * 普通线段树
 * 支持单点修改、区间查询、区间内二分
 *
 * 基本操作：
 * set(p, v)：把 a[p] 修改为 v
 * add(p, v)：把 a[p] 加上 v
 * query(l, r)：查询区间 [l, r] 的 Node
 * range_sum(l, r)：查询区间和
 * range_max(l, r)：查询区间最大值
 * range_min(l, r)：查询区间最小值
 *
 * 线段树上二分（找叶子形 & 合并区间形）：
 * **注意：
 *     find_first/last 中，check(node)==false 必须代表整个区间都不可能有答案；
 *     bisect_right/bisect_left 必须要求随着区间扩展，check 有 TTT...FFF 的单调性**
 * 
 * find_first(l, r, check)：找 [l, r] 中 **第一个满足要求的下标**；不存在返回 n
 *      check = [&](Node node) { return node 代表的这个节点区间里面，是否至少存在一个答案位置；}。
 *      形象理解即为如果 check(node)，就在这个 node 下继续递归找；node 最后一定会递归到满足 check 条件的叶子节点
 *      例：找 [l,r] 第一个 a[i]>=x：int p=find_first(l,r,[&](Node node){return node.mx>=x;});
 *      
 * find_last(l, r, check): 找 [l, r] 中 **最后一个满足要求的下标**；不存在返回 -1
 *      check = [&](Node node) { return node 代表的这个节点区间里面，是否至少存在一个答案位置；}。
 *      形象理解即为如果 check(node)，就在这个 node 下继续递归找；node 最后一定会递归到满足 check 条件的叶子节点
 *      例：找 [l,r] 最后一个 a[i]<=x：int p=find_last(l,r,[&](Node node){return node.mn<=x;});
 * 
 * bisect_right(l, r, check): 从 l 向右累计，返回 **第一个使累计区间!不合法!的位置**；全合法返回 r + 1；重载时省略 r，默认 n - 1
 *      check = [&](Node node) { return node 代表的这个累计区间，是否满足条件并可以继续向右合并；}。
 *      形象理解即为只要 check(node) 就合并这个 node 然后继续向右；node 最后一定是一个第一次不满足 check 条件的 merge 节点
 *      注意 l 开始到 返回值-1 的累计区间是合法的
 *      例：从 l 往右找第一个区间和>=x的位置：int p=bisect_right(l,[&](Node node){return node.sum<x;});
 * 
 * bisect_left(l, r, check): 从 r 向左累计，返回 **第一个使累计区间!不合法!的位置**；全合法返回 l - 1；重载时省略 l，默认 0
 *      check = [&](Node node) { return node 代表的这个累计区间，是否满足条件并可以继续向左合并；}。
 *      形象理解为只要 check(node) 就合并这个 node 然后继续向左；node 最后一定是一个第一次不满足 check 条件的 merge 节点
 *      注意 返回值+1 到 r 的累计区间是合法的
 *      例：从 r 往左找到第一个区间和>x的位置：int p=bisect_left(r,[&](Node node){return node.sum<=x;});
 *
 * 复杂度：
 * 建树 O(n)
 * 修改/查询/二分 O(log n)
 * 前置条件：构造数组非空；修改下标与查询/搜索区间均合法且非空
 */
template<class T>
struct SegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    // -----------------------------修改开始----------------------------------
    struct Node {
        T sum, mx, mn;
    };
    Node E = {0, -INF, INF};  // 单位元
    Node Z = {0, 0, 0};       // 空数组初始化
    Node merge(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);
        return res;
    }
    void leaf(int u, int p, T v) {
        Node& cur = tr[u];
        cur.sum = v;
        cur.mn = v;
        cur.mx = v;
    }
    // -----------------------------修改结束----------------------------------
    int n;
    vector<Node> tr;
    SegTree() {}
    SegTree(int _n) : n(_n) { tr.assign(n * 4 + 5, Z); }
    SegTree(const vector<T>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        build(1, 0, n - 1, a);
    }
    void pushup(int u) { tr[u] = merge(tr[u << 1], tr[u << 1 | 1]); }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) return leaf(u, l, a[l]);
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void add(int u, int l, int r, int p, T v) {
        if (l == r) {
            tr[u].sum += v;
            tr[u].mx += v;
            tr[u].mn += v;
            return;
        }
        int mid = (l + r) >> 1;
        if (p <= mid) add(u << 1, l, mid, p, v);
        else add(u << 1 | 1, mid + 1, r, p, v);
        pushup(u);
    }
    void add(int p, T v) {
        if (p < 0 || p >= n) return;
        add(1, 0, n - 1, p, v);
    }
    void set(int u, int l, int r, int p, T v) {
        if (l == r) return leaf(u, p, v);
        int mid = (l + r) >> 1;
        if (p <= mid) set(u << 1, l, mid, p, v);
        else set(u << 1 | 1, mid + 1, r, p, v);
        pushup(u);
    }
    void set(int p, T v) {
        if (p < 0 || p >= n) return;
        set(1, 0, n - 1, p, v);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    }
    Node query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return E;
        return query(1, 0, n - 1, l, r);
    }
    T range_sum(int l, int r) { return query(l, r).sum; }
    T range_max(int l, int r) { return query(l, r).mx; }
    T range_min(int l, int r) { return query(l, r).mn; }
    T get(int p) { return query(p, p).sum; }
    Node get_node(int p) { return query(p, p); }
    Node get_all() { return tr[1]; }
    int size() { return n; }
    vector<T> to_vec() {
        vector<T> a(n);
        for (int i = 0; i < n; ++i) a[i] = get(i);
        return a;
    }
    /*-----------------------------------------binary search on tree--------------------------------------------------*/
    template<class F>
    int find_first(int u, int l, int r, int ql, int qr, F check) {
        if (qr < l || r < ql) return n;
        if (ql <= l && r <= qr && !check(tr[u])) return n;
        if (l == r) return l;
        int mid = (l + r) >> 1;
        int res = find_first(u << 1, l, mid, ql, qr, check);
        if (res != n) return res;
        return find_first(u << 1 | 1, mid + 1, r, ql, qr, check);
    }
    template<class F>
    int find_first(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return n;
        return find_first(1, 0, n - 1, l, r, check);
    }
    template<class F>
    int find_last(int u, int l, int r, int ql, int qr, F check) {
        if (qr < l || r < ql) return -1;
        if (ql <= l && r <= qr && !check(tr[u])) return -1;
        if (l == r) return l;
        int mid = (l + r) >> 1;
        int res = find_last(u << 1 | 1, mid + 1, r, ql, qr, check);
        if (res != -1) return res;
        return find_last(u << 1, l, mid, ql, qr, check);
    }
    template<class F>
    int find_last(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return -1;
        return find_last(1, 0, n - 1, l, r, check);
    }
    template<class F>
    int bisect_right(int u, int l, int r, int ql, int qr, Node& cur, F check) {
        if (qr < l || r < ql) return n;
        if (ql <= l && r <= qr) {
            Node nxt = merge(cur, tr[u]);
            if (check(nxt)) {
                cur = nxt;
                return n;
            }
            if (l == r) return l;
        }
        int mid = (l + r) >> 1;
        int res = bisect_right(u << 1, l, mid, ql, qr, cur, check);
        if (res != n) return res;
        return bisect_right(u << 1 | 1, mid + 1, r, ql, qr, cur, check);
    }
    template<class F>
    int bisect_right(int l, F check) {
        l = max(l, 0);
        if (l >= n) return n;
        Node cur = E;
        return bisect_right(1, 0, n - 1, l, n - 1, cur, check);
    }
    template<class F>
    int bisect_right(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return min(l, n);
        Node cur = E;
        int res = bisect_right(1, 0, n - 1, l, r, cur, check);
        return res == n ? r + 1 : res;
    }
    template<class F>
    int bisect_left(int u, int l, int r, int ql, int qr, Node& cur, F check) {
        if (qr < l || r < ql) return -1;
        if (ql <= l && r <= qr) {
            Node nxt = merge(tr[u], cur);
            if (check(nxt)) {
                cur = nxt;
                return -1;
            }
            if (l == r) return l;
        }
        int mid = (l + r) >> 1;
        int res = bisect_left(u << 1 | 1, mid + 1, r, ql, qr, cur, check);
        if (res != -1) return res;
        return bisect_left(u << 1, l, mid, ql, qr, cur, check);
    }
    template<class F>
    int bisect_left(int r, F check) {
        r = min(r, n - 1);
        if (r < 0) return -1;
        Node cur = E;
        return bisect_left(1, 0, n - 1, 0, r, cur, check);
    }
    template<class F>
    int bisect_left(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return r;
        Node cur = E;
        int res = bisect_left(1, 0, n - 1, l, r, cur, check);
        return res == -1 ? l - 1 : res;
    }
};
