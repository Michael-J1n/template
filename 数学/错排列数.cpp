/**
 * 对于 1~n 的排列 P，如果任意 i 都有 P[i] != i，那么这是一个错排列
 * 求 n 个数的错排列有多少种方案
 * 
 * 记 n 位错排数为 D[n]
 * 考虑如果 n 放在了第 n 位，前 1~n-1 个数已经摆好，有多少种方法能一步变为 n 位错拍
 * 首先若 1~n-1 都错排，n 可以选择与任何一个数交换
 * 否则若存在且仅有一个数不在错排位置，n 与它交换
 * 因此递推式 D[i] = (i-1)(D[i-1] + D[i-2])
 * 边界情况 D[1] = 0, D[2] = 1
 * */

#include <bits/stdc++.h>
using namespace std;

const int N = 22;
long long D[N];

void init() {
    D[1] = 0, D[2] = 1;
    for (int i = 3; i < N; ++i) D[i] = (i - 1) * (D[i - 1] + D[i - 2]);
}

int main() {
    init();
    int n; cin >> n;
    cout << D[n] << endl;
    return 0;
}