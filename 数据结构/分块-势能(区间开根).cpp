#include <bits/stdc++.h>
using namespace std;

struct blockDecomposition {
    int n, B, m;                            // n: array length; B: block size; m: block number
    vector<long long> a;                    // a: maintained array without lazy tags
    vector<long long> sum;                  // sum: sum of a in each block without lazy tags
    vector<char> done;                      // done: is true when range max <=1
    long long isqrt(long long x) {
        long long y = sqrtl((long double)x);
        while (y + 1 <= x / (y + 1)) ++y;
        while (y > 0 && y > x / y) --y;
        return y;
    }
    blockDecomposition(const vector<long long>& nums) {
        n = nums.size();
        B = sqrt(n) + 1;
        m = (n + B - 1) / B;
        a = nums;
        sum.assign(m, 0);
        done.assign(m, 0);
        for (int i = 0; i < n; ++i) {
            int id = i / B;
            sum[id] += a[i];
        }
        for (int i = 0; i < m; ++i) {
            rebuild(i);
        }
    }
    void rebuild(int b) {
        long long mx = 0;
        for (int i = b * B; i < min(n, (b + 1) * B); ++i) mx = max(mx, a[i]);
        done[b] = mx <= 1;
    }
    void apply(int i) {
        int id = i / B;
        sum[id] -= a[i];
        a[i] = isqrt(a[i]);
        sum[id] += a[i];
    }
    void sqt(int l, int r) {
        int bl = l / B, br = r / B;
        if (bl == br) {
        	if (done[bl]) return;
            for (int i = l; i <= r; ++i) apply(i);
            rebuild(bl);
        } else {
            if (!done[bl]) {
            	for (int i = l; i < (bl + 1) * B; ++i) apply(i);
            	rebuild(bl);
            }
            for (int b = bl + 1; b < br; ++b) {
            	if (!done[b]) {
            		for (int i = b * B; i < min(n, (b + 1) * B); ++i) apply(i);
            		rebuild(b);
            	}
            }
            if (!done[br]) {
            	for (int i = br * B; i <= r; ++i) apply(i);
            	rebuild(br);
            }
        }
    }
    long long query(int l, int r) {
        int bl = l / B, br = r / B;
        long long res = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) res += a[i];
        } else {
            for (int i = l; i < (bl + 1) * B; ++i) res += a[i];
            for (int b = bl + 1; b < br; ++b) {
                res += sum[b];
            }
            for (int i = br * B; i <= r; ++i) res += a[i];
        }
        return res;
    }
};
