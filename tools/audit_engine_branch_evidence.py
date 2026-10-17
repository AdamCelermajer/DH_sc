"""Compare an engine branch and verify its explicit ELF-range hash records.

Read-only Git inspection. Byte provenance does not prove source equivalence,
runtime execution, or complete game/engine reconstruction.
"""
import argparse
import collections
import hashlib
import json
import pathlib
import struct
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]
ORIGINAL_SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
ADDRESS_KEYS = ('elf_va', 'elf_address', 'elf_virtual_address', 'va', 'address')
SIZE_KEYS = ('range_size_bytes', 'range_size', 'elf_size_bytes', 'size_bytes',
             'length_bytes', 'size')
HASH_KEYS = ('original_code_sha256', 'function_bytes_sha256',
             'function_slice_sha256', 'vtable_slice_sha256', 'data_bytes_sha256',
             'range_sha256', 'byte_sha256', 'bytes_sha256', 'arm_bytes_sha256',
             'sha256')


def git(*args, stdin=None):
    return subprocess.check_output(['git', *args], cwd=ROOT, input=stdin)


def commit(ref):
    return git('rev-parse', '--verify', '--end-of-options', ref+'^{commit}').decode().strip()


def tree(ref):
    result = {}
    for entry in git('ls-tree', '-r', '-z', ref).split(b'\0'):
        if not entry:
            continue
        metadata, name = entry.split(b'\t', 1)
        mode, kind, oid = metadata.split()
        if kind == b'blob':
            result[name.decode('utf-8')] = oid.decode()
    return result


def integer(value):
    if isinstance(value, bool):
        raise ValueError('boolean is not a range coordinate')
    return int(value, 0) if isinstance(value, str) else int(value)


def objects(value, pointer='$'):
    if isinstance(value, dict):
        yield pointer, value
        for key, child in value.items():
            yield from objects(child, pointer+'/'+str(key))
    elif isinstance(value, list):
        for index, child in enumerate(value):
            yield from objects(child, pointer+'/'+str(index))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--current-ref', required=True)
    parser.add_argument('--branch-ref', required=True)
    parser.add_argument('--original-elf', type=pathlib.Path, required=True)
    parser.add_argument('--output', type=pathlib.Path, required=True)
    args = parser.parse_args()
    current, branch = commit(args.current_ref), commit(args.branch_ref)
    base = git('merge-base', current, branch).decode().strip()
    current_tree, branch_tree, base_tree = tree(current), tree(branch), tree(base)
    elf = args.original_elf.read_bytes()
    if hashlib.sha256(elf).hexdigest() != ORIGINAL_SHA:
        raise ValueError('original ELF hash does not match the pinned game')
    if elf[:7] != b'\x7fELF\x01\x01\x01' or struct.unpack_from('<H', elf, 18)[0] != 40:
        raise ValueError('expected little-endian ELF32 ARM original')
    phoff = struct.unpack_from('<I', elf, 28)[0]
    phsize, phcount = struct.unpack_from('<HH', elf, 42)
    segments = []
    for index in range(phcount):
        kind, offset, va, _, filesz, _, _, _ = struct.unpack_from('<8I', elf, phoff+index*phsize)
        if kind == 1:
            segments.append((va, offset, filesz))

    paths, ranges, errors, shape_skips = [], [], [], collections.Counter()
    for name, oid in sorted(branch_tree.items()):
        if base_tree.get(name) == oid:
            continue
        raw = git('cat-file', 'blob', oid)
        state = ('missing' if name not in current_tree else
                 'identical' if current_tree[name] == oid else 'different')
        paths.append({'path': name, 'comparison': state, 'branch_blob': oid,
                      'current_blob': current_tree.get(name), 'bytes': len(raw),
                      'sha256': hashlib.sha256(raw).hexdigest(),
                      'generated_bytecode': '__pycache__' in name or name.endswith('.pyc')})
        if not name.endswith('.json'):
            continue
        document = json.loads(raw)
        for pointer, row in objects(document):
            address_key = next((key for key in ADDRESS_KEYS if key in row), None)
            size_key = next((key for key in SIZE_KEYS if key in row), None)
            hash_key = next((key for key in HASH_KEYS if isinstance(row.get(key), str)
                             and len(row[key]) == 64), None)
            if not (address_key and size_key and hash_key):
                if ('address_point_va' in row and size_key and hash_key
                        and not address_key):
                    shape_skips['address_point_without_table_base'] += 1
                continue
            try:
                va, size = integer(row[address_key]), integer(row[size_key])
                if size <= 0:
                    raise ValueError('nonpositive byte range')
                segment = next((seg for seg in segments
                                if seg[0] <= va and va+size <= seg[0]+seg[2]), None)
                if segment is None:
                    raise ValueError('range is not contained in a file-backed PT_LOAD')
                offset = segment[1]+va-segment[0]
                actual = hashlib.sha256(elf[offset:offset+size]).hexdigest()
                expected = row[hash_key].lower()
                record = {'manifest': name, 'json_pointer': pointer,
                          'address': hex(va), 'size': size, 'file_offset': offset,
                          'sha256': expected, 'hash_key': hash_key,
                          'matches_original': actual == expected}
                ranges.append(record)
                if actual != expected:
                    errors.append({**record, 'actual_sha256': actual})
            except (ValueError, TypeError) as error:
                errors.append({'manifest': name, 'json_pointer': pointer,
                               'error': str(error)})
    result = {
        'schema': 'dh2-engine-branch-audit/v1',
        'validation': 'PASS' if not errors else 'FAIL',
        'current_commit': current, 'branch_commit': branch, 'merge_base': base,
        'original_library_sha256': ORIGINAL_SHA,
        'path_counts': dict(collections.Counter(row['comparison'] for row in paths)),
        'generated_bytecode_files': sum(row['generated_bytecode'] for row in paths),
        'range_records_checked': len(ranges),
        'unique_address_size_ranges': len({(row['address'], row['size']) for row in ranges}),
        'metadata_shapes_not_treated_as_ranges': dict(shape_skips),
        'errors': errors, 'paths': paths, 'range_checks': ranges,
        'scope': 'Git blob identities and explicit original ELF byte-range hashes only. '
                 'Address-point-only metadata is not treated as a table base. '
                 'Corpus semantics, source behavior, live wiring and game completion '
                 'require their own verification. No source/import mutation occurs.'
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({key: result[key] for key in ('validation', 'path_counts',
                     'generated_bytecode_files', 'range_records_checked',
                     'unique_address_size_ranges')}))
    if errors:
        print(json.dumps(errors[:5], indent=2))
        raise SystemExit(1)


if __name__ == '__main__':
    main()
