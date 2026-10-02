#include <bits/stdc++.h>
using namespace std;

/**
 * Tomita 算法求图 g 中的最大团，复杂度 O(3^{n/3})，但跑不满。此算法剪枝多，上界紧
 */


vector<int> maximumClique(vector<vector<int>>& g) {
    #define eb emplace_back
    int n = g.size();
    vector<int> cur, ans, deg(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) deg[i] += g[i][j];
    }
    auto colorSort = [&](vector<int>& cand, vector<int>& ord, vector<int>& col) {
        ord.clear(), col.clear();
        vector<int> rem = cand;
        int c = 0;
        while (!rem.empty()) {
            ++c;
            vector<int> nxt, cls;
            for (auto& v : rem) {
                bool ok = 1;
                for (auto& u : cls) {
                    if (g[v][u]) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) ord.eb(v), col.eb(c), cls.eb(v);
                else nxt.eb(v);
            }
            rem.swap(nxt);
        }
    };
    auto dfs = [&](auto&& dfs, vector<int> cand) -> void {
        if (cand.empty()) {
            if (cur.size() > ans.size()) ans = cur;
            return;
        }
        vector<int> ord, col;
        colorSort(cand, ord, col);
        for (int i = ord.size() - 1; i >= 0; --i) {
            if (cur.size() + col[i] <= ans.size()) return;
            int v = ord[i];
            cur.eb(v);
            vector<int> nxt;
            for (int j = 0; j < i; ++j) {
                if (g[v][ord[j]]) nxt.eb(ord[j]);
            }
            if (nxt.empty()) {
                if (cur.size() > ans.size()) ans = cur;
            } else {
                dfs(dfs, nxt);
            }
            cur.pop_back();
        }
    };
    vector<int> cand(n);
    iota(cand.begin(), cand.end(), 0);
    sort(cand.begin(), cand.end(), [&](int x, int y) {
        return deg[x] > deg[y];
    });
    dfs(dfs, cand);
    return ans;
}