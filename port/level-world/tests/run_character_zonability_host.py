"""Execute the original ARM zonability leaf and replay it against the C++ helper."""
from __future__ import annotations
import argparse
import hashlib
import json
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
REFERENCE = ROOT / 'reference/character-zonability'
MAGIC = 0x314e5a43


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def wsl(path: Path) -> str:
    return '/mnt/' + path.drive[0].lower() + str(path)[2:].replace('\\', '/')


def manifest_check(original: Path, manifest: dict) -> None:
    from elftools.elf.elffile import ELFFile
    assert sha(original) == manifest['original_sha256']
    raw = original.read_bytes()
    with original.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name('.symtab').iter_symbols()}
        loads = [segment for segment in elf.iter_segments() if segment['p_type'] == 'PT_LOAD']
        for function in manifest['functions']:
            address = int(function['elf_address'], 0)
            size = function['size']
            symbol = symbols[function['original_symbol']]
            assert (symbol['st_value'], symbol['st_size']) == (address, size), function
            segment = next(item for item in loads if item['p_vaddr'] <= address and
                           address + size <= item['p_vaddr'] + item['p_filesz'])
            offset = int(segment['p_offset']) + address - int(segment['p_vaddr'])
            assert hashlib.sha256(raw[offset:offset + size]).hexdigest() == function['sha256'], function


def original_corpus(original: Path, manifest: dict, corpus: Path) -> dict:
    sys.path.insert(0, str(REPO / 'port/engine-resources/tests'))
    from cpu import Cpu
    from unicorn import UC_HOOK_CODE

    class OracleCpu(Cpu):
        def external(self, uc, address, size, unused):
            if address == self.player_callback:
                self.trace.append(1)  # original virtual IsPlayer fact boundary
                self.put(0, self.player_word)
                uc.reg_write(self.pc, uc.reg_read(self.lr))
                return
            super().external(uc, address, size, unused)

    cpu = OracleCpu(original, False, manifest)
    cpu.player_callback = cpu.callback
    cpu.player_word = 0
    cpu.type_word = 0
    cpu.trace = []
    character, vtable = cpu.data + 0x1000, cpu.data + 0x2000
    cpu.pointer(character, vtable)
    cpu.pointer(vtable + 0x28, cpu.player_callback)

    def observe(uc, address, size, unused):
        if address == 0x3a3094:
            cpu.trace.append(2)  # actual Character::IsFaerie entry
        elif address == 0x3a3054:
            cpu.trace.append(3)  # GetCharType table result supplied here
            cpu.put(0, cpu.type_word)
            uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))
        elif address == 0x38ab60:
            cpu.trace.append(4)  # actual constant-true base-condition entry

    hook = cpu.uc.hook_add(UC_HOOK_CODE, observe)
    players = (0, 1, 2, 0x80000000, 0xffffffff)
    types = (0, 1, 2, 3, 4, 7, 8, 0x80000003, 0xffffffff)
    records = []
    total_player_calls = total_faerie_calls = 0
    try:
        for player in players:
            for char_type in types:
                cpu.player_word, cpu.type_word = player, char_type
                cpu.trace = []
                actual = cpu.invoke(0x3a36e4, [character])
                expected = 0 if player or char_type == 3 else 1
                assert actual == expected, (hex(player), hex(char_type), actual, expected)
                assert cpu.trace and cpu.trace[0] == 1, cpu.trace
                assert (2 in cpu.trace) == (player == 0), cpu.trace
                assert (3 in cpu.trace) == (player == 0), cpu.trace
                assert (4 in cpu.trace) == (player == 0 and char_type != 3), cpu.trace
                total_player_calls += 1
                total_faerie_calls += int(player == 0)
                record = [player, char_type, expected, len(cpu.trace), *cpu.trace]
                record.extend([0] * (8 - len(cpu.trace)))
                records.append(record)
    finally:
        cpu.uc.hook_del(hook)

    required = set(range(0x3a36e4, 0x3a3724, 4))
    required.update(range(0x3a3094, 0x3a30ac, 4))
    required.update(range(0x38ab60, 0x38ab68, 4))
    assert required <= cpu.seen, sorted(hex(value) for value in required - cpu.seen)
    corpus.parent.mkdir(parents=True, exist_ok=True)
    corpus.write_bytes(struct.pack('<II', MAGIC, len(records)) +
                       b''.join(struct.pack('<12I', *record) for record in records))
    return {'comparisons': len(records), 'mismatches': 0,
            'source_virtual_player_calls': total_player_calls,
            'source_faerie_body_calls': total_faerie_calls,
            'source_meet_condition_calls': sum(1 for record in records
                                               if record[0] == 0 and record[1] != 3),
            'original_executed_instructions': len(required),
            'coverage': [hex(value) for value in sorted(required)],
            'corpus_sha256': sha(corpus)}


def run_wsl(*args: str) -> subprocess.CompletedProcess:
    result = subprocess.run(['wsl.exe', '-d', 'Ubuntu-22.04', '--', *args],
                            capture_output=True, text=True, timeout=120)
    assert result.returncode == 0 and not result.stderr.strip(), (args, result.stdout, result.stderr)
    return result


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf', type=Path,
                        default=REPO.parent / 'test_strategy/libDungeonHunter2.so')
    parser.add_argument('--output-dir', type=Path,
                        default=REPO / '.local-inputs/character-zonability')
    parser.add_argument('--report', type=Path,
                        default=ROOT / 'reports/character-zonability-arm32-differential.json')
    args = parser.parse_args()
    original = args.original_elf.resolve()
    manifest = json.loads((REFERENCE / 'original-functions.json').read_text())
    manifest_check(original, manifest)

    source_paths = [ROOT / 'character_zonability.hpp', ROOT / 'character_zonability.cpp',
                    ROOT / 'tests/character_zonability.cpp', ROOT / 'tests/character_zonability_replay.cpp',
                    Path(__file__), REFERENCE / 'original-functions.json',
                    REFERENCE / 'reference/original-functions.asm', REFERENCE / 'NOTES.md']
    source_hashes = {str(path.relative_to(REPO)).replace('\\', '/'): sha(path) for path in source_paths}
    args.output_dir.mkdir(parents=True, exist_ok=True)
    unit = args.output_dir / 'character_zonability_host'
    replay = args.output_dir / 'character_zonability_replay'
    corpus = args.output_dir / 'arm-zonability-fixtures.bin'
    flags = ['-std=c++17', '-O1', '-g', '-fno-fast-math', '-ffp-contract=off',
             '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-Wall', '-Wextra', '-Werror']
    common = ['wsl.exe', '-d', 'Ubuntu-22.04', '--', 'g++', *flags,
              wsl(ROOT / 'character_zonability.cpp')]
    unit_command = common + [wsl(ROOT / 'tests/character_zonability.cpp'), '-o', wsl(unit)]
    replay_command = common + [wsl(ROOT / 'tests/character_zonability_replay.cpp'), '-o', wsl(replay)]
    for command in (unit_command, replay_command):
        built = subprocess.run(command, capture_output=True, text=True, timeout=120)
        assert built.returncode == 0 and not built.stderr.strip(), (command, built.stdout, built.stderr)
    unit_result = run_wsl('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
                          'UBSAN_OPTIONS=halt_on_error=1', wsl(unit))
    original_result = original_corpus(original, manifest, corpus)
    replay_result = run_wsl('env', 'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
                            'UBSAN_OPTIONS=halt_on_error=1', wsl(replay), wsl(corpus))
    host_unit = json.loads(unit_result.stdout)
    host_replay = json.loads(replay_result.stdout)
    assert host_unit['validation'] == host_replay['validation'] == 'PASS'
    assert host_replay['arm_cases'] == original_result['comparisons']
    assert host_replay['portable_provider_calls'] == original_result['source_virtual_player_calls'] + original_result['source_faerie_body_calls']
    assert {key: sha(path) for key, path in zip(source_hashes, source_paths)} == source_hashes
    report = {
        'validation': 'PASS',
        'original_sha256': manifest['original_sha256'],
        'original_elf': str(original),
        'function_manifest_sha256': sha(REFERENCE / 'original-functions.json'),
        'original_arm_execution': original_result,
        'portable_host_tests': host_unit,
        'portable_arm_corpus_replay': host_replay,
        'source_sha256': source_hashes,
        'host_binaries_sha256': {str(unit.relative_to(REPO)): sha(unit),
                                 str(replay.relative_to(REPO)): sha(replay)},
        'corpus_sha256': sha(corpus),
        'sanitizers': ['AddressSanitizer', 'UndefinedBehaviorSanitizer'],
        'sanitizer_findings': 0,
        'scope': 'Pinned original A32 Character::IsZonable, Character::IsFaerie and GameObject::MeetCondition bodies execute; Player virtual fact and GetCharType table word are explicit fixture boundaries. The C++ helper replays final result and ordered top-level predicates. No zone enrollment, in_zone producer, complete CharAI frame or live Ghost pursuit claim.'
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'validation': 'PASS', 'report': str(args.report),
                      'original_arm_execution': original_result,
                      'portable_host_tests': host_unit, 'portable_arm_corpus_replay': host_replay}))


if __name__ == '__main__':
    main()
