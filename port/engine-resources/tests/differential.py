#!/usr/bin/env python3
"""Run original ARM32 instructions against the compiled ARM64 resource port.

Use the exact supplied ELF and all recovered BRES files. This tests the complete
buffer relocation branch, not res::File's split allocation/external-file branch.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import random
import struct
import tempfile
import time
from elftools.elf.elffile import ELFFile
from cpu import Cpu, u32, i32

ROOT = Path(__file__).resolve().parents[1]
SHA256 = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
LAYOUTS = [(0x24, 0x28, 32), (0x34, 0x38, 12), (0x3c, 0x40, 28),
           (0x44, 0x48, 24), (0x4c, 0x50, 20), (0x54, 0x58, 116),
           (0x5c, 0x60, 36), (0x68, 0x6c, 16), (0x70, 0x74, 12),
           (0x78, 0x7c, 144), (0x80, 0x84, 232), (0x88, 0x8c, 16), (0x90, 0x94, 36)]
NAMES = ['animation', 'animation_clip', 'camera', 'light', 'image', 'effect',
         'material', 'geometry', 'controller', 'emitter', 'gnps_emitter', 'force', 'coronas']

def verify_original(path, provenance):
    if hashlib.sha256(path.read_bytes()).hexdigest() != SHA256:
        raise AssertionError('Original ELF SHA-256 mismatch')
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        for row in provenance['functions']:
            addr = int(row['elf_address'], 16)
            segment = next(s for s in loads if s['p_vaddr'] <= addr
                           and addr + row['size'] <= s['p_vaddr'] + s['p_filesz'])
            stream.seek(segment['p_offset'] + addr - segment['p_vaddr'])
            assert hashlib.sha256(stream.read(row['size'])).hexdigest() == row['sha256']

class Checks:
    def __init__(self, old, new, provenance):
        self.old, self.new = old, new
        self.rows = {r['port_symbol']: r for r in provenance['functions']
                     if r['port_symbol'].startswith(('dh2_memory_', 'dh2_limit_'))}
        self.calls = Counter()
        self.comparisons = 0

    def stream_objects(self, cpu, length, cursor=0, start=0, limit=0, lcursor=None, buffer_null=False):
        mem, lim = cpu.data + 0x1000, cpu.data + 0x2000
        buf, name, path = cpu.data + 0x4000, cpu.data + 0x7000, cpu.data + 0x7100
        cpu.uc.mem_write(buf, bytes(range(256)) * 4)
        cpu.uc.mem_write(name, b'borrowed-resource\0')
        cpu.uc.mem_write(path, b'parent/subfile\0')
        cpu.uc.mem_write(cpu.data + 0x6000 - 16, b'\xa5' * 1056)
        cpu.uc.mem_write(cpu.data + 0x8000, bytes(32))
        if cpu.arm64:
            cpu.invoke('dh2_memory_init', [mem, 0 if buffer_null else buf, u32(length), name])
            cpu.uc.mem_write(mem + 20, struct.pack('<I', u32(start)))
            cpu.invoke('dh2_limit_init', [lim, mem, u32(limit), name, path])
            cpu.uc.mem_write(mem + 20, struct.pack('<I', u32(cursor)))
            cpu.uc.mem_write(lim + 28, struct.pack('<I', u32(start if lcursor is None else lcursor)))
        else:
            for obj, prefix, positions in [(mem, 'dh2_memory_', {12: 0 if buffer_null else buf, 24: length, 28: cursor, 52: name}),
                                           (lim, 'dh2_limit_', {32: name, 56: path, 60: limit, 64: start,
                                                               68: u32(start + limit), 72: mem,
                                                               76: start if lcursor is None else lcursor})]:
                cpu.uc.mem_write(obj, bytes(96))
                table = cpu.data + (0x3000 if obj == mem else 0x3200)
                cpu.pointer(obj, table)
                for offset, value in positions.items():
                    cpu.uc.mem_write(obj + offset, struct.pack('<I', u32(value)))
                for offset, suffix in [(12, 'read'), (16, 'async'), (20, 'async_at'),
                                       (24, 'seek'), (32, 'size'), (36, 'position'),
                                       (40, 'name'), (44, 'full_path')]:
                    cpu.pointer(table + offset, cpu.symbols[self.rows[prefix + suffix]['original_symbol']])
        return mem, lim

    def observation(self, cpu, result, pointer=False):
        mem, lim = cpu.data + 0x1000, cpu.data + 0x2000
        mpos = struct.unpack('<i', cpu.uc.mem_read(mem + (20 if cpu.arm64 else 28), 4))[0]
        lpos = struct.unpack('<i', cpu.uc.mem_read(lim + (28 if cpu.arm64 else 76), 4))[0]
        capture = cpu.uc.mem_read(cpu.data + 0x8000, 32 if cpu.arm64 else 24)
        c = struct.unpack('<iiQQIi' if cpu.arm64 else '<iiIIIi', capture)
        c = (c[0], c[1], c[2] - cpu.data if c[2] else 0,
             c[3] - cpu.data if c[3] else 0, c[4], c[5])
        return (result - cpu.data if pointer and result else u32(result), mpos, lpos,
                bytes(cpu.uc.mem_read(cpu.data + 0x6000 - 16, 1056)),
                bytes(cpu.uc.mem_read(cpu.data + 0x8200, 16)), c)

    def stream_case(self, symbol, state, values=(), pointer=False):
        results = []
        for cpu in (self.old, self.new):
            mem, lim = self.stream_objects(cpu, **state)
            obj = mem if symbol.startswith('dh2_memory_') else lim
            args = [obj]
            cpu.uc.mem_write(cpu.data + 0x8200, b'\xcc' * 16)
            for v in values:
                args.append(cpu.data + v[1] if isinstance(v, tuple) and v[0] == 'ptr'
                            else (cpu.symbols['test_completion'] if cpu.arm64 else cpu.callback)
                            if v == 'callback' else u32(v))
            entry = symbol if cpu.arm64 else self.rows[symbol]['original_symbol']
            result = cpu.invoke(entry, args)
            results.append(self.observation(cpu, result, pointer))
        assert results[0] == results[1], (symbol, state, values, results[0][:3], results[1][:3], results[0][-1], results[1][-1])
        self.calls[symbol] += 1
        self.comparisons += 1

def streams(check):
    rng = random.Random(0xD22026)
    out, capture, posptr = ('ptr', 0x6000), ('ptr', 0x8000), ('ptr', 0x8200)
    for name in ['all_in_memory', 'valid', 'size', 'position', 'name', 'full_path', 'buffer']:
        for length in [-2147483648, -1, 0, 1, 256, 2147483647]:
            values = (posptr,) if name == 'buffer' else ()
            check.stream_case('dh2_memory_' + name, dict(length=length, cursor=17), values,
                              name in ('name', 'full_path', 'buffer'))
    for name in ['size', 'position', 'name', 'full_path']:
        check.stream_case('dh2_limit_' + name,
                          dict(length=128, cursor=5, start=11, limit=35, lcursor=19),
                          pointer=name in ('name', 'full_path'))
    check.stream_case('dh2_memory_valid', dict(length=0, buffer_null=True))
    check.stream_case('dh2_memory_buffer', dict(length=0, buffer_null=True), (0,), pointer=True)
    check.stream_case('dh2_memory_buffer', dict(length=0), (0,), pointer=True)
    for _ in range(1200):
        length = rng.randrange(513)
        cursor = rng.randrange(length + 1)
        count = rng.choice([0, 1, length, 1024, rng.randrange(1025), 0x80000000, 0xffffffff])
        state = dict(length=length, cursor=cursor)
        check.stream_case('dh2_memory_read', state, (out, count))
        check.stream_case('dh2_memory_async', state, (out, count, 'callback', capture))
        offset = rng.randrange(length + 21)
        check.stream_case('dh2_memory_async_at', state, (out, count, offset, 'callback', capture))
        start = rng.randrange(length + 1)
        size = rng.randrange(513)  # May extend past the underlying file, provoking short reads.
        lcursor = start + rng.randrange(size + 1)
        state = dict(length=length, cursor=cursor, start=start, limit=size, lcursor=lcursor)
        # Reads only use ranges that cannot cause the original memory reader to
        # copy before its buffer. Huge negative-as-signed counts still execute.
        check.stream_case('dh2_limit_read', state, (out, count))
        check.stream_case('dh2_limit_async', state, (out, count, 'callback', capture))
        offset = rng.randrange(size + 21)
        check.stream_case('dh2_limit_async_at', state, (out, count, offset, 'callback', capture))
    edge = [-2147483648, -1, 0, 1, 256, 2147483647]
    for _ in range(1600):
        size, cursor, requested = [rng.choice(edge) if rng.random() < .5 else i32(rng.getrandbits(32)) for _ in range(3)]
        relative = rng.randrange(2)
        check.stream_case('dh2_memory_seek', dict(length=size, cursor=cursor), (requested, relative))
        start, limit, lcursor = [i32(rng.getrandbits(32)) for _ in range(3)]
        check.stream_case('dh2_limit_seek', dict(length=size, cursor=cursor, start=start,
                                               limit=limit, lcursor=lcursor), (requested, relative))

def corpus(check, root, provenance, maximum=None):
    paths = sorted(root.rglob('*.bdae'))
    if maximum:
        paths = paths[:maximum]
    assert paths, 'No recovered .bdae resources found'
    init = next(r for r in provenance['functions'] if r['scope'] != 'complete function')['original_symbol']
    getters = {r['library_kind']: r['original_symbol'] for r in provenance['functions'] if 'library_kind' in r}
    parts = {r['root_part_kind']: r['original_symbol'] for r in provenance['functions'] if 'root_part_kind' in r}
    version = next(r['original_symbol'] for r in provenance['functions'] if r['port_symbol'] == 'dh2_bres_version')
    total_bytes, total_fixups, pointer_checks, library_checks = 0, 0, 0, 0
    libraries, versions, headers = Counter(), Counter(), Counter()
    digest = hashlib.sha256()
    for number, path in enumerate(paths):
        raw = path.read_bytes()
        h = struct.unpack_from('<15I', raw)
        fixups = struct.unpack_from('<' + 'I' * h[4], raw, h[6])
        payloads = [struct.unpack_from('<I', raw, f)[0] for f in fixups]
        expected = bytearray(raw)
        old, new = check.old, check.new
        oldbuf, newbuf = old.data + 0x100000, new.data + 0x100000
        view, output = new.data + 0x9000, new.data + 0x1000000
        for cpu, buf in [(old, oldbuf), (new, newbuf)]:
            cpu.uc.mem_write(buf - 16, b'\xa5' * 16 + raw + b'\xa5' * 16)
        obj = old.data + 0xa000
        old.uc.mem_write(obj, struct.pack('<I', oldbuf) + bytes(40))
        assert old.invoke(init, [obj], budget=200 + h[4] * 24) == 0
        # Confirm every byte of the original's in-place mutation, not just a
        # host model of how we think its relocations should work.
        struct.pack_into('<H', expected, 6, struct.unpack_from('<H', raw, 6)[0] | 0x8000)
        for i, field in enumerate(fixups):
            struct.pack_into('<I', expected, h[6] + 4 * i, oldbuf + field)
            struct.pack_into('<I', expected, field, oldbuf + payloads[i])
        assert bytes(old.uc.mem_read(oldbuf, len(raw))) == expected, path
        assert new.invoke('dh2_bres_open', [view, newbuf, len(raw)], budget=200 + h[4] * 70) == 0, path
        assert new.invoke('dh2_bres_fixups', [view, output, h[4]], budget=200 + h[4] * 40) == 0, path
        pairs = struct.unpack('<' + 'Q' * (2 * h[4]), new.uc.mem_read(output, h[4] * 16))
        assert all(pairs[2*i] == newbuf + f and pairs[2*i+1] == newbuf + payloads[i]
                   for i, f in enumerate(fixups)), path
        assert bytes(new.uc.mem_read(newbuf, len(raw))) == raw, path
        for cpu, buf in [(old, oldbuf), (new, newbuf)]:
            assert bytes(cpu.uc.mem_read(buf - 16, 16)) == b'\xa5' * 16
            assert bytes(cpu.uc.mem_read(buf + len(raw), 16)) == b'\xa5' * 16
        # Original CColladaDatabase -> CResFile -> serialized header graph.
        database, resource = old.data + 0xb000, old.data + 0xb100
        old.pointer(database, resource)
        old.pointer(resource + 0x24, oldbuf)
        vptr = old.invoke(version, [database])
        assert new.invoke('dh2_bres_version', [view]) - newbuf == vptr - oldbuf
        v = payloads[fixups.index(h[8])] if h[8] in fixups else struct.unpack_from('<I', raw, h[8])[0]
        # Version field points into the length-prefixed string table; return is
        # the character data, already represented by its serialized offset.
        versions[raw[v:raw.index(b'\0', v)].decode('ascii')] += 1
        for kind, oldsymbol in parts.items():
            assert new.invoke('dh2_bres_root_part', [view, kind]) - newbuf == old.invoke(oldsymbol, [database]) - oldbuf
            library_checks += 1
        counts = []
        for kind, (count_at, pointer_at, stride) in enumerate(LAYOUTS):
            n, p = struct.unpack_from('<I', raw, h[8] + count_at)[0], struct.unpack_from('<I', raw, h[8] + pointer_at)[0]
            counts.append(n)
            assert new.invoke('dh2_bres_library_count', [view, kind]) == n, (path, kind, n, p)
            for index in sorted({0, n // 2, n - 1}) if n else []:
                a = old.invoke(getters[kind], [database, index])
                b = new.invoke('dh2_bres_library_item', [view, kind, index])
                assert a - oldbuf == b - newbuf == p + index * stride, (path, kind, index)
                library_checks += 1
            assert new.invoke('dh2_bres_library_item', [view, kind, n]) == 0
            assert new.invoke('dh2_bres_library_item', [view, kind, u32(-1)]) == 0
            libraries[NAMES[kind]] += n
        item = {'path': str(path.relative_to(root)), 'sha256': hashlib.sha256(raw).hexdigest(),
                'bytes': len(raw), 'fixups': h[4], 'library_counts': counts}
        digest.update((json.dumps(item, sort_keys=True, separators=(',', ':')) + '\n').encode())
        headers['bulk_blocks' if h[12] else 'no_bulk_blocks'] += 1
        headers['tail_bytes' if h[14] else 'no_tail_bytes'] += 1
        total_bytes += len(raw)
        total_fixups += h[4]
        pointer_checks += h[4] * 2
        if number % 250 == 0:
            print(json.dumps({'bres_progress': number + 1, 'of': len(paths), 'fixups': total_fixups}), flush=True)
        # Coverage remains enabled throughout. A complete File::Init body is
        # not claimed: its split-buffer and external-reference branches differ.
    return {'files': len(paths), 'bytes': total_bytes, 'fixups': total_fixups,
            'native_pointer_values_compared': pointer_checks,
            'library_pointer_comparisons': library_checks, 'libraries': dict(libraries),
            'versions': dict(versions), 'headers': dict(headers),
            'ordered_asset_manifest_sha256': digest.hexdigest()}

def malformed_and_guards(cpu, sample):
    raw = sample.read_bytes()
    view, buf = cpu.data + 0x9000, cpu.data + 0x100000
    cases = [(b'', 2), (raw[:59], 2)]
    for at, value, error in [(0, 0, 3), (4, 0, 4), (6, 0x8000, 5), (8, 64, 6),
                             (12, len(raw)+1, 7), (20, 0x80000000, 8), (16, 0xffffffff, 9),
                             (24, 64, 9), (28, 0, 9), (60, 25, 9), (64, len(raw), 10),
                             (32, len(raw)-4, 12)]:
        d = bytearray(raw)
        struct.pack_into('<H' if at == 6 else '<I', d, at, value)
        cases.append((bytes(d), error))
    d = bytearray(raw)
    table = struct.unpack_from('<' + 'I' * struct.unpack_from('<I', raw, 16)[0], raw, 60)
    field = next(f for f in table if f >= struct.unpack_from('<I', raw, 32)[0])
    struct.pack_into('<I', d, field, len(raw)+1)
    cases.append((bytes(d), 11))
    for d, error in cases:
        if d:
            cpu.uc.mem_write(buf, d)
        cpu.uc.mem_write(view - 16, b'\xa5' * 80)
        assert cpu.invoke('dh2_bres_open', [view, buf, len(d)]) == error, (error, d[:60])
        assert bytes(cpu.uc.mem_read(view, 48)) == bytes(48)
        assert bytes(cpu.uc.mem_read(view - 16, 16)) == b'\xa5' * 16
        assert bytes(cpu.uc.mem_read(view + 48, 16)) == b'\xa5' * 16
    assert cpu.invoke('dh2_bres_open', [view, 0, 60]) == 1
    cpu.uc.mem_write(buf, raw)
    assert cpu.invoke('dh2_bres_open', [view, buf, len(raw)]) == 0
    output = cpu.data + 0x1000000
    cpu.uc.mem_write(output, b'\xcc' * 32)
    assert cpu.invoke('dh2_bres_fixups', [view, output, 0]) == 13
    assert bytes(cpu.uc.mem_read(output, 32)) == b'\xcc' * 32
    return len(cases) + 2

def synthetic_bres():
    """Valid miniature pointer graph exercises tables absent from the cache."""
    raw = bytearray(8192)
    root, cursor = 256, 1024
    fields = [24, 28, 32, 36, 40, root] + [root+p for _, p, _ in LAYOUTS]
    struct.pack_into('<15I', raw, 0, 0x53455242, 65534, 60, len(raw), len(fields),
                     0, 60, 60+len(fields)*4, root, root+192, len(raw), 0, 0, 0, 0)
    struct.pack_into('<'+'I'*len(fields), raw, 60, *fields)
    string = 60+len(fields)*4+4
    struct.pack_into('<I', raw, root, string)
    raw[string:string+10] = b'0,0,0,777\0'
    for count_at, pointer_at, stride in LAYOUTS:
        struct.pack_into('<I', raw, root+count_at, 3)
        struct.pack_into('<I', raw, root+pointer_at, cursor)
        cursor += stride*3
    return bytes(raw)

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--original', required=True, type=Path)
    p.add_argument('--assets', required=True, type=Path)
    p.add_argument('--ported', type=Path, default=ROOT / 'build/libresources_fixture_arm64.so')
    p.add_argument('--report', type=Path, default=ROOT.parents[1] / 'reports/engine-resources-validation.json')
    p.add_argument('--max-files', type=int, help='Smoke test only; omit for the full recovered corpus')
    a = p.parse_args()
    start = time.monotonic()
    provenance = json.loads((ROOT / 'original-functions.json').read_text())
    verify_original(a.original, provenance)
    old, new = Cpu(a.original, False, provenance), Cpu(a.ported, True, provenance)
    for symbol, size in [('test_memory_size', 32), ('test_limit_size', 48),
                          ('test_view_size', 48), ('test_capture_size', 32)]:
        assert new.invoke(symbol, []) == size, symbol
    check = Checks(old, new, provenance)
    streams(check)
    print(json.dumps({'stream_comparisons': check.comparisons}), flush=True)
    with tempfile.TemporaryDirectory(prefix='dh2-resources-') as directory:
        sample = Path(directory) / 'all-libraries.bdae'
        sample.write_bytes(synthetic_bres())
        synthetic = corpus(check, Path(directory), provenance)
    result = corpus(check, a.assets, provenance, a.max_files)
    malformed = malformed_and_guards(new, next(iter(sorted(a.assets.rglob('*.bdae')))))
    # Explicit safe difference: original negative seek is retained, but copies
    # before the buffer are rejected. Do not execute original undefined reads.
    mem, _ = check.stream_objects(new, 256)
    assert new.invoke('dh2_memory_seek', [mem, u32(-1), 0]) == 1
    assert new.invoke('dh2_memory_read', [mem, new.data + 0x6000, 1]) == 0
    coverage = []
    for row in provenance['functions']:
        addresses = set(range(int(row['elf_address'], 16), int(row['elf_address'], 16)+row['size'], 4))
        coverage.append({'original_symbol': row['original_symbol'], 'scope': row['scope'],
                         'instruction_addresses_executed': len(addresses & old.seen),
                         'range_instruction_addresses': len(addresses)})
    report = {'original_sha256': SHA256, 'complete_engine': False,
              'complete_functions_reconstructed': 35, 'partial_functions_reconstructed': 1,
              'stream_comparisons': check.comparisons, 'stream_cases': dict(check.calls),
              'bres_corpus': result, 'synthetic_bres': synthetic,
              'malformed_or_capacity_cases': malformed,
              'safe_negative_read_case': True, 'mismatches': 0,
              'arm64_pointers_above_4gib': True,
              'dependencies': 'Only imported libc copies/zeroing and the ARM32 caller completion fixture are modeled; engine routines execute original instructions.',
              'limitations': ['Whole-buffer relocation only; split allocations/external resource bases unimplemented.',
                              'SCollada library boundaries/strides recovered; nested payload schemas and rendering unimplemented.',
                              'Original constructors/string/shared ownership ABI unimplemented; port borrows buffers/names.',
                              'Safe bounds checks intentionally reject invalid original copies/indices.',
                              'No Android device execution or game integration for this component.'],
              'original_import_calls': dict(old.import_calls), 'arm64_import_calls': dict(new.import_calls),
              'coverage': coverage, 'elapsed_seconds': round(time.monotonic() - start, 2),
              'sha256': {str(f.relative_to(ROOT)): hashlib.sha256(f.read_bytes()).hexdigest()
                         for f in [ROOT/'resources.cpp', ROOT/'resources.hpp', ROOT/'original-functions.json',
                                   ROOT/'tests/cpu.py', ROOT/'tests/differential.py', ROOT/'tests/fixtures.cpp']}}
    report['sha256']['arm64_test_library'] = hashlib.sha256(a.ported.read_bytes()).hexdigest()
    production = ROOT/'build/libdh2_engine_resources_arm64.so'
    if production.exists():
        with production.open('rb') as stream:
            elf = ELFFile(stream)
            report['arm64_production_library'] = {
                'sha256': hashlib.sha256(production.read_bytes()).hexdigest(),
                'machine': elf['e_machine'],
                'load_segment_alignments': [s['p_align'] for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD'],
                'needed_libraries': [t.needed for t in elf.get_section_by_name('.dynamic').iter_tags()
                                     if t.entry.d_tag == 'DT_NEEDED']}
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'report': str(a.report.resolve()), 'stream_comparisons': check.comparisons,
                      'bres_files': result['files'], 'fixups': result['fixups'], 'mismatches': 0}), flush=True)

if __name__ == '__main__':
    main()
