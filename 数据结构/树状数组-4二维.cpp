#include <bits/stdc++.h>
using namespace std;

/**
 * 二维树状数组模板
 * O(lognlogm) 的单次修改和动态查询子矩阵和
 *
 * add(x, y, v);                  // a[x][y] += v
 * set(x, y, v);                  // a[x][y] 设为 v
 * pref_sum(x, y);                // sum a[0..x][0..y]
 * range_sum(x1, x2, y1, y2);     // sum a[x1..x2][y1..y2]
 * get(x, y);                     // 获取最新的 a[x][y]
 * 前置条件：构造矩阵非空且各行等长；修改坐标合法；查询矩形非空且边界合法
 */
struct BIT2D {
    int n, m;
    vector<vector<int>> c;
    BIT2D(int _n, int _m) : n(_n), m(_m) {
        c.assign(n + 1, vector<int>(m + 1));
    }
    BIT2D(vector<vector<int>>& a) : n(a.size()), m(a[0].size()) {
        c.assign(n + 1, vector<int>(m + 1));
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) c[i][j] = a[i - 1][j - 1];
        }
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                int nj = j + (j & -j);
                if (nj <= m) c[i][nj] += c[i][j];
            }
        }
        for (int i = 1; i <= n; ++i) {
            int ni = i + (i & -i);
            if (ni <= n) {
                for (int j = 1; j <= m; ++j) c[ni][j] += c[i][j];
            }
        }
    }
    void add(int x, int y, int val) {
        ++x, ++y;
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) c[i][j] += val;
        }
    }
    void set(int x, int y, int val) { add(x, y, val - get(x, y)); }
    int pref_sum(int x, int y) {
        ++x, ++y;
        int sum = 0;
        for (int i = x; i >= 1; i &= i - 1) {
            for (int j = y; j >= 1; j &= j - 1) sum += c[i][j];
        }
        return sum;
    }
    int range_sum(int x1, int x2, int y1, int y2) {
        if (x1 > x2 || y1 > y2) return 0;
        int sum = pref_sum(x2, y2);
        if (x1) sum -= pref_sum(x1 - 1, y2);
        if (y1) sum -= pref_sum(x2, y1 - 1);
        if (x1 && y1) sum += pref_sum(x1 - 1, y1 - 1);
        return sum;
    }
    int get(int x, int y) { return range_sum(x, x, y, y); }
};
