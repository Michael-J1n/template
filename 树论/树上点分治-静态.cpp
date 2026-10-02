#include <bits/stdc++.h>
using namespace std;

/**
 * 静态点分树
 * 核心在于把树上任意点对路径，归到它们在点分治递归中“第一次被某个重心分开”的那一层统计，并用 all - sub 容斥保证不重不漏
 * 模板以多次询问整棵树中距离 u 不超过 k 的点数为例；模板中注释所谓统计信息即表示点到 c 的所有距离的集合
 * 
 * 复杂度：
 *      建树 DFS 总量 O(nlogn)，排序总量 O(nlog^2n)
 *      单次询问 O(log^2n)，空间 O(nlogn)
 */
vector<int> centroidDecomposition(vector<vector<pair<int, int>>>& g, vector<pair<int, int>>& qs) {
    int n = g.size();

    vector<int> sz(n);                              // sz[u]      = 当前未被删除的点分连通块里，以 u 为根的 dfs 子树大小
    vector<int> del(n);                             // del[u]     = u 是否已经被选为点分治重心并从后续连通块中删除
    vector<vector<int>> all(n);                     // all[c]     = 以 c 为重心这一层，整个点分连通块点到 c 的统计信息
    vector<vector<vector<int>>> sub(n);             // sub[c][id] = 以 c 为重心这一层，删除 c 后第 id 个点分子快内点到 c 的统计信息
    vector<vector<tuple<int, int, int>>> anc(n);    // anc[c]     = u 在点分树上的所有祖先重心列表 (c, f(u,c), id)；f 代表状态信息，id 代表 u 属于 c 的哪个点分子块，若 u==c 则 id=-1
    
    // 求联通子块大小
    auto getSize = [&](this auto&& getSize, int u, int fa) -> void {
        sz[u] = 1;
        for (auto& [v, w] : g[u]) {
            if (v == fa || del[v]) continue;
            getSize(v, u);
            sz[u] += sz[v];
        }
    };
    // 求联通子块重心
    auto getCentroid = [&](this auto&& getCentroid, int u, int fa, int tot) -> int {
        for (auto& [v, w] : g[u]) {
            if (v == fa || del[v]) continue;
            if (sz[v] * 2 > tot) return getCentroid(v, u, tot);
        }
        return u;
    };
    // 在联通子块 c 中统计所有点到的信息贡献并给每个点打祖先标记；d 代表统计信息（这里是 u 到 c 的距离），c 代表重心编号，id 代表连通块编号
    auto collect = [&](this auto&& collect, int u, int fa, int d, int c, int id) -> void {
        all[c].emplace_back(d);
        sub[c][id].emplace_back(d);
        anc[u].emplace_back(c, d, id);
        for (auto& [v, w] : g[u]) {
            if (v == fa || del[v]) continue;
            collect(v, u, d + w, c, id);
        }
    };
    // 递归对整棵树进行重心分解
    auto solve = [&](this auto&& solve, int u) -> void {
        getSize(u, -1);

        // 求出重心并处理重心信息
        int c = getCentroid(u, -1, sz[u]);
        del[c] = 1;
        all[c].emplace_back(0);
        anc[c].emplace_back(c, 0, -1);
        
        // 收集连通子块中点的信息；这里收集完后由于要维护有序集合还需要排序
        for (auto& [v, w] : g[c]) {
            if (del[v]) continue;
            int id = sub[c].size();
            sub[c].push_back({});
            collect(v, c, w, c, id);
        }
        sort(all[c].begin(), all[c].end());
        for (auto& e : sub[c]) sort(e.begin(), e.end());
        
        // 递归分解每个连通子块
        for (auto& [v, w] : g[c]) {
            if (del[v]) continue;
            solve(v);
        }
    }; solve(0);

    // 处理询问的逻辑
    int q = qs.size();
    vector<int> res(q);
    for (int i = 0; i < q; ++i) {
        auto& [u, k] = qs[i];  // 求 u 距离不超过 k 的点数
        
        for (auto& [c, d, id] : anc[u]) {  // 在每个重心连通块中统计 u-c-x 的贡献（路径上唯一最小重心）
            int lim = k - d;
            if (lim < 0) continue;
            res[i] += upper_bound(all[c].begin(), all[c].end(), lim) - all[c].begin();
            if (id != -1) {
                res[i] -= upper_bound(sub[c][id].begin(), sub[c][id].end(), lim) - sub[c][id].begin();
            }
        }
    }
    return res;
}