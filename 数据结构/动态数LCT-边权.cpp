#include <bits/stdc++.h>
using namespace std;

/**
 * Link-Cut Tree / 动态树
 *
 * LinkCutTree(n)：建立 n 个点，初始没有边
 * connected(x, y)：判断 x、y 是否连通
 * link(x, y, w)：连接权值为 w 的边 x-y，成功返回 true，原本连通返回 false
 * cut(x, y)：删除边 x-y，成功返回 true，该边不存在返回 false
 * makeroot(x)：把 x 设为所在树的根
 * findroot(x)：返回 x 所在树的根
 * split(x, y)：暴露路径 x-y，之后整条路径信息位于 tr[y]
 * modify(x, y, w)：把边 x-y 的权值修改为 w，成功返回 true，该边不存在返回 false
 * path_sum(x, y)：查询路径 x-y 的边权和
 * path_max(x, y)：查询路径 x-y 的最大边权
 * path_min(x, y)：查询路径 x-y 的最小边权
 * path_xor(x, y)：查询路径 x-y 的边权异或和
 * path_size(x, y)：查询路径 x-y 的边数
 *
 * 使用额外节点表示边，原树节点不参与路径信息统计
 * 删除边后会回收对应的边节点，空间 O(n)
 * 除构造外所有操作均摊 O(log n)
 * 路径查询要求 x、y 已连通
 * 前置条件：所有原树点编号位于 [0,n-1]
 * path_max(x, x) 和 path_min(x, x) 返回 0
 * T 必须为支持异或的整数类型
 */
template<class T>
struct LinkCutTree {
    struct Node {
        int ch[2], fa, sz;
        bool rev, edge;
        T val, sum, mx, mn, xr;
    };
    int n;
    vector<Node> tr;
    vector<int> stk, rec;
    unordered_map<long long, int> id;
    LinkCutTree(int _n) : n(_n), tr(n), stk(2 * n) {
        tr.reserve(2 * n);
        id.reserve(2 * n);
        for (int i = 0; i < n; ++i) {
            tr[i] = {{-1, -1}, -1, 0, false, false, 0, 0, numeric_limits<T>::lowest(), numeric_limits<T>::max(), 0};
        }
    }
    long long get_key(int x, int y) {
        if (x > y) swap(x, y);
        return (1LL * x << 32) ^ y;
    }
    void reset(int x, T v) {
        tr[x] = {{-1, -1}, -1, 1, false, true, v, v, v, v, v};
    }
    int new_edge(T v) {
        int x;
        if (!rec.empty()) {
            x = rec.back(); rec.pop_back();
            reset(x, v);
        } else {
            x = tr.size();
            tr.push_back({{-1, -1}, -1, 1, false, true, v, v, v, v, v});
        }
        return x;
    }
    bool isroot(int x) {
        int f = tr[x].fa;
        return f == -1 || (tr[f].ch[0] != x && tr[f].ch[1] != x);
    }
    void pull(int x) {
        auto& u = tr[x];
        u.sz = u.edge;
        u.sum = u.xr = u.edge ? u.val : 0;
        u.mx = u.edge ? u.val : numeric_limits<T>::lowest();
        u.mn = u.edge ? u.val : numeric_limits<T>::max();
        for (int d = 0; d < 2; ++d) {
            int y = u.ch[d];
            if (y == -1) continue;
            u.sz += tr[y].sz;
            u.sum += tr[y].sum;
            u.mx = max(u.mx, tr[y].mx);
            u.mn = min(u.mn, tr[y].mn);
            u.xr ^= tr[y].xr;
        }
    }
    void apply_rev(int x) {
        if (x == -1) return;
        swap(tr[x].ch[0], tr[x].ch[1]);
        tr[x].rev ^= 1;
    }
    void pushdown(int x) {
        if (!tr[x].rev) return;
        apply_rev(tr[x].ch[0]);
        apply_rev(tr[x].ch[1]);
        tr[x].rev = false;
    }
    void rotate(int x) {
        int y = tr[x].fa;
        int z = tr[y].fa;
        int d = tr[y].ch[1] == x;
        int b = tr[x].ch[d ^ 1];
        if (!isroot(y)) {
            tr[z].ch[tr[z].ch[1] == y] = x;
        }
        tr[x].fa = z;
        tr[x].ch[d ^ 1] = y;
        tr[y].fa = x;
        tr[y].ch[d] = b;
        if (b != -1) tr[b].fa = y;
        pull(y);
        pull(x);
    }
    void splay(int x) {
        int y = x, top = 0;
        stk[top++] = y;
        while (!isroot(y)) {
            y = tr[y].fa;
            stk[top++] = y;
        }
        while (top) pushdown(stk[--top]);
        while (!isroot(x)) {
            int y = tr[x].fa;
            int z = tr[y].fa;
            if (!isroot(y)) {
                if ((tr[y].ch[1] == x) == (tr[z].ch[1] == y)) {
                    rotate(y);
                } else {
                    rotate(x);
                }
            }
            rotate(x);
        }
    }
    void access(int x) {
        int y = -1;
        while (x != -1) {
            splay(x);
            tr[x].ch[1] = y;
            if (y != -1) tr[y].fa = x;
            pull(x);
            y = x;
            x = tr[x].fa;
        }
    }
    void makeroot(int x) {
        access(x);
        splay(x);
        apply_rev(x);
    }
    int findroot(int x) {
        access(x);
        splay(x);
        pushdown(x);
        while (tr[x].ch[0] != -1) {
            x = tr[x].ch[0];
            pushdown(x);
        }
        splay(x);
        return x;
    }
    bool connected(int x, int y) {
        if (x == y) return true;
        makeroot(x);
        return findroot(y) == x;
    }
    bool link_node(int x, int y) {
        makeroot(x);
        if (findroot(y) == x) return false;
        tr[x].fa = y;
        return true;
    }
    bool cut_node(int x, int y) {
        makeroot(x);
        access(y);
        splay(y);
        if (tr[y].ch[0] != x || tr[x].ch[1] != -1) return false;
        tr[y].ch[0] = -1;
        tr[x].fa = -1;
        pull(y);
        return true;
    }
    bool link(int x, int y, T w) {
        if (connected(x, y)) return false;
        int e = new_edge(w);
        link_node(x, e);
        link_node(e, y);
        id[get_key(x, y)] = e;
        return true;
    }
    bool cut(int x, int y) {
        auto it = id.find(get_key(x, y));
        if (it == id.end()) return false;
        int e = it->second;
        cut_node(x, e);
        cut_node(e, y);
        id.erase(it);
        rec.emplace_back(e);
        return true;
    }
    bool modify(int x, int y, T v) {
        auto it = id.find(get_key(x, y));
        if (it == id.end()) return false;
        int e = it->second;
        access(e);
        splay(e);
        tr[e].val = v;
        pull(e);
        return true;
    }
    void split(int x, int y) {
        makeroot(x);
        access(y);
        splay(y);
    }
    T path_sum(int x, int y) {
        split(x, y);
        return tr[y].sum;
    }
    T path_max(int x, int y) {
        if (x == y) return 0;
        split(x, y);
        return tr[y].mx;
    }
    T path_min(int x, int y) {
        if (x == y) return 0;
        split(x, y);
        return tr[y].mn;
    }
    T path_xor(int x, int y) {
        split(x, y);
        return tr[y].xr;
    }
    int path_size(int x, int y) {
        split(x, y);
        return tr[y].sz;
    }
};
