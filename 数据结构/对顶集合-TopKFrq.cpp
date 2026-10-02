#include <bits/stdc++.h>
using namespace std;

/**
 * 对顶集合维护滑窗内前 k 小/大的频率
 * 
 * 参数：
 * _K: 控制 k 的大小
 * _sign：sign=1 代表前 k 小的频率，sign=-1 代表前 k 大的频率
 * 类成员：
 * int K, sign;                  // sign = -1: 前 K 大频率; sign = 1: 前 K 小频率
 * unordered_map<int, int> cnt;  // 元素 -> 频率
 * multiset<pair<int, int>> in, out;        // in: 选中的 K 种; out: 其余; key = (sign * freq, -val)
 * long long sum;                       // in 中 freq * val 之和
 * 前置条件：K>=0，sign 只能为 1 或 -1；get_kth 时选中集合非空；元素值不能为 INT_MIN
 * 类方法：
 * 1. insert, erase 同 multiset 语义
 * 2. get_kth, 返回 [val, frq] -> 第 k 大/小频率的 值-频率对
 * 3. query：前 k 大/小频率 对应的元素的 freq * val 之和
 * 
 * 例题：LC3321
 * */
struct TopKFrq {
    int K, sign;
    unordered_map<int, int> cnt;
    multiset<pair<int, int>> in, out;
    long long sum;
    TopKFrq(int _K = 0, int _sign = -1) : K(_K), sign(_sign), sum(0) {}
    pair<int, int> make_key(int f, int x) const { return {sign * f, -x}; }
    void adjust() {
        while (!out.empty() && in.size() < K) {
            auto it = out.begin();
            pair<int, int> key = *it;
            out.erase(it);
            in.insert(key);
            int x = -key.second;
            int f = cnt[x];
            sum += 1LL * f * x;
        }
        while (in.size() > K) {
            auto it = prev(in.end());
            pair<int, int> key = *it;
            in.erase(it);
            out.insert(key);
            int x = -key.second;
            int f = cnt[x];
            sum -= 1LL * f * x;
        }
    }
    void add_key(pair<int, int> key, int f, int x) {
        if (!out.empty() && key >= *out.begin()) out.insert(key);
        else in.insert(key), sum += 1LL * f * x;
        adjust();
    }
    void del_key(pair<int, int> key, int f, int x) {
        auto it = in.find(key);
        if (it != in.end()) {
            in.erase(it);
            sum -= 1LL * f * x;
        } else {
            it = out.find(key);
            if (it != out.end()) out.erase(it);
            else return;
        }
        adjust();
    }
    void insert(int x) {
        int f_old = cnt[x];
        if (f_old) del_key(make_key(f_old, x), f_old, x);
        int f_new = f_old + 1;
        cnt[x] = f_new;
        add_key(make_key(f_new, x), f_new, x);
    }
    void erase(int x) {
        auto it = cnt.find(x);
        if (it == cnt.end()) return;
        int f_old = it->second;
        del_key(make_key(f_old, x), f_old, x);
        int f_new = f_old - 1;
        if (f_new == 0) cnt.erase(it);
        else {
            cnt[x] = f_new;
            add_key(make_key(f_new, x), f_new, x);
        }
    }
    pair<int, int> get_kth() const {
        auto key = *prev(in.end());
        int x = -key.second;
        int f = cnt.at(x);
        return {x, f};
    }
    long long query() const { return sum; }
};


/**
 * 长度为 sz 的滑窗动态求前 k 小/大的频率（的元素的和）；sign=1:小, sign=-1:大
 * */
vector<long long> slideWindow(vector<int>& nums, int sz, int k, int sign) {
    TopKFrq s(k, sign);
    vector<long long> res;
    for (int i = 0; i < nums.size(); ++i) {
        int id = i - sz;
        if (id >= 0) s.erase(nums[id]);
        s.insert(nums[i]);
        if (i >= sz - 1) res.emplace_back(s.query());
    }
    return res;
}
