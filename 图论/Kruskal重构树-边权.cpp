#include <bits/stdc++.h>
using namespace std;

/**
 * Kruskal 重构树（边权版）
 *
 * 给定无向图，边为 {u, v, w}，点编号为 0 到 n - 1。按边权从小到大做 Kruskal，每次合并两个连通块时新建节点 x，令 val[x] = w。
 * 原图点为叶子，x 的子树表示加入所有边权 <= val[x] 的边后形成的连通块。
 *
 * 功能实现：
 * query(u, v)：返回 u 到 v 的最小瓶颈路，即 val[LCA(u, v)]
 * connected(u, v, lim)：判断只保留边权 <= lim 的边时是否连通
 * get_lca(u, v)：返回重构树上 u、v 的 LCA
 * get_top(u, lim)：返回 u 的最高祖先 x，满足 val[x] <= lim
 * range(u, lim)：返回该连通块对应的叶子序区间 [L, R]
 * size(u, lim)：返回该连通块中的原图点数量
 * id[u]：原图点 u 在叶子序中的位置
 * rid[i]：叶子序位置 i 对应的原图点
 *
 * 初始化：
 * init 默认为 true，此时建树后立即完成全部预处理。
 * init = false 时只建立重构树本体，后续查询会按需进行惰性预处理：
 * prepare_leaf()：建立 id / rid / L / R
 * prepare_lift()：建立叶子序与倍增表
 * prepare_rmq()：建立叶子序与 RMQ
 * prepare()：完成全部预处理
 *
 * 复杂度：
 * 仅建树复杂度 O(E log E + n)，空间复杂度 O(n)
 * prepare_leaf：O(n) 时间和空间
 * prepare_lift：O(n log n) 时间和空间
 * prepare_rmq：O(n log n) 时间和空间
 * 完成对应预处理后，query / connected：O(1)；get_lca / get_top / range / size：O(log n)
 *
 * 注意：
 * 1. 原图点权为 a[u] 时，可令 b[id[u]] = a[u]，此后连通块信息均可转化为叶子序上的区间查询。
 * 2. init = false 且需要直接访问 id / rid / L / R 时，应先调用 prepare_leaf()。
 * 3. 默认处理边权 <= lim 的连通性，原图边权必须小于 INF。
 * 4. 若需要处理边权 >= lim，应将边权改为从大到小排序，并反向修改判断条件：
 *      保留的边      排序        虚拟根     RMQ     get_top / connected
 *      w <= lim      从小到大    INF        max     <= lim
 *      w >= lim      从大到小    NINF       min     >= lim
 */
const int INF = 0x3f3f3f3f;
struct KruskalTree {
    int n, tot, root, m, timer, rmq_n;
    bool leaf_ready, lift_ready, rmq_ready;
    vector<int> val, parent, head, nxt, id, rid, L, R, pa, off, st;
    int bit_width(int x) { return 32 - __builtin_clz(x); }
    KruskalTree() {}
    KruskalTree(int _n, vector<array<int, 3>> edges, bool init = true) { build(_n, move(edges), init); }
    void clear_preprocess() {
        vector<int>().swap(id);
        vector<int>().swap(rid);
        vector<int>().swap(L);
        vector<int>().swap(R);
        vector<int>().swap(pa);
        vector<int>().swap(off);
        vector<int>().swap(st);
    }
    void build_rmq(const vector<int>& a) {
        rmq_n = a.size();
        if (rmq_n == 0) {
            off.clear();
            st.clear();
            return;
        }
        int h = bit_width(rmq_n);
        off.assign(h + 1, 0);
        for (int k = 0; k < h; ++k) off[k + 1] = off[k] + rmq_n - (1 << k) + 1;
        st.assign(off[h], 0);
        for (int i = 0; i < rmq_n; ++i) st[i] = a[i];
        for (int k = 1; k < h; ++k) {
            int len = 1 << k, half = len >> 1;
            for (int i = 0; i + len <= rmq_n; ++i) {
                st[off[k] + i] = max(st[off[k - 1] + i], st[off[k - 1] + i + half]);
            }
        }
    }
    void build(int _n, vector<array<int, 3>> edges, bool init = true) {
        n = _n, tot = n, root = -1, m = timer = rmq_n = 0;
        leaf_ready = lift_ready = rmq_ready = false;
        clear_preprocess();
        int cap = 2 * n;
        val.assign(cap, 0);
        parent.assign(cap, -1);
        vector<int> fa(cap);
        iota(fa.begin(), fa.end(), 0);
        auto find = [&](int x) -> int {
            int r = x;
            while (fa[r] != r) r = fa[r];
            while (fa[x] != x) {
                int p = fa[x];
                fa[x] = r, x = p;
            }
            return r;
        };
        ranges::sort(edges, [&](auto& a, auto& b) { return a[2] < b[2]; });
        for (auto& e : edges) {
            auto [u, v, w] = e;
            int fu = find(u), fv = find(v);
            if (fu == fv) continue;
            int x = tot++;
            val[x] = w;
            parent[fu] = parent[fv] = x;
            fa[fu] = fa[fv] = x;
            fa[x] = x;
        }
        vector<int> roots;
        for (int i = 0; i < tot; ++i) {
            if (fa[i] == i) roots.emplace_back(i);
        }
        if (roots.size() == 1) {
            root = roots[0];
        } else {
            root = tot++;
            val[root] = INF;
            for (auto& x : roots) parent[x] = root;
        }
        vector<array<int, 3>>().swap(edges);
        val.resize(tot);
        parent.resize(tot);
        head.assign(tot, -1);
        nxt.assign(tot, -1);
        for (int u = 0; u < tot; ++u) {
            if (parent[u] == -1) continue;
            nxt[u] = head[parent[u]];
            head[parent[u]] = u;
        }
        if (init) prepare();
    }
    void prepare_leaf() {
        if (leaf_ready) return;
        timer = 0;
        id.assign(n, -1);
        rid.assign(n, -1);
        L.assign(tot, INF);
        R.assign(tot, -1);
        vector<int> stk;
        stk.reserve(tot);
        stk.emplace_back(root);
        while (!stk.empty()) {
            int u = stk.back();
            stk.pop_back();
            if (u < n) id[u] = timer, rid[timer++] = u;
            for (int v = head[u]; v != -1; v = nxt[v]) stk.emplace_back(v);
        }
        for (int u = 0; u < tot; ++u) {
            if (u < n) {
                L[u] = R[u] = id[u];
                continue;
            }
            for (int v = head[u]; v != -1; v = nxt[v]) {
                L[u] = min(L[u], L[v]);
                R[u] = max(R[u], R[v]);
            }
        }
        leaf_ready = true;
    }
    void prepare_lift() {
        if (lift_ready) return;
        prepare_leaf();
        m = bit_width(tot);
        pa.assign(tot * m, -1);
        for (int u = tot - 1; u >= 0; --u) {
            pa[u * m] = parent[u];
            for (int i = 1; i < m; ++i) {
                int f = pa[u * m + i - 1];
                if (f != -1) pa[u * m + i] = pa[f * m + i - 1];
            }
        }
        lift_ready = true;
    }
    void prepare_rmq() {
        if (rmq_ready) return;
        prepare_leaf();
        vector<int> cut(n - 1, 0);
        for (int u = n; u < tot; ++u) {
            for (int v = head[u]; v != -1; v = nxt[v]) {
                if (R[v] != R[u]) cut[R[v]] = val[u];
            }
        }
        build_rmq(cut);
        rmq_ready = true;
    }
    void prepare() {
        prepare_lift();
        prepare_rmq();
    }
    bool is_ancestor(int u, int v) {
        prepare_leaf();
        return L[u] <= L[v] && R[v] <= R[u];
    }
    int get_lca(int u, int v) {
        prepare_lift();
        if (L[u] <= L[v] && R[v] <= R[u]) return u;
        if (L[v] <= L[u] && R[u] <= R[v]) return v;
        for (int i = m - 1; i >= 0; --i) {
            int f = pa[u * m + i];
            if (f != -1 && !(L[f] <= L[v] && R[v] <= R[f])) u = f;
        }
        return pa[u * m];
    }
    int query(int u, int v) {
        if (u == v) return 0;
        prepare_rmq();
        int l = id[u], r = id[v];
        if (l > r) swap(l, r);
        int k = 31 - __builtin_clz(r - l);
        return max(st[off[k] + l], st[off[k] + r - (1 << k)]);
    }
    bool connected(int u, int v, int lim) {
        if (u == v) return true;
        int w = query(u, v);
        return w != INF && w <= lim;
    }
    int get_top(int u, int lim) {
        prepare_lift();
        for (int i = m - 1; i >= 0; --i) {
            int f = pa[u * m + i];
            if (f != -1 && val[f] != INF && val[f] <= lim) u = f;
        }
        return u;
    }
    pair<int, int> range(int u, int lim) {
        int x = get_top(u, lim);
        return {L[x], R[x]};
    }
    int size(int u, int lim) {
        auto [l, r] = range(u, lim);
        return r - l + 1;
    }
};