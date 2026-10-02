// 洛谷 P3912
// 计算 [1, n] 内有多少个素数

#include <bits/stdc++.h>
using namespace std;

int calc(int n) {
    int cnt = 0, prim[n]; bool vis[n + 1];
    vis[1] = false;
    for (int i = 2; i <= n; ++i) {
        if (!vis[i]) prim[cnt++] = i;
        for (int j = 0; i * prim[j] <= n; ++j) {
            vis[i * prim[j]] = true;
            if (i % prim[j] == 0) break;
        }
    }
    return cnt;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    cout << calc(n) << '\n';
    return 0;
}