#include <bits/stdc++.h>
using namespace std;

/**
 * 动态开点线段树
 * 一般都是维护权值线段树，且值域不好离散化
 * 
 * 操作：
 * set(p, v)：把位置 p 的值设为 v
 * apply(p, v)：把位置 p 的值加上 v
 * query(l, r)：查询区间 [l, r] 的 Node
 *
 * 注意：值域区间是 [L, R]，由构造函数传入
 * E 是空查询单位元，不影响 max/min
 * Z 是未开点区间的真实零值，会参与 max / min（代表没有被开点的值默认还是 0，而不是 +-inf 这样的最值单位元）
 * 
 * 复杂度：
 * 单次修改 / 查询 O(logV)
 * 空间 O(访问过的点数 * log 值域)
 * 前置条件：L<=R；修改点与查询区间都在 [L,R] 内，且查询区间非空
 */
template<class T>
struct DynamicSegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum, mx, mn;
        int ls, rs;
    };
    T L, R;
    Node E = {0, -INF, INF, 0, 0};
    Node Z = {0, 0, 0, 0, 0};
    int rt;
    vector<Node> tr;
    DynamicSegTree() {}
    DynamicSegTree(T _L, T _R) {
        init(_L, _R);
    }
    void init(T _L, T _R) {
        L = _L, R = _R;
        tr.clear();
        tr.reserve(1 << 20);
        tr.push_back(Z);
        rt = new_node();
    }
    Node new_val(T v) {
        return {v, v, v, 0, 0};
    }
    Node merge(Node a, Node b) {
        return {a.sum + b.sum, max(a.mx, b.mx), min(a.mn, b.mn), 0, 0};
    }
    int new_node() {
        int id = tr.size();
        tr.push_back(Z);
        return id;
    }
    Node get_node(int u) {
        if (!u) return Z;
        return tr[u];
    }
    void pushup(int u) {
        Node cur = merge(get_node(tr[u].ls), get_node(tr[u].rs));
        tr[u].sum = cur.sum;
        tr[u].mx = cur.mx;
        tr[u].mn = cur.mn;
    }
    void modify(int u, T l, T r, T p, T v, int op) {
        if (l == r) {
            if (op == 0) tr[u] = new_val(v);
            else {
                tr[u].sum += v;
                tr[u].mx += v;
                tr[u].mn += v;
            }
            return;
        }
        T mid = midpoint(l, r);
        if (p <= mid) {
            if (!tr[u].ls) tr[u].ls = new_node();
            modify(tr[u].ls, l, mid, p, v, op);
        } else {
            if (!tr[u].rs) tr[u].rs = new_node();
            modify(tr[u].rs, mid + 1, r, p, v, op);
        }
        pushup(u);
    }
    void set(T p, T v) {
        modify(rt, L, R, p, v, 0);
    }
    void apply(T p, T v) {
        modify(rt, L, R, p, v, 1);
    }
    Node query(int u, T l, T r, T ql, T qr) {
        if (!u) return Z;
        if (ql <= l && r <= qr) return tr[u];
        T mid = midpoint(l, r);
        if (qr <= mid) return query(tr[u].ls, l, mid, ql, qr);
        if (ql > mid) return query(tr[u].rs, mid + 1, r, ql, qr);
        return merge(query(tr[u].ls, l, mid, ql, qr), query(tr[u].rs, mid + 1, r, ql, qr));
    }
    Node query(T l, T r) {
        if (l > r) return E;
        l = max(l, L), r = min(r, R);
        if (l > r) return E;
        return query(rt, L, R, l, r);
    }
    Node get(T p) {
        return query(p, p);
    }
};
