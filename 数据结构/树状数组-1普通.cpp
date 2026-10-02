#include <bits/stdc++.h>
using namespace std;

/**
 * 树状数组模板
 * O(logn) 的单次修改和动态查询区间和
 * 
 * add(i, v);         // a[i] += v
 * set(i, v);         // a[i] 设为 v
 * pref_sum(i);       // sum a[0..i]
 * range_sum(l, r);   // sum a[l..r]
 * get(i);            // 获取最新的 a[i]
 * kth(k);            // 最小下标 p，使得 sum a[0..p] >= k；要求 1<=k<=pres(n-1) 且数非负
 * 前置条件：修改下标满足 0<=i<n；区间查询非空且边界合法；kth 时树非空、频率非负且 k 合法
 */
template<class T>
struct BIT {
    int n;
    vector<long long> c;  // c[i] = sum of nums[i-lowbit(i)...i-1]
    BIT(int _n) : n(_n) {
        c.resize(n + 1);
    }
    BIT(vector<T>& a) : n(a.size()) {
        c.resize(n + 1);
        for (int i = 1; i <= n; ++i) c[i] = a[i - 1];
        for (int i = 1; i <= n; ++i) {
            int j = i + (i & -i);
            if (j <= n) c[j] += c[i];
        }
    }
    void add(int id, T val) {
        ++id;
        for (int i = id; i <= n; i += i & -i) c[i] += val;
    }
    void set(int id, T val) { add(id, val - get(id)); }
    long long pref_sum(int id) {
        ++id;
        long long sum = 0;
        for (int i = id; i >= 1; i &= i - 1) sum += c[i];
        return sum;
    }
    long long range_sum(int l, int r) {
        if (l > r) return 0;
        return pref_sum(r) - (l ? pref_sum(l - 1) : 0);
    }
    long long get(int id) { return range_sum(id, id); }
    int kth(long long k) {
        int pos = 0;
        for (int i = __lg(n); ~i; --i) {
            int nxt = pos + (1 << i);
            if (nxt <= n && c[nxt] < k) pos = nxt, k -= c[nxt];
        }
        // return pos - 1;  // 最大下标使得前缀和 < k
        return pos;         // 最小下标使得前缀和 >= k
    }
};
