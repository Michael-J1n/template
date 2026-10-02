#include <bits/stdc++.h>
using namespace std;

/**
 * MEX：数组中没有出现过的最小非负数；即：mex(S) = k <=> 0,1,...,k-1 ∈ S, k !∈ S
 * 
 * 性质：
 * 1. 数组越长/加入的数越多，MEX 单调不减；即：A ⊆ B ==> mex(A) <= mex(B)
 * 2. 长为 n 的数组，其 MEX 只会在 [0,n] 之间，因此记录的大小只用开 n+1
 * 
 * 前置条件：所有输入、插入与删除的值均为非负数；erase(x) 只删除当前存在的 x
 */


// 1. 静态数组 MEX，O(n)
int mex(vector<int>& a) {
    int n = a.size();
    vector<int> cnt(n + 1);
    for (auto& x : a) {
        if (x <= n) ++cnt[x];
    }
    for (int i = 0; i <= n; ++i) {
        if (!cnt[i]) return i;
    }
    return n + 1;
}

// 2. 数组 MEX 的前后缀分解，O(n)
vector<int> pre(vector<int>& a) {
    int n = a.size();
    vector<int> cnt(n + 1), pre(n);
    int mex = 0;
    for (int i = 0; i < n; ++i) {
        if (a[i] <= n) ++cnt[a[i]];
        while (cnt[mex]) ++mex;
        pre[i] = mex;
    }
    return pre;
}
vector<int> suf(vector<int>& a) {
    int n = a.size();
    vector<int> cnt(n + 1), suf(n);
    int mex = 0;
    for (int i = n - 1; ~i; --i) {
        if (a[i] <= n) ++cnt[a[i]];
        while (cnt[mex]) ++mex;
        suf[i] = mex;
    }
    return suf;
}

// 3. 动态维护多重集的 MEX：构造/clear O(n log n)，insert/erase O(log n)，count/contains/get O(1)
struct Mex {
    int n;
    vector<int> cnt;
    set<int> unseen;
    Mex(int _n) : n(_n), cnt(n + 1) {
        for (int i = 0; i <= n; ++i) unseen.insert(i);
    }
    void insert(int x) {
        if (x > n) return;
        ++cnt[x];
        if (cnt[x] == 1) unseen.erase(x);
    }
    void erase(int x) {
        if (x > n) return;
        --cnt[x];
        if (!cnt[x]) unseen.insert(x);
    }
    int count(int x) const {
        if (x > n) return 0;
        return cnt[x];
    }
    bool contains(int x) const { return count(x) > 0; }
    void clear() {
        fill(cnt.begin(), cnt.end(), 0);
        unseen.clear();
        for (int i = 0; i <= n; ++i) unseen.insert(i);
    }
    int get() { return *unseen.begin(); }
};

// 4. 分组 MEX 的时间戳维护
struct MexStamp {
    int n, tim = 0;
    vector<int> vis;
    MexStamp(int _n) : n(_n), vis(n + 1) {}
    int get(vector<int>& a) {
        ++tim;
        for (auto& x : a) {
            if (x <= n) vis[x] = tim;
        }
        for (int i = 0; i <= n; ++i) {
            if (vis[i] != tim) return i;
        }
        return n + 1;
    }
};
