#include <bits/stdc++.h>
using namespace std;
#define eb emplace_back

/**
 * 单调栈求 上一个/下一个 更大值/更小值 的模板
 * 复杂度 O(n)
 * 
 * 注意：传入数组为 0-based，规定 prv 不存在返回 -1，nxt 不存在返回 n；返回值均为下标
 */

// pg[i] = previous index j such that a[j] > a[i]
vector<int> preGt(const vector<int>& a) {
    int n = a.size();
    vector<int> res(n, -1), st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] <= a[i]) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.eb(i);
    }
    return res;
}

// pl[i] = previous index j such that a[j] < a[i]
vector<int> preLs(const vector<int>& a) {
    int n = a.size();
    vector<int> res(n, -1), st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.eb(i);
    }
    return res;
}

// ng[i] = next index j such that a[j] > a[i]
vector<int> nxtGt(const vector<int>& a) {
    int n = a.size();
    vector<int> res(n, n), st;
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && a[st.back()] <= a[i]) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.eb(i);
    }
    return res;
}

// nl[i] = next index j such that a[j] < a[i]
vector<int> nxtLs(const vector<int>& a) {
    int n = a.size();
    vector<int> res(n, n), st;
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.eb(i);
    }
    return res;
}

/**
 * 例题：LC 1793，求子数组最大分数；(i,j) 的分数是 min(nums[i~j] * (j-i+1))，且需要满足 i<=k<=j，k是给定的下标
 * 
 * 枚举子数组最小值，分数最大那么固定最小值是宽度最大，因此最远的左端点可以通过先求出第一个左边小于自己的下标，加1即为最远左端点，右端点同理
 */
int maximumScore(vector<int>& nums, int k) {
    int n = nums.size(), res = 0;
    auto pl = preLs(nums), nl = nxtLs(nums);
    for (int i = 0; i < n; ++i) {
        int l = pl[i] + 1, r = nl[i] - 1;
        if (l <= k && k <= r) res = max(res, (r - l + 1) * nums[i]);
    }
    return res;
}