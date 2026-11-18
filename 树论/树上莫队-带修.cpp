#include <bits/stdc++.h>
using namespace std;

/**
 * 树上莫队（带修，点权）
 * 将树上路径转成欧拉序区间，再用带修莫队移动左右指针和时间
 *
 * 状态定义：
 * Event(op, l, r, id, extra)：op = 0 表示修改节点点权 a[l] = r；op = 1 表示查询欧拉序区间 [l, r]
 * a[u]：当前时间下节点 u 的点权
 * vis[u]：节点 u 在当前欧拉序区间中是否出现奇数次（偶数次被抵消）
 * T：当前已经执行了前 T 次修改
 *
 * 复杂度：
 * 欧拉序长度 m = 2n，B 取 m^(2/3)；询问数、修改数都与 n 同阶，增删 O(1) 时，总时间约 O(n^(5/3))
 * 当修改查询不平衡时，可以有更精细的公式 B ≈ max(m/sqrt(q), (u m²/q)^(1/3))，u 为修改次数
 * 树上预处理 O(n log n)，每条路径转成区间 O(log n)，空间 O(n log n + q + u)
 *
 * 注意：
 * 1. 先建立 preprocess(g, root)，查询用 ord.query(u, v, id) 转换，修改仍使用原树节点编号
 * 2. evs 按题目原顺序传入，查询 id 必须为 0, 1,..., querySize - 1（！！！）
 */
struct Event {
    int op, l, r, id, extra = -1;   // modify: {0, u, val, 0}; query: {1, l, r, id, extra}
};
// 预处理：建立进出序和倍增表 O(n log n)，每条路径转成区间 O(log n)
struct preprocess {
    int n, K;
    vector<int> depth, in, out, seq;
    vector<vector<int>> up;
    preprocess(vector<vector<int>>& g, int root = 0) {
        n = g.size();
        K = 1;
        while ((1LL << K) <= n) ++K;
        up.assign(K, vector<int>(n, root));
        depth.resize(n), in.resize(n), out.resize(n);
        seq.reserve(2 * n);
        vector<int> st; st.reserve(n);
        st.emplace_back(root);
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            if (u >= 0) {
                in[u] = seq.size();
                seq.emplace_back(u);
                for (int k = 1; k < K; ++k) up[k][u] = up[k - 1][up[k - 1][u]];
                st.emplace_back(~u);
                for (int i = int(g[u].size()) - 1; ~i; --i) {
                    int v = g[u][i];
                    if (v == up[0][u]) continue;
                    up[0][v] = u;
                    depth[v] = depth[u] + 1;
                    st.emplace_back(v);
                }
            } else {
                u = ~u;
                out[u] = seq.size();
                seq.emplace_back(u);
            }
        }
    }
    int get_lca(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        int d = depth[u] - depth[v];
        for (int k = 0; k < K; ++k) {
            if (d >> k & 1) u = up[k][u];
        }
        if (u == v) return u;
        for (int k = K - 1; k >= 0; --k) {
            if (up[k][u] != up[k][v]) u = up[k][u], v = up[k][v];
        }
        return up[0][u];
    }
    Event query(int u, int v, int id) const {
        if (in[u] > in[v]) swap(u, v);
        int p = get_lca(u, v);
        if (p == u) return {1, in[u], in[v], id, -1};
        return {1, out[u], in[v], id, p};
    }
};

vector<int> moAlgorithmWithModify(vector<int>& orig_a, vector<Event>& evs, const vector<int>& seq) {
    int n = orig_a.size();
    // 离散化
    vector<int> xs = orig_a;
    for (auto& e : evs) {
        if (e.op == 0) xs.emplace_back(e.r);
    }
    sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
    int tot = xs.size();
    auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
    vector<int> b = orig_a;
    for (int i = 0; i < n; ++i) b[i] = find(b[i]);
    vector<Event> ms, qs;    // ms: {0, u, val, old}，qs: {t, l, r, id, extra}
    vector<int> a = b;
    int updates = 0, q = 0;
    for (auto& e : evs) {
        if (e.op == 0) {
            int val = find(e.r);
            ms.push_back({0, e.l, val, a[e.l]});
            a[e.l] = val;
            ++updates;
        } else {
            qs.push_back({updates, e.l, e.r, e.id, e.extra});
            ++q;
        }
    }
    a = b;
    // 对查询分块；蛇形排序
    int m = seq.size();
    int B = pow(m, 2.0 / 3) + 1;
    // int B1 = max(1, int(m / sqrt(max(1, q)))), B2 = max(1, int(cbrt(1.0 * max(1, updates) * m * m / max(1, q)))), B = max(B1, B2);
    ranges::sort(qs, [&](auto& x, auto& y) {
        int bx = x.l / B, by = y.l / B;
        if (bx != by) return bx < by;
        int rx = x.r / B, ry = y.r / B;
        if (rx != ry) {
            if (bx & 1) return rx > ry;
            return rx < ry;
        }
        if (rx & 1) return x.op > y.op;
        return x.op < y.op;
    });
    /* ----------------------- 核心部分：单点维护 --------------------- */
    vector<int> cnt(tot);                   // 频率统计桶
    vector<int> res(q);                     // 询问对应的答案
    vector<int> vis(n);                     // vis[u]：节点 u 在当前欧拉序区间中是否出现奇数次（偶数次被抵消）
    int cur = 0;                            // 维护当前答案
    int L = 0, R = -1;                      // 当前区间左右端点
    auto add = [&](int u) -> void {         // 节点 u 进入统计集合
        int x = a[u];
        if (cnt[x] == 0) ++cur;
        ++cnt[x];
    };
    auto del = [&](int u) -> void {         // 节点 u 离开统计集合
        int x = a[u];
        --cnt[x];
        if (cnt[x] == 0) --cur;
    };
    /* ------------------------------------------------------------- */
    auto toggle = [&](int i) -> void {      // 翻转状态
        int u = seq[i];
        if (vis[u]) del(u);
        else add(u);
        vis[u] ^= 1;
    };
    auto apply = [&](int t) -> void {       // 执行时间 t 对应的修改
        auto& e = ms[t];
        int u = e.l, v = e.r;
        if (vis[u]) del(u);
        a[u] = v;
        if (vis[u]) add(u);
    };
    auto undo = [&](int t) -> void {        // 撤销时间 t 对应的修改
        auto& e = ms[t];
        int u = e.l, old = e.id;
        if (vis[u]) del(u);
        a[u] = old;
        if (vis[u]) add(u);
    };
    // 处理查询
    int T = 0;
    for (auto& e : qs) {
        int l = e.l, r = e.r, t = e.op, id = e.id, extra = e.extra;
        while (L > l) toggle(--L);
        while (R < r) toggle(++R);
        while (L < l) toggle(L++);
        while (R > r) toggle(R--);
        while (T < t) apply(T++);
        while (T > t) undo(--T);
        if (extra != -1) add(extra);
        res[id] = cur;
        if (extra != -1) del(extra);
    }
    return res;
}

/*
// 示例使用方式（输入节点编号为 1-based，模板内部为 0-based；0 修改，1 查询）
preprocess ord(g);
vector<Event> evs;
int q = 0;
for (int i = 0; i < m; ++i) {
    int op, u, v; cin >> op >> u >> v;
    --u;
    if (op == 0) evs.push_back({0, u, v, 0});
    else evs.push_back(ord.query(u, v - 1, q++));
}
auto res = moAlgorithmWithModify(a, evs, ord.seq);
*/
