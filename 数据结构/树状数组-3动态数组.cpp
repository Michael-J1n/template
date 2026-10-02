#include <bits/stdc++.h>
using namespace std;

/**
 * 动态数组 BIT
 * 核心在于用一个 01 数组 alive 维护原数组每个位置是否还存在
 * 如果删除原数组下标 id，就把 alive[id] 从 1 改成 0
 * 那么 pref_sum(id) 就表示原数组 [0..id] 中还有多少个位置没被删除
 *
 * 操作：
 * alive(id)：查询原数组 id 是否还存在
 * erase(id)：删除原数组 id
 * restore(id)：恢复原数组 id
 * rank(id)：查询原数组 id 在当前动态数组中的 0-based 新下标，要求 alive(id) = 1
 * kth(k)：查询当前动态数组中第 k 个位置对应的原数组下标，k 从 1 开始
 * size(): 当前真实剩余下标数
 * 前置条件：修改/查询下标满足 0<=id<n；kth 时结构非空且 1<=k<=size()
 * 
 * 复杂度：
 * 初始化 O(n)； 单次修改/查询 O(logn)
 */
struct DynamicBIT {
    int n, tot;
    vector<int> c;  // c[i] = sum of alive[i-lowbit(i)..i-1]
    DynamicBIT(int _n) : n(_n), tot(_n) {
        c.resize(n + 1);
        for (int i = 1; i <= n; ++i) c[i] = 1;
        for (int i = 1; i <= n; ++i) {
            int j = i + (i & -i);
            if (j <= n) c[j] += c[i];
        }
    }
    void add(int id, int val) {
        ++id;
        for (int i = id; i <= n; i += i & -i) c[i] += val;
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
    int alive(int id) {
        return range_sum(id, id);
    }
    int size() {
        return tot;
    }
    void erase(int id) {
        if (alive(id)) add(id, -1), --tot;
    }
    void restore(int id) {
        if (!alive(id)) add(id, 1), ++tot;
    }
    int rank(int id) {
        return pref_sum(id) - 1;
    }
    int kth(int k) {
        int pos = 0;
        for (int i = __lg(n); ~i; --i) {
            int nxt = pos + (1 << i);
            if (nxt <= n && c[nxt] < k) pos = nxt, k -= c[nxt];
        }
        return pos;
    }
};


// 例：约瑟夫环问题，圆环上隔着 m 报数出列：
void solve() {
    int n, m; cin >> n >> m;
    DynamicBIT tr(n);
    int cur = 0;  // 从哪个存活者开始报数
    while (tr.size()) {
    	int tot = tr.size();
    	int pos = (cur + m - 1) % tot;  // 需要出列的是第几个人（0-based）
    	int id = tr.kth(pos + 1);
    	cout << id + 1 << " ";          // 用 kth 和输出编号都转化为 1-based
    	tr.erase(id);
    	if (tr.size()) cur = pos % tr.size();
    }
}
