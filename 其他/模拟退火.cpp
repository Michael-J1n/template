#include <bits/stdc++.h>
using namespace std;

/**
 * 模拟退火 / Simulated Annealing【卡时随机化搜索近似全局最优解】
 *
 * 状态下标 0-based；默认目标函数越小越优。
 * 模板以【P2503】为例：将 n 个数分为 m 组，使每组和的均方差最小
 *
 * 支持：局部随机扰动、Metropolis 接受、循环降温、卡时重复退火、维护历史最优状态。
 * 适用：状态空间巨大、目标函数存在大量局部最优、精确算法不可行，且局部扰动的目标变化可以快速计算。
 *
 * 时间复杂度：O(KC)，其中 K 为迭代数，C 为单次扰动与增量计算复杂度
 */
pair<vector<int>, double> simulated_anneal(vector<int> cur, int m) {
    const double max_time = 1.9;                                    // 修改：略小于题目允许给 SA 的时间
    const double t_start = 1e4, t_end = 1e-4, cooling = 0.995;      // 修改：初温、末温、降温系数
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> rnd(0.0, 1.0);
    // ===========================================================================================
    double avg = accumulate(cur.begin(), cur.end(), 0.0) / m;
    auto calc = [&](const vector<int>& a) {
        // 修改：当前数组为 a，计算可以获得的最优值；此处按照排列顺序进行贪心放入
        vector<int> sum(m);
        for (int x : a) {
            int p = min_element(sum.begin(), sum.end()) - sum.begin();
            sum[p] += x;
        }
        double res = 0;
        for (int x : sum) res += (x - avg) * (x - avg);
        return res / m;
    };
    // ===========================================================================================
    shuffle(cur.begin(), cur.end(), rng);
    vector<int> best = cur;
    double cur_val = calc(cur), best_val = cur_val;
    clock_t start = clock();
    double t = t_start;
    int iter = 0;
    while (true) {
        if (!(iter++ & 255) && (double)(clock() - start) / CLOCKS_PER_SEC >= max_time) break;
        if (t < t_end) t = t_start;
        // 修改：随机生成一次扰动；此处随机两个不同下标并交换在排列中的位置
        int x = rng() % cur.size(), y = rng() % (cur.size() - 1);
        if (y >= x) ++y;
        swap(cur[x], cur[y]);
        double nxt_val = calc(cur);
        double delta = nxt_val - cur_val;
        if (delta <= 0 || exp(-delta / t) > rnd(rng)) {
            cur_val = nxt_val;
            if (cur_val < best_val) best = cur, best_val = cur_val;
        } else {
            swap(cur[x], cur[y]);
        }
        t *= cooling;
    }
    return {best, sqrt(best_val)};
}