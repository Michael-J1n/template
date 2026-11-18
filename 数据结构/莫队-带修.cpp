#include <bits/stdc++.h>
using namespace std;

/**
 * 带修莫队
 *
 * 状态定义：
 * Event(op, l, r, id)：op = 0 表示修改 a[l] = r；op = 1 表示查询 [l, r]，答案放到 res[id]
 * a[i]：当前时间下位置 i 的值
 * T：当前已经执行了前 T 次修改
 *
 * 转移：
 * add / del 维护元素进入 / 离开区间 [L, R]
 * apply / undo 维护时间 T，暴力加入 / 撤销修改
 *
 * 复杂度：
 * B 取 n^(2/3)，整体约 O((n + m) n^(2/3)) ~= O(n^(5/3))
 * 当修改查询不平衡时，可以有更精细的公式 B ≈ max(n/sqrt(q), (u n²/q)^(1/3))
 *
 * 注意：
 * 1. evs 按题目原顺序传入
 * 2. 查询 id 必须为 0, 1,..., querySize - 1（！！！）
 * 3. 默认维护区间不同数个数，改题时只改 cnt / cur / add / del 的逻辑即可
 * 4. 如果查询等和特定值有关，修改 Event 多一维 x 即可，但是传进来务必一起加入离散化！！
 * 5. 前置条件：原数组非空；修改位置合法；查询满足 0<=l<=r<n，且查询 id 合法并互不重复
 */
struct Event {
    int op, l, r, id;   // modify: {0, p, v, 0}; query: {1, l, r, id}
};
vector<int> moAlgorithmWithModify(vector<int>& orig_a, vector<Event>& evs) {
    int n = orig_a.size();
    // 离散化
    vector<int> xs = orig_a;
    for (auto& e : evs) {
        if (e.op == 0) xs.emplace_back(e.r);
    }
    sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
    int tot = xs.size();
    auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
    vector<int> b = orig_a;
    for (int i = 0; i < n; ++i) b[i] = find(b[i]);
    vector<Event> ms, qs;    // ms: {0, p, val, old}，qs: {t, l, r, id}
    vector<int> a = b;
    int t = 0, q = 0;
    for (auto& e : evs) {
        if (e.op == 0) {
            int val = find(e.r);
            ms.push_back({0, e.l, val, a[e.l]});
            a[e.l] = val;
            ++t;
        } else {
            qs.push_back({t, e.l, e.r, e.id});
            ++q;
        }
    }
    a = b;
    // 对查询分块；蛇形排序
    int B = pow(n, 2.0 / 3) + 1;
    // int B1 = max(1, int(n / sqrt(max(1, q)))), B2 = max(1, int(cbrt(1.0 * max(1, t) * n * n / max(1, q)))), B = max(B1, B2);
    ranges::sort(qs, [&](auto& x, auto& y) {
        int bx = x.l / B, by = y.l / B;
        if (bx != by) return bx < by;
        int rx = x.r / B, ry = y.r / B;
        if (rx != ry) {
            if (bx & 1) return rx > ry;
            return rx < ry;
        }
        if (rx & 1) return x.op > y.op;
        return x.op < y.op;
    });

    /* ------------------ 核心部分：单点维护 ---------------- */
    vector<int> cnt(tot);                   // 频率统计桶
    vector<int> res(q);                     // 询问对应的答案
    int cur = 0;                            // 维护当前答案
    int L = 0, R = -1;                      // 当前区间左右端点
    auto add = [&](int i) -> void {         // 下标为 i 的元素进入查询区间
        int x = a[i];
        if (cnt[x] == 0) ++cur;
        ++cnt[x];
    };
    auto del = [&](int i) -> void {         // 下标为 i 的元素离开查询区间
        int x = a[i];
        --cnt[x];
        if (cnt[x] == 0) --cur;
    };

    auto apply = [&](int t, int L, int R) -> void {    // 执行时间 t 对应的修改
        auto& e = ms[t];
        int p = e.l, v = e.r;
        if (L <= p && p <= R) del(p);
        a[p] = v;
        if (L <= p && p <= R) add(p);
    };
    auto undo = [&](int t, int L, int R) -> void {     // 撤销时间 t 对应的修改 
        auto& e = ms[t];
        int p = e.l, old = e.id;
        if (L <= p && p <= R) del(p);
        a[p] = old;
        if (L <= p && p <= R) add(p);
    };
    // 处理查询
    int T = 0;
    for (auto& e : qs) {
        int l = e.l, r = e.r, t = e.op, id = e.id;
        while (L > l) add(--L);
        while (R < r) add(++R);
        while (L < l) del(L++);
        while (R > r) del(R--);
        while (T < t) apply(T++, L, R);
        while (T > t) undo(--T, L, R);
        res[id] = cur;
    }
    return res;
}
