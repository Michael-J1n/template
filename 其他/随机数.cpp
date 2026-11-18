#include <bits/stdc++.h>
using namespace std;

/**
 * 随机数生成引擎
 * 区间约定：整数 [l, r]；浮点数 [l, r)。
 */
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int random_int(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
long long random_ll(long long l, long long r) { return uniform_int_distribution<long long>(l, r)(rng); }
double random_double(double l, double r) { return uniform_real_distribution<double>(l, r)(rng); }