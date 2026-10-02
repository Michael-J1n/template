#include <bits/stdc++.h>
using namespace std;

/**
 * 三色 DFS 判环；判断有向图中是否存在环
 * color[u]=0：未访问；color[u]=1：正在当前 DFS 路径中；color[u]=2：已经完全处理；DFS 遇到 color[v]=1，说明存在一条返祖边，当前路径形成环
 *
 * 复杂度：O(n+m)
 */
bool hasCycle(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> color(n);
    auto dfs = [&](this auto&& dfs, int u) -> bool {
        color[u] = 1;
        for (int v : g[u]) {
            if (color[v] == 1 || color[v] == 0 && dfs(v)) return true;
        }
        color[u] = 2;
        return false;
    };
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0 && dfs(i)) return true;
    }
    return false;
}