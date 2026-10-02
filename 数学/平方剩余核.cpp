// 一个数x的平方剩余核core[x]为他所有质因数指数%=2后乘得得数，即为去除最大平方因子后得结果
// 预处理模板，复杂度 O(NlogN)

const int MX = 1e5 + 5;
int core[MX];
int init = [] {
    for (int i = 1; i < MX; ++i) {
        if (core[i] == 0) {
            for (int j = 1; i * j * j < MX; ++j) core[i * j * j] = i;
        }
    }
    return 0;
}();

// core的性质：x * y 为完全平方数 <==> core[x] == core[y]（LC3715,2862）