#!/usr/bin/env python3
"""Strict host build and tests for the bounded floor-type reader."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent
DEFAULT_CACHE = ROOT.parents[2] / 'cache/files/data/3d/modules/swamp/swamp.bdae'


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cache', type=Path, default=DEFAULT_CACHE)
    parser.add_argument('--without-cache', action='store_true')
    args = parser.parse_args()
    compiler = os.environ.get('CXX', 'c++')
    flags = ['-std=c++17', '-O2', '-fno-exceptions', '-fno-rtti',
             '-Wall', '-Wextra', '-Werror']
    with tempfile.TemporaryDirectory(prefix='dh2-floor-types-') as temp:
        executable = Path(temp) / ('floor_types_tests.exe' if os.name == 'nt' else 'floor_types_tests')
        subprocess.run([compiler, *flags, str(ROOT/'floor_types.cpp'),
                        str(ROOT/'tests/floor_types.cpp'), '-o', str(executable)], check=True)
        command = [str(executable)]
        cache_tested = not args.without_cache
        if cache_tested:
            if not args.cache.is_file():
                raise SystemExit(f'supplied cache file not found: {args.cache}')
            command.append(str(args.cache.resolve()))
        result = subprocess.run(command, check=True, capture_output=True, text=True)
        print(json.dumps({
            'host_build': True,
            'strict_warnings': True,
            'host_tests': True,
            'supplied_cache_tests': cache_tested,
            'cache_path': str(args.cache.resolve()) if cache_tested else None,
            'test_output': result.stdout.strip(),
            'generated_files_tracked': False,
        }, indent=2))


if __name__ == '__main__':
    main()
