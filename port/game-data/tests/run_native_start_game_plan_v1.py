"""Compile the NativeStartGame resolver and check it against the packaged source LevelList."""
import argparse
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, required=True)
    parser.add_argument('--data', type=Path,
                        default=ROOT/'port/android-native/app/src/main/assets/data')
    parser.add_argument('--output', type=Path,
                        default=ROOT/'port/game-data/build/native-start-game-plan-v1')
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    executable = args.output/'host.exe'
    sources = [ROOT/'port/game-data/level_tables.cpp',
               ROOT/'port/game-data/native_start_game_plan_v1.cpp',
               ROOT/'port/game-data/tests/native_start_game_plan_v1.cpp']
    command = [str(args.compiler), '-std=c++17', '-O1', '-Wall', '-Wextra', '-Werror',
               '-pedantic', '-fno-fast-math', '-ffp-contract=off',
               *map(str, sources), '-o', str(executable)]
    subprocess.run(command, check=True, capture_output=True, text=True)
    inputs = [args.data/name for name in ('levels_pyarray.bin', 'levels_pyarraynames.bin',
                                          'levels_pystructnames.bin')]
    result = subprocess.run([str(executable), *map(str, inputs)],
                            check=True, capture_output=True, text=True)
    receipt = json.loads(result.stdout)
    if receipt.get('validation') != 'PASS' or receipt.get('mismatches') != 0:
        raise RuntimeError(f'NativeStartGame resolver mismatch: {receipt}')
    print(json.dumps(receipt))


if __name__ == '__main__':
    main()
