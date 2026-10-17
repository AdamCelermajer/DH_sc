"""Compare the native owned level table with the existing recovered catalogue.

All 84 original-cache records and 153 actual range queries are checked. This
does not establish the native Application/current-Level/PlayerInfo producers.
"""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, required=True)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--output', type=Path, default=ROOT/'port/game-data/build/level-tables')
    parser.add_argument('--original-elf', type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    sources = [ROOT/'port/game-data/level_tables.cpp', ROOT/'port/game-data/tests/level_tables.cpp',
               ROOT/'port/level-world/lua_script_level_queries.cpp']
    executable = args.output/'host.exe'
    command = [str(args.compiler), '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic',
               '-fno-fast-math', '-ffp-contract=off', *map(str, sources), '-o', str(executable)]
    subprocess.run(command, check=True, capture_output=True, text=True)
    data = args.cache/'data/pydata'
    paths = [data/name for name in ('levels_pyarray.bin', 'levels_pyarraynames.bin', 'levels_pystructnames.bin')]
    result = subprocess.run([str(executable), *map(str, paths)], check=True, capture_output=True, text=True)
    host = json.loads(result.stdout)
    catalogue_path = ROOT/'port/level-catalogue/catalogue.py'
    spec = importlib.util.spec_from_file_location('dh2_existing_catalogue', catalogue_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    catalogue = module.decode_catalogue(paths[0].read_bytes(), paths[1].read_bytes())
    assert host['validation'] == 'PASS'
    assert host['fast_travel'] == [dataclasses.asdict(row) for row in catalogue.fast_travel]
    assert host['levels'] == [dataclasses.asdict(row) for row in catalogue.levels]
    expected_ranges = [[n, mode, getattr(row, 'monster_lvl_min'+suffix), getattr(row, 'monster_lvl_max'+suffix)]
                       for n, row in enumerate(catalogue.levels)
                       for mode, suffix in enumerate(('', '_hard', '_nightmare'))]
    assert host['ranges'] == expected_ranges
    digest = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    inputs = sources + [ROOT/'port/game-data/level_tables.hpp', ROOT/'port/game-data/data.hpp',
                       ROOT/'port/level-world/lua_script_level_queries.hpp', Path(__file__).resolve(), catalogue_path]
    report = {'validation': 'PASS', 'record_comparisons': 84, 'range_query_comparisons': 153,
              'rejection_cases': host['rejection_cases'], 'mismatches': 0, 'host': host,
              'compiler_command': command, 'executable_sha256': digest(executable),
              'source_sha256': {p.relative_to(ROOT).as_posix(): digest(p) for p in inputs},
              'cache_sha256': {p.name: digest(p) for p in paths}, 'native_wired': False,
              'scope': __doc__}
    if args.original_elf:
        from level_tables_original import compare
        report['original_comparison'] = compare(args.original_elf, paths[0].read_bytes(), host)
        proof_source = Path(__file__).resolve().with_name('level_tables_original.py')
        report['source_sha256'][proof_source.relative_to(ROOT).as_posix()] = digest(proof_source)
    (args.output/'validation.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({key: report[key] for key in ('validation', 'record_comparisons', 'range_query_comparisons', 'rejection_cases', 'mismatches')}))


if __name__ == '__main__':
    main()
