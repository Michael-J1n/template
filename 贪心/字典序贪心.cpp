#include <bits/stdc++.h>
using namespace std;

/**
 * 给定长度相等的字符串 s 以及目标串 target，求将 s 打乱后字典序大于 target 的字典序最小字符串（等价的，给定一个字符多重集，构造一个字符串使得字典序最小且大于 target）
 * 
 * 一种朴素的贪心是：从前向后枚举，对于当前位置，从小到大枚举每个字符是否能填（能填当且仅当填上后，后面按照最优排列（从大到小）可以比 target 的字典序大）；如果可行，就填入这个位置并继续向后按位确认
 * 枚举 n 个位置，26 种字符，一次检查需要构造比较大小为 n 的字符串，因此总复杂度为 O(n^2)
 */
string lexGraterPermutation1(string s, string target) {
    int n = s.size();
    vector<int> cnt(26, 0);
    for (auto& c : s) ++cnt[c - 'a'];
    string res = "";
    // 检查当前位置填写 x，剩下最优排序是否满足要求
    auto check = [&](int x) -> bool {
        string tmp = res;
        tmp.push_back('a' + x);
        for (int i = 25; ~i; --i) {
            if (!cnt[i]) continue;
            int t = cnt[i];
            while (t--) tmp.push_back('a' + i);
        }
        return tmp > target;
    };
    // 从前到后从小到大检查保证当前若可以填入总是最小可行解
    for (int i = 0; i < n; ++i) {
        bool ok = 0;
        for (int j = 0; j < 26; ++j) {
            if (!cnt[j]) continue;
            --cnt[j];
            if (check(j)) {
                res.push_back('a' + j);
                ok = 1;
                break;
            }
            ++cnt[j];
        }
        if (!ok) return "";
    }
    return res;
}

/**
 * 进一步观察，需要重排后的 s 字典序最小但大于 target，一定形如这个状态：前缀与 target 的前缀相等，后面一个位置是大于 target 对应位置的最小字符，尾缀是剩余字符的升序排列。
 * 因此为了最后字典序最小，另一种贪心是：尽量让相等前缀变长；
 * 策略即为能相等就匹配直到不行；这一位置上选择比 target 对应位置大的最小字符；然而会出现一种情况：当前位置没有任何更大的字符可用了，需要从后向前回溯，找到第一个存在大于 target 对应字符的位置
 */
string lexGreaterPermutation(string s, string target) {
    int n = s.size();
    vector<int> cnt(26);
    for (auto& c : s) ++cnt[c - 'a'];
    string res = "";
    // 前面匹配成功，尾缀升序排列
    auto build = [&]() -> string {
        for (int i = 0; i < 26; ++i) {
            if (!cnt[i]) continue;
            int t = cnt[i];
            while (t--) res.push_back('a' + i);
        }
        return res;
    };
    // 匹配最长相等前缀
    int p = 0;
    while (p < n && cnt[target[p] - 'a']) {
        --cnt[target[p] - 'a'];
        res.push_back(target[p]);
        ++p;
    }
    // 寻找比当前位置大的最小字符填入
    while (1) {
        if (p < n) {
            int x = target[p] - 'a';
            for (int j = x + 1; j < 26; ++j) {
                if (cnt[j]) {
                    --cnt[j];
                    res.push_back('a' + j);
                    return build();
                }
            }
        }
        if (res.empty()) return "";
        ++cnt[res.back() - 'a'];
        res.pop_back();
        --p;
    }
    return res;
}