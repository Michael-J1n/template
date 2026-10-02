#include <bits/stdc++.h>
using namespace std;

/** 最长公共子序列 LCS 问题 */

// O(nm) 求字符串 a,b 之间的 LCS；dp[i][j] 表示 a[0..i] 与 b[0..j] 之间的 LCS 长度
int longestCommonSubsequence(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    if (!n || !m) return 0;
    vector dp(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i] == b[j]) dp[i][j] = (i && j ? dp[i - 1][j - 1] : 0) + 1;
            else {
                if (i) dp[i][j] = max(dp[i][j], dp[i - 1][j]);
                if (j) dp[i][j] = max(dp[i][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n - 1][m - 1];
}

// 两数组的最长公共子序列
int longestCommonSubsequence(const vector<int>& a, const vector<int>& b) {
    int n = a.size(), m = b.size();
    if (!n || !m) return 0;
    vector dp(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (a[i] == b[j]) dp[i][j] = (i && j ? dp[i - 1][j - 1] : 0) + 1;
            else {
                if (i) dp[i][j] = max(dp[i][j], dp[i - 1][j]);
                if (j) dp[i][j] = max(dp[i][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n - 1][m - 1];
}


// 带路径恢复的 LCS
void solve() {
    string s, t; cin >> s >> t;
    int n = s.size(), m = t.size();
    vector dp(n + 1, vector<int>(m + 1));  // dp[i][j]=s的前i个字符和t的前j个字符的匹配长度
    for (int i = 1; i <= n; ++i) {
    	for (int j = 1; j <= m; ++j) {
    		if (s[i - 1] == t[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
    		else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
    	}
    }
    string res;
    int i = n, j = m;
    while (i && j) {
    	if (s[i - 1] == t[j - 1]) res += s[i - 1], --i, --j;
    	else if (dp[i - 1][j] > dp[i][j - 1]) --i;
    	else --j;
    }
    ranges::reverse(res);
    cout << res << endl;
}