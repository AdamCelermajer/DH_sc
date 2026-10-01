#!/usr/bin/env python3
"""Exercise checked record boundaries with a real BRES fixture and corruption."""

import argparse
import ctypes as c
from pathlib import Path
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from audit_cache import Bres, Effect, EffectGroup, Image, ImageRef, Material, Parameter, bind  # noqa: E402


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
        for parameter_index in range(material.parameter_count):
            parameter = Parameter()
            assert dll.dh2_material_parameter(c.byref(parameter), c.byref(material), parameter_index) == 0
            reference = ImageRef()
            status = dll.dh2_material_sampler_image(c.byref(reference), c.byref(material), parameter_index)
            assert status == (0 if parameter.type_code == 11 else 5)

    effect = Effect()
    assert dll.dh2_effect_record(c.byref(effect), c.byref(view), 0) == 0
    group = EffectGroup()
    assert dll.dh2_effect_group(c.byref(group), c.byref(effect), 0) == 0
    assert dll.dh2_effect_group(c.byref(group), c.byref(effect), 2) == 2
    assert dll.dh2_effect_group(c.byref(group), c.byref(effect), 1) == 0
    assert dll.dh2_effect_group_image(c.byref(ImageRef()), c.byref(group), -1) == 2

    # The nested arrays and the sampler's extra indirection must stay inside
    # the borrowed image. Mutations preserve the top-level BRES header.
    material = Material()
    assert dll.dh2_material_record(c.byref(material), c.byref(view), 0) == 0
    bad_array = bytearray(raw)
    struct.pack_into('<I', bad_array, material.record - view.bytes + 20, len(raw) - 4)
    bad_buffer, bad_view = opened(dll, bytes(bad_array))
    assert bad_buffer
    assert dll.dh2_material_record(c.byref(Material()), c.byref(bad_view), 0) == 4

    sampler_index = next(j for j in range(material.parameter_count)
                         if read_parameter(dll, material, j).type_code == 11)
    parameter = read_parameter(dll, material, sampler_index)
    image_index_offset = struct.unpack_from('<I', raw, parameter.raw_value - view.bytes)[0]
    bad_sampler = bytearray(raw)
    struct.pack_into('<I', bad_sampler, image_index_offset,
                     dll.dh2_bres_library_count(c.byref(view), 4))
    sampler_buffer, sampler_view = opened(dll, bytes(bad_sampler))
    assert sampler_buffer
    changed = Material()
    assert dll.dh2_material_record(c.byref(changed), c.byref(sampler_view), 0) == 0
    assert dll.dh2_material_sampler_image(c.byref(ImageRef()), c.byref(changed), sampler_index) == 4

    bad_effect = bytearray(raw)
    struct.pack_into('<I', bad_effect, effect.record - view.bytes + 40, len(raw))
    effect_buffer, effect_view = opened(dll, bytes(bad_effect))
    assert effect_buffer
    changed_effect = Effect()
    assert dll.dh2_effect_record(c.byref(changed_effect), c.byref(effect_view), 0) == 0
    assert dll.dh2_effect_group(c.byref(EffectGroup()), c.byref(changed_effect), 1) == 4
    print('material-binding boundary checks passed')


def read_parameter(dll, material, index):
    value = Parameter()
    assert dll.dh2_material_parameter(c.byref(value), c.byref(material), index) == 0
    return value


if __name__ == '__main__':
    main()
