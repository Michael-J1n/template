#include <bits/stdc++.h>
using namespace std;

template<class T>
struct ChairmanTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum;
        int ls, rs;
    };
    int L, R;
    Node E = {0, 0, 0};
    Node Z = {0, 0, 0};
    vector<Node> tr;
    ChairmanTree() {}
    ChairmanTree(int _L, int _R) { init(_L, _R); }
    void init(int _L, int _R) {
        L = _L, R = _R;
        tr.clear();
        tr.reserve(1 << 20);
        tr.push_back(Z);
    }
    Node new_val(T v) { return {v, 0, 0}; }
    Node merge(Node a, Node b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.ls = 0;
        res.rs = 0;
        return res;
    }
    int new_node() {
        int id = tr.size();
        tr.push_back(Z);
        return id;
    }
    int new_node(Node x) {
        int id = tr.size();
        tr.push_back(x);
        return id;
    }
    Node get_node(int u) {
        if (!u) return Z;
        return tr[u];
    }
    int clone(int u) { return new_node(get_node(u)); }
    void pushup(int u) {
        Node cur = merge(get_node(tr[u].ls), get_node(tr[u].rs));
        tr[u].sum = cur.sum;
    }
    int build(int l, int r, vector<T>& a) {
        int u = new_node();
        if (l == r) {
            tr[u] = new_val(a[l]);
            return u;
        }
        int mid = l + r >> 1;
        tr[u].ls = build(l, mid, a);
        tr[u].rs = build(mid + 1, r, a);
        pushup(u);
        return u;
    }
    int modify(int u, int l, int r, int p, T v, int op) {
        int cur = clone(u);
        if (l == r) {
            if (op == 0) tr[cur] = new_val(v);
            else {
                tr[cur].sum += v;
            }
            return cur;
        }
        int mid = l + r >> 1;
        if (p <= mid) tr[cur].ls = modify(tr[cur].ls, l, mid, p, v, op);
        else tr[cur].rs = modify(tr[cur].rs, mid + 1, r, p, v, op);
        pushup(cur);
        return cur;
    }
    int set(int root, int p, T v) {
        if (p < L || p > R) return root;
        return modify(root, L, R, p, v, 0);
    }
    int apply(int root, int p, T v) {
        if (p < L || p > R) return root;
        return modify(root, L, R, p, v, 1);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return get_node(u);
        int mid = l + r >> 1;
        if (qr <= mid) return query(tr[u].ls, l, mid, ql, qr);
        if (ql > mid) return query(tr[u].rs, mid + 1, r, ql, qr);
        return merge(query(tr[u].ls, l, mid, ql, qr), query(tr[u].rs, mid + 1, r, ql, qr));
    }
    Node query(int root, int l, int r) {
        if (l > r) return E;
        l = max(l, L), r = min(r, R);
        if (l > r) return E;
        return query(root, L, R, l, r);
    }
    Node get(int root, int p) { return query(root, p, p); }
    int kth(int rtl, int rtr, int l, int r, int k) {
        if (l == r) return l;
        int mid = l + r >> 1;
        // tr[rt].ls：根为rt的树的左儿子编号，代表前一半值域的值域树；对应树根的sum值做差代表区间内前半值域的元素个数
        int cnt = tr[tr[rtr].ls].sum - tr[tr[rtl].ls].sum;
        if (k <= cnt) return kth(tr[rtl].ls, tr[rtr].ls, l, mid, k);
        else return kth(tr[rtl].rs, tr[rtr].rs, mid + 1, r, k - cnt);
    }
    int kth(int rtl, int rtr, int k) { return kth(rtl, rtr, L, R, k); }
};

/**
 * 主席树求静态区间第 k 小问题
 * 
 * 核心是用主席树维护树上前缀值域：建立一颗可持久化权值线段树；对于每一个前缀，都保存一个版本
 * 由于相同值域下，值域有可差分性质，且频率永远不为负，因此可以树上二分求出 kth
 * 
 * 具体来说，建立前缀主席树；root[0] 代表空前缀，后面 root[i + 1] 储存了 a[0..i] 的前缀上值域的权值线段树
 * 那么 [l, r] 的值频率，即为 root[r + 1] 与 root[l] 两颗权值线段树做差分
 * 对于第 k 小值，进行权值线段树上频率和的二分：如果 k 不超过左半边频率总和，那一定在左半边；否则右半边。最后返回 l（最终确定的值）
 * 实现上注意要离散化，可以将复杂度从 O(nlogV) 降低到 O(nlogn)
 * count_leq(l,r,x)：查询区间内 <=x 的元素数量
 * sum_k_smallest(l,r,k)：查询区间前 k 小元素之和
 * 前置条件：构造数组非空；查询满足 0<=l<=r<n；kth 的 k 位于 [1,r-l+1]，sum_k_smallest 的 k 位于 [0,r-l+1]
 */
struct staticKth {
    int n;
    vector<int> a, xs, rt, sum_rt;
    ChairmanTree<int> tr;
    ChairmanTree<long long> sum_tr;
    staticKth(vector<int>& nums) {
        n = nums.size();
        a = nums;
        xs.reserve(n);  // 离散化
        for (auto& x : a) xs.emplace_back(x);
        ranges::sort(xs); xs.erase(unique(xs.begin(), xs.end()), xs.end());
        int tot = xs.size();
        auto find = [&](int x) -> int { return ranges::lower_bound(xs, x) - xs.begin(); };
        for (auto& x : a) x = find(x);
        rt.resize(n + 1);  // rt[i]: value freq cnt on prefix a[0~i-1]
        sum_rt.resize(n + 1);
        tr.init(0, tot - 1);
        sum_tr.init(0, tot - 1);
        rt[0] = 0;
        sum_rt[0] = 0;
        for (int i = 0; i < n; ++i) {
            rt[i + 1] = tr.apply(rt[i], a[i], 1);
            sum_rt[i + 1] = sum_tr.apply(sum_rt[i], a[i], nums[i]);
        }
    }
    int query(int l, int r, int k) {
        int id = tr.kth(rt[l], rt[r + 1], k);  // frq cnts on a[l~r] = frq cnts on (pref[r] - pref[l-1]) = (rt[r+1] - rt[l])
        return xs[id];
    }
    int count_leq(int l, int r, int x) {
        int id = upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
        if (id < 0) return 0;
        return tr.query(rt[r + 1], 0, id).sum - tr.query(rt[l], 0, id).sum;
    }
    long long sum_k_smallest(int cnt_l, int cnt_r, int sum_l, int sum_r, int l, int r, int k) {
        if (l == r) return 1LL * k * xs[l];
        int mid = (l + r) >> 1;
        int left_cnt = tr.tr[tr.tr[cnt_r].ls].sum - tr.tr[tr.tr[cnt_l].ls].sum;
        if (k <= left_cnt) {
            return sum_k_smallest(tr.tr[cnt_l].ls, tr.tr[cnt_r].ls, sum_tr.tr[sum_l].ls, sum_tr.tr[sum_r].ls, l, mid, k);
        }
        long long left_sum = sum_tr.tr[sum_tr.tr[sum_r].ls].sum - sum_tr.tr[sum_tr.tr[sum_l].ls].sum;
        return left_sum + sum_k_smallest(tr.tr[cnt_l].rs, tr.tr[cnt_r].rs, sum_tr.tr[sum_l].rs, sum_tr.tr[sum_r].rs, mid + 1, r, k - left_cnt);
    }
    long long sum_k_smallest(int l, int r, int k) {
        return sum_k_smallest(rt[l], rt[r + 1], sum_rt[l], sum_rt[r + 1], 0, xs.size() - 1, k);
    }
};
