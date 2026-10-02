#include <bits/stdc++.h>
using namespace std;

const long long MX = 10'000'000'000LL; // 根据题目调整
vector<long long> palindromes;

// 预处理 [1,MX] 中的回文数
auto init = []() {
    for (int base = 1; ; base *= 10) {
        // 生成奇数长度回文数，例如 base = 10，生成的范围是 101 ~ 999
        for (int i = base; i < base * 10; i++) {
            long long x = i;
            for (int t = i / 10; t > 0; t /= 10) x = x * 10 + t % 10;
            if (x > MX) return 0;
            palindromes.push_back(x);
        }
        // 生成偶数长度回文数，例如 base = 10，生成的范围是 1001 ~ 9999
        for (int i = base; i < base * 10; i++) {
            long long x = i;
            for (int t = i; t > 0; t /= 10) x = x * 10 + t % 10;
            if (x > MX) return 0;
            palindromes.push_back(x);
        }
    }
}();