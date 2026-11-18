#include <bits/stdc++.h>
using namespace std;

/**
 * 随机异或哈希；每个元素随机分配 64-bit 值
 *
 * 支持操作：
 * get(a)：计算集合 a 的哈希，O(|a|)
 * toggle(h, x)：切换元素 x 的存在状态，O(1)
 *
 * 适用：集合判等、动态集合维护、集合分组。不同集合的单次哈希碰撞概率为 2^-64
 * 复杂度：O(n)
 */
struct xorHash {
    using u64 = uint64_t;
    vector<u64> w;
    xorHash(int n) : w(n) {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        for (auto& x : w) x = rng();
    }
    u64 get(const vector<int>& a) const {
        u64 h = 0;
        for (int x : a) h ^= w[x];
        return h;
    }
    void toggle(u64& h, int x) const { h ^= w[x]; }
};

/**
 * 前缀哈希
 * 
 * 返回数组 a 的前缀哈希，去重集合语义；用于判断前缀值域范围是否相同
 * 复杂度：O(n)
 */
vector<uint64_t> preHash(vector<int>& a) {
    using u64 = uint64_t;
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    int n = a.size();
    vector<u64> h(n);
    unordered_map<int, u64> mp;
    unordered_set<int> has;
    for (int i = 0; i < n; ++i) {
        if (has.count(a[i])) {
            h[i] = h[i - 1];
            continue;
        }
        if (!mp.count(a[i])) mp[a[i]] = rng();
        h[i] = (i ? h[i - 1] : 0) ^ mp[a[i]];
        has.insert(a[i]);
    }
    return h;
}

/**
 * Hyperregular Trick
 *
 * 索引：位置与区间 ID 均为 0-based，区间为闭区间 [l, r]。每个区间分配独立随机 64-bit 权值，通过异或差分计算每个位置被覆盖的区间集合哈希。
 * 返回 h，其中 h[i] 表示覆盖位置 i 的区间 ID 集合哈希 = XOR_{区间id覆盖i}(w[id])
 *
 * 复杂度：O(n + m)
 */
vector<uint64_t> build_hash(int n, const vector<pair<int, int>>& seg) {
    using u64 = uint64_t;
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    vector<u64> diff(n + 1), h(n);
    for (auto [l, r] : seg) {
        u64 x = rng();
        diff[l] ^= x;
        diff[r + 1] ^= x;
    }
    u64 cur = 0;
    for (int i = 0; i < n; i++) {
        cur ^= diff[i];
        h[i] = cur;
    }
    return h;
}