#include <bits/stdc++.h>
using namespace std;

/**
 * 无向基环树 / 基环森林模板；带边权版
 *
 * WeightedUnicyclicGraph(n, edges)：传入 0-based 无向带权边 (u,v,w)，O(nlogD)
 * in_cycle(u)：判断 u 是否在环上，O(1)
 * depth(u)：返回 u 到所属环的边数，O(1)
 * dist_to_cycle(u)：返回 u 到所属环的边权和，O(1)
 * root(u)：返回 u 所属挂树的环上根节点；u 在环上时返回 u，O(1)
 * cycle_len(u)：返回 u 所在连通块的环长，O(1)
 * cycle_weight(u)：返回 u 所在连通块的环总边权，O(1)
 * same_component(u, v)：判断 u、v 是否在同一个连通块，O(1)
 * same_tree(u, v)：判断 u、v 是否挂在同一个环点下，O(1)
 * kth_anc(u, t)：返回 u 向环方向走 t 步后的节点；t>depth(u) 返回 -1，O(logD)
 * is_anc(u, v)：判断 u 是否是 v 在同一棵挂树中的祖先，O(1)
 * lca(u, v)：返回 u、v 在同一棵挂树中的 LCA；否则返回 -1，O(logD)
 * tree_steps(u, v)：返回同一棵挂树中路径的边数；否则返回 -1，O(logD)
 * tree_dist(u, v)：返回同一棵挂树中路径的边权和；否则返回 -1，O(logD)
 * cycle_forward_steps(u, v)：要求 u、v 在同一个环上，返回按模板环序的边数，O(1)
 * cycle_steps(u, v)：要求 u、v 在同一个环上，返回环上较少边数，O(1)
 * cycle_forward_dist(u, v)：要求 u、v 在同一个环上，返回按模板环序的边权和，O(1)
 * cycle_dist(u, v)：要求 u、v 在同一个环上，返回两条环上简单路径的较小权值和，O(1)
 * steps(u, v)：返回 u、v 间边数最少的简单路径长度；不连通返回 -1，O(logD)
 * dist(u, v)：返回 u、v 间权值和最小的简单路径；不连通返回 -1，O(logD)
 * path_count(u, v)：返回 u、v 间简单路径条数；不连通为 0，否则为 1 或 2，O(1)
 *
 * 其中 D=max(depth(u))。
 * 要求每个连通块恰好含一个环，图中无自环和重边。
 * 要求 n>=1、0<=u,v<n，所有权值结果及中间运算不超过 long long。
 */
struct WeightedUnicyclicGraph {
    struct Info {
        int depth, base, pos, tin, tout;
        long long dist;
    };
    int n, LOG;
    vector<int> up, cycle;
    vector<long long> cyclePre;
    vector<Info> info;
    WeightedUnicyclicGraph(int _n, const vector<tuple<int, int, long long>>& edges) : n(_n) {
        build(edges);
    }
    bool in_cycle(int u) const { return info[u].depth == 0; }
    int depth(int u) const { return info[u].depth; }
    long long dist_to_cycle(int u) const { return info[u].dist; }
    int root(int u) const { return cycle[info[u].base + 1 + info[u].pos]; }
    int cycle_len(int u) const { return cycle[info[u].base]; }
    long long cycle_weight(int u) const {
        int base = info[u].base;
        return cyclePre[base + cycle[base]];
    }
    bool same_component(int u, int v) const {
        return info[u].base == info[v].base;
    }
    bool same_tree(int u, int v) const {
        return info[u].base == info[v].base && info[u].pos == info[v].pos;
    }
    int kth_anc(int u, int t) const {
        if (t < 0 || t > info[u].depth) return -1;
        return lift(u, t);
    }
    bool is_anc(int u, int v) const {
        return same_tree(u, v) && info[u].tin <= info[v].tin && info[v].tin < info[u].tout;
    }
    int lca(int u, int v) const {
        if (!same_tree(u, v)) return -1;
        if (is_anc(u, v)) return u;
        if (is_anc(v, u)) return v;
        for (int k = LOG - 1; k >= 0; --k) {
            int x = up[k * n + u];
            if (!is_anc(x, v)) u = x;
        }
        return up[u];
    }
    int tree_steps(int u, int v) const {
        int p = lca(u, v);
        if (p == -1) return -1;
        return info[u].depth + info[v].depth - 2 * info[p].depth;
    }
    long long tree_dist(int u, int v) const {
        int p = lca(u, v);
        if (p == -1) return -1;
        return info[u].dist + info[v].dist - 2 * info[p].dist;
    }
    int cycle_forward_steps(int u, int v) const {
        int L = cycle_len(u);
        return (info[v].pos - info[u].pos + L) % L;
    }
    int cycle_steps(int u, int v) const {
        int d = cycle_forward_steps(u, v);
        return min(d, cycle_len(u) - d);
    }
    long long cycle_forward_dist(int u, int v) const {
        int base = info[u].base;
        int a = info[u].pos, b = info[v].pos, L = cycle[base];
        if (a <= b) return cyclePre[base + b] - cyclePre[base + a];
        return cyclePre[base + L] - cyclePre[base + a] + cyclePre[base + b];
    }
    long long cycle_dist(int u, int v) const {
        long long d = cycle_forward_dist(u, v);
        return min(d, cycle_weight(u) - d);
    }
    int steps(int u, int v) const {
        if (!same_component(u, v)) return -1;
        if (same_tree(u, v)) return tree_steps(u, v);
        return info[u].depth + info[v].depth + cycle_steps(root(u), root(v));
    }
    long long dist(int u, int v) const {
        if (!same_component(u, v)) return -1;
        if (same_tree(u, v)) return tree_dist(u, v);
        return info[u].dist + info[v].dist + cycle_dist(root(u), root(v));
    }
    int path_count(int u, int v) const {
        if (!same_component(u, v)) return 0;
        return same_tree(u, v) ? 1 : 2;
    }
private:
    int lift(int u, int t) const {
        for (int k = 0; t; ++k, t >>= 1) {
            if (t & 1) u = up[k * n + u];
        }
        return u;
    }
    void build(const vector<tuple<int, int, long long>>& edges) {
        info.assign(n, {-1, -1, -1, -1, -1, 0});
        vector<int> par(n, -1);
        vector<long long> parWeight(n, 0);
        int maxDepth = 0;
        {
            int m = edges.size();
            vector<int> deg(n, 0), start(n + 1, 0);
            for (auto [u, v, w] : edges) ++deg[u], ++deg[v];
            for (int i = 0; i < n; ++i) start[i + 1] = start[i] + deg[i];
            vector<int> cur = start, to(m * 2), ord(n);
            vector<long long> weight(m * 2);
            for (auto [u, v, w] : edges) {
                int a = cur[u]++, b = cur[v]++;
                to[a] = v;
                weight[a] = w;
                to[b] = u;
                weight[b] = w;
            }
            int l = 0, r = 0;
            for (int i = 0; i < n; ++i) {
                if (deg[i] == 1) ord[r++] = i;
            }
            while (l < r) {
                int u = ord[l++];
                deg[u] = 0;
                for (int e = start[u]; e < start[u + 1]; ++e) {
                    int v = to[e];
                    if (!deg[v]) continue;
                    par[u] = v;
                    parWeight[u] = weight[e];
                    if (--deg[v] == 1) ord[r++] = v;
                    break;
                }
            }
            vector<char> vis(n, 0);
            for (int i = 0; i < n; ++i) {
                if (!deg[i] || vis[i]) continue;
                int base = cycle.size();
                cycle.emplace_back(0);
                cyclePre.emplace_back(0);
                int root = i, pre = -1, u = root, pos = 0;
                while (true) {
                    vis[u] = 1;
                    info[u] = {0, base, pos++, -1, -1, 0};
                    cycle.emplace_back(u);
                    int v = -1;
                    long long w = 0;
                    for (int e = start[u]; e < start[u + 1]; ++e) {
                        int x = to[e];
                        if (deg[x] && x != pre) {
                            v = x;
                            w = weight[e];
                            break;
                        }
                    }
                    cyclePre.emplace_back(cyclePre.back() + w);
                    pre = u;
                    u = v;
                    if (u == root) break;
                }
                cycle[base] = pos;
            }
            for (int i = r - 1; i >= 0; --i) {
                int u = ord[i], v = par[u];
                info[u] = {
                    info[v].depth + 1,
                    info[v].base,
                    info[v].pos,
                    -1,
                    -1,
                    info[v].dist + parWeight[u]
                };
                maxDepth = max(maxDepth, info[u].depth);
            }
            fill(deg.begin(), deg.end(), -1);
            for (int i = 0; i < r; ++i) {
                int u = ord[i], v = par[u];
                info[u].tout = deg[v];
                deg[v] = u;
            }
            int timer = 0, cycleSize = cycle.size();
            for (int base = 0; base < cycleSize; base += cycle[base] + 1) {
                int L = cycle[base];
                for (int i = 1; i <= L; ++i) {
                    int root = cycle[base + i], u = root;
                    while (true) {
                        if (info[u].tin == -1) info[u].tin = timer++;
                        int v = deg[u];
                        if (v != -1) {
                            deg[u] = info[v].tout;
                            u = v;
                        } else {
                            info[u].tout = timer;
                            if (u == root) break;
                            u = par[u];
                        }
                    }
                }
            }
        }
        LOG = maxDepth ? __lg(maxDepth) + 1 : 0;
        up.assign(n * LOG, 0);
        if (!LOG) return;
        for (int i = 0; i < n; ++i) up[i] = info[i].depth ? par[i] : i;
        for (int k = 1; k < LOG; ++k) {
            int pre = (k - 1) * n, cur = k * n;
            for (int i = 0; i < n; ++i) {
                up[cur + i] = up[pre + up[pre + i]];
            }
        }
    }
};