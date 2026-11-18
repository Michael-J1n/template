#include <bits/stdc++.h>
using namespace std;

/**
 * 将 a 升序排序，id[i] 为排序后第 i 个元素的原始下标
 * 相同元素按原始下标升序排列
 */
template<class T>
void sort_with_index(vector<T>& a, vector<int>& id) {
    int n = a.size();
    id.resize(n); iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](int i, int j) {
        if (a[i] != a[j]) return a[i] < a[j];
        return i < j;
    });
    vector<T> b(n);
    for (int i = 0; i < n; ++i) b[i] = a[id[i]];
    a.swap(b);
}