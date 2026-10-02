#include <bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;

/**
 * Berlekamp-Massey 求最短线性递推系数
 *
 * 状态定义：
 * 给定序列 a[0..n-1]，求最短 k 阶线性递推：
 * a[i] = c[0] * a[i - 1] + c[1] * a[i - 2] + ... + c[k - 1] * a[i - k]
 *
 * 参数：
 * a[i] 表示序列第 i 项，从 a[0] 开始
 * 若已知递推阶数上界为 K，通常传入至少 2K 项
 *
 * 复杂度：
 * O(n^2)
 *
 * 注意：
 * 返回的 c 顺序是从近到远，和 Kitamasa 模板一致
 * 需要 mod 为质数，且非零误差 d 可以取逆
 */
vector<int> berlekampMassey(vector<int> a) {
    auto norm = [&](long long x) {
        x %= mod;
        if (x < 0) x += mod;
        return x;
    };
    auto qpow = [&](long long a, long long b) {
        long long res = 1;
        while (b) {
            if (b & 1) res = res * a % mod;
            a = a * a % mod, b >>= 1;
        }
        return res;
    };
    for (auto& x : a) x = norm(x);
    vector<int> C{1}, B{1};
    int L = 0, m = 1, b = 1;
    for (int i = 0; i < a.size(); ++i) {
        long long d = a[i];
        for (int j = 1; j <= L; ++j) {
            d = (d + 1LL * C[j] * a[i - j]) % mod;
        }
        d = norm(d);
        if (!d) {
            ++m;
            continue;
        }
        vector<int> T = C;
        long long coef = d * qpow(b, mod - 2) % mod;
        if (C.size() < B.size() + m) C.resize(B.size() + m);
        for (int j = 0; j < B.size(); ++j) {
            C[j + m] = norm(C[j + m] - coef * B[j]);
        }
        if (L * 2 <= i) {
            L = i + 1 - L;
            B = T;
            b = d;
            m = 1;
        } else {
            ++m;
        }
    }
    C.resize(L + 1);
    vector<int> c(L);
    for (int i = 0; i < L; ++i) {
        c[i] = norm(-C[i + 1]);
    }
    return c;
}

/**
 * Kitamasa 求线性递推第 n 项
 *
 * 状态定义：
 * 已知 k 阶线性递推：
 * f[i] = c[0] * f[i - 1] + c[1] * f[i - 2] + ... + c[k - 1] * f[i - k]
 *
 * 参数：
 * a[i] 表示初始值 f[i]，传入 f[0..k-1]
 * c[i] 表示 f[x - i - 1] 的系数，顺序从近到远
 *
 * 复杂度：
 * O(k^2 log n)
 *
 * 注意：
 * a 和 c 长度必须相同
 * a 从 f[0] 开始给
 * 缺项系数补 0
 */
int linearRecurrence(vector<int> a, vector<int> c, long long n) {
    int k = a.size();
    auto norm = [&](long long x) {
        x %= mod;
        if (x < 0) x += mod;
        return x;
    };
    for (auto& x : a) x = norm(x);
    for (auto& x : c) x = norm(x);
    if (n < k) return a[n];
    auto mul = [&](const vector<int>& x, const vector<int>& y) {
        vector<int> t(k * 2 - 1);
        for (int i = 0; i < k; ++i) {
            if (!x[i]) continue;
            for (int j = 0; j < k; ++j) {
                t[i + j] = (t[i + j] + 1LL * x[i] * y[j]) % mod;
            }
        }
        for (int i = k * 2 - 2; i >= k; --i) {
            if (!t[i]) continue;
            for (int j = 0; j < k; ++j) {
                t[i - 1 - j] = (t[i - 1 - j] + 1LL * t[i] * c[j]) % mod;
            }
        }
        t.resize(k);
        return t;
    };
    vector<int> res(k), base(k);
    res[0] = 1;
    if (k == 1) base[0] = c[0];
    else base[1] = 1;
    while (n) {
        if (n & 1) res = mul(res, base);
        base = mul(base, base);
        n >>= 1;
    }
    int ans = 0;
    for (int i = 0; i < k; ++i) {
        ans = (ans + 1LL * res[i] * a[i]) % mod;
    }
    return ans;
}

/**
 * BM + Kitamasa 求线性递推远项
 *
 * 状态定义：
 * f[i] 表示要求的同一个一维序列的第 i 项
 * 若存在 K 维线性状态 V[i]，满足 V[i] = M * V[i - 1]，且 f[i] 是 V[i] 的线性组合
 * 则 f[i] 满足阶数 <= K 的线性递推
 *
 * 流程：
 * 先生成 f[0..2K-1]
 * 用 BM 求最短递推系数 c
 * 保留 f[0..c.size()-1] 作为 Kitamasa 初始值
 * 用 Kitamasa 求 f[n]
 *
 * 复杂度：
 * 生成序列复杂度看具体 DP
 * BM O(K^2)
 * Kitamasa O(k^2 log n)，k 为最短递推阶数
 *
 * 注意：
 * seq 必须是同一个序列的连续前缀，不能把特殊项混进去
 * 若题目 ans[n] 和 seq[t] 下标不同，最后自己换算 t
 */
void solve() {
    long long n; cin >> n;
    int K; cin >> K;  // 递推阶数上界，通常是状态维度
    vector<int> seq;
    // 初始化状态 V[0]
    
    // 这里就类似于普通 DP，进行当前 DP 值计算与状态转移；进行 2*K 次
    auto get = [&]() -> int {
        int cur = 0;
        // cur = f[seq.size()]
        return cur;
    };
    auto step = [&]() -> void {
        // V[i] -> V[i + 1]
    };
    while (seq.size() < K * 2) {
        seq.push_back(get());
        step();
    }
    
    vector<int> c = berlekampMassey(seq);
    if (c.empty()) {
        cout << 0 << '\n';
        return;
    }
    seq.resize(c.size());
    cout << linearRecurrence(seq, c, n) << '\n';
}