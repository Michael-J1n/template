#include <bits/stdc++.h>
using namespace std;

/**
 * 对所有可能的结果进行暴力dfs搜神，状态为指数级的
 * 外部搜索，本质上是完全二叉树上的dfs遍历。需要手动操作回溯（pop掉上一个选的数）
 * 复杂度对照表：
 * 
 *      f(n)    |     n
 * ----------------------——————————
 *      2^n     |     25   子集爆搜
 *   n*2^{n/2}  |     40   折半搜索
 *     n*2^n    |     20   排列相邻无关状压 dp / 子集 SOS DP
 * 3^n, n^2*2^n |     16   排列相邻相关状压 dp / 子集枚举划分 DP / 子集 SOS DP
 *      n!      |     10   排列爆搜
 *     k*nCk    |     24   组合爆搜
 *      nCk     |     28   组合爆搜
 * 
 */

// 1. 子集型回溯：每个数选或不选，共 2^n 个搜素状态；dfs(i) 表示现在考虑第 i 个数，对于 nums[i] 有两个选择：不选或选
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> res;
    vector<int> path;
    
    auto dfs = [&](this auto&& dfs, int i) -> void {
        if (i == n) {
            res.emplace_back(path);
            return;
        }
        dfs(i + 1);
        path.emplace_back(nums[i]);
        dfs(i + 1);
        path.pop_back();
    };
    dfs(0);
    return res;
}

// 2.1 子集型回溯：求出 nums 所有子集；枚举下一个数选谁，共 2^n 个搜索状态；dfs(st) 表示接下来只能从 [st, n-1] 中选择
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> res;
    vector<int> path;

    auto dfs = [&](this auto&& dfs, int st) -> void {
        res.emplace_back(path);
        for (int i = st; i < n; ++i) {
            path.emplace_back(nums[i]);
            dfs(i + 1);
            path.pop_back();
        }
    };
    dfs(0);
    return res;
}

// 2.2 去重子集型回溯：求出 nums 的所有子集，其中不包含重复的子集；排序后枚举下一个数选谁，关键在于同一层不能选相同的数做开头
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    ranges::sort(nums);
    vector<vector<int>> res;
    vector<int> path;
    
    auto dfs = [&](this auto&& dfs, int st) -> void {
        res.emplace_back(path);
        for (int i = st; i < n; ++i) {
            if (i > st && nums[i] == nums[i - 1]) continue;
            path.emplace_back(nums[i]);
            dfs(i + 1);
            path.pop_back();
        }
    };
    dfs(0);
    return res;
}

// 3. 固定长度组合：限制只选 k 个数作为子集，共 nCk 个搜素状态
vector<vector<int>> subsets(vector<int>& nums, int k) {
    int n = nums.size();
    vector<vector<int>> res;
    vector<int> path;
    
    auto dfs = [&](this auto&& dfs, int st) -> void {
        if (path.size() == k) {
            res.emplace_back(path);
            return;
        }
        for (int i = st; i <= n - k + path.size(); ++i) {
            path.emplace_back(nums[i]);
            dfs(i + 1);
            path.pop_back();
        }
    };
    dfs(0);
    return res;
}

// 4.1 全排列形回溯：枚举每个位置填谁，共 n! 个搜索状态；dfs() 表示正在填当前 path.size() 这个位置
vector<vector<int>> permute(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> res;
    vector<int> path;
    vector<char> vis(n, 0);

    auto dfs = [&](this auto&& dfs) -> void {
        if (path.size() == n) {
            res.emplace_back(path);
            return;
        }
        for (int i = 0; i < n; ++i) {
            if (vis[i]) continue;
            vis[i] = 1, path.emplace_back(nums[i]);
            dfs();
            vis[i] = 0, path.pop_back();
        }
    };
    dfs();
    return res;
}
// 使用 STL 的写法：
vector<vector<int>> permute2(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    do {
        // nums 为一个排列
        res.emplace_back(nums);
    } while (next_permutation(nums.begin(), nums.end()));
    return res;
}

// 4.2 全排列型回溯，不包含重复的全排列；关键在于相同的数字在任意排列中需要指定固定的顺序
vector<vector<int>> permute(vector<int>& nums) {
    int n = nums.size();
    ranges::sort(nums);
    vector<vector<int>> res;
    vector<int> path;
    vector<char> vis(n, 0);

    auto dfs = [&](this auto&& dfs) -> void {
        if (path.size() == n) {
            res.emplace_back(path);
            return;
        }
        for (int i = 0; i < n; ++i) {
            if (vis[i] || i && nums[i] == nums[i - 1] && !vis[i - 1]) continue;
            vis[i] = 1, path.emplace_back(nums[i]);
            dfs();
            vis[i] = 0, path.pop_back();
        }
    };
    dfs();
    return res;
}