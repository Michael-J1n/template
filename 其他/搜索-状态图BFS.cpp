#include <bits/stdc++.h>
using namespace std;

/**
 * 隐式图 BFS【求最少操作次数】
 * 适用：起点状态和终点状态的转移中可以由多个对于单个位置的操作组成；求两状态间的最小操作次数（前提是状态空间不大，一般体现在数据范围较小）。
 * 模板以从密码 "0000" 到 target 的最小操作数，其中不能经过 deadends 中的形态
 * 注意有些隐式图上的状态还可以状压，减轻复杂度
 * 
 * 状态作为图上的点，一次合法操作作为边；不显式建图，在 BFS 中现场生成所有邻居
 * 无权图最短路直接 BFS；每个状态第一次访问时的距离就是最短距离
 *
 * 复杂度：O(SD)，其中 S 为可达状态数，D 为单个状态转移数
 * 空间 O(S)
 */
int openLock(vector<string>& deadends, string target) {
    string start = "0000";
    unordered_set<string> ban(deadends.begin(), deadends.end());
    if (ban.contains(start)) return -1;
    queue<string> q; q.emplace(start);
    unordered_set<string> vis; vis.insert(start);
    int dis = 0;  // 当前层与起点之间的边数
    while (!q.empty()) {
        int sz = q.size();
        while (sz--) {
            auto u = q.front(); q.pop();
            if (u == target) return dis;
            for (int i = 0; i < 4; ++i) {
                for (int d : {-1, 1}) {
                    string v = u;
                    v[i] = (v[i] - '0' + d + 10) % 10 + '0';
                    if (ban.contains(v) || vis.count(v)) continue;
                    vis.insert(v);
                    q.emplace(v);
                }
            }
        }
        ++dis;
    }
    return -1;
}