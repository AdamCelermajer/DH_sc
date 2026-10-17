#!/usr/bin/env python3
"""Host-check the original render-state field conversion against cached effect bytes."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
from elftools.elf.elffile import ELFFile


TESTS = Path(__file__).resolve().parent
SCENE = TESTS.parent
ROOT = SCENE.parents[1]
MANIFEST = SCENE / 'reference/swamp-effect-pass-conversion/manifest.json'
SOURCES = [
    TESTS / 'source_state_conversion.cpp',
    SCENE / 'source_state_conversion.cpp',
    SCENE / 'render_state_snapshot.cpp',
    SCENE / 'technique_selector.cpp',
    ROOT / 'port/material-bindings/bindings.cpp',
    ROOT / 'port/engine-resources/resources.cpp',
]
HEADERS = [
    SCENE / 'source_state_conversion.hpp',
    SCENE / 'render_state_snapshot.hpp',
    SCENE / 'technique_selector.hpp',
    ROOT / 'port/material-bindings/bindings.hpp',
    ROOT / 'port/engine-resources/resources.hpp',
]


def digest(path: Path) -> str:
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(chunk)
    return h.hexdigest()


def run(command: list[str | Path], cwd: Path) -> str:
    result = subprocess.run([str(value) for value in command], cwd=cwd,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            text=True, check=False)
    if result.returncode:
        raise RuntimeError('command failed: ' + ' '.join(map(str, command)) +
                           '\n' + result.stdout[-16000:])
    return result.stdout


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cxx', default='g++', help='host C++17 compiler')
    parser.add_argument('--effect-bres', type=Path, required=True,
                        help='original cached effect BRES file')
    parser.add_argument('--scene-bres', type=Path, required=True,
                        help='original cached SWAMP module BRES file')
    parser.add_argument('--output', type=Path,
                        default=SCENE / 'build/source-state-conversion-host')
    args = parser.parse_args()
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error('host C++ compiler not found: ' + args.cxx)
    effect = args.effect_bres.resolve()
    if not effect.is_file():
        parser.error('effect BRES does not exist: ' + str(effect))
    manifest = json.loads(MANIFEST.read_text(encoding='utf-8'))
    effect_spec = manifest['effect_bres']
    effect_hash = digest(effect)
    if effect_hash != effect_spec['sha256'] or effect.stat().st_size != effect_spec['size']:
        raise RuntimeError('effect BRES hash/size mismatch')
    scene = args.scene_bres.resolve()
    scene_spec = manifest['scene_bres']
    if not scene.is_file() or digest(scene) != scene_spec['sha256'] or scene.stat().st_size != scene_spec['size']:
        raise RuntimeError('scene BRES hash/size mismatch')

    original_elf = (ROOT.parent / manifest['original_elf']['path'].removeprefix('work/')).resolve()
    if not original_elf.is_file() or digest(original_elf) != manifest['original_elf']['sha256']:
        raise RuntimeError('pinned original ELF is missing or its hash differs')
    function_body_hashes: dict[str, str] = {}
    with original_elf.open('rb') as stream:
        elf = ELFFile(stream)
        symtab = elf.get_section_by_name('.symtab')
        if symtab is None:
            raise RuntimeError('pinned original ELF has no symbol table')
        all_symbols = list(symtab.iter_symbols())
        for function in manifest['functions']:
            symbol = next((item for item in all_symbols
                           if item.name == function['symbol']), None)
            if symbol is None:
                raise RuntimeError('original function symbol is absent: ' + function['symbol'])
            address = int(symbol['st_value'])
            size = int(symbol['st_size'])
            if address != int(function['address'], 16) or size != function['size']:
                raise RuntimeError('original function address/size mismatch: ' + function['symbol'])
            section_index = symbol['st_shndx']
            if not isinstance(section_index, int):
                raise RuntimeError('original function does not reside in an ELF section')
            section = elf.get_section(section_index)
            offset = address - int(section['sh_addr'])
            body = section.data()[offset:offset + size]
            body_hash = hashlib.sha256(body).hexdigest()
            if len(body) != size or body_hash != function['raw_function_bytes_sha256']:
                raise RuntimeError('original function body hash mismatch: ' + function['symbol'])
            function_body_hashes[function['symbol']] = body_hash

    assembly_hashes: dict[str, str] = {}
    for function in manifest['functions']:
        path = (ROOT / function['assembly']).resolve()
        if ROOT.resolve() not in path.parents or not path.is_file():
            raise RuntimeError('original ARM listing is missing or escapes repository')
        if digest(path) != function['assembly_sha256']:
            raise RuntimeError('original ARM listing hash mismatch: ' + str(path))
        assembly_hashes[function['assembly']] = function['assembly_sha256']

    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    executable = output / ('source-state-conversion-host.exe' if sys.platform == 'win32'
                           else 'source-state-conversion-host')
    command: list[str | Path] = [
        compiler, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
        '-fno-exceptions', '-fno-rtti', '-fno-fast-math', '-ffp-contract=off',
        *SOURCES, '-o', executable,
    ]
    run(command, ROOT)
    result = run([executable, effect, scene], ROOT)
    line = next((item for item in result.splitlines()
                 if item.startswith('{') and item.endswith('}')), None)
    if not line:
        raise RuntimeError('test returned no structured result:\n' + result)
    evidence = json.loads(line)
    if evidence.get('validation') != 'PASS' or evidence.get('random_corpus') is None:
        raise RuntimeError('host converter test did not pass')

    corpus = [
        {'case': f'case-{index:02d}', 'source_hex': row['input'],
         'expected_pass_hex': row['output']}
        for index, row in enumerate(evidence['random_corpus'])
    ]
    corpus_path = output / 'original-arm-differential-inputs.json'
    corpus_path.write_text(json.dumps({
        'validation': 'HOST_OUTPUTS_READY_FOR_ORIGINAL_ARM_DIFFERENTIAL',
        'seed': '0x5d7a10c1',
        'cases': corpus,
        'note': ('case-00 all-zero; case-01 all-ff; case-02 exact selected AL+Sp '
                 'BRES source; case-03 byte ramp; cases-04..35 seeded xorshift '
                 'raw 76-byte inputs. expected_pass_hex is the candidate host output '
                 'and must be checked against original instructions before marking '
                 'the differential complete.'),
    }, indent=2) + '\n', encoding='utf-8')

    selected_source = bytes.fromhex(corpus[2]['source_hex'])
    if len(selected_source) != 0x4c:
        raise RuntimeError('selected AL+Sp source object has the wrong size')
    source_path = output / 'selected-al-sp-source-state.bin'
    source_path.write_bytes(selected_source)
    source_provenance_path = output / 'selected-al-sp-source-state.json'
    source_provenance_path.write_text(json.dumps({
        'effect_bres': str(effect),
        'effect_bres_sha256': effect_hash,
        'scene_bres': str(scene),
        'scene_bres_sha256': digest(scene),
        'effect_name': 'Multilight',
        'effect_group': 1,
        'selector_profile': 'GLES2',
        'selector': 'L1_Vc_Al_Sp_----_----_----',
        'serialized_named_record_ordinal': 4,
        'payload_offset_hex': '0x3498',
        'factory_view_offset_hex': '0x1c',
        'source_state_offset_hex': '0x34b4',
        'source_state_size': len(selected_source),
        'source_state_sha256': digest(source_path),
        'source_state_bytes_hex': selected_source.hex(),
        'note': ('BRES bytes copied from the selected named record pass payload. '
                 'Payload is structurally GLES2-shaped; no claim that the original '
                 'device selected this profile or compiled this technique.'),
    }, indent=2) + '\n', encoding='utf-8')

    original_diff_spec = manifest['original_random_differential']
    original_report_path = (ROOT / original_diff_spec['report']).resolve()
    original_runner_path = (ROOT / original_diff_spec['runner']).resolve()
    original_differential = None
    if original_report_path.is_file():
        if not original_runner_path.is_file():
            raise RuntimeError('pinned original differential runner is missing')
        original_report = json.loads(original_report_path.read_text(encoding='utf-8'))
        if digest(original_report_path) != original_diff_spec['report_sha256']:
            raise RuntimeError('tracked original-instruction report hash mismatch')
        if digest(original_runner_path) != original_diff_spec['runner_sha256']:
            raise RuntimeError('original differential runner hash differs from manifest')
        function = manifest['functions'][0]
        checked_cases = original_report.get('cases', [])
        cases_match = len(checked_cases) == len(corpus)
        if cases_match:
            for expected, actual in zip(corpus, checked_cases):
                if (actual.get('source_hex') != expected['source_hex'] or
                        actual.get('expected_pass_hex') != expected['expected_pass_hex'] or
                        actual.get('matched') is not True or
                        actual.get('original_pass_hex') != expected['expected_pass_hex']):
                    cases_match = False
                    break
        if (original_report.get('validation') != 'PASS' or
                original_report.get('mismatches') != 0 or
                original_report.get('original_arm_cases') != len(corpus) or
                original_report.get('original_sha256') != manifest['original_elf']['sha256'] or
                original_report.get('inputs_sha256') != digest(corpus_path) or
                not cases_match or
                original_report.get('function', {}).get('original_symbol') != function['symbol'] or
                int(original_report.get('function', {}).get('elf_address', '0'), 16) != int(function['address'], 16) or
                original_report.get('function', {}).get('size') != function['size'] or
                original_report.get('function', {}).get('sha256') != function['raw_function_bytes_sha256']):
            raise RuntimeError('original instruction differential does not match this host corpus/manifest')
        transient_report_path = (ROOT / original_diff_spec['transient_build_report']).resolve()
        if (transient_report_path.is_file() and
                digest(transient_report_path) != digest(original_report_path)):
            raise RuntimeError('transient and tracked original differential reports differ')
        original_differential = {
            'validation': 'PASS',
            'path': str(original_report_path),
            'sha256': digest(original_report_path),
            'runner': str(original_runner_path),
            'runner_sha256': digest(original_runner_path),
            'cases': len(corpus),
            'mismatches': 0,
            'original_symbol': function['symbol'],
            'raw_function_bytes_sha256': function['raw_function_bytes_sha256'],
        }

    evidence_report = {k: v for k, v in evidence.items() if k != 'random_corpus'}
    if original_differential:
        evidence_report['original_random_differential'] = 'PASS (36/36 original ARM instructions)'
    report = {
        'validation': ('PASS' if original_differential else
                       'PASS_HOST_ONLY_ORIGINAL_DIFFERENTIAL_PENDING'),
        'scope': ('BRES selector ordinal and pass source object are checked; the '
                  '76-to-32-byte conversion matches the exact original constructor '
                  'fixture, an independent field model, and the actual original '
                  '564-byte function on 36 vectors.' if original_differential else
                  'BRES selector ordinal and pass source object are checked; host '
                  'conversion matches the exact original constructor fixture and an '
                  'independent field model. Random-vector original instruction '
                  'differential is pending the original executor.'),
        'effect_bres': str(effect),
        'effect_bres_sha256': effect_hash,
        'original_manifest_sha256': digest(MANIFEST),
        'original_elf_path': str(original_elf),
        'original_elf_sha256': digest(original_elf),
        'original_function_body_sha256': function_body_hashes,
        'assembly_sha256': assembly_hashes,
        'source_sha256': {
            path.resolve().relative_to(ROOT).as_posix(): digest(path.resolve())
            for path in [*SOURCES, *HEADERS, MANIFEST, Path(__file__).resolve()]
        },
        'corpus_path': str(corpus_path),
        'corpus_sha256': digest(corpus_path),
        'corpus_cases': len(corpus),
        'selected_source_object_path': str(source_path),
        'selected_source_object_sha256': digest(source_path),
        'selected_source_provenance_path': str(source_provenance_path),
        'selected_source_provenance_sha256': digest(source_provenance_path),
        'original_differential': original_differential,
        'original_differential_source_sha256': {
            original_diff_spec['runner']: digest(original_runner_path),
            original_diff_spec['report']: digest(original_report_path),
        } if original_differential else None,
        'evidence': evidence_report,
        'compiler': compiler,
        'compile_command': [str(value) for value in command],
        'host_executable': str(executable),
        'host_executable_sha256': digest(executable),
    }
    report_path = output / 'validation.json'
    report_path.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print('PASS: selected GLES2 AL+Sp BRES source object converted to 32-byte snapshot')
    if original_differential:
        print('PASS: original 564-byte ARM conversion differential (' + str(len(corpus)) + ' cases)')
    else:
        print('original ARM differential pending; corpus=' + str(corpus_path))
    print('report=' + str(report_path))
    print(json.dumps(report))
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, ValueError) as error:
        print(str(error), file=sys.stderr)
        raise SystemExit(1)
