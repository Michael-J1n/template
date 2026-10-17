#include <bits/stdc++.h>
using namespace std;

/**
 * KMP【字符串模式匹配与前缀函数】
 * pi[i] 表示 p[0..i] 的最长真前缀且同时为后缀的长度，即 p[0..pi[i]-1] = p[i-pi[i]+1..i]。
 * 注意若当前长度为 len，跳 fail 链需要调用的是 pi[len - 1]。
 * 
 * 支持操作：
 * KMP(p)：O(m) 预处理模式串 p 的前缀函数。
 * find(s)：返回 p 在 s 中第一次出现的起点，无解返回 -1。
 * find_all(s)：返回 p 在 s 中所有出现位置，允许重叠。
 * count(s)：统计 p 在 s 中出现次数，允许重叠。
 * borders()：返回 p 的所有非空 border 长度，从短到长排列，例如 "ababab" 返回 {2,4,6}。
 * period(): 返回 p 的最短完整循环节长度；空串返回 0（一般周期见文档末尾）。
 * 空模式串在文本的每个间隙匹配：find 返回 0，find_all 返回 0..n，count 返回 n+1。
 * 下标与计数使用 int，要求模式串和文本长度小于 INT_MAX。
 * fail_tree(father, g): 在 s 上建立对应的失配树；father 数组保存父亲关系，g 存树；
 *                       fail 树上的节点 len，表示原串前缀 s[0..len-1]（注意是前缀长度，而不是结尾下标）
 *                       父亲定义为 father[len] = pi[len - 1]，即节点 len 的父亲 = 这个前缀的最长真 border，节点 len 的祖先链 = 长度从大到小的所有 border。
 * 
 * 适用：单模式串匹配、多个文本查询、border / 周期相关问题。
 * 复杂度：预处理 O(m)；单次匹配 O(n)；空间 O(m)。
 */
struct KMP {
    int m;
    string p;
    vector<int> pi;
    KMP(const string& p) : m(p.size()), p(p), pi(m) {
        // 预处理 pi，pi[i] = j 表示 p[0..j-1] = p[i-j+1..i]
        for (int i = 1, j = 0; i < m; ++i) {
            while (j && p[i] != p[j]) j = pi[j - 1];
            if (p[i] == p[j]) ++j;
            pi[i] = j;
        }
    }
    int find(const string& s) {
        if (m == 0) return 0;
        for (int i = 0, j = 0; i < s.size(); ++i) {
            while (j && s[i] != p[j]) j = pi[j - 1];
            if (s[i] == p[j]) ++j;
            if (j == m) return i - m + 1;
        }
        return -1;
    }
    vector<int> find_all(const string& s) {
        vector<int> res;
        if (m == 0) {
            res.resize(s.size() + 1);
            iota(res.begin(), res.end(), 0);
            return res;
        }
        // 不变量：j 为已经匹配上的 pattern 的前缀长度，第 j 个下标位置为待比较位置；
        // 即当前已知 p[0..j-1] = s[i-j..i-1]，下一步尝试匹配 p[j] 与 s[i]。
        // fail 链：前缀匹配失败后，下一步尝试更短的前缀：j = pi[j - 1]；最终跳到 0
        for (int i = 0, j = 0; i < s.size(); ++i) {
            while (j && s[i] != p[j]) j = pi[j - 1];
            if (s[i] == p[j]) ++j;
            // 此时前缀 s[0..i] 的后缀最多能匹配模式串的 p[0..j-1]
            if (j == m) res.emplace_back(i - m + 1), j = pi[j - 1];
        }
        return res;
    }
    int count(const string& s) {
        if (m == 0) return s.size() + 1;
        int res = 0;
        for (int i = 0, j = 0; i < s.size(); ++i) {
            while (j && s[i] != p[j]) j = pi[j - 1];
            if (s[i] == p[j]) ++j;
            if (j == m) ++res, j = pi[j - 1];
        }
        return res;
    }
    vector<int> borders() {
        vector<int> res;
        // 沿着 fail 链跳，k 是 border 长度，此时 m - k 为对应周期长度
        for (int k = m; k; k = pi[k - 1]) res.emplace_back(k);
        reverse(res.begin(), res.end());
        return res;
    }
    int period() {
        if (m == 0) return 0;
        int k = m - pi[m - 1];
        return m % k == 0 ? k : m;
    }
    void fail_tree(vector<int>& father, vector<vector<int>>& g) {
        father.assign(m + 1, 0), g.assign(m + 1, {});
        for (int len = 1; len <= m; ++len) {
            father[len] = pi[len - 1];
            g[father[len]].emplace_back(len);
        }
    }
};

/**
 * KMP 与字符串周期关系：
 * 对于长度为 n 的字符串，p 是一个周期 <=> 长度 n - p 是一个 border。
 * 这里周期意为对于所有 i>=p，都有 s[i]==s[i-p]；border 为即是前缀又是后缀的非空真子串。
 * 
 * 1. 最短周期：n - pi[n-1]（注意这里的周期不要求最后一块完整）
 * 最长 border 长为 k=pi[n-1]，满足 s[0..k-1]=s[n-k..n-1]，因此令 p=n-k 则有 s[0..k-1]=s[p..n-1]，即 s[i-p]=s[i] 逐项对应，为周期。又这里是最长 border，因此是最短周期。
 * 对于前缀s[0..i]的最短周期同理为 (i+1)-pi[i].
 * 
 * 2. 判断是否能由更短串完整重复得到：
 * 周期 p = n-pi[n-1]，同时还要求 n%p==0。如果满足，那么可以得到，s[0..p-1] 是最短完整循环节。
 * 
 * 3. 枚举所有可能的周期：
 * 由于 border 长度 b <=> 周期长度 p=n-b，因此枚举所有 border 就可以得到所有可能的周期。
 */

/**
 * KMP 的 fail 链上 DP：对于每个前缀长度 len，答案只依赖它的 fail 父亲。
 * 通用框架：
 * vector<int> dp(n + 1);
 * for (int len = 1; len <= n; ++len) {
 *     int p = pi[len - 1];
 *     // dp[len] <- (p, dp[p])
 * }
 * 例：给定字符串 s，求 s 的所有前缀的最长周期长度之和。
 * 由于上文提到周期性质，一个字符串的所有周期长度即为其总长减去 border 长度。
 * 需要周期最长，那么即需要 border 最短。固定前缀，沿着其 fail 链跳跃，跳到最后一个不为 0 的长度即为最短 border。
 * 然而枚举前缀 + 跳 fail 链是最坏 O(n^2) 的，然而注意到在从小到大计算时，长度更小的前缀的最短非空 border 长度已经算出来了，因此就可以按照 fail 链 dp 来加速计算到 O(n)。
 * 
 * 有时 fail 链上 dp 也要使用类似  的思想，每一个前缀都要保留最大合法 border 长度 j 的信息，下一轮从它继续（这样 j 不每次重置跳跃，计算量摊还为 O(n)）
 * 例：给定字符串 s，求 s 所有前缀的不重叠 border 个数之和【P2375】。
 * 首先预处理 cnt[len] 为长度 len 的前缀 s[0..len-1] 的非空 border 个数（包括自己），cnt[0]=0，cnt[len]=1+cnt[pi[len-1]]
 * 朴素做法是对于每个前缀，跳 fail 链直到满足不重叠，再累加 cnt。但这样会退化成 n^2.
 * 但考虑一个性质：对于当前前缀上的最大合法不重叠 border，去掉最后一位的这个匹配后，一定会化归到前一个前缀上的合法不重叠 border（但不一定是最大的--考虑恰好不重叠但是完全覆盖的场景）
 * 因此每个前缀都可以维护一个 j 为当前可以合法不重叠匹配上的最大 border 长度，遍历到下一个前缀后先用类似  的方案求出匹配，如果此时不满足不重叠，继续跳 fail 链直到满足条件即可。
 */

/**
 * KMP 的 fail 链树：节点代表前缀信息；
 * 父亲代表沿着 fail 链跳到最长真border；根节点为 0 代表空前缀
 * 例如 s="ababa" 的树：
         0 ""
       /   \
   1 "a"   2 "ab"
     |        |
  3 "aba"  4 "abab"
     |
 5 "ababa"
 * 
 * 在失配树上可以更直观的求出许多信息，尤其是关于自己 border 链的统计。
 * 例：给定字符串 s，多次询问 p,q，表示需要求出前缀 s[0..p-1] 与 s[0..q-1] 的最长公共真 border 的长度。
 * 朴素的想法是从 p 和 q 往前沿着 fail 链跳，求出第一个相等的长度。然而这样是不高效的，考虑所有真 border 恰好是当前点到根路径上的所有节点；
 * 因此两个前缀的公共 border 即为在祖先链上相同的节点，而最长公共真 border 只需要求出 LCA(fa[p],fa[q]) 即可（注意这里要先求 fa，由于自己不能当作真 border）。
 * 
 * 例：给定字符串 s，求它的每个 border 作为子串在 s 中的出现个数【CF432D】。
 * 考虑前缀版本的问题，即，给定 s[0..i]，求该前缀的 border 在该前缀中的出现次数（在  相关题目中中寻找前缀意义上的重叠自问题很重要）。
 * 假设 s[0..i-1] 的问题已经处理好，考虑加入 s[i]。加入后假设真 border 为 s[0..p-1]，那么这个前缀，以及这个 border 的所有 border，出现次数都会 +1.
 * 因此这个操作即是在树上祖先链整体+1，用树上点权差分即可. 最后答案从节点 n 开始不断跳父亲直到根节点，输出点权即可。
 */