"""Replay original-bound AI gold in the actual Android ARM64 world ELF.

This executes library machine code with explicit target/controller/AIS services,
TLS canary and imported libc fixtures. It is not a device or full-game test.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from character_animation_ai_differential import Machine

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Refusing to overwrite Android world replay evidence')
    arm_path = ROOT / 'reports/character-animation-ai-arm64-differential.json'
    arm = json.loads(arm_path.read_bytes())
    assert arm['validation'] == 'PASS' and arm['mismatches'] == 0
    assert all(sha(REPO / name) == value for name, value in arm['source_sha256'].items())
    corpus = ROOT / 'reference/character-animation-ai/consumer-fixtures.bin'
    assert sha(corpus) == arm['corpus_sha256']
    library_hash = sha(args.library)
    with args.library.open('rb') as stream:
        elf = ELFFile(stream)
        assert elf.elfclass == 64 and elf['e_machine'] == 'EM_AARCH64'
        loads = [segment for segment in elf.iter_segments() if segment['p_type'] == 'PT_LOAD']
        assert loads and all(segment['p_align'] >= 16384 and
                             segment['p_offset'] % 16384 == segment['p_vaddr'] % 16384 for segment in loads)
        exported = {symbol.name for symbol in elf.get_section_by_name('.dynsym').iter_symbols()
                    if symbol['st_shndx'] != 'SHN_UNDEF'}
        assert {'dh2_character_animation_ai', 'dh2_character_animation_has_combo',
                'dh2_character_animation_table_id'} <= exported
    machine = Machine(args.library, True, {'functions': []})
    raw = corpus.read_bytes()
    at = 0

    def take(count):
        nonlocal at
        assert at + count <= len(raw), 'Truncated AI corpus'
        result = raw[at:at+count]
        at += count
        return result

    def word():
        return struct.unpack('<I', take(4))[0]

    assert take(4) == b'AAI1'
    cases = word()
    requests = 0
    first_state = None
    for index in range(cases):
        op = word()
        state = take(96)
        params = struct.unpack('<16I', take(64))
        expected = take(96)
        call_count = word()
        calls = tuple(take(128) for _ in range(call_count))
        if first_state is None:
            first_state = state
        actual = machine.execute(op, state, params)
        assert actual == (1, expected, calls), ('Android AI replay mismatch', index, op)
        requests += call_count
    scalars = word()
    for index in range(scalars):
        operation, a, b, c, expected = struct.unpack('<5I', take(20))
        symbol = 'dh2_character_animation_table_id' if operation else 'dh2_character_animation_has_combo'
        actual = machine.c.invoke(symbol, [a, b] if operation else [a, b, c])
        assert actual & 0xffffffff == expected, ('Android lookup mismatch', index)
    assert at == len(raw)
    guards = 0

    def reject(state, op=0, null_state=False, null_service=False, missing_invoke=False, overlapping=False):
        nonlocal guards
        machine.put_state(state)
        before = bytes(machine.c.uc.mem_read(machine.s, 96))
        machine.calls = []
        machine.c.uc.mem_write(machine.services, struct.pack('<QQ', machine.c.data+0xf100,
                                                           0 if missing_invoke else machine.service))
        result = machine.c.invoke('dh2_character_animation_ai',
                                  [0 if null_state else machine.s, op,
                                   0 if null_service else machine.s if overlapping else machine.services])
        assert result & 0xffffffff == 0xffffffff
        assert bytes(machine.c.uc.mem_read(machine.s, 96)) == before and not machine.calls
        guards += 1

    reject(first_state, null_state=True)
    reject(first_state, null_service=True)
    reject(first_state, missing_invoke=True)
    reject(first_state, op=7)
    reject(first_state, overlapping=True)
    for offset, value, width in ((0, 0, 8), (8, 0, 8), (76, 1, 4), (92, 1, 4),
                                 (40, 256, 4), (44, 256, 4), (52, 256, 4), (56, 256, 4),
                                 (60, 256, 4), (64, 256, 4), (68, 256, 4), (72, 128, 4)):
        state = bytearray(first_state)
        struct.pack_into('<Q' if width == 8 else '<I', state, offset, value)
        reject(bytes(state))
    assert (cases, requests, scalars, guards) == (2197, 4530, 90, 17)
    assert sha(args.library) == library_hash
    assert all(sha(REPO / name) == value for name, value in arm['source_sha256'].items())
    report = dict(validation='PASS', consumer_comparisons=cases, ordered_service_requests=requests,
                  scalar_comparisons=scalars, atomic_rejection_checks=guards, mismatches=0,
                  library=str(args.library), library_sha256=library_hash, elf_class=64,
                  machine='AArch64', load_alignment=16384, corpus_sha256=sha(corpus),
                  original_sha256=arm['original_sha256'], arm64_report_sha256=sha(arm_path),
                  source_sha256=arm['source_sha256'], audit_script_sha256=sha(Path(__file__)),
                  imported_services=machine.c.import_calls,
                  scope=__doc__ + ' Actual CMake/NDK Debug world library, not the isolated optimized oracle. '
                        'Only these exported kernels execute; unrelated shared-library dependencies are not '
                        'covered. No APK or physical-device claim.')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
