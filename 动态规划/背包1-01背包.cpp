#include <bits/stdc++.h>
using namespace std;

/**
 * 给定 n 个物品，每个物品有价值 v 与重量 w 可以选择拿或不拿，求总重量不超过给定阈值 W 时的最大价值
 * 
 * **选或不选** 的代表；定义 f[i][j] 为考虑物品 [0..i]，价值**至多**为 j 的最大价值
 * 有 f[i][j] = max(f[i-1][j], f[i-1][j-w]+v)，滚动数组优化即为：f[j] = max{f[j-w] + v}，需要倒序遍历
 * 注意如果需要价值**恰好**为 j，将 f 初始化为 —INF 代表不可达]
 * 
 * 时间复杂度 O(nW)
 * 
 * 注意，一些问题是凑出一定容量时，最小删除个数/最大选择个数，这相当于每个物品的价值是 1
 */

// 1. 重量至多为 W，求最大价值
long long zeroOneKnapsack(vector<int>& w, vector<int>& v, int W) {
    int n = w.size();
    vector<long long> f(W + 1);
    for (int i = 0; i < n; ++i) {
        for (int j = W; j >= w[i]; --j) {
            f[j] = max(f[j], f[j - w[i]] + v[i]);
        }
    }
    return f[W];
}

// 2. 重量恰好为 W，求最大价值【推荐写法】f[j] 代表体积恰好为 j 时的最大价值
const long long NEG = LLONG_MIN / 4;
long long zeroOneKnapsackExact(vector<int>& w, vector<int>& val, int W) {
    int n = w.size();
    vector<long long> f(W + 1, NEG); f[0] = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = W; j >= w[i]; --j) {
            f[j] = max(f[j], f[j - w[i]] + val[i]);
        }
    }
    return f[W];
}

// 3. 重量上限为 W，求可行性（f[j] 代表凑出重量恰好 j 是否可行；没有价值）
bool zeroOneKnapsackFeasible(vector<int>& w, int W) {
    int n = w.size();
    vector<char> f(W + 1, 0);
    f[0] = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = W; j >= w[i]; --j) {
            f[j] |= f[j - w[i]];
        }
    }
    return f[W];
}

// 4. 对于 3 的 bitset 优化版，省 64 倍常数（但 bitset 的容量需要是一个固定的常数上限，即最大可能值域；注意开 bitset 时要 +1）
const int MXW = 1e6;
bool zeroOneKnapsackFeasible(vector<int>& w, int W) {
    int n = w.size();
    bitset<MXW + 1> f; f[0] = 1;
    for (int i = 0; i < n; ++i) {
        f |= f << w[i];
    }
    return f[W];
}

// 5. 求恰好装满容量 W 的方案数
long long zeroOneKnapsackCount(vector<int>& w, int W) {
    int n = w.size();
    vector<long long> f(W + 1);
    f[0] = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = W; j >= w[i]; --j) {
            f[j] += f[j - w[i]];
        }
    }
    return f[W];
}



// 附：二维朴素版，f[i][j] 表示考虑物品 0..i，容量不超过 j 的最大值
// 注意：朴素写法记得继承上一层状态！！
int zeroOneKnapsack_(vector<int>& w, vector<int>& v, int W) {
    int n = w.size();
    vector f(n, vector<int>(W + 1, 0));
    for (int j = w[0]; j <= W; ++j) f[0][j] = v[0];
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j <= W; ++j) {
            f[i][j] = f[i - 1][j];
            if (j >= w[i]) f[i][j] = max(f[i][j], f[i - 1][j - w[i]] + v[i]);
        }
    }
    return f[n - 1][W];
}