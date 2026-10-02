#include <bits/stdc++.h>
using namespace std;

// using ll = long long;
using ll = long double;
using ld = long double;

/** 三维整数点 */
struct P {
    ll x, y, z;
    P() : x(0), y(0), z(0) {}
    P(ll _x, ll _y, ll _z) : x(_x), y(_y), z(_z) {}
    P operator+(const P& o) const { return P(x + o.x, y + o.y, z + o.z); }
    P operator-(const P& o) const { return P(x - o.x, y - o.y, z - o.z); }
};

/** 三维向量点积，参数视为从原点出发的向量 */
ll dot(const P& a, const P& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

/** 三维向量叉积 a x b */
P cross(const P& a, const P& b) {
    return P(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

/** 标量三重积 det(a,b,c) = a · (b × c)，用于四面体体积计算 */
ll det(const P& a, const P& b, const P& c) { return dot(a, cross(b, c)); }

/** 调整单个凸多边形面顶点顺序使其相对于已知内部点具有外法向（必要时整体反转） */
void adjust(const P& inner, vector<P>& face) {
    int m = face.size();
    if (m < 3) return;
    P &a = face[0], &b = face[1], &c = face[2];
    P n = cross(b - a, c - a);  // 平面法向量
    ll s = dot(n, inner - a);  // 与法向量点乘，=0在平面上，>0在法向量同侧，<0在法向量异侧
    if (s > 0) reverse(face.begin(), face.end());
}

/** 计算凸多面体体积，faces 为若干凸多边形面顶点序列（可任意方向），inner 为多面体内部一点 */
ld polyhedronVolume(vector<vector<P>>& faces, const P& inner) {
    ld s = 0;
    for (auto& f : faces) {
        adjust(inner, f);
        int m = f.size();
        if (m < 3) continue;
        P o = f[0];
        for (int i = 1; i + 1 < m; ++i) {
            s += (ld)det(o, f[i], f[i + 1]);
        }
    }
    if (s < 0) s = -s;
    return s / (ld)6;
}

/**
 * 示例用法，内点选择多边形所有点的平均点
 */
int main() {
    int f; cin >> f;
    vector<vector<P>> poly(f);
    ll xs = 0, ys = 0, zs = 0, cnt = 0;
    for (int i = 0; i < f; ++i) {
        int v; cin >> v;
        poly[i].resize(v);
        for (int j = 0; j < v; ++j) {
            ld x, y, z; cin >> x >> y >> z;
            poly[i][j] = {x, y, z};
            xs += x, ys += y, zs += z, ++cnt;
        }
    }
    P Pin(xs / cnt, ys / cnt, zs / cnt);
    cout << fixed << setprecision(2) << polyhedronVolume(poly, Pin) << '\n';
    return 0;
}