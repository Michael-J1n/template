#include <bits/stdc++.h>
using namespace std;

/**
 * SOS DP / Sum Over Subsets
 * 本质上是将状压 DP 转移中要对于每个 mask 都要 O(2^n) 枚举所有对应子集的时间整体优化成 O(n*2^n)
 * 
 * 状态定义：
 * 有朴素定义 f[i][mask] 为只允许修改 0..i 这些位时，所有能从 mask 得到的 submask 的贡献；
 * 如果 i 位为 0，那么 f[i][mask] = f[i-1][mask]，否则 i 位为 1 时，其子集在这一位上可以取 0 或 1，因此 f[i][mask] = merge(f[i-1][mask],f[i-1][mask^(1<<i)]).
 * 滚动优化，定义 f[mask] 表示所有 sub ⊆ mask 的贡献合并结果：f[mask] = merge_{sub ⊆ mask} f[sub]
 * 
 * 状态转移：
 * 初始化 f[mask] 表示 mask 本身的原始贡献
 * 转移时，当 mask 的第 i 位是 1 时，把第 i 位取 0 的子集的贡献合并进第 i 位取 1 的状态: f[mask] = merge(f[mask], f[mask^(1<<i)])
 * 
 * 复杂度：
 * O(n * 2^n)
 */
// 原版
vector<int> sosSubset(vector<int> f, int n) {
    int N = 1 << n;
    int full = N - 1;
    // 修改：子集合并的函数；x,y 代表对应子集的状态值
    auto merge = [&](int x, int y) {
        return max(x, y);
    };
    // 处理完第 i 位以后，f[mask] 表示：对于已经处理过的位 0..i，可以自由决定是否从 mask 中删掉；还没处理的位则必须和 mask 一样。
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