#include <bits/stdc++.h>
using namespace std;

/**
 * 珂朵莉树 / ODT：在操作满足随机且有推平操作（区间 set）时，使用珂朵莉树暴力维护连续颜色段
 *
 * split(pos)：将 pos 所在区间从 pos 处断开，返回以 pos 开头的区间迭代器
 * assign(l,r,v)：将区间 [l,r] 全部赋值为 v
 * add(l,r,v)：将区间 [l,r] 全部加上 v
 * kth(l,r,k)：返回区间 [l,r] 中第 k 小的数
 * for_each(l,r,f)：依次调用 f(段左端点, 段右端点, 段值)；f(L,R,v) 代表 [L,R] 段值为 v，在这段上统计；常见操作：
 *      - count(x==v): [&](ll L,ll R,ll v){ if(v==x) res+=R-L+1; }
 *      - min/max: [&](ll L,ll R,ll v){ res=max(res,v); }
 *      - sum: [&](ll L,ll R,ll V){ res+=(R-L+1)*v; }
 *      - count(distinct): [&](ll L,ll R,ll v){ st.insert(v); }
 *
 * 单次操作会访问若干连续段，复杂度约为 O((访问段数+1) log S)，S 为当前段数；只有在数据随机且频繁 assign 的假设下通常较快
 * 前置条件：初始区间集合/数组非空；操作区间位于维护域内且非空；kth 满足 1<=k<=r-l+1
 * 注意这里构造器使用的数组是 1-based 下标的
 */
struct tr_node {
    long long l, r;
    mutable long long v;
    tr_node(long long l, long long r, long long v) : l(l), r(r), v(v) {}
    bool operator<(const tr_node& o) const { return l < o.l; }
};
struct ODT {
    long long n=1;
    set<tr_node> tree;
    ODT(set<tr_node> st) : tree(st) {
        n = tree.rbegin()->r + 1;
    }
    ODT(const vector<long long>& a) {
    n = a.size() - 1;
        for (int l = 1; l <= n; ) {
            int r = l;
            while (r + 1 <= n && a[r + 1] == a[l]) ++r;
            tree.emplace(l, r, a[l]);
            l = r + 1;
        }
        ++n;
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
        auto end = split(r + 1);
        for (auto it = split(l); it != end; ++it) {
            it->v += v;
        }
    }
    long long kth(long long l, long long r, long long k) {
        auto end = split(r + 1);
        vector<pair<long long, long long>> vp;
        for (auto it = split(l); it != end; ++it) {
            auto l = it->l, r = it->r, v = it->v;
            vp.emplace_back(v, r - l + 1);
        }
        sort(vp.begin(), vp.end());
        for (auto& [v, cnt] : vp) {
            k -= cnt;
            if (k <= 0) return v;
        }
        return -1;
    }
    template<class F>
    void for_each(long long l, long long r, F&& f) {
        auto end = split(r + 1);
        for (auto it = split(l); it != end; ++it) f(it->l, it->r, it->v);
    }
};
