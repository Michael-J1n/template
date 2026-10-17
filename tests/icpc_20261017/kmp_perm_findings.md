# KMP / PermCycles audit (2026-10-17)

Scope: `字符串/匹配-KMP.cpp` and `图论/基环树-置换环.cpp`, snapshot `6abee917`. Public APIs retained. Test source: `tests/icpc_20261017/test_kmp_perm.cpp`.

## Confirmed defects and minimal fixes

- Empty KMP pattern: `KMP("").find("")` returned -1; `find_all("a")` accessed `pi[-1]`; `period()` accessed `pi[-1]`. Explicitly define empty matches at all text gaps: find=0, find_all=0..n, count=n+1, period=0. Added early branches. Empty borders remains empty.
- Rebuilding the failure tree: call `KMP("a").fail_tree(father,g)` twice; `g[0]` became `{1,1}`. `resize` preserved existing adjacency lists and stale root parent. Use `assign` to reset both output containers, including when caller supplies dirty buffers.
- `PermCycles({1,0}).jump(1, LLONG_MAX)`: signed overflow in `1 + LLONG_MAX`. Reduce t modulo cycle length before addition; addition then remains in long long and below twice INT_MAX.
- `distTo` / `rdistTo`: the difference plus cycle length previously evaluated entirely in int. For example L=1,500,000,000, source position=0, destination=1,499,999,999 overflows. Promote expression to long long. This huge-memory case is an arithmetic proof, not an allocated end-to-end runtime test; ordinary and extreme-step differential tests exercise both methods.

## Documentation / preconditions, not behavioral bugs

- `period()` already computes the shortest **complete repetition block**, whereas its opening comment said minimum period. Preserve existing behavior and clarify that comment. `"aba"` returns 3; its general partial-block period is `m-pi[m-1]=2`, as correctly explained in the bottom documentation. An initial regression expecting 2 exposed this ambiguity; that expectation was withdrawn after API review, not counted as a corrected algorithm defect.
- PermCycles input must be a valid 0-based permutation; vertex arguments must be in range. No vertex query exists for n=0; empty construction/cycleCount/rebuild are tested. Default construction requires initializing n and p consistently before build; direct public-field mutation must preserve their consistency.
- Both jump methods require t >= 0, now stated in the API comments. Negative steps and invalid permutations are outside their model.
- KMP indexes and return counts are int; documented lengths < INT_MAX ensure empty-pattern n+1 fits. Memory exhaustion and lengths beyond the declared model are not tested.
- No API rename, macro, optional, __int128, broad refactor or unrelated template changes.

## Commands and observed evidence

Run from repository root:

```sh
g++ -std=c++17 -O1 -g -fsanitize=undefined -fno-sanitize-recover=all tests/icpc_20261017/test_kmp_perm.cpp -o /tmp/test_kmp_perm_baseline
for mode in empty empty_all empty_period period tree jump; do /tmp/test_kmp_perm_baseline "$mode"; done
```

This ran **before production edits**, using the initial partial-period assertion. Outputs in mode order:

1. `FAIL: find`
2. UBSan: applying non-zero offset 18446744073709551612 to null pointer (vector negative index)
3. Same UBSan invalid vector access
4. `FAIL: aba period must be 2` — documentation ambiguity; subsequently corrected the test to retain full-repeat semantics
5. `FAIL: repeated fail_tree duplicates edge`
6. UBSan: signed integer overflow: 1 + 9223372036854775807

Final verification:

```sh
g++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-sanitize-recover=all tests/icpc_20261017/test_kmp_perm.cpp -o /tmp/test_kmp_perm_fixed
ASAN_OPTIONS=detect_leaks=0 /tmp/test_kmp_perm_fixed
```

The initial run without ASAN_OPTIONS encountered an environment-only LeakSanitizer fatal error (`Can't open /proc/2/task`, ptrace unsupported). Disable only leak detection; address and undefined-behavior instrumentation remain active.

Coverage: all 65,025 binary text/pattern pairs with lengths 0..7; 2,000 fixed-seed random pairs; embedded-NUL matching; prefix-function/brute border/full-repetition comparisons; dirty and repeatedly rebuilt failure-tree buffers. All 5,914 permutations for n=0..7 plus 300 seeded random permutations up to 79 vertices; independent traversal oracle for cycle structure, forward/reverse distances, jumps, repeated build. Steps include 0,1,L-1,L,L+1,10^12,LLONG_MAX-1,LLONG_MAX. Seed 20261017. No pre-existing repository-wide runner was found; parent audit coordinates other templates' test suites.

Final result: `PASS: all`, exit status 0, with no ASan/UBSan diagnostics (leak detection disabled for the container limitation above).
