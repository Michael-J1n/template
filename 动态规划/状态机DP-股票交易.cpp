#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3f3f3f3f;
const long long LINF = LLONG_MAX / 2;

/**
 * 股票买卖系列问题
 * 分析每天的状态，要么不持有，要么持有（或long与short），对应设置状态并转移
 * 如果增加一个维度限制，dp 增加一个维度代表此限制即可
 */

// 1. 无限制
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<int> f0(n, INF);  // at day i, max profit w/ on hold
    vector<int> f1(n, INF);  // at day i, max profit w/o hold
    f0[0] = -prices[0], f1[0] = 0;
    for (int i = 1; i < n; ++i) {
        f0[i] = max(f0[i - 1], f1[i - 1] - prices[i]);
        f1[i] = max(f1[i - 1], f0[i - 1] + prices[i]);
    }
    return f1[n - 1];
}

// 2. 限制总共交易 k 次，增加一个维度代表完成的交易次数
int maxProfit(int k, vector<int>& prices) {
    int n = prices.size(), res = 0;
    vector f0(n, vector<int>(k + 1, -INF));  // at day i, completed j trans, max profit w/  hold
    vector f1(n, vector<int>(k + 1, -INF));  // at day i, completed j trans, max profit w/o hold 
    f0[0][0] = -prices[0], f1[0][0] = 0;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j <= k; ++j) {
            f0[i][j] = max(f0[i - 1][j], f1[i - 1][j] - prices[i]);
            f1[i][j] = f1[i - 1][j];
            if (j) f1[i][j] = max(f1[i][j], f0[i - 1][j - 1] + prices[i]);
        }
    }
    for (int j = 0; j <= k; ++j) res = max(res, f1[n - 1][j]);
    return res;
}

// 3. 持有可以是做多或做空
long long maximumProfit(vector<int>& prices, int k) {
    int n = prices.size();
    vector f0(n, vector<long long>(k + 1, -LINF));  // at day i, j trans done, max profit w/ long  hold
    vector f1(n, vector<long long>(k + 1, -LINF));  // at day i, j trans done, max profit w/ short hold
    vector f2(n, vector<long long>(k + 1, -LINF));  // at day i, j trans done, max profit w/o hold
    f0[0][0] = -prices[0], f1[0][0] = prices[0], f2[0][0] = 0;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j <= k; ++j) {
            f0[i][j] = max(f0[i - 1][j], f2[i - 1][j] - prices[i]);
            f1[i][j] = max(f1[i - 1][j], f2[i - 1][j] + prices[i]);
            f2[i][j] = f2[i - 1][j];
            if (j) f2[i][j] = max(f2[i][j], max(f0[i - 1][j - 1] + prices[i], f1[i - 1][j - 1] - prices[i]));
        }
    }
    long long res = 0;
    for (int j = 0; j <= k; ++j) res = max(res, f2[n - 1][j]);
    return res;
}

// 4. 含有一天冷冻期
int maxProfit(vector<int>& prices) {
    int n = prices.size();
    vector<int> f0(n, -INF);  // at day i, max profit w/  hold
    vector<int> f1(n, -INF);  // at day i, max profit w/o hold & on frozen
    vector<int> f2(n, -INF);  // at day i, max profit w/o hold & not on frozen
    f0[0] = -prices[0], f2[0] = 0;
    for (int i = 1; i < n; ++i) {
        f0[i] = max(f0[i - 1], f2[i - 1] - prices[i]);
        f1[i] = f0[i - 1] + prices[i];
        f2[i] = max(f1[i - 1], f2[i - 1]);
    }
    return max(f1[n - 1], f2[n - 1]);
}

// 5. 开仓支付手续费
int maxProfit(vector<int>& prices, int fee) {
    int n = prices.size();
    vector<int> f0(n, -INF);  // at day i, max profit w/  hold
    vector<int> f1(n, -INF);  // at day i, max profit w/o hold
    f0[0] = -prices[0] - fee, f1[0] = 0;
    for (int i = 1; i < n; ++i) {
        f0[i] = max(f0[i - 1], f1[i - 1] - prices[i] - fee);
        f1[i] = max(f1[i - 1], f0[i - 1] + prices[i]);
    }
    return f1[n - 1];
}