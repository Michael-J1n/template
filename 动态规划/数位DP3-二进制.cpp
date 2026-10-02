#include <bits/stdc++.h>
using namespace std;

/**
 * 二进制数位 DP 模板 1 -- 合法数字个数
 * 模板以二进制表示中恰好 target 个 0 的数字为例
 * 求 [low, high] 之间符合要求的非负整数个数
 *
 * 参数：
 * low = 数字下限；
 * high = 数字上限；
 * 【!】target = 题目要求有关的状态数，可选；
 * lead_zero = 前导零是否会影响答案；即若在短数字前补上 0 会改变状态判断，就传 true
 *
 * 返回：
 * [low, high] 之间合法的非负整数个数
 *
 * 复杂度：
 * O(kn)，其中 k 为特殊状态数，n 为二进制数位长度（即 logV）
 */
string change_base(long long x) {
    if (x == 0) return "0";
    string s;
    while (x) {
        s += char('0' + (x & 1));
        x >>= 1;
    }
    reverse(s.begin(), s.end());
    return s;
}
long long digitDP(long long low, long long high, int target, bool lead_zero = true) {
    string low_s = change_base(low), high_s = change_base(high);
    int n = high_s.size();
    int diff_lh = n - low_s.size();                        // 二进制数位对齐的偏移量
    vector memo(n, vector<long long>(target + 1, -1));     // 第一维固定总长度 n；第二维按照题目带来的额外状态设置
    /**
     * 固定前缀，枚举第 i 个二进制位，记忆化搜索合法数字数量
     *
     * i：从高到低枚举到第几个二进制位
     * 【!】cnt0：当前有效二进制位中 0 的数量；这一维状态根据题目要求更改，可以是某个计数 cnt、前一位 pre、余数 rem、状态 mask 等
     *
     * limit_low：前面的有效数位是否都贴着下限
     * limit_high：前面的有效数位是否都贴着上限
     */
    auto dfs = [&](this auto&& dfs, int i, int cnt0, bool limit_low, bool limit_high) -> long long {
        if (cnt0 > target) {  // 当前前缀已经不可能满足条件，提前剪枝
            return 0;
        }
        if (i == n) {         // 所有二进制位填写完毕，当前位于叶子的下一层，即空后缀
            return cnt0 == target;
        }
        if (!limit_low && !limit_high && memo[i][cnt0] >= 0) return memo[i][cnt0];
        int lo = limit_low && i >= diff_lh ? low_s[i - diff_lh] - '0' : 0;
        int hi = limit_high ? high_s[i] - '0' : 1;

        /** ------------------------------------------------------------------------------------------- */
        long long res = 0;
        int d = lo;
        if (lead_zero && limit_low && i < diff_lh) {
            // 要么当前位仍然不开始填写有效数字，要么从当前位开始填写有效数字
            res = dfs(i + 1, 0, true, false);
            d = 1;
        }
        for (; d <= hi; ++d) {
            // 枚举第 i 个二进制位填 d；根据题目要求在当前状态不合法时 continue
            res += dfs(i + 1, cnt0 + (d == 0), limit_low && d == lo, limit_high && d == hi);
        }
        /** ------------------------------------------------------------------------------------------- */
        
        if (!limit_low && !limit_high) memo[i][cnt0] = res;
        return res;
    };
    return dfs(0, 0, true, true);
}