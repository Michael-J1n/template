#include <bits/stdc++.h>
using namespace std;

/** 适用场景：按照题目要求，数组会被分割成若干组，且每一组的判断/处理逻辑是一样的 */

// 外层循环准备工作，内层循环遍历组；数字不满足直接跳过，否则记录这一组开始位置，再从下一个位置开始判断，循环结束后从 start 到 i-1 是满足要求且无法延伸的子数组
// 需要填写的部分：1. check1 返回判断当前数是否满足当开头的条件；2. check2 返回判断当前数是否满足当子数组下一位的条件
int numGroups(vector<int>& nums) {
    int n = nums.size(), i = 0, start;
    int res = 0;
    auto check1 = [&](int start) -> bool {
        return 1;
    };
    auto check2 = [&](int cur, int prv) -> bool {
        return cur == prv;
    };
    while (i < n) {
        if (!check1(nums[i])) {
            ++i;
            continue;
        }
        start = i;
        ++i;
        while (i < n && check2(nums[i], nums[i - 1])) ++i;
        // valid subarr: [start, i - 1]
        res = max(res, i - start);
    }
    return res; 
}


// 字符串版本
int numGroups(const string& nums) {
    int n = nums.size(), i = 0, start;
    int res = 0;
    auto check1 = [&](char start) -> bool {
        return 1;
    };
    auto check2 = [&](char cur, char prv) -> bool {
        return cur == prv;
    };
    while (i < n) {
        if (!check1(nums[i])) {
            ++i;
            continue;
        }
        start = i, ++i;
        while (i < n && check2(nums[i], nums[i - 1])) ++i;
        // valid substr: [start, i - 1]
        res = max(res, i - start);
    }
    return res; 
}