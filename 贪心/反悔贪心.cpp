#include <bits/stdc++.h>
using namespace std;

/**
 * 反悔贪心：做决策时，把所有值存在一个堆中
 * 1. 先全部选上，违反要求时撤销掉一个历史最差选择
 * 2. 先一个都不选，违反要求时选择一个历史最佳选择
 * 前提：当前若违反要求，就不能继续了（否则可能后续值可以补上，这就需要DP了）
 * 
 * 模板以最少加油次数为例；先都不选，等油量没有了，再选一个之前的最大油量加入
 */
int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
    stations.push_back({target, 0});
    long long curp = 0, curf = startFuel;
    priority_queue<int, vector<int>> pq;
    int res = 0;
    for (auto& s : stations) {
        int p = s[0], f = s[1];
        curf -= p - curp, curp = p;
        while (curf < 0 && !pq.empty()) {
            int x = pq.top(); pq.pop();
            curf += x;
            ++res;
        }
        if (curf < 0) return -1;
        pq.emplace(f);
    }
    return res;
}