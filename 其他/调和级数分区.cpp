#include <bits/stdc++.h>
using namespace std;

const int V = 2e5;

/**
 * 调和级数分段枚举：按 floor(i / x) 分组
 * 外层枚举值域 x，内层枚举块 [l,r] 满足上整除 x 后得到的值相同
 * 
 * 适用：值域不大时涉及到 floor(a[i]/x) 的快速信息统计
 *
 * 复杂度：时间 O(V log V)
 */
void harmonicFloorBlock() {
    for (int x = 1; x <= V; ++x) {               // 枚举值域/块长 x
        for (int l = 1, r; l <= V; l = r + 1) {  // 枚举块编号 t 
            int t = l / x; r = min(V, (t + 1) * x - 1);
            // 当前区间值域范围 [l, r] = [max(1,t*x), min(V,(t+1)*x-1)]
            // 块内所有值 i 满足 floor(i/x)=t；即固定 x 时区间所有数下整除 x 的结果是相同的
        }
    }
}

/**
 * 调和级数分段枚举：按 ceil(i / x) 分组
 * 外层枚举值域 x，内层枚举块 [l,r] 满足下整除 x 后得到的值相同
 * 
 * 适用：值域不大时涉及到 ceil(a[i]/x) 的快速信息统计
 *
 * 复杂度：时间 O(V log V)
 */
void harmonicCeilBlock() {
    for (int x = 1; x <= V; ++x) {                     // 枚举值域/块长 x
        for (int l = 1, t = 1; l <= V; l += x, ++t) {  // 枚举块编号 t
            int r = min(V, l + x - 1);
            // 当前区间值域范围 [l, r] = [(t-1)*x+1, min(V,t*x)]
            // 块内所有值 i 满足 ceil(i/x)=t；即固定 x 时区间所有数上整除 x 的结果是相同的
        }
    }
}


/**
 * 例题：【2144D】给定 n 个物品对应的价格 {c1,..,cn}，以及一个系数 y。需要选择一个整数 x>1，给每件物品打折：ci->ceil(ci/x)；每个价格牌造价为 y，但是可以复用：
 * 即如果某些 ceil(ci/x) 和原有 c1..cn 的价格牌相同，可以拿来复用；一个原来的价格牌只能复用一次。求所有 x 中的盈利最大值，n,x<=2e5.
 * 
 * 注意到值域较小，应该联想到将复杂度转移为值域相关。这里可以尝试枚举所有 x=2,..,V，并思考在固定 x 时，如何快速得到盈利值，即 sum(ceil(ci/x))-y*cnt(需要新增的价格牌)
 * ceil(ci/x)=t <=> t-1<ci/x<=t <=> x(t-1)<ci<=xt，因此外层枚举 x 时，内层枚举 t，在 t 对应的区间 (x*(t-1),x*t] 中出现的所有 ci 在打折后的价格都是 t。
 * 枚举 t 的时间复杂度是 V/x 的，总体为调和级数复杂度 VlogV。对于区间，可以先把 ci 在位置上置为 1 后求前缀和，用前缀和差来算出区间中的 ci 个数，他们的贡献即为 t*cnt;
 * 再考虑区间有 cnt 个数会变成加个 t，假设原来的 ci 集合中有 p 个 t，那么这部分需要新的价格牌个数即为 max(0,cnt-p).
 */