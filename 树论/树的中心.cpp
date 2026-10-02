#include <bits/stdc++.h>
using namespace std;

/**
 * 求一颗树的中心
 * 中心是指到其他节点距离最大值最小的一个点，即一个根使得树的高度最小
 * 
 * 结论：
 * 一棵奇直径的树有一个中心，偶直径有两个
 * 形象的理解即为树最中心的点，可以使用剥洋葱法将外层的点顺序剥掉后剩下的即为中心
 */
vector<int> treeCenter(vector<vector<int>>& g) {
    int n = g.size(), left = n;
    if (n == 1) return {0};
    vector<int> deg(n);
    for (int i = 0; i < n; ++i) deg[i] = g[i].size();
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (deg[i] == 1) q.emplace(i);
    }
    while (left > 2) {
        int cursz = q.size();
        left -= cursz;
        while (cursz--) {
            int u = q.front(); q.pop();
            for (auto& v : g[u]) {
                --deg[v];
                if (deg[v] == 1) q.emplace(v);
            }
        }
    }
    vector<int> res;
    while (!q.empty()) res.emplace_back(q.front()), q.pop();
    return res;
}


/**
 * 求正权树的点中心和半径
 * 
 * 返回 {center, radius}
 * center：使其到树中最远点距离最小的顶点
 * radius：center 到树中最远点的距离
 * 
 * 复杂度 O(n)
 */
pair<int, long long> treeCenterW(vector<vector<pair<int, int>>>& g) {
    int n = g.size();
    vector<long long> dis(n);
    auto getDis = [&](int s) -> int {
        fill(dis.begin(), dis.end(), -1); dis[s] = 0;
        auto dfs = [&](this auto&& dfs, int u, int p) -> void {
            for (auto& [v, w] : g[u]) {
                if (v == p) continue;
                dis[v] = dis[u] + w;
                dfs(v, u);
            }
        };
        dfs(s, -1);
        int x = s;
        for (int i = 0; i < n; ++i) {
            if (dis[i] > dis[x]) x = i;
        }
        return x;
    };
    int a = getDis(0);
    int b = getDis(a);
    vector<long long> da = dis;
    getDis(b);
    vector<long long> db = dis;
    int center = 0;
    for (int i = 1; i < n; ++i) {
        if (max(da[i], db[i]) < max(da[center], db[center])) center = i;
    }
    return {center, max(da[center], db[center])};
}