#!/usr/bin/env python3
"""Compare decoded cache pixels to a locally built PowerVR SDK oracle DLL."""

import argparse
import ctypes as c
import hashlib
import json
import os
from pathlib import Path
import struct

from decode import entries


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--library', type=Path, required=True)
    p.add_argument('--reference-library', type=Path, required=True)
    p.add_argument('--cache-source', type=Path, required=True)
    p.add_argument('--dependency-dir', type=Path,
                   help='Windows directory containing the oracle compiler runtime DLLs')
    p.add_argument('--reference-source', type=Path,
                   help='PowerVR SDK PVRTDecompress.cpp used to build the oracle')
    p.add_argument('--report', type=Path)
    a = p.parse_args()
    dependency_handle = os.add_dll_directory(str(a.dependency_dir.resolve())) \
        if a.dependency_dir and hasattr(os, 'add_dll_directory') else None
    ours = c.CDLL(str(a.library.resolve()))
    ours.dh2_texture_decode_rgba8.argtypes = [c.c_void_p, c.c_size_t, c.c_size_t,
                                               c.c_void_p, c.c_size_t]
    ours.dh2_texture_decode_rgba8.restype = c.c_uint32
    reference = c.CDLL(str(a.reference_library.resolve()))
    reference.dh2_reference_pvrtc.argtypes = [c.c_void_p, c.c_uint32, c.c_uint32,
                                               c.c_uint32, c.c_void_p]
    reference.dh2_reference_pvrtc.restype = c.c_uint32
    matched = 0
    combined = hashlib.sha256()
    for name, raw in entries(a.cache_source):
        if not raw.startswith(b'BTEXpvr\0'):
            continue
        height, width = struct.unpack_from('<II', raw, 12)
        bpp = struct.unpack_from('<I', raw, 32)[0]
        source = c.create_string_buffer(raw)
        actual = c.create_string_buffer(width * height * 4)
        expected = c.create_string_buffer(width * height * 4)
        status = ours.dh2_texture_decode_rgba8(actual, len(actual), width * 4,
                                                source, len(raw))
        assert status == 0, (name, status)
        decoded_bytes = reference.dh2_reference_pvrtc(
            c.c_void_p(c.addressof(source) + 60), int(bpp == 2),
            width, height, expected)
        if decoded_bytes != len(raw) - 60:
            raise AssertionError(f'{name}: reference consumed {decoded_bytes} '
                                 f'bytes, expected {len(raw) - 60}')
        if actual.raw != expected.raw:
            offset = next(i for i, pair in enumerate(zip(actual.raw, expected.raw))
                          if pair[0] != pair[1])
            raise AssertionError(f'{name}: first differing RGBA byte {offset}, '
                                 f'ours={actual.raw[offset]}, reference={expected.raw[offset]}')
        combined.update(name.encode('utf-8') + b'\0'
                        + hashlib.sha256(actual.raw).digest())
        matched += 1
    assert matched == 234, matched
    if dependency_handle:
        dependency_handle.close()
    report = {
        'matched_textures': matched,
        'mismatched_textures': 0,
        'comparison': 'byte-for-byte RGBA8 output against PowerVR Native_SDK PVRTDecompressPVRTC',
        'combined_rgba_sha256': combined.hexdigest(),
        'reference_url': 'https://github.com/powervr-graphics/Native_SDK/blob/master/framework/PVRCore/texture/PVRTDecompress.cpp',
    }
    if a.reference_source:
        report['reference_source_sha256'] = hashlib.sha256(a.reference_source.read_bytes()).hexdigest()
    if a.report:
        a.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'{matched} PVRTC1 textures matched the PowerVR SDK oracle byte for byte; '
          f'combined RGBA SHA-256 {combined.hexdigest()}')


if __name__ == '__main__':
    main()
