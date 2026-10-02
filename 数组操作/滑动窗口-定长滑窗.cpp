#include <bits/stdc++.h>
using namespace std;

/** 定长 k 的滑窗；按照题目要求修改 add,del,update */

int fixedWindow(vector<int>& a, int k) {
    int n = a.size(), res = INT_MIN, sum = 0;
    auto add = [&](int x) {
        sum += x;
    };
    auto del = [&](int x) {
        sum -= x;
    };
    auto update = [&]() {
        res = max(res, sum);
    };
    for (int r = 0; r < n; ++r) {
        add(a[r]);
        if (r >= k) del(a[r - k]);
        if (r >= k - 1) update();
    }
    return res;
}