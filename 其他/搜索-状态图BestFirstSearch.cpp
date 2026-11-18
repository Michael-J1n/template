#include <bits/stdc++.h>
using namespace std;

/**
 * 隐式图 + Best First Search【求第 k 小可达状态】
 * 模板以 ABC 451D 为例，求第 k 小的好数，其中好数为通过2的幂字符串拼接成的字符串整数；保证答案不超过 1e9
 * 
 * 状态 u 不显式建图，通过转移现场生成 v；小根堆每次取当前最优状态进行扩展搜索，uset 在入堆时去重
 *
 * 复杂度 O(SD + SlogS)，其中 S 为状态数，D 为单个状态可转移数
 */
const int MX = 1e9;          // 答案上界
vector<pair<int, int>> vec;  // {二的幂，对应的十进制位数} 需要在init中预处理，这里省去
int bestFirstSearch(int k) {
    priority_queue<int, vector<int>, greater<>> pq; pq.emplace(0);
    unordered_set<int> vis; vis.insert(0);
    int cnt = -1;  // 当且是第几小的好数；0 只作为初始生成元，不计入状态（因此取出后是第 0 小的）
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        ++cnt;
        if (cnt == k) return u;
        // 从最优状态出发，按照规则进行扩展
        for (auto& [x, pw] : vec) {
            if (1LL * u * pw + x > MX) break;
            int v = u * pw + x;
            if (vis.count(v)) continue;
            vis.insert(v);
            pq.emplace(v);
        }
    }
}