#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;

/**
 * 状压 DP -- 排列型 相邻无关
 * 相邻无关代表当前状态掩码可以直接推出下一位的条件，和排列中上一位无关
 *
 * 状态定义：f[mask] 表示已经选择 mask（中1代表的数）集合时的代价
 *
 * 状态转移：枚举当前集合 mask，再枚举一个还没选的元素 i 形成 nmask = mask|(1<<i)，使用 f[mask] 的值更新 f[nmask]
 *
 * 复杂度：O(C * n * 2^n)，其中 C 是刷表时更新的复杂度
 */
int subsetDP(int n) {
    int N = 1 << n;
    vector<int> f(N, INF);
    f[0] = 0;  // 可选：对于 f[1<<i] 的初始化；取决于是否可以从空状态转移
    for (int mask = 0; mask < N; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (mask >> i & 1) continue;
            int nmask = mask | (1 << i);
            // 刷表：f[nmask] <-- f[mask]；例如 f[nmask] = min(f[nmask], f[mask] + cost(mask, i))
        }
    }
    return f[N - 1];
}