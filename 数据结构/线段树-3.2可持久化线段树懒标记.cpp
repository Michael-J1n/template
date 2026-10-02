#include <bits/stdc++.h>
using namespace std;

/**
 * 可持久化动态开点线段树；支持区间加懒标记
 * 每次修改只复制访问到的节点，旧版本不变
 *
 * 状态定义：
 * root[i] 由外部维护，表示第 i 个版本 / 结构位置对应的线段树根节点
 * Node 维护区间 sum / mx / mn
 * add_tag[u] 表示节点 u 尚未下传给儿子的区间加懒标记
 *
 * 操作：
 * init(L, R)：             动态开点，不建树，适合值域 V 很大；初始 root = 0
 * apply(root, l, r, v)：   基于 root，把区间 [l, r] 全部加上 v，并返回新 root
 * apply(root, p, v)：      基于 root，把位置 p 加上 v，并返回新 root
 * set(root, p, v)：        基于 root，把位置 p 设为 v，并返回新 root
 * query(root, l, r)：      查询 root 对应版本中区间 [l, r] 的 Node
 * get(root, p)：           查询 root 对应版本中位置 p 的 Node
 *
 * 外部 root 使用：
 * 1. 全 0 初始版本：
 *      vector<int> root(q + 1);
 *      root[0] = 0;
 *      root[i] = seg.apply(root[i - 1], l, r, v);
 * 2. 从旧版本分叉：
 *      root.push_back(seg.apply(root[old_ver], l, r, v));
 * 3. 每次基于最新版：
 *      root.push_back(seg.set(root.back(), p, v));
 *
 * 注意：值域区间是 [L, R]，由构造函数传入
 * E 是空查询单位元，不影响 max / min
 * Z 是未开点区间的真实零值，会参与 max / min
 * modify 可以 pushdown，因为当前节点已经 clone，不会修改旧版本
 * query 不能 pushdown，否则会创建 / 修改节点，因此用 tag 携带祖先懒标记
 *
 * 复杂度：单次修改 / 查询 O(log V)
 * 空间：O(q log V)
 * 前置条件：L<=R；root 合法；修改点、修改区间与查询区间位于 [L,R] 内，且区间非空
 */
template<class T>
struct PersistentLazySegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum, mx, mn;
        int ls, rs;
    };
    T L, R;
    Node E = {0, -INF, INF, 0, 0};
    Node Z = {0, 0, 0, 0, 0};
    vector<Node> tr;
    vector<T> add_tag;
    PersistentLazySegTree() {}
    PersistentLazySegTree(T _L, T _R) {
        init(_L, _R);
    }
    void init(T _L, T _R) {
        L = _L, R = _R;
        tr.clear();
        add_tag.clear();
        tr.reserve(1 << 20);
        add_tag.reserve(1 << 20);
        tr.push_back(Z);
        add_tag.push_back(0);
    }
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
        add_tag.push_back(0);
        return id;
    }
    int new_node(Node x, T tag) {
        int id = tr.size();
        tr.push_back(x);
        add_tag.push_back(tag);
        return id;
    }
    Node get_node(int u) {
        if (!u) return Z;
        return tr[u];
    }
    int clone(int u) {
        if (!u) return new_node();
        return new_node(tr[u], add_tag[u]);
    }
    void apply_add(int u, T l, T r, T v) {
        tr[u].sum += v * (r - l + 1);
        tr[u].mx += v;
        tr[u].mn += v;
        add_tag[u] += v;
    }
    void pushup(int u) {
        Node cur = merge(get_node(tr[u].ls), get_node(tr[u].rs));
        tr[u].sum = cur.sum;
        tr[u].mx = cur.mx;
        tr[u].mn = cur.mn;
    }
    void pushdown(int u, T l, T r) {
        if (add_tag[u] == 0) return;
        T mid = midpoint(l, r);
        tr[u].ls = clone(tr[u].ls);
        tr[u].rs = clone(tr[u].rs);
        apply_add(tr[u].ls, l, mid, add_tag[u]);
        apply_add(tr[u].rs, mid + 1, r, add_tag[u]);
        add_tag[u] = 0;
    }
    int modify(int u, T l, T r, T ql, T qr, T v) {
        int cur = clone(u);
        if (ql <= l && r <= qr) {
            apply_add(cur, l, r, v);
            return cur;
        }
        pushdown(cur, l, r);
        T mid = midpoint(l, r);
        if (ql <= mid) tr[cur].ls = modify(tr[cur].ls, l, mid, ql, qr, v);
        if (qr > mid) tr[cur].rs = modify(tr[cur].rs, mid + 1, r, ql, qr, v);
        pushup(cur);
        return cur;
    }
    int apply(int root, T l, T r, T v) {
        if (l > r) return root;
        l = max(l, L), r = min(r, R);
        if (l > r) return root;
        return modify(root, L, R, l, r, v);
    }
    int apply(int root, T p, T v) { return apply(root, p, p, v); }
    int set(int root, T p, T v) {
        if (p < L || p > R) return root;
        T cur = get(root, p).sum;
        return apply(root, p, v - cur);
    }
    Node add_node(Node x, T l, T r, T v) {
        x.sum += v * (r - l + 1);
        x.mx += v;
        x.mn += v;
        return x;
    }
    Node query(int u, T l, T r, T ql, T qr, T tag) {
        if (ql <= l && r <= qr) return add_node(get_node(u), l, r, tag);
        T mid = midpoint(l, r);
        if (u) tag += add_tag[u];
        if (qr <= mid) return query(tr[u].ls, l, mid, ql, qr, tag);
        if (ql > mid) return query(tr[u].rs, mid + 1, r, ql, qr, tag);
        return merge(query(tr[u].ls, l, mid, ql, qr, tag), query(tr[u].rs, mid + 1, r, ql, qr, tag));
    }
    Node query(int root, T l, T r) {
        if (l > r) return E;
        l = max(l, L), r = min(r, R);
        if (l > r) return E;
        return query(root, L, R, l, r, 0);
    }
    Node get(int root, T p) { return query(root, p, p); }
};
