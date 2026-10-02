#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

// 最低位 1 / 最高位 1 的下标
inline pair<int, int> get(int mask) {
    int l = __builtin_ctz(mask), r = __lg(mask);
    return {l, r};
}

// 反转比特序（32 / 64 位）：b31...b0 -> b0...b31
inline uint32_t rev32(uint32_t x) {
    x = (x >> 1 & 0x55555555) | ((x & 0x55555555) << 1);
    x = (x >> 2 & 0x33333333) | ((x & 0x33333333) << 2);
    x = (x >> 4 & 0x0f0f0f0f) | ((x & 0x0f0f0f0f) << 4);
    x = (x >> 8 & 0x00ff00ff) | ((x & 0x00ff00ff) << 8);
    x = (x >> 16) | (x << 16);
    return x;
}
inline uint64_t rev64(uint64_t x) {
    x = (x >> 1 & 0x5555555555555555ULL) | ((x & 0x5555555555555555ULL) << 1);
    x = (x >> 2 & 0x3333333333333333ULL) | ((x & 0x3333333333333333ULL) << 2);
    x = (x >> 4 & 0x0f0f0f0f0f0f0f0fULL) | ((x & 0x0f0f0f0f0f0f0f0fULL) << 4);
    x = (x >> 8 & 0x00ff00ff00ff00ffULL) | ((x & 0x00ff00ff00ff00ffULL) << 8);
    x = (x >> 16 & 0x0000ffff0000ffffULL) | ((x & 0x0000ffff0000ffffULL) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}
inline int reverseBits(int x) { return (int)rev32((uint32_t)x); }
inline ll reverseBitsll(ll x) { return (ll)rev64((uint64_t)x); }

// 1 的个数（popcount）：如 0b10110 -> 3
inline int popcount(unsigned x) { return __builtin_popcount(x); }
inline int popcountll(ull x) { return __builtin_popcountll(x); }
inline int popcount(int x) { return __builtin_popcount((unsigned)x); }
inline int popcount(ll x) { return __builtin_popcountll((ull)x); }

// trailing zeros（最低位 1 右边 0 的个数，x!=0）：如 ...1000 -> 3
inline int ctz(unsigned x) { return __builtin_ctz(x); }
inline int ctzll(ull x) { return __builtin_ctzll(x); }
inline int ctz(int x) { return __builtin_ctz((unsigned)x); }
inline int ctz(ll x) { return __builtin_ctzll((ull)x); }

// leading zeros（最高位 1 左边 0 的个数，x!=0）：如 0001xxxx -> 3
inline int clz(unsigned x) { return __builtin_clz(x); }
inline int clzll(ull x) { return __builtin_clzll(x); }
inline int clz(int x) { return __builtin_clz((unsigned)x); }
inline int clz(ll x) { return __builtin_clzll((ull)x); }

// bit 长度：最高位 1 的位置+1（x==0 返回 0），如 0b10110 -> 5
inline int bitlen(unsigned x) { return x ? 32 - __builtin_clz(x) : 0; }
inline int bitlen(ull x) { return x ? 64 - __builtin_clzll(x) : 0; }
inline int bitlen(int x) { return bitlen((unsigned)x); }
inline int bitlen(ll x) { return bitlen((ull)x); }

// floor(log2(x))，x>0：最高位 1 的下标，如 0b10110 -> 4
inline int flog2(unsigned x) { return 31 - __builtin_clz(x); }
inline int flog2(ull x) { return 63 - __builtin_clzll(x); }
inline int flog2(int x) { return flog2((unsigned)x); }
inline int flog2(ll x) { return flog2((ull)x); }

// lowbit：最低位的 1 对应的 2^k：保留最低位 1，其余清 0，如 ...101000 -> ...001000
inline unsigned lowbit(unsigned x) { return x & -x; }
inline ull lowbit(ull x) { return x & -x; }
inline int lowbit(int x) { return x & -x; }
inline ll lowbit(ll x) { return x & -x; }

// 判断是否 2 的幂（严格正）：如 1,2,4,8,true；3,5,false
inline bool isPow2(unsigned x) { return x && (x & (x - 1)) == 0; }
inline bool isPow2(ull x) { return x && (x & (x - 1)) == 0; }
inline bool isPow2(int x) { return x > 0 && (x & (x - 1)) == 0; }
inline bool isPow2(ll x) { return x > 0 && (x & (x - 1)) == 0; }

// 按位取反（限定宽度的 mask 版本）：只翻转低 w 位，其余保持原样
inline unsigned bitnot(unsigned x, int w = 32) {
    if (w >= 32) return ~x;
    unsigned mask = (w == 32 ? ~0u : ((1u << w) - 1));
    return (~x) & mask;
}
inline ull bitnotll(ull x, int w = 64) {
    if (w >= 64) return ~x;
    ull mask = (w == 64 ? ~0ull : ((1ull << w) - 1));
    return (~x) & mask;
}

// 取第 k 位（从 0 开始）：如 x=...01010, k=1 -> 1；k=2 -> 0
inline int getbit(ull x, int k) { return (x >> k) & 1ull; }
inline int getbit(ll x, int k) { return (int)((((ull)x) >> k) & 1ull); }

// 把第 k 位置 1：...0... -> ...1...
inline ull setbit(ull x, int k) { return x | (1ull << k); }
inline ll setbit(ll x, int k) { return (ll)(((ull)x) | (1ull << k)); }

// 把第 k 位置 0：...1... -> ...0...
inline ull clearbit(ull x, int k) { return x & ~(1ull << k); }
inline ll clearbit(ll x, int k) { return (ll)(((ull)x) & ~(1ull << k)); }

// 翻转第 k 位：...0... <-> ...1...
inline ull flipbit(ull x, int k) { return x ^ (1ull << k); }
inline ll flipbit(ll x, int k) { return (ll)(((ull)x) ^ (1ull << k)); }

// ----------------------------------------------------------------------------------------------------
// 找到最近的 2 的幂【重要】；返回的是0-based下标的位置。若需要具体值，用 1<<m

// 最小 m，使 2^m>=n
int ceil_log2(int n) { return n == 1 ? 0 : __lg(n - 1) + 1; }

// 最小 m，使 2^m>n
int next_log2(int n) { return __lg(n) + 1; }

// 最大 m，使 2^m<n
int prev_log2(int n) { return n == 1 ? -1 : __lg(n - 1); }

// 最大 m，使 2^m<=n
// 应用1. 给定区间 [l,r]，之内的数在 m=__lg(l^r) 之上的位完全相同（即 m 为区间中最高允许不同的位）
// 应用2. 对于 [0,n] 中的所有数，m=__lg(n)，则 n 的最高位1是第m位，且[0,n]中的所有数的可用bit不超过第 m 位，即任意数之间的位运算结果的首1最高在第 m 位
int floor_log2(int n) { return __lg(n); }

// ----------------------------------------------------------------------------------------------------

// 去掉最低位的 1：...101000 -> ...100000
inline unsigned clear_lowbit(unsigned x) { return x & (x - 1); }
inline ull clear_lowbit(ull x) { return x & (x - 1); }

// 保留从低位开始连续的 1（其余清 0）：...xxx0111 -> ...0000111
inline unsigned keep_trailing_ones(unsigned x) { return x ^ (x + 1); }
inline ull keep_trailing_ones(ull x) { return x ^ (x + 1); }

// 低 k 位的 mask：k=5 -> ...00011111
inline unsigned mask_low(int k) {
    if (k <= 0) return 0u;
    if (k >= 32) return ~0u;
    return (1u << k) - 1;
}
inline ull mask_lowll(int k) {
    if (k <= 0) return 0ull;
    if (k >= 64) return ~0ull;
    return (1ull << k) - 1;
}

// [l, r] 之间的比特全部置 1：例如 l=2,r=5 -> ...00111100
inline ull mask_range(int l, int r) {
    return mask_lowll(r + 1) ^ mask_lowll(l);
}

// 抹掉从低位开始连续的一串 1：...xxx0111 -> ...xxx0000
inline unsigned clear_trailing_ones(unsigned x) { return x & (x + 1u); }
inline ull clear_trailing_ones(ull x) { return x & (x + 1ull); }

// 从低位到最低位 1 这一段全部变成 1：...xxx0100 -> ...xxx0111
inline unsigned ones_upto_lowbit(unsigned x) { return x ^ (x - 1u); }
inline ull ones_upto_lowbit(ull x) { return x ^ (x - 1ull); }

// x & ~y：清掉 x 中 y 为 1 的位：x=...1011, y=...1100 -> ...0011
inline unsigned clear_bits(unsigned x, unsigned y) { return x & ~y; }
inline ull clear_bits(ull x, ull y) { return x & ~y; }

// ~x & y：只取 y 中 x 为 0 的位：x=...1011, y=...1101 -> ...0100
inline unsigned keep_zero_bits(unsigned x, unsigned y) { return (~x) & y; }
inline ull keep_zero_bits(ull x, ull y) { return (~x) & y; }
