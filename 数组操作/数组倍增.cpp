#include <bits/stdc++.h>
using namespace std;

/**
 * 数组上倍增
 * 
 * nxt[i] 表示原数组中 i 跳一步可以到达的下一个位置；若 nxt[i] = n 说明没有下一个合法位置
 *
 * up[u][j] = 从 u 开始，跳 2^j 步到达的位置
 * sum[u][j] = 从 u 开始，跳 2^j 步时经过的位置权值和，包含 u，不包含 up[u][j]
 * 
 * 预处理 O(nlogV)，查询 O(logV)
 */
int n, u, k, target;
long long v;
const int LOG = 31;
void binaryLift(vector<int>& nxt, vector<int>& w) {
    // 预处理，j 语义上都表示**恰好** 2^j 步
    // 最后一个值是哨兵 n
    vector up(n + 1, vector<int>(LOG, n));
    vector sum(n + 1, vector<long long>(LOG));
    for (int i = 0; i < n; ++i) up[i][0] = nxt[i], sum[i][0] = w[i];
    for (int j = 1; j < LOG; ++j) {
        for (int i = 0; i <= n; ++i) {
            int x = up[i][j - 1];
            up[i][j] = up[x][j - 1];
            sum[i][j] = sum[i][j - 1] + sum[x][j - 1];
        }
    }

    // 从 u 跳 k 步；结束后 cur = 跳 k 步到达的位置
    {
        int cur = u;
        for (int j = 0; j < LOG; ++j) {
            if (k >> j & 1) {
                cur = up[cur][j];
                if (cur == n) break;
            }
        }
    }

    // 最多从 u 跳 k 步；如果中途没有下一个位置，就停在最后一个合法位置
    // 结束后 cur = 最终位置，step = 实际跳的步数
    {
        int cur = u, step = 0;
        for (int j = LOG - 1; ~j; --j) {
            if (step + (1 << j) <= k && up[cur][j] != n) {
                cur = up[cur][j];
                step += 1 << j;
            }
        }
    }

    // 跳到最后一个 <target 的位置；结束后 cur = 最后一个 <target 的位置，step = 跳的步数
    // 如果需要第一个 >= target 的位置，再做一次 cur = up[cur][0], ++step
    // 如果需要最后一个 <=target 的位置，把判断中的 <target 改成 <=target；如果需要第一个 >target 的位置，再做一次 cur = up[cur][0], ++step；这时候如果 cur==n 或 cur<=target，说明不存在
    {
        int cur = u, step = 0;
        for (int j = LOG - 1; ~j; --j) {
            if (up[cur][j] != n && up[cur][j] < target) {
                cur = up[cur][j];
                step += 1 << j;
            }
        }
    }

    // 从 u 开始累加权值，找到第一个使前缀和 >=v 的位置，即第一个满足 w[u] + ... + w[cur] >= v 的 cur
    // 结束后 cur = 这个位置；如果 cur==n，说明所有位置的权值和仍然 <v
    // 要求 w[i]>=0；如果需要第一个 >v 的位置，把判断中的 <need 改成 <=need 即可
    {
        int cur = u;
        long long need = v;
        for (int j = LOG - 1; ~j; --j) {
            if (sum[cur][j] < need) {
                need -= sum[cur][j];
                cur = up[cur][j];
            }
        }
    }
}