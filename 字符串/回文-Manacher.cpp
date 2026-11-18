#include <bits/stdc++.h>
using namespace std;

/**
 * Manacher【线性预处理所有回文半径】
 * d1[i]：以 i 为中心的奇数回文半径，包含中心本身；d2[i]：以 i - 1 和 i 之间为中心的偶数回文半径。下标 0-based，区间使用闭区间 [l, r]。
 *
 * 重要性质：
 * 以   i   为中心的奇回文：[i-d1[i]+1, i+d1[i]-1]，长度 d1[i]*2-1，共有 d1[i] 个
 * 以 i-1|i 为中心的偶回文：[i-d2[i]  , i+d2[i]-1]，长度 d2[i]*2  ，共有 d2[i] 个
 * 从 i 开始的最长回文：[i, rp[i]]，长度 rp[i]-i+1
 * 以 i 结尾的最长回文：[lp[i], i]，长度 i-lp[i]+1
 *
 * 支持操作：
 * Manacher(s)：预处理字符串 s 的 d1, d2 信息。
 * Manacher(a)：预处理数组 a 的 d1, d2 信息。
 * check(l, r)：O(1) 判断区间 [l, r] 是否为回文。
 * get(lp, rp)：lp[i] = 以 i 为右端点的最长回文的最左端点；rp[i] = 以 i 为左端点的最长回文的最右端点。
 * count(st, ed)：st[i] = 以 i 为开头的回文串总数；ed[i] = 以 i 为结尾的回文串总数。
 *
 * 适用：静态序列回文判断、最长回文、回文半径统计。
 *
 * 复杂度：预处理 O(n)，单次查询 O(1)，空间 O(n)。
 */
struct Manacher {
    int n;
    string s;
    vector<int> d1, d2;
    Manacher(string s) : s(s), n(s.size()), d1(n), d2(n) {
        // d1[i]：以 i 为中心的奇回文半径，包含 i；[l, r] 为当前最靠右的奇回文
        // l -- (----- l+r-1 -----) || (----- i -----) -- r
        //       <-k->       <-k->      <-k->   <-k->
        for (int i = 0, l = 0, r = -1; i < n; ++i) {
            // 镜像位置 l + r - i；进入 while 前 [i - k + 1, i + k - 1] 已知为回文
            int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
            d1[i] = k--;
            if (i + k > r) l = i - k, r = i + k;
        }
        // d2[i]：以 i - 1 | i 为中心的偶回文半径；[l, r] 为当前最靠右的偶回文
        for (int i = 0, l = 0, r = -1; i < n; ++i) {
            // 镜像中心 l + r - i + 1；进入 while 前 [i - k, i + k - 1] 已知为回文
            int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
            d2[i] = k--;
            if (i + k > r) l = i - k - 1, r = i + k;
        }
    }
    Manacher(const vector<int>& a) : n(a.size()), d1(n), d2(n) {
        for (int i = 0, l = 0, r = -1; i < n; ++i) {
            int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n && a[i - k] == a[i + k]) k++;
            d1[i] = k--;
            if (i + k > r) l = i - k, r = i + k;
        }
        for (int i = 0, l = 0, r = -1; i < n; ++i) {
            int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n && a[i - k - 1] == a[i + k]) k++;
            d2[i] = k--;
            if (i + k > r) l = i - k - 1, r = i + k;
        }
    }
    bool check(int l, int r) {
        if (l > r) return false;
        int len = r - l + 1;
        if (len & 1) return d1[l + r >> 1] >= len / 2 + 1;
        return d2[l + r + 1 >> 1] >= len / 2;
    }
    void get(vector<int>& lp, vector<int>& rp) {  // 回文中心单调指针维护最大边界
        lp.resize(n), rp.resize(n);
        int p1 = 0, p2 = 0;
        for (int i = 0; i < n; ++i) {
            // 处理奇中心
            while (p1 <= i && p1 + d1[p1] - 1 < i) ++p1;
            lp[i] = 2 * p1 - i;  // [p1-(i-p1)..p1-1][p1][p1+1..i]
            // 处理偶中心
            while (p2 <= i && p2 + d2[p2] - 1 < i) ++p2;
            if (p2 <= i) lp[i] = min(lp[i], 2 * p2 - i - 1);
        }
        p1 = p2 = n - 1;
        for (int i = n - 1; i >= 0; --i) {
            // 处理奇中心
            while (p1 >= i && p1 - d1[p1] + 1 > i) --p1;
            rp[i] = 2 * p1 - i;
            // 处理偶中心
            while (p2 >= i && p2 - d2[p2] > i) --p2;
            if (p2 >= i) rp[i] = max(rp[i], 2 * p2 - i - 1);
        }
    }
    void count(vector<int>& st, vector<int>& ed) {  // 差分数组维护区间加
        st.assign(n + 1, 0), ed.assign(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            // 处理奇中心
            st[i - d1[i] + 1]++, st[i + 1]--;
            ed[i]++, ed[i + d1[i]]--;
            // 处理偶中心
            if (d2[i]) {
                st[i - d2[i]]++, st[i]--;
                ed[i]++, ed[i + d2[i]]--;
            }
        }
        for (int i = 1; i <= n; ++i) st[i] += st[i - 1], ed[i] += ed[i - 1];
        st.pop_back(), ed.pop_back();
    }
};