<!-- KACTL 打印样式：A4 横向三栏；建议使用同名 PDF 直接打印。 -->
<style>
@page {
  size: A4 landscape;
  margin: 8mm 6mm 6mm;
  @top-left {
    content: "Regional · 额外打印";
    font: 6pt "SimSun", serif;
  }
  @top-right {
    content: counter(page) " / " counter(pages);
    font: 6pt "Consolas", monospace;
  }
}
@media print {
  html, body {
    margin: 0 !important;
    padding: 0 !important;
    max-width: none !important;
    background: white !important;
    color: black !important;
  }
  body, .markdown-body, .markdown-preview {
    column-count: 3;
    column-gap: 4mm;
    column-rule: 0.2pt solid #999;
    column-fill: auto;
    font: 7pt/1.1 "SimSun", serif;
  }
  body:has(.markdown-body, .markdown-preview) { column-count: auto; }
  h1, h2, h3, h4 {
    padding: 0 !important;
    break-after: avoid;
    page-break-after: avoid;
    font-family: "SimHei", "SimSun", sans-serif;
    color: black !important;
  }
  h1, h2 {
    margin: 1.5mm 0 0.7mm !important;
    border-bottom: 0.4pt solid black;
    font-size: 9pt !important;
    line-height: 1.1 !important;
  }
  h3, h4 {
    margin: 1.2mm 0 0.4mm !important;
    border: 0 !important;
    font-size: 8pt !important;
    line-height: 1.1 !important;
  }
  pre, pre code {
    margin: 0 !important;
    padding: 0 !important;
    border: 0 !important;
    border-radius: 0 !important;
    background: transparent !important;
    box-shadow: none !important;
    font: 7pt/1.1 "Consolas", "SimSun", monospace !important;
    tab-size: 2;
    white-space: pre-wrap !important;
    overflow-wrap: anywhere;
    word-break: normal;
    overflow: visible !important;
    break-inside: auto !important;
    page-break-inside: auto !important;
    orphans: 2;
    widows: 2;
  }
  pre { margin-bottom: 1mm !important; }
  p, ul, ol { margin: 0.5mm 0 !important; }
  a { color: black !important; text-decoration: none; }
  .code-line { display: block; break-inside: avoid; }
}
</style>

## 杂项

### 1. 随机引擎
```CPP
/**
 * 随机数生成引擎
 * 区间约定：整数 [l, r]；浮点数 [l, r)。
 */
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int random_int(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }
long long random_ll(long long l, long long r) { return uniform_int_distribution<long long>(l, r)(rng); }
double random_double(double l, double r) { return uniform_real_distribution<double>(l, r)(rng); }
```

### 2. 随机哈希
```CPP
/**
 * 随机异或哈希；每个元素随机分配 64-bit 值
 *
 * 支持操作：
 * get(a)：计算集合 a 的哈希，O(|a|)
 * toggle(h, x)：切换元素 x 的存在状态，O(1)
 *
 * 适用：集合判等、动态集合维护、集合分组。不同集合的单次哈希碰撞概率为 2^-64
 * 复杂度：O(n)
 */
struct xorHash {
  using u64 = uint64_t;
  vector<u64> w;
  xorHash(int n) : w(n) {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    for (auto& x : w) x = rng();
  }
  u64 get(const vector<int>& a) const {
    u64 h = 0;
    for (int x : a) h ^= w[x];
    return h;
  }
  void toggle(u64& h, int x) const { h ^= w[x]; }
};

/**
 * 前缀哈希
 *
 * 返回数组 a 的前缀哈希，去重集合语义；用于判断前缀值域范围是否相同
 * 复杂度：O(n)
 */
vector<uint64_t> preHash(vector<int>& a) {
  using u64 = uint64_t;
  mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
  int n = a.size();
  vector<u64> h(n);
  unordered_map<int, u64> mp;
  unordered_set<int> has;
  for (int i = 0; i < n; ++i) {
    if (has.count(a[i])) {
      h[i] = h[i - 1];
      continue;
    }
    if (!mp.count(a[i])) mp[a[i]] = rng();
    h[i] = (i ? h[i - 1] : 0) ^ mp[a[i]];
    has.insert(a[i]);
  }
  return h;
}

/**
 * Hyperregular Trick
 *
 * 索引：位置与区间 ID 均为 0-based，区间为闭区间 [l, r]。每个区间分配独立随机 64-bit 权值，通过异或差分计算每个位置被覆盖的区间集合哈希。
 * 返回 h，其中 h[i] 表示覆盖位置 i 的区间 ID 集合哈希 = XOR_{区间id覆盖i}(w[id])
 *
 * 复杂度：O(n + m)
 */
vector<uint64_t> build_hash(int n, const vector<pair<int, int>>& seg) {
  using u64 = uint64_t;
  mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
  vector<u64> diff(n + 1), h(n);
  for (auto [l, r] : seg) {
    u64 x = rng();
    diff[l] ^= x;
    diff[r + 1] ^= x;
  }
  u64 cur = 0;
  for (int i = 0; i < n; i++) {
    cur ^= diff[i];
    h[i] = cur;
  }
  return h;
}
```

## DP

### 1. 凸包优化
```CPP
/**
 * Convex Hull Trick（一般形，上凸壳）优化 DP：dp[x] = max{mi * x + bi}，其中 x 为关于 i 的函数，mi 与 bi 为关于 j 的函数
 *
 * 维护一组直线 y = m * x + b，支持：
 *   - add(m, b): 在 O(log n) 时间插入一条直线
 *   - query(x):  在 O(log n) 时间查询 max(m * x + b)
 *
 * 注意：
 * 1. 斜率、查询顺序都不需要单调；允许负值
 * 2. 若要求 min，可在插入时用 add(-m, -b)，查询后再取相反数
 * 3. 如果一开始没有，也要先插入一条(0,0)线
 */
struct Line {
  using ll = long long;
  ll m, b;
  mutable long double x;
  bool operator<(const Line &o) const { return m < o.m; }
  bool operator<(long double v) const { return x < v; }
};
struct LineContainer : multiset<Line, less<>> {
  using ll = long long;
  static constexpr long double INF = 1e30L;
  static long double intersect(const Line &a, const Line &b) {
    if (a.m == b.m) return a.b > b.b ? INF : -INF;
    return (long double)(b.b - a.b) / (long double)(a.m - b.m);
  }
  bool isect(iterator x, iterator y) {
    if (y == end()) {
      x->x = INF;
      return false;
    }
    if (x->m == (*y).m) x->x = x->b > (*y).b ? INF : -INF;
    else x->x = intersect(*x, *y);
    return x->x >= (*y).x;
  }
  void add(ll m, ll b) {
    auto z = insert({m, b, 0}), y = z++, x = y;
    while (isect(y, z)) z = erase(z);
    if (x != begin() && isect(--x, y)) isect(x, erase(y));
    while ((y = x) != begin() && (--x)->x >= y->x) isect(x, erase(y));
  }
  ll query(ll x) const {
    assert(!empty());
    auto it = lower_bound((long double)x);
    if (it == end()) --it;
    return it->m * x + it->b;
  }
};
```

### 2. 矩阵优化
```CPP
/**
 * 可以使用矩阵加速的状态转移，快速求作用 k 次后的结果
 * 例如 dp[t+1][j] = sum_i(dp[t][i] * w[i][j])；具体需要自己写出矩阵递推式
 *
 * 复杂度：O(n^3 logk)
 */
using Matrix = vector<vector<long long>>;
Matrix mul(const Matrix& a, const Matrix& b) {
  int n = a.size(), m = b.size(), k = b[0].size();
  Matrix c(n, vector<long long>(k));
  for (int i = 0; i < n; ++i) {
    for (int t = 0; t < m; ++t) {
      if (!a[i][t]) continue;
      for (int j = 0; j < k; ++j) {
        (c[i][j] += a[i][t] * b[t][j] % mod) %= mod;
      }
    }
  }
  return c;
}
Matrix qpow(Matrix a, long long b) {
  int n = a.size();
  Matrix res(n, vector<long long>(n));
  for (int i = 0; i < n; ++i) res[i][i] = 1;
  while (b) {
    if (b & 1) res = mul(res, a);
    b >>= 1;
    if (b) a = mul(a, a);
  }
  return res;
}
```

### 3. min-plus 快速幂优化
```CPP
/**
 * Min-Plus 矩阵快速幂；加速“恰好走 k 次转移”的 DP
 * 对于类似 dp(a+b,i,j) = min_k (dp(a,i,k) + dp(b,k,j)) 或 dp(i,x) = min_v (dp(i-1,v) + cost(v,u)) 的转移，使用快速幂将 n^3k 优化为 n^3logk
 * 关键在于结构 F_(a+b) = F_a ⊗ F_b，其中 ⊗ 为 min_plus 卷积。
 * 若需要 max-plus，将取 min 改为取 max 即可
 *
 * 基础转移矩阵 A：
 * A[i][j] = 状态 i 经过 1 次转移到状态 j 的最小代价
 * Min-Plus 乘法：C[i][j] = min_k(A[i][k] + B[k][j])；表示把“i -> k 的一段转移”和“k -> j 的一段转移”拼接
 * 因此 A^k[i][j] = 状态 i 恰好经过 k 次转移到状态 j 的最小代价
 *
 * 复杂度：O(n^3 log k)
 *
 * 注意：min-plus 矩阵的单位元为 {{0,LINF},{LINF,0}};
 */
using Matrix = vector<vector<long long>>;
Matrix mul(const Matrix& a, const Matrix& b) {
  int n = a.size();
  vector c(n, vector<long long>(n, LINF));
  for (int i = 0; i < n; ++i) {
    for (int k = 0; k < n; ++k) {
      if (a[i][k] == LINF) continue;
      for (int j = 0; j < n; ++j) {
        if (b[k][j] == LINF) continue;
        c[i][j] = min(c[i][j], a[i][k] + b[k][j]);
      }
    }
  }
  return c;
}
vector<long long> mul(const Matrix& a, const vector<long long>& b) {
  int n = a.size();
  vector<long long> c(n, LINF);
  for (int i = 0; i < n; ++i) {
    for (int k = 0; k < n; ++k) {
      if (a[i][k] == LINF || b[k] == LINF) continue;
      c[i] = min(c[i], a[i][k] + b[k]);
    }
  }
  return c;
}
Matrix qpow(Matrix a, long long k) {
  int n = a.size();
  vector res(n, vector<long long>(n, LINF));
  for (int i = 0; i < n; ++i) res[i][i] = 0;
  while (k) {
    if (k & 1) res = mul(res, a);
    a = mul(a, a), k >>= 1;
  }
  return res;
}
```

### 4. 数位 DP
```CPP
/**
 * 数位 DP 模板 2 -- 合法数字数位之和
 *
 * 求 [low, high] 之间的符合要求的所有整数的价值之和（例如 f(x)=x 代表满足约束的元素和，f(x)=digitsum(x) 代表满足约束的元素的数位和）
 * 模板中以不同数字不超过 k 个的数位之和为例
 * 数位 DP 可以看作一种树形 DP，在计算价值之和时使用树形 DP 相应的转移式思考技巧即可
 *
 * 参数：
 * low = 数字下限；
 * high = 数字上限；
 * 【!】k = 题目要求有关的状态数，可选；
 * lead_zero = 前导零是否会影响答案；即若补上前导零会改变状态判断，就传 true
 *
 * 返回：
 * [low, high] 之间满足约束的整数的数位价值和
 *
 * 复杂度：
 * O(kn)，其中其中 k 为特殊状态数，n 为十进制数位长度（即 lgV）
 */
long long digitDPContribution(long long low, long long high, int k, bool lead_zero = true) {
  string low_s = to_string(low), high_s = to_string(high);
  int n = high_s.size();
  int diff_lh = n - low_s.size();  // 数位对齐的偏移量

  vector memo(n, vector<pair<long long, long long>>(1 << 10, {-1, -1}));  // 第一维度固定总长度 n；第二维度按照题目带来的额外状态（pre 或 cnt 等）设置
  /**
   * 固定前缀，枚举第 i 位，记忆化搜索 {子树中的合法数个数，子树中的合法数价值和}【子树根为 i，此时除去自己，到叶子还有 n - 1 - i 的深度距离】
   * i：从高到低第几位
   * 【!】mask：当前填过数字的比特集合【这一位状态根据题目要求更改；可以是数字中的某种计数 cnt，也可以是数字中前一位填写的数字 pre 等】
   * limit_low：前面的数位是否都贴着下限
   * limit_high：前面的数位是否都贴着上限
   */
  auto dfs = [&](this auto&& dfs, int i, int mask, bool limit_low, bool limit_high) -> pair<long long, long long> {
    if (i == n) {  // 填完所有位数，返回当前是否满足条件（没有额外条件返回 {1,0}）【这里表示搜到叶子的再下一层，即空后缀上；是一种合法方案，但数位空无贡献】
      return {1, 0};
    }
    if (!limit_low && !limit_high && memo[i][mask].first >= 0) return memo[i][mask];
    int lo = limit_low && i >= diff_lh ? low_s[i - diff_lh] - '0' : 0;
    int hi = limit_high ? high_s[i] - '0' : 9;

    /** ------------------------------------------------------------------------------------------- */
    long long cnt = 0, sum = 0;
    int d = lo;
    if (lead_zero && limit_low && i < diff_lh) {  // 若前导零影响答案，先跳过当前前导空位（不考虑第 i 位产生的贡献，即当前 d = NONE）；再把当前位调整为 1
      auto [sub_cnt, sub_sum] = dfs(i + 1, 0, true, false);
  // 当前位无效，只有后缀中的数字才有机会贡献
      cnt += sub_cnt;
      sum += sub_sum;
  // cnt %= mod; sum %= mod;
      d = 1;
    }
    for (; d <= hi; ++d) {
  // 枚举第 i 位填 d；根据题目要求在当前状态不合法的时候 continue
      int nmask = mask | 1 << d;
      if (popcount((uint32_t)nmask) > k) continue;
  // sum 根据具体价值函数类似树形 DP 的更新；这里是数位和，若是数字和，改成 sum += sub_sum + d * pw[n - i - 1] * sub_cnt，其中 pw[t] = 10^t%mod 是预处理的幂
  // 对于 sum，即价值总和的更新：首先后缀中的数位对于总价值的贡献和即为 Σsub_sum；再考虑当前位的贡献：若合法，当前 d 的价值会贡献 sub_cnt 次（可以任选一个接上）
      auto [sub_cnt, sub_sum] = dfs(i + 1, nmask, limit_low && d == lo, limit_high && d == hi);
      cnt += sub_cnt;
      sum += sub_sum + sub_cnt * d;
  // cnt %= mod; sum %= mod;
    }
    /** ------------------------------------------------------------------------------------------- */

    pair<long long, long long> res = {cnt, sum};
    if (!limit_low && !limit_high) memo[i][mask] = res;
    return res;
  };
  return dfs(0, 0, true, true).second;
}
```

## 字符串

### 1. Manacher
```CPP
/**
 * Manacher【线性预处理所有回文半径】
 * d1[i]：以 i 为中心的奇数回文半径，包含中心本身；d2[i]：以 i - 1 和 i 之间为中心的偶数回文半径。下标 0-based，区间使用闭区间 [l, r]。
 *
 * 重要性质：
 * 以   i   为中心的奇回文：[i-d1[i]+1, i+d1[i]-1]，长度 d1[i]*2-1，共有 d1[i] 个
 * 以 i-1|i 为中心的偶回文：[i-d2[i]  , i+d2[i]-1]，长度 d2[i]*2  ，共有 d2[i] 个
 * 从 i 开始的最长回文：[i, rp[i]]，长度 rp[i]-i+1
 * 以 i 结尾的最长回文：[lp[i], i]，长度 i-lp[i]+1
 *
 * 支持操作：
 * Manacher(s)：预处理字符串 s 的 d1, d2 信息。
 * Manacher(a)：预处理数组 a 的 d1, d2 信息。
 * check(l, r)：O(1) 判断区间 [l, r] 是否为回文。
 * get(lp, rp)：lp[i] = 以 i 为右端点的最长回文的最左端点；rp[i] = 以 i 为左端点的最长回文的最右端点。
 * count(st, ed)：st[i] = 以 i 为开头的回文串总数；ed[i] = 以 i 为结尾的回文串总数。
 *
 * 适用：静态序列回文判断、最长回文、回文半径统计。
 *
 * 复杂度：预处理 O(n)，单次查询 O(1)，空间 O(n)。
 */
struct Manacher {
  int n;
  string s;
  vector<int> d1, d2;
  Manacher(string s) : s(s), n(s.size()), d1(n), d2(n) {
  // d1[i]：以 i 为中心的奇回文半径，包含 i；[l, r] 为当前最靠右的奇回文
  // l -- (----- l+r-1 -----) || (----- i -----) -- r
  //       <-k->       <-k->      <-k->   <-k->
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
  // 镜像位置 l + r - i；进入 while 前 [i - k + 1, i + k - 1] 已知为回文
      int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
      while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
      d1[i] = k--;
      if (i + k > r) l = i - k, r = i + k;
    }
  // d2[i]：以 i - 1 | i 为中心的偶回文半径；[l, r] 为当前最靠右的偶回文
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
  // 镜像中心 l + r - i + 1；进入 while 前 [i - k, i + k - 1] 已知为回文
      int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
      while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
      d2[i] = k--;
      if (i + k > r) l = i - k - 1, r = i + k;
    }
  }
  Manacher(const vector<int>& a) : n(a.size()), d1(n), d2(n) {
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
      int k = i > r ? 1 : min(d1[l + r - i], r - i + 1);
      while (i - k >= 0 && i + k < n && a[i - k] == a[i + k]) k++;
      d1[i] = k--;
      if (i + k > r) l = i - k, r = i + k;
    }
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
      int k = i > r ? 0 : min(d2[l + r - i + 1], r - i + 1);
      while (i - k - 1 >= 0 && i + k < n && a[i - k - 1] == a[i + k]) k++;
      d2[i] = k--;
      if (i + k > r) l = i - k - 1, r = i + k;
    }
  }
  bool check(int l, int r) {
    if (l > r) return false;
    int len = r - l + 1;
    if (len & 1) return d1[l + r >> 1] >= len / 2 + 1;
    return d2[l + r + 1 >> 1] >= len / 2;
  }
  void get(vector<int>& lp, vector<int>& rp) {  // 回文中心单调指针维护最大边界
    lp.resize(n), rp.resize(n);
    int p1 = 0, p2 = 0;
    for (int i = 0; i < n; ++i) {
  // 处理奇中心
      while (p1 <= i && p1 + d1[p1] - 1 < i) ++p1;
      lp[i] = 2 * p1 - i;  // [p1-(i-p1)..p1-1][p1][p1+1..i]
  // 处理偶中心
      while (p2 <= i && p2 + d2[p2] - 1 < i) ++p2;
      if (p2 <= i) lp[i] = min(lp[i], 2 * p2 - i - 1);
    }
    p1 = p2 = n - 1;
    for (int i = n - 1; i >= 0; --i) {
  // 处理奇中心
      while (p1 >= i && p1 - d1[p1] + 1 > i) --p1;
      rp[i] = 2 * p1 - i;
  // 处理偶中心
      while (p2 >= i && p2 - d2[p2] > i) --p2;
      if (p2 >= i) rp[i] = max(rp[i], 2 * p2 - i - 1);
    }
  }
  void count(vector<int>& st, vector<int>& ed) {  // 差分数组维护区间加
    st.assign(n + 1, 0), ed.assign(n + 1, 0);
    for (int i = 0; i < n; ++i) {
  // 处理奇中心
      st[i - d1[i] + 1]++, st[i + 1]--;
      ed[i]++, ed[i + d1[i]]--;
  // 处理偶中心
      if (d2[i]) {
        st[i - d2[i]]++, st[i]--;
        ed[i]++, ed[i + d2[i]]--;
      }
    }
    for (int i = 1; i <= n; ++i) st[i] += st[i - 1], ed[i] += ed[i - 1];
    st.pop_back(), ed.pop_back();
  }
};
```

### 2. KMP
```CPP
/**
 * KMP【字符串模式匹配与前缀函数】
 * pi[i] 表示 p[0..i] 的最长真前缀且同时为后缀的**长度**，即 p[0..pi[i]-1] = p[i-pi[i]+1..i]。
 * 注意若当前长度为 len（即 s[0..len-1]），跳 fail 链需要调用的是 pi[len - 1]。
 *
 * 支持操作：
 * KMP(p)：O(m) 预处理模式串 p 的前缀函数。
 * find(s)：返回 p 在 s 中第一次出现的起点，无解返回 -1。
 * find_all(s)：返回 p 在 s 中所有出现位置，允许重叠。
 * count(s)：统计 p 在 s 中出现次数，允许重叠。
 * borders()：返回 p 的所有非空 border 长度，从短到长排列，例如 "ababab" 返回 {2,4,6}。
 * period(): 返回 p 的最小周期长度（更多扩展见文档末尾）。
 * fail_tree(father, g): 在 s 上建立对应的失配树；father 数组保存父亲关系，g 存树；
 *                       fail 树上的节点 len，表示原串前缀 s[0..len-1]（注意是前缀长度，而不是结尾下标）
 *                       父亲定义为 father[len] = pi[len - 1]，即节点 len 的父亲 = 这个前缀的最长真 border，节点 len 的祖先链 = 长度从大到小的所有 border。
 *
 * 适用：单模式串匹配、多个文本查询、border / 周期相关问题。
 * 复杂度：预处理 O(m)；单次匹配 O(n)；空间 O(m)。
 */
struct KMP {
  int m;
  string p;
  vector<int> pi;
  KMP(const string& p) : m(p.size()), p(p), pi(m) {
  // 预处理 pi，pi[i] = j 表示 p[0..j-1] = p[i-j+1..i]
  // 不变量：i：当前正在处理的字符下标；j：已经匹配的模式串前缀长度=匹配模式串后的字符下标
  // [0..j-1](j)...[i-j..i-1](i)
    for (int i = 1, j = 0; i < m; ++i) {
      while (j && p[i] != p[j]) j = pi[j - 1];
      if (p[i] == p[j]) ++j;
      pi[i] = j;
    }
  }
  int find(const string& s) {
  // 不变量：p[0..j-1] = s[i-j..i-1]；想象固定 s，p 在下面滑
    for (int i = 0, j = 0; i < s.size(); ++i) {
      while (j && s[i] != p[j]) j = pi[j - 1];
      if (s[i] == p[j]) ++j;
      if (j == m) return i - m + 1;
    }
    return -1;
  }
  vector<int> find_all(const string& s) {
    vector<int> res;
  // 不变量：j 为已经匹配上的 pattern 的前缀长度，第 j 个下标位置为待比较位置；
  // 即当前已知 p[0..j-1] = s[i-j..i-1]，下一步尝试匹配 p[j] 与 s[i]。
  // fail 链：前缀匹配失败后，下一步尝试更短的前缀：j = pi[j - 1]；最终跳到 0
    for (int i = 0, j = 0; i < s.size(); ++i) {
      while (j && s[i] != p[j]) j = pi[j - 1];
      if (s[i] == p[j]) ++j;
  // 此时前缀 s[0..i] 的后缀最多能匹配模式串的 p[0..j-1]
      if (j == m) res.emplace_back(i - m + 1), j = pi[j - 1];
    }
    return res;
  }
  int count(const string& s) {
    int res = 0;
    for (int i = 0, j = 0; i < s.size(); ++i) {
      while (j && s[i] != p[j]) j = pi[j - 1];
      if (s[i] == p[j]) ++j;
      if (j == m) ++res, j = pi[j - 1];
    }
    return res;
  }
  vector<int> borders() {
    vector<int> res;
  // 沿着 fail 链跳，k 是 border 长度，此时 m - k 为对应周期长度
    for (int k = m; k; k = pi[k - 1]) res.emplace_back(k);
    reverse(res.begin(), res.end());
    return res;
  }
  int period() {
    int k = m - pi[m - 1];
    return m % k == 0 ? k : m;
  }
  void fail_tree(vector<int>& father, vector<vector<int>>& g) {
    father.resize(m + 1), g.resize(m + 1);
    for (int len = 1; len <= m; ++len) {
      father[len] = pi[len - 1];
      g[father[len]].emplace_back(len);
    }
  }
};
```

### 3. Z函数
```CPP
/**
 * Z-Function【求每个后缀与原串的 LCP】
 * 定义 z[i] = |LCP(s[i:], s)|
 *
 * 对于模式串在每个位置能匹配的最长长度：构造 pattern + '#' + text，求出 z-func。假设 pattern 大小为 m，那么结果数组的 z[m+1+i] 即表示 pattern 与 text[i..] 的 LCP 长度.
 * 注意 z[i] = x 代表长度为 [1,x] 的前缀都能被匹配到。x 是最大的可能长度。
 *
 * 复杂度：O(n)
 */
vector<int> z_function(const string& s) {
  int n = s.size();
  vector<int> z(n); z[0] = n;
  // 不变量 i：当前计算的位置；[l, r]：当前右端点最远的 Z-box，满足 s[l..r] = s[0..r-l]
  for (int i = 1, l = 0, r = -1; i < n; ++i) {
    if (i <= r) {
  // s: 0 ------ j ======= r-l ...... l ------ i ======= r
  //    [------ prefix ------]        [------ Z-box -----]
      int j = i - l;
      z[i] = min(z[j], r - i + 1);
    }
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
    if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
  }
  return z;
}
```

## 数学

### 1. 逆元递推
```CPP
int inv[MX];
void init() {
	inv[1] = 1;
	for (int i = 2; i < MX; ++i) {
		inv[i] = 1LL * (mod - mod / i) * inv[mod % i] % mod;
	}
}
```

### 2. lpf 递推
```CPP
int cnt = 0, prim[MX]; bool vis[MX];
int lpf[MX];
void init() {
  vis[1] = false;
  for (int i = 2; i < MX; ++i) {
    if (!vis[i]) prim[++cnt] = i, lpf[i] = i;
    for (int j = 1; i * prim[j] < MX; ++j) {
      vis[i * prim[j]] = true, lpf[i * prim[j]] = prim[j];
      if (i % prim[j] == 0) break;
    }
  }
}
```

### 3. 平方剩余核
```CPP
/**
 * 平方剩余核
 * 定义：一个数 x 的平方剩余核 core[x] 为他所有质因数指数 %=2 后乘得得数，即为去除最大平方因子后得结果
 * 性质：
 * 1. x * y 为完全平方数 <==> core[x] == core[y]【重要！】
 * 2. 记 core[x]=a, core[y]=b，则 core[x * y] = a*b / gcd(a,b)^2
 * 3. 使 x * y 成为完全平方数的最小正整数 y 为 core[x]，所有满足条件的 y 为 {core[x] * k^2}
 *
 * core(x) 作为集合的视角：令 S(x) 为保存了 x 的奇数幂次的素因子的集合，则 core(x) 即为 S(x) 中元素的乘积。
 * 区间 [l,r] 的乘积为完全平方数 <==> S(P[r])==S(P[l-1])，其中 P[i] 为前缀 a[0..i] 上的积。
 */
int core[MX];
void init() {
  memset(core, 0, sizeof core);
  for (int i = 1; i < MX; ++i) {
    if (core[i]) continue;
    for (int j = 1; 1LL * i * j * j < MX; ++j) core[i * j * j] = i;
  }
}
```

### 4. 线性筛
```CPP
int prim[N], cnt = 0; char vis[N];
// ---------------------------------
int phi[N];
int mu[N];
int a[N], d[N];
long long g[N], f[N];
// ---------------------------------
void init() {
	vis[1] = true;
  phi[1] = 1, a[1] = 0, d[1] = 1, g[1] = 0, f[1] = 1, mu[1] = 1;
	for (int i = 2; i < N; ++i) {
		if (!vis[i]) {
      prim[++cnt] = i;
      phi[i] = i - 1;
      a[i] = 1, d[i] = 2;
      g[i] = 1 + i, f[i] = 1 + i;
      mu[i] = -1;
    }
		for (int j = 1; i * prim[j] < N; ++j) {
			int p = prim[j];
			vis[i * p] = 1;
			if (i % p == 0) {
				phi[i * p] = phi[i] * p;
				a[i * p] = a[i] + 1, d[i * p] = d[i] / a[i * p] * (a[i * p] + 1);
				g[i * p] = g[i] * p + 1, f[i * p] = f[i] / g[i] * g[i * p];
				mu[i * p] = 0;
				break;
			}
			phi[i * p] = phi[i] * (p - 1);
			a[i * p] = 1, d[i * p] = d[i] * 2;
			g[i * p] = 1 + p, f[i * p] = f[i] * g[p];
			mu[i * p] = -mu[i];
		}
	}
}
```

## 数据结构

### 1. HH 项链
```CPP
struct BIT {
  int n;
  vector<int> c;  // c[i] = sum of nums[i-lowbit(i)...i-1]
  BIT(int _n) : n(_n) { c.resize(n + 1); }
  BIT(vector<int>& a) : n(a.size()) {
    c.resize(n + 1);
    for (int i = 1; i <= n; ++i) c[i] = a[i - 1];
    for (int i = 1; i <= n; ++i) {
      int j = i + (i & -i);
      if (j <= n) c[j] += c[i];
    }
  }
  void add(int id, int val) {
    ++id;
    for (int i = id; i <= n; i += i & -i) c[i] += val;
  }
  void set(int id, int val) {
    add(id, val - get(id));
  }
  int pref_sum(int id) {
    ++id;
    int sum = 0;
    for (int i = id; i >= 1; i &= i - 1) sum += c[i];
    return sum;
  }
  int range_sum(int l, int r) {
    if (l > r) return 0;
    return pref_sum(r) - (l ? pref_sum(l - 1) : 0);
  }
  int get(int id) {
    return range_sum(id, id);
  }
  int kth(int k) {
    int pos = 0;
    for (int i = __lg(n); ~i; --i) {
      int nxt = pos + (1 << i);
      if (nxt <= n && c[nxt] < k) pos = nxt, k -= c[nxt];
    }
  // return pos - 1;  // 最大下标使得前缀和 < k
    return pos;  // 最小下标使得前缀和 >= k
  }
};

/**
 * 给定一个颜色数组，以及 q 个无修改的查询 [l,r]，表示查询 [l,r] 上唯一颜色个数
 *
 * HH 项链 trick：将询问离线，右端点排序
 * 数组从左到右，额外维护 last[x] 表示 x 上一次出现的地方。HH trick 即为只把 last 的位置置 1，其他置 0
 * 保证询问时永远只有最靠右的元素贡献一次计数
 *
 * 注意：很多 HHTrick 处理的问题，也都可以用莫队（但注意复杂度更劣）
 * 前置条件：数组非空；每个询问满足 0<=l<=r<n，id 位于 [0,q-1] 且互不重复
 *
 * 扩展：求数组的所有子数组 cnt(不同元素个数) 之和
 * 定义 f(r) 为以 r 为右端点的所有子数组的唯一颜色数之和。从左到右遍历 r 求和即为答案，考虑如何求 f(r).
 * 首先由于 HHTrick，我们可以维护一个 last 数组来记录当前元素的上一个最近出现位置（没出现过即为 -1）；
 * 那么考虑从 r-1 变为 r 时，如何从 f(r-1) 转移到 f(r)：对于左端点 l∈[last[a[r]]+1,r] 的位置，子数组 [l,r] 的唯一颜色数增加一个；其余不变。
 * 因此可以维护懒标记线段树，转移时将 [last+1,r] 整体 +1，查询 [0,r] 所有数之和即为 f(r) 值。
 */
struct Query {
  int l, r;
  int id;
};
vector<int> HHTrick(vector<int>& a, vector<Query>& qs) {
  int n = a.size(), m = qs.size();
  vector<int> res(m);
  sort(qs.begin(), qs.end(), [&](auto& q1, auto& q2) {
    return q1.r < q2.r;
  });
  BIT tr(n);
  vector<int> xs = a;
  sort(xs.begin(), xs.end());
  xs.erase(unique(xs.begin(), xs.end()), xs.end());
  vector<int> last(xs.size(), -1);
  int j = 0;
  for (int i = 0; i < n; ++i) {
    int x = lower_bound(xs.begin(), xs.end(), a[i]) - xs.begin();
    if (last[x] != -1) tr.add(last[x], -1);
    last[x] = i;
    tr.add(i, 1);
    while (j < m && qs[j].r == i) {
      auto& [l, r, id] = qs[j];
      res[id] = tr.range_sum(l, r);
      ++j;
    }
  }
  return res;
}
```

### 2. 二维数点
```CPP
/**
 * 给定 n 个二维点 points，以及 q 个询问 [a,b,c,d] qs，求在 [a,c] x [b,d] 范围内（含边界）的点有多少个
 *
 * 本质上是二维偏序计数：计算有多少对 (x,y) 满足 a<=x<=c && b<=y<=d
 * 类似差分扫描线思想的转化：count(x1<=x<=x2, y1<=y<=y2) = count(x<=x2, y1<=y<=y2) - count(x<x1, y1<=y<=y2)
 * 将 x 维度建立为按顺序遍历的“时间维度”，y 维度需要查询&&修改当前时刻满足在值域范围内的点数，使用树状数组维护即可
 *
 * 复杂度：O((n + q) log ∑y)
 * 前置条件：每个询问矩形满足 x1<=x2、y1<=y2
 */
struct Query {
  int x1, y1, x2, y2;
};
vector<long long> count2D(vector<pair<int, int>> points, const vector<Query>& qs) {
  struct Event {
    long long x;
    int y1, y2;
    int type, id;
  };
  int n = points.size(), q = qs.size();
  vector<int> ys;
  ys.reserve(n);
  for (auto& [x, y] : points) ys.emplace_back(y);
  sort(ys.begin(), ys.end());
  ys.erase(unique(ys.begin(), ys.end()), ys.end());
  vector<Event> evs;
  evs.reserve(q << 1);
  for (int i = 0; i < q; ++i) {
    auto& [x1, y1, x2, y2] = qs[i];
    evs.emplace_back(x2, y1, y2, 1, i);
    evs.emplace_back(1LL * x1 - 1, y1, y2, -1, i);
  }
  sort(points.begin(), points.end());
  sort(evs.begin(), evs.end(), [&](auto& a, auto& b) {
    return a.x < b.x;
  });
  BIT<int> tr(ys.size());
  vector<long long> res(q);
  int j = 0;
  for (auto& [x, y1, y2, type, id] : evs) {
    while (j < n && points[j].first <= x) {
      int y = lower_bound(ys.begin(), ys.end(), points[j].second) - ys.begin();
      tr.add(y, 1);
      ++j;
    }
    int l = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
    int r = upper_bound(ys.begin(), ys.end(), y2) - ys.begin() - 1;
    if (l <= r) res[id] += type * tr.range_sum(l, r);
  }
  return res;
}
```

### 3. 扫描线
```CPP
/**
 * 线段树扫描线：矩形面积并
 *
 * rect = {x1, y1, x2, y2}  代表矩形的左下角，右上角坐标
 * 注意由于是连续段长，线段树下标 i 表示离散化后的 y 小段 [ys[i], ys[i + 1])
 * 前置条件：每个矩形满足 x1<=x2、y1<=y2
 *
 * 复杂度：O(n log n)，其中 n 为 rects.size()
 */
struct Event {
  // 横坐标：x；纵坐标范围：[y1,y2]；事件：v=1 代表边进入，v=-1 代表边出去
  int x, y1, y2, v;
  bool operator<(const Event& e) const {
    if (x != e.x) return x < e.x;
    return v > e.v;
  }
};
struct ScanSegTree {
  // 线段树节点 [l, r] 表示 y 轴值域区间 [ys[l], ys[r+1])
  // 值域区间 [y1, y2) 对应线段树节点 [l, r]，其中 l=lowerbound(ys,y1), r=lowerbound(ys,y2)-1
  int n;
  vector<int> cnt;  // 当前节点被多少个区间修改操作完整覆盖；类似于懒标记（但不用 pushdown）
  vector<long long> len;  // 当前节点区间内，覆盖区间的并集长度
  vector<int>& ys;
  ScanSegTree(int _n, vector<int>& _ys) : n(_n), ys(_ys) {
    cnt.assign(n * 4 + 5, 0);
    len.assign(n * 4 + 5, 0);
  }
  void pushup(int u, int l, int r) {
    if (cnt[u]) len[u] = 1LL * ys[r + 1] - ys[l];  // 被某个区间完整覆盖了，因此并集长度就是区间长度；否则递归下去计算
    else if (l == r) len[u] = 0;
    else len[u] = len[u << 1] + len[u << 1 | 1];
  }
  void add(int u, int l, int r, int ql, int qr, int v) {
    if (ql <= l && r <= qr) {
      cnt[u] += v;
      pushup(u, l, r);
      return;
    }
    int mid = (l + r) >> 1;
    if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
    if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
    pushup(u, l, r);
  }
  void add(int l, int r, int v) {
    if (l > r) return;
    add(1, 0, n - 1, l, r, v);
  }
  long long query() {
    return len[1];
  }
};
long long scanLine(vector<array<int, 4>>& rects) {
  vector<Event> evs; evs.reserve(rects.size() * 2);
  vector<int> ys; ys.reserve(rects.size() * 2);
  for (auto& a : rects) {
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    evs.push_back({x1, y1, y2, 1});
    evs.push_back({x2, y1, y2, -1});
    ys.emplace_back(y1), ys.emplace_back(y2);
  }
  if (evs.empty()) return 0;
  long long res = 0;
  // 对于值域进行离散化，ys[i] 表示第 i 个离散化坐标
  // 规定值域区间均为左闭右开
  // 线段树叶子 i 表示 [ys[i], ys[i+1]) 这一段区间；因此线段树节点 [l,r] 对应真实坐标区间 [ys[l], ys[r+1]).
  sort(ys.begin(), ys.end()); ys.erase(unique(ys.begin(), ys.end()), ys.end());
  int m = ys.size() - 1;
  ScanSegTree tr(m, ys);
  sort(evs.begin(), evs.end());
  int lastx = evs[0].x;
  for (int i = 0; i < evs.size();) {
    int x = evs[i].x;
  // 在整个 [lastx, x) 区间内，活跃矩形集合没有发生改变
    res += tr.query() * (1LL * x - lastx);
    int j = i;
    while (j < evs.size() && evs[j].x == x) {
      int l = lower_bound(ys.begin(), ys.end(), evs[j].y1) - ys.begin();
      int r = lower_bound(ys.begin(), ys.end(), evs[j].y2) - ys.begin() - 1;
  // y 轴值域区间 [y1, y2) 对应的线段树节点为 [l, r]
      if (l <= r) tr.add(l, r, evs[j].v);
      ++j;
    }
    lastx = x, i = j;
  }
  return res;
}
```

### 4. 小波矩阵
```CPP
/**
 * 小波矩阵 / Wavelet Matrix【静态区间顺序统计、值域计数】
 * 维护静态数组 a，支持区间值域查询。
 * 数组下标为 0-based，查询区间为闭区间 [l, r]，第 k 小的 k 从 1 开始。
 *
 * 功能：
 * 构造：WaveletMatrix wm(std::move(a))
 * kth(l, r, k)：返回 [l, r] 中第 k 小，1-based
 * lt_cnt(l, r, x)：返回 [l, r] 中严格 <x 的元素个数
 * le_cnt(l, r, x)：返回 [l, r] 中 <=x 的元素个数
 * cnt_range(l, r, x, y)：返回 [l, r] 中值属于 [x, y] 的元素个数
 * rank(l, r, x)：返回 x 在 [l, r] 中的排名，1-based
 * pre_val(l, r, x)：返回 [l, r] 中最大 <x 的值
 * lower_bound(l, r, x)：返回 [l, r] 中最小 >=x 的值
 * upper_bound(l, r, x)：返回 [l, r] 中最小 >x 的值
 * nxt_val(l, r, x)：等价于 upper_bound
 *
 * 适用：静态数组上的区间顺序统计、值域计数、前驱后继查询。
 *
 * 复杂度：
 * 建树 O(n log n + n log m)，空间 O(n log m)
 * 单次查询 O(log m)，m 为不同值个数。
 *
 * 前置条件：构造数组非空；查询区间合法且非空；kth 的 k 位于 [1, r-l+1]。
 */
#pragma GCC target("popcnt")
template<class T>
struct WaveletMatrix {
  static constexpr T INF = numeric_limits<T>::max(), NINF = numeric_limits<T>::lowest();
  struct BitVector {
    vector<unsigned long long> bits;
    vector<int> pre;
    BitVector() {}
    BitVector(int n) {
      int m = (n + 63) >> 6;
      bits.assign(m + 1, 0);
      pre.assign(m + 1, 0);
    }
    void set(int i) { bits[i >> 6] |= 1ULL << (i & 63); }
    void build() {
      int m = pre.size() - 1;
      for (int i = 0; i < m; ++i) pre[i + 1] = pre[i] + __builtin_popcountll(bits[i]);
    }
    int rank0(int i) const {
      int b = i >> 6, k = i & 63;
      return i - pre[b] - __builtin_popcountll(bits[b] & ((1ULL << k) - 1));
    }
  };
  int n = 0, m = 0, LOG = 1;
  vector<T> vals;
  vector<int> mid;
  vector<BitVector> pre;
  WaveletMatrix() = default;
  WaveletMatrix(vector<T> a) { build(std::move(a)); }
  void build(vector<T> a) {
    n = a.size(); vals = a;
    sort(vals.begin(), vals.end()); vals.erase(unique(vals.begin(), vals.end()), vals.end());
    m = vals.size(); LOG = 0;
    while ((1LL << LOG) < m) ++LOG;
    pre.clear(); mid.clear();
    if (!LOG) return;
    vector<int> cur(n);
    for (int i = 0; i < n; ++i) cur[i] = std::lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin();
    vector<T>().swap(a);
    pre.resize(LOG); mid.resize(LOG);
    vector<int> nxt(n);
    for (int d = 0; d < LOG; ++d) {
      pre[d] = BitVector(n);
      int bit = LOG - 1 - d, cnt0 = 0;
      for (int i = 0; i < n; ++i) cnt0 += !((cur[i] >> bit) & 1);
      mid[d] = cnt0;
      int p0 = 0, p1 = cnt0;
      for (int i = 0; i < n; ++i) {
        if ((cur[i] >> bit) & 1) pre[d].set(i), nxt[p1++] = cur[i];
        else nxt[p0++] = cur[i];
      }
      pre[d].build();
      cur.swap(nxt);
    }
  }
  T kth(int l, int r, int k) const {
    int x = 0;
    for (int d = 0; d < LOG; ++d) {
      int zl = pre[d].rank0(l), zr = pre[d].rank0(r + 1), cnt0 = zr - zl;
      if (k <= cnt0) l = zl, r = zr - 1;
      else k -= cnt0, x |= 1 << (LOG - 1 - d), l = mid[d] + l - zl, r = mid[d] + r - zr;
    }
    return vals[x];
  }
  int count_id_less(int l, int r, int x) const {
    if (x <= 0) return 0;
    if (x >= m) return r - l + 1;
    int res = 0;
    for (int d = 0; d < LOG; ++d) {
      int zl = pre[d].rank0(l), zr = pre[d].rank0(r + 1);
      if ((x >> (LOG - 1 - d)) & 1) res += zr - zl, l = mid[d] + l - zl, r = mid[d] + r - zr;
      else l = zl, r = zr - 1;
    }
    return res;
  }
  int lt_cnt(int l, int r, T x) const { return count_id_less(l, r, std::lower_bound(vals.begin(), vals.end(), x) - vals.begin()); }
  int le_cnt(int l, int r, T x) const { return count_id_less(l, r, std::upper_bound(vals.begin(), vals.end(), x) - vals.begin()); }
  int cnt_range(int l, int r, T x, T y) const {
    if (y < x) return 0;
    return le_cnt(l, r, y) - lt_cnt(l, r, x);
  }
  int rank(int l, int r, T x) const { return lt_cnt(l, r, x) + 1; }
  T pre_val(int l, int r, T x) const {
    int k = lt_cnt(l, r, x);
    if (k == 0) return NINF;
    return kth(l, r, k);
  }
  T lower_bound(int l, int r, T x) const {
    int k = lt_cnt(l, r, x) + 1;
    if (k > r - l + 1) return INF;
    return kth(l, r, k);
  }
  T upper_bound(int l, int r, T x) const {
    int k = le_cnt(l, r, x) + 1;
    if (k > r - l + 1) return INF;
    return kth(l, r, k);
  }
  T nxt_val(int l, int r, T x) const { return upper_bound(l, r, x); }
};
```

### 5. 带修莫队
```CPP
/**
 * 带修莫队
 *
 * 状态定义：
 * Event(op, l, r, id)：op = 0 表示修改 a[l] = r；op = 1 表示查询 [l, r]，答案放到 res[id]
 * a[i]：当前时间下位置 i 的值
 * T：当前已经执行了前 T 次修改
 *
 * 转移：
 * add / del 维护元素进入 / 离开区间 [L, R]
 * apply / undo 维护时间 T，暴力加入 / 撤销修改
 *
 * 复杂度：
 * B 取 n^(2/3)，整体约 O((n + m) n^(2/3)) ~= O(n^(5/3))
 * 当修改查询不平衡时，可以有更精细的公式 B ≈ max(n/sqrt(q), (u n²/q)^(1/3))
 *
 * 注意：
 * 1. evs 按题目原顺序传入
 * 2. 查询 id 必须为 0, 1,..., querySize - 1（！！！）
 * 3. 默认维护区间不同数个数，改题时只改 cnt / cur / add / del 的逻辑即可
 * 4. 如果查询等和特定值有关，修改 Event 多一维 x 即可，但是传进来务必一起加入离散化！！
 * 5. 前置条件：原数组非空；修改位置合法；查询满足 0<=l<=r<n，且查询 id 合法并互不重复
 */
struct Event {
  int op, l, r, id;  // modify: {0, p, v, 0}; query: {1, l, r, id}
};
vector<int> moAlgorithmWithModify(vector<int>& orig_a, vector<Event>& evs) {
  int n = orig_a.size();
  // 离散化
  vector<int> xs = orig_a;
  for (auto& e : evs) {
    if (e.op == 0) xs.emplace_back(e.r);
  }
  sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
  int tot = xs.size();
  auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
  vector<int> b = orig_a;
  for (int i = 0; i < n; ++i) b[i] = find(b[i]);
  vector<Event> ms, qs;  // ms: {0, p, val, old}，qs: {t, l, r, id}
  vector<int> a = b;
  int t = 0, q = 0;
  for (auto& e : evs) {
    if (e.op == 0) {
      int val = find(e.r);
      ms.push_back({0, e.l, val, a[e.l]});
      a[e.l] = val;
      ++t;
    } else {
      qs.push_back({t, e.l, e.r, e.id});
      ++q;
    }
  }
  a = b;
  // 对查询分块；蛇形排序
  int B = pow(n, 2.0 / 3) + 1;
  // int B1 = max(1, int(n / sqrt(max(1, q)))), B2 = max(1, int(cbrt(1.0 * max(1, t) * n * n / max(1, q)))), B = max(B1, B2);
  ranges::sort(qs, [&](auto& x, auto& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    int rx = x.r / B, ry = y.r / B;
    if (rx != ry) {
      if (bx & 1) return rx > ry;
      return rx < ry;
    }
    if (rx & 1) return x.op > y.op;
    return x.op < y.op;
  });

  /* ------------------ 核心部分：单点维护 ---------------- */
  vector<int> cnt(tot);  // 频率统计桶
  vector<int> res(q);  // 询问对应的答案
  int cur = 0;  // 维护当前答案
  int L = 0, R = -1;  // 当前区间左右端点
  auto add = [&](int i) -> void {  // 下标为 i 的元素进入查询区间
    int x = a[i];
    if (cnt[x] == 0) ++cur;
    ++cnt[x];
  };
  auto del = [&](int i) -> void {  // 下标为 i 的元素离开查询区间
    int x = a[i];
    --cnt[x];
    if (cnt[x] == 0) --cur;
  };

  auto apply = [&](int t, int L, int R) -> void {  // 执行时间 t 对应的修改
    auto& e = ms[t];
    int p = e.l, v = e.r;
    if (L <= p && p <= R) del(p);
    a[p] = v;
    if (L <= p && p <= R) add(p);
  };
  auto undo = [&](int t, int L, int R) -> void {  // 撤销时间 t 对应的修改
    auto& e = ms[t];
    int p = e.l, old = e.id;
    if (L <= p && p <= R) del(p);
    a[p] = old;
    if (L <= p && p <= R) add(p);
  };
  // 处理查询
  int T = 0;
  for (auto& e : qs) {
    int l = e.l, r = e.r, t = e.op, id = e.id;
    while (L > l) add(--L);
    while (R < r) add(++R);
    while (L < l) del(L++);
    while (R > r) del(R--);
    while (T < t) apply(T++, L, R);
    while (T > t) undo(--T, L, R);
    res[id] = cur;
  }
  return res;
}
```

## 树论

### 1. 启发式合并
```CPP
/**
 * 树上启发式合并不仅能解决子树统计信息，还可以在**合并的同时进行跨子树的查询**
 * 前提是路径 (x,y) 的信息可以通过 info(x),info(y),info(lca(x,y)) 快速得到；
 * 关键在于，任何一条查询路径 (x,y) 可以唯一写成 x-u-y 的路径，u=lca(x,y) 是当前合并的根节点
 *
 * 注意：
 * 1. 如果路径 x -- rt 的信息不能通过对于 x -- 0 和 rt -- 0 的信息差分得到，可以考虑先进行倍增预处理
 * 2. 路径统计同时可以考虑树形 DP，由于每条路径只会在 LCA 处被统计一次，因此若树形 DP 可行应当优先考虑
 *
 * 维护全局统计桶，每次在合并之前先查询跨子树贡献；注意统计桶一定记录的是到根节点的信息！！查询的时候对当前根做差
 * 模板例题：计算 sum_{u-v} w(u-v) * cnt(u-v)，其中 u-v 是边 uv，cnt[e] 表示边 e 在所有长为 k 的简单路径中的出现次数
 * 关键等式: dep[x] + dep[u] - 2*dep[lca] = k ==> targdep = k + 2dep[lca] - dep[x]
 *
 * 复杂度 O(n * logn)
 */
long long DSUonTree(vector<vector<pair<int, int>>>& g, int k, int root = 0) {
  int n = g.size();
  vector<int> sz(n), son(n, -1), dfn(n), out(n);
  vector<int> seq; seq.reserve(n);
  vector<int> dep(n);
  vector<long long> dis(n);
  int timer = 0;
  auto dfs1 = [&](this auto&& dfs1, int u, int fa) -> void {
    sz[u] = 1;
    dfn[u] = timer++;
    seq.emplace_back(u);
    for (auto& [v, w] : g[u]) {
      if (v == fa) continue;
      dep[v] = dep[u] + 1;
      dis[v] = dis[u] + w;
      dfs1(v, u);
      sz[u] += sz[v];
      if (son[u] == -1 || sz[v] > sz[son[u]]) son[u] = v;
    }
    out[u] = timer;
  }; dfs1(root, -1);
  /** ------------------------------------------------------------------------- */
  long long res = 0;
  vector<int> cnt(n);  // cnt[d] = 当前全局桶中深度为 d 的节点数量
  vector<long long> sum(n);  // sum[d] = 当前全局桶中深度为 d 的所有节点的 dis 之和
  auto add = [&](int x) -> void {  // 将节点 x 的信息加入桶
  	++cnt[dep[x]];
  	sum[dep[x]] += dis[x];
  };
  auto del = [&](int x) -> void {  // 将节点 x 的信息移出桶
  	--cnt[dep[x]];
  	sum[dep[x]] -= dis[x];
  };
  auto query = [&](int x, int rt) -> void {  // 全局桶更新 rt 子树中节点 x 的答案/贡献
  // 假设 targ 是桶中可以与 x 配对的合法点：x -- rt -- targ
  // dep[x] + dep[targ] - 2dep[rt] = k => dep[targ] = k + 2dep[rt] - dep[x]
  	int targd = k + 2 * dep[rt] - dep[x];
  	if (targd < 0 || targd >= n) return;
  	res += (dis[x] - 2 * dis[rt]) * cnt[targd] + sum[targd];
  };
  /** ------------------------------------------------------------------------- */
  auto dfs2 = [&](this auto&& dfs2, int u, int fa, bool keep) -> void {
    for (auto& [v, w] : g[u]) {
      if (v == fa || v == son[u]) continue;
      dfs2(v, u, false);  // calculate answer for light sons first, but doesnt store its contribution in bucket
    }
    if (son[u] != -1) dfs2(son[u], u, true);  // calculate answer for heavy son then, and keep info in bucket
    query(u, u);  // query u with heavy subtree
    add(u);
    for (auto& [v, w] : g[u]) {
      if (v == fa || v == son[u]) continue;
      for (int i = dfn[v]; i < out[v]; ++i) {
        int x = seq[i];
        query(x, u);  // brutely query for light subtree
      }
      for (int i = dfn[v]; i < out[v]; ++i) {
        int x = seq[i];
        add(x);  // brutely add for light subtree
      }
    }
    if (!keep) {
      for (int i = dfn[u]; i < out[u]; ++i) {
        int x = seq[i];
        del(x);
      }
    }
  }; dfs2(root, -1, true);
  return res;
}
```

### 2. 树上莫队
```CPP
/**
 * 树上莫队（不带修，点权）
 * 将树上路径转成欧拉序上的区间，再用普通莫队移动左右指针
 *
 * 路径转区间（先保证 in[u] <= in[v]，记 p = lca(u, v)）：
 * 1. p == u：路径对应 [in[u], in[v]]，不需要额外补点
 * 2. p != u：路径对应 [out[u], in[v]]，但少了 p，回答时临时加入 p，再撤销
 *
 * 复杂度：
 * 设点数 n、询问数 q、块长 B、增删单次均摊 T、读取答案 A，欧拉序长度为 2n
 * 总时间 O((n + q) log n + q log q + (qB + n²/B)T + qA)，空间 O(n log n + q)
 * 若 q 与 n 同阶，增删和读取答案均 O(1)，总时间 O(n * sqrt n)
 */
struct Query {
  int l, r, id, extra;  // 欧拉序区间，询问编号，补点
};
// 预处理：建立进出序和倍增表 O(n log n)，每条路径转成区间 O(log n)
struct preprocess {
  int n, K;
  vector<int> depth, in, out, seq;
  vector<vector<int>> up;
  preprocess(vector<vector<int>>& g, int root = 0) {
    n = g.size();
    K = 1;
    while ((1LL << K) <= n) ++K;
    up.assign(K, vector<int>(n, root));
    depth.resize(n), in.resize(n), out.resize(n);
    seq.reserve(2 * n);
    vector<int> st; st.reserve(n);
    st.emplace_back(root);
    while (!st.empty()) {
      int u = st.back(); st.pop_back();
      if (u >= 0) {
        in[u] = seq.size();
        seq.emplace_back(u);
        for (int k = 1; k < K; ++k) up[k][u] = up[k - 1][up[k - 1][u]];
        st.emplace_back(~u);
        for (int i = int(g[u].size()) - 1; ~i; --i) {
          int v = g[u][i];
          if (v == up[0][u]) continue;
          up[0][v] = u;
          depth[v] = depth[u] + 1;
          st.emplace_back(v);
        }
      } else {
        u = ~u;
        out[u] = seq.size();
        seq.emplace_back(u);
      }
    }
  }
  int get_lca(int u, int v) const {
    if (depth[u] < depth[v]) swap(u, v);
    int d = depth[u] - depth[v];
    for (int k = 0; k < K; ++k) {
      if (d >> k & 1) u = up[k][u];
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; --k) {
      if (up[k][u] != up[k][v]) u = up[k][u], v = up[k][v];
    }
    return up[0][u];
  }
  Query query(int u, int v, int id) const {
    if (in[u] > in[v]) swap(u, v);
    int p = get_lca(u, v);
    if (p == u) return {in[u], in[v], id, -1};
    return {out[u], in[v], id, p};
  }
};

vector<int> moAlgorithm(vector<int>& a, vector<Query>& qs, const vector<int>& seq) {
  int n = a.size();
  int q = qs.size();
  // 离散化
  vector<int> xs = a;
  sort(xs.begin(), xs.end()); xs.erase(unique(xs.begin(), xs.end()), xs.end());
  int tot = xs.size();
  auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
  for (int i = 0; i < n; ++i) a[i] = find(a[i]);
  // 对查询分块；蛇形排序
  int m = seq.size();
  int B = sqrt(m) + 1;
  // int B = max(1, int(m / sqrt(max(1, q))));
  ranges::sort(qs, [&](const Query& x, const Query& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    if (bx & 1) return x.r > y.r;
    return x.r < y.r;
  });
  /* ----------------------- 核心部分：单点维护 --------------------- */
  vector<int> cnt(tot);  // 频率统计桶
  vector<int> ans(q);  // 询问对应的答案
  vector<int> vis(n);  // vis[u]：节点 u 在当前欧拉序区间中是否出现奇数次（偶数次被抵消）
  int cur = 0;  // 维护当前答案
  int L = 0, R = -1;  // 当前区间左右端点
  auto add = [&](int u) -> void {  // 节点 u 进入统计集合
    int x = a[u];
    if (cnt[x] == 0) ++cur;
    ++cnt[x];
  };
  auto del = [&](int u) -> void {  // 节点 u 离开统计集合
    int x = a[u];
    --cnt[x];
    if (cnt[x] == 0) --cur;
  };
  /* ------------------------------------------------------------- */
  auto toggle = [&](int i) -> void {  // 翻转状态
    int u = seq[i];
    if (vis[u]) del(u);
    else add(u);
    vis[u] ^= 1;
  };
  for (auto& [l, r, id, extra] : qs) {
    while (L > l) toggle(--L);
    while (R < r) toggle(++R);
    while (L < l) toggle(L++);
    while (R > r) toggle(R--);
    if (extra != -1) add(extra);
    ans[id] = cur;
    if (extra != -1) del(extra);
  }
  return ans;
}
```

## 几何 

### 1. 叉积
```CPP
/**
 * 两向量 a, b 叉积：
 * >0: 从 a 转向 b 是逆时针
 * <0: 从 a 转向 b 是顺时针
 * =0: 两向量共线（平行或有零向量）
 * 几何意义：a, b 为邻边构成的平行四边形的有向面积
 */
long long cross(Point a,Point b) { return a.x * b.y - a.y * b.x; }
/**
 * 三点 a, b, c 叉积，等价于向量 b - a 与向量 c - a 的叉积
 *
 * >0: c 在有向直线 ab 左侧
 * <0: c 在有向直线 ab 右侧
 * =0: a, b, c 三点共线
 * 几何意义：三角形 abc 的两倍有向面积
 */
long long cross(Point a,Point b,Point c) { return cross(b - a, c - a); }
```

### 2. 凸包
```CPP
struct Point {
  long long x, y;
  Point() : x(0), y(0) {}
  Point(long long _x, long long _y) : x(_x), y(_y) {}
  Point operator-(const Point &o) const { return Point(x - o.x, y - o.y); }
  bool operator<(const Point &o) const {
    if (x != o.x) return x < o.x;
    return y < o.y;
  }
  bool operator==(const Point &o) const { return x == o.x && y == o.y; }
};
long long cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
long long cross(const Point &a, const Point &b, const Point &c) { return cross(b - a, c - a); }

/**
 * 返回点集 p 的凸包 ch，保证凸包中的点按逆时针排序
 *
 * 核心算法：Andrew 算法；按照 x 坐标排序，使用单调栈维护凸包的下边界和上边界。关键是 cross>0 代表左转，cross<0 代表右转
 * 下文中 a=stk[top-1], b=stk[top], c=pts[i].
 * 下凸包：从左到右扫描，维护一条折线，使得所有已处理的点都在折线的上方或边界上；折线始终保持向左转，即相邻两条边的 cross > 0。
 *         对于栈中连续两点 a, b 与新点 c，若：
 *           cross > 0：左转，符合要求，保留 b
 *           cross <= 0：右转或共线，b 在 ac 上方或之间，弹出 b
 * 上凸包：从右到左扫描，维护一条折线，使得所有已处理的点都在折线的下方或边界上；折线始终保持向左转，即相邻两条边的 cross > 0。
 *         对于栈中连续两点 a, b 与新点 c，若：
 *           cross > 0：左转，符合要求，保留 b
 *           cross <= 0：右转或共线，b 在 ac 下方或之间，弹出 b
 *
 * 复杂度：O(n log n)，瓶颈在于排序
 */
vector<Point> convexHull(vector<Point>& p) {
  sort(p.begin(), p.end()); p.erase(unique(p.begin(), p.end()), p.end());
  int n = p.size();
  if (n <= 1) return p;
  vector<Point> s; s.resize(n * 2);
  // 求下凸包
  int top = -1;
  for (int i = 0; i < n; ++i) {
    while (top > 0 && cross(s[top - 1], s[top], p[i]) <= 0) --top;
    s[++top] = p[i];
  }
  // 求上凸包
  int t = top;
  for (int i = n - 2; ~i; --i) {
    while (top > t && cross(s[top - 1], s[top], p[i]) <= 0) --top;
    s[++top] = p[i];
  }
  --top;
  s.resize(top + 1); return s;
}
```

### 3. 旋转卡壳
```CPP
/**
 * 旋转卡壳
 *
 * 本质上是双指针；此处为遍历框架，需要根据题目在循环中更新 res
 * 不变量：
 *      i 枚举有向边 ch[i] -> ch[(i+1)%n]；
 *      每轮 while 结束后，j 是使该有向边与顶点构成的叉积最大的点，即离这条支撑线最远的顶点。
 * 复杂度：O(n)
 */
long long rotateCalliper(vector<Point>& ch) {
  int n = ch.size();
  if (n <= 2) {  // 注意处理退化的情况
    return 0;
  }
  long long res = 0;
  int j = 1;  // 旋转卡壳中注意 j 在第二个点为起点
  for (int i = 0; i < n; ++i) {
    while (/** 这里写 j 指针单调性移动的条件；注意 dis 不是单峰的！要用 cross */
      cross(ch[i], ch[(i + 1) % n], ch[j]) < cross(ch[i], ch[(i + 1) % n], ch[(j + 1) % n])) {
      j = (j + 1) % n;
    }
  }
  return res;
}
```

### 4. 半平面交
```CPP
struct FPoint { long double x, y; };
int half(const Point& p) {
  if (p.y > 0) return 0;
  if (p.y < 0) return 1;
  return p.x >= 0 ? 0 : 1;
}
// 构造直线 l(a,b)：沿着 a->b 走，要保留的区域必须在左手边
struct Line {
  Point a, b, dir;
  Line() : a(Point()), b(Point()) {}
  Line(const Point& _a, const Point& _b) : a(_a), b(_b), dir(b - a) {}
  bool operator<(const Line& l) const {
    int ha = half(dir), hb = half(l.dir);
    if (ha != hb) return ha < hb;
    long long c = cross(dir, l.dir);
    if (c != 0) return c > 0;
    return cross(dir, l.a - a) < 0;
  }
};
FPoint intersect_point(const Line& m, const Line& n) {
  long double t = (long double)cross(n.a - m.a, n.dir) / cross(m.dir, n.dir);
  return {(long double)m.a.x + m.dir.x * t, (long double)m.a.y + m.dir.y * t};
}
bool on_right(const FPoint& p, const Line& l) { return l.dir.x * (p.y - l.a.y) - l.dir.y * (p.x - l.a.x) <= 0; }

/**
 * 半平面交
 * 返回半平面交的边界直线，按极角逆时针排序
 *
 * 交点为 intersect_point(res[i], res[(i+1)%res.size()])，使用 FPoint
 *
 * 复杂度：O(n log n)
 */
vector<Line> half_plane_intersect(vector<Line>& ls) {
  sort(ls.begin(), ls.end());
  int n = ls.size(), h = 0, t = -1;
  vector<Line> dq; dq.resize(n);
  for (int i = 0; i < n; ++i) {
    if (i > 0 && half(ls[i].dir) == half(ls[i - 1].dir) && cross(ls[i].dir, ls[i - 1].dir) == 0) continue;
    while (h < t && on_right(intersect_point(dq[t - 1], dq[t]), ls[i])) --t;
    while (h < t && on_right(intersect_point(dq[h], dq[h + 1]), ls[i])) ++h;
    if (h <= t && cross(dq[t].dir, ls[i].dir) == 0) return {};
    dq[++t] = ls[i];
  }
  while (h < t && on_right(intersect_point(dq[t - 1], dq[t]), dq[h])) --t;
  while (h < t && on_right(intersect_point(dq[h], dq[h + 1]), dq[t])) ++h;
  if (t - h + 1 < 3) return {};
  for (int i = h; i <= t; ++i) {
    if (cross(dq[i].dir, dq[i == t ? h : i + 1].dir) <= 0) return {};
  }
  return vector<Line>(dq.begin() + h, dq.begin() + t + 1);
}
```

### 5. 最大内接圆
```CPP
struct FPoint { long double x, y; };
int half(const Point& p) {
  if (p.y > 0) return 0;
  if (p.y < 0) return 1;
  return p.x >= 0 ? 0 : 1;
}
// 沿着 a->b 走，保留左侧半平面，再将直线向左平移 r
struct Line {
  FPoint a;
  Point dir;
  Line() : a{0, 0}, dir(Point()) {}
  Line(const Point& _a, const Point& _b, long double r) : a{(long double)_a.x, (long double)_a.y}, dir(_b - _a) {
    long double len = hypotl(dir.x, dir.y);
    a.x -= dir.y * r / len; a.y += dir.x * r / len;
  }
  bool operator<(const Line& l) const {
    int ha = half(dir), hb = half(l.dir);
    if (ha != hb) return ha < hb;
    long long c = cross(dir, l.dir);
    if (c != 0) return c > 0;
    return dir.x * (l.a.y - a.y) - dir.y * (l.a.x - a.x) < 0;
  }
};
FPoint intersect_point(const Line& m, const Line& n) {
  long double t = ((n.a.x - m.a.x) * n.dir.y - (n.a.y - m.a.y) * n.dir.x) / cross(m.dir, n.dir);
  return {m.a.x + m.dir.x * t, m.a.y + m.dir.y * t};
}
bool on_right(const FPoint& p, const Line& l) { return l.dir.x * (p.y - l.a.y) - l.dir.y * (p.x - l.a.x) <= 0; }

/** 返回半平面交的边界直线，按极角逆时针排序；无正面积交集时返回空 */
vector<Line> half_plane_intersect(vector<Line>& ls) {
  sort(ls.begin(), ls.end());
  int n = ls.size(), h = 0, t = -1;
  vector<Line> dq; dq.resize(n);
  for (int i = 0; i < n; ++i) {
    if (i > 0 && half(ls[i].dir) == half(ls[i - 1].dir) && cross(ls[i].dir, ls[i - 1].dir) == 0) continue;
    while (h < t && on_right(intersect_point(dq[t - 1], dq[t]), ls[i])) --t;
    while (h < t && on_right(intersect_point(dq[h], dq[h + 1]), ls[i])) ++h;
    if (h <= t && cross(dq[t].dir, ls[i].dir) == 0) return {};
    dq[++t] = ls[i];
  }
  while (h < t && on_right(intersect_point(dq[t - 1], dq[t]), dq[h])) --t;
  while (h < t && on_right(intersect_point(dq[h], dq[h + 1]), dq[t])) ++h;
  if (t - h + 1 < 3) return {};
  for (int i = h; i <= t; ++i) {
    if (cross(dq[i].dir, dq[i == t ? h : i + 1].dir) <= 0) return {};
  }
  return vector<Line>(dq.begin() + h, dq.begin() + t + 1);
}

/**
 * 返回点集 pts 的凸包的最大内接圆 {圆心, 半径}
 *
 * 二分半径，将凸包各边向内平移后求半平面交为固定半径时圆心的可行域
 *
 * 复杂度：O(n log n)
 */
pair<FPoint, long double> maximumInscribedCircle(vector<Point>& pts) {
  vector<Point> ch = convexHull(pts);
  int n = ch.size();
  if (n == 0) return {{0, 0}, 0};
  FPoint o{(long double)ch[0].x, (long double)ch[0].y};
  if (n < 3) return {o, 0};
  Point origin = ch[0];
  for (Point& p : ch) p = p - origin;
  o = {0, 0};
  Point low = ch[0], high = ch[0];
  for (const Point& p : ch) {
    low.x = min(low.x, p.x); low.y = min(low.y, p.y);
    high.x = max(high.x, p.x); high.y = max(high.y, p.y);
  }
  long double l = 0, r = min((long double)high.x - low.x, (long double)high.y - low.y) / 2;
  for (int it = 0; it < 80; ++it) {
    long double mid = (l + r) / 2;
    vector<Line> ls; ls.reserve(n);
    for (int i = 0; i < n; ++i) ls.emplace_back(ch[i], ch[(i + 1) % n], mid);
    vector<Line> res = half_plane_intersect(ls);
    if (res.empty()) { r = mid; continue; }
    l = mid; o = {0, 0};
    int m = res.size();
    for (int i = 0; i < m; ++i) {
      FPoint p = intersect_point(res[i], res[(i + 1) % m]);
      o.x += p.x / m; o.y += p.y / m;
    }
  }
  return {{o.x + origin.x, o.y + origin.y}, l};
}

```
