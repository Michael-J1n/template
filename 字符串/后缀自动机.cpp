#include <bits/stdc++.h>
using namespace std;

/**
 * 后缀自动机 SAM：统计一个串中出现至少两次的不同子串个数
 * 复杂度：构建与统计 O(n)，空间 O(n)
 */

struct SAM {
    static const int SIG = 52;

    vector<array<int, SIG>> nxt;
    vector<int> link, slen, occ;
    int last;

    int id(char ch) {
        if ('a' <= ch && ch <= 'z') return ch - 'a';
        return 26 + (ch - 'A');
    }

    void init(int n) {
        nxt.clear(), link.clear(), slen.clear(), occ.clear();
        nxt.reserve(2 * n + 5), link.reserve(2 * n + 5), slen.reserve(2 * n + 5), occ.reserve(2 * n + 5);
        nxt.push_back({}), nxt[0].fill(-1);
        link.push_back(-1), slen.push_back(0), occ.push_back(0);
        last = 0;
    }

    void extend(char ch) {
        int c = id(ch), cur = nxt.size();
        nxt.push_back({}), nxt[cur].fill(-1);
        slen.push_back(slen[last] + 1), link.push_back(0), occ.push_back(1);

        int p = last;
        while (p != -1 && nxt[p][c] == -1) nxt[p][c] = cur, p = link[p];
        if (p == -1) link[cur] = 0;
        else {
            int q = nxt[p][c];
            if (slen[p] + 1 == slen[q]) link[cur] = q;
            else {
                int cq = (int)nxt.size();
                nxt.push_back(nxt[q]);
                slen.push_back(slen[p] + 1);
                link.push_back(link[q]);
                occ.push_back(0);

                while (p != -1 && nxt[p][c] == q) nxt[p][c] = cq, p = link[p];
                link[q] = link[cur] = cq;
            }
        }

        last = cur;
    }

    long long solve() {
        int sz = nxt.size();
        int mx = 0;
        for (int i = 0; i < sz; ++i) mx = max(mx, slen[i]);

        vector<int> cnt(mx + 1), ord(sz);
        for (int i = 0; i < sz; ++i) ++cnt[slen[i]];
        for (int i = 1; i <= mx; ++i) cnt[i] += cnt[i - 1];
        for (int i = sz - 1; i >= 0; --i) ord[--cnt[slen[i]]] = i;

        for (int i = sz - 1; i >= 1; --i) {
            int v = ord[i];
            int p = link[v];
            if (p >= 0) occ[p] += occ[v];
        }

        long long ans = 0;
        for (int v = 1; v < sz; ++v) {
            if (occ[v] >= 2) ans += (long long)slen[v] - slen[link[v]];
        }

        return ans;
    }
};