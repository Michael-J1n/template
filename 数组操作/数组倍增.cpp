#include <bits/stdc++.h>
using namespace std;

/**
 * 数组上倍增
 * 
 * nxt[i] 表示原数组中 i 跳一步可以到达的下一个位置，取值范围为 [0, n]
 * nxt[i] = n 表示没有下一个合法位置；哨兵 n 跳任意步仍为 n，权值为 0
 * 要求 nxt.size() = w.size() = n，0 <= u < n，k >= 0
 *
 * up[j][u] = 从 u 开始，跳 2^j 步到达的位置
 * sum[j][u] = 从 u 开始，跳 2^j 步时每一步出发位置的权值和；提前到达 n 后用 0 补齐
 *             对无环链，包含 u，不包含 up[j][u]
 * 
 * 预处理、空间 O(n LOG)，查询 O(LOG)
 * 第一维是倍增层 j，第二维是位置 i，使预处理内层遍历 i 时连续访问同一层
 */
int n, u, k, target;
long long v;

const int LOG = 31;  // 或按 n 动态取 LOG: __lg(n) + 1

void binaryLift(vector<int>& nxt, vector<int>& w) {
    // 预处理，j 语义上都表示 **恰好** 2^j 步
    // 每层的最后一个位置是哨兵 n
    vector up(LOG, vector<int>(n + 1, n));
    vector sum(LOG, vector<long long>(n + 1));
    for (int i = 0; i < n; ++i) up[0][i] = nxt[i], sum[0][i] = w[i];
    for (int j = 1; j < LOG; ++j) {
        for (int i = 0; i <= n; ++i) {
            int x = up[j - 1][i];
            up[j][i] = up[j - 1][x];
            sum[j][i] = sum[j - 1][i] + sum[j - 1][x];
        }
    }

    {  // 从 u 跳 k 步；结束后 cur = 跳 k 步到达的位置
        int cur = u;
        for (int j = 0; j < LOG; ++j) {
            if (k >> j & 1) {
                cur = up[j][cur];
                if (cur == n) break;
            }
        }
    }

    {  // 最多从 u 跳 k 步；如果中途没有下一个位置，就停在最后一个合法位置
       // 结束后 cur = 最终位置，step = 实际跳的步数
        int cur = u, step = 0;
        for (int j = LOG - 1; ~j; --j) {
            if (step + (1 << j) <= k && up[j][cur] != n) {
                cur = up[j][cur];
                step += 1 << j;
            }
        }
    }

    {  // 跳到最后一个 <target 的位置；结束后 cur = 最后一个 <target 的位置，step = 跳的步数
       // 如果需要第一个 >= target 的位置，循环结束后再做一次 cur = up[0][cur], ++step；cur == n 表示不存在
       // 如果需要最后一个 <=target 的位置，把判断中的 <target 改成 <=target；
       // 如果需要第一个 >target 的位置，先改成 <=target，再在循环结束后 cur = up[0][cur], ++step；cur == n 表示不存在
        int cur = u, step = 0;
        for (int j = LOG - 1; ~j; --j) {
            if (up[j][cur] != n && up[j][cur] < target) {
                cur = up[j][cur];
                step += 1 << j;
            }
        }
    }

    {  // 从 u 开始累加权值，找到第一个使前缀和 >=v 的位置，即第一个满足 w[u] + ... + w[cur] >= v 的 cur
       // 结束后 cur = 这个位置；如果 cur==n，说明所有位置的权值和仍然 <v
       // 要求 w[i]>=0；如果需要第一个 >v 的位置，把判断中的 <need 改成 <=need 即可
        int cur = u;
        long long need = v;
        for (int j = LOG - 1; ~j; --j) {
            if (sum[j][cur] < need) {
                need -= sum[j][cur];
                cur = up[j][cur];
            }
        }
    }
}
