#!/usr/bin/env python3
"""Compile and run the bounded original render-pass-state snapshot decoder."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys


TESTS = Path(__file__).resolve().parent
ROOT = TESTS.parents[2]
IMPLEMENTATION = TESTS.parent / 'render_state_snapshot.cpp'
HEADER = TESTS.parent / 'render_state_snapshot.hpp'
TEST_SOURCE = TESTS / 'render_state_snapshot.cpp'
REFERENCE = TESTS.parent / 'reference/swamp-render-state-audit/original-functions.json'


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(command: list[str | Path], cwd: Path) -> str:
    result = subprocess.run([str(arg) for arg in command], cwd=cwd,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, check=False)
    if result.returncode:
        raise RuntimeError('command failed: ' + ' '.join(map(str, command)) +
                           '\n' + result.stdout[-16000:])
    return result.stdout


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cxx', default='g++', help='host C++17 compiler')
    parser.add_argument('--output', type=Path,
                        default=ROOT / 'port/scene-materials/build/render-state-snapshot-host')
    args = parser.parse_args()
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error('host C++ compiler not found: ' + args.cxx)
    manifest = json.loads(REFERENCE.read_text(encoding='utf-8'))
    assembly_paths = sorted({entry['assembly'] for entry in manifest['functions']})
    assembly_hashes: dict[str, str] = {}
    for relative in assembly_paths:
        path = (ROOT / relative).resolve()
        if ROOT.resolve() not in path.parents:
            raise RuntimeError('original assembly path escapes repository: ' + relative)
        if not path.is_file():
            raise RuntimeError('original assembly listing is missing: ' + relative)
        expected = next(entry['assembly_sha256'] for entry in manifest['functions']
                        if entry['assembly'] == relative)
        actual = digest(path)
        if actual != expected:
            raise RuntimeError('original assembly listing hash mismatch: ' + relative)
        assembly_hashes[relative] = actual
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / ('render-state-snapshot-host.exe' if sys.platform == 'win32'
                           else 'render-state-snapshot-host')
    test_cpp = TEST_SOURCE.resolve()
    implementation_cpp = IMPLEMENTATION.resolve()
    if test_cpp == implementation_cpp:
        parser.error('test and implementation paths unexpectedly alias')
    command = [compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
               '-fno-exceptions', '-fno-rtti', '-fno-fast-math', '-ffp-contract=off',
               str(test_cpp), str(implementation_cpp), '-o', str(executable)]
    if command[10] != str(test_cpp) or command[11] != str(implementation_cpp):
        parser.error('compile command source paths are inconsistent')
    stdout = run(command, ROOT)
    del stdout
    result = run([executable], ROOT)
    line = next((text for text in result.splitlines()
                 if text.startswith('{') and text.endswith('}')), None)
    if not line:
        raise RuntimeError('decoder test did not return structured output:\n' + result)
    evidence = json.loads(line)
    if evidence.get('validation') != 'PASS' or evidence.get('snapshot_bytes') != 32:
        raise RuntimeError('decoder test returned an invalid report')

    report = {
        'scope': ('strict decoding of the recovered 32-byte renderpass::SRenderState '
                  'snapshot and exact static GLenum maps; no AL pass bytes supplied, '
                  'no shader execution or device test'),
        'validation': 'PASS',
        'evidence': evidence,
        'original_elf_sha256': manifest['original_sha256'],
        'original_manifest_sha256': digest(REFERENCE),
        'verified_assembly_sha256': assembly_hashes,
        'source_sha256': {
            path.relative_to(ROOT).as_posix(): digest(path)
            for path in (IMPLEMENTATION, HEADER, TEST_SOURCE, Path(__file__).resolve())
        },
        'compiler': compiler,
        'compile_command': [str(value) for value in command],
        'host_executable': str(executable),
        'host_executable_sha256': digest(executable),
    }
    report_path = output / 'validation.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('PASS: recovered 32-byte pass-state decoder and constructor/GL map fixtures')
    print('report=' + str(report_path))
    print(json.dumps(report))
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError) as error:
        print(str(error), file=sys.stderr)
        raise SystemExit(1)
