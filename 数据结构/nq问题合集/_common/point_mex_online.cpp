#include <bits/stdc++.h>
using namespace std;

/*
PointMexOnline：单点修改 + 区间 mex，在线，0-based，闭区间 [l,r]。

mex(l,r) <= n，所以只维护值域 [0,n] 的出现位置集合。

复杂度：
1. set/add：O(logn)。
2. mex：O(mex * logn)，最坏 O(nlogn)。

若允许离线，优先使用 point_mex_mo.cpp。
*/

struct PointMexOnline {
    using ll = long long;
    int n;
    vector<ll> a;
    vector<set<int>> pos;

    PointMexOnline() {}
    PointMexOnline(const vector<ll>& nums) {
        init(nums);
    }
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        pos.assign(n + 1, {});
        for (int i = 0; i < n; ++i) {
            if (0 <= a[i] && a[i] <= n) pos[a[i]].insert(i);
        }
    }
    void set(int p, ll v) {
        if (p < 0 || p >= n) return;
        if (a[p] == v) return;
        if (0 <= a[p] && a[p] <= n) pos[a[p]].erase(p);
        a[p] = v;
        if (0 <= a[p] && a[p] <= n) pos[a[p]].insert(p);
    }
    void add(int p, ll v) {
        if (p < 0 || p >= n || !v) return;
        set(p, a[p] + v);
    }
    int mex(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        for (int x = 0; x <= n; ++x) {
            auto it = pos[x].lower_bound(l);
            if (it == pos[x].end() || *it > r) return x;
        }
        return n + 1;
    }
};
