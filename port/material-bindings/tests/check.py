#!/usr/bin/env python3
"""Exercise checked record boundaries with a real BRES fixture and corruption."""

import argparse
import ctypes as c
from pathlib import Path
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from audit_cache import Bres, Image, Material, bind  # noqa: E402


def opened(dll, raw):
    buffer = c.create_string_buffer(raw)
    view = Bres()
    assert dll.dh2_bres_open(c.byref(view), buffer, len(raw)) == 0
    return buffer, view


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--fixture', type=Path, required=True)
    args = parser.parse_args()
    dll = bind(args.library)
    raw = args.fixture.read_bytes()
    buffer, view = opened(dll, raw)
    assert buffer and view.bytes
    image = Image()
    assert dll.dh2_image_record(c.byref(image), c.byref(view), 0) == 0
    assert dll.dh2_image_record(c.byref(image), c.byref(view), -1) == 2
    assert dll.dh2_image_record(c.byref(image), c.byref(view), 999999) == 2
    assert dll.dh2_image_record(None, c.byref(view), 0) == 1

    # A BRES-valid pointer that lacks a terminator must be rejected by the
    # record view instead of producing an unbounded native string read.
    corrupt = bytearray(raw)
    root = struct.unpack_from('<I', corrupt, 32)[0]
    image_table = struct.unpack_from('<I', corrupt, root + 0x50)[0]
    struct.pack_into('<I', corrupt, image_table, len(corrupt) - 1)
    corrupt[-1] = ord('X')
    corrupt_buffer, corrupt_view = opened(dll, bytes(corrupt))
    assert corrupt_buffer
    assert dll.dh2_image_record(c.byref(image), c.byref(corrupt_view), 0) == 3

    for index in range(dll.dh2_bres_library_count(c.byref(view), 6)):
        material = Material()
        assert dll.dh2_material_record(c.byref(material), c.byref(view), index) == 0
        result = dll.dh2_material_local_effect(c.byref(material))
        assert (result == -1) == bool(material.external_effect_file)
    print('material-binding boundary checks passed')


if __name__ == '__main__':
    main()
