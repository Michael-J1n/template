#include <bits/stdc++.h>
using namespace std;

/**
 * B-Tree / AVX2 B-Tree【动态有序多重集合】
 *
 * 索引 / 区间约定：
 * 数组下标 0-based；rank / kth 为 1-based；cnt_range(l, r) 统计闭区间 [l, r]。
 *
 * 支持操作：
 * insert(x)：插入一个 x
 * erase_one(x)：删除一个 x，若有多个只删一个
 * erase_all(x)：删除所有 x
 * clear()：清空整棵树
 * size()：返回元素总数，包含重复元素
 * find(x)：判断 x 是否存在
 * count(x)：返回 x 的出现次数
 * lt_cnt(x)：返回严格 <x 的元素出现次数
 * le_cnt(x)：返回 <=x 的元素出现次数
 * cnt_range(l, r)：返回 [l, r] 内元素个数
 * rank(x)：返回 x 的排名，1-based，等于 1 + #(< x)
 * kth(k)：返回第 k 小，1-based，要求 1 <= k <= size()
 * min_val()：返回最小值，要求非空
 * max_val()：返回最大值，要求非空
 * lower_bound(x)：返回最小 >=x 的值，不存在返回 numeric_limits<int>::max()
 * upper_bound(x)：返回最小 >x 的值，不存在返回 numeric_limits<int>::max()
 * pre_val(x)：返回最大 <x 的值，不存在返回 numeric_limits<int>::lowest()
 * nxt_val(x)：返回最小 >x 的值，不存在返回 numeric_limits<int>::max()
 *
 * 适用：
 * int 动态有序多重集合；使用 AVX2 加速节点内查找。
 * 适合支持 AVX2 的 GCC / Clang x86-64 环境。
 *
 * 复杂度：
 * 设 m 为不同值个数，树高 O(log m)，每个节点最多 63 个不同键。
 * 所有查询和修改访问 O(log m) 个 B-Tree 节点。
 * size O(1)，clear O(1)。
 */
#include <immintrin.h>
// #pragma GCC optimize("O3")
// #pragma GCC target("avx2")
struct BTree {
    static constexpr int ORDER = 64;
    static constexpr int MAX_KEYS = ORDER - 1;
    static constexpr int MIN_KEYS = (ORDER - 1) / 2;
    struct Node {
        int key[ORDER];
        int ch[ORDER];
        int cnt[ORDER];
        int sub[ORDER];
        int num, sz;
        bool leaf;
    };
    vector<Node> t;
    int rt, bin, ptr;
    BTree(int cap = 0) : rt(0), bin(0), ptr(0) {
        if (cap) t.reserve(cap / MIN_KEYS + 8);
        t.emplace_back();
    }
    __attribute__((target("avx2")))
    inline static int lower_pos_avx2(const int a[], int x) {
        __m256i vx = _mm256_set1_epi32(x);
        int res = 0, st = 0;
        if (a[31] < x) res = 32, st = 32;
        for (int i = st; i < st + 32; i += 8) {
            __m256i va = _mm256_loadu_si256((const __m256i*)&a[i]);
            __m256i cmp = _mm256_cmpgt_epi32(vx, va);
            int mask = _mm256_movemask_ps(_mm256_castsi256_ps(cmp));
            res += __builtin_popcount((unsigned)mask);
        }
        return res;
    }
    __attribute__((target("avx2")))
    inline static void insert_key_avx2(int a[], int x, int p) {
        int pre[8];
        pre[0] = x;
        for (int i = 1; i < 8; ++i) pre[i] = a[i * 8 - 1];
        __m256i vx = _mm256_set1_epi32(x);
        const __m256i idx = _mm256_setr_epi32(7, 0, 1, 2, 3, 4, 5, 6);
        for (int c = 0; c < 8; ++c) {
            int st = c * 8;
            __m256i va = _mm256_loadu_si256((const __m256i*)&a[st]);
            __m256i v = _mm256_insert_epi32(va, pre[c], 7);
            __m256i shifted = _mm256_permutevar8x32_epi32(v, idx);
            __m256i cmp = _mm256_cmpgt_epi32(vx, va);
            __m256i res = _mm256_blendv_epi8(shifted, va, cmp);
            _mm256_storeu_si256((__m256i*)&a[st], res);
        }
        a[p] = x;
    }
    __attribute__((target("avx2")))
    inline static void erase_key_avx2(int a[], int x) {
        int nxt[8];
        for (int i = 0; i < 7; ++i) nxt[i] = a[i * 8 + 8];
        nxt[7] = INT_MAX;
        __m256i vx = _mm256_set1_epi32(x);
        const __m256i idx = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 0);
        for (int c = 0; c < 8; ++c) {
            int st = c * 8;
            __m256i va = _mm256_loadu_si256((const __m256i*)&a[st]);
            __m256i v = _mm256_insert_epi32(va, nxt[c], 0);
            __m256i shifted = _mm256_permutevar8x32_epi32(v, idx);
            __m256i cmp = _mm256_cmpgt_epi32(vx, va);
            __m256i res = _mm256_blendv_epi8(shifted, va, cmp);
            _mm256_storeu_si256((__m256i*)&a[st], res);
        }
    }
    void init_node(int u, bool leaf) {
        fill(t[u].key, t[u].key + ORDER, INT_MAX);
        fill(t[u].ch, t[u].ch + ORDER, 0);
        fill(t[u].cnt, t[u].cnt + ORDER, 0);
        fill(t[u].sub, t[u].sub + ORDER, 0);
        t[u].num = t[u].sz = 0;
        t[u].leaf = leaf;
    }
    int new_node(bool leaf) {
        int u;
        if (bin) {
            u = bin;
            bin = t[u].ch[0];
        } else {
            u = ++ptr;
            if (u == t.size()) t.emplace_back();
        }
        init_node(u, leaf);
        return u;
    }
    void recycle(int u) {
        t[u].ch[0] = bin;
        bin = u;
    }
    void pull(int u) {
        int sz = 0;
        for (int i = 0; i < t[u].num; ++i) sz += t[u].sub[i] + t[u].cnt[i];
        sz += t[u].sub[t[u].num];
        t[u].sz = sz;
    }
    void split_child(int u, int p) {
        int v = t[u].ch[p], w = new_node(t[v].leaf);
        int mid = MAX_KEYS / 2, mid_key = t[v].key[mid], mid_cnt = t[v].cnt[mid];
        t[w].num = MAX_KEYS - mid - 1;
        for (int i = 0; i < t[w].num; ++i) {
            t[w].key[i] = t[v].key[mid + 1 + i];
            t[w].cnt[i] = t[v].cnt[mid + 1 + i];
        }
        if (!t[v].leaf) {
            for (int i = 0; i <= t[w].num; ++i) {
                t[w].ch[i] = t[v].ch[mid + 1 + i];
                t[w].sub[i] = t[v].sub[mid + 1 + i];
            }
        }
        t[v].num = mid;
        for (int i = mid; i < ORDER; ++i) t[v].key[i] = INT_MAX, t[v].cnt[i] = 0;
        if (!t[v].leaf) for (int i = mid + 1; i < ORDER; ++i) t[v].ch[i] = t[v].sub[i] = 0;
        pull(v), pull(w);
        for (int i = t[u].num; i > p; --i) {
            t[u].key[i] = t[u].key[i - 1];
            t[u].cnt[i] = t[u].cnt[i - 1];
            t[u].ch[i + 1] = t[u].ch[i];
            t[u].sub[i + 1] = t[u].sub[i];
        }
        t[u].key[p] = mid_key;
        t[u].cnt[p] = mid_cnt;
        t[u].ch[p] = v;
        t[u].sub[p] = t[v].sz;
        t[u].ch[p + 1] = w;
        t[u].sub[p + 1] = t[w].sz;
        ++t[u].num;
        pull(u);
    }
    void insert_non_full(int u, int x) {
        int p = lower_pos_avx2(t[u].key, x);
        if (p < t[u].num && t[u].key[p] == x) {
            ++t[u].cnt[p];
            ++t[u].sz;
            return;
        }
        if (t[u].leaf) {
            insert_key_avx2(t[u].key, x, p);
            for (int i = t[u].num; i > p; --i) t[u].cnt[i] = t[u].cnt[i - 1];
            t[u].cnt[p] = 1;
            ++t[u].num;
            ++t[u].sz;
            return;
        }
        if (t[t[u].ch[p]].num == MAX_KEYS) {
            split_child(u, p);
            if (t[u].key[p] == x) {
                ++t[u].cnt[p];
                ++t[u].sz;
                return;
            }
            if (t[u].key[p] < x) ++p;
        }
        int v = t[u].ch[p];
        insert_non_full(v, x);
        t[u].sub[p] = t[v].sz;
        ++t[u].sz;
    }
    int erase_leaf_key(int u, int p) {
        int x = t[u].key[p], removed = t[u].cnt[p];
        erase_key_avx2(t[u].key, x);
        for (int i = p; i + 1 < t[u].num; ++i) t[u].cnt[i] = t[u].cnt[i + 1];
        t[u].cnt[t[u].num - 1] = 0;
        --t[u].num;
        t[u].sz -= removed;
        return removed;
    }
    pair<int, int> min_entry(int u) const {
        while (!t[u].leaf) u = t[u].ch[0];
        return {t[u].key[0], t[u].cnt[0]};
    }
    pair<int, int> max_entry(int u) const {
        while (!t[u].leaf) u = t[u].ch[t[u].num];
        return {t[u].key[t[u].num - 1], t[u].cnt[t[u].num - 1]};
    }
    void borrow_left(int u, int p) {
        int v = t[u].ch[p], w = t[u].ch[p - 1];
        int vn = t[v].num, wn = t[w].num;
        for (int i = vn; i > 0; --i) t[v].key[i] = t[v].key[i - 1], t[v].cnt[i] = t[v].cnt[i - 1];
        if (!t[v].leaf) {
            for (int i = vn + 1; i > 0; --i) t[v].ch[i] = t[v].ch[i - 1], t[v].sub[i] = t[v].sub[i - 1];
            t[v].ch[0] = t[w].ch[wn];
            t[v].sub[0] = t[w].sub[wn];
            t[w].ch[wn] = t[w].sub[wn] = 0;
        }
        t[v].key[0] = t[u].key[p - 1];
        t[v].cnt[0] = t[u].cnt[p - 1];
        t[u].key[p - 1] = t[w].key[wn - 1];
        t[u].cnt[p - 1] = t[w].cnt[wn - 1];
        t[w].key[wn - 1] = INT_MAX;
        t[w].cnt[wn - 1] = 0;
        ++t[v].num;
        --t[w].num;
        pull(v), pull(w);
        t[u].sub[p - 1] = t[w].sz;
        t[u].sub[p] = t[v].sz;
        pull(u);
    }
    void borrow_right(int u, int p) {
        int v = t[u].ch[p], w = t[u].ch[p + 1];
        int vn = t[v].num, wn = t[w].num;
        t[v].key[vn] = t[u].key[p];
        t[v].cnt[vn] = t[u].cnt[p];
        if (!t[v].leaf) {
            t[v].ch[vn + 1] = t[w].ch[0];
            t[v].sub[vn + 1] = t[w].sub[0];
        }
        t[u].key[p] = t[w].key[0];
        t[u].cnt[p] = t[w].cnt[0];
        for (int i = 0; i + 1 < wn; ++i) t[w].key[i] = t[w].key[i + 1], t[w].cnt[i] = t[w].cnt[i + 1];
        t[w].key[wn - 1] = INT_MAX;
        t[w].cnt[wn - 1] = 0;
        if (!t[w].leaf) {
            for (int i = 0; i < wn; ++i) t[w].ch[i] = t[w].ch[i + 1], t[w].sub[i] = t[w].sub[i + 1];
            t[w].ch[wn] = t[w].sub[wn] = 0;
        }
        ++t[v].num;
        --t[w].num;
        pull(v), pull(w);
        t[u].sub[p] = t[v].sz;
        t[u].sub[p + 1] = t[w].sz;
        pull(u);
    }
    void merge_right(int u, int p) {
        int v = t[u].ch[p], w = t[u].ch[p + 1];
        int vn = t[v].num, wn = t[w].num, un = t[u].num;
        t[v].key[vn] = t[u].key[p];
        t[v].cnt[vn] = t[u].cnt[p];
        for (int i = 0; i < wn; ++i) {
            t[v].key[vn + 1 + i] = t[w].key[i];
            t[v].cnt[vn + 1 + i] = t[w].cnt[i];
        }
        if (!t[v].leaf) {
            for (int i = 0; i <= wn; ++i) {
                t[v].ch[vn + 1 + i] = t[w].ch[i];
                t[v].sub[vn + 1 + i] = t[w].sub[i];
            }
        }
        t[v].num = vn + 1 + wn;
        pull(v);
        for (int i = p; i + 1 < un; ++i) t[u].key[i] = t[u].key[i + 1], t[u].cnt[i] = t[u].cnt[i + 1];
        t[u].key[un - 1] = INT_MAX;
        t[u].cnt[un - 1] = 0;
        for (int i = p + 1; i < un; ++i) t[u].ch[i] = t[u].ch[i + 1], t[u].sub[i] = t[u].sub[i + 1];
        t[u].ch[un] = t[u].sub[un] = 0;
        --t[u].num;
        t[u].sub[p] = t[v].sz;
        pull(u);
        recycle(w);
    }
    int fill_child(int u, int p) {
        if (p > 0 && t[t[u].ch[p - 1]].num > MIN_KEYS) {
            borrow_left(u, p);
            return p;
        }
        if (p < t[u].num && t[t[u].ch[p + 1]].num > MIN_KEYS) {
            borrow_right(u, p);
            return p;
        }
        if (p < t[u].num) {
            merge_right(u, p);
            return p;
        }
        merge_right(u, p - 1);
        return p - 1;
    }
    int erase_rec(int u, int x, bool all) {
        int p = lower_pos_avx2(t[u].key, x);
        if (p < t[u].num && t[u].key[p] == x) {
            if (!all && t[u].cnt[p] > 1) {
                --t[u].cnt[p];
                --t[u].sz;
                return 1;
            }
            if (t[u].leaf) return erase_leaf_key(u, p);
            int removed = all ? t[u].cnt[p] : 1;
            int l = t[u].ch[p], r = t[u].ch[p + 1];
            if (t[l].num > MIN_KEYS) {
                auto [v, c] = max_entry(l);
                t[u].key[p] = v;
                t[u].cnt[p] = c;
                erase_rec(l, v, true);
                t[u].sub[p] = t[l].sz;
                t[u].sz -= removed;
                return removed;
            }
            if (t[r].num > MIN_KEYS) {
                auto [v, c] = min_entry(r);
                t[u].key[p] = v;
                t[u].cnt[p] = c;
                erase_rec(r, v, true);
                t[u].sub[p + 1] = t[r].sz;
                t[u].sz -= removed;
                return removed;
            }
            merge_right(u, p);
            int v = t[u].ch[p];
            int d = erase_rec(v, x, all);
            t[u].sub[p] = t[v].sz;
            t[u].sz -= d;
            return d;
        }
        if (t[u].leaf) return 0;
        int q = p;
        if (t[t[u].ch[q]].num == MIN_KEYS) q = fill_child(u, q);
        int v = t[u].ch[q];
        int d = erase_rec(v, x, all);
        t[u].sub[q] = t[v].sz;
        t[u].sz -= d;
        return d;
    }
    void insert(int x) {
        if (!rt) {
            rt = new_node(true);
            t[rt].key[0] = x;
            t[rt].cnt[0] = 1;
            t[rt].num = t[rt].sz = 1;
            return;
        }
        if (t[rt].num == MAX_KEYS) {
            int old = rt;
            rt = new_node(false);
            t[rt].ch[0] = old;
            t[rt].sub[0] = t[old].sz;
            split_child(rt, 0);
        }
        insert_non_full(rt, x);
    }
    void erase_one(int x) {
        if (!rt) return;
        erase_rec(rt, x, false);
        if (t[rt].num) return;
        int old = rt;
        if (t[rt].leaf) rt = 0;
        else rt = t[rt].ch[0];
        recycle(old);
    }
    void erase_all(int x) {
        if (!rt) return;
        erase_rec(rt, x, true);
        if (t[rt].num) return;
        int old = rt;
        if (t[rt].leaf) rt = 0;
        else rt = t[rt].ch[0];
        recycle(old);
    }
    void clear() { rt = bin = ptr = 0; }
    int size() const { return rt ? t[rt].sz : 0; }
    bool find(int x) const {
        int u = rt;
        while (u) {
            int p = lower_pos_avx2(t[u].key, x);
            if (p < t[u].num && t[u].key[p] == x) return true;
            if (t[u].leaf) return false;
            int v = t[u].ch[p];
            __builtin_prefetch(&t[v]);
            u = v;
        }
        return false;
    }
    int count(int x) const {
        int u = rt;
        while (u) {
            int p = lower_pos_avx2(t[u].key, x);
            if (p < t[u].num && t[u].key[p] == x) return t[u].cnt[p];
            if (t[u].leaf) return 0;
            u = t[u].ch[p];
        }
        return 0;
    }
    int lt_cnt(int x) const {
        int u = rt, res = 0;
        while (u) {
            int p = lower_pos_avx2(t[u].key, x);
            for (int i = 0; i < p; ++i) res += t[u].sub[i] + t[u].cnt[i];
            if (p < t[u].num && t[u].key[p] == x) return res + t[u].sub[p];
            if (t[u].leaf) return res;
            u = t[u].ch[p];
        }
        return res;
    }
    int le_cnt(int x) const {
        int u = rt, res = 0;
        while (u) {
            int p = lower_pos_avx2(t[u].key, x);
            for (int i = 0; i < p; ++i) res += t[u].sub[i] + t[u].cnt[i];
            if (p < t[u].num && t[u].key[p] == x) return res + t[u].sub[p] + t[u].cnt[p];
            if (t[u].leaf) return res;
            u = t[u].ch[p];
        }
        return res;
    }
    int cnt_range(int l, int r) const {
        if (r < l) return 0;
        return le_cnt(r) - lt_cnt(l);
    }
    int rank(int x) const { return lt_cnt(x) + 1; }
    int kth(int k) const {
        int u = rt;
        while (u) {
            bool go = false;
            for (int i = 0; i < t[u].num; ++i) {
                if (k <= t[u].sub[i]) {
                    u = t[u].ch[i];
                    go = true;
                    break;
                }
                k -= t[u].sub[i];
                if (k <= t[u].cnt[i]) return t[u].key[i];
                k -= t[u].cnt[i];
            }
            if (go) continue;
            if (t[u].leaf) return 0;
            u = t[u].ch[t[u].num];
        }
        return 0;
    }
    int min_val() const {
        int u = rt;
        while (!t[u].leaf) u = t[u].ch[0];
        return t[u].key[0];
    }
    int max_val() const {
        int u = rt;
        while (!t[u].leaf) u = t[u].ch[t[u].num];
        return t[u].key[t[u].num - 1];
    }
    int lower_bound(int x) const {
        int u = rt, res = numeric_limits<int>::max();
        while (u) {
            int p = lower_pos_avx2(t[u].key, x);
            if (p < t[u].num) res = t[u].key[p];
            if (t[u].leaf) break;
            u = t[u].ch[p];
        }
        return res;
    }
    int upper_bound(int x) const {
        int u = rt, res = numeric_limits<int>::max();
        while (u) {
            int p = lower_pos_avx2(t[u].key, x);
            if (p < t[u].num && t[u].key[p] == x) ++p;
            if (p < t[u].num) res = t[u].key[p];
            if (t[u].leaf) break;
            u = t[u].ch[p];
        }
        return res;
    }
    int pre_val(int x) const {
        int u = rt, res = numeric_limits<int>::lowest();
        while (u) {
            int p = lower_pos_avx2(t[u].key, x);
            if (p) res = t[u].key[p - 1];
            if (t[u].leaf) break;
            u = t[u].ch[p];
        }
        return res;
    }
    int nxt_val(int x) const { return upper_bound(x); }
};