#include <bits/stdc++.h>
using namespace std;

/**
 * 离散化二维树状数组【动态二维点修改与矩形权值和】
 * 坐标使用原始坐标值；查询矩形为闭区间 [x1, x2] × [y1, y2]。
 * 预处理所有可能被修改的点，每个外层 BIT 节点仅离散化实际需要的 y 坐标。
 *
 * 支持操作：
 * init(points)：把将要出现的所有 (x,y) 加入，进行离散化处理
 * add(x, y, v)：点 (x, y) 的权值增加 v；
 * pref_sum(x, y)：查询 px <= x 且 py <= y 的点权和；
 * query(x1, y1, x2, y2)：查询矩形内点权和。
 *
 * 适用：修改坐标可提前获得的动态二维点修改、矩形计数、矩形权值和。
 *
 * 复杂度：预处理 O(n log^2 n)；修改、查询 O(log^2 n)；空间 O(n log n)。
 * 前置条件：所有可能调用 add(x, y, v) 的点必须在初始化时给出。
 */
struct BIT2D {
    int n;
    vector<int> xs;
    vector<vector<int>> ys;
    vector<vector<long long>> bit;
    BIT2D(const vector<pair<int, int>>& points) { init(points); }
    void init(const vector<pair<int, int>>& points) {
        xs.clear();
        for (auto [x, y] : points) xs.emplace_back(x);
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = xs.size();
        ys.assign(n + 1, {});
        bit.assign(n + 1, {});
        for (auto [x, y] : points) {
            int p = lower_bound(xs.begin(), xs.end(), x) - xs.begin() + 1;
            for (int i = p; i <= n; i += i & -i) ys[i].emplace_back(y);
        }
        for (int i = 1; i <= n; ++i) {
            sort(ys[i].begin(), ys[i].end());
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
            bit[i].assign(ys[i].size() + 1, 0);
        }
    }
    void bit_add(int u, int y, long long v) {
        int p = lower_bound(ys[u].begin(), ys[u].end(), y) - ys[u].begin() + 1;
        for (int i = p; i < bit[u].size(); i += i & -i) bit[u][i] += v;
    }
    long long bit_query(int u, int y) const {
        int p = upper_bound(ys[u].begin(), ys[u].end(), y) - ys[u].begin();
        long long res = 0;
        for (int i = p; i; i -= i & -i) res += bit[u][i];
        return res;
    }
    void add(int x, int y, long long v) {
        int p = lower_bound(xs.begin(), xs.end(), x) - xs.begin() + 1;
        for (int i = p; i <= n; i += i & -i) bit_add(i, y, v);
    }
    long long pref_sum(int x, int y) const {
        int p = upper_bound(xs.begin(), xs.end(), x) - xs.begin();
        long long res = 0;
        for (int i = p; i; i -= i & -i) res += bit_query(i, y);
        return res;
    }
    long long query(int x1, int y1, int x2, int y2) const {
        if (x1 > x2 || y1 > y2) return 0;
        return pref_sum(x2, y2) - pref_sum(x1 - 1, y2) - pref_sum(x2, y1 - 1) + pref_sum(x1 - 1, y1 - 1);
    }
};


/**
 * 离散化二维树状数组【矩形加与矩形和】
 * 坐标 0-based；修改与查询矩形均为闭区间。
 * 初始化时传入所有未来会修改到的差分点坐标。
 *
 * 支持操作：
 * init(pts)：对于所有会出现的点离散化。若有矩形 [x1, x2] × [y1, y2]，需要收集点 (x1,y1), (x1,y2+1), (x2+1,y1), (x2+1,y2+1)
 * add_rect(x1, y1, x2, y2, v)：矩形 [x1, x2] × [y1, y2] 增加 v；
 * query(x1, y1, x2, y2)：查询矩形 [x1, x2] × [y1, y2] 的权值和。
 *
 * 适用：坐标范围很大、实际出现的修改坐标较少的二维矩形修改与查询。
 *
 * 复杂度：
 * 初始化 O(n log^2 n)，修改与查询 O(log^2 n)，空间 O(n log n)。
 */
struct BIT2DRange {
    int n;
    vector<int> xs;
    vector<vector<int>> ys;
    vector<vector<long long>> b, bx, by, bxy;
    BIT2DRange(const vector<pair<int, int>>& points) { init(points); }
    void init(const vector<pair<int, int>>& points) {
        xs.clear();
        for (auto [x, y] : points) xs.emplace_back(x);
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = xs.size();
        ys.assign(n + 1, {});
        for (auto [x, y] : points) {
            int p = lower_bound(xs.begin(), xs.end(), x) - xs.begin() + 1;
            for (int i = p; i <= n; i += i & -i) ys[i].emplace_back(y);
        }
        b.resize(n + 1);
        bx.resize(n + 1);
        by.resize(n + 1);
        bxy.resize(n + 1);
        for (int i = 1; i <= n; ++i) {
            sort(ys[i].begin(), ys[i].end());
            ys[i].erase(unique(ys[i].begin(), ys[i].end()), ys[i].end());
            int m = ys[i].size();
            b[i].assign(m + 1, 0);
            bx[i].assign(m + 1, 0);
            by[i].assign(m + 1, 0);
            bxy[i].assign(m + 1, 0);
        }
    }
    void add(vector<vector<long long>>& bit, int x, int y, long long v) {
        int p = lower_bound(xs.begin(), xs.end(), x) - xs.begin() + 1;
        for (int i = p; i <= n; i += i & -i) {
            int q = lower_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin() + 1;
            for (int j = q; j < bit[i].size(); j += j & -j) bit[i][j] += v;
        }
    }
    long long pref(const vector<vector<long long>>& bit, int x, int y) {
        long long res = 0;
        int p = upper_bound(xs.begin(), xs.end(), x) - xs.begin();
        for (int i = p; i > 0; i -= i & -i) {
            int q = upper_bound(ys[i].begin(), ys[i].end(), y) - ys[i].begin();
            for (int j = q; j > 0; j -= j & -j) res += bit[i][j];
        }
        return res;
    }
    void add(int x, int y, long long v) {
        add(b, x, y, v);
        add(bx, x, y, v * x);
        add(by, x, y, v * y);
        add(bxy, x, y, v * x * y);
    }
    void add_rect(int x1, int y1, int x2, int y2, long long v) {
        add(x1, y1, v);
        add(x1, y2 + 1, -v);
        add(x2 + 1, y1, -v);
        add(x2 + 1, y2 + 1, v);
    }
    long long pref_sum(int x, int y) {
        if (x < 0 || y < 0) return 0;
        long long X = x + 1, Y = y + 1;
        return X * Y * pref(b, x, y) - Y * pref(bx, x, y) - X * pref(by, x, y) + pref(bxy, x, y);
    }
    long long query(int x1, int y1, int x2, int y2) { return pref_sum(x2, y2) - pref_sum(x1 - 1, y2) - pref_sum(x2, y1 - 1) + pref_sum(x1 - 1, y1 - 1); }
};