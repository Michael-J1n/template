#include <bits/stdc++.h>
using namespace std;

/**
 * 返回图 g 中的最大团 maximum clique，复杂度 O(3^{n/3})，但一般不会跑满，实际递归深度是最大团大小
 */

vector<int> maximumClique(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> f(n), cur, ans;
    auto ok = [&](int x) {
        for (auto& y : cur) {
            if (!g[x][y]) return false;
        }
        return true;
    };
    auto dfs = [&](this auto&& dfs, int u) -> bool {
        for (int v = u + 1; v < n; ++v) {
            if (cur.size() + f[v] <= ans.size()) break;
            if (!ok(v)) continue;
            cur.emplace_back(v);
            if (dfs(v)) return true;
            cur.pop_back();
        }
        if (cur.size() > ans.size()) {
            ans = cur;
            return true;
        }
        return false;
    };
    for (int i = n - 1; ~i; --i) {
        cur.clear(); cur.emplace_back(i);
        dfs(i);
        f[i] = ans.size();
    }
    return ans;
}