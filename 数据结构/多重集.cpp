#include <bits/stdc++.h>
using namespace std;

/**
 * 缓冲分块有序集 / Buffered Multiset【高性能动态有序多重集合】
 *
 * 索引 / 区间约定：
 * 内部数组 0-based；rank / kth 为 1-based；cnt_range(l, r) 统计闭区间 [l, r]。
 *
 * 支持操作：
 * insert(x)：插入一个 x
 * erase_one(x)：删除一个 x
 * erase_all(x)：删除所有 x
 * clear()：清空
 * size()：返回元素总数，包含重复元素
 * find(x)：判断 x 是否存在
 * count(x)：返回 x 的出现次数
 * lt_cnt(x)：返回严格 <x 的元素个数
 * le_cnt(x)：返回 <=x 的元素个数
 * cnt_range(l, r)：返回 [l, r] 内元素个数
 * rank(x)：返回 x 的排名，1-based
 * kth(k)：返回第 k 小，1-based
 * min_val() / max_val()：返回最小值 / 最大值，要求非空
 * lower_bound(x) / upper_bound(x)：无解返回 numeric_limits<int>::max()
 * pre_val(x)：无解返回 numeric_limits<int>::lowest()
 * nxt_val(x)：无解返回 numeric_limits<int>::max()
 *
 * 适用：
 * int 动态有序多重集合；大量插入 / 删除可通过缓冲和整体重建降低常数。
 * 单个对象约占二十余 MB，建议定义为全局或 static（static buffered_multiset tr;）；但注意多测时要记得提前 clear()。
 *
 * 复杂度：
 * insert / erase_one 先 O(1) 加入缓冲。
 * 查询前自动处理缓冲；小批修改逐个应用，大批修改使用 radix sort + O(n) 重建。
 * 查询本身约 O(log n)，但单次查询可能触发整体重建。
 */
struct buffered_multiset {
    static constexpr int block_half = 100;
    static constexpr int block_capacity = block_half * 2;
    static constexpr int max_blocks = 12005;
    static constexpr int group_shift = 3;
    static constexpr int group_size = 1 << group_shift;
    static constexpr int max_pending = 1100005;
    int buf[max_blocks][block_capacity + 2];
    short block_id[max_blocks], free_block[max_blocks];
    unsigned char block_len[max_blocks];
    int bit[(max_blocks + group_size - 1) / group_size + 2];
    int group_max[(max_blocks + group_size - 1) / group_size + 2];
    int insert_buf[max_pending], erase_buf[max_pending], tmp_buf[max_pending];
    unsigned radix_cnt[4][256];
    int block_cnt, pool_cnt, free_cnt;
    int committed_size, insert_cnt, erase_cnt;
    buffered_multiset() { clear(); }
    int* block(int i) { return buf[block_id[i]]; }
    const int* block(int i) const { return buf[block_id[i]]; }
    static int msb(int x) { return 1u << (31 - __builtin_clz((unsigned)x)); }
    int alloc_block() { return free_cnt ? free_block[--free_cnt] : pool_cnt++; }
    void reset_blocks() {
        pool_cnt = free_cnt = 0;
        block_cnt = 1;
        block_id[0] = alloc_block();
        block_len[0] = 0;
    }
    void build_bit() {
        int groups = (block_cnt + group_size - 1) >> group_shift;
        memset(bit, 0, groups * sizeof(int));
        for (int g = 0; g < groups; ++g) {
            int s = 0, l = g << group_shift, r = min(l + group_size, block_cnt);
            for (int i = l; i < r; ++i) s += block_len[i];
            bit[g] = s;
        }
        for (int i = 0; i < groups; ++i) {
            int j = i | (i + 1);
            if (j < groups) bit[j] += bit[i];
        }
    }
    void build_group_max(int from = 0) {
        int groups = block_cnt >> group_shift;
        for (int g = from; g < groups; ++g) {
            int b = ((g + 1) << group_shift) - 1;
            group_max[g] = block(b)[block_len[b] - 1];
        }
    }
    void bit_add(int b, int d) {
        int groups = (block_cnt + group_size - 1) >> group_shift;
        for (int i = b >> group_shift; i < groups; i |= i + 1) bit[i] += d;
    }
    int bit_sum(int groups) const {
        int res = 0;
        for (int i = groups - 1; i >= 0; i = (i & (i + 1)) - 1) res += bit[i];
        return res;
    }
    pair<int, int> lower_pos(int x) const {
        if (!block_len[0]) return {0, 0};
        int last = block_cnt - 1;
        if (x > block(last)[block_len[last] - 1]) return {last, block_len[last]};
        int passed = 0, groups = block_cnt >> group_shift;
        if (groups) {
            for (int d = msb(groups); d; d >>= 1) {
                int nxt = passed + d;
                if (nxt <= groups && group_max[nxt - 1] < x) passed = nxt;
            }
        }
        int b = passed << group_shift;
        while (block(b)[block_len[b] - 1] < x) ++b;
        int p = 0, n = block_len[b];
        const int* a = block(b);
        for (int d = msb(n); d; d >>= 1) {
            int nxt = p + d;
            if (nxt <= n && a[nxt - 1] < x) p = nxt;
        }
        return {b, p};
    }
    pair<int, int> upper_pos(int x) const {
        if (!block_len[0]) return {0, 0};
        int last = block_cnt - 1;
        if (x >= block(last)[block_len[last] - 1]) return {last, block_len[last]};
        int passed = 0, groups = block_cnt >> group_shift;
        if (groups) {
            for (int d = msb(groups); d; d >>= 1) {
                int nxt = passed + d;
                if (nxt <= groups && group_max[nxt - 1] <= x) passed = nxt;
            }
        }
        int b = passed << group_shift;
        while (block(b)[block_len[b] - 1] <= x) ++b;
        int p = 0, n = block_len[b];
        const int* a = block(b);
        for (int d = msb(n); d; d >>= 1) {
            int nxt = p + d;
            if (nxt <= n && a[nxt - 1] <= x) p = nxt;
        }
        return {b, p};
    }
    int pos_cnt(pair<int, int> p) const {
        int g = p.first >> group_shift, res = bit_sum(g);
        for (int i = g << group_shift; i < p.first; ++i) res += block_len[i];
        return res + p.second;
    }
    pair<int, int> kth_pos(int k) const {
        int groups = (block_cnt + group_size - 1) >> group_shift;
        int p = -1, sum = 0;
        for (int d = msb(groups); d; d >>= 1) {
            int nxt = p + d;
            if (nxt < groups && sum + bit[nxt] < k) sum += bit[nxt], p = nxt;
        }
        int b = (p + 1) << group_shift;
        k -= sum;
        while (k > block_len[b]) k -= block_len[b++];
        return {b, k - 1};
    }
    void insert_at(pair<int, int> p, int x) {
        int b = p.first, pos = p.second, n = block_len[b];
        int* a = block(b);
        memmove(a + pos + 1, a + pos, (n - pos) * sizeof(int));
        a[pos] = x;
        ++block_len[b];
        if ((b & 7) == 7) group_max[b >> 3] = a[block_len[b] - 1];
        bit_add(b, 1);
        if (block_len[b] != block_capacity) return;
        int nb = b + 1;
        memmove(block_id + nb + 1, block_id + nb, (block_cnt - nb) * sizeof(short));
        memmove(block_len + nb + 1, block_len + nb, (block_cnt - nb) * sizeof(unsigned char));
        ++block_cnt;
        block_id[nb] = alloc_block();
        block_len[b] = block_len[nb] = block_half;
        memcpy(block(nb), block(b) + block_half, block_half * sizeof(int));
        build_group_max(b >> group_shift);
        build_bit();
    }
    void erase_at(pair<int, int> p) {
        int b = p.first, pos = p.second, n = block_len[b];
        int* a = block(b);
        memmove(a + pos, a + pos + 1, (n - pos - 1) * sizeof(int));
        --block_len[b];
        bit_add(b, -1);
        if (block_len[b]) {
            if ((b & 7) == 7) group_max[b >> 3] = a[block_len[b] - 1];
            return;
        }
        if (block_cnt == 1) return;
        free_block[free_cnt++] = block_id[b];
        memmove(block_id + b, block_id + b + 1, (block_cnt - b - 1) * sizeof(short));
        memmove(block_len + b, block_len + b + 1, (block_cnt - b - 1) * sizeof(unsigned char));
        --block_cnt;
        build_group_max(b >> group_shift);
        build_bit();
    }
    void radix_sort(int* a, int n) {
        auto pass = [&](int* src, int* dst, unsigned* cnt, int shift, bool sign) {
            memset(cnt, 0, 256 * sizeof(unsigned));
            for (int i = 0; i < n; ++i) {
                unsigned v = (unsigned)src[i];
                unsigned k = sign ? ((v >> shift) ^ 0x80) & 255 : (v >> shift) & 255;
                ++cnt[k];
            }
            for (int i = 1; i < 256; ++i) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; --i) {
                unsigned v = (unsigned)src[i];
                unsigned k = sign ? ((v >> shift) ^ 0x80) & 255 : (v >> shift) & 255;
                dst[--cnt[k]] = src[i];
            }
        };
        pass(a, tmp_buf, radix_cnt[0], 0, false);
        pass(tmp_buf, a, radix_cnt[1], 8, false);
        pass(a, tmp_buf, radix_cnt[2], 16, false);
        pass(tmp_buf, a, radix_cnt[3], 24, true);
    }
    void apply_pending() {
        committed_size += insert_cnt - erase_cnt;
        while (insert_cnt) {
            int x = insert_buf[--insert_cnt];
            insert_at(lower_pos(x), x);
        }
        while (erase_cnt) {
            int x = erase_buf[--erase_cnt];
            erase_at(lower_pos(x));
        }
    }
    void rebuild() {
        bool bulk_erase = committed_size + 40 * block_half < erase_cnt * 2;
        radix_sort(insert_buf, insert_cnt);
        if (bulk_erase) radix_sort(erase_buf, erase_cnt);
        int ii = 0, di = 0, out = 0;
        if (bulk_erase) {
            for (int i = 0; i < block_cnt; ++i) {
                const int* a = block(i);
                for (int j = 0; j < block_len[i]; ++j) {
                    int x = a[j];
                    while (ii < insert_cnt && insert_buf[ii] <= x) {
                        if (di < erase_cnt && insert_buf[ii] == erase_buf[di]) ++ii, ++di;
                        else tmp_buf[out++] = insert_buf[ii++];
                    }
                    if (di < erase_cnt && x == erase_buf[di]) ++di;
                    else tmp_buf[out++] = x;
                }
            }
            while (ii < insert_cnt) {
                if (di < erase_cnt && insert_buf[ii] == erase_buf[di]) ++ii, ++di;
                else tmp_buf[out++] = insert_buf[ii++];
            }
            committed_size = out;
        } else {
            for (int i = 0; i < block_cnt; ++i) {
                const int* a = block(i);
                for (int j = 0; j < block_len[i]; ++j) {
                    int x = a[j];
                    while (ii < insert_cnt && insert_buf[ii] <= x) tmp_buf[out++] = insert_buf[ii++];
                    tmp_buf[out++] = x;
                }
            }
            while (ii < insert_cnt) tmp_buf[out++] = insert_buf[ii++];
            committed_size = out;
        }
        reset_blocks();
        for (int i = 0; i < out; ++i) {
            int b = block_cnt - 1;
            block(b)[block_len[b]++] = tmp_buf[i];
            if (block_len[b] == block_half && i + 1 < out) {
                block_id[block_cnt] = alloc_block();
                block_len[block_cnt++] = 0;
            }
        }
        build_group_max();
        build_bit();
        insert_cnt = 0;
        if (!bulk_erase) {
            committed_size -= erase_cnt;
            while (erase_cnt) {
                int x = erase_buf[--erase_cnt];
                erase_at(lower_pos(x));
            }
        }
        erase_cnt = 0;
    }
    void prepare() {
        if (!insert_cnt && !erase_cnt) return;
        if (committed_size + 40 * block_half < insert_cnt * 2) rebuild();
        else apply_pending();
    }
    void insert(int x) { insert_buf[insert_cnt++] = x; }
    void erase_one(int x) { erase_buf[erase_cnt++] = x; }
    void erase_all(int x) {
        int c = count(x);
        while (c--) erase_buf[erase_cnt++] = x;
    }
    void clear() {
        reset_blocks();
        committed_size = insert_cnt = erase_cnt = 0;
        build_bit();
    }
    int size() const { return committed_size + insert_cnt - erase_cnt; }
    bool find(int x) {
        prepare();
        auto p = lower_pos(x);
        return p.second < block_len[p.first] && block(p.first)[p.second] == x;
    }
    int count(int x) {
        prepare();
        return pos_cnt(upper_pos(x)) - pos_cnt(lower_pos(x));
    }
    int lt_cnt(int x) {
        prepare();
        return pos_cnt(lower_pos(x));
    }
    int le_cnt(int x) {
        prepare();
        return pos_cnt(upper_pos(x));
    }
    int cnt_range(int l, int r) {
        if (r < l) return 0;
        prepare();
        return pos_cnt(upper_pos(r)) - pos_cnt(lower_pos(l));
    }
    int rank(int x) {
        prepare();
        return pos_cnt(lower_pos(x)) + 1;
    }
    int kth(int k) {
        prepare();
        auto p = kth_pos(k);
        return block(p.first)[p.second];
    }
    int min_val() {
        prepare();
        return block(0)[0];
    }
    int max_val() {
        prepare();
        int b = block_cnt - 1;
        return block(b)[block_len[b] - 1];
    }
    int lower_bound(int x) {
        prepare();
        auto p = lower_pos(x);
        if (p.second == block_len[p.first]) return numeric_limits<int>::max();
        return block(p.first)[p.second];
    }
    int upper_bound(int x) {
        prepare();
        auto p = upper_pos(x);
        if (p.second == block_len[p.first]) return numeric_limits<int>::max();
        return block(p.first)[p.second];
    }
    int pre_val(int x) {
        prepare();
        auto p = lower_pos(x);
        if (p.second) return block(p.first)[p.second - 1];
        if (p.first) {
            int b = p.first - 1;
            return block(b)[block_len[b] - 1];
        }
        return numeric_limits<int>::lowest();
    }
    int nxt_val(int x) { return upper_bound(x); }
};