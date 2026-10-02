#include <bits/stdc++.h>
using namespace std;

/**
 * 给定一个非递减且下标从0开始的数组，进行二分查找:
 * 
    first_ge(x) // 第一个 >= x 的位置，不存在返回 -1
    first_gt(x) // 第一个 >  x 的位置，不存在返回 -1
    first_lt(x) // 第一个 <  x 的位置，不存在返回 -1
    first_le(x) // 第一个 <= x 的位置，不存在返回 -1
    first_eq(x) // 第一个 == x 的位置，不存在返回 -1

    last_ge(x)  // 最后一个 >= x 的位置，不存在返回 -1
    last_gt(x)  // 最后一个 >  x 的位置，不存在返回 -1
    last_lt(x)  // 最后一个 <  x 的位置，不存在返回 -1
    last_le(x)  // 最后一个 <= x 的位置，不存在返回 -1
    last_eq(x)  // 最后一个 == x 的位置，不存在返回 -1
 */

template<class T>
struct BinarySearch {
    vector<T> a;
    int n;
    BinarySearch(const vector<T>& _a) : a(_a), n(a.size()) {}
    int first_ge(const T& x) {
        int p = lower_bound(a.begin(), a.end(), x) - a.begin();
        return p == n ? -1 : p;
    }
    int first_gt(const T& x) {
        int p = upper_bound(a.begin(), a.end(), x) - a.begin();
        return p == n ? -1 : p;
    }
    int first_lt(const T& x) {
        return n && a[0] < x ? 0 : -1;
    }
    int first_le(const T& x) {
        return n && a[0] <= x ? 0 : -1;
    }
    int first_eq(const T& x) {
        int p = lower_bound(a.begin(), a.end(), x) - a.begin();
        return p < n && a[p] == x ? p : -1;
    }
    int last_ge(const T& x) {
        return n && a[n - 1] >= x ? n - 1 : -1;
    }
    int last_gt(const T& x) {
        return n && a[n - 1] > x ? n - 1 : -1;
    }
    int last_lt(const T& x) {
        int p = lower_bound(a.begin(), a.end(), x) - a.begin() - 1;
        return p >= 0 ? p : -1;
    }
    int last_le(const T& x) {
        int p = upper_bound(a.begin(), a.end(), x) - a.begin() - 1;
        return p >= 0 ? p : -1;
    }
    int last_eq(const T& x) {
        int p = upper_bound(a.begin(), a.end(), x) - a.begin() - 1;
        return p >= 0 && a[p] == x ? p : -1;
    }
};