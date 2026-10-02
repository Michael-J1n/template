#include <bits/stdc++.h>
using namespace std;

/**
 * 数位 DP 模板 2 -- 合法数字数位之和
 * 
 * 求 [low, high] 之间的符合要求的所有整数的价值之和（例如 f(x)=x 代表满足约束的元素和，f(x)=digitsum(x) 代表满足约束的元素的数位和）
 * 模板中以不同数字不超过 k 个的数位之和为例
 * 数位 DP 可以看作一种树形 DP，在计算价值之和时使用树形 DP 相应的转移式思考技巧即可
 * 
 * 参数：
 * low = 数字下限；
 * high = 数字上限；
 * 【!】k = 题目要求有关的状态数，可选；
 * lead_zero = 前导零是否会影响答案；即若补上前导零会改变状态判断，就传 true
 * 
 * 返回：
 * [low, high] 之间满足约束的整数的数位价值和
 * 
 * 复杂度：
 * O(kn)，其中其中 k 为特殊状态数，n 为十进制数位长度（即 lgV）
 */
long long digitDPContribution(long long low, long long high, int k, bool lead_zero = true) {
    string low_s = to_string(low), high_s = to_string(high);
    int n = high_s.size();
    int diff_lh = n - low_s.size();                                         // 数位对齐的偏移量
    
    vector memo(n, vector<pair<long long, long long>>(1 << 10, {-1, -1}));  // 第一维度固定总长度 n；第二维度按照题目带来的额外状态（pre 或 cnt 等）设置
    /**
     * 固定前缀，枚举第 i 位，记忆化搜索 {子树中的合法数个数，子树中的合法数价值和}【子树根为 i，此时除去自己，到叶子还有 n - 1 - i 的深度距离】
     * i：从高到低第几位
     * 【!】mask：当前填过数字的比特集合【这一位状态根据题目要求更改；可以是数字中的某种计数 cnt，也可以是数字中前一位填写的数字 pre 等】
     * limit_low：前面的数位是否都贴着下限
     * limit_high：前面的数位是否都贴着上限
     */
    auto dfs = [&](this auto&& dfs, int i, int mask, bool limit_low, bool limit_high) -> pair<long long, long long> {
        if (i == n) {         // 填完所有位数，返回当前是否满足条件（没有额外条件返回 {1,0}）【这里表示搜到叶子的再下一层，即空后缀上；是一种合法方案，但数位空无贡献】
            return {1, 0};
        }
        if (!limit_low && !limit_high && memo[i][mask].first >= 0) return memo[i][mask];
        int lo = limit_low && i >= diff_lh ? low_s[i - diff_lh] - '0' : 0;
        int hi = limit_high ? high_s[i] - '0' : 9;

        /** ------------------------------------------------------------------------------------------- */
        long long cnt = 0, sum = 0;
        int d = lo;
        if (lead_zero && limit_low && i < diff_lh) {    // 若前导零影响答案，先跳过当前前导空位（不考虑第 i 位产生的贡献，即当前 d = NONE）；再把当前位调整为 1
            auto [sub_cnt, sub_sum] = dfs(i + 1, 0, true, false);
            // 当前位无效，只有后缀中的数字才有机会贡献
            cnt += sub_cnt;
            sum += sub_sum;
            // cnt %= mod; sum %= mod;
            d = 1;
        }
        for (; d <= hi; ++d) {
            // 枚举第 i 位填 d；根据题目要求在当前状态不合法的时候 continue
            int nmask = mask | 1 << d;
            if (popcount((uint32_t)nmask) > k) continue;
            // sum 根据具体价值函数类似树形 DP 的更新；这里是数位和，若是数字和，改成 sum += sub_sum + d * pw[n - i - 1] * sub_cnt，其中 pw[t] = 10^t%mod 是预处理的幂
            // 对于 sum，即价值总和的更新：首先后缀中的数位对于总价值的贡献和即为 Σsub_sum；再考虑当前位的贡献：若合法，当前 d 的价值会贡献 sub_cnt 次（可以任选一个接上）
            auto [sub_cnt, sub_sum] = dfs(i + 1, nmask, limit_low && d == lo, limit_high && d == hi);
            cnt += sub_cnt;
            sum += sub_sum + sub_cnt * d;
            // cnt %= mod; sum %= mod;
        }
        /** ------------------------------------------------------------------------------------------- */

        pair<long long, long long> res = {cnt, sum};
        if (!limit_low && !limit_high) memo[i][mask] = res;
        return res;
    };
    return dfs(0, 0, true, true).second;
}