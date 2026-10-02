// O(N^2)，且大n会炸int

#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;
int C[N];

int init = [] {
    memset(C, 0, sizeof C);
    C[0] = 1;
    for (int i = 1; i < N; ++i) {
        C[i] = 0;
        for (int j = 0; j < i; ++j)
            C[i] += C[j] * C[i - 1 - j];
    }
    return 0;
}();