#include <bits/stdc++.h>
using namespace std;

/**
 * AC 自动机 / Aho-Corasick Automaton【多模式串匹配】
 * 字符集默认为 'a' ~ 'z'；Trie 节点 0-based，0 为根
 *
 * 节点信息：
 * Trie 中边代表走一个字符，节点代表从根开始到这里边上所有字符顺序链接形成的前缀字符串
 * 节点 u 代表从根到 u 的整条路径字符串；除根外，进入 u 的那条边对应一个字符。节点在树中本身的数字编号只和插入顺序有关
 * ch[id]：节点编号；build 前表示字符 BASE + id 对应的 Trie 儿子，build 后表示当前 AC 状态读入字符 BASE + id 后转移到的节点
 *         即：有这个孩子就向下跳，否则不断沿着 fail 链侧跳直到有对应的孩子，再朝孩子向下跳得到的结果。在 build 中已经处理；因此 t[u].ch[id] 直接指向结果节点（若都匹配不上则是根节点）
 * fail：返回当前节点对应字符串的最长真后缀中，同时也是 Trie 中某个模式串前缀的最长字符串所对应的树中节点
 *       即：fail[u] 表示：如果当前这个前缀太长、接下来走不下去了，那它的哪个最长后缀还能继续作为某个模式串的前缀？
 * cnt：恰好在当前节点结束的模式串数量
 * out：当前状态下匹配结束的模式串总数，即当前节点及其 fail 链上的 cnt 之和
 *
 * 支持操作：
 * insert(s)：插入模式串 s，返回 s 对应的终止节点编号
 * build()：构建 fail 指针，补全所有自动机转移，并维护 out
 * step(u, c)：当前 AC 状态（节点编号为）u，尾部接入字符 c 后的新状态（对应的节点编号）
 * match(s)：查询文本 s 中所有模式串的出现次数之和
 * count_each(s)：返回每个模式串在文本 s 中的出现次数
 *
 * 适用：多模式串匹配、关键词查找、多个模式串同时匹配同一文本
 * 关键过程：init u = 0; for (c : s) u -> step(u, c)；形象理解：我一边扫文本，一边始终记住：当前文本结尾，最长还能和某个模式串前缀对上的那一段是什么。
 *
 * 复杂度：设 L 为所有模式串总长度，V 为 Trie 节点数，Σ 为字符集大小，T 为文本长度
 * insert：O(|s|)，所有模式串共 O(L)；build：O(VΣ)；match：O(T)
 * 空间：O(VΣ)
 */
struct AC {
    static constexpr int SIGMA = 26;
    static constexpr char BASE = 'a';
    struct Node {
        // build 后 ch[u][id] 指向沿 fail 链寻找后，第一个能够通过字符 id 转移到的状态；若始终不存在则为根。
        array<int, SIGMA> ch{};
        // fail：匹配此节点代表的某模式串前缀的最长后缀使得存在模式串的前缀等于此后缀，返回此前缀的节点编号；cnt：以此节点结尾的模式串个数；out：满足本身与当前串后缀相等的模式串的总数
        int fail = 0, cnt = 0, out = 0;
    };
    vector<Node> t = {Node()};  // 树根为节点 0
    vector<int> pos, ord;       // pos[i]：第 i 个模式串对应的树节点编号；ord：按照 build 时的 BFS 顺序的数组
    int insert(const string& s) {
        int u = 0;
        for (char c : s) {
            int id = c - BASE;
            if (!t[u].ch[id]) t[u].ch[id] = t.size(), t.push_back(Node());
            u = t[u].ch[id];
        }
        ++t[u].cnt;
        pos.push_back(u);
        return u;
    }
    void build() {
        queue<int> q;
        for (int id = 0; id < SIGMA; ++id) {
            int v = t[0].ch[id];
            if (v) t[v].out = t[v].cnt, q.push(v);
        }
        // BFS 为每个点建立 fail 边
        while (!q.empty()) {
            int u = q.front(); q.pop();
            ord.push_back(u);
            for (int id = 0; id < SIGMA; ++id) {
                int& v = t[u].ch[id];
                if (!v) {
                    // 没有 id 作为 Trie 中儿子，t[u].ch[id] 为不断沿着 fail 链找到有这个孩子为止
                    v = t[t[u].fail].ch[id];
                    continue;
                }
                // 否则有这个 Trie 儿子，递归式的计算它的 fail 与 out，并加入 BFS 队列
                t[v].fail = t[t[u].fail].ch[id];
                t[v].out = t[v].cnt + t[t[v].fail].out;
                q.push(v);
            }
        }
    }
    int step(int u, char c) { return t[u].ch[c - BASE]; }
    /** 使用下列方法之前必须把所有模式串进行 insert，并进行 build */
    long long match(const string& s) {
        int u = 0;
        long long res = 0;
        for (char c : s) {
            u = step(u, c);
            res += t[u].out;
        }
        return res;
    }
    vector<long long> count_each(const string& s) {
        vector<long long> occ(t.size());
        int u = 0;
        // 计算每个模式串出现次数：每次只记录当前最长模式串前缀作为后缀，然后用反拓扑序传播累加更小的
        for (char c : s) {
            u = step(u, c);
            ++occ[u];
        }
        for (int i = ord.size() - 1; ~i; --i) {
            int u = ord[i];
            occ[t[u].fail] += occ[u];
        }
        vector<long long> res(pos.size());
        for (int i = 0; i < pos.size(); ++i) res[i] = occ[pos[i]];
        return res;
    }
    void walk_fail(int u) {
        for (int v = u; v; v = t[v].fail) {
            // 处理 v；v 依次为 u, fail[u], fail[fail[u]], ...
        }
    }
};