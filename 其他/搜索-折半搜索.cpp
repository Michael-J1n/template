#include <bits/stdc++.h>
using namespace std;

/**
 * 折半搜索 MIM，当 n~=30~40 时，分为两部分分别搜索，再按照要求合并
 * 复杂度 2^{n/2}*log(2^{n/2}) ~= n2^{n/2}
 * 
 * 模板：求 nums 的子集和等于 target 的方案数
 * dfs(i, r, s, v) 表示在 nums 的 [l, r) 中搜索（注意左闭右开），当前和为 s，保存结果到 v 中的搜索状态
 */

int count(vector<int>& nums, int target) {
    int n = nums.size(), m = n >> 1;
    vector<int> L, R;

    auto dfs = [&](this auto&& dfs, int i, int r, int s, vector<int>& v) -> void {
        if (i == r) {
            v.emplace_back(s);
            return;
        }
        dfs(i + 1, r, s, v);
        dfs(i + 1, r, s + nums[i], v);
    };
    dfs(0, m, 0, L), dfs(m, n, 0, R);
    ranges::sort(R);
    
    int res = 0;
    for (auto& x : L) {
        auto [l, r] = equal_range(R.begin(), R.end(), target - x);
        res += r - l;
    }
    return res;
}

// 如果要求最大不超过 target 的子集和：
// auto it = upper_bound(R.begin(), R.end(), target - x);
// if (it != R.begin()) res = max(res, x + *prev(it));

// 如果要求最接近 target 的子集和：
// auto it = lower_bound(R.begin(), R.end(), target - x);
// if (it != R.end()) res = min(res, abs(x + *it - target));
// if (it != R.begin()) res = min(res, abs(x + *prev(it) - target));