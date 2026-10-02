#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll maxSubarraySum(const vector<int>& a) {
    ll cur = a[0], res = a[0];
    for (int i = 1; i < a.size(); ++i) {
        cur = max(1LL * a[i], cur + a[i]);
        res = max(res, cur);
    }
    return res;
}
ll minSubarraySum(const vector<int>& a) {
    ll cur = a[0], res = a[0];
    for (int i = 1; i < a.size(); ++i) {
        cur = min(1LL * a[i], cur + a[i]);
        res = min(res, cur);
    }
    return res;
}


// 带左右端点版本

tuple<ll, int, int> maxSubarray(const vector<int>& a) {
    int n = a.size();
    ll cur = a[0], res = a[0];
    int st = 0, l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (cur + a[i] < a[i]) cur = a[i], st = i;
        else cur += a[i];
        if (cur > res) res = cur, l = st, r = i;
    }
    return {res, l, r};
}
tuple<ll, int, int> minSubarray(const vector<int>& a) {
    int n = a.size();
    ll cur = a[0], res = a[0];
    int st = 0, l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (cur + a[i] > a[i]) cur = a[i], st = i;
        else cur += a[i];
        if (cur < res) res = cur, l = st, r = i;
    }
    return {res, l, r};
}