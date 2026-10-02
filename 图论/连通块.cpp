#include <bits/stdc++.h>
using namespace std;

/**
 * 用法：
 * 
    CC cc(g);
    for (int i = 0; i < cc.cnt; ++i) // 每个连通块

    cc.cnt;              // 连通块个数
    cc.bel[u];           // u 属于哪个连通块
    cc.siz[c];           // 第 c 个连通块点数
    cc.ed[c];            // 第 c 个连通块边数
    cc.nodes[c];         // 第 c 个连通块所有点(默认不加)

    cc.same(u, v);       // u, v 是否同块
    cc.connected();      // 整图是否连通
    cc.size(u);          // u 所在块大小
    cc.edges(u);         // u 所在块边数
    cc.isTree(c);        // 第 c 块是不是树
    cc.hasCycle(c);      // 第 c 块有没有环
*/
struct CC {
    #define eb emplace_back
    using ll = long long;
    int n, cnt = 0;
    const vector<vector<int>>& g;
    vector<int> bel, siz;
    vector<ll> ed;
    // vector<vector<int>> nodes;
    CC(const vector<vector<int>>& _g): n(_g.size()), g(_g), bel(n, -1) {
        for (int i = 0; i < n; ++i) {
            if (bel[i] != -1) continue;
            siz.eb(0), ed.eb(0);
            // nodes.eb(vector<int>());
            dfs(i);
            ed[cnt] /= 2;
            ++cnt;
        }
    }
    void dfs(int u) {
        bel[u] = cnt;
        ++siz[cnt];
        ed[cnt] += g[u].size();
        // nodes[cnt].eb(u);
        for (auto& v : g[u]) {
            if (bel[v] == -1) dfs(v);
        }
    }
    bool same(int u, int v) {
        return bel[u] == bel[v];
    }
    bool connected() {
        return cnt == 1;
    }
    int size(int u) {
        return siz[bel[u]];
    }
    ll edges(int u) {
        return ed[bel[u]];
    }
    bool isTree(int c) {
        return ed[c] == siz[c] - 1;
    }
    bool hasCycle(int c) {
        return ed[c] >= siz[c];
    }
};