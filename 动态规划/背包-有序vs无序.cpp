#include <bits/stdc++.h>
using namespace std;

/**
 * 考虑完全背包求方案数的问题。不同的排列顺序算作一种方案数。
 * 如果不同的排列顺序算作不同呢？只需要调换循环即可
 * 
 * 还是定义 f[j] 为恰好装满 j 的方案数，有 f[j] += f[j - x]
 * 但是完全背包第一维枚举物品，考虑选几个
 * 而考虑顺序时第二位枚举物品，枚举最后一个数塞谁
 */

// 无序，复杂度 O(nW)
int countUnordered(vector<int>& nums, int W) {
    vector<int> f(W + 1);
    f[0] = 1;
    for (auto& x : nums) {
        for (int j = x; j <= W; ++j) {
            f[j] += f[j - x];
        }
    }
    return f[W];
}

// 有序，复杂度 O(nW)
int countOrdered(vector<int>& nums, int W) {
    vector<int> f(W + 1);
    f[0] = 1;
    for (int j = 1; j <= W; ++j) {
        for (auto& x : nums) {
            if (j >= x) f[j] += f[j - x];
        }
    }
    return f[W];
}