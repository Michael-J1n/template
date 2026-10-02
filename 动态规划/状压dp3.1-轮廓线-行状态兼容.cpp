#include <bits/stdc++.h>
using namespace std;
const long long LINF = 4e18;

/**
 * 轮廓线 DP - 行状态兼容形
 * 
 * 适用 n,m 都较小的棋盘；定义 f[i][mask] 为考虑行 0..i，第 i 行的状态是 mask 的状态值
 * 注意 n 小 m 大时要交换
 * 模板以 nxm 的棋盘选出互补相邻的格子使得总和最大为例（八个方向的相邻）
 * 
 * 复杂度：O(n * m * 2^m + 4^m)
 */
void profileDP(vector<vector<int>>& a) {
	int n = a.size(), m = a[0].size();
    int M = 1 << m;
    // 预处理单独成行时的合法状态
	vector<int> valid;
	for (int mask = 0; mask < M; ++mask) {
		if (!(mask & (mask << 1))) valid.emplace_back(mask);
	}
    // sum[i][mask] = 第 i 行，填写状态为 mask 时数字之和
	vector sum(n, vector<long long>(M));
	for (int i = 0; i < n; ++i) {
		for (auto& mask : valid) {
			for (int j = 0; j < m; ++j) {
				if (mask >> j & 1) sum[i][mask] += a[i][j];
			}
		}
	}
    // prv[mask] = 当前行状态为 mask 时，上一行的合法前驱状态
	vector<vector<int>> prv(M);
	for (auto& mask : valid) {
		for (auto& pmask : valid) {
			if ((mask & pmask) || (mask & (pmask << 1)) || (mask & (pmask >> 1))) continue;
			prv[mask].emplace_back(pmask);
		}
	}
    // f[i][mask] = 考虑行 0..i，第 i 行填写状态为 mask 时的最大总和
	vector f(n, vector<long long>(M, -LINF));
	for (auto& mask : valid) f[0][mask] = sum[0][mask];
	for (int i = 1; i < n; ++i) {
		for (auto& mask : valid) {
			for (auto& pmask : prv[mask]) {
				f[i][mask] = max(f[i][mask], f[i - 1][pmask] + sum[i][mask]);
			}
		}
	}
    // 枚举最后一行的填写状态更新答案
	long long res = -LINF;
	for (auto& mask : valid) res = max(res, f[n - 1][mask]);
	cout << res << endl;
}