// floating-point ternary search template (unimodal / convex/concave)
// usage:
//   auto f = [&](ld x) -> ld { ... };               // unimodal on [l, r]
//   ld xopt = ternaryArgmin(l, r, f);               // argmin
//   ld vopt = f(xopt);                              // minimum value

#include <bits/stdc++.h>
using namespace std;

using ld = long double;

// convex find min ∪
template <class F>
ld ternaryArgmin(ld l, ld r, F f) {
    for (int it = 0; it < 200; ++it) {
        ld m1 = l + (r - l) / 3, m2 = r - (r - l) / 3;
        if (f(m1) <= f(m2)) r = m2;
        else l = m1;
    }
    return (l + r) / 2;
}

// concave find max ∩
template <class F>
ld ternaryArgmax(ld l, ld r, F f) {
    for (int it = 0; it < 200; ++it) {
        ld m1 = l + (r - l) / 3, m2 = r - (r - l) / 3;
        if (f(m1) >= f(m2)) r = m2;
        else l = m1;
    }
    return (l + r) / 2;
}