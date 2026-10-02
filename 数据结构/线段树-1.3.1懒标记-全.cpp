#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vll = vector<ll>;

/**
 * 懒标记线段树
 * 支持区间赋值、区间加、区间取反、区间查询、区间内二分
 * 
 * 懒标记定义：
 * has_set[u] = 1 表示当前区间存在待下传的区间赋值
 * set_tag[u] 表示把整个区间赋值为 set_tag[u]
 * add_tag[u] 表示把整个区间加上 add_tag[u]
 * neg_tag[u] 表示把整个区间取反一次（注意这里 neg 是 mul 只有 x-1 操作的特殊形式）
 *
 * 基本操作：
 * set(l, r, v)：把区间 [l, r] 全部赋值为 v
 * add(l, r, v)：把区间 [l, r] 全部加上 v
 * neg(l, r)：把区间 [l, r] 全部取反
 * query(l, r)：查询区间 [l, r] 的 Node
 * range_sum(l, r)：查询区间和
 * range_max(l, r)：查询区间最大值
 * range_min(l, r)：查询区间最小值
 *
 * 单点操作：
 * set(p, v)：把 a[p] 修改为 v
 * add(p, v)：把 a[p] 加上 v
 * get(p)：查询 a[p]
 *
 * 线段树上二分：
 * **注意 find_first/last 中，check(node)==false 必须代表整个区间都不可能有答案；max_right/min_left 必须要求随着区间扩展，check 有 TTT...FFF 的单调性**
 * 
 * find_first(l, r, check)：找 [l, r] 中 **第一个满足要求的下标**；不存在返回 n
 *      check = [&](Node node) { return node 代表的这个节点区间里面，是否至少存在一个答案位置；}。
 *      形象理解即为如果 check(node)，就在这个 node 下继续递归找；node 最后一定会递归到满足 check 条件的叶子节点
 *      
 * find_last(l, r, check): 找 [l, r] 中 **最后一个满足要求的下标**；不存在返回 -1
 *      check = [&](Node node) { return node 代表的这个节点区间里面，是否至少存在一个答案位置；}。
 *      形象理解即为如果 check(node)，就在这个 node 下继续递归找；node 最后一定会递归到满足 check 条件的叶子节点
 * 
 * max_right(l, r, check): 从 l 向右累计，返回 **第一个使累计区间!不合法!的位置**；全合法返回 r + 1；重载时省略 r，默认 n - 1
 *      check = [&](Node node) { return node 代表的这个累计区间，是否满足条件并可以继续向右合并；}。
 *      形象理解即为只要 check(node) 就合并这个 node 然后继续向右；node 最后一定是一个第一次不满足 check 条件的 merge 节点
 *      注意 l 开始到 返回值-1 的累计区间是合法的
 * 
 * min_left(l, r, check): 从 r 向左累计，返回 **第一个使累计区间!不合法!的位置**；全合法返回 l - 1；重载时省略 l，默认 0
 *      check = [&](Node node) { return node 代表的这个累计区间，是否满足条件并可以继续向左合并；}。
 *      形象理解为只要 check(node) 就合并这个 node 然后继续向左；node 最后一定是一个第一次不满足 check 条件的 merge 节点
 *      注意 l 开始到 返回值-1 的累计区间是合法的
 * 
 * 包装接口：
 * first_ge(l, r, v)：返回 [l, r] 中第一个 >= v 的位置，不存在返回 n
 * last_ge(l, r, v)：返回 [l, r] 中最后一个 >= v 的位置，不存在返回 -1
 * first_le(l, r, v)：返回 [l, r] 中第一个 <= v 的位置，不存在返回 n
 * last_le(l, r, v)：返回 [l, r] 中最后一个 <= v 的位置，不存在返回 -1
 * lower_bound_sum(l, k)：返回从 l 开始，前缀和第一次 >= k 的位置
 * lower_bound_sum(l, r, k)：返回 [l, r] 内前缀和第一次 >= k 的位置，不存在返回 r + 1
 * lower_bound_sum_rev(r, k)：返回从 r 开始向左，后缀和第一次 >= k 的位置
 * lower_bound_sum_rev(l, r, k)：返回 [l, r] 内后缀和第一次 >= k 的位置，不存在返回 l - 1
 * 注意： lower_bound_sum 依赖 check 的单调性；通常要求被搜索方向上的元素非负
 * 
 * 复杂度：
 * 建树 O(n)；修改/查询/二分 O(log n)
 * 前置条件：构造数组非空；修改、查询与搜索区间均合法且非空
 */
template<class T>
struct LazySegTree {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T sum, mx, mn;
    };
    int n;
    Node E = {0, -INF, INF};
    vector<Node> tr;
    vector<T> add_tag, set_tag;
    vector<char> has_set;
    vector<char> neg_tag;
    LazySegTree() {}
    LazySegTree(int _n) : n(_n) {
        tr.assign(n * 4 + 5, {0, 0, 0});
        add_tag.assign(n * 4 + 5, 0);
        set_tag.assign(n * 4 + 5, 0);
        has_set.assign(n * 4 + 5, 0);
        neg_tag.assign(n * 4 + 5, 0);
    }
    LazySegTree(const vector<T>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        add_tag.assign(n * 4 + 5, 0);
        set_tag.assign(n * 4 + 5, 0);
        has_set.assign(n * 4 + 5, 0);
        neg_tag.assign(n * 4 + 5, 0);
        build(1, 0, n - 1, a);
    }
    Node merge(const Node& a, const Node& b) {
        Node res;
        res.sum = a.sum + b.sum;
        res.mx = max(a.mx, b.mx);
        res.mn = min(a.mn, b.mn);
        return res;
    }
    void pushup(int u) {
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void apply_set(int u, int l, int r, T v) {
        tr[u] = {v * (r - l + 1), v, v};
        set_tag[u] = v;
        add_tag[u] = 0;
        has_set[u] = 1;
        neg_tag[u] = 0;
    }
    void apply_add(int u, int l, int r, T v) {
        tr[u].sum += v * (r - l + 1);
        tr[u].mx += v;
        tr[u].mn += v;
        if (has_set[u]) set_tag[u] += v;
        else add_tag[u] += v;
    }
    void apply_neg(int u) {
        tr[u].sum = -tr[u].sum;
        T nmx = -tr[u].mn, nmn = -tr[u].mx;
        tr[u].mn = nmn, tr[u].mx = nmx;
        if (has_set[u]) {
            set_tag[u] = -set_tag[u];
        } else {
            add_tag[u] = -add_tag[u], neg_tag[u] ^= 1;
        }
    }
    void pushdown(int u, int l, int r) {
        int mid = (l + r) >> 1;
        if (has_set[u]) {
            apply_set(u << 1, l, mid, set_tag[u]);
            apply_set(u << 1 | 1, mid + 1, r, set_tag[u]);
            has_set[u] = 0;
        }
        if (neg_tag[u]) {
            apply_neg(u << 1);
            apply_neg(u << 1 | 1);
            neg_tag[u] = 0;
        }
        if (add_tag[u]) {
            apply_add(u << 1, l, mid, add_tag[u]);
            apply_add(u << 1 | 1, mid + 1, r, add_tag[u]);
            add_tag[u] = 0;
        }
    }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) {
            tr[u] = {a[l], a[l], a[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void add(int u, int l, int r, int ql, int qr, T v) {
        if (ql <= l && r <= qr) {
            apply_add(u, l, r, v);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
        if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
        pushup(u);
    }
    void add(int l, int r, T v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        add(1, 0, n - 1, l, r, v);
    }
    void set(int u, int l, int r, int ql, int qr, T v) {
        if (ql <= l && r <= qr) {
            apply_set(u, l, r, v);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) set(u << 1, l, mid, ql, qr, v);
        if (qr > mid) set(u << 1 | 1, mid + 1, r, ql, qr, v);
        pushup(u);
    }
    void set(int l, int r, T v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        set(1, 0, n - 1, l, r, v);
    }
    void neg(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            apply_neg(u);
            return;
        }
        pushdown(u, l, r);
        int mid = l + r >> 1;
        if (ql <= mid) neg(u << 1, l, mid, ql, qr);
        if (qr > mid) neg(u << 1 | 1, mid + 1, r, ql, qr);
        pushup(u);
    }
    void neg(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        neg(1, 0, n - 1, l, r);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    }
    Node query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return E;
        return query(1, 0, n - 1, l, r);
    }
    T range_sum(int l, int r) { return query(l, r).sum; }
    T range_max(int l, int r) { return query(l, r).mx; }
    T range_min(int l, int r) { return query(l, r).mn; }
    void add(int p, T v) { add(p, p, v); }
    void set(int p, T v) { set(p, p, v); }
    T get(int p) { return query(p, p).sum; }
    vector<T> to_vec() {
        vector<T> a(n);
        for (int i = 0; i < n; ++i) a[i] = get(i);
        return a;
    }
    /*-----------------------------------------binary search on tree--------------------------------------------------*/
    template<class F>
    int find_first(int u, int l, int r, int ql, int qr, F check) {
        if (qr < l || r < ql) return n;
        if (ql <= l && r <= qr && !check(tr[u])) return n;
        if (l == r) return l;
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        int res = find_first(u << 1, l, mid, ql, qr, check);
        if (res != n) return res;
        return find_first(u << 1 | 1, mid + 1, r, ql, qr, check);
    }
    template<class F>
    int find_first(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return n;
        return find_first(1, 0, n - 1, l, r, check);
    }
    template<class F>
    int find_last(int u, int l, int r, int ql, int qr, F check) {
        if (qr < l || r < ql) return -1;
        if (ql <= l && r <= qr && !check(tr[u])) return -1;
        if (l == r) return l;
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        int res = find_last(u << 1 | 1, mid + 1, r, ql, qr, check);
        if (res != -1) return res;
        return find_last(u << 1, l, mid, ql, qr, check);
    }
    template<class F>
    int find_last(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return -1;
        return find_last(1, 0, n - 1, l, r, check);
    }
    template<class F>
    int max_right(int u, int l, int r, int ql, int qr, Node& cur, F check) {
        if (qr < l || r < ql) return n;
        if (ql <= l && r <= qr) {
            Node nxt = merge(cur, tr[u]);
            if (check(nxt)) {
                cur = nxt;
                return n;
            }
            if (l == r) return l;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        int res = max_right(u << 1, l, mid, ql, qr, cur, check);
        if (res != n) return res;
        return max_right(u << 1 | 1, mid + 1, r, ql, qr, cur, check);
    }
    template<class F>
    int max_right(int l, F check) {
        l = max(l, 0);
        if (l >= n) return n;
        Node cur = E;
        return max_right(1, 0, n - 1, l, n - 1, cur, check);
    }
    template<class F>
    int max_right(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return min(l, n);
        Node cur = E;
        int res = max_right(1, 0, n - 1, l, r, cur, check);
        return res == n ? r + 1 : res;
    }
    template<class F>
    int min_left(int u, int l, int r, int ql, int qr, Node& cur, F check) {
        if (qr < l || r < ql) return -1;
        if (ql <= l && r <= qr) {
            Node nxt = merge(tr[u], cur);
            if (check(nxt)) {
                cur = nxt;
                return -1;
            }
            if (l == r) return l;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        int res = min_left(u << 1 | 1, mid + 1, r, ql, qr, cur, check);
        if (res != -1) return res;
        return min_left(u << 1, l, mid, ql, qr, cur, check);
    }
    template<class F>
    int min_left(int r, F check) {
        r = min(r, n - 1);
        if (r < 0) return -1;
        Node cur = E;
        return min_left(1, 0, n - 1, 0, r, cur, check);
    }
    template<class F>
    int min_left(int l, int r, F check) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return r;
        Node cur = E;
        int res = min_left(1, 0, n - 1, l, r, cur, check);
        return res == -1 ? l - 1 : res;
    }
    int first_ge(int l, int r, T v) { return find_first(l, r, [&](auto x) { return x.mx >= v; }); }
    int last_ge(int l, int r, T v) { return find_last(l, r, [&](auto x) { return x.mx >= v; }); }
    int first_le(int l, int r, T v) { return find_first(l, r, [&](auto x) { return x.mn <= v; }); }
    int last_le(int l, int r, T v) { return find_last(l, r, [&](auto x) { return x.mn <= v; }); }
    int lower_bound_sum(int l, T k) { return max_right(l, [&](auto x) { return x.sum < k; }); }
    int lower_bound_sum(int l, int r, T k) { return max_right(l, r, [&](auto x) { return x.sum < k; }); }
    int lower_bound_sum_rev(int r, T k) { return min_left(r, [&](auto x) { return x.sum < k; }); }
    int lower_bound_sum_rev(int l, int r, T k) { return min_left(l, r, [&](auto x) { return x.sum < k; }); }
};


/**
 * 懒标记线段树怎么写（多个标记时）
 * 懒标记线段树的关键在于维护把标记贡献到当且节点 (apply) 并将标记下传给儿子 (pushdown)
 * 在 apply 与 pushdown 时，处理不同标记的先后顺序是很重要的
 * 
 * 节点 u 上的懒标记 tag 代表：u 以及是正确的真实值了，u 子树内不包括 u 的所有节点带有标记 tag
 * apply 代表将当前线段树节点 u 的 tag 给贡献到自身数据上
 * pushdown 代表把标记下传两个儿子，并清空自己的标记
 * 
 * 一. 
 * 对于 apply 一个新的值，分两步走：
 * 1. 把节点标记转化成自身真实值贡献；
 * 2. 把新的 tag 值和已有 tag 值相融合；这里考虑不同 tag 的复合要注意如何维持一个正确的仿射变换（但 set 的优先级太大了，有 set_tag 的化要分别讨论有 set_tag 和没有 set_tag 的仿射变换）；
 *    注意这个仿射变换 apply 维护的先后关系就是 pushdown 中下传标记的顺序
 *    这个顺序不变量意味着，apply 前后的仿射公式是一样的（例如 set -> set，没有 set 时 m*x + c -> set 或 m * x + c）
 *      以区间和查询和区间加，区间乘为例，显然需要维护一个 add_tag 和 mul_tag；
 *      对于 apply_add(u,l,r,v)，那么区间会加上 (r-l+1)*v，有 (x*m+a)+c = x*m + a+c，因此 mul_tag 不会被影响，而 add_tag 会加上 v
 *      对于 apply_mul(u,l,r,v)，那么区间会加上 tr[u].sum*(v-1)，如果有加法标记，那 (x*m+c)*v = x*m*v + c*v，因此 mul_tag*v, add_tag*v
 * 
 * 二. 
 * 对于 pushdown 一个节点：
 * 按照 tags 的优先级顺序考虑，对于左右儿子都 apply 这个 tag，并把当且节点的 tag 变成单位元
 *      还是以同样的例子，显然乘法优先级更高；
 *      因此先对于左右儿子 apply_mul，再把当前节点的 mul_tag 设为 1，再对于左右儿子 apply_add，再把当前节点的 add_tag 设为 0
 * 
 * 三.
 * 对于修改一个节点：
 * Base case 是递归到节点完全包含于修改区间 (ql <= l && r <= qr)，这时候直接相应的对这个节点 apply tag
 * 否则先要 pushdown 这个节点，再判断左右儿子区间是否和查询区间有相交 (ql<= mid 左，qr > mid 右)，然后递归修改，最后 pushup 上来
 * 
 * 四.
 * 对于查询一个节点：
 * Base case 也是递归到节点完全包含于修改区间，这时候返回当前 node 的统计值
 * 否则先要 pushdown 这个节点，再分别统计左右儿子的信息并做融合。不用 pushup。
 */
int p;
// 前置条件：构造数组非空，p>0；区间合法且非空；所有 long long 乘法中间结果均不溢出
struct LazySegTree2 {
    int n;
	vll tr;
	vll add_tag, mul_tag;
	LazySegTree2(const vll& a) : n(a.size()) {
		tr.assign(n * 4 + 5, 0);
		add_tag.assign(n * 4 + 5, 0);
		mul_tag.assign(n * 4 + 5, 1);
		build(1, 0, n - 1, a);
	}
	void pushup(int u) {
		tr[u] = (tr[u << 1] + tr[u << 1 | 1]) % p;
	}
	void apply_add(int u, int l, int r, ll v) {
		(tr[u] += v * (r - l + 1)) %= p;
		(add_tag[u] += v) %= p;
	}
	void apply_mul(int u, int l, int r, ll v) {
		tr[u] = v * tr[u] % p;
		if (add_tag[u]) (add_tag[u] *= v) %= p;
		(mul_tag[u] *= v) %= p;
	}
	void pushdown(int u, int l, int r) {
		int mid = l + r >> 1;
		if (mul_tag[u] != 1) {
			apply_mul(u << 1, l, mid, mul_tag[u]);
			apply_mul(u << 1 | 1, mid + 1, r, mul_tag[u]);
			mul_tag[u] = 1;
		}
		if (add_tag[u]) {
			apply_add(u << 1, l, mid, add_tag[u]);
			apply_add(u << 1 | 1, mid + 1, r, add_tag[u]);
			add_tag[u] = 0;
		}
	}
	void build(int u, int l, int r, const vll& a) {
		if (l == r) {
			tr[u] = a[l] % p;
			return;
		}
		int mid = l + r >> 1;
		build(u << 1, l, mid, a);
		build(u << 1 | 1, mid + 1, r, a);
		pushup(u);
	}
	void add(int u, int l, int r, int ql, int qr, ll v) {
		if (ql <= l && r <= qr) {
			apply_add(u, l, r, v);
			return;
		}
		pushdown(u, l, r);
		int mid = l + r >> 1;
		if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
		if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
		pushup(u);
	}
	void add(int l, int r, ll v) {
		add(1, 0, n - 1, l, r, v);
	}
	void mul(int u, int l, int r, int ql, int qr, ll v) {
		if (ql <= l && r <= qr) {
			apply_mul(u, l, r, v);
			return;
		}
		pushdown(u, l, r);
		int mid = l + r >> 1;
		if (ql <= mid) mul(u << 1, l, mid, ql, qr, v);
		if (qr > mid) mul(u << 1 | 1, mid + 1, r, ql, qr, v);
		pushup(u);
	}
	void mul(int l, int r, ll v) {
		mul(1, 0, n - 1, l, r, v);
	}
	ll query(int u, int l, int r, int ql, int qr) {
		if (ql <= l && r <= qr) {
			return tr[u];
		}
		pushdown(u, l, r);
		int mid = l + r >> 1;
		ll res = 0;
		if (ql <= mid) (res += query(u << 1, l, mid, ql, qr)) %= p;
		if (qr > mid) (res += query(u << 1 | 1, mid + 1, r, ql, qr)) %= p;
		return res;
	}
	ll query(int l, int r) {
		return query(1, 0, n - 1, l, r);
	}
};


/**
 * 练习1：有区间set,mul,add 的线段树
 * 
 * 优先级：set>mul>add，因此维护仿射变换为：有 set 就是 set 值，否则 mx + c
 * 注意 set 没有单位元，因此加入 has_set 来判断当前是否有set标记
 * 按照优先级来看 apply 的过程：
 * apply set：
 * 直接更新 set_tag 以及 has_set，再相应的把 add_tag 和 mul_tag 变成单位元
 * apply mul：
 * 如果 has_set，那么 set_tag 会被 x v，否则 (mx+c)*v -> mvx + cv，因此 add 和 mul 的 tag 都要 x v
 * apply add：
 * 如果 has_set，那么 set_tag 会被 + v，否则 (mx+c)+v -> mx + c+v，因此只有 add_tag 被 + v
 * 前置条件：构造数组非空，p>0；区间合法且非空；所有 long long 乘法中间结果均不溢出
 */
// int p;
struct LazySegTree3 {
	int n;
	vll tr;
	vll set_tag, mul_tag, add_tag;
	vector<char> has_set;
	LazySegTree3(vll& a) : n(a.size()) {
		tr.assign(n * 4 + 5, 0);
		set_tag.assign(n * 4 + 5, 0);
		has_set.assign(n * 4 + 5, 0);
		mul_tag.assign(n * 4 + 5, 1);
		add_tag.assign(n * 4 + 5, 0);
		build(1, 0, n - 1, a);
	}
	void pushup(int u) {
		tr[u] = (tr[u << 1] + tr[u << 1 | 1]) % p;
	}
	void apply_set(int u, int l, int r, ll v) {
		tr[u] = (v * (r - l + 1)) % p;
		set_tag[u] = v % p;
		has_set[u] = 1;
		mul_tag[u] = 1;
		add_tag[u] = 0;
	}
	void apply_mul(int u, int l, int r, ll v) {
		tr[u] = (tr[u] * v) % p;
		if (has_set[u]) {
			(set_tag[u] *= v) %= p;
		} else {
			(add_tag[u] *= v) %= p;
			(mul_tag[u] *= v) %= p;
		}
	}
	void apply_add(int u, int l, int r, ll v) {
		(tr[u] += v * (r - l + 1)) %= p;
		if (has_set[u]) {
			(set_tag[u] += v) %= p;
		} else {
			(add_tag[u] += v) %= p;
		}
	}
	void pushdown(int u, int l, int r) {
		int mid = l + r >> 1;
		if (has_set[u]) {
			apply_set(u << 1, l, mid, set_tag[u]);
			apply_set(u << 1 | 1, mid + 1, r, set_tag[u]);
			has_set[u] = 0;
		}
		if (mul_tag[u] != 1) {
			apply_mul(u << 1, l, mid, mul_tag[u]);
			apply_mul(u << 1 | 1, mid + 1, r, mul_tag[u]);
			mul_tag[u] = 1;
		}
		if (add_tag[u]) {
			apply_add(u << 1, l, mid, add_tag[u]);
			apply_add(u << 1 | 1, mid + 1, r, add_tag[u]);
			add_tag[u] = 0;
		}
	}
	void build(int u, int l, int r, vll& a) {
		if (l == r) {
			tr[u] = a[l] % p;
			return;
		}
		int mid = l + r >> 1;
		build(u << 1, l, mid, a);
		build(u << 1 | 1, mid + 1, r, a);
		pushup(u);
	}
	void set(int u, int l, int r, int ql, int qr, ll v) {
		if (ql <= l && r <= qr) {
			apply_set(u, l, r, v);
			return;
		}
		pushdown(u, l, r);
		int mid = l + r >> 1;
		if (ql <= mid) set(u << 1, l, mid, ql, qr, v);
		if (qr > mid) set(u << 1 | 1, mid + 1, r, ql, qr, v);
		pushup(u);
	}
	void set(int l, int r,  ll v) {
		set(1, 0, n - 1, l, r, v);
	}
	void mul(int u, int l, int r, int ql, int qr, ll v) {
		if (ql <= l && r <= qr) {
			apply_mul(u, l, r, v);
			return;
		}
		pushdown(u, l, r);
		int mid = l + r >> 1;
		if (ql <= mid) mul(u << 1, l, mid, ql, qr, v);
		if (qr > mid) mul(u << 1 | 1, mid + 1, r, ql, qr, v);
		pushup(u);
	}
	void mul(int l, int r, ll v) {
		mul(1, 0, n - 1, l, r, v);
	}
	void add(int u, int l, int r, int ql, int qr, ll v) {
		if (ql <= l && r <= qr) {
			apply_add(u, l, r, v);
			return;
		}
		pushdown(u, l, r);
		int mid = l + r >> 1;
		if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
		if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
		pushup(u);
	}
	void add(int l, int r, ll v) {
		add(1, 0, n - 1, l, r, v);
	}
	ll query(int u, int l, int r, int ql, int qr) {
		if (ql <= l && r <= qr) {
			return tr[u];
		}
		pushdown(u, l, r);
		int mid = l + r >> 1;
		ll res = 0;
		if (ql <= mid) (res += query(u << 1, l, mid, ql, qr)) %= p;
		if (qr > mid) (res += query(u << 1 | 1, mid + 1, r, ql, qr)) %= p;
		return res;
	}
	ll query(int l, int r) {
		return query(1, 0, n - 1, l, r);
	}
};


/**
 * 练习 2：操作为区间+k，查询为区间均值/方差，如何维护？
 * 
 * 首先只有区间+k，因此维护一个加法懒标记。关键是方差中有 sum X^2 的项，每个节点的值除了 sum = sum X，还要维护一个 sum2 = sum X^2
 * 那么只需要在区间加时，在 apply 中正确维护 sum2 的增量即可。
 * 注意到 sum X^2 --> sum (X+k)^2 的增量为 2k*sum X + k*k，因此把增量加入 sum2 即可。sum 与 add_tag 正常维护。
 * 
 * 注意两个细节：
 * 1. 增量中的 sum X 是区间还没有 +k 时的 sum，因此要先更新 sum2，再更新 sum，否则 sum2 的增量就过大了
 * 2. 是否需要考虑是 sum (X+k1)^2 --> sum (X+k1+k2)^2 的处理？答案是不需要，这里 sum2 的不变量就是 sum X^2，考虑之前如果更新过，那么 k1 带来的增量同样会更新 sum，因此增量计算时 sum 就包含之前 k1 的影响了
 * 启发：
 * 1. apply 维护节点值与 tag 值时抓住不变量，在 apply_tag 的时候不要考虑之前的旧值影响（因为已经被之前的操作维护了）
 * 2. 对于维护的节点值是复合函数，看如何从 sum f(x) 写为 sum f(x+v) := sum f(x) + Delta，这样节点值 +Delta，add_tag +v，并且不用考虑之前 +v_old 的影响
 *      类似的一个例子：维护区间加，以及区间 sin(x) 求和
 *      考虑 sum sin(x) -> sum sin(x + v) = sum sinx cosv + cosx sinv = cosv sum sinx + sinv sum cosx，因此要维护 sum_sin 和 sum_cos。sum cos(x) -> sum cos(x+v) 同理展开即可。注意在 apply tag 的时候不要串联更新！
 * 前置条件：构造数组非空；修改、查询区间合法且非空
 */
struct LazySegTree4 {
    static constexpr long double INF = numeric_limits<long double>::max() / 2;
    struct Node {
        long double sum, sum2;  // sum x, sum x^2
    };
    int n;
    Node E = {0, 0};
    vector<Node> tr;
    vector<long double> add_tag;
    LazySegTree4(const vector<long double>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        add_tag.assign(n * 4 + 5, 0);
        build(1, 0, n - 1, a);
    }
    Node merge(const Node& a, const Node& b) {
        long double cursum = a.sum + b.sum;
        long double cursum2 = a.sum2 + b.sum2;
        return {cursum, cursum2};
    }
    void pushup(int u) {
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void apply_add(int u, int l, int r, long double v) {
        long double len = r - l + 1;
        tr[u].sum2 += 2 * v * tr[u].sum + v * v * len;
        tr[u].sum += v * (r - l + 1);
        add_tag[u] += v; 
    }
    void pushdown(int u, int l, int r) {
        int mid = (l + r) >> 1;
        if (add_tag[u]) {
            apply_add(u << 1, l, mid, add_tag[u]);
            apply_add(u << 1 | 1, mid + 1, r, add_tag[u]);
            add_tag[u] = 0;
        }
    }
    void build(int u, int l, int r, const vector<long double>& a) {
        if (l == r) {
            tr[u] = {a[l], a[l] * a[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void add(int u, int l, int r, int ql, int qr, long double v) {
        if (ql <= l && r <= qr) {
            apply_add(u, l, r, v);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) add(u << 1, l, mid, ql, qr, v);
        if (qr > mid) add(u << 1 | 1, mid + 1, r, ql, qr, v);
        pushup(u);
    }
    void add(int l, int r, long double v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        add(1, 0, n - 1, l, r, v);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    }
    Node query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return E;
        return query(1, 0, n - 1, l, r);
    }
    long double avg(int l, int r) {
    	long double len = r - l + 1;
    	Node node = query(l, r);
    	long double ex = node.sum / len;
    	return ex;
    }
    long double var(int l, int r) {
    	long double len = r - l + 1;
    	Node node = query(l, r);
    	long double ex2 = node.sum2 / len;
    	long double ex = node.sum / len;
    	return ex2 - ex * ex;
    }
};


/**
 * 线段树上二分专题讲解：
 * 有两种类型的二分（单点与区间），要分辨什么时候用哪个：
 *      find_first: 答案由某个单点 a[i] 是否满足条件决定。check(node) 表示这个节点区间里是否可能存在这样的点
 *      max_right: 答案由整个累计区间 [l,i] 是否满足条件决定。check(node) 表示当前累计区间还未越过边界，可以争端合并
 * 大部分线段树上二分的难题采用的是 max_right；find_first 只适合处理找一个单点，而 max_right 适合处理性质or限制是跨区间的题目
 * 
 * 例子：
 * find_first 形：
 * 1. 找第一个 >=v 的 a[i]：int p = tr.find_first(l, r, [&](auto node) { return node.mx >= v; }); 否则最大值都没有 v，这个区间中肯定没有符合的点
 * 2. 找第一个 a[i] 不在 [lo,hi] 的范围：int p = tr.find_first(l, r, [&](auto node) { return a[i].mn < lo || a[i].mx > high; }); 
 * 
 * max_right 形（注意check条件要写**反向**，因为找的是第一个不满足点）：
 * 1. 第一个 p，使得 sum(l,p)>=k: int p = tr.max_right(l, r, [&](auto node) { return node.sum < k; }); 注意这里返回第一个不满足 check 的，所以 check 要和希望的方向取反
 * 2. 最后一个 p，使得 l 到 p 的所有元素都在 [lo,hi] 区间：int p = tr.max_right(l, r, [&](auto node) { return lo <= node.mn && node.mx <= hi; }) - 1;
 * 接下来假设节点维护了 pre:最长全1前缀，suf:最长全1后缀，mx:最长连续1
 * 3. 第一个 p，使得 [l,p] 中出现连续 k 个 1：int p = tr.max_right(l, r, [&](auto node) { return node.mx < k; });  如果连续1个数比k小就一直往右合并，直到第一个不满足的位置即为第一个满足==k个连续1的位置。
 */


/**
 * 附：01 线段树：值只有 0，1 的线段树
 * 【P2572】
 * Node 维护前缀 1 个数，后缀 1 个数，最大连续 1 个数，区间长度，前缀 0 个数，后缀 0 个数，最大连续 0 个数，1 的总个数
 * 支持区间 set，区间 neg（0变1，1变0）
 * 查询1返回1的总数，查询2返回最大连续1个数
 * 前置条件：构造数组非空且元素仅为 0/1；set 的赋值也仅为 0/1；区间合法且非空
 */
template<class T>
struct LazySegTree01 {
    static constexpr T INF = numeric_limits<T>::max() / 2;
    struct Node {
        T pre, suf, mx, len;
    	T pre0, suf0, mx0;
    	T sum;
    };
    int n;
    Node E = {0, 0, 0, 1, 0, 0, 0, 0};
    vector<Node> tr;
    vector<T> set_tag;
    vector<char> has_set;
    vector<char> neg_tag;
    LazySegTree01(const vector<T>& a) : n(a.size()) {
        tr.assign(n * 4 + 5, E);
        set_tag.assign(n * 4 + 5, 0);
        has_set.assign(n * 4 + 5, 0);
        neg_tag.assign(n * 4 + 5, 0);
        build(1, 0, n - 1, a);
    }
    Node merge(const Node& a, const Node& b) {
    	Node res;
    	res.pre = a.pre;
    	if (a.len == a.pre) res.pre = a.len + b.pre;
    	res.suf = b.suf;
    	if (b.len == b.suf) res.suf = b.len + a.suf;
    	res.mx = max(max(a.mx, b.mx), a.suf + b.pre);
    	res.len = a.len + b.len;
    	res.pre0 = a.pre0;
    	if (a.len == a.pre0) res.pre0 = a.len + b.pre0;
    	res.suf0 = b.suf0;
    	if (b.len == b.suf0) res.suf0 = b.len + a.suf0;
    	res.mx0 = max(max(a.mx0, b.mx0), a.suf0 + b.pre0);
    	res.sum = a.sum + b.sum;
    	return res;
    }
    void pushup(int u) {
        tr[u] = merge(tr[u << 1], tr[u << 1 | 1]);
    }
    void apply_set(int u, int l, int r, T v) {
        int len = r - l + 1;
        tr[u] = {len * v, len * v, len * v, len, len * (v ^ 1), len * (v ^ 1), len * (v ^ 1), len * v};
        set_tag[u] = v;
        has_set[u] = 1;
        neg_tag[u] = 0;
    }
    void apply_neg(int u) {
    	Node old = tr[u];
        tr[u] = {old.pre0, old.suf0, old.mx0, old.len, old.pre, old.suf, old.mx, old.len - old.sum};
        if (has_set[u]) {
            set_tag[u] ^= 1;
        } else {
            neg_tag[u] ^= 1;
        }
    }
    void pushdown(int u, int l, int r) {
        int mid = (l + r) >> 1;
        if (has_set[u]) {
            apply_set(u << 1, l, mid, set_tag[u]);
            apply_set(u << 1 | 1, mid + 1, r, set_tag[u]);
            has_set[u] = 0;
        }
        if (neg_tag[u]) {
            apply_neg(u << 1);
            apply_neg(u << 1 | 1);
            neg_tag[u] = 0;
        }
    }
    void build(int u, int l, int r, const vector<T>& a) {
        if (l == r) {
            tr[u] = {a[l], a[l], a[l], 1, (a[l] ^ 1), (a[l] ^ 1), (a[l] ^ 1), a[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        pushup(u);
    }
    void set(int u, int l, int r, int ql, int qr, T v) {
        if (ql <= l && r <= qr) {
            apply_set(u, l, r, v);
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) set(u << 1, l, mid, ql, qr, v);
        if (qr > mid) set(u << 1 | 1, mid + 1, r, ql, qr, v);
        pushup(u);
    }
    void set(int l, int r, T v) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        set(1, 0, n - 1, l, r, v);
    }
    void neg(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            apply_neg(u);
            return;
        }
        pushdown(u, l, r);
        int mid = l + r >> 1;
        if (ql <= mid) neg(u << 1, l, mid, ql, qr);
        if (qr > mid) neg(u << 1 | 1, mid + 1, r, ql, qr);
        pushup(u);
    }
    void neg(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return;
        neg(1, 0, n - 1, l, r);
    }
    Node query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(u << 1, l, mid, ql, qr);
        if (ql > mid) return query(u << 1 | 1, mid + 1, r, ql, qr);
        return merge(query(u << 1, l, mid, ql, qr), query(u << 1 | 1, mid + 1, r, ql, qr));
    }
    Node query(int l, int r) {
        l = max(l, 0), r = min(r, n - 1);
        if (l > r) return E;
        return query(1, 0, n - 1, l, r);
    }
    T query1(int l, int r) {
    	Node node = query(l, r);
    	return node.sum;
    }
    T query2(int l, int r) {
    	Node node = query(l, r);
    	return node.mx;
    }
};
