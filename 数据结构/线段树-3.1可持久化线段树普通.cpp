#include <bits/stdc++.h>
using namespace std;

/**
 * 可持久化线段树
 * 每次修改只复制从根到叶子的一条链，旧版本不变
 *
 * 状态定义：
 * root[i] 由外部维护，表示第 i 个版本 / 第 i 个前缀 / 树上第 i 个点对应的线段树根节点
 * Node 维护区间 sum / mx / mn
 *
 * 操作：
 * init(L, R)：动态开点，不建树，适合值域 V 很大；初始 root = 0
 * init(a)：按数组建整棵树，返回初始 root，时间/空间 O(n)
 * set(root, p, v)：  基于 root，把位置 p 的值设为 v，并返回新 root
 * apply(root, p, v)：基于 root，把位置 p 的值加上 v，并返回新 root
 * query(root, l, r)：查询 root 对应版本中区间 [l, r] 的 Node
 * get(root, p)：     查询 root 对应版本中位置 p 的 Node
 *
 * 外部 root 使用：
 * 初始化 PersistentSegTree tr(0, MXV);
 * 1. 全 0 初始版本：
 *      vector<int> root(q + 1);
 *      root[0] = 0;
 *      root[i] = seg.apply(root[i - 1], p, v);
 * 2. 数组初始版本（可持久化数组）：
 *      vector<int> ver(q + 1);
 *      ver[0] = seg.init(a);                       // ver[0]: 初始数组
 *      ver[i] = seg.set(root[i - 1], p, v);        // ver[i]：第 i 次修改后的数组版本
 * 3. 主席树前缀版本（区间kth）：
 *      vector<int> root(n + 1);
 *      root[0] = 0;                                // root[0]: 空前缀值域树
 *      root[i + 1] = seg.apply(root[i], a[i], 1);  // root[i]: 前缀 a[0..i-1] 值域统计树
 * 4. 树上主席树（路径kth）：
 *      vector<int> root(n);                        // root[u]: 链 tree_root 到 u 的路径前缀值域统计树
 *      root[u] = seg.apply(fa == -1 ? 0 : root[fa], a[u], 1);
 *
 * 注意：值域区间是 [L, R]，由构造函数传入
 * E 是空查询单位元，不影响 max/min
 * Z 是未开点区间的真实零值，会参与 max / min
 *
 * 复杂度：O(V + q log V)
 * 初始化 O(n)，单次修改 / 查询 O(logn)（若没有离散化，则是 O(V) 与 O(logV)）
 * 空间 O(初始建树空间 + 修改次数 * logV)
 * 前置条件：值域满足 L<=R，数组建树时数组非空；root 合法；修改点与查询区间位于值域内，且查询区间非空
 */
template<class T>
struct PersistentSegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum, mx, mn;
        int ls, rs;
    };
    int L, R;
    Node E = {0, -INF, INF, 0, 0};
    Node Z = {0, 0, 0, 0, 0};
    vector<Node> tr;
    PersistentSegTree() {}
    PersistentSegTree(int _L, int _R) {
        init(_L, _R);
    }
    void init(int _L, int _R) {
        L = _L, R = _R;
        tr.clear();
        tr.reserve(1 << 20);
        tr.push_back(Z);
    }
    int init(vector<T>& a) {
        L = 0, R = a.size() - 1;
        tr.clear();
        tr.reserve(1 << 20);
        tr.push_back(Z);
        return build(L, R, a);
    }
    Node new_val(T v) { return {v, v, v, 0, 0}; }
    Node merge(Node a, Node b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);
        return res;
    }
    int new_node() {
        int id = tr.size();
        tr.push_back(Z);
        return id;
    }
    int new_node(Node x) {
        int id = tr.size();
        tr.push_back(x);
        return id;
    }
    Node get_node(int u) {
        if (!u) return Z;
        return tr[u];
    }
    int clone(int u) { return new_node(get_node(u)); }
    void pushup(int u) {
        Node cur = merge(get_node(tr[u].ls), get_node(tr[u].rs));
        tr[u].sum = cur.sum;
        tr[u].mx = cur.mx;
        tr[u].mn = cur.mn;
    }
    int build(int l, int r, vector<T>& a) {
        int u = new_node();
        if (l == r) {
            tr[u] = new_val(a[l]);
            return u;
        }
        int mid = midpoint(l, r);
        tr[u].ls = build(l, mid, a);
        tr[u].rs = build(mid + 1, r, a);
        pushup(u);
        return u;
    }
    int modify(int u, int l, int r, int p, T v, int op) {
        int cur = clone(u);
        if (l == r) {
            if (op == 0) tr[cur] = new_val(v);
            else {
                tr[cur].sum += v;
                tr[cur].mx += v;
                tr[cur].mn += v;
            }
            return cur;
        }
        int mid = midpoint(l, r);
        if (p <= mid) tr[cur].ls = modify(tr[cur].ls, l, mid, p, v, op);
        else tr[cur].rs = modify(tr[cur].rs, mid + 1, r, p, v, op);
        pushup(cur);
        return cur;
    }
    int set(int root, int p, T v) {
        if (p < L || p > R) return root;
        return modify(root, L, R, p, v, 0);
    }
    int apply(int root, int p, T v) {
        if (p < L || p > R) return root;
        return modify(root, L, R, p, v, 1);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return get_node(u);
        int mid = midpoint(l, r);
        if (qr <= mid) return query(tr[u].ls, l, mid, ql, qr);
        if (ql > mid) return query(tr[u].rs, mid + 1, r, ql, qr);
        return merge(query(tr[u].ls, l, mid, ql, qr), query(tr[u].rs, mid + 1, r, ql, qr));
    }
    Node query(int root, int l, int r) {
        if (l > r) return E;
        l = max(l, L), r = min(r, R);
        if (l > r) return E;
        return query(root, L, R, l, r);
    }
    Node get(int root, int p) { return query(root, p, p); }
};
