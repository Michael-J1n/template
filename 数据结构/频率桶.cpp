#include <bits/stdc++.h>
using namespace std;

/**
 * 频率表 + 频率的频率表
 *
 * 构造：erase_zero 代表键无效后是否释放内存；参数两个代表 最大可能频率值 maxTot 以及预期会有的种类数 expectedKind
 * 1. 普通动态键集合：FreqTable<int, hash<int>, true> frq(n, n);
 * 2. 固定键集合：FreqTable<int, hash<int>, false> frq(n, n);
 * 
 * FreqTable(n, cap)：最大元素总数为 n，预留 cap 种键的哈希空间
 * 前置条件：任一值的实际频率不超过构造参数 maxTot（即这里的 n）
 * count(x)：返回值 x 当前的出现次数
 * count_freq(f)：返回出现次数恰好为 f 的值的种数
 * contains(x)：判断值 x 当前是否存在
 * min_freq()：返回当前最小正频率，空表返回 0
 * max_freq()：返回当前最大频率，空表返回 0
 * add(x)：加入一个值 x
 * del(x)：删除一个值 x，不存在返回 false
 * erase(x)：删除值 x 的所有出现，返回删除数量
 *
 * tot：当前元素总数
 * kind：当前不同值的数量
 * diff：当前不同正频率的数量
 * erase_zero=true：频率归零后删除哈希表中的键
 * erase_zero=false：频率归零后保留哈希表中的键
 *
 * 单次操作期望 O(1)，空间 O(n + kind)
 */
template<class T, class Hash = hash<T>, bool erase_zero = true>
struct FreqTable {
    struct Node { int cnt, pre, nxt; };
    int n;
    unordered_map<T, int, Hash> cnt;
    vector<Node> f;
    int tot, kind, diff;
    FreqTable(int _n, int cap = 0) : n(_n) {
        cnt.max_load_factor(0.7);
        if (cap) cnt.reserve(cap);
        f.resize(n + 2); f[0].nxt = n + 1; f[n + 1].pre = 0;
        tot = kind = diff = 0;
    }
    void link(int l, int x) {
        int r = f[l].nxt;
        f[l].nxt = x, f[x].pre = l, f[x].nxt = r, f[r].pre = x;
    }
    void cut(int x) {
        int l = f[x].pre, r = f[x].nxt;
        f[l].nxt = r, f[r].pre = l;
    }
    int count(const T& x) const {
        auto it = cnt.find(x);
        return it == cnt.end() ? 0 : it->second;
    }
    int count_freq(int x) const { return f[x].cnt; }
    int min_freq() const { return f[0].nxt == n + 1 ? 0 : f[0].nxt; }
    int max_freq() const { return f[n + 1].pre; }
    bool contains(const T& x) const { return count(x) != 0; }
    void add(const T& x) {
        auto [it, inserted] = cnt.try_emplace(x, 0);
        int &c = it->second, old = c, l = 0;
        if (old) {
            l = f[old].pre;
            if (--f[old].cnt == 0) cut(old), --diff;
            else l=old;
        } else ++kind;
        ++c, ++tot;
        if (++f[c].cnt == 1) link(l, c), ++diff;
    }
    bool del(const T& x) {
        auto it = cnt.find(x);
        if (it == cnt.end() || it->second == 0) return false;
        int &c = it->second, old = c, l = f[old].pre;
        if (--f[old].cnt == 0) cut(old), --diff;
        --c, --tot;
        if (c == 0) {
            --kind;
            if constexpr (erase_zero) cnt.erase(it);
        } else if (++f[c].cnt == 1) link(l, c), ++diff;
        return true;
    }
    int erase(const T& x) {
        auto it = cnt.find(x);
        if (it == cnt.end() || it->second == 0) return 0;
        int old = it->second;
        if (--f[old].cnt == 0) cut(old), --diff;
        tot -= old, --kind;
        if constexpr (erase_zero) cnt.erase(it);
        else it->second = 0;
        return old;
    }
};
