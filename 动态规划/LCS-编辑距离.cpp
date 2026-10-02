#include <bits/stdc++.h>
using namespace std;

/**
 * f[i][j]：word1前i个，word2前j个，变相等的修改次数
 * 边界条件：f[i][0] = i, f[0][j] = j 对应空串不断增减字符达到匹配的情况
 * 状态转移：
 * 如果相等字符，变为子问题f[i-1][j-1]
 * 否则末尾不相同，可以把任一末位删掉对应，对应f[i-1][j],f[i][j-1]，或者可以替换一个末尾使得相等，子问题f[i-1][j-1]
 * */
int minDistance(string word1, string word2) {
    int n = word1.size(), m = word2.size();
    vector<vector<int>> f(n + 1, vector<int>(m + 1));
    for (int j = 1; j <= m; ++j) f[0][j] = j;
    for (int i = 1; i <= n; ++i) f[i][0] = i;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (word1[i - 1] == word2[j - 1]) f[i][j] = f[i - 1][j - 1];
            else f[i][j] = min({f[i - 1][j], f[i][j - 1], f[i - 1][j - 1]}) + 1;
        }
    }
    return f[n][m];
}