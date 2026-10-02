#include <bits/stdc++.h>
using namespace std;

/**
 * SOS DP / Sum Over Subsets
 * 
 * 状态定义：
 * 定义 f[mask] 表示所有 sub ⊆ mask 的贡献合并结果：f[mask] = merge_{sub ⊆ mask} f[sub]
 * 
 * 状态转移：
 * 初始化 f[mask] 表示 mask 本身的原始贡献
 * 转移时，若 mask 第 i 位为 1，考虑前面所有位不变，第 i..0 位可以自由选择，则有 f[mask] = merge(f[mask], f[mask ^ (1 << i)])
 * 
 * 复杂度：
 * O(n * 2^n)
 */

// 原版
vector<int> sosSubset(vector<int> f, int n) {
    int N = 1 << n;
    int full = N - 1;

    // 修改：子集合并的函数
    auto merge = [&](int x, int y) {
        return max(x, y);
    };

    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < N; ++mask) {
            if (mask >> i & 1) {
                f[mask] = merge(f[mask], f[mask ^ (1 << i)]);
            }
        }
    }
    return f;
}

// 位运算优化
vector<int> sosSubset(vector<int>& a) {
    int mx = ranges::max(a);
    int w = bit_width((uint32_t) mx);
    int N = 1 << w;

    vector<int> f(N);
    for (int x : a) {
        // 修改 f 初值
        ++f[x];
    }

    for (int i = 0; i < w; ++i) {
        for (int mask = 0; mask < N; ++mask) {
            mask |= 1 << i;
            int submask = mask ^ (1 << i);
            
            // f[mask] <--merge-- f[submask]
            f[mask] += f[submask];
        }
    }
    return f;
}