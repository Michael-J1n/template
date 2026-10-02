/**
 * 自适应辛普森积分求任意函数的定积分值
 * 缺点：依赖一开始采样的三个点，不能都是0
 */

#include <bits/stdc++.h>
using namespace std;

using ld = long double;
template<class F>
ld simpson(F f, ld l, ld r) {
    ld m = (l + r) / 2;
    return (f(l) + 4 * f(m) + f(r)) * (r - l) / 6;
}
template<class F>
ld asr(F f, ld l, ld r, ld eps, ld s) {
    ld m = (l + r) / 2;
    ld ls = simpson(f, l, m), rs = simpson(f, m, r);
    if (fabsl(ls + rs - s) <= 15 * eps) return ls + rs + (ls + rs - s) / 15;
    return asr(f, l, m, eps / 2, ls) + asr(f, m, r, eps / 2, rs);
}
template<class F>
ld integrate(F f, ld l, ld r, ld eps = 1e-12L) {  // 需要加快计算可把 eps 改小为 1e-8
    return asr(f, l, r, eps, simpson(f, l, r));
}


// 示例用法：传入 lambda 定义的 f
void solve() {
    auto f = [&](ld x) -> ld {
        return sinl(x);
    };
    cout << fixed << setprecision(15) << integrate(f, 0, acosl(-1.0L)) << '\n';
}