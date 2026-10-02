#include <bits/stdc++.h>
using namespace std;

/**
 * 函数图倍增：jump(u,k) 返回从 u 沿 nxt 走 k 步后的点，构建 O(nlogK)，查询 O(logK)
 * 如果 k 是 int，把 LOG 改成 31
 */
const int LOG = 63;
struct FunctionalGraph {
    int n;
    vector<vector<int>> up;
    FunctionalGraph(const vector<int>& nxt) : n(nxt.size()) {
        up.assign(LOG, vector<int>(n));
        up[0] = nxt;
        for (int k = 1; k < LOG; ++k) {
            for (int i = 0; i < n; ++i) {
                up[k][i] = up[k - 1][up[k - 1][i]];
            }
        }
    }
    int jump(int cur, long long k) {
        for (int j = LOG - 1; ~j; --j) {
            if (k >> j & 1) cur = up[cur][j];
        }
        return cur;
    }
};