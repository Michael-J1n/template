#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;

/**
 * 一般在网格图中（但一般图也可以使用），操作只有两种代价：0 或 1，可以使用 01 BFS
 * 算法：使用双端队列，代价 0 的更新放队首，代价 1 的更新放对尾
 */

// 网格图
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
int zeroOneGridBFS(vector<vector<int>>& grid) {
    int n = grid.size(), m = grid[0].size();
    vector dis(n, vector<int>(m, INF)); dis[0][0] = 0;
    deque<pair<int, int>> dq; dq.push_front({0, 0});
    while (!dq.empty()) {
        auto [x, y] = dq.front(); dq.pop_front();
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            int w = grid[nx][ny];
            if (dis[nx][ny] > dis[x][y] + w) {
                dis[nx][ny] = dis[x][y] + w;
                if (!w) dq.push_front({nx, ny});
                else dq.push_back({nx, ny});
            }
        }
    }
    return dis[n - 1][m - 1];
}

// 一般图
int zeroOneBFS(int n, vector<vector<pair<int, int>>>& g, int s, int t) {
    vector<int> dis(n + 1, INF); dis[s] = 0;
    deque<int> q; q.push_front(s);
    while (!q.empty()) {
        int u = q.front(); q.pop_front();
        for (auto& [v, w] : g[u]) {
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (!w) q.push_front(v);
                else q.push_back(v);
            }
        }
    }
    return dis[t];
}