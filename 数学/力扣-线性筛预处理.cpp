#include <bits/stdc++.h>
using namespace std;

const int N = 5e5 + 5;
int cnt = 0, prim[N], vis[N];

auto init = [] {
    vis[1] = true;
    for (int i = 2; i < N; ++i) {
        if (!vis[i]) prim[++cnt] = i;
        for (int j = 1; j <= cnt && i * prim[j] < N; ++j) {
            vis[i * prim[j]] = true;
            if (i % prim[j] == 0) break;
        }
    }
    return 0;
}();