#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> get(int n, vector<vector<int>>& edges) {
    vector<vector<int>> g(n);
    for (auto& e : edges) {
        int u = e[0], v = e[1];
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    return g;
}

vector<vector<pair<int, int>>> get(int n, vector<vector<int>>& edges) {
    vector<vector<pair<int, int>>> g(n);
    for (auto& e : edges) {
        int u = e[0], v = e[1], w = e[2];
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    return g;
}