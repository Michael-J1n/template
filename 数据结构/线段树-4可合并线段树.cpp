#include <bits/stdc++.h>
using namespace std;

/**
 * 线段树合并模板：动态开点权值线段树
 * 常用于权值线段树在各个节点维护可重集的场景
 *
 * 使用场景：
 * 每个点（子树根/并查集连通块）维护一棵权值线段树
 * 需要把两个集合的信息合并，例如树上子树信息合并；常用于维护值域上的出现次数、最大值、最小值、第 k 小等
 *
 * 功能：
 * add(rt, x, v)：在 rt 这棵线段树中，把值 x 的权值加 v；第一次被 add 会动态分配 rt
 * merge(x, y)：把 y 这棵线段树合并进 x，返回合并后的根
 * query(rt, l, r)：查询 rt 中值域 [l, r] 的 Node
 * kth(rt, k)：查询 rt 中第 k 小的值
 * 
 * 注意：
 * 1. merge(x, y) 是破坏性合并，返回合并后的根，保留 x，丢弃 y
 * 2. 两棵线段树必须维护同一个值域 [L, R]
 * 3. E 是空查询单位元，Z 是未开点区间的真实零值
 * 4. kth 按 sum 查询第 k 小，要求权值是非负频率语义
 * 5. 一般配合 vector<int> rt(n) 使用；一开始 rt 置零即可，在后续 add 时再动态开点
 *      初始化：seg.add(rt[u], a[u], 1)
 *      合并：rt[u] = seg.merge(rt[u], rt[v])
 * 6. 前置条件：L<=R；add 的 x 与 query 区间位于 [L,R] 内；查询区间非空；kth 的树非空且 k 合法；不得把同一个非零根与自身合并
 * 
 * 复杂度：
 * 单次 add：O(logV)
 * 单次 kth / query：O(logV)
 * 所有 merge 总复杂度：O(总开点数)，因为每个被合并掉的节点最多被访问一次
 */
template<class T>
struct MergeSegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum, mx, mn;
        int ls, rs;
    };
    int L, R;
    Node E = {0, -INF, INF, 0, 0};
    Node Z = {0, 0, 0, 0, 0};
    vector<Node> tr;
    MergeSegTree() {}
    MergeSegTree(int _L, int _R) {
        init(_L, _R);
    }
    void init(int _L, int _R) {
        L = _L, R = _R;
        tr.clear();
        tr.reserve(1 << 20);
        tr.push_back(Z);
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
    void add(int& u, int l, int r, int x, T v) {
        if (!u) u = new_node();
        if (l == r) {
            tr[u].sum += v;
            tr[u].mx += v;
            tr[u].mn += v;
            return;
        }
        int mid = midpoint(l, r);
        if (x <= mid) {
            int p = tr[u].ls;
            add(p, l, mid, x, v);
            tr[u].ls = p;
        } else {
            int p = tr[u].rs;
            add(p, mid + 1, r, x, v);
            tr[u].rs = p;
        }
        pushup(u);
    }
    void add(int& rt, int x, T v = 1) {
        add(rt, L, R, x, v);
    }
    int merge(int x, int y, int l, int r) {
        if (!y) return x;
        if (!x) return y;
        if (l == r) {
            tr[x].sum += tr[y].sum;
            tr[x].mx += tr[y].mx;
            tr[x].mn += tr[y].mn;
            return x;
        }
        int mid = midpoint(l, r);
        tr[x].ls = merge(tr[x].ls, tr[y].ls, l, mid);
        tr[x].rs = merge(tr[x].rs, tr[y].rs, mid + 1, r);
        pushup(x);
        return x;
    }
    int merge(int x, int y) {
        return merge(x, y, L, R);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return E;
        if (!u) return Z;
        if (ql <= l && r <= qr) return tr[u];
        int mid = midpoint(l, r);
        return merge(query(tr[u].ls, l, mid, ql, qr), query(tr[u].rs, mid + 1, r, ql, qr));
    }
    Node query(int rt, int l, int r) {
        if (l > r) return E;
        l = max(l, L), r = min(r, R);
        if (l > r) return E;
        return query(rt, L, R, l, r);
    }
    int kth(int u, int l, int r, T k) {
        if (l == r) return l;
        int mid = midpoint(l, r);
        T left = tr[tr[u].ls].sum;
        if (k <= left) return kth(tr[u].ls, l, mid, k);
        return kth(tr[u].rs, mid + 1, r, k - left);
    }
    int kth(int rt, T k) {
        if (!rt || k <= 0 || tr[rt].sum < k) return -1;
        return kth(rt, L, R, k);
    }
};
