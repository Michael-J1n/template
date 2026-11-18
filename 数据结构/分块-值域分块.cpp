#include <bits/stdc++.h>
using namespace std;

/**
 * 动态有序多重集合
 * multiset 语义，用根号时间的增删换来常数时间的查询，适合修改较少、查询较多的情况。
 * 索引约定：值域 [0, n - 1]，rank 和 kth 的排名为 1-based。del(x) 要求 x 存在，所有查询的 x 必须在值域内。
 *
 * 支持操作：
 * add(x)：插入一个 x
 * del(x)：删除一个 x
 * cnt(x)：查询 x 的出现次数
 * cnt_le(x)：查询 <= x 的元素数量
 * rank(x)：查询 x 的排名，即严格 cnt(<x) + 1
 * kth(k)：查询第 k 小的元素，不存在返回 n
 * prv(x)：查询严格小于 x 的最大元素，不存在返回 INT_MIN
 * nxt(x)：查询严格大于 x 的最小元素，不存在返回 INT_MAX
 * sum()：查询所有元素之和
 * sum_lt(x)：查询严格小于 x 的元素之和
 * sum_gt(x)：查询严格大于 x 的元素之和
 * size()：查询所有元素个数
 *
 * 复杂度（记离散化后值域大小为 n）：
 * 初始化：O(n)；修改均摊 O(sqrt(n))；所有查询 O(1).
 */
struct VBlock {
    int n, B, m;
    /**
     * pre[i] = i 所在块中的 cnt(<=i)；pre_sum[i] = i 所在块中的 sum(<=i)
     * prefeix_cnt[b] = 前 b 个块的元素总数；prefix_sum[b] = 前 b 个块的元素之和
     * f[b][k] = 第 b 块中第 k 小的元素
     * mp[k] = 全局第 k 小元素的所在块
     */
    vector<int> pre, prefix_cnt, mp;
    vector<long long> pre_sum, prefix_sum;
    vector<vector<int>> f;
    VBlock(int n) : n(n) {
        B = sqrt(n) + 1;
        m = (n + B - 1) / B;
        pre.resize(n); pre_sum.resize(n);
        prefix_cnt.resize(m + 1); prefix_sum.resize(m + 1);
        f.assign(m, vector<int>(1));
        mp.resize(1);
    }
    void add(int x) {
        int b = x / B;
        int r = min(n, (b + 1) * B) - 1;
        int f_size = f[b].size(), mp_size = mp.size();
        if (f_size == pre[r] + 1) f[b].push_back(0);
        if (mp_size == prefix_cnt[m] + 1) mp.push_back(0);
        for (int i = r; i >= x; --i) {
            f[b][++pre[i]] = i;
            pre_sum[i] += x;
        }
        for (int i = m; i > b; --i) {
            mp[++prefix_cnt[i]] = i - 1;
            prefix_sum[i] += x;
        }
    }
    void del(int x) {
        int b = x / B;
        int r = min(n, (b + 1) * B) - 1;
        for (int i = x; i <= r; ++i) {
            f[b][pre[i]--] = i + 1;
            pre_sum[i] -= x;
        }
        for (int i = b + 1; i <= m; ++i) {
            mp[prefix_cnt[i]--] = i;
            prefix_sum[i] -= x;
        }
    }
    int cnt(int x) { return pre[x] - (x % B ? pre[x - 1] : 0); }
    int cnt_le(int x) { return prefix_cnt[x / B] + pre[x]; }
    int rank(int x) { return cnt_le(x) - cnt(x) + 1; }
    int kth(int k) {
        if (k <= 0 || k > prefix_cnt[m]) return n;
        int b = mp[k];
        return f[b][k - prefix_cnt[b]];
    }
    int prv(int x) {
        int k = rank(x) - 1;
        return k ? kth(k) : INT_MIN;
    }
    int nxt(int x) {
        int k = cnt_le(x) + 1;
        return k <= prefix_cnt[m] ? kth(k) : INT_MAX;
    }
    long long sum() { return prefix_sum[m]; }
    long long sum_lt(int x) {
        if (x == 0) return 0;
        return prefix_sum[(x - 1) / B] + pre_sum[x - 1];
    }
    long long sum_gt(int x) { return sum() - prefix_sum[x / B] - pre_sum[x]; }
    int size() { return prefix_cnt[m]; }
};