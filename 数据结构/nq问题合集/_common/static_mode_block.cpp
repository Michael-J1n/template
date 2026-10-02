#include <bits/stdc++.h>
using namespace std;

/*
StaticModeBlock：静态区间众数，0-based，闭区间 [l,r]。

经典分块：
1. 离散化值域。
2. best[i][j] 预处理完整块 [i,j] 的众数。
3. 查询时取中间完整块众数作为候选，再暴力两侧散块元素修正。

约定：次数相同取值最小的众数。

复杂度：
1. build：O(n * n/B + V * n/B)。
2. query：O(Blogn)。

B 可手动传入；默认 sqrt(n)。
*/

struct StaticModeBlock {
    using ll = long long;
    int n, B, m;
    vector<ll> vals;
    vector<int> a, bel, lb, rb;
    vector<vector<int>> pos;
    vector<int> best;
    vector<int> vis;
    int timer = 0;

    StaticModeBlock() {}
    StaticModeBlock(const vector<ll>& nums, int _B = 0) {
        init(nums, _B);
    }
    int isqrt(int x) const {
        int l = 0, r = min(x, 46340), ans = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (1LL * mid * mid <= x) ans = mid, l = mid + 1;
            else r = mid - 1;
        }
        return ans;
    }
    int& best_at(int l, int r) {
        return best[l * m + r];
    }
    int best_at(int l, int r) const {
        return best[l * m + r];
    }
    void init(const vector<ll>& nums, int _B = 0) {
        n = nums.size();
        vals = nums;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        a.resize(n);
        for (int i = 0; i < n; ++i) a[i] = lower_bound(vals.begin(), vals.end(), nums[i]) - vals.begin();
        int V = vals.size();
        pos.assign(V, {});
        for (int i = 0; i < n; ++i) pos[a[i]].emplace_back(i);

        B = _B ? _B : max(1, isqrt(max(1, n)) + 1);
        m = (n + B - 1) / B;
        bel.resize(n);
        lb.resize(m);
        rb.resize(m);
        for (int b = 0; b < m; ++b) {
            lb[b] = b * B;
            rb[b] = min(n, (b + 1) * B) - 1;
            for (int i = lb[b]; i <= rb[b]; ++i) bel[i] = b;
        }

        best.assign(m * m, -1);
        vector<int> cnt(V, 0);
        for (int l = 0; l < m; ++l) {
            fill(cnt.begin(), cnt.end(), 0);
            int cur = -1;
            for (int r = l; r < m; ++r) {
                for (int i = lb[r]; i <= rb[r]; ++i) {
                    int x = a[i], c = ++cnt[x];
                    if (cur == -1 || c > cnt[cur] || (c == cnt[cur] && vals[x] < vals[cur])) cur = x;
                }
                best_at(l, r) = cur;
            }
        }
        vis.assign(V, 0);
    }
    int count_id(int id, int l, int r) const {
        const auto& v = pos[id];
        return upper_bound(v.begin(), v.end(), r) - lower_bound(v.begin(), v.end(), l);
    }
    pair<ll, int> mode_pair(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return {0, 0};
        int bl = bel[l], br = bel[r];
        int ans = -1, ansCnt = 0;
        auto check = [&](int x) {
            if (vis[x] == timer) return;
            vis[x] = timer;
            int c = count_id(x, l, r);
            if (ans == -1 || c > ansCnt || (c == ansCnt && vals[x] < vals[ans])) ans = x, ansCnt = c;
        };
        if (++timer == INT_MAX) {
            fill(vis.begin(), vis.end(), 0);
            timer = 1;
        }
        if (bl + 1 <= br - 1) check(best_at(bl + 1, br - 1));
        if (bl == br) {
            for (int i = l; i <= r; ++i) check(a[i]);
        } else {
            for (int i = l; i <= rb[bl]; ++i) check(a[i]);
            for (int i = lb[br]; i <= r; ++i) check(a[i]);
        }
        return ans == -1 ? pair<ll, int>{0, 0} : pair<ll, int>{vals[ans], ansCnt};
    }
    ll mode(int l, int r) {
        return mode_pair(l, r).first;
    }
};
