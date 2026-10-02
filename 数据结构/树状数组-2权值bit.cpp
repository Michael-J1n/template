#include <bits/stdc++.h>
using namespace std;

/**
 * 权值 BIT
 * 下标为值，同时维护该值的出现次数和值之和
 * 常见建模：逆序对计数；动态查询某个值域内数的数量与总和
 *
 * 状态定义：
 * xs[i] 表示第 i 小的离散值
 * c 中维护 cnt[xs[i]]，也就是这个值当前出现次数
 * s 中维护 xs[i] * cnt[xs[i]]，也就是这个值贡献的总和
 *
 * 功能：
 * add(x, val)：让值 x 的出现次数 += val
 * count(x)：查询值 x 的出现次数
 * count_le(x)：查询 <= x 的数有多少个
 * count_lt(x)：查询 < x 的数有多少个
 * count_range(l, r)：查询值域 [l, r] 中有多少个数
 * sum(x)：查询所有等于 x 的数的总和
 * sum_le(x)：查询所有 <= x 的数的总和
 * sum_lt(x)：查询所有 < x 的数的总和
 * sum_range(l, r)：查询值域 [l, r] 中所有数的总和
 * query_le(x)：同时返回 {count_le(x), sum_le(x)}
 * query_lt(x)：同时返回 {count_lt(x), sum_lt(x)}
 * query_range(l, r)：同时返回值域 [l, r] 的 {数量, 总和}
 * kth(k)：查询第 k 小的数，k 从 1 开始
 * size()：添加的值的数量
 * tot_sum(): 添加的值的总和
 * 前置条件：add 的 x 必须出现在构造时的 vals 中；频率保持非负；kth 时树非空且 1<=k<=size()
 *
 * 复杂度：
 * 初始化 O(nlogn)
 * 单次修改/查询 O(logn)
 *
 * 注意：
 * 1. 所有可能被 add 的 x 必须提前出现在 vals 里
 * 2. kth(k) 要求 1 <= k <= size()
 * 3. 数量使用 int，总和与统计答案使用 long long（逆序对记得 ll）
 */
struct ValueBIT {
    int n;
    int tot_cnt = 0;
    long long total_sum = 0;
    vector<int> xs, c;
    vector<long long> s;
    ValueBIT(vector<int>& vals) {
        xs = vals;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = xs.size();
        c.resize(n + 1);
        s.resize(n + 1);
    }
    int find(int x) { return ranges::lower_bound(xs, x) - xs.begin(); }
    void add(int x, int val) {
        int id = find(x); ++id;
        tot_cnt += val;
        total_sum += 1LL * x * val;
        for (int i = id; i <= n; i += i & -i) c[i] += val, s[i] += 1LL * x * val;
    }
    pair<int, long long> pref_query(int id) {
        ++id;
        int cnt = 0;
        long long sum = 0;
        for (int i = id; i >= 1; i &= i - 1) cnt += c[i], sum += s[i];
        return {cnt, sum};
    }
    int pref_sum(int id) { return pref_query(id).first; }
    long long pref_value_sum(int id) { return pref_query(id).second; }
    pair<int, long long> query_le(int x) {
        int id = ranges::upper_bound(xs, x) - xs.begin() - 1;
        if (id < 0) return {0, 0};
        return pref_query(id);
    }
    pair<int, long long> query_lt(int x) {
        int id = find(x) - 1;
        if (id < 0) return {0, 0};
        return pref_query(id);
    }
    pair<int, long long> query_range(int l, int r) {
        if (l > r) return {0, 0};
        auto [cnt1, sum1] = query_le(r);
        auto [cnt2, sum2] = query_lt(l);
        return {cnt1 - cnt2, sum1 - sum2};
    }
    int count_le(int x) { return query_le(x).first; }
    int count_lt(int x) { return query_lt(x).first; }
    int count_range(int l, int r) { return query_range(l, r).first; }
    int count(int x) { return count_range(x, x); }
    long long sum_le(int x) { return query_le(x).second; }
    long long sum_lt(int x) { return query_lt(x).second; }
    long long sum_range(int l, int r) { return query_range(l, r).second; }
    long long sum(int x) { return sum_range(x, x); }
    int size() { return tot_cnt; }
    long long tot_sum() { return total_sum; }
    int kth(int k) {
        int pos = 0;
        for (int i = __lg(n); ~i; --i) {
            int nxt = pos + (1 << i);
            if (nxt <= n && c[nxt] < k) pos = nxt, k -= c[nxt];
        }
        return xs[pos];
    }
};
