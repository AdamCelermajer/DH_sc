"""Replay both original effect corpora through the actual CMake main binaries."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
WORLD = ROOT / 'port/level-world'


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
        raise RuntimeError('Preserve the previous native effects proof')
    files = [WORLD / name for name in (
        'CMakeLists.txt', 'character_native_effects.cpp', 'character_native_effects.hpp',
        'character_native_fsm.cpp', 'character_native_fsm.hpp', 'native_body.cpp',
        'character_timers.cpp', 'tests/character_native_effects.cpp',
        'tests/character_native_effects_focus.cpp')]
    files += [ROOT / 'port/game-data/CMakeLists.txt', Path(__file__)]
    sources = {path.relative_to(ROOT).as_posix(): sha(path) for path in files}
    suites = {
        'effects': ('character_native_effects_audit', 'effect-fixtures.bin', 2648, 6606, 125845),
        'focus_event': ('character_native_effects_focus_audit', 'focus-fixtures.bin', 1176, 8943, 77273)}
    arm_reports, gold_hashes = {}, {}
    for name, (_, fixture, cases, services, _) in suites.items():
        path = WORLD / ('reports/character-native-effects' + ('-focus' if name == 'focus_event' else '') + '-arm64-differential.json')
        report = json.loads(path.read_text())
        gold = WORLD / 'reference/character-native-effects' / fixture
        assert report['validation'] == 'PASS' and report['gold_sha256'] == sha(gold)
        arm_reports[name] = dict(path=path.relative_to(ROOT).as_posix(), sha256=sha(path))
        gold_hashes[name] = sha(gold)
    commands = []

    def run(*command):
        result = subprocess.run(['wsl.exe', '--cd', linux(ROOT), *command],
                                capture_output=True, text=True, timeout=120)
        commands.append(dict(arguments=list(command), returncode=result.returncode,
                             stdout=result.stdout, stderr=result.stderr))
        assert result.returncode == 0 and not result.stderr.strip(), commands[-1]
        return result.stdout.strip()

    world_so = args.build + '/libdh2_level_world.so'
    data_so = args.build + '/game-data/libdh2_game_data.so'
    binaries = [world_so, data_so, args.build + '/script-runtime/libdh2_script_runtime.so']
    binaries += [args.build + '/' + entry[0] for entry in suites.values()]

    def hashes():
        return {line.split(maxsplit=1)[1]: line.split()[0]
                for line in run('sha256sum', *binaries).splitlines()}

    before = hashes()
    records = json.loads(run('cat', args.build + '/compile_commands.json'))
    suffixes = ['/level-world/' + name for name in (
        'character_native_effects.cpp', 'tests/character_native_effects.cpp',
        'tests/character_native_effects_focus.cpp')]
    compiler = [record for record in records if any(record['file'].endswith(suffix) for suffix in suffixes)]
    assert len(compiler) == 3 and all('-fsanitize=address,undefined' in record['command'] for record in compiler)
    audits, dependencies = {}, {}
    for name, (target, fixture, cases, services, checks) in suites.items():
        executable = args.build + '/' + target
        dependencies[name] = run('ldd', executable)
        assert all(token in dependencies[name] for token in (world_so, data_so, 'libasan.so', 'libubsan.so'))
        assert 'not found' not in dependencies[name]
        audit = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
            'UBSAN_OPTIONS=halt_on_error=1', executable,
            linux(WORLD / 'reference/character-native-effects' / fixture)))
        assert audit['validation'] == 'PASS' and audit['module_library'] == audit['body_library'] == world_so
        assert (audit['original_cases'], audit['ordered_services'], audit['checks']) == (cases, services, checks)
        if name == 'effects':
            assert audit['timer_library'] == world_so and audit['genuine_native_body_pin']
        else:
            assert audit['RNG_library'] == data_so and audit['actual_native_focus_unpins'] == 2
        audits[name] = audit
    assert before == hashes() and sources == {path.relative_to(ROOT).as_posix(): sha(path) for path in files}
    args.output.write_text(json.dumps(dict(validation='PASS', scope=__doc__, host_audits=audits,
        source_sha256=sources, binary_sha256=before, compiler_records=compiler,
        gold_sha256=gold_hashes, arm64_reports=arm_reports, commands=commands,
        dependencies=dependencies, sanitizer_findings=0,
        source_registered_StateInfo_owned=False, full_timer_expiry_transition=False,
        packaged_APK=False, live_game_effects_verified=False, physical_arm64_verified=False), indent=2) + '\n')
    print(json.dumps(dict(validation='PASS', audits=audits, binaries=before)))


if __name__ == '__main__':
    main()
