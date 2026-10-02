#include <bits/stdc++.h>
using namespace std;

/**
 * 树上启发式合并不仅能解决子树统计信息，还可以在**合并的同时进行跨子树的查询**
 * 前提是路径 (x,y) 的信息可以通过 info(x),info(y),info(lca(x,y)) 快速得到
 * 关键在于，任何一条查询路径 (x,y) 可以唯一写成 x-u-y 的路径，u=lca(x,y) 是当前合并的根节点
 * 
 * 维护全局统计桶，每次在合并之前先查询跨子树贡献；注意统计桶一定记录的是到根节点的信息！！查询的时候对当前根做差
 * 模板例题：给定有边权树和 Q 个查询，查询是否存在一个路径的长度为 queries[i]
 * 关键等式: d[x] + d[y] - 2d[u] = q, 所以查询 d[y] = q - d[x] + 2d[u]
 * 
 * 复杂度 O(Q + n * logn)
 */
vector<bool> DSUonTree(vector<vector<pair<int, int>>>& g, vector<int>& qs, int root = 0) {
    // preprocess dfs to get heavy son and dfn order
    int n = g.size();
    vector<int> sz(n), son(n, -1), dfn(n), out(n);
    vector<long long> dis(n);
    vector<int> seq; seq.reserve(n);
    int timer = 0;
    auto dfs1 = [&](this auto&& dfs1, int u, int fa) -> void {
        sz[u] = 1;
        dfn[u] = timer++;
        seq.emplace_back(u);
        for (auto& [v, w] : g[u]) {
            if (v == fa) continue;
            dis[v] = dis[u] + w;
            dfs1(v, u);
            sz[u] += sz[v];
            if (son[u] == -1 || sz[v] > sz[son[u]]) son[u] = v;
        }
        out[u] = timer;
    }; dfs1(root, -1);

    /** -------------------------------------------------------------------- */
    int Q = qs.size();
    vector<bool> res(Q);                      // res[i] = answer for the i-th query
    unordered_map<long long, int> cnt;        // global bucket; cnt[x] = #{path which has distance x from root}
    cnt.reserve(2 * n);
    // 将节点 x 的信息在桶中计入
    auto add = [&](int x) -> void {
        ++cnt[dis[x]];
    };
    // 将节点 x 的信息在桶中删除
    auto del = [&](int x) -> void {
        --cnt[dis[x]];
    };
    // 当前根为 rt，对子树点 x 查询
    auto query = [&](int x, int rt) -> void {
        for (int i = 0; i < Q; ++i) {
            if (res[i]) continue;
            long long targ = dis[rt] * 2 - dis[x] + qs[i];
            if (cnt.count(targ)) res[i] = 1;
        }
    };
    /** -------------------------------------------------------------------- */

    // dsu on tree to merge and query answer
    auto dfs2 = [&](this auto&& dfs2, int u, int fa, bool keep) -> void {
        for (auto& [v, w] : g[u]) {
            if (v == fa || v == son[u]) continue;
            dfs2(v, u, false);                                    // calculate answer for light sons first, but doesnt store its contribution in bucket
        }
        if (son[u] != -1) dfs2(son[u], u, true);                  // calculate answer for heavy son then, and keep info in bucket
        query(u, u);                                              // query u with heavy subtree
        add(u);                                                   // add u itself to bucket
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


/**
 * 例2：计算 sum_{u-v} w(u-v) * cnt(u-v)，其中 u-v 是边 uv，cnt[e] 表示边 e 在所有长为 k 的简单路径中的出现次数
 * 还是利用启发式合并跨子树查询的关键性质：【每一个树上路径都可以不重不漏的在路径两端点的 LCA 上统计一次】
 * 
 * 因此 DSUonTree 枚举每个 LCA，然后来想如何统计 边权*计数 的和：在每个 LCA 处，统计所有经过这个点的长为 k 的路径，每个路径上每个边权都被累加一次
 * 令 dep[x] 为根到 x 的路径边数，dis[x] 为根到 x 的路径边权和；
 * 令桶维护 cnt[d], sum[d] 分别代表当前深度为 d 的节点数量，深度为 d 的节点的 dis 之和
 * 对于增删点 x，那么 cnt[dep[x]] 和 sum[dep[x]] 分别进行增删即可（量分别是 1 和 dis）
 * 关键在于询问：
 * 对于点 x，和桶进行配对：由于路径长度需要为 k，有 dep[x] + dep[u] - 2*dep[lca] = k，因此 targdep = k + 2dep[lca] - dep[x]（这也是重要转化模板）
 * 那么新边 u-x 可以和桶中深度为 targdep 的点配对；u-x 的边权和会出现 cnt[targdep] 次，而所有深度为 targdep 的点到 u 的距离又都会增加一次，因此还要加上 sum[targdep]
 */
long long DSUonTree(vector<vector<pair<int, int>>>& g, int k, int root = 0) {
    int n = g.size();
    vector<int> sz(n), son(n, -1), dfn(n), out(n), dep(n);
    vector<long long> dis(n);
    vector<int> seq; seq.reserve(n);
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

    /** -------------------------------------------------------------------- */
    long long res = 0;
    vector<long long> cnt(n), sum(n);
    auto add = [&](int x) -> void {
    	++cnt[dep[x]];
    	sum[dep[x]] += dis[x];
    };
    auto del = [&](int x) -> void {
    	--cnt[dep[x]];
    	sum[dep[x]] -= dis[x];
    };
    auto query = [&](int x, int rt) -> void {
    	// dep[x] + dep[targ] - 2dep[rt] = k => dep[targ] = k + 2dep[rt] - dep[x]
    	int targd = k + 2 * dep[rt] - dep[x];
    	if (targd < 0 || targd >= n) return;
    	// dis(x,rt) will be calc cnt[targ] times; all sum[targd] will be added one more time
    	res += (dis[x] - 2 * dis[rt]) * cnt[targd] + sum[targd];
    };
    /** -------------------------------------------------------------------- */

    // dsu on tree to merge and query answer
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
                query(x, u);                                   // brutely query for light subtree
            }
            for (int i = dfn[v]; i < out[v]; ++i) {
                int x = seq[i];
                add(x);                                        // brutely add for light subtree
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