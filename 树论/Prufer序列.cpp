#include <bits/stdc++.h>
using namespace std;

/**
 * Cayley 公式：K_n 的生成树有 n^{n-2} 种；
 * deg[u] = 1 + (u 在 Prufer 序列中出现次数)。
 * 
 * 扩展结论：
 * 1. 给定 n 个顶点，要求顶点 i 的度数恰好为 d[i]，求方案数（其中 d[i]>=1，sum(d[i])=2(n-1)）
 * 满足条件的标号无根树个数为：(n-2)! / Π{(d[i]-1)!}；思路：构造长度为 n-2 的序列，其中数字 i 恰好出现 d[i]-1 次。
 * 
 * 2. 给定 n 个顶点，以及 n 个度数构成的多重集合，求方案数（其中 d[i]>=1，sum(d[i])=2(n-1)）
 * 记多重集合中 k 出现了 cnt[k] 次；首先考虑将 n 个度数分配给 n 个顶点有 n! / Π{cnt[k]!} 种方案； 
 * 每一种分配都对应着 1 的情况，因此总共有 n!*(n-2)! / Π{(d[i]-1)!}Π{cnt[k]!} 种方案。
 * 
 * 3. 要求顶点 u 的度数恰好为 d 的方案数：
 * C(n-2, d-1) * (n-1)^(n-d-1)；思路：序列中恰好 d-1 个位置为 u，其余位置可以填写另外 n-1 个编号
 * 
 * 4. 指定 k 个点是叶子：
 * (n-k)^(n-2)；思路：叶子一定不出现在序列中，因此每个位置只有 n-k 种放法
 * 
 * 5. 给定 K_n 中一个包含 k 个顶点的固定连通子树，包含这棵子树的生成树数量为
 */

/**
 * Prüfer 序列编码：
 * 无根树转 Prüfer 序列；返回长度为 n - 2 的 Prüfer 序列。
 * 
 * 复杂度：O(n)
 */
vector<int> prufer_encode(const vector<vector<int>>& g) {
    int n = g.size();
    vector<int> parent(n, -1), order = {n - 1}, deg(n);
    for (int i = 0; i < n; ++i) {
        int u = order[i];
        deg[u] = g[u].size();
        for (int v : g[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            order.push_back(v);
        }
    }
    int ptr = 0;
    while (deg[ptr] != 1) ++ptr;
    int leaf = ptr;
    vector<int> code; code.reserve(n - 2);
    for (int i = 0; i < n - 2; ++i) {
        int v = parent[leaf];
        code.push_back(v);
        deg[leaf] = 0;
        if (--deg[v] == 1 && v < ptr) leaf = v;
        else {
            ++ptr;
            while (deg[ptr] != 1) ++ptr;
            leaf = ptr;
        }
    }
    return code;
}
// 接受父亲数组的重载；父亲数组大小应为 n-1
vector<int> prufer_encode(const vector<int>& father) {
    int n = father.size() + 1;
    vector<int> deg(n);
    for (int v : father) ++deg[v];
    int ptr = 0;
    while (deg[ptr]) ++ptr;
    int leaf = ptr;
    vector<int> code; code.reserve(n - 2);
    for (int i = 0; i < n - 2; ++i) {
        int v = father[leaf];
        code.push_back(v);
        if (--deg[v] == 0 && v < ptr) leaf = v;
        else {
            ++ptr;
            while (deg[ptr]) ++ptr;
            leaf = ptr;
        }
    }
    return code;
}

/**
 * Prüfer 序列解码：
 * Prüfer 序列还原无根树；返回树的 n - 1 条无向边。
 * 
 * 复杂度：O(n)
 */
vector<pair<int, int>> prufer_decode(const vector<int>& code) {
    int n = code.size() + 2;
    vector<int> deg(n, 1);
    for (int v : code) ++deg[v];
    int ptr = 0;
    while (deg[ptr] != 1) ++ptr;
    int leaf = ptr;
    vector<pair<int, int>> edges; edges.reserve(n - 1);
    for (int v : code) {
        edges.emplace_back(leaf, v);
        deg[leaf] = 0;
        if (--deg[v] == 1 && v < ptr) leaf = v;
        else {
            ++ptr;
            while (deg[ptr] != 1) ++ptr;
            leaf = ptr;
        }
    }
    edges.emplace_back(leaf, n - 1);
    return edges;
}