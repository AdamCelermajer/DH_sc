#!/usr/bin/env python3
"""Check malformed fixtures and every external texture in the complete ZIP."""

import argparse
from collections import Counter
import ctypes as c
import hashlib
import json
from pathlib import Path
import struct
import zlib
import zipfile


class View(c.Structure):
    _fields_ = [
        ('bytes', c.c_void_p), ('size', c.c_size_t),
        ('payload', c.c_void_p), ('payload_size', c.c_size_t), ('payload_offset', c.c_size_t),
        ('width', c.c_uint32), ('height', c.c_uint32),
        ('flags', c.c_uint32), ('bits_per_pixel', c.c_uint32),
        ('mipmaps', c.c_uint32), ('surfaces', c.c_uint32), ('alpha_mask', c.c_uint32),
        ('tga_descriptor', c.c_uint32), ('kind', c.c_uint32), ('format', c.c_uint32),
    ]


ERROR = {'ok': 0, 'null': 1, 'short': 2, 'unknown': 3, 'header': 4,
         'extent': 5, 'payload': 6, 'trailing': 7, 'unsupported': 8}
KIND = {'btex': 1, 'pvr': 2, 'tga': 3, 'png': 4}
FORMAT = {'pvrtc2': 1, 'pvrtc4': 2, 'bgra8': 3, 'png': 4}


def open_bytes(dll, raw):
    buffer = c.create_string_buffer(bytes(raw))
    view = View()
    error = dll.dh2_texture_open(c.byref(view), buffer, len(raw))
    return error, view, buffer


def pvr(width=8, height=8, typ=25, bpp=4, mipmaps=0, surfaces=1):
    # Independently construct the legacy header using its documented field order.
    data = bytes((i * 37) & 255 for i in range(max(width, 8) * max(height, 8) * bpp // 8))
    header = struct.pack('<13I', 52, height, width, mipmaps, typ,
                         len(data), bpp, 0, 0, 0, 0, 0x21525650, surfaces)
    return b'BTEXpvr\0' + header + data


def tga(width=2, height=2, descriptor=0x28, footer=False):
    header = struct.pack('<BBBHHBHHHHBB', 0, 0, 2, 0, 0, 0,
                         0, 0, width, height, 32, descriptor)
    pixels = bytes(range(width * height * 4))
    return header + pixels + (b'\0' * 8 + b'TRUEVISION-XFILE.\0' if footer else b'')


def png():
    def chunk(typ, data):
        return struct.pack('>I', len(data)) + typ + data + struct.pack('>I', zlib.crc32(typ + data))
    ihdr = struct.pack('>IIBBBBB', 1, 1, 8, 6, 0, 0, 0)
    return b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', ihdr) + chunk(b'IDAT', zlib.compress(b'\0\xff\0\0\xff')) + chunk(b'IEND', b'')


def fixtures(dll):
    def check(raw, expected, kind=0):
        before = hashlib.sha256(raw).digest()
        error, view, buffer = open_bytes(dll, raw)
        assert error == expected, (error, expected)
        if kind:
            assert view.kind == kind, (view.kind, kind)
        assert hashlib.sha256(buffer.raw[:len(raw)]).digest() == before
        return view

    raw = pvr()
    view = check(raw, ERROR['ok'], KIND['btex'])
    assert (view.width, view.height, view.payload_offset, view.payload_size, view.format) == (8, 8, 60, 32, FORMAT['pvrtc4'])
    check(raw[8:], ERROR['ok'], KIND['pvr'])
    check(raw[:8], ERROR['short'])
    check(raw[:60], ERROR['payload'])
    check(raw + b'!', ERROR['trailing'])
    damaged = bytearray(raw); damaged[8 + 44] ^= 1
    check(damaged, ERROR['header'])
    damaged = bytearray(raw); struct.pack_into('<I', damaged, 8 + 8, 0)
    check(damaged, ERROR['extent'])
    damaged = bytearray(raw); struct.pack_into('<I', damaged, 8 + 20, 4096)
    check(damaged, ERROR['payload'])
    unsupported = bytearray(raw); struct.pack_into('<I', unsupported, 8 + 16, 7)
    assert check(unsupported, ERROR['unsupported'], KIND['btex']).flags == 7
    check(pvr(mipmaps=1), ERROR['unsupported'], KIND['btex'])
    check(pvr(surfaces=2), ERROR['unsupported'], KIND['btex'])
    check(pvr(typ=24, bpp=2, width=16), ERROR['ok'], KIND['btex'])

    raw = tga(footer=True)
    view = check(raw, ERROR['ok'], KIND['tga'])
    assert (view.format, view.payload_offset, view.payload_size, view.tga_descriptor) == (FORMAT['bgra8'], 18, 16, 0x28)
    check(tga(), ERROR['ok'], KIND['tga'])
    check(tga() + b'garbage', ERROR['trailing'])
    check(tga()[:20], ERROR['payload'])
    damaged = bytearray(tga()); damaged[2] = 10
    check(damaged, ERROR['unsupported'], KIND['tga'])
    raw = png()
    view = check(raw, ERROR['ok'], KIND['png'])
    assert (view.format, view.width, view.height) == (FORMAT['png'], 1, 1)
    damaged = bytearray(raw); damaged[29] ^= 1
    check(damaged, ERROR['payload'])
    check(raw + b'!', ERROR['trailing'])
    check(b'?' * 32, ERROR['unknown'])
    assert dll.dh2_texture_open(None, None, 0) == ERROR['null']
    view = View()
    assert dll.dh2_texture_open(c.byref(view), None, 0) == ERROR['null']


def audit_zip(dll, path):
    counts, pvr_flags, pvr_formats, dimensions = Counter(), Counter(), Counter(), Counter()
    checked = 0
    with zipfile.ZipFile(path) as z:
        for info in z.infolist():
            if info.is_dir() or not info.filename.lower().endswith(('.tga', '.png')):
                continue
            raw = z.read(info)
            error, view, buffer = open_bytes(dll, raw)
            assert error == ERROR['ok'], (info.filename, error)
            assert view.size == len(raw) and view.bytes == c.addressof(buffer)
            assert view.payload == c.addressof(buffer) + view.payload_offset
            assert view.payload_offset + view.payload_size <= len(raw)
            extension = Path(info.filename).suffix.lower()
            if raw.startswith(b'BTEXpvr\0'):
                assert extension == '.tga' and view.kind == KIND['btex']
                h = struct.unpack_from('<13I', raw, 8)
                assert h[0] == 52 and h[11] == 0x21525650
                assert (view.width, view.height, view.payload_size) == (h[2], h[1], h[5])
                assert len(raw) == 60 + h[5]
                assert h[3] == 0 and h[12] == 1
                if h[4] & 255 == 24:
                    assert h[6] == 2 and h[5] == max(h[2], 16) * max(h[1], 8) // 4
                    assert view.format == FORMAT['pvrtc2']
                else:
                    assert h[4] & 255 == 25 and h[6] == 4
                    assert h[5] == max(h[2], 8) * max(h[1], 8) // 2
                    assert view.format == FORMAT['pvrtc4']
                pvr_flags[str(h[4])] += 1
                pvr_formats['pvrtc_2bpp' if h[4] & 255 == 24 else 'pvrtc_4bpp'] += 1
                counts['btex_pvr_v2'] += 1
            elif extension == '.tga':
                assert view.kind == KIND['tga'] and view.format == FORMAT['bgra8']
                assert raw[2] == 2 and raw[16] == 32
                assert (view.width, view.height) == struct.unpack_from('<HH', raw, 12)
                assert view.payload_size == 4 * view.width * view.height
                counts['tga_bgra8'] += 1
            else:
                assert extension == '.png' and raw.startswith(b'\x89PNG\r\n\x1a\n')
                assert view.kind == KIND['png'] and view.format == FORMAT['png']
                assert (view.width, view.height) == struct.unpack_from('>II', raw, 16)
                counts['png_encoded'] += 1
            dimensions[f'{view.width}x{view.height}'] += 1
            checked += 1
    assert dict(counts) == {'btex_pvr_v2': 234, 'tga_bgra8': 8, 'png_encoded': 121}, counts
    with path.open('rb') as source:
        sha = hashlib.file_digest(source, 'sha256').hexdigest()
    return {'all_checks_passed': True,
            'archive_sha256': sha,
            'texture_files_checked': checked,
            'containers': dict(counts),
            'pvr_formats': dict(pvr_formats),
            'pvr_raw_flags': dict(sorted(pvr_flags.items())),
            'dimensions': dict(sorted(dimensions.items())),
            'unsupported_cache_formats': [],
            'scope': 'external .tga and .png files in complete owner-supplied cache ZIP; encoded payload views only',
            'not_validated': ['PVRTC pixel decode and upload', 'PNG decompression/pixels',
                              'BRES image references and materials', 'renderer or gameplay']}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--library', type=Path, required=True)
    p.add_argument('--cache-zip', type=Path)
    p.add_argument('--report', type=Path)
    a = p.parse_args()
    dll = c.CDLL(str(a.library.resolve()))
    dll.dh2_texture_open.argtypes = [c.POINTER(View), c.c_void_p, c.c_size_t]
    dll.dh2_texture_open.restype = c.c_uint32
    fixtures(dll)
    print('Synthetic format, bounds, corruption and immutability checks passed.')
    if a.cache_zip:
        report = audit_zip(dll, a.cache_zip)
        print(json.dumps(report, indent=2))
        if a.report:
            a.report.parent.mkdir(parents=True, exist_ok=True)
            a.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
