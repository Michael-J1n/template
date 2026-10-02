#include <bits/stdc++.h>
using namespace std;

/**
 * 线段树扫描线：矩形面积并
 *
 * rect = {x1, y1, x2, y2}  // 左下角，右上角
 * 注意由于是连续段长，线段树下标 i 表示离散化后的 y 小段 [ys[i], ys[i + 1]]
 * 前置条件：每个矩形满足 x1<=x2、y1<=y2
 */
struct Event {
    int x, y1, y2, v;
};
struct ScanSegTree {
    int n;
    vector<int> cnt;
    vector<long long> len;
    vector<int>& ys;
    ScanSegTree(int _n, vector<int>& _ys) : n(_n), ys(_ys) {
        cnt.assign(n * 4 + 5, 0);
        len.assign(n * 4 + 5, 0);
    }
    void pushup(int u, int l, int r) {
        if (cnt[u]) len[u] = 1LL * ys[r + 1] - ys[l];
        else if (l == r) len[u] = 0;
        else len[u] = len[u << 1] + len[u << 1 | 1];
    }
    void add(int u, int l, int r, int ql, int qr, int v) {
        if (ql <= l && r <= qr) {
            cnt[u] += v;
            pushup(u, l, r);
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
        if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
        pushup(u, l, r);
    }
    void add(int l, int r, int v) {
        if (l > r) return;
        add(1, 0, n - 1, l, r, v);
    }
    long long query() {
        return len[1];
    }
};
long long scanLine(vector<array<int, 4>>& rects) {
    vector<Event> evs; evs.reserve(rects.size() * 2);
    vector<int> ys; ys.reserve(rects.size() * 2);
    for (auto& a : rects) {
        int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
        evs.push_back({x1, y1, y2, 1});
        evs.push_back({x2, y1, y2, -1});
        ys.emplace_back(y1), ys.emplace_back(y2);
    }
    if (evs.empty()) return 0;
    sort(ys.begin(), ys.end()); ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int m = ys.size() - 1;
    sort(evs.begin(), evs.end(), [&](Event& a, Event& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.v > b.v;
    });
    long long res = 0;
    ScanSegTree tr(m, ys);
    int lastx = evs[0].x;
    for (int i = 0; i < evs.size();) {
        int x = evs[i].x;
        res += tr.query() * (1LL * x - lastx);
        int j = i;
        while (j < evs.size() && evs[j].x == x) {
            int l = lower_bound(ys.begin(), ys.end(), evs[j].y1) - ys.begin();
            int r = lower_bound(ys.begin(), ys.end(), evs[j].y2) - ys.begin() - 1;
            if (l <= r) tr.add(l, r, evs[j].v);
            ++j;
        }
        lastx = x, i = j;
    }
    return res;
}
