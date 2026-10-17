#include <bits/stdc++.h>
namespace point_weight {
#include "../../树论/重链剖分-点权.cpp"
}
namespace edge_weight {
#include "../../树论/重链剖分-边权.cpp"
}
using namespace std;

long long tree_count = 0, path_count = 0, subtree_count = 0;
void require(bool condition, const string& message) {
    if (!condition) throw runtime_error(message);
}

vector<int> unpack(const vector<pair<int, int>>& ranges, const vector<int>& rev) {
    vector<int> result;
    int n = rev.size();
    for (auto [left, right] : ranges) {
        require(0 <= left && left <= right && right < n, "invalid closed path range");
        for (int i = left; i <= right; ++i) result.push_back(rev[i]);
    }
    sort(result.begin(), result.end());
    require(adjacent_find(result.begin(), result.end()) == result.end(), "duplicate path node/edge");
    return result;
}

void check_tree(vector<vector<int>> graph, int root) {
    int n = graph.size();
    point_weight::HLD points(graph, root);
    edge_weight::HLD edges(graph, root);
    vector<int> parent(n, -2), depth(n), order{root};
    parent[root] = -1;
    for (size_t i = 0; i < order.size(); ++i) {
        int u = order[i];
        for (int v : graph[u]) if (parent[v] == -2) {
            parent[v] = u;
            depth[v] = depth[u] + 1;
            order.push_back(v);
        }
    }
    int order_size = order.size();
    require(order_size == n, "test input disconnected");
    require(points.fa == parent && edges.fa == parent, "parent mismatch");
    require(points.dep == depth && edges.dep == depth, "depth mismatch");
    vector<int> weights(n);
    for (int u = 0; u < n; ++u) {
        weights[u] = u % 3 == 0 ? INT_MIN : u % 3 == 1 ? INT_MAX : -u;
        require(points.rev[points.dfn[u]] == u && edges.rev[edges.dfn[u]] == u, "inverse dfn mismatch");
    }
    auto point_seq = points.toSeq(weights), edge_seq = edges.toSeq(weights);
    for (int u = 0; u < n; ++u) {
        require(point_seq[points.dfn[u]] == weights[u], "point toSeq mismatch");
        require(edge_seq[edges.dfn[u]] == weights[u], "edge toSeq mismatch");
        vector<int> point_subtree, edge_subtree;
        for (int v = 0; v < n; ++v) {
            int ancestor = v;
            while (ancestor != -1 && ancestor != u) ancestor = parent[ancestor];
            if (ancestor == u) {
                point_subtree.push_back(v);
                if (v != u) edge_subtree.push_back(v);
            }
        }
        auto point_range = points.subtree(u), edge_range = edges.subtree(u);
        require(unpack({point_range}, points.rev) == point_subtree, "point subtree mismatch");
        if (edge_subtree.empty()) require(edge_range.first == edge_range.second + 1, "leaf empty range mismatch");
        else require(unpack({edge_range}, edges.rev) == edge_subtree, "edge subtree mismatch");
        int subtree_size = point_subtree.size();
        require(points.sz[u] == subtree_size && edges.sz[u] == subtree_size, "size mismatch");
        subtree_count += 2;
        for (int v = 0; v < n; ++v) {
            vector<int> expected_points, expected_edges;
            int x = u, y = v;
            while (x != y) {
                if (depth[x] < depth[y]) swap(x, y);
                expected_points.push_back(x);
                expected_edges.push_back(x);
                x = parent[x];
            }
            expected_points.push_back(x);
            sort(expected_points.begin(), expected_points.end());
            sort(expected_edges.begin(), expected_edges.end());
            require(unpack(points.path(u, v), points.rev) == expected_points, "point path mismatch");
            require(unpack(edges.path(u, v), edges.rev) == expected_edges, "edge path mismatch");
            if (u == v) require(edges.path(u, v).empty(), "same-node edge path nonempty");
            path_count += 2;
        }
    }
    ++tree_count;
}

int main(int argc, char** argv) {
    if (argc == 4 && string(argv[1]) == "chain") {
        int n = stoi(argv[2]);
        vector<vector<int>> graph(n);
        for (int u = 1; u < n; ++u) {
            graph[u - 1].push_back(u);
            graph[u].push_back(u - 1);
        }
        if (string(argv[3]) == "point") {
            point_weight::HLD hld(graph);
            require(hld.sz[0] == n, "chain size mismatch");
        } else {
            edge_weight::HLD hld(graph);
            require(hld.sz[0] == n, "chain size mismatch");
        }
        cout << "PASS chain " << n << ' ' << argv[3] << '\n';
        return 0;
    }
    mt19937 random(20261017);
    for (int n = 1; n <= 40; ++n) {
        for (int shape = 0; shape < 3; ++shape) {
            vector<vector<int>> graph(n);
            for (int u = 1; u < n; ++u) {
                int parent = shape == 0 ? u - 1 : shape == 1 ? 0 : (u - 1) / 2;
                graph[u].push_back(parent);
                graph[parent].push_back(u);
            }
            for (int root = 0; root < n; ++root) check_tree(graph, root);
        }
    }
    for (int iteration = 0; iteration < 1000; ++iteration) {
        int n = uniform_int_distribution<int>(1, 40)(random);
        vector<vector<int>> graph(n);
        for (int u = 1; u < n; ++u) {
            int parent = uniform_int_distribution<int>(0, u - 1)(random);
            graph[u].push_back(parent);
            graph[parent].push_back(u);
        }
        for (auto& neighbors : graph) shuffle(neighbors.begin(), neighbors.end(), random);
        check_tree(graph, uniform_int_distribution<int>(0, n - 1)(random));
    }
    cout << "PASS trees=" << tree_count << " path_checks=" << path_count
         << " subtree_checks=" << subtree_count << " seed=20261017\n";
}
