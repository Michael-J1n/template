#include <bits/stdc++.h>
using namespace std;

int n, m;
struct BIT {
    int n;
    vector<int> c;  // c[i] = sum of nums[i-lowbit(i)...i-1]
    BIT(int _n) : n(_n) {
        c.resize(n + 1);
    }
    BIT(vector<int>& a) : n(a.size()) {
        c.resize(n + 1);
        for (int i = 1; i <= n; ++i) c[i] = a[i - 1];
        for (int i = 1; i <= n; ++i) {
            int j = i + (i & -i);
            if (j <= n) c[j] += c[i];
        }
    }
    void add(int id, int val) {
        ++id;
        for (int i = id; i <= n; i += i & -i) c[i] += val;
    }
    void set(int id, int val) {
        add(id, val - get(id));
    }
    int pref_sum(int id) {
        ++id;
        int sum = 0;
        for (int i = id; i >= 1; i &= i - 1) sum += c[i];
        return sum;
    }
    int range_sum(int l, int r) {
        if (l > r) return 0;
        return pref_sum(r) - (l ? pref_sum(l - 1) : 0);
    }
    int get(int id) {
        return range_sum(id, id);
    }
    int kth(int k) {
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
 * 给定一个颜色数组，以及 q 个无修改的查询 [l,r]，表示查询 [l,r] 上唯一颜色个数
 * 
 * HH 项链 trick：将询问离线，右端点排序
 * 数组从左到右，额外维护 last[x] 表示 x 上一次出现的地方。HH trick 即为只把 last 的位置置 1，其他置 0
 * 保证询问时永远只有最靠右的元素贡献一次计数
 * 
 * 注意：很多 HHTrick 处理的问题，也都可以用莫队（但注意复杂度更劣）
 * 前置条件：数组非空；每个询问满足 0<=l<=r<n，id 位于 [0,q-1] 且互不重复
 * 
 * 扩展：求数组的所有子数组 cnt(不同元素个数) 之和
 * 定义 f(r) 为以 r 为右端点的所有子数组的唯一颜色数之和。从左到右遍历 r 求和即为答案，考虑如何求 f(r).
 * 首先由于 HHTrick，我们可以维护一个 last 数组来记录当前元素的上一个最近出现位置（没出现过即为 -1）；
 * 那么考虑从 r-1 变为 r 时，如何从 f(r-1) 转移到 f(r)：对于左端点 l∈[last[a[r]]+1,r] 的位置，子数组 [l,r] 的唯一颜色数增加一个；其余不变。
 * 因此可以维护懒标记线段树，转移时将 [last+1,r] 整体 +1，查询 [0,r] 所有数之和即为 f(r) 值。
 */
struct Query {
    int l, r;
    int id;
};
vector<int> HHTrick(vector<int>& a, vector<Query>& qs) {
    int n = a.size(), m = qs.size();
    vector<int> res(m);
    sort(qs.begin(), qs.end(), [&](auto& q1, auto& q2) {
        return q1.r < q2.r;
    });
    BIT tr(n);
    vector<int> xs = a;
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    vector<int> last(xs.size(), -1);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        int x = lower_bound(xs.begin(), xs.end(), a[i]) - xs.begin();
        if (last[x] != -1) tr.add(last[x], -1);
        last[x] = i;
        tr.add(i, 1);
        while (j < m && qs[j].r == i) {
            auto& [l, r, id] = qs[j];
            res[id] = tr.range_sum(l, r);
            ++j;
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cin >> m;
    vector<Query> qs(m);
    for (int i = 0; i < m; ++i) {
        int l, r; cin >> l >> r; --l, --r;
        qs[i] = {l, r, i};
    }
    vector<int> res = HHTrick(a, qs);
    for (auto& x : res) cout << x << "\n";
    return 0;
}
