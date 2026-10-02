#include <bits/stdc++.h>
using namespace std;

/**
 * 莫队（不带修）
 * 离线后排序，顺序处理每个询问，暴力从上一个区间的答案转移到下一个区间答案（一步一步移动即可）
 * 对于查询分块，左指针每块动 B 次，总共 qB，右指针每次动 n，共 n/B 块，共 n^2/B
 *
 * 使用场景：
 * 1. 静态数组，多次离线区间查询 [l, r]
 * 2. 当前区间从 [L, R] 移动到目标 [l, r] 时，可以用 add / del 维护答案
 * 3. 常见维护：区间不同数个数、区间众数相关、出现次数贡献、颜色对数等
 *
 * 复杂度：
 * 假设数组长为 n，询问数量 q，块长 B，增删单次均摊 T，读取答案为 A，则复杂度为 O(q log q + (qB + n²/B)T + qA) = 排序 + 指针移动 + 回答
 * 若 q 与 n 同阶，增删 O(1)，复杂度为 O(n * sqrt n)；若增删 O(log n)，复杂度为 O(n * sqrt n * log n)
 * 
 * 注意：
 * 1. 如果值域很大，先离散化
 * 2. n 和 q 不同阶时，块长动态的取 n / sqrt(q)
 * 3. 前置条件：数组非空；每个询问满足 0<=l<=r<n，id 位于 [0,q-1] 且互不重复
 */
struct Query {
    int l, r, id;
};
vector<int> moAlgorithm(vector<int>& a, vector<Query>& qs) {
    int n = a.size();
    int q = qs.size();
    // 离散化
    vector<int> xs = a;
    sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
    int tot = xs.size();
    auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
    for (int i = 0; i < n; ++i) a[i] = find(a[i]);
    // 对查询分块；蛇形排序
    int B = sqrt(n) + 1;
    // int B = max(1, int(n / sqrt(q)));
    ranges::sort(qs, [&](const Query& x, const Query& y) {
        int bx = x.l / B, by = y.l / B;
        if (bx != by) return bx < by;
        if (bx & 1) return x.r > y.r;
        return x.r < y.r;
    });
    /* ------------------ 核心部分：单点维护 ---------------- */
    vector<int> cnt(tot);                   // 频率统计桶
    vector<int> ans(q);                     // 询问对应的答案
    int cur = 0;                            // 维护当前答案
    auto add = [&](int i) -> void {         // 下标为 i 的元素进入查询区间
        int x = a[i];
        if (cnt[x] == 0) ++cur;
        ++cnt[x];
    };
    auto del = [&](int i) -> void {         // 下标为 i 的元素离开查询区间
        int x = a[i];
        --cnt[x];
        if (cnt[x] == 0) --cur;
    };
    // 处理查询
    int L = 0, R = -1;
    for (auto& [l, r, id] : qs) {
        while (L > l) add(--L);
        while (R < r) add(++R);
        while (L < l) del(L++);
        while (R > r) del(R--);
        ans[id] = cur;
    }
    return ans;
}
