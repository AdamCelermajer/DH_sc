#!/usr/bin/env python3
"""Inspect recovered BRES files through the compiled reconstruction, read-only."""
import argparse
import ctypes
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
NAMES = ['animation', 'animation_clip', 'camera', 'light', 'image', 'effect',
         'material', 'geometry', 'controller', 'emitter', 'gnps_emitter', 'force', 'coronas']
STRIDES = [32, 12, 28, 24, 20, 116, 36, 16, 12, 144, 232, 16, 36]
ERRORS = ['ok', 'null_input', 'short_header', 'magic', 'byte_order', 'already_relocated',
          'header_size', 'file_size', 'external_base', 'fixup_table', 'fixup_field',
          'fixup_target', 'root', 'capacity', 'library', 'string']

class View(ctypes.Structure):
    _fields_ = [('bytes', ctypes.c_void_p), ('size', ctypes.c_size_t),
                *[(name, ctypes.c_uint32) for name in
                  ['fixup_count', 'fixup_offset', 'root_offset', 'tail_offset',
                   'bulk_size', 'block_count', 'tail_size']]]

def load_library(path):
    library = ctypes.CDLL(str(path.resolve()))
    library.dh2_bres_open.argtypes = [ctypes.POINTER(View), ctypes.c_void_p, ctypes.c_size_t]
    library.dh2_bres_open.restype = ctypes.c_uint32
    library.dh2_bres_library_count.argtypes = [ctypes.POINTER(View), ctypes.c_uint32]
    library.dh2_bres_library_count.restype = ctypes.c_uint32
    library.dh2_bres_library_item.argtypes = [ctypes.POINTER(View), ctypes.c_uint32, ctypes.c_int32]
    library.dh2_bres_library_item.restype = ctypes.c_void_p
    library.dh2_bres_version.argtypes = [ctypes.POINTER(View)]
    library.dh2_bres_version.restype = ctypes.c_void_p
    return library

def inspect(path, library):
    data = path.read_bytes()
    buffer = ctypes.create_string_buffer(data)
    view = View()
    error = library.dh2_bres_open(ctypes.byref(view), buffer, len(data))
    result = {'path': str(path), 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
              'status': ERRORS[error] if error < len(ERRORS) else str(error)}
    if error:
        return result
    version = library.dh2_bres_version(ctypes.byref(view))
    result.update({'version': ctypes.string_at(version).decode('utf-8') if version else None,
                   'fixups': view.fixup_count, 'root_offset': view.root_offset,
                   'bulk_bytes': view.bulk_size, 'bulk_blocks': view.block_count,
                   'tail_bytes': view.tail_size})
    tables = {}
    for kind, name in enumerate(NAMES):
        n = library.dh2_bres_library_count(ctypes.byref(view), kind)
        first = library.dh2_bres_library_item(ctypes.byref(view), kind, 0)
        tables[name] = {'count': n, 'stride_bytes': STRIDES[kind],
                        'offset': first - view.bytes if first else None}
    result['libraries'] = tables
    return result

def main():
    p = argparse.ArgumentParser()
    p.add_argument('paths', type=Path, nargs='+', help='Files or directories of recovered .bdae files')
    p.add_argument('--library', type=Path, default=ROOT/'build/libdh2_engine_resources_host.so')
    p.add_argument('--output', type=Path, help='Optional UTF-8 JSON-lines catalog')
    a = p.parse_args()
    library = load_library(a.library)
    paths = sorted({file for path in a.paths for file in
                    (path.rglob('*.bdae') if path.is_dir() else [path])})
    entries = [inspect(path, library) for path in paths]
    if a.output:
        a.output.parent.mkdir(parents=True, exist_ok=True)
        a.output.write_text(''.join(json.dumps(row, sort_keys=True) + '\n' for row in entries))
        print(json.dumps({'files': len(entries), 'valid': sum(e['status'] == 'ok' for e in entries),
                          'output': str(a.output.resolve())}))
    else:
        print(json.dumps(entries[0] if len(entries) == 1 else entries, indent=2))

if __name__ == '__main__':
    main()
