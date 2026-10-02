#include <bits/stdc++.h>
using namespace std;

/**
 * 01 Trie / Xor Trie
 * 
 * 状态定义：
 * 维护一个可重复集合，节点 u 表示某个二进制前缀，t[u].ch[0/1] 表示下一位为 0/1 的儿子，t[u].cnt 表示这个前缀下当前存在的数的个数。
 * B 表示最高位，默认 int 为 30，long long 为 62，因此默认维护非负整数值域 [0, 2^(B+1)-1]。
 * 前置条件：T 按非负整数/无符号位模式解释，0<=B<类型位数；需选择、取最值的操作在非空集合上调用且 k 合法。
 * 
 * 操作：
 * insert(x)：插入一个 x。
 * count(x)：查询 x 的出现次数。
 * remove(x)：删除一个 x，不存在则不操作。
 * erase(x)：删除 x 的所有出现。
 * kth(k)：查询集合中第 k 小的数，k 为 1-based，不存在返回 numeric_limits<T>::min()。
 * rank(x)：查询集合中严格小于 x 的数的个数。
 * query_max(x)：查询 max{x^y}，y 在集合中，空集返回 numeric_limits<T>::min()。
 * query_max_val(x)：查询一个 y，使得 x^y 最大，空集返回 -1。
 * query_min(x)：查询 min{x^y}，y 在集合中，空集返回 numeric_limits<T>::max()。
 * query_min_val(x)：查询一个 y，使得 x^y 最小，空集返回 -1。
 * size()：查询集合大小。
 * empty()：判断集合是否为空。
 * clear()：清空集合。
 * prev(x)：查询最大的 < x 的数，不存在返回 numeric_limits<T>::min()。
 * prev_eq(x)：查询最大的 <= x 的数，不存在返回 numeric_limits<T>::min()。
 * lower_bound(x)：查询最小的 >= x 的数，不存在返回 numeric_limits<T>::min()。
 * upper_bound(x)：查询最小的 > x 的数，不存在返回 numeric_limits<T>::min()。
 * count_xor_less(x, k)：查询满足 (x^y)<k 的 y 的个数。
 * count_xor_leq(x, k)：查询满足 (x^y)<=k 的 y 的个数。
 * kth_xor(x, k)：查询第 k 小的 x^y，k 为 1-based，不存在返回 numeric_limits<T>::min()。
 * kth_xor_val(x, k)：查询一个 y，使得 x^y 是第 k 小，空集或 k 非法返回 -1。
 * 
 * 复杂度：
 * 所有操作 O(B)，空间 O(插入次数 * B)，删除不回收节点，只减少 cnt。
 * 
 * 注意：
 * T 应该为 signed 整数类型，常用 int / long long。
 * 模板只适合维护非负数；如果插入负数，rank / kth / prev / lower_bound 的普通整数有序语义不成立。
 * 自定义 B 时需要保证 0 <= B < numeric_limits<T>::digits，否则移位和返回值语义可能出错。
 * 构造函数第二个参数和 reserve_nodes(cap) 可预留节点容量；clear() 清空但保留容量。
 */
template<class T = int>
struct XorTrie {
    static_assert(is_signed_v<T>);
    using U = make_unsigned_t<T>;
    struct Node {
        int ch[2], cnt;
        Node() { ch[0] = ch[1] = -1, cnt = 0; }
    };
    vector<Node> t;
    int B;
    XorTrie(int _B = numeric_limits<T>::digits - 1, int cap = 0) : B(_B) {
        if (cap) t.reserve(cap + 1);
        t.emplace_back();
    }
    void reserve_nodes(int cap) { t.reserve(cap + 1); }
    void insert(T x) {
        int u = 0;
        ++t[u].cnt;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1);
            if (t[u].ch[c] == -1) t[u].ch[c] = t.size(), t.emplace_back();
            u = t[u].ch[c];
            ++t[u].cnt;
        }
    }
    int count(T x) {
        int u = 0;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1);
            u = t[u].ch[c];
            if (u == -1) return 0;
        }
        return t[u].cnt;
    }
    void remove(T x) {
        if (!count(x)) return;
        int u = 0;
        --t[u].cnt;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1);
            u = t[u].ch[c];
            --t[u].cnt;
        }
    }
    void erase(T x) {
        int k = count(x);
        if (!k) return;
        int u = 0;
        t[u].cnt -= k;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1);
            u = t[u].ch[c];
            t[u].cnt -= k;
        }
    }
    T kth(int k) {
        if (k <= 0 || k > t[0].cnt) return numeric_limits<T>::min();
        int u = 0;
        T res = 0;
        for (int b = B; ~b; --b) {
            int l = t[u].ch[0], lc = (l == -1 ? 0 : t[l].cnt);
            if (k <= lc) u = l;
            else k -= lc, u = t[u].ch[1], res |= T(U(1) << b);
        }
        return res;
    }
    int rank(T x) {
        int u = 0, res = 0;
        for (int b = B; ~b; --b) {
            if (u == -1) break;
            int c = int((U(x) >> b) & 1);
            if (c) {
                int l = t[u].ch[0];
                if (l != -1) res += t[l].cnt;
            }
            u = t[u].ch[c];
        }
        return res;
    }
    T query_max(T x) {
        if (!t[0].cnt) return numeric_limits<T>::min();
        int u = 0;
        T res = 0;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1), v = t[u].ch[c ^ 1];
            if (v != -1 && t[v].cnt > 0) res |= T(U(1) << b), u = v;
            else u = t[u].ch[c];
        }
        return res;
    }
    T query_max_val(T x) {
        if (!t[0].cnt) return T(-1);
        int u = 0;
        T y = 0;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1), w = c ^ 1, v = t[u].ch[w], bit;
            if (v != -1 && t[v].cnt > 0) bit = w, u = v;
            else bit = c, u = t[u].ch[c];
            y |= T(U(bit) << b);
        }
        return y;
    }
    T query_min(T x) {
        if (!t[0].cnt) return numeric_limits<T>::max();
        int u = 0;
        T res = 0;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1), v = t[u].ch[c];
            if (v != -1 && t[v].cnt > 0) u = v;
            else u = t[u].ch[c ^ 1], res |= T(U(1) << b);
        }
        return res;
    }
    T query_min_val(T x) {
        if (!t[0].cnt) return T(-1);
        int u = 0;
        T y = 0;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1), v = t[u].ch[c], bit;
            if (v != -1 && t[v].cnt > 0) bit = c, u = v;
            else bit = c ^ 1, u = t[u].ch[c ^ 1];
            y |= T(U(bit) << b);
        }
        return y;
    }
    int size() { return t[0].cnt; }
    bool empty() { return !t[0].cnt; }
    void clear() { t.clear(); t.emplace_back(); }
    T prev(T x) { return kth(rank(x)); }
    T prev_eq(T x) { return kth(rank(x) + count(x)); }
    T lower_bound(T x) { return kth(rank(x) + 1); }
    T upper_bound(T x) { return kth(rank(x) + count(x) + 1); }
    int count_xor_less(T x, T k) {
        if (k <= 0) return 0;
        if (B + 1 < numeric_limits<T>::digits && k >= T(U(1) << (B + 1))) return t[0].cnt;
        int u = 0, res = 0;
        for (int b = B; ~b && u != -1; --b) {
            int c = int((U(x) >> b) & 1), v = int((U(k) >> b) & 1);
            if (v) {
                int same = t[u].ch[c];
                if (same != -1) res += t[same].cnt;
                u = t[u].ch[c ^ 1];
            } else {
                u = t[u].ch[c];
            }
        }
        return res;
    }
    int count_xor_leq(T x, T k) {
        if (k < 0) return 0;
        T lim = (B + 1 == numeric_limits<T>::digits ? numeric_limits<T>::max() : T((U(1) << (B + 1)) - 1));
        if (k >= lim) return t[0].cnt;
        return count_xor_less(x, k + 1);
    }
    T kth_xor(T x, int k) {
        if (k <= 0 || k > t[0].cnt) return numeric_limits<T>::min();
        int u = 0;
        T res = 0;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1);
            int same = t[u].ch[c], sc = (same == -1 ? 0 : t[same].cnt);
            if (k <= sc) {
                u = same;
            } else {
                k -= sc;
                u = t[u].ch[c ^ 1];
                res |= T(U(1) << b);
            }
        }
        return res;
    }
    T kth_xor_val(T x, int k) {
        if (k <= 0 || k > t[0].cnt) return T(-1);
        int u = 0;
        T y = 0;
        for (int b = B; ~b; --b) {
            int c = int((U(x) >> b) & 1);
            int same = t[u].ch[c], sc = (same == -1 ? 0 : t[same].cnt);
            if (k <= sc) {
                y |= T(U(c) << b);
                u = same;
            } else {
                k -= sc;
                y |= T(U(c ^ 1) << b);
                u = t[u].ch[c ^ 1];
            }
        }
        return y;
    }
};
