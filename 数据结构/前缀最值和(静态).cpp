#include <bits/stdc++.h>
using namespace std;

vector<int> nxtGt(const vector<int>& a) {
    int n = a.size();
    vector<int> res(n, n), st;
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && a[st.back()] <= a[i]) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.emplace_back(i);
    }
    return res;
}

/**
 * PrefixMaxSum：查询静态区间前缀最大值之和
 *
 * query(l,r) = sum_{i=l}^r max(a[l..i])
 * 做法为单调栈 + 数组倍增；如果需要前缀最小值之和，把单调栈改成 nxtLs 即可。
 *
 * 接口：PrefixMaxSum(a), query(l,r)
 * 适用：静态数组，支持任意区间查询，不支持修改
 *
 * 复杂度：预处理 O(n log n)，查询 O(log n)
 */
struct PrefixMaxSum {
    int n, LOG;
    vector<int> a;
    // up[j][i]：从 i 沿 nxt 跳 2^j 次的终点
    // sum[j][i]：这段跳跃覆盖的前缀最大值之和（包含起点，不包含终点）
    vector<vector<int>> up;
    vector<vector<long long>> sum;
    PrefixMaxSum(const vector<int>& a) : n(a.size()), a(a) {
        LOG = __lg(n) + 1;
        vector<int> nxt = nxtGt(a);
        up.assign(LOG, vector<int>(n + 1, n));
        sum.assign(LOG, vector<long long>(n + 1));
        for (int i = 0; i < n; ++i) {
            up[0][i] = nxt[i];
            sum[0][i] = 1LL * a[i] * (nxt[i] - i);
        }
        for (int j = 1; j < LOG; ++j) {
            for (int i = 0; i <= n; ++i) {
                int x = up[j - 1][i];
                up[j][i] = up[j - 1][x];
                sum[j][i] = sum[j - 1][i] + sum[j - 1][x];
            }
        }
    }
    long long query(int l, int r) const {
        int cur = l;
        long long res = 0;
        for (int j = LOG - 1; ~j; --j) {
            if (up[j][cur] <= r) {
                res += sum[j][cur];
                cur = up[j][cur];
            }
        }
        res += 1LL * a[cur] * (r - cur + 1);
        return res;
    }
};