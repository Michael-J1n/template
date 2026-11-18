#include <bits/stdc++.h>
using namespace std;

template<class T>
struct BIT {
    int n;
    vector<long long> c;  // c[i] = sum of nums[i-lowbit(i)...i-1]
    BIT(int _n) : n(_n) {
        c.resize(n + 1);
    }
    BIT(vector<T>& a) : n(a.size()) {
        c.resize(n + 1);
        for (int i = 1; i <= n; ++i) c[i] = a[i - 1];
        for (int i = 1; i <= n; ++i) {
            int j = i + (i & -i);
            if (j <= n) c[j] += c[i];
        }
    }
    void add(int id, T val) {
        ++id;
        for (int i = id; i <= n; i += i & -i) c[i] += val;
    }
    void set(int id, T val) { add(id, val - get(id)); }
    long long pref_sum(int id) {
        ++id;
        long long sum = 0;
        for (int i = id; i >= 1; i &= i - 1) sum += c[i];
        return sum;
    }
    long long range_sum(int l, int r) {
        if (l > r) return 0;
        return pref_sum(r) - (l ? pref_sum(l - 1) : 0);
    }
    long long get(int id) { return range_sum(id, id); }
    int kth(long long k) {
        int pos = 0;
        for (int i = __lg(n); ~i; --i) {
            int nxt = pos + (1 << i);
            if (nxt <= n && c[nxt] < k) pos = nxt, k -= c[nxt];
        }
        // return pos - 1;  // 最大下标使得前缀和 < k
        return pos;         // 最小下标使得前缀和 >= k
    }
};

/**
 * 给定 n 个二维点 points，以及 q 个询问 [a,b,c,d] qs，求在 [a,c] x [b,d] 范围内（含边界）的点有多少个
 * 
 * 本质上是二维偏序计数：计算有多少对 (x,y) 满足 a<=x<=c && b<=y<=d
 * 类似差分扫描线思想的转化：count(x1<=x<=x2, y1<=y<=y2) = count(x<=x2, y1<=y<=y2) - count(x<x1, y1<=y<=y2)
 * 将 x 维度建立为按顺序遍历的“时间维度”，y 维度需要查询&&修改当前时刻满足在值域范围内的点数，使用树状数组维护即可
 * 
 * 复杂度：O((n + q) log ∑y)
 * 前置条件：每个询问矩形满足 x1<=x2、y1<=y2
 */
struct Query {
    int x1, y1, x2, y2;
};
vector<long long> count2D(vector<pair<int, int>> points, const vector<Query>& qs) {
    struct Event {
        long long x;
        int y1, y2;
        int type, id;
    };
    int n = points.size(), q = qs.size();
    vector<int> ys;
    ys.reserve(n);
    for (auto& [x, y] : points) ys.emplace_back(y);
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    vector<Event> evs;
    evs.reserve(q << 1);
    for (int i = 0; i < q; ++i) {
        auto& [x1, y1, x2, y2] = qs[i];
        evs.emplace_back(x2, y1, y2, 1, i);
        evs.emplace_back(1LL * x1 - 1, y1, y2, -1, i);
    }
    sort(points.begin(), points.end());
    sort(evs.begin(), evs.end(), [&](auto& a, auto& b) {
        return a.x < b.x;
    });
    BIT<int> tr(ys.size());
    vector<long long> res(q);
    int j = 0;
    for (auto& [x, y1, y2, type, id] : evs) {
        while (j < n && points[j].first <= x) {
            int y = lower_bound(ys.begin(), ys.end(), points[j].second) - ys.begin();
            tr.add(y, 1);
            ++j;
        }
        int l = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
        int r = upper_bound(ys.begin(), ys.end(), y2) - ys.begin() - 1;
        if (l <= r) res[id] += type * tr.range_sum(l, r);
    }
    return res;
}
