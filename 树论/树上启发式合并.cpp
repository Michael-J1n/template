#include <bits/stdc++.h>
using namespace std;

/**
 * DSU on tree / 树上启发式合并模板
 *
 * 常见题目：对于 u 子树内的静态/离线查询，需要维护一个桶（e.g., 集合或哈希表）供重儿子不断使用并合并
 * 这时可以使用 DSU on tree 减轻合并桶的时空复杂度，前提是增删代价较轻，且每个点储存的值不随子树根变化而变化
 *
 * 复杂度：树上每个点作为某个轻子树的成员最多 logn 次，因此瓶颈是最多 nlogn 次暴力枚举
 * 假如对于桶的增删是 O(C) 的，总复杂度即为：O(C * n * logn)
 * 
 * 关键不变量（！）：
 *      - dfs2(u, keep)       进入时，全局桶是空的
 *      - dfs2(u, false)      返回时，全局桶也是空的
 *      - dfs2(son[u], true)  返回时，全局桶保留了重儿子 subtree(son[u]) 的信息
 *      - dfs2(u, true)       返回时，全局桶保留了 subtree(u) 的信息
 */
const int MXV = 2e5 + 5;  // 颜色数
vector<int> DSUOnTree(vector<vector<int>>& g, vector<int>& a, int root = 0) {
    int n = g.size();
    vector<int> sz(n), son(n, -1), dfn(n), out(n);
    vector<int> seq; seq.reserve(n);
    int timer = 0;
    auto dfs1 = [&](this auto&& dfs1, int u, int fa) -> void {
        sz[u] = 1;
        dfn[u] = timer++;
        seq.emplace_back(u);
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs1(v, u);
            sz[u] += sz[v];
            if (son[u] == -1 || sz[v] > sz[son[u]]) son[u] = v;
        }
        out[u] = timer;
    }; dfs1(root, -1);
    /** ------------------------------------------------------------------------- */
    vector<int> res(n);       // res[u] = 子树 u 对应的答案
    vector<int> cnt(MXV);     // 全局统计桶
    int cur = 0;              // 当前答案
    auto add = [&](int x) -> void {     // 将节点 x 的信息加入桶
        if (++cnt[a[x]] == 1) ++cur;
    };
    auto del = [&](int x) -> void {     // 将节点 x 的信息移出桶
        if (--cnt[a[x]] == 0) --cur;
    };
    auto query = [&](int rt) -> void {  // 全局桶更新 rt 子树的答案
        res[rt] = cur;
    };
    /** ------------------------------------------------------------------------- */
    auto dfs2 = [&](this auto&& dfs2, int u, int fa, bool keep) -> void {
        for (auto& v : g[u]) {
            if (v == fa || v == son[u]) continue;
            dfs2(v, u, false);                                    // calculate answer for light sons first, but doesnt store its contribution in bucket
        }
        if (son[u] != -1) dfs2(son[u], u, true);                  // calculate answer for heavy son then, and keep info in bucket
        add(u);                                                   // add u itself to bucket
        for (auto& v : g[u]) {
            if (v == fa || v == son[u]) continue;
            for (int i = dfn[v]; i < out[v]; ++i) {
                int x = seq[i];
                add(x);                                           // brutely add info from light sons
            }
        }
        query(u);                                                 // now the bucket has the info for whole subtree(u)
        if (!keep) {
            for (int i = dfn[u]; i < out[u]; ++i) {
                int x = seq[i];
                del(x);
            }
        }
    }; dfs2(root, -1, true);
    return res;
}