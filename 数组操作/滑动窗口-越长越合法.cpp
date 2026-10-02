#include <bits/stdc++.h>
using namespace std;

/** 按照题目要求修改参数，add，del，以及 ok（判断当前 a[l...r] 是否合法）*/

// 越长越合法求合法子数组数量；枚举到 r 时 l 在循环结束后是第一个不合法左端点
int countLongLegal(vector<int>& a, int k) {
    int n = a.size(), l = 0, res = 0, sum = 0;

    auto add = [&](int x) {
        sum += x;
    };
    auto del = [&](int x) {
        sum -= x;
    };
    auto ok = [&]() {
        return sum >= k;
    };

    for (int r = 0; r < n; ++r) {
        add(a[r]);
        while (l <= r && ok()) del(a[l++]);
        res += l;
    }
    return res;
}

// 越长越合法求最短合法子数组长度；枚举到 r 时 l 在循环结束后是第一个不合法左端点
int minLongLegal(vector<int>& a, int k) {
    int n = a.size(), l = 0, res = n + 1, sum = 0;

    auto add = [&](int x) {
        sum += x;
    };
    auto del = [&](int x) {
        sum -= x;
    };
    auto ok = [&]() {
        return sum >= k;
    };

    for (int r = 0; r < n; ++r) {
        add(a[r]);
        while (l <= r && ok()) {
            res = min(res, r - l + 1);
            del(a[l++]);
        }
    }
    return res == n + 1 ? -1 : res;
}