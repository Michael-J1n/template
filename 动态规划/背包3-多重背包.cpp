#include <bits/stdc++.h>
using namespace std;

/**
 * 多重背包求容量不超过 W 时的最大价值
 * 
 * 状态定义：f[j] = 容量不超过 j 时的最大价值
 * 状态转移：将每种物品二进制拆分为若干个 0/1 物品，倒序转移
 * 传入：w[i] 重量，v[i] 价值，c[i] 数量，W 背包容量
 * 返回：最大价值
 * 
 * 复杂度：O(W * sum(log(c[i])))
 */
long long multipleKnapsack(vector<int>& w, vector<int>& v, vector<int>& c, int W) {
    int n = w.size();
    vector<long long> f(W + 1);
    for (int i = 0; i < n; ++i) {
        if (c[i] >= W / w[i]) {
            for (int j = w[i]; j <= W; ++j) f[j] = max(f[j], f[j - w[i]] + v[i]);
        } else {
            int cnt = c[i];
            for (int k = 1; cnt; k <<= 1) {
                int cur = min(k, cnt), nw = cur * w[i];
                long long nv = 1LL * cur * v[i];
                for (int j = W; j >= nw; --j) f[j] = max(f[j], f[j - nw] + nv);
                cnt -= cur;
            }
        }
    }
    return f[W];
}