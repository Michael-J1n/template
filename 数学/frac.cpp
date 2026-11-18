#include <bits/stdc++.h>
using namespace std;

struct Frac {
    long long x, y, sgn;
    Frac(long long _x = 0, long long _y = 1) {
        if (!_x) x = 0, y = 1, sgn = 0;
        else {
            unsigned long long ax = _x < 0 ? 0ULL - _x : _x;
            unsigned long long ay = _y < 0 ? 0ULL - _y : _y;
            unsigned long long g = gcd(ax, ay);
            if (g > LLONG_MAX) x = y = 1;
            else x = _x / (long long)g, y = _y / (long long)g;
            long long sx = (x > 0) - (x < 0), sy = (y > 0) - (y < 0);
            sgn = sx * sy;
        }
    }
    Frac operator+() const { return *this; }
    Frac operator-() const { return Frac(-x, y); }
    Frac operator+(const Frac& o) const { return Frac(x * o.y + y * o.x, y * o.y); }
    Frac operator-(const Frac& o) const { return Frac(x * o.y - y * o.x, y * o.y); }
    Frac operator*(const Frac& o) const { return Frac(x * o.x, y * o.y); }
    Frac operator/(const Frac& o) const { return Frac(x * o.y, y * o.x); }
    Frac& operator+=(const Frac& o) { return *this = *this + o; }
    Frac& operator-=(const Frac& o) { return *this = *this - o; }
    Frac& operator*=(const Frac& o) { return *this = *this * o; }
    Frac& operator/=(const Frac& o) { return *this = *this / o; }
    bool operator==(const Frac& o) const { return x * o.y == y * o.x; }
    bool operator!=(const Frac& o) const { return !(*this == o); }
    bool operator<(const Frac& o) const {
        if (sgn != o.sgn) return sgn < o.sgn;
        if ((y > 0) == (o.y > 0)) return x * o.y < y * o.x;
        return x * o.y > y * o.x;
    }
    bool operator>(const Frac& o) const { return o < *this; }
    bool operator<=(const Frac& o) const { return !(o < *this); }
    bool operator>=(const Frac& o) const { return !(*this < o); }
};
