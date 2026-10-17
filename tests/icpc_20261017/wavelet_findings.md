# Wavelet audit (2026-10-17)

Scope: `数据结构/小波矩阵.cpp` and `数据结构/小波矩阵-带修.cpp`.

## Confirmed defects and minimal fixes

1. Dynamic constructor/build does not compile when instantiated: unqualified `count(bits.begin(), bits.end(), 0)` resolves to the class's `count(int,int,T)` member. Qualified this single call as `std::count`. Baseline test compilation failed with `cannot convert std::vector<int>::iterator to int` for both int and long long.
2. Both `range_freq` implementations can return negative counts for reversed value bounds. Minimal repro: array `{0}`, `range_freq(0,0,1,-1)` returned -1; now 0. Added `low > high` guard. After fixing compilation, the original test failed `FAIL range_freq [0,0]`.
3. Both `count_id_less` implementations mishandle non-adjacent empty ranges. Minimal repro: array `{0,1,2}`, `count_leq(2,0,2)` returned -1; now 0. Added `l > r` early return. Before this change the test failed `FAIL range_freq [2,0]`. This also ensures empty-range rank=1 and predecessor/successor sentinels.

Documented existing strict predecessor `<x`, inclusive successor `>=x`, and valid-k requirement. No algorithm rewrite or public API changes.

## Reproduction commands (repository root)

```sh
g++ -std=c++17 -O2 -Wall -Wextra tests/icpc_20261017/test_wavelet.cpp -o /tmp/test_wavelet
/tmp/test_wavelet
g++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer tests/icpc_20261017/test_wavelet.cpp -o /tmp/test_wavelet_san
ASAN_OPTIONS=detect_leaks=0 /tmp/test_wavelet_san
```

Seed 20261017. Brute oracle sorts the actual subarray, compares every valid k, counts, rank, all probe-pair frequency intervals, strict predecessor, inclusive successor. 240 cases across int and long long, n=0..39, up to 120 point updates per nonempty case, rebuilds, duplicate/extreme values including INT_MIN/MAX and LLONG_MIN/MAX, absent query values, empty ranges at every boundary and non-adjacent empty ranges. Every dynamic update is immediately followed by a singleton query; subsequent rounds compare full query APIs. Added explicit m=0,1,2,3,4,7,8,9,15,16,17 compression cardinality tests (including constructor without extra candidates).

Optimized run: PASS. The default sanitizer invocation failed at LeakSanitizer shutdown with `Cannot open /proc/2/task` / ptrace restriction; leak detection is disabled for the ASan+UBSan rerun and passes with no ASan/UBSan diagnostics.

## Preconditions / limitations (not additional confirmed defects)

- Nonempty query endpoints must be within the array. Empty ranges use array boundary indexes (0 <= l <= n, -1 <= r < n); arbitrary INT_MIN/MAX endpoints are not promised (unlike ST2D clipping). `kth` requires 1 <= k <= length; it has no defined result on an empty range. `update` requires a valid position and a value supplied at build time. Default construction must be followed by build.
- Absence sentinels may equal an actual extreme value; this is the existing API contract.
- Index/count storage uses int, so sizes must fit int and practical memory limits. Value arithmetic is avoided by compression; tested full int/long long value limits without overflow.
- Dynamic treap complexity is expected, not deterministic worst-case; this audit does not stress adversarial priorities or contest-scale memory/time.
- Repository has no discovered project-wide test/build command; other audit tests are being handled by their owners.
