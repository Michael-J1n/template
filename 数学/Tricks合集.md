## 推式子

1. 固定 cur，x 取 [l,r] 之间值，
sum |x - cur| = totalSum - totalCnt * cur + 2 * (cnt * cur - sum)
其中 cnt = {y: y < x}，sum = sum{y: y < x}，totalSum 和 totalCnt 是区间中元素总和和总个数
这可以使用权值树状数组进行计算


## GCD 与 LCM

1. 【ABC445e】给定数列 a，对于每个 i，求 LCM_{j!=i}(a[j]) % MOD 的值
虽然 LCM 满足结合律可以线段树查询，但是会严重爆 LL，并且 lcm(a,lcm(b,c))%mod != lcm(a,lcm(b,c)%mod)%mod，所以不能直接合并
考虑每个 a[i] 分解质因数为 p1^a1*p2^a2...，对于所有 a，所有出现过的素数 pi 的最大出现过的指数为 mxi，那么 LCM(a[i])=p1^mx1 p2^mx2 ...
现在考虑把一个 a[i] 给移除。那么对于 a[i] 中所有 pi^ai，如果 ai==mxi，说明 LCM 将会变小，对应的 pi 的指数将变为 sei，为所有出现中第二大的指数（没有第二大就是0）
因此可以先 LPF 快速预处理分解 a 得到所有 p 和对应 mx,se，再处理所有 qpow(p,mx)%mod 的连乘。对于每个 i，判断每个质数是否会掉到 se，这时只要乘以 inv(qpow(p,mx-se)) 即可


## 组合数相关

1. 截断组合数：求不带 mod 的组合数，但当组合数超过一个阈值 k 时就提前返回。如何避免溢出和快速截断？
注意到 nCm = n!/(m!(n-m)!) = n(n-1)..(n-m+1)/m(m-1)..1 = n/1 * (n-1)/2 * (n-2)/3 * ...
因此定义 res=1，然后循环 res*=(n-i)/=(i+1) 即可，当超过阈值就直接返回。
```CPP
// 截断到 k 的 C(n,m); 注意要取 y=min(m,n-m)，做 C(n,y)，保证计算过程单调上升从而可以提前截断
long long res = 1;
int y = min(m, n - m);
for (int i = 0; i < y; ++i) {
    res *= 1LL * (n - i);
    res /= (i + 1);
    if (res >= k) return k;
}
return res;
```

2. 给定 sz 个位置，每个字符有 cnt1,cnt2,..,cntk 个，求不同的排列个数：
根据排列公式，答案为 (sz)!/(cnt1!*cnt2!.. *cntk!)，但这样计算会溢出。
换个方向考虑：对于第一个字符，sz 中选 cnt1 个位置，接下来的字符在 sz-cnt1 中选 cnt2 个位置.. 因此这个公式等于：
C(sz, cnt1) * C(sz-cnt1, cnt2) * ... 变为 k 个组合数计算，便于中途取 mod 或者大于某个阈值时直接剪枝退出
```CPP
// (sz)!/(cnt1!..cnt26!) = C(sz,cnt1)*C(sz-cnt1,cnt2)..；对于 k 进行上限截断
long long res = 1;
int rem = sz;
for (auto& x : cnt) {
    if (!x) continue;
    int y = min(x, rem - x);
    for (int i = 0; i < y; ++i) {
        res *= 1LL * (rem - i);
        res /= (i + 1);
        if (res >= k) return k;
    }
    rem -= x;
}
return res;
```


## 生成函数相关

1. 如果出现类似和是一个定制，考虑这样的生成函数：(x^a1 + x^a2 + ... + x^an) ^ k

例1：给定数组 a，求 a 中（可以重复元素）的三元组可以生成的不同元素个数
考虑 F(x) = (x^a1 + x^a2 + ... + x^an) ^ 3，这个多项式的项数即为 ai+aj+ak 的不同取值个数

例2：求 sum_{a1 + a2 + ... + ak == n, a1,..,ak>0} a1a2...ak
考虑 F(x) = (a1 x^a1 + a2 x^a2 + ...)^k，那么 [x^n]F(x) 即为答案。n 代表指数和为 n，k 代表共需要 k 项
计算时使用恒等式 sum ix^i = x / (1-x)^2

2. 【1096G】在偶数 n 个位置上填写数，每个位置能填写的数 d 是一个给定的集合 D，其中 0<=d<=9. 求前 n/2 个位置的数位和与后 n/2 个位置的数位和相等的填方法总数
碰到这种**每个位置独立填选择，最后求的是选择到数之和的，考虑生成函数 F(x)=sum_{d\in D} x^d**
对于这个题，考虑在前 n/2 个位置上填写数，生成函数即为 G(x)=(sum_{d\in D} x^d)^{n/2}；fs=[x^s]G(x) 即为数位和为s的填法数。
对于前 n/2 个位置填了 s，后面一定也是 s，同样有 fs 种填法，总数 fs^2.

因此做法为：构造单位多项式 sum_{d\in D} x^d，再做 n/2 次幂得到生成函数。最后求系数平方和即为答案。细节上注意，G(x) 的次数最多到 n/2*9。
```CPP
void solve() {
	int n, k; cin >> n >> k;
	int m=n>>1, need=9*m+1;
	poly F(need);
	for(int i=0;i<k;++i){
		int x; cin>>x; F[x]=1;
    }
	auto G = poly_pow(F, need, m);
	ll res=0;
	for(auto&x:G)(res+=1LL*x*x%mod)%=mod;
	cout<<res<<endl;
}
```