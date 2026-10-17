#!/usr/bin/env python3
"""Pixel-level PVRTC fixtures plus a complete-cache decode audit."""

import argparse
import ctypes as c
import hashlib
import json
from pathlib import Path
import struct
import zipfile


def pvr(width, height, bpp, words):
    payload = b''.join(struct.pack('<II', modulation, color)
                       for modulation, color in words)
    expected = width * height * bpp // 8
    assert len(payload) == expected, (len(payload), expected)
    header = struct.pack('<13I', 52, height, width, 0, 24 if bpp == 2 else 25,
                         len(payload), bpp, 0, 0, 0, 0, 0x21525650, 1)
    return b'BTEXpvr\0' + header + payload


def word(a, b, modulation=0, mode=0):
    # The fixture colors are opaque 5-bit R/G/B values.
    def a_bits(rgb):
        r, g, blue = rgb
        assert blue in (0, 31)
        return 0x8000 | r << 10 | g << 5 | (blue >> 1) << 1 | mode

    def b_bits(rgb):
        r, g, blue = rgb
        return 0x8000 | r << 10 | g << 5 | blue

    return modulation, a_bits(a) | (b_bits(b) << 16)


RED, GREEN, BLUE, WHITE, BLACK = (31, 0, 0), (0, 31, 0), (0, 0, 31), (31, 31, 31), (0, 0, 0)


def decode(dll, raw, width, height, stride=None):
    stride = stride or width * 4
    # Four extra bytes serve as a write bound canary.
    output = c.create_string_buffer(b'\xA5' * (stride * height + 4))
    source = c.create_string_buffer(raw)
    before = hashlib.sha256(source.raw).digest()
    status = dll.dh2_texture_decode_rgba8(output, stride * height,
                                           stride, source, len(raw))
    assert status == 0, status
    assert output.raw[stride * height:stride * height + 4] == b'\xA5' * 4
    assert hashlib.sha256(source.raw).digest() == before
    return output.raw


def pixel(image, width, x, y, stride=None):
    stride = stride or width * 4
    return tuple(image[y * stride + 4 * x:y * stride + 4 * x + 4])


def fixtures(dll):
    red_blue = word(RED, BLUE)
    data = pvr(8, 8, 4, [red_blue] * 4)
    image = decode(dll, data, 8, 8, 40)
    assert all(pixel(image, 8, x, y, 40) == (255, 0, 0, 255)
               for y in range(8) for x in range(8))
    assert all(image[y * 40 + 32:y * 40 + 40] == b'\xA5' * 8 for y in range(8))
    image = decode(dll, pvr(8, 8, 4, [word(RED, BLUE, 0xFFFFFFFF)] * 4), 8, 8)
    assert pixel(image, 8, 3, 4) == (0, 0, 255, 255)
    # Punch-through code 2 averages A and B but forces transparent alpha.
    image = decode(dll, pvr(8, 8, 4, [word(RED, BLUE, 2, 1)] * 4), 8, 8)
    assert pixel(image, 8, 0, 0) == (127, 0, 127, 0)
    assert pixel(image, 8, 1, 0) == (255, 0, 0, 255)

    # Morton order for a 2x2 word grid is (0,0), (0,1), (1,0), (1,1).
    words = [word(RED, RED), word(GREEN, GREEN),
             word(BLUE, BLUE), word(WHITE, WHITE)]
    image = decode(dll, pvr(8, 8, 4, words), 8, 8)
    assert pixel(image, 8, 2, 2) == (255, 0, 0, 255)
    assert pixel(image, 8, 6, 2) == (0, 0, 255, 255)
    assert pixel(image, 8, 2, 6) == (0, 255, 0, 255)
    assert pixel(image, 8, 6, 6) == (255, 255, 255, 255)

    image = decode(dll, pvr(16, 8, 2, [word(RED, BLUE)] * 4), 16, 8)
    assert pixel(image, 16, 7, 4) == (255, 0, 0, 255)
    image = decode(dll, pvr(16, 8, 2, [word(RED, BLUE, 0xFFFFFFFF)] * 4), 16, 8)
    assert pixel(image, 16, 7, 4) == (0, 0, 255, 255)
    image = decode(dll, pvr(16, 8, 2, [word(RED, BLUE, 0, 1)] * 4), 16, 8)
    assert pixel(image, 16, 2, 1) == (255, 0, 0, 255)

    # At (2,1), horizontal neighbors are A and vertical neighbors are B.
    modulation = (3 << 2) | (3 << 18)
    horizontal = modulation | 1
    vertical = horizontal | (1 << 20)
    image = decode(dll, pvr(16, 8, 2, [word(RED, BLUE, horizontal, 1)] * 4), 16, 8)
    assert pixel(image, 16, 2, 1) == (255, 0, 0, 255)
    image = decode(dll, pvr(16, 8, 2, [word(RED, BLUE, vertical, 1)] * 4), 16, 8)
    assert pixel(image, 16, 2, 1) == (0, 0, 255, 255)
    image = decode(dll, pvr(16, 8, 2, [word(RED, BLUE, modulation, 1)] * 4), 16, 8)
    assert pixel(image, 16, 2, 1) == (127, 0, 127, 255)

    source = c.create_string_buffer(data)
    short = c.create_string_buffer(8 * 8 * 4)
    assert dll.dh2_texture_decode_rgba8(short, len(short) - 2, 32, source, len(data)) == 9
    assert dll.dh2_texture_decode_rgba8(short, len(short), 31, source, len(data)) == 9
    assert dll.dh2_texture_decode_rgba8(None, 0, 32, source, len(data)) == 9
    overlap = c.create_string_buffer(data + b'\0' * 300)
    assert dll.dh2_texture_decode_rgba8(overlap, 256, 32, overlap, len(data)) == 9
    assert dll.dh2_texture_decode_rgba8(short, len(short), 32, source, len(data) - 1) == 6


def entries(source):
    if source.is_dir():
        for path in sorted(source.rglob('*.tga')):
            yield path.relative_to(source).as_posix(), path.read_bytes()
    else:
        with zipfile.ZipFile(source) as archive:
            for name in sorted(n for n in archive.namelist() if n.lower().endswith('.tga')):
                yield name, archive.read(name)


def audit_cache(dll, source):
    dimensions = {}
    formats = {2: 0, 4: 0}
    combined = hashlib.sha256()
    count = 0
    for name, raw in entries(source):
        if not raw.startswith(b'BTEXpvr\0'):
            continue
        height, width = struct.unpack_from('<II', raw, 12)
        bpp = struct.unpack_from('<I', raw, 32)[0]
        rgba = decode(dll, raw, width, height)
        image_hash = hashlib.sha256(rgba[:width * height * 4]).digest()
        combined.update(name.encode('utf-8'))
        combined.update(b'\0')
        combined.update(image_hash)
        dimensions[f'{width}x{height}'] = dimensions.get(f'{width}x{height}', 0) + 1
        formats[bpp] += 1
        count += 1
    assert count == 234 and formats == {2: 17, 4: 217}, (count, formats)
    return {'decoded_files': count, 'formats': {'pvrtc1_2bpp': formats[2],
                                                'pvrtc1_4bpp': formats[4]},
            'dimensions': dimensions, 'combined_rgba_sha256': combined.hexdigest(),
            'scope': 'external BTEX/PVR v2 PVRTC1 textures in the complete cache'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--library', type=Path, required=True)
    p.add_argument('--cache-source', type=Path,
                   help='Complete extracted cache root or original cache ZIP')
    p.add_argument('--report', type=Path)
    a = p.parse_args()
    dll = c.CDLL(str(a.library.resolve()))
    dll.dh2_texture_decode_rgba8.argtypes = [c.c_void_p, c.c_size_t, c.c_size_t,
                                              c.c_void_p, c.c_size_t]
    dll.dh2_texture_decode_rgba8.restype = c.c_uint32
    fixtures(dll)
    print('PVRTC1 2bpp/4bpp pixel, mode, Morton and bounds fixtures passed.')
    if a.cache_source:
        report = audit_cache(dll, a.cache_source)
        print(json.dumps(report, indent=2))
        if a.report:
            a.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
