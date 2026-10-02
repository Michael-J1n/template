#include <bits/stdc++.h>
using namespace std;

/**
 * Z-Function【求每个后缀与原串的 LCP】
 * 定义 z[i] = |LCP(s[i:], s)|
 *
 * 对于模式串在每个位置能匹配的最长长度：构造 pattern + '#' + text，求出 z-func。假设 pattern 大小为 m，那么结果数组的 z[m+1+i] 即表示 pattern 与 text[i..] 的 LCP 长度.
 * 注意 z[i] = x 代表长度为 [1,x] 的前缀都能被匹配到。x 是最大的可能长度。
 * 
 * 复杂度：O(n)
 */
vector<int> z_function(const string& s) {
    int n = s.size();
    vector<int> z(n); z[0] = n;
    // 不变量 i：当前计算的位置；[l, r]：当前右端点最远的 Z-box，满足 s[l..r] = s[0..r-l]
    for (int i = 1, l = 0, r = -1; i < n; ++i) {
        if (i <= r) {
            // s: 0 ------ j ======= r-l ...... l ------ i ======= r
            //    [------ prefix ------]        [------ Z-box -----]
            int j = i - l;
            z[i] = min(z[j], r - i + 1);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    return z;
}