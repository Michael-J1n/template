#include <bits/stdc++.h>
using namespace std;

/**
 * 完全背包：最大价值
 * 不同于 01 背包的每件物品选或不选，完全背包每件物品可以选任意多件；因此枚举时正序枚举，可以通过同层物品串联转移
 * 
 * 状态定义：
 * f[j] 表示容量不超过/恰好为 j 时能得到的最大价值（区别在于初始化，恰好要初始化不可达状态为 -INF）
 * 
 * 状态转移：
 * 对每个物品 i，正序枚举容量 j：f[j] = max(f[j], f[j - w[i]] + v[i])
 * 
 * 复杂度：
 * O(nW)
 * 
 * 注意完全背包唯一不同在于体积维度的枚举顺序反了
 */

// 1. 容量至多为 W
int completeKnapsack(vector<int>& w, vector<int>& v, int W) {
    int n = w.size();
    vector<int> f(W + 1, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = w[i]; j <= W; ++j) {
            f[j] = max(f[j], f[j - w[i]] + v[i]);
        }
    }
    return f[W];
}

// 2. 容量恰好为 W【推荐】
const int INF = 0x3f3f3f3f;
int completeKnapsackExact(vector<int>& w, vector<int>& v, int W) {
    int n = w.size();
    vector<int> f(W + 1, -INF); f[0] = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = w[i]; j <= W; ++j) {
            f[j] = max(f[j], f[j - w[i]] + v[i]);
        }
    }
    return f[W] < 0 ? -1 : f[W];
}

// 3. 恰好装满 W 的方案数
long long completeKnapsackCount(vector<int>& w, int W) {
    int n = w.size();
    vector<long long> f(W + 1);
    f[0] = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = w[i]; j <= W; ++j) {
            f[j] += f[j - w[i]];
        }
    }
    return f[W];
}



// 附：二维朴素版，f[i][j] 表示物品 0..i 中选重量至多为 j 的最大价值，每个物品不限制选择次数
// 转移时允许同层更新
int completeKnapsack(vector<int>& w, vector<int>& v, int W) {
    int n = w.size();
    vector f(n, vector<int>(W + 1, 0));
    for (int j = 0; j <= W; ++j) f[0][j] = j / w[0] * v[0];
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j <= W; ++j) {
            f[i][j] = f[i - 1][j];
            if (j >= w[i]) f[i][j] = max(f[i][j], f[i][j - w[i]] + v[i]);
        }
    }
    return f[n - 1][W];
}