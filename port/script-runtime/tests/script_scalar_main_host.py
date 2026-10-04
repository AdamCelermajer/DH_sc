"""Replay original scalar gold through the main sanitized native Lua library.

This records an existing central build, never rebuilds it or alters older proofs.
Compiler commands describe this build configuration; they are not an Android
compiler-input capture or a physical-device execution claim.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def linux(path):
    return '/mnt/c/' + str(path.resolve()).replace('\\', '/')[3:]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Refusing to replace an existing scalar proof')
    sources = [ROOT / name for name in (
        'CMakeLists.txt', 'script_scalar_bindings.h', 'script_scalar_bindings.c',
        'script_runtime.h', 'script_runtime.c', 'tests/script_scalar.cpp',
        'tests/script_scalar_main_host.py',
        'reference/scalar-bindings/scalar-original-gold.bin',
        'reports/script-scalar-arm64-differential.json')]
    source_hashes = {str(p.relative_to(REPO)): sha(p) for p in sources}
    commands = []

    def run(*command):
        result = subprocess.run(['wsl.exe', '--', *command], capture_output=True,
                                text=True, timeout=60)
        commands.append(dict(arguments=command, returncode=result.returncode,
                             stdout=result.stdout, stderr=result.stderr))
        if result.returncode or result.stderr.strip():
            raise RuntimeError(commands[-1])
        return result.stdout

    executable = args.build + '/script-runtime/script_scalar_audit'
    library = args.build + '/script-runtime/libdh2_script_runtime.so'
    dependency_text = run('ldd', executable)
    assert library in dependency_text and 'not found' not in dependency_text
    assert 'libasan.so' in dependency_text and 'libubsan.so' in dependency_text

    def binaries():
        return {line.split(maxsplit=1)[1].strip(): line.split()[0]
                for line in run('sha256sum', executable, library).splitlines()}

    binary_hashes = binaries()
    compile_commands = json.loads(run('cat', args.build + '/compile_commands.json'))
    selected = [c for c in compile_commands
                if c['file'].endswith(('/script_scalar_bindings.c', '/tests/script_scalar.cpp'))]
    assert len(selected) == 2
    assert all('-fsanitize=address,undefined' in c['command'] and
               '-fno-fast-math' in c['command'] for c in selected)
    imported = run('nm', '-D', executable)
    exported = run('nm', '-D', library)
    names = ['to_fixed', 'from_fixed', 'mul_fixed', 'div_fixed',
             'bit_not', 'bit_and', 'bit_or', 'bit_xor', 'bind']
    for name in names:
        symbol = 'dh2_script_scalar_' + name
        assert (' U ' + symbol) in imported and (' T ' + symbol) in exported
    gold = ROOT / 'reference/scalar-bindings/scalar-original-gold.bin'
    audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
                          'UBSAN_OPTIONS=halt_on_error=1', executable, linux(gold)))
    assert audit['validation'] == 'PASS' and audit['original_gold_cases'] == 13978
    assert audit['actual_VM_checks'] == 37 and audit['mismatches'] == 0
    assert binary_hashes == binaries()
    assert source_hashes == {str(p.relative_to(REPO)): sha(p) for p in sources}
    report = dict(validation='PASS', scope=__doc__, source_sha256=source_hashes,
                  binary_sha256=binary_hashes, compiler_commands=selected,
                  linked_dependencies=dependency_text, host_audit=audit,
                  original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',
                  sanitizer_findings=0, main_runtime_library_executed=True,
                  physical_arm64_verified=False, apk_executed=False, commands=commands)
    args.output.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(validation='PASS', audit=audit, binary_sha256=binary_hashes)))


if __name__ == '__main__':
    main()
