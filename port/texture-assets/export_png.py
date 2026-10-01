#!/usr/bin/env python3
"""Export one supported BTEX/PVR v2 PVRTC1 texture as an RGBA PNG."""

import argparse
import ctypes as c
import os
from pathlib import Path
import struct
import zlib


class View(c.Structure):
    _fields_ = [
        ('bytes', c.c_void_p), ('size', c.c_size_t),
        ('payload', c.c_void_p), ('payload_size', c.c_size_t), ('payload_offset', c.c_size_t),
        ('width', c.c_uint32), ('height', c.c_uint32),
        ('flags', c.c_uint32), ('bits_per_pixel', c.c_uint32),
        ('mipmaps', c.c_uint32), ('surfaces', c.c_uint32), ('alpha_mask', c.c_uint32),
        ('tga_descriptor', c.c_uint32), ('kind', c.c_uint32), ('format', c.c_uint32),
    ]


def chunk(kind, payload):
    return (struct.pack('>I', len(payload)) + kind + payload
            + struct.pack('>I', zlib.crc32(kind + payload)))


def export(library, input_path, output_path, max_pixels):
    raw = input_path.read_bytes()
    dll = c.CDLL(str(library.resolve()))
    dll.dh2_texture_open.argtypes = [c.POINTER(View), c.c_void_p, c.c_size_t]
    dll.dh2_texture_open.restype = c.c_uint32
    dll.dh2_texture_decode_rgba8.argtypes = [c.c_void_p, c.c_size_t, c.c_size_t,
                                              c.c_void_p, c.c_size_t]
    dll.dh2_texture_decode_rgba8.restype = c.c_uint32
    source = c.create_string_buffer(raw)
    view = View()
    status = dll.dh2_texture_open(c.byref(view), source, len(raw))
    if status != 0:
        raise ValueError(f'texture parse failed (error {status})')
    if view.format not in (1, 2):
        raise ValueError('only PVRTC1 2bpp/4bpp is supported by this exporter')
    if view.width * view.height > max_pixels:
        raise ValueError(f'image exceeds {max_pixels:,} pixel export limit')
    stride = view.width * 4
    pixels = c.create_string_buffer(stride * view.height)
    status = dll.dh2_texture_decode_rgba8(pixels, len(pixels), stride, source, len(raw))
    if status != 0:
        raise ValueError(f'PVRTC decode failed (error {status})')
    compressor = zlib.compressobj(level=6)
    compressed = bytearray()
    for y in range(view.height):
        compressed.extend(compressor.compress(b'\0' + pixels.raw[y * stride:(y + 1) * stride]))
    compressed.extend(compressor.flush())
    ihdr = struct.pack('>IIBBBBB', view.width, view.height, 8, 6, 0, 0, 0)
    png = b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', ihdr)
    png += chunk(b'IDAT', compressed) + chunk(b'IEND', b'')
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_bytes(png)
    print(f'{view.width}x{view.height} RGBA PNG: {output_path}')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('input', type=Path)
    p.add_argument('output', type=Path)
    library_name = 'dh2_texture_assets.dll' if os.name == 'nt' else 'libdh2_texture_assets.so'
    p.add_argument('--library', type=Path, default=Path(__file__).parent / 'build' / library_name)
    p.add_argument('--max-pixels', type=int, default=16_777_216)
    a = p.parse_args()
    if a.max_pixels <= 0:
        p.error('--max-pixels must be positive')
    export(a.library, a.input, a.output, a.max_pixels)


if __name__ == '__main__':
    main()
