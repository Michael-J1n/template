# ICPC targeted audit plan

Goal: verify the eight selected template files at remote main 6abee917a184809fa0b1e1c4358f1b6a923f46c3, not the user's local templates.
Architecture: isolated snapshot and branch; independent brute-force oracles; retain public interfaces except the explicitly requested Fibonacci signature. Publish only selected changes on a new remote branch, preserving the full base tree.
Tech stack: C++17-compatible templates, GCC 13, Python 3, UBSan/ASan.
Spec: user request of 2026-10-17 in this session.

Constraints: 0-based indexes, closed intervals, 1-based k/rank. No mass style rewrite. New code uses snake_case, int with long long for arithmetic, no integer macros or __int128. No changes to main or existing work.

Review focus: extreme jump counts; repeated builders; empty query ranges; signed integer extremes; deep recursion.

- [x] Fibonacci/ST2D: test old behavior and requested interfaces; use recurrence/matrix oracle and rectangle scans; implement iterative int fib(long long n) with global const int mod; remove at wrappers preserving clipping; verify n=0, LLONG_MAX, mod=1 and INT_MAX, empty rectangles and custom operators.
- [x] KMP/Fail Tree and permutation: brute substring, border and cycle walking tests; isolate repeat-build, empty and overflow failures; minimal fixes and documented contracts.
- [x] HLD point/edge variants: compare expanded path/subtree intervals to parent-walk oracle for random roots; probe long chain stack risk separately.
- [x] Both Wavelet Matrix variants: compare every operation to sorted slices and updates to plain arrays; compile concrete instantiations, test duplicates, extrema and empty intervals.
- [x] Run all tests with sanitizers and optimized builds; review changed lines and record limits in one audit report.
- [ ] Recheck remote main, create a unique branch from the fixed base, commit only verified files using the base tree, create PR without merging.
