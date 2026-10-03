"""Build and bind an isolated sanitized replay of actual combat-query gold."""
import argparse, hashlib, json, shlex, subprocess, zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
WSL_REPO = '/mnt/c/Users/adamc/Desktop/workspace/DH_sc'

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def run(*args):
    result = subprocess.run(['wsl.exe', '--cd', WSL_REPO, *args],
                            capture_output=True, text=True, timeout=120)
    assert result.returncode == 0, (result.returncode, result.stdout, result.stderr)
    assert not result.stderr.strip(), result.stderr
    return result.stdout.strip()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--executable', default='/home/adampalace/dh2-world-build/character_combat_queries_isolated_audit')
    parser.add_argument('--rebuild', action='store_true')
    parser.add_argument('--output', type=Path, default=ROOT/'reports/character-combat-queries-host-audit.json')
    parser.add_argument('--cache', type=Path, default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'))
    args = parser.parse_args()
    reference = ROOT/'reference/character-combat-queries'
    evidence_path = reference/'original-arm64.json'
    evidence = json.loads(evidence_path.read_text())
    assert evidence['validation'] == 'PASS' and evidence['mismatches'] == 0
    assert sha(reference/'query-fixtures.bin') == evidence['corpus_sha256']
    assert sha(reference/'original-functions.json') == evidence['manifest_sha256']
    for name, expected in evidence['source_sha256'].items():
        assert sha(REPO/name) == expected, name
    sources = ['port/level-world/character_combat_queries.cpp',
               'port/level-world/tests/character_combat_queries.cpp',
               'port/game-data/data.cpp', 'port/game-data/animation_tables.cpp']
    tracked = sources + ['port/level-world/character_combat_queries.hpp',
                         'port/game-data/data.hpp', 'port/game-data/animation_tables.hpp',
                         'port/level-world/tests/character_combat_queries_host.py',
                         'port/level-world/tests/character_combat_queries_differential.py']
    source_hashes = {name: sha(REPO/name) for name in tracked}
    scratch = REPO/'.local-inputs/character-combat-queries-discovery'
    snapshot = scratch/'host-build-inputs.json'
    command = ['g++', '-std=c++17', '-Wall', '-Wextra', '-Werror', '-fno-fast-math',
               '-ffp-contract=off', '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
               '-g', *sources, '-o', args.executable]
    if args.rebuild:
        run(*command)
    executable_hash = run('sha256sum', args.executable).split()[0]
    compiler = run('g++', '--version').splitlines()[0]
    inputs = {'source_sha256': source_hashes, 'executable_sha256': executable_hash,
              'compiler': compiler, 'command': shlex.join(command)}
    assert all(sha(REPO/name) == expected for name, expected in source_hashes.items()), 'Build inputs changed'
    if args.rebuild:
        snapshot.write_text(json.dumps(inputs, indent=2)+'\n')
    else:
        assert snapshot.exists() and json.loads(snapshot.read_text()) == inputs, 'Rebuild with stable inputs first'
    host = json.loads(run('env', 'ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                         'UBSAN_OPTIONS=halt_on_error=1', args.executable,
                         'port/level-world/reference/character-combat-queries/query-fixtures.bin',
                         'port/android-native/app/src/main/assets'))
    assert host['validation'] == 'PASS' and host['mismatches'] == 0
    for name in ('comparisons', 'range_queries', 'inventory_queries', 'combo_queries'):
        assert host[name] == evidence[name], name
    assert host['real_table_bridges'] == 448 and host['native_guards'] == 15
    assert sha(args.cache) == '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    assets = REPO/'port/android-native/app/src/main/assets'
    asset_names = ['data/character_properties_pyarray.bin', 'data/character_properties_pyarraynames.bin',
                   'data/character_properties_pystructnames.bin', 'data/animations_pyarray.bin',
                   'data/animations_pyarraynames.bin', 'data/animations_pystructnames.bin',
                   'data/animations_dictionary_pyarray.bin', 'data/animations_dictionary_pyarraynames.bin']
    cache_inputs = []
    with zipfile.ZipFile(args.cache) as archive:
        for name in asset_names:
            path = assets/name
            raw = path.read_bytes()
            matches = [entry for entry in archive.namelist() if entry.endswith('/'+path.name) and archive.read(entry) == raw]
            assert len(matches) == 1, (name, matches)
            cache_inputs.append({'asset': name, 'cache_entry': matches[0], 'bytes': len(raw), 'sha256': sha(path)})
    assert all(sha(REPO/name) == expected for name, expected in source_hashes.items()), 'Replay inputs changed'
    report = {'validation': 'PASS', 'host_audit': host,
              'sanitizers': ['AddressSanitizer', 'UndefinedBehaviorSanitizer'], 'sanitizer_findings': 0,
              **inputs, 'build_inputs_sha256': sha(snapshot), 'cache_sha256': sha(args.cache),
              'cache_inputs': cache_inputs, 'original_instruction_evidence': {
                  'report_sha256': sha(evidence_path), **evidence},
              'scope': 'Isolated native query source plus real native property/animation-table loaders. Resolved ItemTable fixture rows and equipment references are supplied read-only inputs; item serialization and inventory ownership are not reconstructed by this module.'}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'validation': 'PASS', **host}))

if __name__ == '__main__':
    main()
