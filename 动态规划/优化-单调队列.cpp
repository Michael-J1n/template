#include <bits/stdc++.h>
using namespace std;

/**
 * 单调队列优化 DP 模板
 * 优化形如 ndp[j] = max_k (dp[k]) + val[j] 的转移，其中 k 的合法范围为 [max(0,j-len[j]), min(n-1,j+len[j])]
 * 
 * 参数：dp 为滚动数组上一层的 dp；len[j] 代表 j 处合法范围半径，val[j] 代表 j 处价值
 * 条件：区间左右端点随 j 单调不降；模板中 dp 会被直接更新为 ndp
 * dq 维护候选下标，满足对应 dp 值单调递减；p 表示尚未加入队列的最小下标；每次更新前在队首删除已经离开左端点的状态，队尾删除不可能成为最优解的状态，并加入新的合法下标
 * 
 * 复杂度：O(n)
 */
void monotonicQueueDP(vector<long long>& dp, vector<int>& len, vector<long long>& val) {
    int n = dp.size();
    vector<long long> ndp(n, -4e18);
    deque<int> dq;
    int p = 0;
    for (int j = 0; j < n; ++j) {
        int l = max(0, j - len[j]), r = min(n - 1, j + len[j]);
        while (!dq.empty() && dq.front() < l) dq.pop_front();
        while (p <= r) {
            while (!dq.empty() && dp[p] >= dp[dq.back()]) dq.pop_back();
            dq.push_back(p);
            ++p;
        }
        ndp[j] = dp[dq.front()] + val[j];
    }
    swap(dp, ndp);
}