# 重链剖分审计（6abee917）

审计范围：`树论/重链剖分-点权.cpp`、`树论/重链剖分-边权.cpp`。未修改模板源文件。

## 结论

- 正确性：满足树/下标/长度前置条件时，未发现路径拆分、子树区间或 toSeq 错误。两版均为 0-based 闭区间；任意构造根正确；边权同点路径为空集合，叶子子树为 `l == r + 1`，调用线段树前须判空。
- 已复现的资源风险：点权 `dfs1` 第 46 行、`dfs2` 第 58 行；边权对应第 49/61 行均递归，链的递归深度为 O(n)。GCC 13.3.0、`-O2`、8 MiB 栈下，两版 n=10,000 通过，n=50,000、100,000、200,000 均 SIGSEGV（returncode -11）。ASan 对两版 n=50,000 明确报告 `stack-overflow`（returncode 1）。这是合法输入上的栈资源限制，非区间公式错误；具体失效阈值依赖编译器和评测栈。按最小修补范围保留实现，赛前遇到深链需选择显式栈 DFS 版本或核实评测栈，不能假设 2e5 链安全。
- 前置条件：图须非空、连通、无环的无向树，节点及 root 在 `[0,n)`，n 可由 int 表示；toSeq 实参长度至少 n。空图、非法根、短权值数组不是受支持输入，不应混作算法反例。默认构造对象需先用完整构造结果赋值再查询；公开 dfs1/dfs2 并非独立重建接口。
- 边权输入契约：a[u] 必须按当前构造根表示父边权，root 槽位仍需存在（值任意）。更换根后不能直接复用旧根的父边权数组。toSeq 原样保留 root 槽位，路径/子树边区间正确排除它。
- 方向契约：返回区间适用于可交换合并，不能直接用于字符串拼接/矩阵连乘等有方向操作；原注释已说明。
- 文档精度：两版第 19 行“其余查询 O(logn)”不能覆盖 toSeq；准确为构造 O(n)、path O(log n)、subtree O(1)、toSeq O(n)，空间 O(n)，其中递归栈最坏 O(n)。未为该文字问题改动源文件。

## 测试覆盖与结果

`test_hld.cpp` 用独立 BFS 父节点/深度与朴素父链求路径，逐节点父链判断子树；展开并排序返回闭区间，核对精确点/边集合，同时禁止重复与越界。

- 链、星形、二叉形：n=1..40，每个节点均作根，共 2460 个实例。
- 随机树：固定 mt19937 seed=20261017，1000 个实例，n=1..40，随机根，随机邻接表顺序。
- 总计 3460 个树/根实例，5,149,390 次点权/边权路径核对，174,062 次点权/边权子树核对；每个实例覆盖全部有序点对，包含单点路径和双向路径。
- 每个实例核对 fa、dep、sz、dfn/rev 逆映射、两版 toSeq，权值包含 INT_MIN、INT_MAX 和负数。toSeq 不运算权值，不发生求和溢出；本测试不把线段树聚合溢出归责于 HLD。
- 优化构建和 ASan/UBSan 全量测试均通过，无诊断。

## 复现命令

从仓库根目录执行（测试与模板均使用 C++17）：

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic tests/icpc_20261017/test_hld.cpp -o /tmp/icpc_test_hld
/tmp/icpc_test_hld
g++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer tests/icpc_20261017/test_hld.cpp -o /tmp/icpc_test_hld_san
ASAN_OPTIONS=detect_leaks=0 /tmp/icpc_test_hld_san
```

两次预期输出：

```text
PASS trees=3460 path_checks=5149390 subtree_checks=174062 seed=20261017
```

独立子进程运行资源失败用例，防止阻断其他测试：

```bash
python - <<'PY'
import os, resource, subprocess
def set_limit():
    resource.setrlimit(resource.RLIMIT_STACK, (8388608, 8388608))
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
for kind in ['point', 'edge']:
    for n in [10000, 50000, 100000, 200000]:
        p = subprocess.run(['/tmp/icpc_test_hld', 'chain', str(n), kind],
                           preexec_fn=set_limit, capture_output=True, text=True)
        print(kind, n, p.returncode, p.stdout.strip())
    p = subprocess.run(['/tmp/icpc_test_hld_san', 'chain', '50000', kind],
                       preexec_fn=set_limit, capture_output=True, text=True,
                       env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0'))
    print(kind, 'ASan', p.returncode)
    print('\n'.join(p.stderr.splitlines()[:7]))
PY
```
