"""Replay initialization prerequisites in actual central CMake libraries.

This driver builds nothing. Separate original-instruction gold and sanitizer
checks establish the service kernels, not live monster AI or APK execution.
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
        raise RuntimeError('Preserve existing initialization-services proof')
    world = ROOT / 'port/level-world'
    data = ROOT / 'port/game-data'
    modules = ('character_spatial_bindings', 'character_host_context', 'character_game_design')
    sources = [world / 'CMakeLists.txt', Path(__file__)]
    for module in modules:
        sources += [world / (module + ext) for ext in ('.hpp', '.cpp')]
        sources.append(world / 'tests' / (module + '.cpp'))
    sources.append(world / 'reference/character-game-design/registered_names.inc')
    source_hashes = {path.relative_to(ROOT).as_posix(): sha(path) for path in sources}
    proofs = {}
    for module, name, field in (
        (modules[0], 'character-spatial-bindings-host-audit.json', 'source_sha256'),
        (modules[1], 'character-host-context-host-audit.json', 'source_sha256'),
        (modules[2], 'character-game-design-host-audit.json', 'source_and_input_sha256')):
        path = world / 'reports' / name
        proof = json.loads(path.read_text())
        assert proof['validation'] == 'PASS'
        for name, digest in proof[field].items():
            normalized = name.replace('\\', '/')
            if normalized in source_hashes:
                assert source_hashes[normalized] == digest, ('Frozen input changed', normalized)
        proofs[path.relative_to(ROOT).as_posix()] = sha(path)
    inputs = {
        'spatial': [world / 'reference/character-spatial-bindings/spatial-original-gold.bin'],
        'host': [world / 'reference/character-host-context/host-context-fixtures.bin',
                 data / 'reference/level-tables'],
        'design': [world / 'reference/character-game-design/real-cache-inputs.bin',
                   data / 'reference/game-design-tables/table-fixtures.bin']}
    suites = dict(zip(inputs, (module + '_audit' for module in modules)))
    input_files = [path for paths in inputs.values() for path in paths if path.is_file()]
    input_files += [data / 'reference/level-tables' / name for name in
                    ('levels_pyarray.bin', 'levels_pyarraynames.bin', 'levels_pystructnames.bin')]
    input_hashes = {path.relative_to(ROOT).as_posix(): sha(path) for path in input_files}
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
    runtime_so = args.build + '/script-runtime/libdh2_script_runtime.so'
    binaries = [world_so, data_so, runtime_so] + [args.build + '/' + target for target in suites.values()]

    def binary_hashes():
        return {line.split(maxsplit=1)[1]: line.split()[0]
                for line in run('sha256sum', *binaries).splitlines()}

    before = binary_hashes()
    compiler = json.loads(run('cat', args.build + '/compile_commands.json'))
    suffixes = ['/level-world/' + module + '.cpp' for module in modules]
    suffixes += ['/level-world/tests/' + module + '.cpp' for module in modules]
    records = [record for record in compiler if any(record['file'].endswith(suffix) for suffix in suffixes)]
    assert len(records) == 6
    assert all('-fsanitize=address,undefined' in record['command'] for record in records)
    symbols = run('nm', '-D', '-C', '--defined-only', world_so)
    assert 'dh2::character::CharacterGameDesign::initialize(' in symbols
    audits, dependencies = {}, {}
    for name, target in suites.items():
        executable = args.build + '/' + target
        dependencies[name] = run('ldd', executable)
        assert all(value in dependencies[name] for value in
                   (world_so, runtime_so, 'libasan.so', 'libubsan.so'))
        assert 'not found' not in dependencies[name]
        audits[name] = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
            'UBSAN_OPTIONS=halt_on_error=1', executable, *(linux(path) for path in inputs[name])))
        assert audits[name]['validation'] == 'PASS'
        assert audits[name]['runtime_library'] == runtime_so
    spatial, host, design = (audits[name] for name in inputs)
    assert spatial['mismatches'] == design['mismatches'] == 0
    assert spatial['kernel_library'] == world_so and spatial['original_gold_cases'] == 2746
    assert host['module_library'] == host['GetLevel_library'] == world_so
    assert host['level_decoder_library'] == data_so and host['original_cases'] == 1233
    assert host['checks'] == 31004 and host['decoded_VM_row_tiers'] == 153
    assert design['world_library'] == world_so and design['data_library'] == data_so
    assert design['original_authored_names_compared'] == 1024 and design['actual_VM_checks'] == 2061
    assert design['actual_finalizers'] == 1 and design['real_constant_assignments'] == 5608
    assert before == binary_hashes()
    assert source_hashes == {path.relative_to(ROOT).as_posix(): sha(path) for path in sources}
    assert input_hashes == {path.relative_to(ROOT).as_posix(): sha(path) for path in input_files}
    report = dict(validation='PASS', scope=__doc__, host_audits=audits,
        source_sha256=source_hashes, input_sha256=input_hashes,
        prerequisite_proof_sha256=proofs, binary_sha256=before,
        compiler_records=records, linked_dependencies=dependencies, commands=commands,
        sanitizer_findings=0, main_library_integrated=True, live_monster_Init=False,
        full_enemy_AI=False, source_Application_session_ownership=False,
        packaged_APK=False, physical_arm64_verified=False)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(validation='PASS', audits=audits, binaries=before)))


if __name__ == '__main__':
    main()
