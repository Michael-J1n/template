#include <bits/stdc++.h>
using namespace std;

/**
 * 维护一个 GF(2) 线性空间，使得所有插入的数的任意子集 XOR 都可以由这些基向量组合得到
 * 
 * insert(x)：插入 x，如果线性无关，入基
 * can(x)：x 是否能被表示
 * rank()：返回线性基的维度 (rank)； 若 rank = k 则不同子集 XOR 数量为 2^k
 * max_xor(x)：x 与线性空间中的某个值异或所得的最大值
 * merge(other)：将另一个线性基合并进来
 * 前置条件：输入按非负整数/无符号位模式解释，B 必须落在类型的合法位编号内
 */
struct LinearBasis {
    static constexpr int B = 31;
    int p[B + 1];
    LinearBasis() {
        memset(p, 0, sizeof(p));
    }
    bool insert(int x) {
        for (int i = B; ~i; --i) {
            if (!(x >> i & 1)) continue;
            if (!p[i]) {
                p[i] = x;
                return true;
            }
            x ^= p[i];
        }
        return false;
    }
    LinearBasis(const vector<int>& a) {
        memset(p, 0, sizeof(p));
        for (auto& x : a) insert(x);
    }
    bool can(int x) {
        for (int i = B; ~i; --i) {
            if (!(x >> i & 1)) continue;
            if (!p[i]) return false;
            x ^= p[i];
        }
        return true;
    }
    int rank() {
        int res = 0;
        for (int i = 0; i <= B; ++i) res += !!p[i];
        return res;
    }
    int max_xor(int x = 0) const {
        for (int i = B; ~i; --i) x = max(x, x ^ p[i]);
        return x;
    }
    void merge(const LinearBasis& other) {
        for (int i = 0; i <= B; ++i) if (other.p[i]) insert(other.p[i]);
    }
};
