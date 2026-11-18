#include <bits/stdc++.h>
using namespace std;

/**
 * 动态开点线段树（带懒标记）
 *
 * 操作：
 * DynamicSegTree(L, R)：初始化动态线段树，值域保证为 [L,R]
 * apply(l, r, v)：把区间 [l, r] 全部加上 v
 * set(p, v)：把单点 p 设成 v
 * query(l, r)：查询区间 [l, r] 的 Node
 * 
 * 注意：值域区间是 [L, R]，由构造函数传入
 * E 是单位元节点，Z(l, r) 代表区间 [l, r] 节点的初始值
 * 
 * 复杂度：
 * 单次区间修改 / 查询 O(logV)
 * 空间 O(操作次数 * log 值域)
 * 前置条件：L<=R；修改点、修改区间与查询区间都在 [L,R] 内，且区间非空
 */
template<class T>
struct DynamicSegTree {
    // -----------------------------修改开始----------------------------------
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
    void pushup(int u) {  // 不能用 merge，否则会丢失儿子指针
        Node &cur = tr[u], &lson = tr[tr[u].ls], &rson = tr[tr[u].rs];
        cur.sum = lson.sum + rson.sum;
        // cur.len 在创建 new_node 时就已经确定，不用修改
    }
    // 按需要修改懒标记
    vector<T> add_tag;
    void apply_add(int u, T l, T r, T v) {
        tr[u].sum += v * tr[u].len;
        add_tag[u] += v;
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
        add_tag.clear(); add_tag.reserve(1 << 20); add_tag.push_back(0);
        rt = new_node(L, R);
    }
    int new_node(T l, T r) {
        int id = tr.size();
        tr.push_back(Z(l, r));
        add_tag.push_back(0);
        return id;
    }
    void pushdown(int u, T l, T r) {
        if (add_tag[u] == 0) return;
        T mid = midpoint(l, r);
        if (!tr[u].ls) tr[u].ls = new_node(l, mid);
        if (!tr[u].rs) tr[u].rs = new_node(mid + 1, r);
        apply_add(tr[u].ls, l, mid, add_tag[u]);
        apply_add(tr[u].rs, mid + 1, r, add_tag[u]);
        add_tag[u] = 0;
    }
    void modify(int u, T l, T r, T ql, T qr, T v) {
        if (ql <= l && r <= qr) return apply_add(u, l, r, v);
        pushdown(u, l, r);
        T mid = midpoint(l, r);
        if (ql <= mid) {
            if (!tr[u].ls) tr[u].ls = new_node(l, mid);
            modify(tr[u].ls, l, mid, ql, qr, v);
        }
        if (qr > mid) {
            if (!tr[u].rs) tr[u].rs = new_node(mid + 1, r);
            modify(tr[u].rs, mid + 1, r, ql, qr, v);
        }
        pushup(u);
    }
    void apply(T l, T r, T v) {
        if (l > r) return;
        l = max(l, L), r = min(r, R);
        if (l > r) return;
        modify(rt, L, R, l, r, v);
    }
    void apply(T p, T v) { apply(p, p, v); }
    void set(T p, T v) {
        T cur = get(p).sum;
        apply(p, p, v - cur);
    }
    Node query(int u, T l, T r, T ql, T qr) {
        if (!u) return {min(r, qr) - max(l, ql) + 1, 0, 0, 0};
        if (ql <= l && r <= qr) return tr[u];
        pushdown(u, l, r);
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