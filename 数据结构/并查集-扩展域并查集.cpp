#include <bits/stdc++.h>
using namespace std;

struct UFS {
	int n, cc;
    int mx;
	vector<int> pa, sz;
	UFS(int _n) : n(_n), cc(_n), mx(1) {
		pa.resize(n), sz.resize(n);
		for (int i = 0; i < n; ++i) pa[i] = i, sz[i] = 1;
	}
	int find(int x) {
        if (pa[x] != x) pa[x] = find(pa[x]);
        return pa[x];
    }
    void unite(int x, int y) {
    	int px = find(x), py = find(y);
    	if (px == py) return;
    	if (sz[px] < sz[py]) swap(px, py);
    	pa[py] = px;
    	sz[px] += sz[py], sz[py] = 0;
        --cc, mx = max(mx, sz[px]);
    }
    bool check(int x, int y) { return find(x) == find(y); }
    int size(int x) { return sz[find(x)]; }
};
/**
 * 扩展域并查集；维护二元互斥关系
 * 
 * 题目给定多个敌对/盟友（or 奇/偶 or 天敌/食物）关系，检查哪些条件和之前矛盾；
 * 本质是拆点，将一个点拆为两个元：x 拆为 x_friend 和 x_enemy
 * 然后利用“敌人的敌人就是朋友”关系来检查矛盾
 *
 * x 表示 x 所在阵营，x + n 表示 x 的对立阵营
 * - 若 x 和 y 同类： 合并 x 和 y，合并 x + n 和 y + n
 * - 若 x 和 y 敌对： 合并 x 和 y + n，合并 x + n 和 y
 */
void solve() {
    int n, q; cin >> n >> q;
    UFS s(2 * n);
    int res = 0;                        // 矛盾话数
    for (int i = 0; i < q; ++i) {
        int op, x, y; cin >> op >> x >> y; --x, --y;
        if (op == 1) {                  // x 和 y 同类
            if (s.check(x, y + n)) {
                ++res;
            } else {
                s.unite(x, y);
                s.unite(x + n, y + n);
            }
        } else {                        // x 和 y 敌对
            if (s.check(x, y)) {
                ++res;
            } else {
                s.unite(x, y + n);
                s.unite(x + n, y);
            }
        }
    }
    cout << res << "\n";
}