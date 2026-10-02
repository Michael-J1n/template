#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;

/**
 * 在网格图问题中，如果题目涉及到可以使用 k 次特殊操作，例如 边权置零/打折/反转，障碍物消除 等，将每个节点分成 k 份，形成分层图
 * 定义 dis[u][t] 为从起点到 u，用了 t 次特殊操作后的最小距离
 */
// 以消除至多 k 个障碍为例
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
int layerGridBFS(vector<vector<int>>& grid, int k) {
    int n = grid.size(), m = grid[0].size();
    vector dis(n, vector(m, vector<int>(k + 1, -1)));
    dis[0][0][0] = 0;
    queue<tuple<int, int, int>> q;
    q.push({0, 0, 0});
    while (!q.empty()) {
        auto [x, y, t] = q.front(); q.pop();
        if (x == n - 1 && y == m - 1) return dis[x][y][t];
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            int nt = t + grid[nx][ny];
            if (nt > k || dis[nx][ny][nt] != -1) continue;
            dis[nx][ny][nt] = dis[x][y][t] + 1;
            q.push({nx, ny, nt});
        }
    }
    return -1;
}

/**
 * BFS 分层图有个优点：如果维度特别多，可以进行状态-值反演：将状态维度变为表示的值，然后队列更新中用 int sz=q.size();while(sz--) 来取出队列（即层序更新）
 * 这样距离就是层数（维护一个cnt），省去一个状态维度。（即为把 dis 数组换成 vis 数组，然后距离通过队列层序得到；但无论如何队列中的 Node 状态不能省略，即最坏复杂度不变，只是剪枝）
 */
// LC 3568，收集所有垃圾（'L'格）最少步数；要求每时刻能量非负
struct Node {
    int x, y;
    int e;
    int mask;
};
const int N = 25, M = 1030;
int mxE[N][N][M];  // 在 (x,y)，收集的集合为 mask 时，剩余的最多能量
class Solution {
public:
    int minMoves(vector<string>& grid, int energy) {
        int n = grid.size(), m = grid[0].size();
        int sx, sy, cnt = 0;
        vector<vector<int>> id(n, vector<int>(m, -1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (grid[i][j] == 'S') {
                    sx = i, sy = j;
                } else if (grid[i][j] == 'L') {
                    id[i][j] = cnt;
                    ++cnt;
                }
            }
        }
        int M = 1 << cnt;
        for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) for (int mask = 0; mask < M; ++mask) mxE[i][j][mask] = -1;
        mxE[sx][sy][0] = energy;
        queue<Node> q; q.emplace(Node(sx, sy, energy, 0));
        int d = 0;
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {  // 当且队列中所有点和初始点的距离为 d
                auto [x, y, e, mask] = q.front(); q.pop();
                if (mask == M - 1) return d;
                if (e == 0) continue;
                for (int i = 0; i < 4; ++i) {
                    int nx = x + dx[i], ny = y + dy[i];
                    if (nx < 0 || nx >= n || ny < 0 || ny >= m || grid[nx][ny] == 'X') continue;
                    int nm = mask;
                    if (grid[nx][ny] == 'L') nm |= 1 << id[nx][ny];
                    int ne = grid[nx][ny] == 'R' ? energy : e - 1;
                    if (mxE[nx][ny][nm] >= ne) continue;
                    mxE[nx][ny][nm] = ne;
                    q.emplace(Node(nx, ny, ne, nm));
                }
            }
            ++d;
        }
        return -1;
    }
};