#include <bits/stdc++.h>
using namespace std;

/**
 * 字典树，multiset 语义，默认处理小写字母 'a' ~ 'z'，即为一个 26 叉树。
 * 若字符集不连续，无法使用 SIGMA+BASE 的方法，需要提前手写映射 int->char, char->int。
 * 
 * 功能（假设 L 为单词长度）：
 * 1. insert(word) 插入单词，O(L)
 * 2. erase(word) 删除单词（的一次出现），O(L)
 * 3. search(word) 判断单词是否存在，O(L)
 * 4. starts_with(prefix) 判断前缀是否存在，O(L)
 * 5. query(prefix) 统计拥有此前缀的单词数量，O(L)
 * 6. kth(k) 返回字典序第 k 小 (1-based index) 单词，O(L)
 * 7. get_prefix_node(prefix) 返回前缀 prefix 对应的节点编号，不存在返回 -1，O(L)
 * 8. get_all_words() 获取当前树中所有单词，O(∑L)
 * 9. dfs_chain() 沿链做字典树 DFS，O(L)
 * 10. dfs_all() 遍历整颗字典树 DFS，O(∑L)
 * 11. clear(), empty(), size() 与 multiset 语义一样
 *
 * Trie 等价于 BasicTrie<26, 'a'>；其他连续字符集可使用 BasicTrie<SIGMA, BASE>。
 * 前置条件：字符均位于 [BASE, BASE+SIGMA)；erase 只删除存在的单词；kth 时 1<=k<=size()
 * 编译要求：dfs_chain / dfs_all 使用显式对象参数 lambda，需要 C++23
 */
template<int SIGMA = 26, char BASE = 'a'>
struct BasicTrie {
    struct Node {
        array<int, SIGMA> ch;
        // cnt: 多少个单词经过此节点；word_cnt：多少个单次恰好结束在此节点。
        int cnt, word_cnt;
        Node() { ch.fill(-1); cnt = word_cnt = 0; }
    };
    vector<Node> t;
    vector<int> free_nodes;
    BasicTrie(int cap = 0) {
        if (cap) t.reserve(cap + 1);
        t.emplace_back();
    }
    void reserve_nodes(int cap) { t.reserve(cap + 1); }
    int new_node() {
        if (!free_nodes.empty()) {
            int u = free_nodes.back();
            free_nodes.pop_back();
            t[u] = Node();
            return u;
        }
        t.emplace_back();
        return t.size() - 1;
    }
    void recycle_subtree(int root) {
        vector<int> stk{root};
        while (!stk.empty()) {
            int u = stk.back();
            stk.pop_back();
            for (int v : t[u].ch) if (v != -1) stk.emplace_back(v);
            free_nodes.emplace_back(u);
        }
    }
    void insert(const string& word) {
        int u = 0;
        ++t[u].cnt;
        for (auto& c : word) {
            int id = c - BASE;
            if (t[u].ch[id] == -1) t[u].ch[id] = new_node();
            u = t[u].ch[id];
            ++t[u].cnt;
        }
        ++t[u].word_cnt;
    }
    bool erase(const string& word) {
        // if (!search(word)) return false;  // 若不保证存在，将此行解除注释
        int u = 0;
        vector<int> path{0}, edge;
        --t[u].cnt;
        for (auto& c : word) {
            int id = c - BASE;
            u = t[u].ch[id];
            --t[u].cnt;
            edge.emplace_back(id);
            path.emplace_back(u);
        }
        --t[u].word_cnt;
        for (int i = 1; i < path.size(); ++i) {
            if (t[path[i]].cnt) continue;
            t[path[i - 1]].ch[edge[i - 1]] = -1;
            recycle_subtree(path[i]);
            break;
        }
        return true;
    }
    bool search(const string& word) {
        int u = get_prefix_node(word);
        return u != -1 && t[u].word_cnt > 0;
    }
    bool starts_with(const string& prefix) { return query(prefix) > 0; }
    int query(const string& prefix) {
        int u = get_prefix_node(prefix);
        return u == -1 ? 0 : t[u].cnt;
    }
    string kth(int k) {
        if (k <= 0 || k > t[0].cnt) return "";
        string res;
        int u = 0;
        while (true) {
            if (t[u].word_cnt >= k) return res;
            k -= t[u].word_cnt;
            for (int i = 0; i < SIGMA; ++i) {
                int v = t[u].ch[i];
                if (v == -1) continue;
                if (t[v].cnt < k) k -= t[v].cnt;
                else {
                    res += char(BASE + i);
                    u = v;
                    break;
                }
            }
        }
    }
    int get_root() { return 0; }
    int get_prefix_node(const string& s) {
        int u = 0;
        for (auto& c : s) {
            int id = c - BASE;
            if (t[u].ch[id] == -1) return -1;
            u = t[u].ch[id];
        }
        return u;
    }
    vector<string> get_all_words() {
        vector<string> res;
        string cur;
        auto dfs = [&](auto&& dfs, int u) -> void {
            for (int i = 0; i < t[u].word_cnt; ++i) res.emplace_back(cur);
            for (int i = 0; i < SIGMA; ++i) {
                int v = t[u].ch[i];
                if (v == -1 || !t[v].cnt) continue;
                cur += char(BASE + i);
                dfs(dfs, v);
                cur.pop_back();
            }
        }; dfs(dfs, 0);
        return res;
    }
    // 沿着指定字符串 s 对应的 Trie 链向下走
    void dfs_chain(const string& s) {
        string path;
        // 进入 dfs(u, dep) 时，u 表示字符串 s[0..dep-1] 对应的 Trie 节点，并且 path == s.substr(0, dep)
        auto dfs = [&](auto&& dfs, int u, int dep) -> void {
            if (dep == s.size()) {
                // path == s，整个 s 已经走完
                return;
            }
            int id = s[dep] - BASE;
            int v = t[u].ch[id];
            if (v == -1 || !t[v].cnt) return;
            path += s[dep];

            // 当前 path 是 s 的一个前缀，v 是这个前缀对应的 Trie 节点
            // if (t[v].word_cnt) {}

            dfs(dfs, v, dep + 1); path.pop_back();
        }; dfs(dfs, 0, 0);
    }
    // 在整颗 Trie 上进行树上 DFS
    void dfs_all() {
        string path;
        // 进入 dfs(u) 时，path 恰好就是 Trie 节点 u 所代表的字符串
        auto dfs = [&](auto&& dfs, int u) -> void {
            // t[u].cnt = 有多少单词经过这个节点；t[u].word_cnt = 有多少单词刚好等于 path
            if (t[u].word_cnt) {
                // path 是一个存在的单词
                // multiset 语义下，出现次数是 t[u].word_cnt
            }
            for (int i = 0; i < SIGMA; ++i) {
                int v = t[u].ch[i];
                if (v == -1 || !t[v].cnt) continue;
                path += char(BASE + i);
                dfs(dfs, v);
                path.pop_back();
            }
        }; dfs(dfs, 0);
    }
    void clear() {
        t.clear();
        free_nodes.clear();
        t.emplace_back();
    }
    bool empty() const { return !t[0].cnt; }
    int size() const { return t[0].cnt; }
}; using Trie = BasicTrie<>;
