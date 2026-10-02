#include <bits/stdc++.h>
using namespace std;

/**
 * Tarjan 求无向图点双连通分量 vDCC，并返回圆方树
 * vDCC：连通块内部不存在割点；将每个点双缩为一个方点，并保留割点作为圆点；一个简单环必定完整包含于一个 vDCC
 * low[u] = u 这个 DFS 子树，在不走父边的情况下，能通过返祖边绕回到的最早祖先的 dfn
 *
 * （！！！）返回：
 * 返回点双缩点后的圆方树 tree；记 cnt = dcc.size()
 * comp[u] 会被修改为 u 在圆方树中的节点编号，dcc[i] 会被修改为第 i 个点双包含的原图点；cut[u] 表示 u 是否是割点
 * 那么原图点编号 0~n-1，方点编号 n~n+cnt-1，第 i 个点双 dcc[i] 对应的方点编号为 n+i
 * tree 中只有割点会作为圆点参与连边（即，有效节点只有割点的编号与所有bcc编号）；如果割点 u 属于点双 dcc[i]，则圆方树中连边 u -- (n + i)（因此，这颗树一定是二分的：割点u<->方点n+i）
 *
 * 复杂度：O(n + m)
 */
vector<vector<int>> vDCC(vector<vector<int>>& g, vector<int>& comp, vector<vector<int>>& dcc, vector<int>& cut) {
    int n = g.size(), timer = 0;
    vector<int> dfn(n, -1), low(n), stk;
    stk.reserve(n);
    cut.assign(n, 0);
    dcc.clear();
    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        dfn[u] = low[u] = timer++;
        stk.emplace_back(u);
        int child = 0;
        bool skipped = false;
        for (auto& v : g[u]) {
            if (v == fa && !skipped) {
                skipped = true;
                continue;
            }
            if (dfn[v] == -1) {
                ++child;
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    if (fa != -1) cut[u] = 1;
                    dcc.emplace_back();
                    auto& cur = dcc.back();
                    while (stk.back() != v) {
                        cur.emplace_back(stk.back());
                        stk.pop_back();
                    }
                    cur.emplace_back(v);
                    stk.pop_back();
                    cur.emplace_back(u);
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (fa == -1 && child >= 2) cut[u] = 1;
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] != -1) continue;
        if (g[i].empty()) {
            dfn[i] = low[i] = timer++;
            dcc.push_back({i});
        } else {
            dfs(i, -1);
            stk.pop_back();
        }
    }
    int cnt = dcc.size();
    vector<vector<int>> tree(n + cnt);
    comp.assign(n, -1);
    for (int i = 0; i < cnt; ++i) {
        int id = n + i;
        for (auto& u : dcc[i]) {
            if (cut[u]) {
                tree[u].emplace_back(id);
                tree[id].emplace_back(u);
                comp[u] = u;
            } else {
                comp[u] = id;
            }
        }
    }
    return tree;
}

// 对于原图点 u，求 bcc 编号：if (!cut[u]) return comp[u] - n
// 对于原图点 u，求圆方树节点编号：return comp[u]
// 对于圆方树方节点 x，求 bcc 编号：if (x > n) return x - n
// 对于 bcc 编号 id，求圆方树方节点编号：return id + n