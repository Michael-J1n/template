#include <bits/stdc++.h>
using namespace std;

/**
 * 函数图模板 / 有向基环树模板（ρ 形图）；带边权和版
 *
 * FunctionalGraph(nxt, w)：传入 0-based 后继数组和边权数组，w[u] 为 u->nxt[u] 的边权，O(nlogD)
 * jump(u, t)：返回从 u 沿 nxt 走 t 步后到达的点，O(logD)
 * jump_sum(u, t)：返回 {走 t 步后到达的点, 经过边的权值和}，O(logD)
 * walk_sum(u, t)：返回从 u 沿 nxt 走 t 步经过边的权值和，O(logD)
 * in_cycle(u)：判断 u 是否在环上，O(1)
 * pre_len(u)：返回 u 到第一次进入环所需的步数，O(1)
 * pre_weight(u)：返回 u 到第一次进入环经过边的权值和，O(1)
 * period(u)：返回 u 所在环或最终进入的环的环长，O(1)
 * cycle_weight(u)：返回 u 所在环或最终进入的环的一圈边权和，O(1)
 * entry(u)：返回 u 第一次进入环时到达的环上节点；u 在环上时返回 u，O(1)
 * distinct(u, t)：返回从 u 开始走至多 t 步经过的不同节点数，包含起点，O(1)
 * is_anc(u, v)：判断 u 是否是 v 在同一棵反向入环树中的祖先，O(1)
 * cycle_dist(u, v)：要求 u、v 在同一个环上，返回从 u 沿 nxt 到 v 的步数，O(1)
 * cycle_dist_weight(u, v)：要求 u、v 在同一个环上，返回从 u 沿环到 v 的边权和，O(1)
 * dist_to(u, v)：返回从 u 沿 nxt 到达 v 的最少步数，无法到达返回 -1，O(1)
 * weight_to(u, v)：返回从 u 沿 nxt 第一次到达 v 的边权和，无法到达返回 nullopt，O(1)
 * reach(u, v)：判断从 u 沿 nxt 是否能够到达 v，O(1)
 *
 * 其中 D=max(pre_len(u))。
 * 要求 n>=1、nxt.size()==w.size()、0<=nxt[u]<n、t>=0。
 * 要求所有查询结果及中间边权和不超过 long long。
 */
struct FunctionalGraph {
    struct Info {
        int dist, base, pos, tin, tout;
        long long weight;
    };
    int n, LOG;
    vector<int> up, cycle;
    vector<long long> upSum, cyclePre;
    vector<Info> info;
    FunctionalGraph(const vector<int>& nxt, const vector<long long>& w) : n(nxt.size()) {
        build(nxt, w);
    }
    int jump(int u, long long t) const {
        if (t < info[u].dist) return lift(u, t);
        t -= info[u].dist;
        int base = info[u].base, L = cycle[base];
        return cycle[base + 1 + (info[u].pos + t % L) % L];
    }
    pair<int, long long> jump_sum(int u, long long t) const {
        if (t < info[u].dist) return lift_sum(u, t);
        long long res = info[u].weight;
        t -= info[u].dist;
        int base = info[u].base, L = cycle[base];
        res += cycle_sum(base, info[u].pos, t);
        int v = cycle[base + 1 + (info[u].pos + t % L) % L];
        return {v, res};
    }
    long long walk_sum(int u, long long t) const {
        return jump_sum(u, t).second;
    }
    bool in_cycle(int u) const { return info[u].dist == 0; }
    int pre_len(int u) const { return info[u].dist; }
    long long pre_weight(int u) const { return info[u].weight; }
    int period(int u) const { return cycle[info[u].base]; }
    long long cycle_weight(int u) const {
        int base = info[u].base;
        return cyclePre[base + cycle[base]];
    }
    int entry(int u) const { return cycle[info[u].base + 1 + info[u].pos]; }
    long long distinct(int u, long long t) const {
        return min(t + 1, 1LL * info[u].dist + period(u));
    }
    bool is_anc(int u, int v) const {
        return info[u].tin <= info[v].tin && info[v].tin < info[u].tout;
    }
    int cycle_dist(int u, int v) const {
        int L = period(u);
        return (info[v].pos - info[u].pos + L) % L;
    }
    long long cycle_dist_weight(int u, int v) const {
        int d = cycle_dist(u, v);
        return cycle_sum(info[u].base, info[u].pos, d);
    }
    long long dist_to(int u, int v) const {
        if (info[u].base != info[v].base) return -1;
        if (!in_cycle(v)) {
            if (!is_anc(v, u)) return -1;
            return info[u].dist - info[v].dist;
        }
        return info[u].dist + cycle_dist(entry(u), v);
    }
    optional<long long> weight_to(int u, int v) const {
        if (info[u].base != info[v].base) return nullopt;
        if (!in_cycle(v)) {
            if (!is_anc(v, u)) return nullopt;
            return info[u].weight - info[v].weight;
        }
        return info[u].weight + cycle_dist_weight(entry(u), v);
    }
    bool reach(int u, int v) const { return dist_to(u, v) != -1; }
private:
    int lift(int u, long long t) const {
        for (int k = 0; t; ++k, t >>= 1) {
            if (t & 1) u = up[k * n + u];
        }
        return u;
    }
    pair<int, long long> lift_sum(int u, long long t) const {
        long long res = 0;
        for (int k = 0; t; ++k, t >>= 1) {
            if (t & 1) {
                res += upSum[k * n + u];
                u = up[k * n + u];
            }
        }
        return {u, res};
    }
    long long cycle_sum(int base, int pos, long long t) const {
        int L = cycle[base];
        long long total = cyclePre[base + L];
        long long res = t / L * total;
        int rem = t % L;
        int r = pos + rem;
        if (r <= L) {
            res += cyclePre[base + r] - cyclePre[base + pos];
        } else {
            res += total - cyclePre[base + pos] + cyclePre[base + r - L];
        }
        return res;
    }
    void build(const vector<int>& nxt, const vector<long long>& w) {
        info.assign(n, {-1, -1, -1, -1, -1, 0});
        int maxDist = 0;
        {
            vector<int> ind(n, 0), ord(n);
            for (int i = 0; i < n; ++i) ++ind[nxt[i]];
            int l = 0, r = 0;
            for (int i = 0; i < n; ++i) {
                if (!ind[i]) ord[r++] = i;
            }
            while (l < r) {
                int u = ord[l++], v = nxt[u];
                if (--ind[v] == 0) ord[r++] = v;
            }
            int cycleCnt = 0, cycleNum = 0;
            for (int i = 0; i < n; ++i) {
                if (!ind[i]) continue;
                int u = i;
                ord[r + cycleNum++] = i;
                while (ind[u]) {
                    ind[u] = 0;
                    ++cycleCnt;
                    u = nxt[u];
                }
            }
            cycle.assign(cycleCnt + cycleNum, 0);
            cyclePre.assign(cycleCnt + cycleNum, 0);
            int p = 0;
            for (int i = 0; i < cycleNum; ++i) {
                int root = ord[r + i], base = p++, u = root, pos = 0;
                cyclePre[base] = 0;
                while (true) {
                    info[u] = {0, base, pos++, -1, -1, 0};
                    cycle[p] = u;
                    cyclePre[p] = cyclePre[p - 1] + w[u];
                    ++p;
                    u = nxt[u];
                    if (u == root) break;
                }
                cycle[base] = pos;
            }
            for (int i = r - 1; i >= 0; --i) {
                int u = ord[i], v = nxt[u];
                info[u] = {
                    info[v].dist + 1,
                    info[v].base,
                    info[v].pos,
                    -1,
                    -1,
                    w[u] + info[v].weight
                };
                maxDist = max(maxDist, info[u].dist);
            }
            fill(ind.begin(), ind.end(), -1);
            for (int i = 0; i < r; ++i) {
                int u = ord[i], v = nxt[u];
                info[u].tout = ind[v];
                ind[v] = u;
            }
            int timer = 0, cycleSize = cycle.size();
            for (int base = 0; base < cycleSize; base += cycle[base] + 1) {
                int L = cycle[base];
                for (int i = 1; i <= L; ++i) {
                    int root = cycle[base + i], u = root;
                    while (true) {
                        if (info[u].tin == -1) info[u].tin = timer++;
                        int v = ind[u];
                        if (v != -1) {
                            ind[u] = info[v].tout;
                            u = v;
                        } else {
                            info[u].tout = timer;
                            if (u == root) break;
                            u = nxt[u];
                        }
                    }
                }
            }
        }
        int maxStep = max(0, maxDist - 1);
        LOG = maxStep ? __lg(maxStep) + 1 : 0;
        up.assign(n * LOG, 0);
        upSum.assign(n * LOG, 0);
        if (!LOG) return;
        for (int i = 0; i < n; ++i) {
            up[i] = nxt[i];
            upSum[i] = w[i];
        }
        for (int k = 1; k < LOG; ++k) {
            int pre = (k - 1) * n, cur = k * n;
            for (int i = 0; i < n; ++i) {
                up[cur + i] = up[pre + up[pre + i]];
                upSum[cur + i] = upSum[pre + i] + upSum[pre + up[pre + i]];
            }
        }
    }
};