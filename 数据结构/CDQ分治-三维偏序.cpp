#include <bits/stdc++.h>
using namespace std;

/** 权值 BIT */
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

/**
 * CDQ 分治【离线统计三维偏序】
 * 问题：给定 n 个元素，每个元素有 (a[i],b[i],c[i]) 三个值。n<=1e5,V<=1e5. 求 f(i)={#j:a[j]<=a[i],b[j]<=b[i],c[j]<=c[i]}
 * 
 * 参数：
 * 1. [l,r] 代表要计算的区间
 * 2. a 代表按照第一维 x 升序排序后的数组；tmp 是归并排序辅助数组，初始化为 a 即可
 * 3. bit 为权值树状数组，z 值域作为下标
 * 
 * 算法：
 * 思想是分治；对于 [l,r] 上的问题，假设已经做完 [l,mid],[mid+1,r]，剩下的贡献是左边给右边的贡献。
 * 点预先按第一维 x 排序，solve_cdq 统计左半对右半的贡献；分治过程中区间按第二维 y 归并有序，第三维 z 使用 ValueBIT 维护。这样进入 [l,r] 时保证左半的所有 x 都小于右半的所有 y，结束后保证所有点按照 y 升序排序
 *
 * 答案：solve(0, a.size() - 1)
 * 对于一个点 (x,y,z)，其所在 node，满足条件的答案数为：node.ans + node.cnt - 1
 * 
 * 复杂度：O(n log^2 n)
 */
struct Node {
    int x, y, z;
    int cnt = 1;  // 相同的 (x,y,z) 点的重复次数
    int ans = 0;  // 满足偏序条件的左侧点总数
    bool operator<(const Node& node) const {
        return x < node.x;
    }
};
void solve_cdq(int l, int r, vector<Node>& a, vector<Node>& tmp, ValueBIT& bit) {
    if (l == r) return;
    int mid = l + r >> 1;
    solve_cdq(l, mid, a, tmp, bit), solve_cdq(mid + 1, r, a, tmp, bit);
    // 左右内部贡献计算结束；保证左半 x 都小于右半 x，并且两半内部都按照 y 升序排序
    int i = l;
    for (int j = mid + 1; j <= r; ++j) {
        // 加入满足第二维度要求的元素
        while (i <= mid && a[i].y <= a[j].y) {
            bit.add(a[i].z, a[i].cnt);
            ++i;
        }
        // 对于第三个维度进行统计
        a[j].ans += bit.count_le(a[j].z);
    }
    // 撤销全局 BIT 的贡献
    for (int j = l; j < i; j++) bit.add(a[j].z, -a[j].cnt);
    // 归并排序保证 y 按照升序
    i = l;
    int j = mid + 1, p = l;
    while (i <= mid && j <= r) {
        if (a[i].y <= a[j].y) tmp[p++] = a[i++];
        else tmp[p++] = a[j++];
    }
    while (i <= mid) tmp[p++] = a[i++];
    while (j <= r) tmp[p++] = a[j++];
    for (int k = l; k <= r; k++) a[k] = tmp[k];
}