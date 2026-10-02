#include <bits/stdc++.h>
using namespace std;

/**
 * 频率表 + 频率的频率表
 *
 * 状态定义：
 * cnt[x] 表示 x 当前出现次数
 * ccnt[f] 表示有多少种值出现次数恰好为 f
 * mx 表示最大正频率
 * mn 表示最小正频率
 * diff 表示有多少个正频率 f 满足 ccnt[f] > 0
 * tot 表示当前元素总数
 * kind 表示当前不同值个数
 *
 * 复杂度：
 * add 均摊 O(1)
 * count / count_freq / min_freq / max_freq / total / kinds / distinct_freqs 均为 O(1)
 *
 * 注意：
 * n 传当前集合最大可能大小
 * 任一值的实际频率不得超过 n
 * 不支持 del
 */
struct FreqTable {
    unordered_map<int,int> cnt;
    vector<int> ccnt;
    int mx, mn, diff, tot, kind;
    FreqTable(int n) {
        cnt.max_load_factor(0.7);
        cnt.reserve(n * 2 + 1);
        ccnt.resize(n + 2);
        mx = mn = diff = tot = kind = 0;
    }
    void add(int x) {
        int& cur = cnt[x];
        int old = cur;
        if (old) {
            if (--ccnt[old] == 0) --diff;
        } else {
            ++kind;
        }
        ++cur;
        if (++ccnt[cur] == 1) ++diff;
        ++tot;
        if (cur > mx) mx = cur;
        if (old == 0) mn = 1;
        else if (old == mn && ccnt[old] == 0) mn = cur;
    }
    int count(int x) const {
        auto it = cnt.find(x);
        return it == cnt.end() ? 0 : it->second;
    }
    int count_freq(int f) const { return ccnt[f]; }
    int min_freq() const { return mn; }
    int max_freq() const { return mx; }
    int total() const { return tot; }
    int kinds() const { return kind; }
    int distinct_freqs() const { return diff; }
};
