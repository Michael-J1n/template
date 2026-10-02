#include <bits/stdc++.h>
using namespace std;
#define int long long

/**
 * 给定一个有权无向树，动态指定一些节点为重要节点，求使得所有重要节点联通的最小边权和
 * 
 * 使得所有重要节点联通的最小树即为这些节点的虚树；对于虚树的周长有如下结论：
 * 将所有关键节点（不包含LCA）按照 dfn 序写下来，相邻两项求树上距离（首尾也算相邻）即为周长的两倍
 * 
 * 因此可以建立有序集保存 {dfn[u],u}，每次新增或删除关键点只需要修改距离增量即可
 */

struct LCA {
    using pii = pair<int, int>;
    int n;
    vector<int> depth, first, lg, euler;
    vector<int> dis;
    vector<int> dfn;
    vector<vector<int>> st;
    int timer;
    LCA(vector<vector<pii>>& g, int root = 0) {
        n = g.size();
        timer = 0;
        depth.resize(n);
        dis.resize(n);
        dfn.resize(n);
        first.assign(n, -1);
        auto& dep = depth;
        auto& fir = first;
        auto& eu = euler;
        auto& ds = dis;
        auto& dn = dfn;
        auto& t = timer;
        auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
            fir[u] = eu.size();
            eu.emplace_back(u);
            dn[u] = t++;
            for (auto& [v, w] : g[u]) {
                if (v == fa) continue;
                dep[v] = dep[u] + 1;
                ds[v] = ds[u] + w;
                dfs(v, u);
                eu.emplace_back(u);
            }
        };
        dfs(root, -1);
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
        return dis[x] + dis[y] - 2 * dis[z];
    }
    int getDepth(int x) {
        return depth[x];
    }
    int getRootDist(int x) {
        return dis[x];
    }
    int len(int x, int y) {
        int z = getLCA(x, y);
        return depth[x] + depth[y] - 2 * depth[z];
    }
    bool onPath(int x, int u, int v) {
        return len(u, x) + len(x, v) == len(u, v);
    }
};

signed main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v, w; cin >> u >> v >> w; --u, --v;
        g[u].emplace_back(v, w), g[v].emplace_back(u, w);
    }
    LCA tr(g);
    int m; cin >> m;
    set<pair<int, int>> s;
    int cur = 0;
    auto add = [&](int u) -> void {
        pair<int, int> p = {tr.dfn[u], u};
        if (s.empty()) {
            s.insert(p);
            return;
        }
        auto it = s.lower_bound(p);
        auto prv = it == s.begin() ? prev(s.end()) : prev(it);
        auto nxt = it == s.end() ? s.begin() : it;
        int l = prv->second, r = nxt->second;
        cur -= tr.dist(l, r);
        cur += tr.dist(l, u) + tr.dist(u, r);
        s.insert(p);
    };
    auto del = [&](int u) -> void {
        pair<int, int> p = {tr.dfn[u], u};
        if (s.size() == 1) {
            s.erase(p);
            return;
        }
        auto it = s.find(p);
        auto prv = it == s.begin() ? prev(s.end()) : prev(it);
        auto nxt = it == prev(s.end()) ? s.begin() : next(it);
        int l = prv->second, r = nxt->second;
        cur += tr.dist(l, r);
        cur -= tr.dist(l, u) + tr.dist(u, r);
        s.erase(p);
    };
    for (int i = 0; i < m; ++i) {
        char op; cin >> op;
        if (op == '?') {
            cout << (cur >> 1) << "\n";
            continue;
        }
        int u; cin >> u; --u;
        if (op == '+') add(u);
        else if (op == '-') del(u);
    }
    return 0;
}