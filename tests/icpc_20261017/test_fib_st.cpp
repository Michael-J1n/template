#include <bits/stdc++.h>
using namespace std;
#include "../../数学/斐波那契数.cpp"
#include "../../数据结构/ST表-二维.cpp"
struct MaxOp {
    int operator()(int x, int y) const { return max(x, y); }
};
struct GcdOp {
    int operator()(int x, int y) const { return gcd(x, y); }
};
struct Matrix {
    long long a, b, c, d;
};
Matrix multiply(Matrix x, Matrix y) {
    return {(x.a * y.a + x.b * y.c) % mod, (x.a * y.b + x.b * y.d) % mod,
            (x.c * y.a + x.d * y.c) % mod, (x.c * y.b + x.d * y.d) % mod};
}
int fib_oracle(long long n) {
    Matrix a{1, 0, 0, 1}, b{1, 1, 1, 0};
    while (n) {
        if (n & 1) a = multiply(a, b);
        b = multiply(b, b);
        n >>= 1;
    }
    return a.b % mod;
}
int main() {
    static_assert(is_same_v<decltype(fib(0LL)), int>);
    mt19937_64 rng(20261017);
    int a = 0, b = 1 % mod;
    for (int n = 0; n <= 100000; ++n) {
        assert(fib(n) == a);
        int c = (1LL * a + b) % mod;
        a = b, b = c;
    }
    vector<long long> large{LLONG_MAX, LLONG_MAX - 1, 0, 1};
    for (int bit = 0; bit < 63; ++bit) {
        large.push_back(1LL << bit);
        large.push_back((1LL << bit) - 1);
    }
    for (int i = 0; i < 2000; ++i) large.push_back(rng() & LLONG_MAX);
    for (long long n : large) assert(fib(n) == fib_oracle(n));
    long long checks = 0;
    for (int n = 1; n <= 7; ++n) for (int m = 1; m <= 8; ++m) {
        vector<vector<int>> grid(n, vector<int>(m));
        for (auto& row : grid) for (auto& x : row) x = int(rng() % 2001) - 1000;
        grid[0][0] = INT_MAX;
        grid[n - 1][m - 1] = INT_MIN;
        ST2D<int> st(grid);
        ST2D<int, MaxOp> mx(grid, MaxOp(), INT_MIN);
        vector<int> xs{INT_MIN, -2, -1, INT_MAX}, ys = xs;
        for (int i = 0; i <= n + 1; ++i) xs.push_back(i);
        for (int j = 0; j <= m + 1; ++j) ys.push_back(j);
        for (int x1 : xs) for (int x2 : xs) for (int y1 : ys) for (int y2 : ys) {
            int mn = INT_MAX, ma = INT_MIN;
            for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j)
                if (x1 <= i && i <= x2 && y1 <= j && j <= y2) mn = min(mn, grid[i][j]), ma = max(ma, grid[i][j]);
            assert(st.query(x1, y1, x2, y2) == mn);
            assert(mx.query(x1, y1, x2, y2) == ma);
            ++checks;
        }
    }
    vector<vector<int>> grid{{6, 9, 15}, {12, 18, 30}};
    ST2D<int, GcdOp> st(grid, GcdOp(), 0);
    assert(st.query(0, 0, 1, 2) == 3);
    assert(st.query(1, 1, 1, 2) == 6);
    assert(st.query(2, 0, 1, 2) == 0);
    cout << "fib/ST2D PASS mod=" << mod << " rectangle_cases=" << checks << '\n';
}
