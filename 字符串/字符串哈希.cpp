#include <bits/stdc++.h>
using namespace std;

/**
 * 双模随机字符串哈希【子串判等】
 * StringHash(s) 预处理字符串 s，get(l, r) 返回闭区间 [l, r] 的双哈希值。
 * 所有实例共享同一个随机 base，可直接比较不同字符串中等长子串的哈希值。
 *
 * 适用：快速判断子串是否相等，以及结合二分进行 LCP、回文、字符串匹配等问题。
 *
 * 复杂度：预处理 O(n)，单次 get O(1)
 */
using u64 = uint64_t;
struct StringHash {
    static const int mod1 = 1000000007;
    static const int mod2 = 1000000009;
    inline static vector<int> p1 = {1};
    inline static vector<int> p2 = {1};
    static int _get() {
        static const int base = []() {
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            return uniform_int_distribution<int>(256, mod1 - 1)(rng);
        }();
        return base;
    }
    static void ensure(int n) {
        int base = _get();
        while (p1.size() <= n) p1.emplace_back(1LL * p1.back() * base % mod1), p2.emplace_back(1LL * p2.back() * base % mod2);
    }
    vector<int> h1, h2;
    StringHash(const string& s) {
        int n = s.size(), base = _get();
        ensure(n);
        h1.resize(n); h2.resize(n);
        for (int i = 0; i < n; ++i) {
            int x = (unsigned char)s[i] + 1;
            if (!i) h1[i] = x, h2[i] = x;
            else h1[i] = (1LL * h1[i - 1] * base + x) % mod1, h2[i] = (1LL * h2[i - 1] * base + x) % mod2;
        }
    }
    StringHash(const vector<int>& a) {
        int n = a.size(), base = _get();
        ensure(n);
        h1.resize(n); h2.resize(n);
        for (int i = 0; i < n; ++i) {
            int x1 = (a[i] % mod1 + mod1) % mod1 + 1;
            int x2 = (a[i] % mod2 + mod2) % mod2 + 1;
            if (x1 == mod1) x1 = 0;
            if (x2 == mod2) x2 = 0;
            if (!i) h1[i] = x1, h2[i] = x2;
            else h1[i] = (1LL * h1[i - 1] * base + x1) % mod1, h2[i] = (1LL * h2[i - 1] * base + x2) % mod2;
        }
    }
    pair<int, int> getp(int l, int r) const {
        long long x1 = h1[r], x2 = h2[r];
        if (l) {
            int len = r - l + 1;
            x1 -= 1LL * h1[l - 1] * p1[len] % mod1; if (x1 < 0) x1 += mod1;
            x2 -= 1LL * h2[l - 1] * p2[len] % mod2; if (x2 < 0) x2 += mod2;
        }
        return {(int)x1, (int)x2};
    }
    u64 get(int l, int r) {
        auto [x, y] = getp(l, r);
        return (uint64_t)(uint32_t)x << 32 | (uint32_t)y;
    }
};


/**
 * 使用字符串哈希的操作实例：
 * 
 * 基于 StringHash 维护原串与反串哈希，所有下标均为 0-based，区间均为闭区间 [l, r]。
 *
 * checkEqual(l1, r1, l2, r2)：判断当前字符串的两个子串 s[l1..r1] 与 s[l2..r2] 是否相等。
 * checkEqual(l1, r1, l2, r2, t)：判断当前字符串的子串 s[l1..r1] 与另一字符串的子串 t[l2..r2] 是否相等。
 * LCP(i, j)：返回两个后缀 s[i..] 与 s[j..] 的最长公共前缀长度。
 * checkPalindrome(l, r)：判断子串 s[l..r] 是否为回文串。
 * match(t)：返回模式串 t 在 s 中所有完整匹配的起始位置。
 * checkExist(len)：判断 s 中是否存在两个相同的长度为 len 的子串，两个子串允许重叠。
 * maxRepeat()：返回 s 中最长重复子串的长度，重复出现的两个子串允许重叠。
 * checkPeriod(k)：判断 k 是否为 s 的周期，即对所有合法 i 均满足 s[i] = s[i + k]。不要求 n 能被 k 整除。
 * checkRepeatPrefix(k)：判断 s 是否恰好由长度为 k 的前缀重复若干次构成。
 * compare(l1, r1, l2, r2)：按字典序比较 s[l1..r1] 与 s[l2..r2]。返回 -1 表示第一个子串更小，0 表示相等，1 表示第一个子串更大。
 *
 * 复杂度：初始化 O(n)；
 * LCP/compare O(log n)；match O(n + |t|)；checkExist O(n log n)；maxRepeat O(n log^2 n)；其余查询 O(1)
 */
struct StringCheck {
    int n;
    string s;
    StringHash hs, hr;
    static string reversed(string s) {
        reverse(s.begin(), s.end());
        return s;
    }
    StringCheck(const string& _s) : n(_s.size()), s(_s), hs(s), hr(reversed(s)) {}
    bool checkEqual(int l1, int r1, int l2, int r2) {
        return r1 - l1 == r2 - l2 && hs.get(l1, r1) == hs.get(l2, r2);
    }
    bool checkEqual(int l1, int r1, int l2, int r2, const string& t) {
        StringHash ht(t);
        return r1 - l1 == r2 - l2 && hs.get(l1, r1) == ht.get(l2, r2);
    }
    int LCP(int i, int j) {
        int l = 0, r = n - max(i, j);
        while (l < r) {
            int mid = l + r + 1 >> 1;
            if (hs.get(i, i + mid - 1) == hs.get(j, j + mid - 1)) l = mid;
            else r = mid - 1;
        }
        return l;
    }
    bool checkPalindrome(int l, int r) {
        return hs.get(l, r) == hr.get(n - 1 - r, n - 1 - l);
    }
    vector<int> match(const string& t) {
        StringHash ht(t);
        int m = t.size();
        auto target = ht.get(0, m - 1);
        vector<int> res; res.reserve(n);
        for (int i = 0; i + m <= n; ++i) {
            if (hs.get(i, i + m - 1) == target) res.emplace_back(i);
        }
        return res;
    }
    bool checkExist(int len) {
        unordered_set<uint64_t> st;
        for (int i = 0; i + len <= n; ++i) {
            auto h = hs.get(i, i + len - 1);
            if (st.count(h)) return true;
            st.insert(h);
        }
        return false;
    }
    int maxRepeat() {
        int l = 0, r = n;
        while (l < r) {
            int mid = l + r + 1 >> 1;
            if (checkExist(mid)) l = mid;
            else r = mid - 1;
        }
        return l;
    }
    bool checkPeriod(int k) {
        if (k == n) return true;
        return hs.get(0, n - k - 1) == hs.get(k, n - 1);
    }
    bool checkRepeatPrefix(int k) {
        return n % k == 0 && checkPeriod(k);
    }
    int compare(int l1, int r1, int l2, int r2) {
        int len1 = r1 - l1 + 1, len2 = r2 - l2 + 1;
        int l = 0, r = min(len1, len2);
        while (l < r) {
            int mid = l + r + 1 >> 1;
            if (hs.get(l1, l1 + mid - 1) == hs.get(l2, l2 + mid - 1)) l = mid;
            else r = mid - 1;
        }
        if (l == min(len1, len2)) {
            if (len1 == len2) return 0;
            return len1 < len2 ? -1 : 1;
        }
        return s[l1 + l] < s[l2 + l] ? -1 : 1;
    }
};