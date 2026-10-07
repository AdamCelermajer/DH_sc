"""Audit owned integers and registered design tables in actual central DSOs.

The table audit runs original monster Init with native constants/metadata/Level.
World inputs and debug effects are explicit test services, not a live-game claim.
No module or test is compiled by this driver; CMake compiler records and dladdr
observations bind the frozen implementation to the actual main libraries.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]


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
        raise RuntimeError('Preserve the existing game-services proof')
    world = ROOT / 'port/level-world'
    data = ROOT / 'port/game-data'
    runtime = ROOT / 'port/script-runtime'
    source_files = [world / name for name in (
        'CMakeLists.txt', 'character_script_owner.cpp', 'character_script_owner.hpp',
        'character_script_owner_bindings.inc', 'character_script_virtual.cpp',
        'character_level.cpp', 'character_level.hpp',
        'tests/character_script_owner.cpp', 'tests/character_script_owner_external.cpp',
        'tests/character_script_owner_include.cpp', 'tests/character_script_owner_integers.cpp')]
    source_files += [data / name for name in (
        'CMakeLists.txt', 'game_design_tables.cpp', 'game_design_tables.hpp',
        'tests/game_design_tables.cpp', 'properties.cpp', 'class_tables.cpp')]
    source_files += [runtime / name for name in (
        'script_runtime.c', 'script_runtime.h', 'script_int_bindings.cpp',
        'script_int_bindings.hpp', 'script_constants.cpp', 'script_constants.hpp',
        'script_design_bindings.c', 'script_design_bindings.h')]
    source_files.append(Path(__file__))
    sources = {path.relative_to(ROOT).as_posix(): sha(path) for path in source_files}
    for report_path, field in (
        (world / 'reports/character-script-owner-integers-host-audit.json', 'source_sha256'),
        (data / 'reports/game-design-tables-arm64-differential.json', 'source_bindings')):
        report = json.loads(report_path.read_text())
        assert report['validation'] == 'PASS'
        for name, digest in report[field].items():
            assert sha(ROOT / name) == digest, ('Frozen implementation changed', name)

    commands = []

    def run(*command):
        result = subprocess.run(['wsl.exe', '--cd', linux(ROOT), *command],
                                capture_output=True, text=True, timeout=120)
        commands.append(dict(arguments=list(command), returncode=result.returncode,
                             stdout=result.stdout, stderr=result.stderr))
        assert result.returncode == 0 and not result.stderr.strip(), commands[-1]
        return result.stdout.strip()

    runtime_so = args.build + '/script-runtime/libdh2_script_runtime.so'
    world_so = args.build + '/libdh2_level_world.so'
    data_so = args.build + '/game-data/libdh2_game_data.so'
    resource_dir = ROOT / '.local-inputs/character-script-owner-extension'
    resources = {name: resource_dir / (name + '.luac')
                 for name in ('_commons', 'follower', 'monster', 'rene')}
    authored = json.loads((world / 'reference/character-script-kinds/authored-ai-rows.json').read_text())
    for record in authored['required_cache_resources']:
        assert sha(resources[record['requested']]) == record['sha256']
    asset_dir = ROOT / 'port/android-native/app/src/main/assets/data'
    fixtures = dict(owner=world / 'reference/character-script-owner/owner-fixtures.bin',
                    tables=data / 'reference/game-design-tables/table-fixtures.bin',
                    level=world / 'reference/character-level/level-fixtures.bin')
    suites = {
        'owner_integers': ('character_script_owner_integers_audit', [resources['_commons']]),
        'legacy_owner': ('character_script_owner_audit', [fixtures['owner'], resources['_commons']]),
        'external_owner': ('character_script_owner_external_audit', list(resources.values())),
        'owned_Include': ('character_script_owner_include_audit', [resources['_commons'], resources['follower']]),
        'design_tables': ('game_design_tables_audit', [fixtures['level'], resources['_commons'],
            resources['monster'], asset_dir, asset_dir / 'ai_pycst.bin',
            asset_dir / 'design_pycst.bin', fixtures['tables']])}
    binaries = [world_so, data_so, runtime_so] + [args.build + '/' + value[0] for value in suites.values()]

    def binary_hashes():
        return {line.split(maxsplit=1)[1]: line.split()[0]
                for line in run('sha256sum', *binaries).splitlines()}

    before = binary_hashes()
    compiler = json.loads(run('cat', args.build + '/compile_commands.json'))
    suffixes = ['/level-world/character_script_owner.cpp', '/game-data/game_design_tables.cpp']
    suffixes += ['/level-world/tests/' + name + '.cpp' for name in (
        'character_script_owner_integers', 'character_script_owner',
        'character_script_owner_external', 'character_script_owner_include')]
    suffixes.append('/game-data/tests/game_design_tables.cpp')
    compiler_records = [record for record in compiler
                        if any(record['file'].endswith(suffix) for suffix in suffixes)]
    assert len(compiler_records) == 7
    assert all('-fsanitize=address,undefined' in record['command'] for record in compiler_records)
    audits, dependencies = {}, {}
    for name, (target, inputs) in suites.items():
        executable = args.build + '/' + target
        dependencies[name] = run('ldd', executable)
        assert all(text in dependencies[name] for text in
                   (world_so, data_so, runtime_so, 'libasan.so', 'libubsan.so'))
        assert 'not found' not in dependencies[name]
        audits[name] = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
            'UBSAN_OPTIONS=halt_on_error=1', executable, *(linux(path) for path in inputs)))
        assert audits[name]['validation'] == 'PASS'
        if name != 'design_tables':
            assert audits[name]['owner_library'] == world_so
            assert audits[name]['runtime_library'] == runtime_so
    integers = audits['owner_integers']
    assert integers['integer_library'] == runtime_so and integers['lifecycle_library'] == world_so
    assert integers['checks'] == 6979 and integers['actual_close_finalizers'] == 2
    assert audits['legacy_owner']['checks'] == 6283
    assert audits['external_owner']['checks'] == 2863
    assert audits['owned_Include']['checks'] == 2005
    tables = audits['design_tables']
    assert tables['design_table_library'] == data_so
    assert tables['property_library'] == tables['level_library'] == world_so
    assert tables['constants_library'] == runtime_so
    assert tables['registered_lookup_cases'] == 1110 and tables['comparisons'] == 870
    assert before == binary_hashes()
    assert sources == {path.relative_to(ROOT).as_posix(): sha(path) for path in source_files}
    report = dict(validation='PASS', scope=__doc__, host_audits=audits,
        source_sha256=sources, binary_sha256=before, compiler_records=compiler_records,
        linked_dependencies=dependencies, commands=commands,
        resource_sha256={name: sha(path) for name, path in resources.items()},
        gold_sha256={name: sha(path) for name, path in fixtures.items()},
        sanitizer_findings=0, main_owner_native_integer_mode=True,
        main_native_design_tables=True, original_full_Application_owned=False,
        generic_VM_close_reentry=False, live_enemy_AI=False,
        full_game_verified=False, packaged_APK=False, physical_arm64_verified=False)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(validation='PASS', audits=audits, binaries=before)))


if __name__ == '__main__':
    main()
