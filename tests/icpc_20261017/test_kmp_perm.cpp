#include <bits/stdc++.h>
using namespace std;
#include "../../字符串/匹配-KMP.cpp"
#include "../../图论/基环树-置换环.cpp"

void check(bool ok, const string& message) {
    if (!ok) { cerr << "FAIL: " << message << '\n'; exit(1); }
}
vector<int> naive_matches(const string& s, const string& p) {
    vector<int> result;
    int n = s.size(), m = p.size();
    for (int i = 0; i <= n - m; ++i)
        if (s.substr(i, m) == p) result.push_back(i);
    return result;
}
void check_kmp(const string& s, const string& p) {
    KMP kmp(p);
    auto matches = naive_matches(s, p);
    check(kmp.find(s) == (matches.empty() ? -1 : matches[0]), "find");
    check(kmp.find_all(s) == matches, "find_all");
    int match_count = matches.size();
    check(kmp.count(s) == match_count, "count");
    int m = p.size();
    vector<int> borders;
    for (int len = 1; len <= m; ++len)
        if (p.substr(0, len) == p.substr(m - len)) borders.push_back(len);
    check(kmp.borders() == borders, "borders");
    int period = 0;
    for (int len = 1; len <= m; ++len) {
        bool ok = true;
        for (int i = len; i < m; ++i) if (p[i] != p[i - len]) ok = false;
        if (ok && m % len == 0) { period = len; break; }
    }
    check(kmp.period() == period, "period for " + p);
    for (int i = 0; i < m; ++i) {
        int expected = 0;
        for (int len = 1; len <= i; ++len)
            if (p.substr(0, len) == p.substr(i + 1 - len, len)) expected = len;
        check(kmp.pi[i] == expected, "pi");
    }
    vector<int> father(40, -9);
    vector<vector<int>> g(40, vector<int>{99});
    for (int rep = 0; rep < 2; ++rep) {
        kmp.fail_tree(father, g);
        int father_size = father.size(), graph_size = g.size();
        check(father_size == m + 1 && graph_size == m + 1, "tree size");
        check(father[0] == 0, "tree root");
        vector<vector<int>> expected(m + 1);
        for (int len = 1; len <= m; ++len) {
            check(father[len] == kmp.pi[len - 1], "tree father");
            expected[father[len]].push_back(len);
        }
        check(g == expected, "tree edges");
    }
}
void check_fail_tree_dp(const string& p) {
    KMP kmp(p);
    int n = p.size();
    vector<int> father, cnt(n + 1), occurrences(n + 1, 1);
    vector<vector<int>> g;
    kmp.fail_tree(father, g);
    for (int len = 1; len <= n; ++len) {
        cnt[len] = cnt[father[len]] + 1;
        int expected = 0;
        for (int k = 1; k <= len; ++k) expected += p.substr(0, k) == p.substr(len - k, k);
        check(cnt[len] == expected, "fail-chain border DP");
    }
    for (int len = n; len > 0; --len) occurrences[father[len]] += occurrences[len];
    for (int len = 1; len <= n; ++len) {
        int expected = naive_matches(p, p.substr(0, len)).size();
        check(occurrences[len] == expected, "fail-tree occurrence propagation");
    }
    for (int u = 1; u <= n; ++u) for (int v = 1; v <= n; ++v) {
        int x = father[u], y = father[v];
        while (x != y) {
            if (x > y) x = father[x];
            else y = father[y];
        }
        int expected = 0;
        for (int len = 1; len < min(u, v); ++len)
            if (p.substr(0, len) == p.substr(u - len, len) && p.substr(0, len) == p.substr(v - len, len)) expected = len;
        check(x == expected, "LCA of parents gives longest common proper border");
    }
}
void check_perm(const vector<int>& p) {
    PermCycles pc(p);
    int n = p.size(), expected_cycles = 0;
    vector<int> seen(n);
    for (int u = 0; u < n; ++u) {
        if (!seen[u]) {
            ++expected_cycles;
            for (int x = u; !seen[x]; x = p[x]) seen[x] = 1;
        }
        vector<int> walk{u};
        for (int x = p[u]; x != u; x = p[x]) walk.push_back(x);
        int len = walk.size();
        check(pc.period(u) == len, "cycle period");
        auto cycle = pc.getCycle(u);
        int cycle_size = cycle.size();
        check(cycle_size == len, "getCycle size");
        for (int i = 0; i < len; ++i) check(p[cycle[i]] == cycle[(i + 1) % len], "cycle order");
        vector<long long> steps{0, 1, len - 1, len, len + 1, 1000000000000LL, LLONG_MAX - 1, LLONG_MAX};
        for (long long step : steps) {
            int r = step % len;
            check(pc.jump(u, step) == walk[r], "jump");
            check(pc.jumpBack(u, step) == walk[(len - r) % len], "jumpBack");
        }
        for (int v = 0; v < n; ++v) {
            int distance = -1;
            for (int i = 0; i < len; ++i) if (walk[i] == v) distance = i;
            check(pc.sameCycle(u, v) == (distance != -1), "sameCycle");
            check(pc.distTo(u, v) == distance, "distTo");
            check(pc.rdistTo(u, v) == (distance == -1 ? -1 : (len - distance) % len), "rdistTo");
        }
    }
    check(pc.cycleCount() == expected_cycles, "cycleCount");
    auto cycles = pc.cyc;
    pc.build();
    check(pc.cyc == cycles, "rebuild cycles");
}
int main(int argc, char** argv) {
    string mode = argc > 1 ? argv[1] : "all";
    if (mode == "empty") { check_kmp("", ""); check_kmp("abc", ""); }
    else if (mode == "empty_all") { check(KMP("").find_all("a") == vector<int>({0, 1}), "empty all"); }
    else if (mode == "empty_period") { check(KMP("").period() == 0, "empty period"); }
    else if (mode == "period") {
        KMP kmp("aba");
        check(kmp.period() == 3, "aba full-repeat period is 3");
        check(kmp.m - kmp.pi.back() == 2, "aba partial period is 2");
    }
    else if (mode == "tree") {
        KMP kmp("a"); vector<int> father; vector<vector<int>> g;
        kmp.fail_tree(father, g); kmp.fail_tree(father, g);
        check(g[0] == vector<int>{1}, "repeated fail_tree duplicates edge");
    } else if (mode == "jump") { check(PermCycles({1, 0}).jump(1, LLONG_MAX) == 0, "large jump"); }
    else {
        vector<string> words{""};
        for (int len = 1; len <= 7; ++len)
            for (int mask = 0; mask < (1 << len); ++mask) {
                string s(len, 'a');
                for (int i = 0; i < len; ++i) if ((mask >> i) & 1) s[i] = 'b';
                words.push_back(s);
            }
        for (const auto& p : words) {
            check_fail_tree_dp(p);
            for (const auto& s : words) check_kmp(s, p);
        }
        check_kmp(string("\0a\0a", 4), string("\0a", 2));
        mt19937 rng(20261017);
        for (int rep = 0; rep < 2000; ++rep) {
            string s(rng() % 80, 'a'), p(rng() % 25, 'a');
            for (char& ch : s) ch += rng() % 4;
            for (char& ch : p) ch += rng() % 4;
            check_kmp(s, p);
            check_fail_tree_dp(p);
        }
        for (int n = 0; n <= 7; ++n) {
            vector<int> p(n); iota(p.begin(), p.end(), 0);
            do { check_perm(p); } while (next_permutation(p.begin(), p.end()));
        }
        for (int rep = 0; rep < 300; ++rep) {
            vector<int> p(rng() % 80); iota(p.begin(), p.end(), 0);
            shuffle(p.begin(), p.end(), rng); check_perm(p);
        }
    }
    cout << "PASS: " << mode << '\n';
}
