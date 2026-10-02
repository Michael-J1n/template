#include <bits/stdc++.h>
using namespace std;

/**
 * 区间分块模板 1（不需要维护排序块）
 * 此模板以区间加、区间和查询为例
 * 
 * 将数列分为每块大小为 B 的块，对于 [l,r] 的操作，左右散块暴力，中间整块使用懒标记
 * 
 * 复杂度：初始化 O(n)；修改/查询 O(B + n / B)
 * 
 * 注意：
 * 1. 查询整块时记住考虑 tag[b] 带来的额外信息
 * 2. 初始化记得要用 a 数组初始化信息
 */
struct blockDecomposition1 {
    int n, B, m;                            // n: array length; B: block size; m: block number
    vector<long long> a;                    // a: maintained array without lazy tags
    vector<long long> sum;                  // sum: sum of a in each block without lazy tags
    vector<long long> tag;                  // tag: lazy add tag for each block
    // element id -> block id: i -> i/B
    // block b's range: [b * B, min(n - 1, (b + 1) * B - 1)]
    blockDecomposition1(const vector<long long>& nums) {
        n = nums.size();
        B = sqrt(n) + 1;
        m = (n + B - 1) / B;
        a = nums;
        sum.assign(m, 0);
        tag.assign(m, 0);
        for (int i = 0; i < n; ++i) {
            int id = i / B;
            sum[id] += a[i];
        }
    }
    void apply(int i, long long v) {
        int id = i / B;
        a[i] += v;
        sum[id] += v;
    }
    void add(int l, int r, long long v) {
        int bl = l / B, br = r / B;
        if (bl == br) {
            for (int i = l; i <= r; ++i) apply(i, v);
        } else {
            for (int i = l; i < (bl + 1) * B; ++i) apply(i, v);
            for (int b = bl + 1; b < br; ++b) tag[b] += v;
            for (int i = br * B; i <= r; ++i) apply(i, v);
        }
    }
    long long query(int l, int r) {
        int bl = l / B, br = r / B;
        long long res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) res += a[i] + tag[bl];
        } else {
            for (int i = l; i < (bl + 1) * B; ++i) res += a[i] + tag[bl];
            for (int b = bl + 1; b < br; ++b) {
                int len = min(n, (b + 1) * B) - b * B;
                res += sum[b] + tag[b] * len;
            }
            for (int i = br * B; i <= r; ++i) res += a[i] + tag[br];
        }
        return res;
    }
};

/**
 * 区间分块模板 2（需要维护排序块）
 * 此模板以查询 [l, r] 内在值域 [x, y] 内的元素个数计数为例
 * 
 * 将数列分为每块大小为 B 的块，对于 [l,r] 的操作，左右散块暴力，中间整块使用懒标记
 * 
 * 复杂度：初始化 O(n log B)；修改 O(B log B)，查询 O(B + (n/B) log B)
 * 
 * 注意：
 * 1. 查询时记住考虑 tag[b] 带来的额外信息
 * 2. 初始化记得要用 a 数组初始化信息
 */
struct blockDecomposition2 {
    int n, B, m;                            // n:   array length; B: block size; m: block number; 
    vector<long long> a;                    // a:   maintained array
    vector<long long> tag;                  // tag: lazy tag for each block
    vector<vector<long long>> s;            // s:   sorted list for each block
    // element id -> block id: i -> i/B
    // block b's range: [b * B, min( n - 1, (b + 1) * B - 1 )]
    blockDecomposition2(const vector<long long>& nums) {
        n = nums.size();
        B = sqrt(n) + 1;
        m = (n + B - 1) / B;
        a = nums;
        tag.assign(m, 0);
        s.resize(m);
        for (int i = 0; i < m; ++i) s[i].reserve(B);
        for (int i = 0; i < n; ++i) {
            int id = i / B;
            s[id].emplace_back(a[i]);
        }
        for (int i = 0; i < m; ++i) ranges::sort(s[i]);
    }
    void rebuild(int b) {
        auto& vec = s[b];
        vec.clear();
        for (int i = b * B; i < min(n, (b + 1) * B); ++i) vec.emplace_back(a[i]);
        ranges::sort(vec);
    }
private:
    void apply(int i, long long v) {  // note: after making brutely apply on single points, must rebuild the partial block
        // int b = i / B
        a[i] += v;   
    }
public:
    void add(int l, int r, long long v) {
        int bl = l / B, br = r / B;
        if (bl == br) {
            for (int i = l; i <= r; ++i) apply(i, v);
            rebuild(bl);
        } else {
            for (int i = l; i < (bl + 1) * B; ++i) apply(i, v);
            rebuild(bl);
            for (int b = bl + 1; b < br; ++b) tag[b] += v;
            for (int i = br * B; i <= r; ++i) apply(i, v);
            rebuild(br);
        }
    }
    bool check(int i, long long x, long long y) {
        int b = i / B;
        long long cur = a[i] + tag[b];
        return x <= cur && cur <= y;
    }
    int query(int l, int r, long long x, long long y) {  // note: when answering queries, consider the tag information!
        int bl = l / B, br = r / B;
        int res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) res += check(i, x, y);
        } else {
            for (int i = l; i < (bl + 1) * B; ++i) res += check(i, x, y);
            for (int b = bl + 1; b < br; ++b) {
                auto& vec = s[b];
                int L = ranges::lower_bound(vec, x - tag[b]) - vec.begin();
                int R = ranges::upper_bound(vec, y - tag[b]) - vec.begin() - 1;
                if (L <= R) res += R - L + 1;
            }
            for (int i = br * B; i <= r; ++i) res += check(i, x, y);
        }
        return res;
    }
};
