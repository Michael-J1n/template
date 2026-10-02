#include <bits/stdc++.h>
using namespace std;

/**
 * 合法子序列 DP
 * 
 * 状态定义：
 * 定义 f[x] 为以 **值 x 结尾** 的合法子序列的最优状态
 * 
 * 状态转移：
 * 假设 y 为一个合法的前趋值，用 f[y] 更新 f[x]
 * 
 * 复杂度：
 * 假设每个元素的前驱个数为 O(k)，则总复杂度为 O(nk)
 * 
 * 模板中 pre 填写所有能转移到 x 的前驱值 y
 * 
 * 注意：如果同层会有相同值更新，要开一个 nf 防止串联更新
 */

// 最长长度
int legalSubseqBest(vector<int>& a) {
    unordered_map<int, int> f;
    int res = 0;

    auto pre = [&](int x) {
        vector<int> p;
        
        return p;
    };

    for (auto& x : a) {
        int cur = 1;
        for (auto& y : pre(x)) {
            if (f.count(y)) cur = max(cur, f[y] + 1);
        }
        f[x] = max(f[x], cur);
        res = max(res, f[x]);
    }
    return res;
}

// 合法子序列个数
int legalSubseqCount(vector<int>& a) {
    const int mod = 1e9 + 7;
    using ll = long long;

    unordered_map<int, long long> f;
    long long res = 0;

    auto pre = [&](int x) {
        vector<int> p;
        
        return p;
    };

    for (auto& x : a) {
        long long cur = 1;
        for (auto& y : pre(x)) {
            if (f.count(y)) (cur += f[y]) %= mod;
        }
        (f[x] += cur) %= mod;
        (res += cur) %= mod;
    }
    return (int)res;
}