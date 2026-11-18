#include <bits/stdc++.h>
using namespace std;

/**
 * 动态开点线段树
 * 
 * 操作：
 * DynamicSegTree(L, R)：初始化动态线段树，值域保证为 [L,R]
 * set(p, v)：把位置 p 的值设为 v
 * apply(p, v)：把位置 p 的值与 v 通过 f 函数合并
 * query(l, r)：查询区间 [l, r] 的 Node
 *
 * 注意：值域区间是 [L, R]，由构造函数传入
 * E 是单位元节点，Z(l, r) 代表区间 [l, r] 节点的初始值
 * 
 * 复杂度：
 * 单次修改 / 查询 O(logV)
 * 空间 O(访问过的点数 * log 值域)
 * 前置条件：L<=R；修改点与查询区间都在 [L,R] 内，且查询区间非空
 */
template<class T>
struct DynamicSegTree {
    // -----------------------------修改开始----------------------------------
    T f(const T& x, const T& y) {
        return x + y;
    }
    struct Node {
        T len, sum;
        int ls, rs;
    };
    Node E = {0, 0, 0, 0};
    Node Z(T l, T r) {
        return {r - l + 1, 0, 0, 0};
    }
    Node merge(Node a, Node b) {
        Node res;
        res.len = a.len + b.len;
        res.sum = a.sum + b.sum;
        res.ls = res.rs = 0;
        return res;
    }
    void pushup(int u) {
        Node &cur = tr[u], &lson = tr[tr[u].ls], &rson = tr[tr[u].rs];
        cur.sum = lson.sum + rson.sum;
        // cur.len 在创建 new_node 时就已经确定，不用修改
    }
    // -----------------------------修改结束----------------------------------
    T L, R;
    int rt;
    vector<Node> tr;
    DynamicSegTree() {}
    DynamicSegTree(T _L, T _R) { init(_L, _R); }
    void init(T _L, T _R) {
        L = _L, R = _R;
        tr.clear(); tr.reserve(1 << 20); tr.push_back(E);
        rt = new_node(L, R);
    }
    int new_node(T l, T r) {
        int id = tr.size();
        tr.push_back(Z(l, r));
        return id;
    }
    void modify(int u, T l, T r, T p, T v, int op) {
        if (l == r) {
            if (op == 0) tr[u].sum = v;
            else tr[u].sum = f(tr[u].sum, v);
            return;
        }
        T mid = midpoint(l, r);
        if (p <= mid) {
            if (!tr[u].ls) tr[u].ls = new_node(l, mid);
            modify(tr[u].ls, l, mid, p, v, op);
        } else {
            if (!tr[u].rs) tr[u].rs = new_node(mid + 1, r);
            modify(tr[u].rs, mid + 1, r, p, v, op);
        }
        pushup(u);
    }
    void set(T p, T v) {
        if (p < L || p > R) return;
        modify(rt, L, R, p, v, 0);
    }
    void apply(T p, T v) {
        if (p < L || p > R) return;
        modify(rt, L, R, p, v, 1);
    }
    Node query(int u, T l, T r, T ql, T qr) {
        if (!u) return {min(r, qr) - max(l, ql) + 1, 0, 0, 0};
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
    Node get(T p) { return query(p, p); }
};