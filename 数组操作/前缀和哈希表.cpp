#include <bits/stdc++.h>
using namespace std;

/**
 * 前缀和哈希表：
 * 0<=j<=i, s[j...i] = s[i] - s[j-1] = goal, (s[i] - goal = s[j-1]) <--- [value(i) = key(j)] (!!!)
 * res += cnt[value(i)], ++cnt[key(i)]
 * 
 * 注：一般定义 s[-1]=0，所以有初始值 cnt[0]=1
 * 模板拆分 value(i)=key(j), 查询用 res+=cnt[value(i)]，但更新要用 ++cnt[key(i)]
 */
int prefixSumWithHashMap(vector<int>& nums, int goal) {
    int n = nums.size(), res = 0;
    int sum = 0;
    unordered_map<int, int> cnt; cnt.reserve(n * 2 + 5); cnt.max_load_factor(0.7); 
    cnt[0] = 1;
    auto key = [&]() -> int {  // 拿来存表的值
        return sum;
    };
    auto val = [&]() -> int {  // 拿去查表的值
        return sum - goal;
    };
    for (int i = 0; i < n; ++i) {
        sum += nums[i];
        res += cnt[val()];
        ++cnt[key()];
    }
    return res;
}


// 例：求有多少非空子数组和为 goal
// s[j...i]=goal, sum[i] - goal = sum[j - 1], 0<=j<=i
// 枚举到i时，表中有 sum[i-1],...,sum[0],sum[-1]=0
int numSubarraysWithSum(vector<int>& nums, int goal) {
    int n = nums.size(), sum = 0, res = 0;
    unordered_map<int, int> cnt; cnt[0] = 1;
    for (int i = 0; i < n; ++i) {
        sum += nums[i], res += cnt[sum - goal];
        ++cnt[sum];
    }
    return res;
}