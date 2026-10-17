#include "../../数据结构/小波矩阵.cpp"
#include "../../数据结构/小波矩阵-带修.cpp"

void check(bool ok, const char* name, int l, int r) {
    if (!ok) {
        cerr << "FAIL " << name << " [" << l << ',' << r << "]\n";
        exit(1);
    }
}

template<class W, class T>
void verify(const W& w, const vector<T>& a, int l, int r, const vector<T>& probes) {
    vector<T> b;
    for (int i = l; i <= r; ++i) b.push_back(a[i]);
    sort(b.begin(), b.end());
    int len = b.size();
    for (int k = 1; k <= len; ++k) check(w.kth(l, r, k) == b[k - 1], "kth", l, r);
    for (T x : probes) {
        int less = lower_bound(b.begin(), b.end(), x) - b.begin();
        int leq = upper_bound(b.begin(), b.end(), x) - b.begin();
        check(w.count_less(l, r, x) == less, "count_less", l, r);
        check(w.count_leq(l, r, x) == leq, "count_leq", l, r);
        check(w.count(l, r, x) == leq - less, "count", l, r);
        check(w.rank(l, r, x) == less + 1, "rank", l, r);
        check(w.prev_value(l, r, x) == (less ? b[less - 1] : numeric_limits<T>::lowest()), "prev", l, r);
        check(w.next_value(l, r, x) == (less < len ? b[less] : numeric_limits<T>::max()), "next", l, r);
        for (T y : probes) {
            int expected = 0;
            for (T z : b) expected += x <= z && z <= y;
            check(w.range_freq(l, r, x, y) == expected, "range_freq", l, r);
        }
    }
}

template<class T>
void run(vector<T> probes) {
    mt19937 rng(20261017);
    int pn = probes.size();
    for (int tc = 0; tc < 120; ++tc) {
        int n = tc < 18 ? tc : rng() % 40;
        vector<T> a(n);
        for (T& x : a) x = probes[rng() % pn];
        WaveletMatrix<T> s(a);
        DynamicWaveletMatrix<T> d(a, probes);
        for (int l = 0; l <= n; ++l) {
            verify(s, a, l, l - 1, probes);
            verify(d, a, l, l - 1, probes);
        }
        if (n >= 2) {
            verify(s, a, n - 1, 0, probes);
            verify(d, a, n - 1, 0, probes);
        }
        for (int step = 0; step < 120 && n; ++step) {
            int l = rng() % n, r = rng() % n;
            if (l > r) swap(l, r);
            verify(s, a, l, r, probes);
            verify(d, a, l, r, probes);
            int p = rng() % n;
            a[p] = probes[rng() % pn];
            d.update(p, a[p]);
            s.build(a);
            verify(d, a, p, p, probes);
        }
        s.build({});
        d.build({}, probes);
        verify(s, vector<T>{}, 0, -1, probes);
        verify(d, vector<T>{}, 0, -1, probes);
    }
}

void check_cardinalities() {
    for (int m : {0, 1, 2, 3, 4, 7, 8, 9, 15, 16, 17}) {
        vector<int> a(m), probes{-100, 100};
        iota(a.begin(), a.end(), -m / 2);
        probes.insert(probes.end(), a.begin(), a.end());
        WaveletMatrix<int> s(a);
        DynamicWaveletMatrix<int> d(a);
        verify(s, a, 0, m - 1, probes);
        verify(d, a, 0, m - 1, probes);
        for (int p = 0; p < m; ++p) {
            a[p] = m / 2 - (m > 0 && m % 2 == 0);
            d.update(p, a[p]);
            verify(d, a, 0, m - 1, probes);
        }
    }
}
int main() {
    check_cardinalities();
    run<int>({INT_MIN, INT_MIN + 1, -7, -1, 0, 1, 7, INT_MAX - 1, INT_MAX});
    run<long long>({LLONG_MIN, LLONG_MIN + 1, -7, -1, 0, 1, 7, LLONG_MAX - 1, LLONG_MAX});
    cout << "wavelet: PASS seed=20261017, int/long long, 240 cases, 120 update rounds per nonempty case\n";
}
