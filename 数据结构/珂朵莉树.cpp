#include <bits/stdc++.h>
using namespace std;

/**
 * 珂朵莉树 / ODT：在操作满足随机且有推平操作（区间 set）时，使用珂朵莉树暴力维护连续颜色段
 *
 * 方法：
 * ODT(a)：使用数组 a 初始化，a[i] 代表颜色值
 * ODT(set<tr_node>)：使用 node 集合初始化；例如：ODT odt(set<tr_node>{{1,n,0}}) 表示 [1,n] 全部初始化为 0
 * get(pos)：获得 pos 下标位置的值；O(log n)，严格摊还
 * split(pos)：将 pos 所在区间从 pos 处断开，返回以 pos 开头的区间迭代器；O(log n)，严格摊还
 * split(l,r)：将区间 [l,r] 在左右边界断开成为新段；每次只会多出两段，严格摊还最终段一共只有 O(n + q) 个
 * assign(l,r,v)：将区间 [l,r] 全部赋值为 v；O(log n + k)，其中 k 为操作涉及的区间数量，q 次总计 O(n + q log n)
 * add(l,r,v)：将区间 [l,r] 全部加上 v；O(log n + k)，不严格均摊
 * kth(l,r,k)：返回区间 [l,r] 中第 k 小的数；O(log n + k log k)，不严格均摊
 * for_each(l,r,f)：对于下标在 [l, r] 中的每一段，依次调用 f(段左端点, 段右端点, 段颜色值)；
 *                  f(L,R,v) 代表 [L,R] 段值为 v，在这段上统计；
 *                  总复杂度 O(log n + k)，在 for_each(l,r,f) 后紧接 assign(l,r,v) 可以保证严格均摊（每个旧段只会被访问并消灭一次，保证所有操作累计遍历的旧端数为 n + Q）
 *                  常见操作：
 *      - count(x==v): [&](ll L,ll R,ll v){ if(v==x) res+=R-L+1; }
 *      - min/max: [&](ll L,ll R,ll v){ res=max(res,v); }
 *      - sum: [&](ll L,ll R,ll V){ res+=(R-L+1)*v; }
 *      - count(distinct): [&](ll L,ll R,ll v){ st.insert(v); }
 *
 * 注意：
 * 1. 使用 0-based 数组
 * 2. 前置条件：初始区间集合/数组非空；操作区间位于维护域内且非空；kth 满足 1<=k<=r-l+1
 * 3. 代码中的 n 代表右侧哨兵位置，不是数组大小
 * 4.【重要】ODT 可以只维护连续颜色段，其他数组统计值额外使用其他结构维护（例如 444C，先 for_each 对每个 [L,R,v] 在对应的线段树区间上操作，再 assign 保证均摊）
 *    这个 trick 非常重要。例：洛谷 P13766；给定数组 a,b；操作1：将 [l,r] 的 a 值增加 x；操作2：找到使得 sum_{i=l..r} |a[i]-y|*b[i] 最小的 y，再将 [l,r] 的 a 赋值为 y
 *    首先发现此题第二个操作就是找到加权中位数紧接着推平操作，因此适合 ODT 均摊维护。然而只用 ODT，操作 1 会被卡到 O(nq) 从而 TLE。
 *    此题的 Trick 为：**ODT 只为颜色段进行分裂与合并，段内的真实值用 BIT 维护！**
 *    考虑操作 1，每次用 ODT 中的 add 会被卡至 O(nq)，但是可以只使用 ODT 进行 split(l,r) 将颜色段分开，然后真实值用 a 上的差分 BIT 进行区间加来维护。
 *    对于操作 2，首先用 for_each 收集每个颜色段中的 {值，当前值总出现次数(即b在[L,R]的和)}，这个值是在 BIT 上单点查来的真实值。然后排序后计算中位数，最后再维护推平操作即可。
 *    复杂度分析：操作 1 的分裂，每次最多只增加两个新段。因此总共的段数量是 O(n + q) 的。对于操作 2，每碰到一个颜色段是 log 复杂度的，但由于 ODT 的摊还，每个段最多被碰到一次就会被推平（推平会再增加 O(q) 个段）
 *    综上，总共还是只有 O(n + q) 个颜色段会在整个过程中出现，而每一段最多只会被统计到一次，因此总复杂度严格摊还到 O((n + q) log (n + q)).
 */
struct tr_node {
    long long l, r;
    mutable long long v;
    tr_node(long long l, long long r, long long v) : l(l), r(r), v(v) {}
    bool operator<(const tr_node& o) const { return l < o.l; }
};
struct ODT {
    long long n = 1;
    set<tr_node> tree;
    ODT(set<tr_node> st) : tree(st) { n = tree.rbegin()->r + 1; }
    ODT(const vector<long long>& a) : n(a.size() - 1) {
        for (int l = 0; l <= n; ) {
            int r = l;
            while (r + 1 <= n && a[r + 1] == a[l]) ++r;
            tree.emplace(l, r, a[l]);
            l = r + 1;
        }
        ++n;
    }
    long long get(long long pos) const {
        auto it = tree.upper_bound(tr_node(pos, 0, 0));
        return prev(it)->v;
    }
    auto split(long long pos) {
        if (pos >= n) return tree.end();
        auto it = tree.lower_bound(tr_node(pos, 0, 0));
        if (it != tree.end() && it->l == pos) return it;
        --it;
        long long l = it->l, r = it->r, v = it->v;
        tree.erase(it);
        tree.emplace(l, pos - 1, v);
        return tree.emplace(pos, r, v).first;
    }
    void split(int l, int r) {
        split(l);
        if (r + 1 < n) split(r + 1);
    }
    auto merge_around(set<tr_node>::iterator it) {
        if (it != tree.begin()) {
            auto pre = prev(it);
            if (pre->r + 1 == it->l && pre->v == it->v) {
                long long l = pre->l, r = it->r, v = it->v;
                tree.erase(pre);
                tree.erase(it);
                it = tree.emplace(l, r, v).first;
            }
        }
        auto nxt = next(it);
        if (nxt != tree.end() && it->r + 1 == nxt->l && it->v == nxt->v) {
            long long l = it->l, r = nxt->r, v = it->v;
            tree.erase(nxt);
            tree.erase(it);
            it = tree.emplace(l, r, v).first;
        }
        return it;
    }
    void assign(long long l, long long r, long long v) {
        auto end = split(r + 1), begin = split(l);
        tree.erase(begin, end);
        merge_around(tree.emplace(l, r, v).first);
    }
    void add(long long l, long long r, long long v) {
        auto begin = split(l), end = split(r + 1);
        for (auto it = begin; it != end; ++it) {
            it->v += v;
        }
        // 以下为可选，合并首尾相同段
        merge_around(begin);
        merge_around(prev(tree.upper_bound(tr_node(r, 0, 0))));
    }
    template<class F>
    void for_each(long long l, long long r, F&& f) const {
        auto it = prev(tree.upper_bound(tr_node(l, 0, 0)));
        for (; it != tree.end() && it->l <= r; ++it) {
            f(max(l, it->l), min(r, it->r), it->v);
        }
    }
    long long kth(long long l, long long r, long long k) const {
        vector<pair<long long, long long>> vp;
        for_each(l, r, [&](long long L, long long R, long long v) {
            vp.emplace_back(v, R - L + 1);
        });
        sort(vp.begin(), vp.end());
        for (auto& [v, cnt] : vp) {
            k -= cnt;
            if (k <= 0) return v;
        }
        return -1;
    }
};
