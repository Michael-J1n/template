/** 洛谷 P1835
 * 求区间 [L, R] 中素数个数，1<=L<=R<2^31, R-L<=1e6
 * 在 [L,R] 中合数所有的素因子只来自于 [2,sqrt{R}]，因此现予处理这部分的素数
 * 然后由类似于埃式筛的思想将 [L,R] 内的合数划去；特别的，a[i] = false 意为 i + l 为合数
 * */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll N = (1 << 17);
const int MX = 1e6 + 5;
bool a[MX];
ll cnt = 0, prim[N]; bool vis[N];

// 线性筛模板
void init(int n) {
    for (int i = 2; i <= n; ++i) {
        if (!vis[i]) prim[++cnt] = i;
        for (int j = 1; i * prim[j] <= n; ++j) {
            vis[i * prim[j]] = true;
            if (i % prim[j] == 0) break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    ll l, r; cin >> l >> r;
    
    l += (l == 1);
    if (l > r) {
        cout << 0 << '\n';
        return 0;
    }

    // 筛出 [1,sqrt{R}] 中的素数
    init((int)ceil(sqrt(r)));
    memset(a, true, sizeof a);
    // 对于 [L,R] 中的数打标记
    for (int i = 1; i <= cnt; ++i) {
        int p = prim[i];
        // L<=j*p<=R
        for (int j = max(2LL, l / p); 1LL * j * p <= r; ++j) {
            a[j * p - l] = false;
        }
    }
    int res = 0;
    for (int i = 0; i <= r - l; ++i) {
        if (a[i]) ++res;
    }
    cout << res << '\n';
    return 0;
}