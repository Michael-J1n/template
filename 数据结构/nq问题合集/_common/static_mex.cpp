#include <bits/stdc++.h>
using namespace std;

/*
StaticMex：静态区间 mex，0-based，闭区间 [l,r]。

mex(l,r) <= r-l+1 <= n，所以只关心值域 [0,n]。
root[i] 表示前缀 [0,i-1] 内每个值最后出现位置的持久化线段树。
查询 [l,r] 时，在 root[r+1] 中找最小 x，使 last[x] < l+1。

复杂度：build O(nlogn)，query O(logn)。
*/

struct StaticMex {
    using ll = long long;
    struct Node {
        int lc, rc, mn;
    };
    int n, tot;
    vector<int> root;
    vector<Node> tr;

    StaticMex() {}
    StaticMex(const vector<ll>& a) {
        init(a);
    }
    int clone(int u) {
        tr.push_back(tr[u]);
        return ++tot;
    }
    int update(int u, int l, int r, int p, int v) {
        int x = clone(u);
        if (l == r) {
            tr[x].mn = v;
            return x;
        }
        int mid = (l + r) >> 1;
        if (p <= mid) tr[x].lc = update(tr[u].lc, l, mid, p, v);
        else tr[x].rc = update(tr[u].rc, mid + 1, r, p, v);
        tr[x].mn = min(tr[tr[x].lc].mn, tr[tr[x].rc].mn);
        return x;
    }
    int first_less(int u, int l, int r, int v) const {
        while (l < r) {
            int mid = (l + r) >> 1;
            int lc = tr[u].lc;
            if (tr[lc].mn < v) {
                u = lc;
                r = mid;
            } else {
                u = tr[u].rc;
                l = mid + 1;
            }
        }
        return l;
    }
    void init(const vector<ll>& a) {
        n = a.size();
        tot = 0;
        root.assign(n + 1, 0);
        tr.clear();
        int lg = n <= 1 ? 1 : 32 - __builtin_clz(n);
        tr.reserve((long long)(n + 1) * (lg + 1) + 5);
        tr.push_back({0, 0, 0});
        root[0] = 0;
        for (int i = 0; i < n; ++i) {
            root[i + 1] = root[i];
            if (0 <= a[i] && a[i] <= n) root[i + 1] = update(root[i], 0, n, (int)a[i], i + 1);
        }
    }
    int mex(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        return first_less(root[r + 1], 0, n, l + 1);
    }
};
