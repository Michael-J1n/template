#include <bits/stdc++.h>
using namespace std;

/**
 * 线段树扫描线：矩形面积并
 *
 * rect = {x1, y1, x2, y2}  代表矩形的左下角，右上角坐标
 * 注意由于是连续段长，线段树下标 i 表示离散化后的 y 小段 [ys[i], ys[i + 1])
 * 前置条件：每个矩形满足 x1<=x2、y1<=y2
 * 
 * 复杂度：O(n log n)，其中 n 为 rects.size()
 */
struct Event {
    // 横坐标：x；纵坐标范围：[y1,y2]；事件：v=1 代表边进入，v=-1 代表边出去
    int x, y1, y2, v;
    bool operator<(const Event& e) const {
        if (x != e.x) return x < e.x;
        return v > e.v;
    }
};
struct ScanSegTree {
    // 线段树节点 [l, r] 表示 y 轴值域区间 [ys[l], ys[r+1])
    // 值域区间 [y1, y2) 对应线段树节点 [l, r]，其中 l=lowerbound(ys,y1), r=lowerbound(ys,y2)-1
    int n;
    vector<int> cnt;        // 当前节点被多少个区间修改操作完整覆盖；类似于懒标记（但不用 pushdown）
    vector<long long> len;  // 当前节点区间内，覆盖区间的并集长度
    vector<int>& ys;
    ScanSegTree(int _n, vector<int>& _ys) : n(_n), ys(_ys) {
        cnt.assign(n * 4 + 5, 0);
        len.assign(n * 4 + 5, 0);
    }
    void pushup(int u, int l, int r) {
        if (cnt[u]) len[u] = 1LL * ys[r + 1] - ys[l];  // 被某个区间完整覆盖了，因此并集长度就是区间长度；否则递归下去计算
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
    long long res = 0;
    // 对于值域进行离散化，ys[i] 表示第 i 个离散化坐标
    // 规定值域区间均为左闭右开
    // 线段树叶子 i 表示 [ys[i], ys[i+1]) 这一段区间；因此线段树节点 [l,r] 对应真实坐标区间 [ys[l], ys[r+1]).
    sort(ys.begin(), ys.end()); ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int m = ys.size() - 1;
    ScanSegTree tr(m, ys);
    sort(evs.begin(), evs.end());
    int lastx = evs[0].x;
    for (int i = 0; i < evs.size();) {
        int x = evs[i].x;
        // 在整个 [lastx, x) 区间内，活跃矩形集合没有发生改变
        res += tr.query() * (1LL * x - lastx);
        int j = i;
        while (j < evs.size() && evs[j].x == x) {
            int l = lower_bound(ys.begin(), ys.end(), evs[j].y1) - ys.begin();
            int r = lower_bound(ys.begin(), ys.end(), evs[j].y2) - ys.begin() - 1;
            // y 轴值域区间 [y1, y2) 对应的线段树节点为 [l, r]
            if (l <= r) tr.add(l, r, evs[j].v);
            ++j;
        }
        lastx = x, i = j;
    }
    return res;
}
