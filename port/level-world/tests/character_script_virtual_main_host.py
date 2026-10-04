"""Source-bound audit of external initial/VCB code in the actual world DSO.

Shared libraries are read-only during this audit. It uses the existing original
gold and genuine native Lua/alias ownership, without claiming live external AI.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build', default='/home/adampalace/dh2-world-build')
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    assert not a.output.exists(), 'Refusing to replace a proof'
    sources = [ROOT/x for x in ('character_script_virtual.hpp', 'character_script_virtual.cpp',
               'tests/character_script_virtual.cpp', 'character_script_call_timer.hpp')]
    sources += [REPO/'port/script-runtime'/x for x in ('script_runtime.h', 'script_runtime.c',
                'script_function_alias.h', 'script_function_alias.cpp')]
    sources.append(Path(__file__))
    before = {str(x.relative_to(REPO)): sha(x) for x in sources}
    commands = []

    def run(*args):
        r = subprocess.run(['wsl.exe', '--', *args], capture_output=True, text=True, timeout=60)
        commands.append({'arguments': list(args), 'returncode': r.returncode,
                         'stdout': r.stdout, 'stderr': r.stderr})
        assert r.returncode == 0 and not r.stderr.strip(), commands[-1]
        return r.stdout.strip()

    world = a.build+'/libdh2_level_world.so'
    runtime = a.build+'/script-runtime/libdh2_script_runtime.so'
    exe = a.build+'/character_script_virtual_audit'
    binaries = {x: run('sha256sum', x).split()[0] for x in (world, runtime, exe)}
    deps = run('ldd', exe)
    assert world in deps and runtime in deps and 'libasan.so' in deps and 'libubsan.so' in deps
    compiler = run('ninja', '-C', a.build, '-t', 'commands', 'character_script_virtual_audit')
    for unit in ('character_script_virtual.cpp', 'script_runtime.c', 'script_function_alias.cpp',
                 'tests/character_script_virtual.cpp'):
        rows = [x for x in compiler.splitlines() if unit in x and ' -c ' in x]
        assert rows and all('-fsanitize=address,undefined' in x for x in rows), unit
    gold = ROOT/'reference/character-script-kinds/virtual-fixtures.bin'
    proof = ROOT/'reports/character-script-virtual-arm64-differential.json'
    evidence = json.loads(proof.read_bytes())
    assert evidence['validation'] == 'PASS' and sha(gold) == evidence['gold_sha256']
    gold_hash = sha(gold)
    gold_linux = '/mnt/c/'+str(gold.resolve()).replace('\\', '/')[3:]
    checks = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                           'UBSAN_OPTIONS=halt_on_error=1', exe, gold_linux))
    assert checks['validation'] == 'PASS' and checks['checks'] == 49244
    assert checks['library'] == world and checks['runtime_library'] == checks['alias_library'] == runtime
    assert before == {str(x.relative_to(REPO)): sha(x) for x in sources}
    assert binaries == {x: run('sha256sum', x).split()[0] for x in binaries}
    assert sha(gold) == gold_hash
    report = {'validation': 'PASS', 'scope': __doc__, 'host_audit': checks,
              'source_sha256': before, 'binary_sha256': binaries, 'compiler_commands': compiler,
              'gold_sha256': gold_hash, 'arm64_report_sha256': sha(proof),
              'original_sha256': evidence['original_sha256'], 'linked_dependencies': deps,
              'sanitizers': ['AddressSanitizer', 'UndefinedBehaviorSanitizer'], 'sanitizer_findings': 0,
              'actual_main_world_library_executed': True, 'full_live_ai_verified': False,
              'physical_arm64_verified': False, 'commands': commands}
    a.output.parent.mkdir(parents=True, exist_ok=True)
    with a.output.open('x', encoding='utf-8', newline='\n') as f:
        f.write(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': 'PASS', 'report': str(a.output), 'checks': checks}))


if __name__ == '__main__':
    main()
