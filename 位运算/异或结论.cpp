/**
 * 结论：f(n) = 1^2^...^n 有mod4周期性 (n,1,n+1,0)
 */

// O(1) 求 1 到 n 异或和
int f(int n) {
    switch (n % 4) {
        case 0: return n;
        case 1: return 1;
        case 2: return n + 1;
        default: return 0;
    }
}