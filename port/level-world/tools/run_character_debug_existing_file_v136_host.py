"""Focused source DebugSwitches existing-file parser and absent-file gold."""
import hashlib
import json
import argparse
import shlex
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
WORLD = ROOT / 'port/level-world'
OUT = ROOT / '.local-inputs/debug-existing-file-v136'


def linux(path):
    value = str(path.resolve()).replace('\\', '/')
    return '/mnt/' + value[0].lower() + value[2:]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--syntax-callers', action='store_true')
    options = parser.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / 'files').mkdir(exist_ok=True)
    source = WORLD / 'character_design_services.cpp'
    test = WORLD / 'tests/character_debug_existing_file_v136.cpp'
    binary = OUT / 'host'
    gold = WORLD / 'reference/character-design-services/debug-services-fixtures.bin'
    commands = [
        ['wsl.exe', '-e', '/usr/bin/g++', '-std=c++17', '-O1', '-g',
         '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined',
         '-fno-omit-frame-pointer', '-I' + linux(WORLD), linux(source),
         linux(test), '-o', linux(binary)],
        ['wsl.exe', '-e', '/usr/bin/timeout', '30s', '/usr/bin/env',
         'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
         'UBSAN_OPTIONS=halt_on_error=1', linux(binary), linux(gold), linux(OUT / 'files')],
    ]
    runs = []
    for command in commands:
        result = subprocess.run(command, capture_output=True, text=True, timeout=60)
        runs.append(dict(command=command, exit_code=result.returncode,
                         stdout=result.stdout, stderr=result.stderr))
        print(result.stdout, end='')
        if result.stderr:
            print(result.stderr, end='')
        if result.returncode:
            raise SystemExit(result.returncode)
    inputs = [source, test, WORLD / 'character_design_services.hpp',
              WORLD / 'character_debug_stdio_v136.hpp', gold]
    callers = {
        'original_ui_session.cpp': 'character::load_debug_stdio_v136(debug.get(),&debug_files,directory.c_str())',
        'front_ui_session_v87.cpp': 'character::load_debug_stdio_v136(debug.get(),&debug_files,directory.c_str())',
        'source_campaign_runtime_v61.cpp': 'dh2::character::load_debug_stdio_v136(world->debug.get(),world->debug_files,world->files_directory.c_str())',
    }
    native = ROOT / 'port/android-native/app/src/main/cpp'
    for name, call in callers.items():
        text = (native / name).read_text()
        if call not in ''.join(text.split()):
            raise RuntimeError('Actual caller/directory binding absent: ' + name)
        inputs.append(native / name)
    if options.syntax_callers:
        for abi in ('arm64-v8a', 'x86_64'):
            database = ROOT / ('port/android-native/app/.cxx/tools/debug/' + abi + '/compile_commands.json')
            entries = json.loads(database.read_text())
            for name in callers:
                entry = next(e for e in entries if e['file'].replace('\\', '/').endswith('/' + name))
                arguments = entry.get('arguments') or shlex.split(entry['command'].replace('\\', '/'))
                flags, skip = [], False
                for argument in arguments:
                    if skip:
                        skip = False
                        continue
                    if argument == '-o':
                        skip = True
                    elif argument != '-c':
                        flags.append(argument)
                command = flags + ['-fsyntax-only']
                result = subprocess.run(command, cwd=entry['directory'], capture_output=True, text=True, timeout=60)
                runs.append(dict(command=command, abi=abi, source=name, exit_code=result.returncode,
                                 stdout=result.stdout, stderr=result.stderr))
                print(abi, name, result.returncode)
                if result.stderr:
                    print(result.stderr, end='')
                if result.returncode:
                    raise SystemExit(result.returncode)
    receipt = dict(status='PASS', scope=__doc__, runs=runs,
                   sha256={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
                           for p in inputs})
    (OUT / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')


if __name__ == '__main__':
    main()
