#include <bits/stdc++.h>
using namespace std;

int treeDP(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> dp(n);

    auto dfs = [&](this auto&& dfs, int u, int fa) -> void {
        // init dp[u]

        int cnt = 0; char child = 1;
        for (auto& v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            int ndp;
            // ndp <-- dp[u], dp[v], cnt: 只考虑之前子树的状态，当前子树的状态，已经合并状态的子树个数
            
            ++cnt; child = 0;
            dp[u] = ndp;
        }
    };

    dfs(0, -1);
    return dp[0];
}