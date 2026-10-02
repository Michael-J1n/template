#include <bits/stdc++.h>
using namespace std;

/**
 * 类似对顶堆的思想，对顶集合维护滑窗内前 k 小/大的值
 * 参数：
 * 1. _K: 控制 k 的大小
 * 2. _sign：sign=1 代表前 k 小的值，sign=-1代表前 k 大的值
 * 类成员：
 * 1. int K, sign:                 sign = 1: 前 K 小; sign = -1: 前 K 大
 * 2. multiset<int> in, out:       in: 选中的 K 种; out: 其余; v' = sign * v
 * 3. long long sum:                      in 中元素的和（在 sign 变换后空间）
 * 前置条件：K>=0，sign 只能为 1 或 -1；get_kth 时选中集合非空；sign=-1 时输入值不能为 INT_MIN
 * 类方法：
 * 1. insert, erase：与普通 multiset 同语义
 * 2. get_kth: 第 k 大/小(由sign决定) 元素
 * 3. query:  前 k 大/小 元素之和
 * 
 * 例题：LC3013
 * */
struct TopKValue {
    int K, sign;
    multiset<int> in, out;
    long long sum;
    TopKValue(int _K = 0, int _sign = 1) : K(_K), sign(_sign), sum(0) {}
    void adjust() {
        while (!out.empty() && in.size() < K) {
            auto it = out.begin();
            int v = *it;
            out.erase(it);
            in.insert(v);
            sum += v;
        }
        while (in.size() > K) {
            auto it = prev(in.end());
            int v = *it;
            in.erase(it);
            out.insert(v);
            sum -= v;
        }
    }
    void insert(int x) {
        int v = x * sign;
        if (!out.empty() && v >= *out.begin()) out.insert(v);
        else in.insert(v), sum += v;
        adjust();
    }
    void erase(int x) {
        int v = x * sign;
        auto it = in.find(v);
        if (it != in.end()) {
            in.erase(it);
            sum -= v;
        } else {
            it = out.find(v);
            if (it != out.end()) out.erase(it);
            else return;
        }
        adjust();
    }
    int get_kth() {
        if (in.size() < K) return -1;
        return *in.rbegin() * sign;
    }
    long long query() const { return sum * sign; }
};


/**
 * 长度为 sz 的滑窗动态求前 k 小/大的值（的和）；sign=1:小, sign=-1:大
 * */
vector<long long> slideWindow(vector<int>& nums, int sz, int k, int sign) {
	TopKValue s(k, sign);
	vector<long long> res;
	for (int i = 0; i < nums.size(); ++i) {
		int id = i - sz;
		if (id >= 0) s.erase(nums[id]);
		s.insert(nums[i]);
		if (i >= sz - 1) res.emplace_back(s.query());
	}
	return res;
}
