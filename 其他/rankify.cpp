#include <bits/stdc++.h>
using namespace std;

/**
 * 将一个长为 n，元素互不相同的数组转化成他们的 0-based 排名
 * 例如：
 * {3, 4, 7, 8} -> {1, 2, 0, 3}
 * 注意：
 * 如果需要 1-base rank，将最后的结果每一位 +1 即可
 * 此模板适合将一个数组转化成排列从而使用置换环函数图跳跃模板，或离线静态提取排名
 */

template<class T>
vector<int> toPerm(vector<T> a) {
    int n = a.size();
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    ranges::sort(id, [&](int i, int j) {
        return a[i] < a[j];
    });
    vector<int> p(n);
    for (int rank = 0; rank < n; ++rank) {
        p[id[rank]] = rank;
        // p[id[rank]] = rank + 1;
    }
    return p;
}