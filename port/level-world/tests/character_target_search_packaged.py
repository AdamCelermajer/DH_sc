"""Read-only target-search comparison using the ARM64 DSO from a frozen APK.

Original traversal, filters and heap run against the actual packaged native
instructions. Character/provider callbacks and libm imports remain caller
fixtures. This does not prove live gameplay, Bionic libm or GPU/device parity.
The existing original gold and earlier reports are never rewritten.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import zipfile

from elftools.elf.elffile import ELFFile
from character_target_search_differential import (
    ELF, FIELDS, OPS, REF, REPO, ROOT, NativeSearch, OriginalSearch,
    corpus, object_words, same, sha, words,
)


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build-capture', type=Path, required=True)
    p.add_argument('--extract-dir', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    assert not a.report.exists(), 'Refusing to replace a proof report'
    assert not a.extract_dir.exists(), 'Extraction directory must be new'
    bound = [ELF, REF/'original-functions.json', REF/'search-fixtures.bin',
             ROOT/'character_target_search.cpp', ROOT/'character_target_search.hpp',
             ROOT/'tests/character_target_search_discovery.py',
             ROOT/'tests/character_target_search_differential.py', Path(__file__),
             a.build_capture]
    before = {str(x.resolve()): sha(x) for x in bound}
    with zipfile.ZipFile(a.build_capture) as capture:
        manifest = json.loads(capture.read('build-capture.json'))
        assert manifest['validation'] == 'BUILD_INPUTS_CAPTURED'
        apk = capture.read('packaged-app-debug.apk')
        assert digest(apk) == manifest['apks']['packaged']['sha256']
        assert len(apk) == manifest['apks']['packaged']['bytes']
        for source in ('character_target_search.cpp', 'character_target_search.hpp'):
            key = 'port/level-world/'+source
            raw = capture.read('source/'+key)
            assert digest(raw) == manifest['source_sha256'][key]
            assert raw == (ROOT/source).read_bytes(), 'Captured source differs'
            for project in ('packaged', 'studio'):
                for abi in ('arm64-v8a', 'x86_64'):
                    assert manifest['compiler_inputs'][project][abi]['repository_inputs'][key] == digest(raw)
    member = 'lib/arm64-v8a/libdh2_level_world.so'
    with zipfile.ZipFile(io.BytesIO(apk)) as package:
        library = package.read(member)
        libraries = {}
        for name in package.namelist():
            if not name.startswith('lib/') or not name.endswith('.so'):
                continue
            raw = package.read(name)
            elf = ELFFile(io.BytesIO(raw))
            assert elf.elfclass == 64 and name.split('/')[1] in ('arm64-v8a', 'x86_64')
            alignment = [s['p_align'] for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
            assert alignment and min(alignment) >= 16384
            libraries[name] = {'sha256': digest(raw), 'minimum_load_alignment': min(alignment)}
    assert len(libraries) == 16
    elf = ELFFile(io.BytesIO(library))
    assert elf['e_machine'] == 'EM_AARCH64'
    exported = {s.name for s in elf.get_section_by_name('.dynsym').iter_symbols() if s['st_shndx'] != 'SHN_UNDEF'}
    for symbol in ('dh2_target_search', 'dh2_target_list_init', 'dh2_target_pop'):
        assert symbol in exported
    original_manifest = json.loads((REF/'original-functions.json').read_bytes())
    assert sha(ELF) == original_manifest['original_sha256']
    a.extract_dir.mkdir(parents=True)
    path = a.extract_dir/'libdh2_level_world.so'
    path.write_bytes(library)
    old, new = OriginalSearch(), NativeSearch(path)
    records = []
    callbacks = accepted = 0
    cases = corpus()
    for i, case in enumerate(cases):
        expected, actual = old.execute(case), new.execute(case)
        assert expected['trace'] == actual['trace'], (i, case['label'], 'callback order')
        assert expected['after_visible'] == actual['after_visible'], (i, 'reentry effects')
        assert len(expected['output']) == len(actual['output']), (i, 'target count')
        for e, n in zip(expected['output'], actual['output']):
            assert e[0] == n[0] and e[3:] == n[3:] and same(e[1], n[1]) and same(e[2], n[2]), (i, case['label'], e, n)
        header = [len(case['objects']), len(case['rooms']), case['sort'], case['heading'],
                  case['melee'], case['radius'], case['cone'], case.get('mutate', 0xffffffff),
                  case.get('mutation_target', 0), case.get('reentry', 0xffffffff),
                  len(expected['output']), len(expected['trace'])]
        raw = words(*header)
        for obj in case['objects']:
            raw += words(*object_words(obj))
        for room in case['rooms']:
            raw += words(len(room), *room)
        for target in expected['output']:
            raw += words(*target)
        for op, index in expected['trace']:
            raw += words(OPS.index(op)+1, index)
        records.append(raw+words(*expected['after_visible']))
        callbacks += len(expected['trace'])
        accepted += len(expected['output'])
    math_records = old.c.math_records
    replay = words(0x31525354, len(records))+b''.join(records)+words(len(math_records))
    replay += b''.join(words(op, raw, value) for (op, raw), value in sorted(math_records.items()))
    assert replay == (REF/'search-fixtures.bin').read_bytes(), 'Original replay differs from frozen gold'
    assert before == {str(x.resolve()): sha(x) for x in bound}, 'Proof inputs changed'
    assert path.read_bytes() == library
    report = {
        'validation': 'PASS', 'scope': __doc__, 'comparisons': len(cases),
        'ordered_callbacks': callbacks, 'accepted_records': accepted,
        'synchronous_same_list_reentry_cases': sum('reentry' in x for x in cases),
        'mismatches': 0, 'build_capture_sha256': sha(a.build_capture),
        'apk_sha256': digest(apk), 'apk_bytes': len(apk), 'apk_library_member': member,
        'arm64_library_sha256': digest(library), 'original_sha256': sha(ELF),
        'corpus_sha256': digest(replay), 'source_and_proof_bindings': before,
        'packaged_libraries': libraries, 'actual_packaged_instructions_executed': True,
        'existing_gold_preserved': True, 'libm_caller_fixture_records': len(math_records),
        'original_import_calls': old.c.import_calls, 'native_import_calls': new.c.import_calls,
        'arithmetic_nan_comparison': 'unordered class; finite and non-arithmetic words exact',
        'full_game_verified': False, 'physical_arm64_verified': False,
    }
    a.report.parent.mkdir(parents=True, exist_ok=True)
    with a.report.open('x', encoding='utf-8', newline='\n') as f:
        f.write(json.dumps(report, indent=2)+'\n')
    print(json.dumps({k: v for k, v in report.items() if k not in ('source_and_proof_bindings', 'packaged_libraries', 'original_import_calls', 'native_import_calls')}))


if __name__ == '__main__':
    main()
