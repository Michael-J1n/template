#include <bits/stdc++.h>
using namespace std;

/**
 * 给定一个数组，无修改，给定询问 [l,r] 求区间众数（频率相等按数字大小排序）
 * 
 * 众数关键性质：[l,r] 中的众数，候选值只有两类：1. 完整块 L~R 整大块的众数；2. 两边散值；预处理维护完整块，散块暴力算
 * 
 * 状态定义：
 * best[l][r] 为块 l~r 之间出现次数最多的值
 * pre_cnt[b][x] 为值 x 在块 [0~b] 中的出现次数
 * 
 * 复杂度 O(n sqrt n + q sqrt n)
 * 前置条件：构造数组非空；查询区间 [l,r] 非空且位于 [0,n-1]
 */
struct Block {
    int n, B, m;
    vector<int> a;                  // a[i]  = nums[i] 离散化后的压缩值
    vector<int> xs; int tot;        // xs[i] = 离散化值 i 对应的原值
    vector<int> cnt;                // cnt[x] = 临时记录当前查询中枚举多少个 x，查询后必须全部撤销清空
    vector<vector<int>> best;       // best[bl][br]  = 块 bl ~ br 之间出现最多次数的数
    vector<vector<int>> pre_cnt;    // pre_cnt[b][x] = 块 0 ~ b 之间 x 的出现次数
    Block(vector<int>& nums) {
        n = nums.size();
        B = sqrt(n) + 1;
        m = (n + B - 1) / B;
        a = nums;
        xs = nums;
        ranges::sort(xs); xs.erase(unique(xs.begin(), xs.end()), xs.end());
        auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
        for (auto& x : a) x = find(x);
        tot = xs.size();
        cnt.resize(tot);
        best.resize(m, vector<int>(m));
        pre_cnt.resize(m, vector<int>(tot));
        for (int b = 0; b < m; ++b) {
            int mx_cnt = 0, val;
            vector<int> tmp_cnt(tot);
            for (int i = b * B; i < n; ++i) {
                ++tmp_cnt[a[i]];
                if (tmp_cnt[a[i]] > mx_cnt || tmp_cnt[a[i]] == mx_cnt && a[i] < val) mx_cnt = tmp_cnt[a[i]], val = a[i];
                best[b][i / B] = val;
            }
        }
        for (int b = 0; b < m; ++b) {
            if (b) pre_cnt[b] = pre_cnt[b - 1];
            for (int i = b * B; i <= min(n - 1, (b + 1) * B - 1); ++i) ++pre_cnt[b][a[i]];
        }
    }
    int query(int l, int r) {
        int bl = l / B, br = r / B;
        int mx_cnt = 0, val;
        auto update = [&](int x, int l = -1, int r = -1) -> void {
            if (l > r) l = -1, r = -1;
            ++cnt[x];
            int cur_cnt = cnt[x];  // 散块中的出现次数
            if (l != -1 && r != -1) cur_cnt += pre_cnt[r][x] - pre_cnt[l - 1][x];  // 完整块的出现次数
            if (cur_cnt > mx_cnt || cur_cnt == mx_cnt && x < val) val = x, mx_cnt = cur_cnt;
        };
        if (bl == br) {
            for (int i = l; i <= r; ++i) update(a[i]);
            for (int i = l; i <= r; ++i) --cnt[a[i]];
        } else {
            // 完整块众数
            if (bl + 1 <= br - 1) {
                val = best[bl + 1][br - 1];
                mx_cnt = pre_cnt[br - 1][val] - pre_cnt[bl][val];
            }
            // 散块出现最多的值
            for (int i = l; i < (bl + 1) * B; ++i) update(a[i], bl + 1, br - 1);
            for (int i = br * B; i <= r; ++i) update(a[i], bl + 1, br - 1);
            // 统计结束后清空临时桶的贡献
            for (int i = l; i < (bl + 1) * B; ++i) --cnt[a[i]];
            for (int i = br * B; i <= r; ++i) --cnt[a[i]];
        }
        return xs[val];
    }
};
