#include <bits/stdc++.h>
using namespace std;

/**
 * 求无权树的直径
 * 
 * 两次搜索得到直径端点：第一次随机走到最远点，第二次从该点出发走到最远点
 * 搜索结束后 u 和 v 为直径端点；从 v 开始一直沿着 fa[x] 走的链为直径；返回直径长度
 * 
 * 复杂度 O(n)
 */
int treeDiameter(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> fa, dis;
    auto bfs = [&](int s) -> int {
        fa.assign(n, -1);
        dis.assign(n, -1); dis[s] = 0;
        queue<int> q; q.emplace(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& v : g[u]) {
                if (dis[v] != -1) continue;
                dis[v] = dis[u] + 1, q.emplace(v);
                fa[v] = u;
            }
        }
        int x = s;
        for (int i = 0; i < n; ++i) {
            if (dis[i] > dis[x]) x = i;
        }
        return x;
    };
    int u = bfs(0), v = bfs(u);
    int d = -1;
    for (int x = v; x != -1; x = fa[x]) ++d;
    return d;
}

/**
 * 求正边权树的直径
 */
long long treeDiameterW(vector<vector<pair<int, int>>>& g) {
    int n = g.size();
    vector<int> fa; vector<long long> dis;
    auto dfs = [&](this auto&& dfs, int x, int p) -> void {
        for (auto& [y, w] : g[x]) {
            if (y == p) continue;
            fa[y] = x;
            dis[y] = dis[x] + w;
            dfs(y, x);
        }
    };
    auto get = [&](int s) -> int {
        fa.assign(n, -1);
        dis.assign(n, 0);
        dfs(s, -1);
        int x = s;
        for (int i = 0; i < n; ++i) {
            if (dis[i] > dis[x]) x = i;
        }
        return x;
    };
    int u = get(0), v = get(u);
    // for (int x = v; x != -1; x = fa[x]) 是直径链
    return dis[v];
}

/**
 * 森林求每棵树的直径
 */
vector<int> forestDiameters(vector<vector<int>>& g) {
    int n = g.size();
    vector<char> vis(n);
    auto get = [&](int s) -> pair<int, int> {
    	int x = s, mx = 0;
    	auto dfs = [&](this auto&& dfs, int u, int fa, int d) -> void {
    		vis[u] = 1;
    		if (d > mx) mx = d, x = u;
    		for (auto& v : g[u]) {
    			if (v == fa) continue;
    			dfs(v, u, d + 1);
    		}
    	};
    	dfs(s, -1, 0);
    	return {x, mx};
    };
    vector<int> diameter(n, -1);
    for (int i = 0; i < n; ++i) {
        if (vis[i]) continue;
        auto [u, _] = get(i);
        auto [v, d] = get(u);
        diameter[i] = d;
    }
    return diameter;
}


/**
 * 一些关于直径的结论【重要】：
 * 
 * 1. 直径端点一定是叶子（即 deg=1）
 * 
 * 2. 设 a, b 为直径端点，对于树上任一点 x，有：
 *   【ecc(x) = max(dis(x,a), dis(x,b))】，其中 ecc(x) 代表 x 到全树最远点的距离
 * 
 * 3. 设 a, b 为直径端点，直径为 D，那么 x 在直径上当且仅当【dis(x,a) + dis(x,b) = D】
 * 
 * 4. 定义半径 【R = min_x ecc(x)】
 *      在无权树中有 R = (D+1)/2；如果 D 为偶数，直径中心点为树中心，否则直径中间边两端点都是中心
 *      在有权树中有 R 为中心 C 到直径两端 (a,b) 的距离最大值 R = max(dis(C,a), dis(C,b))
 *          中心 C 的求法：假设已经求出直径两端 a,b，分别开始 DFS 并记录数组 disa, disb 代表 a 到节点，b 到节点的距离；
 *                       如果 max(disa[u],disb[u]) < max(disa[center],disb[center]), 就把 center 更新为 u。做完后 R = max(disa[center],disb[center])
 *  
 * 5. 设树中心为 c，半径为 R，对于树上任一点 x，有：
 *   【ecc(x) = R + dis(x, C) = (D+1)/2 + dis(x, C)】
 * 
 * 6. 假设有两树直径分别为 d1, d2，选择第一棵树中的 u 和第二棵树中的 v，加一条长为 w 的边，
 *    设 e1 = ecc1(u), e2 = ecc2(v)，则连接后的直径为：【D' = max(d1, d2, e1 + w + e2)】；这是因为新直径要么在原树中，要么经过新边
 *          推论1：要连接两无权树使新直径最小，应当连接两树中心：【D' = max(d1, d2, (d1+1)/2 + (d2+1)/2 + 1)】
 *          推论2：要连接两有权树使新直径最小，应当连接两树中心；记 r1,r2 是原本两树的半径，d1,d2 是原本两树的直径，w 是加边的长度
 *                      - 新直径如何计算？【D' = max(d1, d2, r1+r2+w)】
 *                      - 新半径如何计算？【R' = min(max(r1, r2+w), max(r2, r1+w))】；注意取 min 的两项是原来两中点在新树中的离心率
 *                      - 新中心如何计算？【C' = c1 iff R'=max(r1, r2+w)】
 *          推论3：两树合并后直径端点一定可以从 a,b,c,d 即原直径端点中选出
 * 
 * 7. 假设 a, b 为直径端点，加入新点 x 并保证仍然为树，那么新直径长为
 *   【D' = max(D, dis(x,a), dis(x,b))】
 */