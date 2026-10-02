#include <bits/stdc++.h>
using namespace std;

/*
LinearAssignmentSolver：C++ 版 linear_sum_assignment，0-based，支持矩形矩阵。

功能：
1. 解决标准带权一对一分配问题 assignment problem。
2. cost[i][j] 表示第 i 行匹配第 j 列的代价。
3. 默认求最小总代价：
   minimize sum(cost[i][matchR[i]])
4. maximize = true 时求最大总收益：
   maximize sum(score[i][matchR[i]])
5. 支持负权。
6. 支持矩形矩阵。
7. 不支持容量、多重匹配、路径依赖、额外组合约束。

矩形矩阵语义：
1. 若 n <= m：
   每一行都会匹配一个列。
   匹配数量为 n。
   可能有一些列不被匹配。
2. 若 n > m：
   每一列都会匹配一个行。
   匹配数量为 m。
   可能有一些行不被匹配。
3. 这和 scipy.optimize.linear_sum_assignment 的矩形行为类似：
   匹配数量为 min(n, m)。

成员变量：
1. n：行数。
2. m：列数。
3. matchR[i]：
   第 i 行匹配到的列。
   若第 i 行未匹配，则为 -1。
4. matchC[j]：
   第 j 列匹配到的行。
   若第 j 列未匹配，则为 -1。

接口：
1. LinearAssignmentSolver(cost)
   建立最小权 assignment solver。
2. LinearAssignmentSolver(score, true)
   建立最大权 assignment solver。
3. work()
   计算最优匹配，返回最优值。
   调用 work() 之后才能读取 matchR / matchC / pairs()。
4. pairs()
   返回所有匹配边 {row, col}。

复杂度：
令 k = min(n, m), s = max(n, m)。
时间复杂度 O(k^2 s)。
方阵 n x n 时为 O(n^3)。
空间复杂度 O(ns)。

禁边：
1. 最小权时，把不能选的边设为 BIG。
2. 最大权时，把不能选的边设为 -BIG。
3. BIG 建议使用 INF / 4。
4. work() 后检查匹配中是否使用了禁边。
5. 不建议直接把禁边设成 INF，容易在运算中溢出。

注意：
1. 输入矩阵不能是 ragged matrix，每一行长度必须相同。
2. cost / score 的绝对值不要接近 LLONG_MAX。
3. 如果总代价可能超过 long long，需要把返回值和 res 改成 __int128。
4. 这是标准 assignment，不是普通无权二分图最大匹配。
5. 无权最大匹配用 Hopcroft-Karp。
6. 有容量限制用费用流 / DP。
*/

struct LinearAssignmentSolver {
    using ll = long long;
    static constexpr ll INF = (1LL << 62);
    int n, m, sgn;
    vector<vector<ll>> a;
    vector<int> matchR, matchC;
    LinearAssignmentSolver(const vector<vector<ll>>& _a, bool maximize = 0) : a(_a) {
        n = a.size(), m = n ? a[0].size() : 0;
        sgn = maximize ? -1 : 1;
        if (maximize) {
            for (auto& r : a) {
                for (auto& x : r) x = -x;
            }
        }
        matchR.assign(n, -1), matchC.assign(m, -1);
    }
    ll solveRect(const vector<vector<ll>>& c, vector<int>& row, vector<int>& col) {
        int nn = c.size(), mm = c[0].size();
        vector<ll> u(nn), v(mm), dist(mm);
        vector<int> pre(mm), col4row(nn, -1), row4col(mm, -1), rem(mm);
        vector<char> SR(nn), SC(mm);
        for (int root = 0; root < nn; ++root) {
            fill(SR.begin(), SR.end(), 0);
            fill(SC.begin(), SC.end(), 0);
            fill(dist.begin(), dist.end(), INF);
            for (int j = 0; j < mm; ++j) rem[j] = mm - 1 - j;
            int cnt = mm, cur = root, sink = -1;
            ll delta = 0;
            while (sink == -1) {
                SR[cur] = 1;
                int idx = -1;
                ll best = INF;
                for (int it = 0; it < cnt; ++it) {
                    int j = rem[it];
                    ll x = delta + c[cur][j] - u[cur] - v[j];
                    if (x < dist[j]) dist[j] = x, pre[j] = cur;
                    if (dist[j] < best || (dist[j] == best && row4col[j] == -1)) best = dist[j], idx = it;
                }
                delta = best;
                int j = rem[idx];
                if (row4col[j] == -1) sink = j;
                else cur = row4col[j];
                SC[j] = 1;
                rem[idx] = rem[--cnt];
            }
            u[root] += delta;
            for (int i = 0; i < nn; ++i) {
                if (SR[i] && i != root) u[i] += delta - dist[col4row[i]];
            }
            for (int j = 0; j < mm; ++j) {
                if (SC[j]) v[j] -= delta - dist[j];
            }
            int j = sink;
            while (1) {
                int i = pre[j];
                row4col[j] = i;
                swap(j, col4row[i]);
                if (i == root) break;
            }
        }
        row = col4row, col = row4col;
        ll res = 0;
        for (int i = 0; i < nn; ++i) res += c[i][row[i]];
        return res;
    }
    ll work() {
        if (!n || !m) return 0;
        ll res;
        if (n <= m) res = solveRect(a, matchR, matchC);
        else {
            vector<vector<ll>> b(m, vector<ll>(n));
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) b[j][i] = a[i][j];
            }
            vector<int> row, col;
            res = solveRect(b, row, col);
            fill(matchR.begin(), matchR.end(), -1), fill(matchC.begin(), matchC.end(), -1);
            for (int j = 0; j < m; ++j) matchC[j] = row[j], matchR[row[j]] = j;
        }
        return res * sgn;
    }
    vector<pair<int, int>> pairs() {
        vector<pair<int, int>> res;
        for (int i = 0; i < n; ++i) {
            if (matchR[i] != -1) res.emplace_back(i, matchR[i]);
        }
        return res;
    }
};