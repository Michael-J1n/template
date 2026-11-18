#include <bits/stdc++.h>
using namespace std;

void SCC(vector<vector<int>>& g, vector<int>& comp) {
    int n = g.size(), timer = 0, cnt = 0;
    vector<int> dfn(n, -1), low(n), in(n), stk;
    comp.assign(n, -1);
    auto dfs = [&](this auto&& dfs, int u) -> void {
        dfn[u] = low[u] = timer++;
        stk.emplace_back(u), in[u] = 1;
        for (auto& v : g[u]) {
            if (dfn[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            while (true) {
                int x = stk.back(); stk.pop_back();
                in[x] = 0, comp[x] = cnt;
                if (x == u) break;
            }
            ++cnt;
        }
    };
    for (int i = 0; i < n; ++i) {
        if (dfn[i] == -1) dfs(i);
    }
}

/**
 * 2-SAT 求一组可行解
 *
 * 每个限制 {i, p, j, q} 表示：A_i = p  ==>  A_j = q；（注意不用手动添加逆否命题）
 * 建边：A_i = p      -->  A_j = q
 *      A_j = q ^ 1  -->  A_i = p ^ 1
 *
 * 返回：
 * 有解返回 true，并将 res[i] 设为 A_i 的取值 0/1
 * 无解返回 false
 *
 * 复杂度：O(n + m)
 */
bool twoSAT(int n, vector<array<int, 4>>& lim, vector<int>& res) {
    vector<vector<int>> g(2 * n);
    auto id = [&](int i, int p) {  // id for A[i] takse value p
        return i * 2 + p;
    };
    for (auto& [i, p, j, q] : lim) {
        g[id(i, p)].emplace_back(id(j, q));
        g[id(j, q ^ 1)].emplace_back(id(i, p ^ 1));
    }
    vector<int> comp;
    SCC(g, comp);
    res.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        if (comp[id(i, 0)] == comp[id(i, 1)]) return false;  // A[i] need to take both 0 and 1, contradict
        res[i] = comp[id(i, 1)] < comp[id(i, 0)];            // take the value with deeper topo layer
    }
    return true;
}

/**
 * 如何翻译布尔推导式（以下省去蕴含的逆否命题，在模板中会自动建边）：
 * 
 * 1. x == p || y == q：
 *      x == p ^ 1 ==> y == q
 * 2. x == p && y == q:
 *      x == p ^ 1 ==> x == p； y == q ^ 1 ==> y == q
 * 3. x and y == 0:
 *      x == 1 ==> y == 0
 * 4. x and y == 1:
 *      x == 0 ==> x == 1; y == 0 ==> y == 1
 * 5. x or y == 1:
 *      x == 0 ==> y == 1
 * 6. x or y == 0:
 *      x == 1 ==> x == 0; y == 1 ==> y == 0
 * 7. x xor y == 0:
 *      x == 0 ==> y == 0; x == 1 ==> y == 1
 * 8. x xor y == 1:
 *      x == 0 ==> y == 1; x == 1 ==> y == 0
 */



/**
 * 2-SAT 增强版
 *
 * lim 中每个 {i, p, j, q} 表示 x[i] = p ==> x[j] = q
 * res[i] 表示变量 i 的强制情况：-1：0 和 1 都可行；0：必须取 0；1：必须取 1
 *
 * 复杂度：O(n * (n + m))
 */
bool twoSAT(int n, vector<array<int, 4>>& lim, vector<int>& res) {
    vector<vector<int>> g(2 * n);
    auto id = [&](int i, int p) {
        return i * 2 + p;
    };
    for (auto& [i, p, j, q] : lim) {
        g[id(i, p)].emplace_back(id(j, q));
        g[id(j, q ^ 1)].emplace_back(id(i, p ^ 1));
    }
    vector<int> comp;
    SCC(g, comp);
    res.assign(n, -1);
    for (int i = 0; i < n; ++i) {
        if (comp[id(i, 0)] == comp[id(i, 1)]) return false;
    }
    vector<int> vis(2 * n);
    int tag = 0;
    auto check = [&](int s, int t) {
        ++tag;
        auto dfs = [&](this auto&& dfs, int u) -> bool {
            if (u == t) return true;
            vis[u] = tag;
            for (auto& v : g[u]) {
                if (vis[v] != tag && dfs(v)) return true;
            }
            return false;
        };
        return dfs(s);
    };
    for (int i = 0; i < n; ++i) {
        int x = id(i, 0), y = id(i, 1);
        if (check(x, y)) res[i] = 1;  // if taking 0 will force to take 1, must choose 1
        else if (check(y, x)) res[i] = 0;
        else res[i] = -1;
    }
    return true;
}