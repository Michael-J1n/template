#include <bits/stdc++.h>
using namespace std;

#define eb emplace_back

vector<vector<int>> a;  // 矩阵

int n = a.size(), m = a[0].size();

vector<vector<int>> diag, diagRev;
vector<vector<int>> anti, antiRev;

// 1. 正对角线 ↘
// i - j = const
// 外部顺序: 右上 -> 左下
for (int d = 1 - m; d <= n - 1; ++d) {
    vector<int> v;
    int x = max(0, d), y = x - d;
    while (x < n && y < m) v.eb(a[x][y]), ++x, ++y;
    diag.eb(v);
}

// 2. 正对角线反向 ↖
// i - j = const
// 外部顺序: 右上 -> 左下
for (int d = 1 - m; d <= n - 1; ++d) {
    vector<int> v;
    int x = min(n - 1, m - 1 + d), y = x - d;
    while (x >= 0 && y >= 0) v.eb(a[x][y]), --x, --y;
    diagRev.eb(v);
}

// 3. 副对角线 ↙
// i + j = const
// 外部顺序: 左上 -> 右下
for (int s = 0; s <= n + m - 2; ++s) {
    vector<int> v;
    int x = max(0, s - m + 1), y = s - x;
    while (x < n && y >= 0) v.eb(a[x][y]), ++x, --y;
    anti.eb(v);
}

// 4. 副对角线反向 ↗
// i + j = const
// 外部顺序: 左上 -> 右下
for (int s = 0; s <= n + m - 2; ++s) {
    vector<int> v;
    int x = min(n - 1, s), y = s - x;
    while (x >= 0 && y < m) v.eb(a[x][y]), --x, ++y;
    antiRev.eb(v);
}