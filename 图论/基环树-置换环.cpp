#include <bits/stdc++.h>
using namespace std;

/**
 * PermCycles：置换环模板，0-based。
 * 模型：
 * 有 n 个点，p 是一个排列，每个点 i 连向 p[i]，整张图由若干个有向环组成。
 * 适合处理的结构：
 * 置换跳跃，排列幂，环长查询，同环判断，环上有向距离。
 *
 * 功能与复杂度：
 * 1. PermCycles pc(p)：传入 0-based permutation 并自动分环，O(n)
 * 2. check(u,v)：判断 u,v 是否在同一个环上，O(1)
 * 3. period(u)：返回 u 所在环的长度，O(1)
 * 4. jump(u,t)：返回从 u 沿 p 走 t 步后的点，O(1)
 * 5. rjump(u,t)：返回从 u 沿 p 的反方向走 t 步后的点，O(1)
 * 6. dist(u,v)：返回 u 沿 p 走到 v 的最少步数，不同环返回 -1，O(1)
 * 7. rdist(u,v)：返回 u 沿 p 的反方向走到 v 的最少步数，不同环返回 -1，O(1)
 * 8. get(u)：返回 u 所在的整个环，O(1)
 * 9. count(): 返回总环数，O(1)
 */
struct PermCycles {
    int n;
    vector<int> p, inv, cid, pos;
    vector<vector<int>> cyc;
    PermCycles() {}
    PermCycles(const vector<int>& _p) : n(_p.size()), p(_p) { build(); }
    void build() {
        inv.assign(n, -1);
        for (int i = 0; i < n; ++i) inv[p[i]] = i;
        cid.assign(n, -1), pos.assign(n, -1);
        cyc.clear();
        for (int i = 0; i < n; ++i) {
            if (cid[i] != -1) continue;
            int u = i, c = cyc.size();
            vector<int> v;
            while (cid[u] == -1) cid[u] = c, pos[u] = v.size(), v.emplace_back(u), u = p[u];
            cyc.emplace_back(v);
        }
    }
    bool check(int u, int v) const { return cid[u] == cid[v]; }
    int period(int u) const { return cyc[cid[u]].size(); }
    int jump(int u, long long t) const {
        int c = cid[u], L = cyc[c].size();
        return cyc[c][(pos[u] + t) % L];
    }
    int rjump(int u, long long t) const {
        int c = cid[u], L = cyc[c].size();
        return cyc[c][(pos[u] - t % L + L) % L];
    }
    long long dist(int u, int v) const {
        if (cid[u] != cid[v]) return -1;
        int c = cid[u], L = cyc[c].size();
        return (pos[v] - pos[u] + L) % L;
    }
    long long rdist(int u, int v) const {
        if (cid[u] != cid[v]) return -1;
        int c = cid[u], L = cyc[c].size();
        return (pos[u] - pos[v] + L) % L;
    }
    const vector<int>& get(int u) const { return cyc[cid[u]]; }
    int count() const { return cyc.size(); }
};