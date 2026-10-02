#include <bits/stdc++.h>
using namespace std;

struct LCA {
    int n;
    vector<int> depth, first, lg, euler, father;
    vector<int> dfn;
    int timer;
    vector<vector<int>> st;
    void dfs(vector<vector<int>>& g, int u, int fa) {
        first[u] = euler.size();
        euler.emplace_back(u);
        dfn[u] = timer++;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            depth[v] = depth[u] + 1;
            father[v] = u;
            dfs(g, v, u);
            euler.emplace_back(u);
        }
    }
    LCA() {}
    LCA(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        timer = 0;
        depth.resize(n);
        dfn.resize(n);
        first.assign(n, -1);
        father.resize(n, -1);
        dfs(g, root, -1);
        int m = euler.size();
        lg.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) lg[i] = lg[i >> 1] + 1;
        int K = lg[m] + 1;
        st.assign(K, vector<int>(m));
        st[0] = euler;
        for (int k = 1; k < K; ++k) {
            for (int i = 0; i + (1 << k) <= m; ++i) {
                int x = st[k - 1][i];
                int y = st[k - 1][i + (1 << (k - 1))];
                st[k][i] = depth[x] < depth[y] ? x : y;
            }
        }
    }
    int getLCA(int x, int y) {
        int l = first[x], r = first[y];
        if (l > r) swap(l, r);
        int k = lg[r - l + 1];
        int a = st[k][l];
        int b = st[k][r - (1 << k) + 1];
        return depth[a] < depth[b] ? a : b;
    }
    int dist(int x, int y) {
        int z = getLCA(x, y);
        return depth[x] + depth[y] - 2 * depth[z];
    }
    bool onPath(int x, int u, int v) {
        return dist(u, x) + dist(x, v) == dist(u, v);
    }
};

template<class T>
struct ChairmanTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum;
        int ls, rs;
    };
    int L, R;
    Node E = {0, 0, 0};
    Node Z = {0, 0, 0};
    vector<Node> tr;
    ChairmanTree() {}
    ChairmanTree(int _L, int _R) {
        init(_L, _R);
    }
    void init(int _L, int _R) {
        L = _L, R = _R;
        tr.clear();
        tr.reserve(1 << 20);
        tr.push_back(Z);
    }
    Node newVal(T v) {
        return {v, 0, 0};
    }
    Node merge(Node a, Node b) {
        return {a.sum + b.sum, 0, 0};
    }
    int newNode() {
        int id = tr.size();
        tr.push_back(Z);
        return id;
    }
    int newNode(Node x) {
        int id = tr.size();
        tr.push_back(x);
        return id;
    }
    Node getNode(int u) {
        if (!u) return Z;
        return tr[u];
    }
    int clone(int u) {
        return newNode(getNode(u));
    }
    void pushup(int u) {
        Node cur = merge(getNode(tr[u].ls), getNode(tr[u].rs));
        tr[u].sum = cur.sum;
    }
    int build(int l, int r, vector<T>& a) {
        int u = newNode();
        if (l == r) {
            tr[u] = newVal(a[l]);
            return u;
        }
        int mid = l + r >> 1;
        tr[u].ls = build(l, mid, a);
        tr[u].rs = build(mid + 1, r, a);
        pushup(u);
        return u;
    }
    int modify(int u, int l, int r, int p, T v, int op) {
        int cur = clone(u);
        if (l == r) {
            if (op == 0) tr[cur] = newVal(v);
            else {
                tr[cur].sum += v;
            }
            return cur;
        }
        int mid = l + r >> 1;
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
        if (ql <= l && r <= qr) return getNode(u);
        int mid = l + r >> 1;
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
    Node get(int root, int p) {
        return query(root, p, p);
    }
    int kth(int rtu, int rtv, int rtp, int rtfp, int l, int r, int k) {
        if (l == r) return l;
        int mid = l + r >> 1;
        int cnt = tr[tr[rtu].ls].sum + tr[tr[rtv].ls].sum - tr[tr[rtp].ls].sum - tr[tr[rtfp].ls].sum;
        if (k <= cnt) return kth(tr[rtu].ls, tr[rtv].ls, tr[rtp].ls, tr[rtfp].ls, l, mid, k);
        else return kth(tr[rtu].rs, tr[rtv].rs, tr[rtp].rs, tr[rtfp].rs, mid + 1, r, k - cnt);
    }
    int kth(int rtu, int rtv, int rtp, int rtfp, int k) {
        return kth(rtu, rtv, rtp, rtfp, L, R, k);
    }
};

/**
 * 给定一颗点权树，每次询问 u, v 之间路径上点权第 k 小值
 * 
 * 对于值域第 k 小问题，考虑主席树（每个点建立一个权值线段树，可持久化维护他们）差分维护
 * 在树上，建立主席树维护前缀权值统计数：tr[0] 代表空前缀，tr[u] 代表树上链 0->u-1 的前缀值域线段树
 * 那么由树上差分可知，u -> v 路径上的权值统计信息即可由 tr[u] + tr[v] - tr[lca(u,v)] - tr[father[lca(u,v)]] 得到，树上二分即可
 * 
 * 注意：如果是边权，一样的处理，将边权挂到深度更大的一侧的点权上，然后执行相同的差分树上二分（只不过变为 -2*tr[lca(u,v)]）
 */

struct staticKthTreePath {
    int n;
    vector<int> father;
    vector<int> xs;
    vector<int> rt;
    ChairmanTree<int> tr;
    LCA* lca;
    staticKthTreePath(vector<vector<int>>& g, vector<int>& w) {
        n = g.size();
        lca = new LCA(g);
        
        xs.reserve(n);
        for (auto& x : w) xs.emplace_back(x);
        ranges::sort(xs); xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int tot = xs.size();
        auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
        for (auto& x : w) x = find(x);

        father.resize(n, -1);
        rt.resize(n);
        tr.init(0, tot - 1);
        auto dfs = [&](auto&& dfs, int u, int fa) -> void {
            this->rt[u] = this->tr.apply(fa == -1 ? 0 : this->rt[fa], w[u], 1);
            for (auto& v : g[u]) {
                if (v == fa) continue;
                this->father[v] = u;
                dfs(dfs, v, u);
            }
        };
        dfs(dfs, 0, -1);
    }
    int query(int u, int v, int k) {
        int p = lca->getLCA(u, v), fp = father[p];
        int id = tr.kth(rt[u], rt[v], rt[p], fp == -1 ? 0 : rt[fp], k);
        return xs[id];
    }
};