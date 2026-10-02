#include <bits/stdc++.h>
using namespace std;

/*
PointInversionOnline：单点修改 + 区间逆序对，在线，0-based，闭区间 [l,r]。

这是正确优先的通用模板：
1. set/add 单点修改 O(1)。
2. inversion_count(l,r)：取出区间后归并计数，O(len loglen)。

动态区间逆序对没有像 sum/max/kth 那样短小稳定的通用最优模板；
极限题通常需要结合数据范围写专门分块/离线分治。
*/

struct PointInversionOnline {
    using ll = long long;
    static constexpr int S = 32;
    int n;
    vector<ll> a;
    mutable vector<ll> buf, tmp;

    PointInversionOnline() {}
    PointInversionOnline(const vector<ll>& nums) {
        init(nums);
    }
    void init(const vector<ll>& nums) {
        a = nums;
        n = a.size();
        buf.reserve(n);
        tmp.reserve(n);
    }
    void set(int p, ll v) {
        if (p < 0 || p >= n) return;
        if (a[p] == v) return;
        a[p] = v;
    }
    void add(int p, ll v) {
        if (p < 0 || p >= n || !v) return;
        a[p] += v;
    }
    long long cdq_inv(vector<ll>& v, vector<ll>& tmp, int L, int R) const {
        if (R - L <= S) {
            long long res = 0;
            for (int i = L; i < R; ++i) {
                for (int j = i + 1; j < R; ++j) res += v[i] > v[j];
            }
            for (int i = L + 1; i < R; ++i) {
                ll x = v[i];
                int j = i - 1;
                while (j >= L && v[j] > x) v[j + 1] = v[j], --j;
                v[j + 1] = x;
            }
            return res;
        }
        int M = (L + R) >> 1;
        long long res = cdq_inv(v, tmp, L, M) + cdq_inv(v, tmp, M, R);
        int i = L, j = M, k = L;
        while (i < M && j < R) {
            if (v[i] <= v[j]) tmp[k++] = v[i++];
            else res += M - i, tmp[k++] = v[j++];
        }
        while (i < M) tmp[k++] = v[i++];
        while (j < R) tmp[k++] = v[j++];
        copy(tmp.begin() + L, tmp.begin() + R, v.begin() + L);
        return res;
    }
    long long inversion_count(int l, int r) const {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return 0;
        int m = r - l + 1;
        buf.resize(m);
        tmp.resize(m);
        copy(a.begin() + l, a.begin() + r + 1, buf.begin());
        return cdq_inv(buf, tmp, 0, m);
    }
};
