#include <bits/stdc++.h>
using namespace std;

struct Complex {
    int re, im;
    Complex(int _re = 0, int _im = 0) : re(_re), im(_im) {}
    bool operator==(const Complex& c) const {
        return re == c.re && im == c.im;
    }
    Complex operator+(const Complex& c) const {
        return Complex(re + c.re, im + c.im);
    }
    Complex operator-(const Complex& c) const {
        return Complex(re - c.re, im - c.im);
    }
    Complex operator*(const Complex& c) const {
        return Complex(
            re * c.re - im * c.im,
            re * c.im + im * c.re
        );
    }
};
