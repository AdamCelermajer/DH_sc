"""Audit selected timer calls through the actual world and Lua shared libraries.

The source AIS gate, aliases, actual VM and native timers compose here. The
test still supplies the selected AIS service identity; ScriptOwner/manager
ownership, Android gameplay and physical device behavior are separate.
"""
import hashlib
import json
import re
import subprocess
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
WORLD = REPO/'port/level-world'
RUNTIME = REPO/'port/script-runtime'
BUILD = '/home/adampalace/dh2-world-build'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(*arguments):
    r = subprocess.run(['wsl.exe', '--', *arguments], capture_output=True, text=True, timeout=90)
    assert r.returncode == 0 and not r.stderr.strip(), (arguments, r.returncode, r.stdout, r.stderr)
    return r.stdout


def main():
    output = WORLD/'reports/character-script-call-timer-main-linked-host-audit.json'
    assert not output.exists(), 'Refusing to replace a saved audit'
    sources = [WORLD/name for name in ('character_script_call_timer.cpp', 'character_script_call_timer.hpp',
               'tests/character_script_call_timer.cpp', 'character_ai_events.cpp', 'character_ai_events.hpp',
               'character_script_timers.cpp', 'character_script_timers.hpp', 'character_timers.cpp',
               'character_timers.hpp', 'CMakeLists.txt')]
    sources += [p for p in RUNTIME.rglob('*') if p.is_file() and p.suffix in ('.h', '.c', '.cpp') and 'tests' not in p.parts]
    sources += [RUNTIME/'CMakeLists.txt', Path(__file__)]
    bindings = {p.relative_to(REPO).as_posix(): sha(p) for p in sources}
    executable = BUILD+'/character_script_call_timer_audit'
    dependencies = run('ldd', executable)
    assert 'not found' not in dependencies
    paths = {executable}
    for line in dependencies.splitlines():
        m = re.search(r'=>\s+(/\S+)|^\s*(/\S+)', line)
        if m:
            paths.add(m.group(1) or m.group(2))
    assert {BUILD+'/libdh2_level_world.so', BUILD+'/script-runtime/libdh2_script_runtime.so'} <= paths

    def hashes():
        return {line.split(maxsplit=1)[1].strip(): line.split()[0]
                for line in run('sha256sum', *sorted(paths)).splitlines()}

    binary_before = hashes()
    commands = run('ninja', '-C', BUILD, '-t', 'commands', 'dh2_level_world', 'dh2_script_runtime')
    units = ('character_script_call_timer.cpp', 'character_ai_events.cpp',
             'character_timers.cpp', 'script_runtime.c', 'lua/lvm.c', 'script_function_alias.cpp')
    for unit in units:
        rows = [line for line in commands.splitlines() if unit in line and ' -c ' in line]
        assert rows and all('-fsanitize=address,undefined' in row for row in rows), unit
    for library, symbol in ((BUILD+'/libdh2_level_world.so', 'dh2_character_script_call_timer'),
                            (BUILD+'/script-runtime/libdh2_script_runtime.so', 'dh2_script_alias_clear_contents')):
        assert symbol in run('nm', '-D', library)
    reference = WORLD/'reference/character-script-call-timer'
    original = json.loads((reference/'original-probe.json').read_text())
    gold = reference/'timer-call-reference.bin'
    assert original['validation'] == 'PASS' and original['gold_sha256'] == sha(gold)
    common = WORLD/'reference/character-script-update/lua-inputs/ai-commons.luac'
    assert sha(common) == '20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    wsl = lambda p: '/mnt/c/Users/adamc/Desktop/workspace/DH_sc/'+p.relative_to(REPO).as_posix()
    command = ['env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1', 'UBSAN_OPTIONS=halt_on_error=1', executable, wsl(gold), wsl(common)]
    audit = json.loads(run(*command))
    assert audit['validation'] == 'PASS' and audit['mismatches'] == 0
    assert audit['original_cases'] == 576 and audit['original_active_calls'] == 288
    assert binary_before == hashes()
    assert bindings == {p.relative_to(REPO).as_posix(): sha(p) for p in sources}
    report = {'validation': 'PASS', 'scope': __doc__, 'host_audit': audit,
              'source_sha256': bindings, 'binary_sha256': binary_before,
              'existing_dso_compiler_commands': commands, 'linked_dependencies': dependencies,
              'run_command': command, 'gold_sha256': sha(gold), 'original_probe_sha256': sha(reference/'original-probe.json'),
              'actual_commons_sha256': sha(common), 'sanitizer_findings': 0,
              'full_ais_or_manager_ownership': False, 'packaged_android_verified': False,
              'physical_arm64_verified': False}
    output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': 'PASS', 'host_audit': audit, 'sanitizer_findings': 0}))


if __name__ == '__main__':
    main()
