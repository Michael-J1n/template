#include <bits/stdc++.h>
using namespace std;

/**
 * Next 自动机【子序列匹配】
 * 返回 nxt 数组，其中 nxt[i][c] 表示 s 中下标 >=i 的位置里，字符 c 第一次出现的位置；不存在则为 n
 * 使用时 pos 表示下一个字符允许开始匹配的位置；匹配字符 c 后令 pos=nxt[pos][c-'a']+1
 * 
 * 适用：固定字符串 s，频繁查询当前位置之后某字符第一次出现的位置；多次进行子序列匹配
 * 
 * 复杂度：预处理 O(26n)，单次跳转 O(1)
 */
vector<vector<int>> nextAutomaton(const string& s) {
    int n = s.size();
    vector nxt(n + 1, vector<int>(26, n));
    for (int i = n - 1; ~i; --i) {
        nxt[i] = nxt[i + 1];
        nxt[i][s[i] - 'a'] = i;
    }
    return nxt;
}

/* 例如找 T 在 S 中的第一次子序列出现 */
vector<int> findSubseq(const string& s, const string& t) {
    auto nxt = nextAutomaton(s);
    int n = s.size(), m = t.size(), id = 0;
    vector<int> res;
    for (int i = 0; i < m; ++i) {
        id = nxt[id][t[i] - 'a'];
        if (id == n) return {};
        res.emplace_back(id);
        if (i + 1 < m) ++id;
    }
    return res;
}