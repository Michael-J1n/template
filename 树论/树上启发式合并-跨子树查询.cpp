#include <bits/stdc++.h>
using namespace std;

/**
 * 树上启发式合并不仅能解决子树统计信息，还可以在**合并的同时进行跨子树的查询**
 * 前提是路径 (x,y) 的信息可以通过 info(x),info(y),info(lca(x,y)) 快速得到；
 * 关键在于，任何一条查询路径 (x,y) 可以唯一写成 x-u-y 的路径，u=lca(x,y) 是当前合并的根节点
 * 
 * 注意：
 * 1. 如果路径 x -- rt 的信息不能通过对于 x -- 0 和 rt -- 0 的信息差分得到，可以考虑先进行倍增预处理
 * 2. 路径统计同时可以考虑树形 DP，由于每条路径只会在 LCA 处被统计一次，因此若树形 DP 可行应当优先考虑
 * 
 * 维护全局统计桶，每次在合并之前先查询跨子树贡献；注意统计桶一定记录的是到根节点的信息！！查询的时候对当前根做差
 * 模板例题：计算 sum_{u-v} w(u-v) * cnt(u-v)，其中 u-v 是边 uv，cnt[e] 表示边 e 在所有长为 k 的简单路径中的出现次数
 * 关键等式: dep[x] + dep[u] - 2*dep[lca] = k ==> targdep = k + 2dep[lca] - dep[x]
 * 
 * 复杂度 O(n * logn)
 */
long long DSUonTree(vector<vector<pair<int, int>>>& g, int k, int root = 0) {
    int n = g.size();
    vector<int> sz(n), son(n, -1), dfn(n), out(n);
    vector<int> seq; seq.reserve(n);
    vector<int> dep(n);
    vector<long long> dis(n);
    int timer = 0;
    auto dfs1 = [&](this auto&& dfs1, int u, int fa) -> void {
        sz[u] = 1;
        dfn[u] = timer++;
        seq.emplace_back(u);
        for (auto& [v, w] : g[u]) {
            if (v == fa) continue;
            dep[v] = dep[u] + 1;
            dis[v] = dis[u] + w;
            dfs1(v, u);
            sz[u] += sz[v];
            if (son[u] == -1 || sz[v] > sz[son[u]]) son[u] = v;
        }
        out[u] = timer;
    }; dfs1(root, -1);
    /** ------------------------------------------------------------------------- */
    long long res = 0;
    vector<int> cnt(n);        // cnt[d] = 当前全局桶中深度为 d 的节点数量
    vector<long long> sum(n);  // sum[d] = 当前全局桶中深度为 d 的所有节点的 dis 之和
    auto add = [&](int x) -> void {            // 将节点 x 的信息加入桶
    	++cnt[dep[x]];
    	sum[dep[x]] += dis[x];
    };
    auto del = [&](int x) -> void {            // 将节点 x 的信息移出桶
    	--cnt[dep[x]];
    	sum[dep[x]] -= dis[x];
    };
    auto query = [&](int x, int rt) -> void {  // 全局桶更新 rt 子树中节点 x 的答案/贡献
        // 假设 targ 是桶中可以与 x 配对的合法点：x -- rt -- targ
    	// dep[x] + dep[targ] - 2dep[rt] = k => dep[targ] = k + 2dep[rt] - dep[x]
    	int targd = k + 2 * dep[rt] - dep[x];
    	if (targd < 0 || targd >= n) return;
    	res += (dis[x] - 2 * dis[rt]) * cnt[targd] + sum[targd];
    };
    /** ------------------------------------------------------------------------- */
    auto dfs2 = [&](this auto&& dfs2, int u, int fa, bool keep) -> void {
        for (auto& [v, w] : g[u]) {
            if (v == fa || v == son[u]) continue;
            dfs2(v, u, false);                                    // calculate answer for light sons first, but doesnt store its contribution in bucket
        } 
        if (son[u] != -1) dfs2(son[u], u, true);                  // calculate answer for heavy son then, and keep info in bucket
        query(u, u);                                              // query u with heavy subtree
        add(u);
        for (auto& [v, w] : g[u]) {
            if (v == fa || v == son[u]) continue;
            for (int i = dfn[v]; i < out[v]; ++i) {             
                int x = seq[i];
                query(x, u);                                      // brutely query for light subtree
            }
            for (int i = dfn[v]; i < out[v]; ++i) {
                int x = seq[i];
                add(x);                                           // brutely add for light subtree
            }
        }
        if (!keep) {
            for (int i = dfn[u]; i < out[u]; ++i) {
                int x = seq[i];
                del(x);
            }
        }
    }; dfs2(root, -1, true);
    return res;
}