#include <bits/stdc++.h>
using namespace std;

const int N = 10010;

vector<double> ys;
int find(double y) {
    return lower_bound(ys.begin(), ys.end(), y) - ys.begin();
}

struct Segment {
    double x, y1, y2;
    int k;
    bool operator<(const Segment& s) {
        return x < s.x;
    }
} seg[N * 2];

struct Node {
    int l, r;
    int cnt;
    double len;
} tr[N * 8];

void pushup(int u) {
    if (tr[u].cnt) tr[u].len = ys[tr[u].r + 1] - ys[tr[u].l];
    else if (tr[u].l != tr[u].r) tr[u].len = tr[u << 1].len + tr[u << 1 | 1].len;
    else tr[u].len = 0;
}

void build(int u, int l, int r) {
    tr[u] = {l, r, 0, 0};
    if (l != r) {
        int mid = l + r >> 1;
        build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
    }
}

void modify(int u, int l, int r, int k) {
    if (l <= tr[u].l && tr[u].r <= r) tr[u].cnt += k;
    else {
        int mid = tr[u].l + tr[u].r >> 1;
        if (l <= mid) modify(u << 1, l, r, k);
        if (r > mid) modify(u << 1 | 1, l, r, k);
    }
    pushup(u);
}

int n, T = 1;
int main() {
    while (cin >> n, n) {
        ys.clear();
        for (int i = 0, j = 0; i < n; ++i) {
            double x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            ys.emplace_back(y1), ys.emplace_back(y2);
            seg[j++] = {x1, y1, y2, 1}, seg[j++] = {x2, y1, y2, -1};
        }
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        sort(seg, seg + n * 2);

        build(1, 0, ys.size() - 2);
        double res = 0;
        for (int i = 0; i < n * 2; ++i) {
            if (i > 0) res += tr[1].len * (seg[i].x - seg[i - 1].x);
            modify(1, find(seg[i].y1), find(seg[i].y2) - 1, seg[i].k);
        }
        cout << "Test case #" << T++ << endl;
        cout << "Total explored area: " << fixed << setprecision(2) << res << endl << endl;
    }
    return 0;
}