/**
 * 作用：构建后缀数组sa，回答“第k小后缀的起点”查询
 * 复杂度：O(nlogn)
 * 示例用法：
 * SuffixArray sa; sa.init(s);
 * cout << sa.sa[i] << endl;
 */

#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    string s;
    int n;
    // sa[i] 返回字典序第 i 小后缀在原字符串中的起始位置下标
    vector<int> sa, rk;

    void init(const string& _s) {
        s = _s, n = s.size();
        sa.resize(n), rk.resize(n);
        vector<int> tmp(n), cnt(max(256, n) + 1);

        for (int i = 0; i < n; ++i) rk[i] = (unsigned char)s[i], sa[i] = i;

        for (int k = 1;; k <<= 1) {
            auto key2 = [&](int i) { return i + k < n ? rk[i + k] + 1 : 0; };
            auto key1 = [&](int i) { return rk[i] + 1; };
            int m = max(256, n) + 1;

            fill(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; ++i) ++cnt[key2(i)];
            for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; --i) tmp[--cnt[key2(sa[i])]] = sa[i];
            sa.swap(tmp);

            fill(cnt.begin(), cnt.end(), 0);
            for (int i = 0; i < n; ++i) ++cnt[key1(i)];
            for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; --i) tmp[--cnt[key1(sa[i])]] = sa[i];
            sa.swap(tmp);

            int cls = 0;
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i) {
                int a = sa[i - 1], b = sa[i];
                if (rk[a] != rk[b] || key2(a) != key2(b)) ++cls;
                tmp[b] = cls;
            }
            rk.swap(tmp);
            if (cls == n - 1) break;
        }
    }
};