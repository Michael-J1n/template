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
 * 整体二分 / Parallel Binary Search【求多个对象第一次满足条件的操作时刻】
 *
 * 下标约定：对象、位置、操作均 0-based，答案域为闭区间 [l, r]。可额外令 q 表示“所有操作结束后仍未满足”。
 * 以洛谷【P3527】为例，m 个点属于 n 个国家，每个国家有阈值。每次事件为区间加，求每个城市在最少多少次事件后满足阈值。由于需要动态区间加，使用树状数组作为外部维护的数据结构。
 *
 * 状态定义：
 * solve(l, r, ids) 表示 ids 中所有对象的答案均落在 [l, r]。最后答案：solve(0, m, all_ids)，其中 m 代表所有操作之后仍未满足（[0,m-1] 为答案值域）
 * need[id] 表示经过操作 [0, l - 1] 后，对象 id 距离满足条件还缺少的贡献。
 *
 * 适用：每个对象都要二分答案求解，因此可以全局对于所有对象整体二分；例如本题，或区间kth：每个区间都要对值域二分，因此可以整体对值域二分并对查询分类（例：P3332【https://www.luogu.com.cn/record/298997850】）
 * 注意：递归到后半区间的一定要保留前半区间的事件的影响！
 * 
 * 复杂度：O((m + q) log m log q)，其中 m 为每次判断需要扫描的复杂度，q 为修改事件个数。
 */
int n, m;
vector<int> ans, need;    // ans[i]=j代表第i个国家在操作j之后i满足，need[i]=第i个国家阈值（会跟随递归过程动态修改）
vector<vector<int>> pos;  // pos[i]=第i个国家管辖的下标列表
struct Event {            // 事件：区间 [L,R] 加 x
    int l, r;
    long long x;
};
vector<Event> events;
BIT<int> bit;
/** ids 中的所有元素答案在 [l, r] 中 */
void solve(int l, int r, vector<int>& ids) {
    if (ids.empty()) return;
    if (l == r) {
        for (int id : ids) ans[id] = l;
        return;
    }
    int mid = l + r >> 1;
    // 将前一半事件全部应用；【修改：循环内的事件处理逻辑】
    for (int i = l; i <= mid; ++i) {
        auto& [L, R, x] = events[i];
        if (L <= R) {
            bit.add(L, x);
            if (R + 1 < m) bit.add(R + 1, -x);
        } else {
            bit.add(L, x);
            bit.add(0, x);
            if (R + 1 < m) bit.add(R + 1, -x);
        }
    }
    // L 中存储所有已经满足的事件，R 中存储所有还未满足的事件；注意所有已经满足的事件保留前半段带来的影响！【修改：循环内的元素满足逻辑】
    vector<int> L, R;
    for (int id : ids) {
        long long sum = 0;
        for (int p : pos[id]) {
            sum += bit.pref_sum(p);
            if (sum >= need[id]) break;
        }
        if (sum >= need[id]) L.push_back(id);
        else need[id] -= sum, R.push_back(id);
    }
    // 撤销所有前一半事件的影响，再进行递归；【修改：循环内的事件删除逻辑】
    for (int i = l; i <= mid; ++i) {
        auto& [L, R, x] = events[i];
        if (L <= R) {
            bit.add(L, -x);
            if (R + 1 < m) bit.add(R + 1, x);
        } else {
            bit.add(L, -x);
            bit.add(0, -x);
            if (R + 1 < m) bit.add(R + 1, x);
        }
    }
    solve(l, mid, L), solve(mid + 1, r, R);
}