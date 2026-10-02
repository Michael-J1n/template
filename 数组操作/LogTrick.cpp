#include <bits/stdc++.h>
using namespace std;

/**
 * 按位或 LogTrick
 * 返回 res 数组，其中 res[i]=(v,cnt) 表示以 a[i] 为右端点的子数组有 cnt 个或和为 v
 * 
 * 关键：或和单调不减，维护的集合升序且最多只有logV个不同结果
 * 复杂度：时空均为 O(nlogV) = O(30n)
 */
vector<vector<pair<int, int>>> orLogTrick(vector<int>& a) {
    int n = a.size();
    vector<vector<pair<int, int>>> res(n);
    vector<pair<int, int>> cur;
    for (int i = 0; i < n; ++i) {
        int x = a[i];
        vector<pair<int, int>> nxt;
        nxt.emplace_back(x, 1);
        for (auto& [v, cnt] : cur) {
            int nv = v | x;
            if (nxt.back().first == nv) nxt.back().second += cnt;
            else nxt.emplace_back(nv, cnt);
        }
        cur = nxt;
        res[i] = cur;
    }
    return res;
}

/**
 * 按位与 LogTrick
 * 返回 res 数组，其中 res[i]=(v,cnt) 表示以 a[i] 为右端点的子数组有 cnt 个与和为 v
 * 
 * 关键：与和单调不增，维护的集合降序且最多只有logV个不同结果
 * 复杂度：时空均为 O(nlogV) = O(30n)
 */
vector<vector<pair<int, int>>> andLogTrick(vector<int>& a) {
    int n = a.size();
    vector<vector<pair<int, int>>> res(n);
    vector<pair<int, int>> cur;
    for (int i = 0; i < n; ++i) {
        int x = a[i];
        vector<pair<int, int>> nxt;
        nxt.emplace_back(a[i], 1);
        for (auto& [v, cnt] : cur) {
            int nv = v & x;
            if (nxt.back().first == nv) nxt.back().second += cnt;
            else nxt.emplace_back(nv, cnt);
        }
        cur = nxt;
        res[i] = cur;
    }
    return res;
}

/**
 * 子数组gcd LogTrick
 * 返回 res 数组，其中 res[i]=(v,cnt) 表示以 a[i] 为右端点的子数组有 cnt 个 gcd和 为 v
 * 
 * 关键：gcd新结果一定是旧结果的约数，维护的集合降序且最多只有logV个不同结果
 * 复杂度：时空均为 O(nlogV) = O(30n)
 */
vector<vector<pair<int, int>>> gcdLogTrick(vector<int>& a) {
    int n = a.size();
    vector<vector<pair<int, int>>> res(n);
    vector<pair<int, int>> cur;
    for (int i = 0; i < n; ++i) {
        int x = a[i];
        vector<pair<int, int>> nxt;
        for (auto& [v, cnt] : cur) {
            int nv = gcd(v, x);
            if (nxt.back().first == nv) nxt.back().second += cnt;
            else nxt.emplace_back(nv, cnt);
        }
        cur = nxt;
        res[i] = cur;
    }
    return res;
}