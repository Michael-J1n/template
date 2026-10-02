// 洛谷 P3383，线性筛素数

#include <bits/stdc++.h>
using namespace std;

// 筛结束后共有 cnt 个素数保存在 prim[1] 到 prim[cnt]；一个数 x 为素数当且仅当 x!=1 && !vis[x]

const int MX = 1e6 + 5;
int cnt = 0, prim[MX]; bool vis[MX];
void init() {
    vis[1] = false;
    for (int i = 2; i < MX; ++i) {
        if (!vis[i]) prim[++cnt] = i;
        for (int j = 1; i * prim[j] < MX; ++j) {
            vis[i * prim[j]] = true;
            if (i % prim[j] == 0) break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    init();
    int n, q; cin >> n >> q;
    while (q--) {
        int k; cin >> k;
        cout << prim[k] << '\n';
    }
    return 0;
}