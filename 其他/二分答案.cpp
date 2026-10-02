#include <bits/stdc++.h>
using namespace std;

/**
 * 二分答案模板，使用一个 logV 的时间增加一个条件
 * 求什么就二分什么，需要答案满足 **单调性**
 * 常见题型：满足单调性约束的求解，或最大化最小值/最小化最大值问题
 * 
 * l, r 填写可能答案中最小值，最大值（如果有不可达情况的，在 binMxAns 中 l 设为 -1， binMnAns 中 r 设为 MX+1）
 * check 函数为当某个值固定后，检查是否满足
 */

//【最小化最大值】或【求最小可满足的答案】：本质是值越大，越能满足；
// 需要满足：在 x 从小到大变化时，判定结果为 false false... false | true(find!) true true...
int binMnAns() {
    int l, r;
    // 检查只能使用 <=x 的值，是否能满足条件；或检查 x 作为答案是否可行，即是否存在一个合法方案使得其对应值 <=x。
    auto check = [&](int x) -> bool {};
    while (l < r) {
        int mid = l + r >> 1;
        if (check(mid)) r = mid;
        else l = mid + 1;
    }
    return r;
}

//【最大化最小值】或求【最大可满足的答案】：本质是值越大，越不能满足；
// 需要满足：在 x 从小到大变化时，判定结果为 true true... true(find!) | false false...
int binMxAns() {
    int l, r;
    // 检查只能使用 >=x 的值，是否能满足条件；或检查 x 作为答案是否可行，即是否存在一个合法方案使得其对应值 >=x。
    auto check = [&](int x) -> bool {};
    while (l < r) {
        int mid = l + r + 1 >> 1;
        if (check(mid)) l = mid;
        else r = mid - 1;
    }
    return l;
}

// 浮点数二分，需指定迭代次数
double binDbAns() {
    double l, r;
    auto check = [&](double x) -> bool {};
    for (int i = 0; i < 100; ++i) {
        double mid = (l + r) / 2.0;
        if (check(mid)) r = mid;
        else l = mid;
    }
    return r;
}