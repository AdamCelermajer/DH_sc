"""Audit actual native world/Lua DSOs through source event35 and CharacterTimers.

Selected AIS identities/services in this focused host fixture are caller-owned;
this does not prove original LuaManager construction or APK gameplay ownership.
"""
import argparse, hashlib, json, re, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]

def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest()
def linux(path): return '/mnt/c/' + str(path.resolve()).replace('\\', '/')[3:]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', default='/home/adampalace/dh2-world-build')
    parser.add_argument('--output', type=Path, default=ROOT/'reports/character-script-timers-host-audit.json')
    args = parser.parse_args()
    sources = [ROOT/name for name in ('character_script_timers.hpp', 'character_script_timers.cpp',
        'character_timers.hpp', 'character_timers.cpp', 'character_ai_events.hpp', 'character_ai_events.cpp',
        'tests/character_script_timers.cpp', 'CMakeLists.txt')]
    runtime = REPO/'port/script-runtime'
    sources += [p for p in runtime.rglob('*') if p.is_file() and
        (p.suffix in ('.c', '.h') and 'tests' not in p.parts or p.name == 'CMakeLists.txt')]
    sources.append(Path(__file__))
    before = {p.relative_to(REPO).as_posix(): sha(p) for p in sources}
    commands = []
    def run(*arguments):
        result = subprocess.run(['wsl.exe', '--', *arguments], capture_output=True, text=True, timeout=120)
        commands.append({'arguments': list(arguments), 'returncode': result.returncode,
                         'stdout': result.stdout, 'stderr': result.stderr})
        assert result.returncode == 0 and not result.stderr.strip(), commands[-1]
        return result.stdout.strip()
    cache = run('cmake', '-LA', '-N', args.build)
    assert 'DH2_SCRIPT_SANITIZERS:BOOL=ON' in cache
    compiler = run('ninja', '-C', args.build, '-t', 'commands', 'character_script_timers_audit')
    units = ('character_script_timers.cpp', 'character_ai_events.cpp', 'character_timers.cpp',
             'script_runtime.c', 'script_game_bindings.c', 'lua/lvm.c')
    for unit in units:
        lines = [line for line in compiler.splitlines() if unit in line and ' -c ' in line]
        assert lines and all('-fsanitize=address,undefined' in line for line in lines), unit
    executable = args.build + '/character_script_timers_audit'
    dependencies = run('ldd', executable)
    expected = (args.build+'/libdh2_level_world.so', args.build+'/script-runtime/libdh2_script_runtime.so')
    assert all(path in dependencies for path in expected)
    assert 'libasan.so' in dependencies and 'libubsan.so' in dependencies
    binaries = {executable, *expected}
    binaries.update(re.findall(r'(/[^\s]+\.so(?:\.\d+)*)', dependencies))
    binary_before = {path: run('sha256sum', path).split()[0] for path in sorted(binaries)}
    common = ROOT/'reference/character-script-update/lua-inputs/ai-commons.luac'
    assert sha(common) == '20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
        'UBSAN_OPTIONS=halt_on_error=1', executable, linux(common)))
    assert audit['validation'] == 'PASS' and audit['source_ai_event35_composed']
    assert audit['timer_expiries'] == audit['actual_timer_id_reads'] == 7
    assert audit['selected_ais_vm_calls'] == 6 and audit['inactive_ais_gate_verified']
    assert all(sha(REPO/name) == digest for name, digest in before.items())
    assert binary_before == {path: run('sha256sum', path).split()[0] for path in sorted(binaries)}
    evidence = ROOT/'reports/character-ai-events-arm64-differential.json'
    event_proof = json.loads(evidence.read_text())
    assert event_proof['validation'] == 'PASS'
    for name, digest in event_proof['source_sha256'].items(): assert sha(REPO/name) == digest
    report = {'validation': 'PASS', 'host_audit': audit, 'source_sha256': before,
              'binary_sha256': binary_before, 'linked_dependencies': dependencies,
              'original_ai_event_report_sha256': sha(evidence), 'common_sha256': sha(common),
              'compiler_commands': compiler, 'commands': commands,
              'sanitizers': ['AddressSanitizer', 'UndefinedBehaviorSanitizer'], 'sanitizer_findings': 0,
              'scope': __doc__, 'full_game_playable': False, 'physical_arm64_tested': False}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': 'PASS', 'report': str(args.output), 'checks': audit}))

if __name__ == '__main__': main()
