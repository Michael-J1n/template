#include <bits/stdc++.h>
using namespace std;

/**
 * 有一类题型，是求第 k 个满足要求的数（k 是 1-based 下）
 * 两种处理方式：
 * 1. 试填法 -- 当答案本身是一个需要逐位构造的对象
 * 2. 二分法 -- 当答案是一个可以计算的数值，且具有单调性
 */

/**
 * 试填法求 kth（字典序第 k 小的合法答案）
 * 求第 k 小的答案是谁；类似于减枝搜索树的过程
 * 
 * 复杂度 O(n * V * T)，其中 V 为每个位置的试填选择数，T 为单次 calc 的复杂度；一般来说 O(26n) 加上预处理阶乘/组合数
 */
string kth(int n, int k, vector<int>& cnt) {
    auto calc = [&](int rem) -> int {
        // 根据当前已经试填后的状态，计算还有 rem 个位置的剩余部分有多少个合法状态
    };
    if (calc(n) < k) return "";
    string res;
    for (int i = 0; i < n; ++i) {       // 从左到右确定每个位置
        for (int c = 0; c < 26; ++c) {  // 按照字典序枚举所有可能值
            if (!cnt[c]) continue;
            --cnt[c];                   // 试填 c
            int p = calc(n - 1 - i);    // 当前填好固定，后面的自由位不限制，有多少合法状态
            if (k <= p) {               // 第 k 个答案就在所有当前位置填 c 的方案中；保留 c 确定下一个位置
                res.push_back('a' + c);
                break;
            } else {                    // 跳过全部 p 个当前位置填 c 的方案，恢复 c，尝试下一个更大字符（把大小为 p 的子树剪掉）
                k -= p;
                ++cnt[c];
            }
        }
    }
    return res;
}

/**
 * 二分答案法求 kth（值第 k 小的合法答案）
 * 求第 k 小的答案是谁；使用二分
 * 
 * 复杂度 O(T * logV)，其中 V 是值域，T 为单次检查计算的复杂度
 */
int kth(int l, int r, int k) {
    auto calc = [&](int x) -> int {
        // 计算小于等于 x 的答案有多少个
    };
    while (l < r) {
        int mid = l + r >> 1;
        int p = calc(mid);
        if (k <= p) {  // 小于等于 mid 的答案至少有 k 个，第 k 小答案不大于 mid
            r = mid;
        } else {       // 小于等于 mid 的答案不足 k 个，第 k 小答案大于 mid
            l = mid + 1;
        }
    }
    return l;
}
