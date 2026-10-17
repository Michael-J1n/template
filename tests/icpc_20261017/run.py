#!/usr/bin/env python3
"""Run from any directory: python3 tests/icpc_20261017/run.py [--sanitize]."""
import argparse
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument('--sanitize', action='store_true')
args = parser.parse_args()
root = Path(__file__).resolve().parents[2]
tests = Path(__file__).resolve().parent
compiler = shlex.split(os.environ.get('CXX', 'g++'))
flags = ['-std=c++17', '-O2', '-Wall', '-Wextra']
if args.sanitize:
    flags = ['-std=c++17', '-O1', '-g', '-fsanitize=address,undefined',
             '-fno-sanitize-recover=all', '-fno-omit-frame-pointer']
env = os.environ.copy()
# LeakSanitizer cannot inspect /proc threads in the execution sandbox.
# Address/undefined-behavior instrumentation remains enabled.
if args.sanitize:
    env['ASAN_OPTIONS'] = 'detect_leaks=0'

def run(source, target):
    subprocess.run(compiler + flags + [str(source), '-o', str(target)], check=True)
    subprocess.run([str(target)], env=env, check=True)

with tempfile.TemporaryDirectory(prefix='icpc_audit_') as directory:
    temp = Path(directory)
    for name in ['fib_st', 'kmp_perm', 'hld', 'wavelet']:
        run(tests / f'test_{name}.cpp', temp / name)
    # Test the actual Fibonacci source under alternative user-selected global
    # constants. Only the constant declaration is substituted; body is unchanged.
    source = (root / '数学/斐波那契数.cpp').read_text()
    declaration = 'const int mod = 1e9 + 7;'
    assert source.count(declaration) == 1
    driver = (tests / 'test_fib_st.cpp').read_text().split('    long long checks = 0;')[0]
    driver += '    cout << "fib variant PASS mod=" << mod << "\\n";\n}\n'
    driver = driver.replace('#include "../../数据结构/ST表-二维.cpp"', '')
    for modulus in [1, 2, 998244353, 1073741825, 2147483647]:
        variant = source.replace(declaration, f'const int mod = {modulus};')
        path = temp / f'fib_{modulus}.cpp'
        path.write_text(driver.replace('#include "../../数学/斐波那契数.cpp"', variant))
        run(path, temp / f'fib_{modulus}')
print('All ICPC targeted audit tests passed.', flush=True)
