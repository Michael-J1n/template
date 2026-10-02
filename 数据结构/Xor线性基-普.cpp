#include <bits/stdc++.h>
using namespace std;

/**
 * 给定数组 a，返回在异或空间中张成 a 的基向量组合，复杂度 O(nB) = O(30n)
 * max_xor(basis, x)：x 与线性空间中的某个值异或所得的最大值
 * merge_basis(a, b)：合并两个线性基
 * 前置条件：输入按非负整数/无符号位模式解释，且 0<=B<类型位数
 */

template<class T>
vector<T> xorBasis(const vector<T>& a, int B = sizeof(T) * 8 - 1) {
    vector<T> p(B + 1);
    for (T x : a) {
        for (int i = B; ~i; --i) {
            if (!(x >> i & 1)) continue;
            if (!p[i]) {
                p[i] = x;
                break;
            }
            x ^= p[i];
        }
    }
    vector<T> basis;
    basis.reserve(B + 1);
    for (int i = 0; i <= B; ++i) {
        if (p[i]) basis.emplace_back(p[i]);
    }
    return basis;
}

template<class T>
T max_xor(const vector<T>& basis, T x = 0) {
    for (auto it = basis.rbegin(); it != basis.rend(); ++it) x = max(x, x ^ *it);
    return x;
}

template<class T>
vector<T> merge_basis(const vector<T>& a, const vector<T>& b, int B = sizeof(T) * 8 - 1) {
    vector<T> vals;
    vals.reserve(a.size() + b.size());
    vals.insert(vals.end(), a.begin(), a.end());
    vals.insert(vals.end(), b.begin(), b.end());
    return xorBasis(vals, B);
}
