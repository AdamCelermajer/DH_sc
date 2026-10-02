#!/usr/bin/env python3
"""Check the nine observed type-1 BRES records without treating them as runtime meshes."""
import argparse
import ctypes as c
import hashlib
import json
import math
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from api import Attribute, Input, Mesh, Primitive, Type1Geometry, bind

FILES = (
    'data/3d/characters/dragon/dragon.bdae',
    'data/3d/interface/skill_dh2_monster_dragon_attack_02.bdae',
    'data/3d/interface/skill_dh2_monster_dragon_attack_03.bdae',
    'data/3d/interface/skill_dh2_monster_dragon_intimidate.bdae',
    'data/3d/interface/skill_dh2_monster_dragon_intimidate_01.bdae',
    'data/3d/interface/skill_dh2_monster_dragon_intimidate_01b.bdae',
    'data/3d/interface/skill_dh2_monster_dragon_intimidate_02.bdae',
    'data/3d/interface/skill_dh2_monster_dragon_intimidate_03.bdae',
    'data/3d/interface/spell_dh2_splined_projectile.bdae',
)


def u(raw, offset):
    return struct.unpack_from('<I', raw, offset)[0]


def opened(dll, raw):
    data = Input(dll, bytes(raw))
    value = Type1Geometry()
    assert dll.dh2_mesh_open(c.byref(Mesh()), c.byref(data.view), 0) == 3
    assert dll.dh2_type1_geometry_open(c.byref(value), c.byref(data.view), 0) == 0
    return data, value


def reject(dll, raw, expected):
    data = Input(dll, bytes(raw))
    value = Type1Geometry()
    error = dll.dh2_type1_geometry_open(c.byref(value), c.byref(data.view), 0)
    assert error == expected, (error, expected)
    assert value.embedded_mesh.vertices == 0


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    dll = bind(args.library)
    checked = []
    vertices = triangles = scalar_checks = index_checks = 0
    for relative in FILES:
        raw = (args.cache / relative).read_bytes()
        data, value = opened(dll, raw)
        mesh = value.embedded_mesh
        assert list(value.opaque_header) == [0, 15, 3, 0, 0]
        assert mesh.stride == 32 and mesh.attributes == 3 and mesh.primitives == 1
        assert mesh.vertices in (24, 52, 78)
        assert dll.dh2_bres_library_count(c.byref(data.view), 7) >= 1
        assert dll.dh2_type1_geometry_open(c.byref(Type1Geometry()), c.byref(data.view), 9999) == 2
        attributes = []
        for i, components in enumerate((3, 3, 2)):
            attr = Attribute()
            assert dll.dh2_mesh_attribute(c.byref(mesh), i, c.byref(attr)) == 0
            assert (attr.type, attr.components, attr.stride, attr.vertices) == (6, components, 32, mesh.vertices)
            attributes.append(attr)
            for k in range(mesh.vertices):
                expected = struct.unpack('<' + 'f' * components, c.string_at(attr.data + k * attr.stride, components * 4))
                actual = (c.c_float * 16)()
                assert dll.dh2_attribute_read(c.byref(attr), k, actual)
                assert tuple(actual[:components]) == expected
                assert all(math.isfinite(x) for x in expected)
                if i == 0:
                    assert all(mesh.minimum[j] <= expected[j] <= mesh.maximum[j] for j in range(3))
                scalar_checks += components
            assert not dll.dh2_attribute_read(c.byref(attr), mesh.vertices, (c.c_float * 16)())
        primitive = Primitive()
        assert dll.dh2_mesh_primitive(c.byref(mesh), 0, c.byref(primitive)) == 0
        assert primitive.collada_type == 0 and primitive.index_width == 2
        assert primitive.index_count == primitive.declared_count * 3
        assert list(primitive.attributes[:5]) == [0, 1, -1, -1, 2]
        for k in range(primitive.index_count):
            expected = struct.unpack('<H', c.string_at(primitive.indices + k * 2, 2))[0]
            actual = c.c_uint32()
            assert dll.dh2_index_read(c.byref(primitive), k, c.byref(actual))
            assert actual.value == expected
            assert primitive.minimum_index <= expected <= primitive.maximum_index < mesh.vertices
            index_checks += 1
        vertices += mesh.vertices
        triangles += primitive.declared_count
        checked.append({'file': relative, 'sha256': hashlib.sha256(raw).hexdigest(),
                        'opaque_header': list(value.opaque_header), 'vertices': mesh.vertices,
                        'triangles': primitive.declared_count})

    # Corrupt the owner's first type-1 record in memory. These are schema
    # boundary checks; no modified game bytes are written or checked in.
    raw = bytearray((args.cache / FILES[0]).read_bytes())
    root = u(raw, 32)
    record = u(raw, root + 108)
    payload = u(raw, record + 12)
    embedded = payload + 20
    stream = u(raw, embedded + 8)
    primitive = u(raw, embedded + 16)
    baseline = bytes(raw)
    struct.pack_into('<I', raw, record + 12, len(raw) - 8)
    reject(dll, raw, 2)  # Opaque prefix / embedded mesh truncated.
    raw[:] = baseline
    struct.pack_into('<I', raw, stream + 12, 99)
    reject(dll, raw, 4)  # Attribute array counts disagree.
    raw[:] = baseline
    struct.pack_into('<I', raw, stream + 36, len(raw) - 1)
    reject(dll, raw, 2)  # Vertex bytes exceed the file.
    raw[:] = baseline
    index_offset = u(raw, primitive + 44)
    struct.pack_into('<H', raw, index_offset, u(raw, embedded + 4))
    reject(dll, raw, 7)  # First index equals the vertex count.
    raw[:] = baseline
    struct.pack_into('<I', raw, payload, 0x12345678)
    _, changed = opened(dll, raw)
    assert list(changed.opaque_header) == [0x12345678, 15, 3, 0, 0]

    report = {'all_checks_passed': True,
              'scope': 'nine cache type-1 records; checked embedded mesh bytes only; no original runtime construction',
              'files': checked, 'totals': {'type1_records': len(checked), 'embedded_vertices': vertices,
                                         'embedded_triangles': triangles, 'scalar_checks': scalar_checks,
                                         'index_checks': index_checks, 'malformed_cases': 4},
              'host_library_sha256': hashlib.sha256(args.library.read_bytes()).hexdigest()}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report['totals']))


if __name__ == '__main__':
    main()
