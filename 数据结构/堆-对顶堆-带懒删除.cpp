#include <bits/stdc++.h>
using namespace std;

/**
 * 对顶堆，维护一个小根堆，一个大根堆
 * 使用情景：**滑动窗口动态数据流**
 * 维护小根堆 small 保存了滑窗中前 k 小元素
 * 如果希望维护滑窗中前 k 大元素，在 emplace 时将元素取负即可
 * LazyHeap 的 less<int> 为大根堆，greater<int> 为小根堆
 * 前置条件：top/pop 只在逻辑堆非空时调用；懒删除只删除当前存在的元素
 * */
template <class Cmp>
struct LazyHeap {
    using ll = long long;
    priority_queue<int, vector<int>, Cmp> pq;
    unordered_map<int, int> erased;
    unordered_map<int, int> cnt;
    void prune() {
        while (!pq.empty()) {
            ll x = pq.top();
            auto it = erased.find(x);
            if (it != erased.end() && it->second > 0) {
                if (--it->second == 0) erased.erase(it);
                pq.pop();
            }
            else break;
        }
    }
    void push(int x) {
        pq.emplace(x);
        ++cnt[x];
    }
    bool erase(int x) {
	    auto it = cnt.find(x);
	    if (it == cnt.end() || it->second == 0) return false;
	    if (--it->second == 0) cnt.erase(it);
	    ++erased[x];
	    return true;
	}
    void pop() {
        prune();
        if (pq.empty()) return;
        ll x = pq.top(); pq.pop();
        auto it = cnt.find(x);
        if (it != cnt.end() && --it->second == 0) cnt.erase(it);
    }
    int top() {
        prune();
        return pq.top();
    }
    bool empty() {
        prune();
        return cnt.empty();
    }
};
struct DualHeap {
    using ll = long long;
    int k;   // 滑窗中需要的k的值（不一定是滑窗大小）
    ll sum;  // 滑窗前 k 小和
    LazyHeap<less<int>> small;     // 前 k 小的元素
    LazyHeap<greater<int>> large;  // 其他在滑窗中的元素
    int sz1, sz2;  // 两堆算上懒删除后的实际大小
    
    // 前置条件：k>=0；erase 只删除当前存在的元素
    DualHeap(int _k) : k(_k), sum(0), sz1(0), sz2(0) {}
    void adjust() {
        while (sz1 < k && sz2 > 0) {
            if (large.empty()) break;
            int x = large.top(); large.pop();
            sum += x, ++sz1, --sz2;
            small.push(x);
        }
        while (sz1 > k) {
            if (small.empty()) break;
            int x = small.top(); small.pop();
            sum -= x, --sz1, ++sz2;
            large.push(x);
        }
    }
    void emplace(int x) {
	    if (!small.empty() && x <= small.top()) small.push(x), sum += x, ++sz1;
	    else large.push(x), ++sz2;
	    adjust();
	}
	void erase(int x) {
	    if (!small.empty() && x <= small.top()) {
	        if (small.erase(x)) sum -= x, --sz1;
	        else if (large.erase(x)) --sz2;
	        else return;
	    } else {
	        if (large.erase(x)) --sz2;
	        else if (small.erase(x)) sum -= x, --sz1;
	        else return;
	    }
	    adjust();
	}
};


/**
 * 示例滑窗封装：求 sz 大小的滑窗前 k 小数字的和
 * 
 * 均摊复杂度：插入删除均为 O(log(sz))，因此整体 O(nlog(sz))
 * */
vector<long long> slideWindow(vector<int>& nums, int sz, int k) {
	DualHeap dh(k);
	vector<long long> res;
	for (int i = 0; i < nums.size(); ++i) {
		int id = i - sz;
		if (id >= 0) dh.erase(nums[id]);
		dh.emplace(nums[i]);
		if (i >= sz - 1 && dh.sz1 == k) res.emplace_back(dh.sum);
	}
	return res;
}
