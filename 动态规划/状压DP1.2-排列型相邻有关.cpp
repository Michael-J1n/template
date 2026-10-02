#include <bits/stdc++.h>
using namespace std;

/**
 * 状压 DP -- 排列型 相邻有关
 * 相邻有关代表下一位的条件不仅和之前的状态掩码有关，还和排列中上一位有关，因此多开一个维度代表上一位
 *
 * 状态定义：f[mask][last] 表示已经选择 mask（中1代表的数）集合，且排列最后一位为 last 时的代价
 *
 * 状态转移：枚举当前集合 mask 以及排列中最后一个数 last，再枚举填写的下一个元素 nxt 进行更新：f[nmask][nxt] <-- f[mask][last]
 *
 * 复杂度：O(C * n^2 * 2^n)，其中 C 为状态转移复杂度
 */
int subsetDP(int n) {
    int N = 1 << n;
    vector f(N, vector<int>(n));
    f[1][0] = 0;  // 可选：对于 f[1<<i][i] 的初始化；取决于是否可以从空状态转移
    for (int mask = 0; mask < N; ++mask) {
        for (int last = 0; last < n; ++last) {
            if (!(mask >> last & 1)) continue;  // check feasibility of f[mask][last]
            for (int nxt = 0; nxt < n; ++nxt) {
                if (mask >> nxt & 1) continue;  // check feasibility of nxt <-- last
                int nmask = mask | (1 << nxt);
                // 刷表转移：f[nmask][nxt] <-- f[mask][last]
            }
        }
    }
    int res;
    // res <-- f[N - 1][last]
    return res;
}