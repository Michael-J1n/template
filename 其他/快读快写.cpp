#include <bits/stdc++.h>
using namespace std;

/**
 * FastIO【高速读写】
 *
 * 支持：
 * read(x)：读取整数 / char / string
 * read(a, b, ...)：连续读取多个变量
 * write(x)：输出 x 并换行
 * write(x, end)：输出 x，结尾输出 end
 * pc(c)：输出单个字符
 * flush()：立即刷新输出缓冲区
 */
#include <unistd.h>
struct FastIO {
    static constexpr int S = 1 << 20;
    char ibuf[S], obuf[S];
    int ipos = 0, ilen = 0, opos = 0;
    inline static constexpr char digit[] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";
    ~FastIO() { flush(); }
    inline char gc() {
        if (ipos == ilen) {
            ilen = ::read(0, ibuf, S);
            ipos = 0;
        }
        return ibuf[ipos++];
    }
    template<class T>
    inline void read(T& x) {
        char c = gc();
        while (c != '-' && (c < '0' || c > '9')) c = gc();
        bool neg = false;
        if (c == '-') neg = true, c = gc();
        using U = make_unsigned_t<T>;
        U y = 0;
        while (c >= '0' && c <= '9') {
            y = y * 10 + c - '0';
            c = gc();
        }
        if constexpr (is_signed_v<T>) x = neg ? static_cast<T>(U(0) - y) : static_cast<T>(y);
        else x = static_cast<T>(y);
    }
    inline void read(char& c) {
        c = gc();
        while (c <= ' ') c = gc();
    }
    inline void read(string& s) {
        char c = gc();
        while (c <= ' ') c = gc();
        s.clear();
        while (c > ' ') {
            s.push_back(c);
            c = gc();
        }
    }
    template<class T, class... U>
    inline void read(T& x, U&... y) {
        read(x);
        (read(y), ...);
    }
    inline void flush() {
        if (!opos) return;
        ::write(1, obuf, opos);
        opos = 0;
    }
    inline void pc(char c) {
        if (opos == S) flush();
        obuf[opos++] = c;
    }
    template<class T>
    inline void write(T x, char end = '\n') {
        if (opos > S - 64) flush();
        using U = make_unsigned_t<T>;
        U y;
        if constexpr (is_signed_v<T>) {
            if (x < 0) {
                obuf[opos++] = '-';
                y = U(0) - U(x);
            } else {
                y = U(x);
            }
        } else {
            y = x;
        }
        char s[32];
        int p = 32;
        while (y >= 100) {
            int r = y % 100;
            y /= 100;
            s[--p] = digit[r * 2 + 1];
            s[--p] = digit[r * 2];
        }
        if (y < 10) {
            s[--p] = '0' + y;
        } else {
            s[--p] = digit[y * 2 + 1];
            s[--p] = digit[y * 2];
        }
        int len = 32 - p;
        memcpy(obuf + opos, s + p, len);
        opos += len;
        if (end) obuf[opos++] = end;
    }
    inline void write(char c, char end = '\n') {
        pc(c);
        if (end) pc(end);
    }
    inline void write(const string& s, char end = '\n') {
        for (char c : s) pc(c);
        if (end) pc(end);
    }
} io;