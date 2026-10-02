#include <bits/stdc++.h>
using namespace std;

/**
 * 数位 DP 模板 1 -- 合法数字个数
 * 模板以恰好 target 个 0 的数字为例
 * 求 [low, high] 之间的符合要求的整数个数；可以看作一种节点是 0~9 值的树上 DP；从根到合法叶片的一条链对应一个合法整数
 * 
 * 参数：
 * low = 数字下限；
 * high = 数字上限；
 * 【!】target = 题目要求有关的状态数，可选；
 * lead_zero = 前导零是否会影响答案；即若补上前导零会改变状态判断，就传 true
 * 
 * 返回：
 * [low, high] 之间合法的整数个数
 * 
 * 复杂度：
 * O(kn)，其中 k 为特殊状态数，n 为十进制数位长度（即 lgV）
 */
long long digitDP(long long low, long long high, int target, bool lead_zero = true) {
    string low_s = to_string(low), high_s = to_string(high);
    int n = high_s.size();
    int diff_lh = n - low_s.size();                        // 数位对齐的偏移量
    
    vector memo(n, vector<long long>(target + 1, -1));     // 第一维度固定总长度 n；第二维度按照题目带来的额外状态（pre 或 cnt 等）设置
    /**
     * 固定前缀，枚举第 i 位，记忆化搜索有多少种合法数字
     * i：从高到低第几位
     * 【!】cnt0：当前数字中有效 0 的数量【这一位状态根据题目要求更改；可以是数字中的某种计数 cnt，也可以是数字中前一位填写的数字 pre 等】
     * limit_low：前面的数位是否都贴着下限
     * limit_high：前面的数位是否都贴着上限
     */
    auto dfs = [&](this auto&& dfs, int i, int cnt0, bool limit_low, bool limit_high) -> long long {
        if (cnt0 > target) {  // 前缀已经不满足条件，提前剪枝
            return 0;
        }
        if (i == n) {         // 填完所有位数，返回当前是否满足条件（没有额外条件返回 1）【这里表示搜到叶子的再下一层，即空后缀上】
            return cnt0 == target;
        }
        if (!limit_low && !limit_high && memo[i][cnt0] >= 0) return memo[i][cnt0];
        int lo = limit_low && i >= diff_lh ? low_s[i - diff_lh] - '0' : 0;
        int hi = limit_high ? high_s[i] - '0' : 9;
        
        /** ------------------------------------------------------------------------------------------- */
        long long res = 0;
        int d = lo;
        if (lead_zero && limit_low && i < diff_lh) {    // 若前导零影响答案，先跳过当前前导空位（当前 d 为 NONE）；再把当前位调整为 1
            res = dfs(i + 1, 0, true, false);
            d = 1;
        }
        for (; d <= hi; ++d) {
            // 枚举第 i 位填 d；根据题目要求在当前状态不合法的时候 continue
            res += dfs(i + 1, cnt0 + (d == 0), limit_low && d == lo, limit_high && d == hi);
        }
        /** ------------------------------------------------------------------------------------------- */

        if (!limit_low && !limit_high) memo[i][cnt0] = res;
        return res;
    }; 
    return dfs(0, 0, true, true);
}


/**
 * 例题：Windy 数【P13085】
 * 求 [lo,hi] 之间，不包含前导零且相邻的两个数位之间绝对差至少为 2 的正整数数量
 */
long long digitDP(long long low, long long high, bool lead_zero = true) {
    string low_s = to_string(low), high_s = to_string(high);
    int n = high_s.size(), diff_lh = n - low_s.size();
    vector memo(n, vector<long long>(12, -1));
    auto dfs = [&](this auto&& dfs, int i, int pre, bool limit_low, bool limit_high) -> long long {
        if (i == n) {
            return 1;
        }
        if (!limit_low && !limit_high && memo[i][pre] >= 0) return memo[i][pre];
        int lo = limit_low && i >= diff_lh ? low_s[i - diff_lh] - '0' : 0;
        int hi = limit_high ? high_s[i] - '0' : 9;
        long long res = 0;
        int d = lo;
        if (lead_zero && limit_low && i < diff_lh) {
            res = dfs(i + 1, 11, true, false);
            d = 1;
        }
        for (; d <= hi; ++d) {
            if (abs(d - pre) < 2) continue;
            res += dfs(i + 1, d, limit_low && d == lo, limit_high && d == hi);
        }
        if (!limit_low && !limit_high) memo[i][pre] = res;
        return res;
    };
    return dfs(0, 11, true, true);
}


/**
 * 其他应用：求排名
 * 假设数位 DP 实现了 count(x) = [0,x] 中合法整数个数；有：count(x) 单调不减，可以二分
 * 1. 找第 k 小合法整数 <==> 找最小 x 满足 count(x)>=k
 * 2. 给定合法数 x，求排名：count(x-1)+1
 * 
 * 例题：求第 n 个数位数字奇偶交替的数
 */
long long digitDP(long long low, long long high, bool lead_zero = true) {
    string low_s = to_string(low);
    string high_s = to_string(high);
    int n = high_s.size();
    int diff_lh = n - low_s.size();
    vector memo(n, vector<long long>(2, -1));
    auto dfs = [&](this auto&& dfs, int i, int pre, bool limit_low, bool limit_high) -> long long {
        if (i == n) {
            return 1;
        }
        if (!limit_low && !limit_high && memo[i][pre] >= 0) return memo[i][pre];
        int lo = limit_low && i >= diff_lh ? low_s[i - diff_lh] - '0' : 0;
        int hi = limit_high ? high_s[i] - '0' : 9;
        long long res = 0;
        int d = lo;
        if (lead_zero && limit_low && i < diff_lh) {
            res = dfs(i + 1, pre, true, false);
            d = 1;
        }
        for (; d <= hi; ++d) {
            if ((d & 1) == pre) continue;
            res += dfs(i + 1, d & 1, limit_low && d == lo, limit_high && d == hi);
        }
        if (!limit_low && !limit_high) memo[i][pre] = res;
        return res;
    };
    return dfs(0, -1, true, true);
}
/* -------------------------------------------------------- */
using ll = long long;
void solve() {
	ll n; cin >> n;
	// rank = n <==> smallest x s.t. cnt[..x]>=n
	auto check = [&](ll x) -> bool {
		return digitDP(10, x, 1) >= n;
	};
    ll l = 10, r = 4e18;
    while (l < r) {
    	ll mid = l + r >> 1;
    	if (check(mid)) r = mid;
    	else l = mid + 1;
    }
    cout << r << endl;
}
/* -------------------------------------------------------- */