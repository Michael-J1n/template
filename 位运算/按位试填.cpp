#include <bits/stdc++.h>
using namespace std;

/**
 * 从高位到低位贪心试填答案
 * 模板以选择 k 个数使得按位与最大为例
 * 
 * 需要修改 check(mask)：
 * check(mask) 返回 true，表示当前答案可以包含 mask 的所有 1 位；
 * check(mask) 返回 false，表示当前这些 1 位不能同时保留。
 * 
 * 适用条件：check(mask) 要有类似单调性。也就是 mask 可行时，mask 的子集也可行。这样才能从高位到低位贪心确定最大答案。
 * 
 * 复杂度：设 check 的复杂度为 O(T)，则总时间复杂度为 O(31 * T)
 */
int bitTry(const vector<int>& a, int k) {
    auto check = [&](int mask) -> bool {  // 检查目标为 mask 时是否合法
        int cnt = 0;
        for (auto& x : a) {
            if ((x & mask) == mask) ++cnt;
        }
        return cnt >= k;
    };  
    int res = 0;
    for (int b = 30; ~b; --b) {
        int cur = res | (1 << b);  // 当前位尝试加入答案
        if (check(cur)) res = cur;
    }
    return res;
}