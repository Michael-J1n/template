#include <bits/stdc++.h>
using namespace std;

/**
 * 1. O(2^popcount) 枚举子集
 * 
 * 2. O(3^n) 枚举两个不相交子集
 * 
 */
// 枚举 mask 的所有非空子集
void enumerateSubmasks(int mask) {
    for (int b = mask; b; b = (b - 1) & mask) {
        // b 是 mask 的子集
    }
}
// 枚举 mask 的所有子集
void enumerateSubmasksWithEmpty(int mask) {
    for (int b = mask; ; b = (b - 1) & mask) {
        // b 是 mask 的子集，包含空集
        if (!b) break;
    }
}
// 枚举 n 比特内的两个不相交子集
void enumerateDisjointPairs(int n) {
    int N = 1 << n, full = N - 1;
    for (int a = 0; a < N; ++a) {
        int rem = full ^ a;
        for (int b = rem; b; b = (b - 1) & rem) {
            // a 和 b 是两个不相交子集
        }
    }
}

/**
 * 例题：最优转账
 * 给定一组边权关系 {u,v,w}，表示 u 欠 v 共 w 钱，求平账需要的最少转账次数。首先可以把边权转为点权：{u,v,w} 为 val[u]-=w, val[v]+=w 来维护每个人的净支出/流入
 * 思考：如果是最小转账总和，是固定的，实现中正负分组相向双指针，每次平掉 min(neg[i],pos[i]) 即可
 * 如果是最少转账次数，考虑一个零和团：如果他是一个极小零和团，那么假设为 {x1,x2,...,xn}，满足 x1+x2+..+xn=0，且没有任何一个非空真子集满足这个性质
 * 首先，它可以用 n-1 次转账平账：x1-x1,x2-x2,..,xn+x1+x2+..+x{n-1} 结果全是 0. 现在假设可以用 n-2 次平，那么有两个数没有被转账过，且他们非零，因此一定是两个非空不交子集分别贡献的结果，与极小零和团矛盾。
 * 
 * 因此，题目转化为：对于所有不为0的点权，分出最多的组使得每个组都是零和的。组内答案为 gsize-1，因此全局答案为 m-cnt，其中 m 为非零点个数，cnt 为组数
 * 考虑状压 DP，定义 dp[mask] 为将 mask 分成最多的零和不交子集个数。
 * 枚举所有子集 sub，若 sum[sub]=0，则可以将他作为一个子集划分出去，更新 dp[mask]<-dp[sub]+dp[mask^sub].
 * 
 * 实现上，对于所有子集，先预处理出子集的和（使用__lg取首位1）；在转移上，如果 mask 本身和不为零，无法划分，保持为 -INT_MAX 代表不可达状态
 * 否则，枚举子集 sub 使得 sum[sub]=0 作为第一层划分：(sub,mask^sub)，然后来计算转移
 */
int minTransitions(int n, const vector<tuple<int, int, long long>>& edges) {
    vector<long long> val(n);
    for (auto& [u, v, w] : edges) val[u] -= w, val[v] += w;
    vector<long long> a;
    for (auto& x : val) {
        if (x) a.emplace_back(x);
    }
    int m = a.size(), M = 1 << m;
    vector<long long> sum(M);  // sum[mask] = 子集 mask 的和
    for (int mask = 1; mask < M; ++mask) {
        int i = __lg(mask);
        sum[mask] = sum[mask ^ (1 << i)] + a[i];
    }
    vector<int> dp(M, -INT_MAX); dp[0] = 0;  // dp[mask] = 将集合 mask 分为零和团的最多团数
    for (int mask = 1; mask < M; ++mask) {
        if (sum[mask]) continue;
        dp[mask] = 1;
        for (int sub = mask; sub; sub = (sub - 1) & mask) {
            if (sum[sub]) continue;
            dp[mask] = max(dp[mask], dp[sub] + dp[mask ^ sub]);
        }
    }
    return m - dp[M - 1];
}