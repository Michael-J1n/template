#include <bits/stdc++.h>
using namespace std;

/**
 * 错排列数
 * 对于 1~n 的排列 P，如果任意 i 都有 P[i] != i，那么这是一个错排列
 * 求 n 个数的错排列的方案数 D[n]；有 D[i] = (i-1)(D[i-1] + D[i-2])
 * 
 * 考虑如果 n 放在了第 n 位，前 1~n-1 个数已经摆好，有多少种方法能一步变为 n 位错排
 * 首先若 1~n-1 都错排，n 可以选择与任何一个数交换；否则若存在且仅有一个数不在错排位置，n 与它交换
 * 
 * 例题：给定 1~n 排列，求有多少种排列使得恰好有 m 个满足 a[i]==i；
 * 首先有 C(n,m) 个选法选出不动点；剩余的为 n-m 的错排问题，方案数 D[n-m].
 * */
const int mod = 1e9 + 7;
const int N = 1e6 + 5;
long long D[N];
void init() {
    D[0] = 1, D[1] = 0, D[2] = 1;
    for (int i = 3; i < N; ++i) D[i] = 1LL * (i - 1) * ((D[i - 1] + D[i - 2]) % mod) % mod;
}