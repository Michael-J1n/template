#include <bits/stdc++.h>
using namespace std;

/**
 * 记忆化搜索模板
 * 注意：
 * 1. Base case 要正确处理
 * 2. 记得将结果存储在 memo 中
 */
void solve() {
    string s; cin >> s;
    int n = s.size();
    vector memo(n, vector<int>(n, -1));
    auto dfs = [&](this auto&& dfs, int l, int r) -> int {
    	if (l == r || l > r) return 0;
    	if (memo[l][r] != -1) return memo[l][r];
    	auto& res = memo[l][r];
    	if (s[l] == s[r]) res = dfs(l + 1, r - 1);
    	else res = 1 + min(dfs(l + 1, r), dfs(l, r - 1));
    	return res;
    };
    cout << dfs(0, n - 1) << endl;
}