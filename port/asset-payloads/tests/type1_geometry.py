#!/usr/bin/env python3
"""Validate the bounded alias contract for all nine observed type-1 records."""
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
from api import Attribute, Input, Mesh, Primitive, Type1Geometry, bind, string


CASES = (
    {
        'file': 'data/3d/characters/dragon/dragon.bdae',
        'size': 107028,
        'sha256': '97e980e2b5516fcff63f713427a729c736d9d955a25309cd5b56bfb78cf55fce',
        'geometries': 4, 'record': 0x623c, 'payload': 0x627c,
        'id': 'Circle01-spline', 'source_id': '_colbox_dragon-mesh',
        'source_name': '_colbox_dragon', 'vertices': 24, 'triangles': 12,
        'material': 'ColorMaterial',
        'vertex_sha256': '4c64ff9e47f401baac29f87d9bce679a2ccad2c8b470bd68e3b9558a549e669d',
        'index_sha256': '1103d407310c34042b6a050c281e16e3e46b2345f3cfca5a2540273a83109e6a',
    },
    {
        'file': 'data/3d/interface/skill_dh2_monster_dragon_attack_02.bdae',
        'size': 52696,
        'sha256': '7b721e0454499b23c08c851942c3ce4f57fc6ccd8c4503a84fe65566ce43c7fa',
        'geometries': 5, 'record': 0xa3dc, 'payload': 0xa42c,
        'id': 'Circle01-spline', 'source_id': '_mesh_cone2_nobatch01-mesh',
        'source_name': '_mesh_cone2_nobatch01', 'vertices': 52, 'triangles': 48,
        'material': 'fx_magic_lenz_flares_bwa',
        'vertex_sha256': 'ec4fc919d4c971435dcf43d38f976184af7f2ccdac2088092c18b174b2b32505',
        'index_sha256': '6bd2d3e182d79c13f5a324029e9e2d04ad7397c2ad4e66299bb68172af0ce3d4',
    },
    {
        'file': 'data/3d/interface/skill_dh2_monster_dragon_attack_03.bdae',
        'size': 48636,
        'sha256': '1a74b8cd645b5a62df0d0d849952f722b47b53f7ea97abb567d0249beb779d90',
        'geometries': 5, 'record': 0x9400, 'payload': 0x9450,
        'id': 'Circle01-spline', 'source_id': '_mesh_cone2_nobatch01-mesh',
        'source_name': '_mesh_cone2_nobatch01', 'vertices': 52, 'triangles': 48,
        'material': 'fx_magic_lenz_flares_bwa',
        'vertex_sha256': 'ec4fc919d4c971435dcf43d38f976184af7f2ccdac2088092c18b174b2b32505',
        'index_sha256': '6bd2d3e182d79c13f5a324029e9e2d04ad7397c2ad4e66299bb68172af0ce3d4',
    },
    {
        'file': 'data/3d/interface/skill_dh2_monster_dragon_intimidate.bdae',
        'size': 40128,
        'sha256': 'bbf1daae7bebb4fe55c52de8b0234265f349164dbba021ac0e5aa8196d2f6381',
        'geometries': 5, 'record': 0x60a4, 'payload': 0x60f4,
        'id': 'Circle01-spline', 'source_id': '_mesh_tornado_b_nobatch02-mesh',
        'source_name': '_mesh_tornado_b_nobatch02', 'vertices': 78, 'triangles': 96,
        'material': 'Standard_7',
        'vertex_sha256': 'ca2fc4f46984135eb03fbbc51a39c96c192960347b4feacc3a234dda30562643',
        'index_sha256': 'de18df6ee14e0763e7f6ddf944941f428e339821bb9499553745c37b4794b644',
    },
    {
        'file': 'data/3d/interface/skill_dh2_monster_dragon_intimidate_01.bdae',
        'size': 40492,
        'sha256': 'f12fd64bd4da6507c1b210532e796e3894e905b87d4b6d573d04c399227fff26',
        'geometries': 5, 'record': 0x6210, 'payload': 0x6260,
        'id': 'Circle01-spline', 'source_id': '_mesh_tornado_b_nobatch02-mesh',
        'source_name': '_mesh_tornado_b_nobatch02', 'vertices': 78, 'triangles': 96,
        'material': 'Standard_7',
        'vertex_sha256': 'ca2fc4f46984135eb03fbbc51a39c96c192960347b4feacc3a234dda30562643',
        'index_sha256': 'de18df6ee14e0763e7f6ddf944941f428e339821bb9499553745c37b4794b644',
    },
    {
        'file': 'data/3d/interface/skill_dh2_monster_dragon_intimidate_01b.bdae',
        'size': 41448,
        'sha256': 'bdcee26bc73850a0fa938a2d11b5570df20dc010f3cb96430a1584d808b69348',
        'geometries': 5, 'record': 0x65cc, 'payload': 0x661c,
        'id': 'Circle01-spline', 'source_id': '_mesh_tornado_b_nobatch02-mesh',
        'source_name': '_mesh_tornado_b_nobatch02', 'vertices': 78, 'triangles': 96,
        'material': 'Standard_7',
        'vertex_sha256': 'ca2fc4f46984135eb03fbbc51a39c96c192960347b4feacc3a234dda30562643',
        'index_sha256': 'de18df6ee14e0763e7f6ddf944941f428e339821bb9499553745c37b4794b644',
    },
    {
        'file': 'data/3d/interface/skill_dh2_monster_dragon_intimidate_02.bdae',
        'size': 41320,
        'sha256': 'f6f4879cbd4064eca6cecabb072635e6eb381b1e99e8d046d7edb1684ff34f12',
        'geometries': 5, 'record': 0x654c, 'payload': 0x659c,
        'id': 'Circle01-spline', 'source_id': '_mesh_tornado_b_nobatch02-mesh',
        'source_name': '_mesh_tornado_b_nobatch02', 'vertices': 78, 'triangles': 96,
        'material': 'Standard_7',
        'vertex_sha256': 'ca2fc4f46984135eb03fbbc51a39c96c192960347b4feacc3a234dda30562643',
        'index_sha256': 'de18df6ee14e0763e7f6ddf944941f428e339821bb9499553745c37b4794b644',
    },
    {
        'file': 'data/3d/interface/skill_dh2_monster_dragon_intimidate_03.bdae',
        'size': 41852,
        'sha256': '9b1ab2eab19ec3dd0f534a7a7b61df1de4b96e802c8375c885eb078aa261c15e',
        'geometries': 5, 'record': 0x6760, 'payload': 0x67b0,
        'id': 'Circle01-spline', 'source_id': '_mesh_tornado_b_nobatch02-mesh',
        'source_name': '_mesh_tornado_b_nobatch02', 'vertices': 78, 'triangles': 96,
        'material': 'Standard_7',
        'vertex_sha256': 'ca2fc4f46984135eb03fbbc51a39c96c192960347b4feacc3a234dda30562643',
        'index_sha256': 'de18df6ee14e0763e7f6ddf944941f428e339821bb9499553745c37b4794b644',
    },
    {
        'file': 'data/3d/interface/spell_dh2_splined_projectile.bdae',
        'size': 9576,
        'sha256': 'a66c76d81c4df6b17211e72318585ef4d12f24483a1f7a4a31a3dcbcaff07670',
        'geometries': 2, 'record': 0x1e8c, 'payload': 0x1eac,
        'id': 'Line01-spline', 'source_id': '_mesh_projectile-mesh',
        'source_name': '_mesh_projectile', 'vertices': 24, 'triangles': 12,
        'material': '_7_-_Default',
        'vertex_sha256': '15658bfab24dceec4de43eb657971df1c909f207bfd08c9ecf8cceb2e74b5824',
        'index_sha256': '1103d407310c34042b6a050c281e16e3e46b2345f3cfca5a2540273a83109e6a',
    },
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
    assert value.source_mesh_geometry == 0


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    dll = bind(args.library)
    checked = []
    vertices = triangles = scalar_checks = index_checks = 0
    for expected in CASES:
        raw = (args.cache / expected['file']).read_bytes()
        assert len(raw) == expected['size']
        assert hashlib.sha256(raw).hexdigest() == expected['sha256']
        data, value = opened(dll, raw)
        mesh = value.embedded_mesh
        root = u(raw, 32)
        record = u(raw, root + 108)
        assert dll.dh2_bres_library_count(c.byref(data.view), 7) == expected['geometries']
        assert record == expected['record']
        assert u(raw, record + 8) == 1 and u(raw, record + 12) == expected['payload']
        assert value.source_mesh_geometry == 1
        assert list(value.opaque_header) == [0, 15, 3, 0, 0]
        assert string(mesh.id) == expected['id'] and string(mesh.name) == ''

        source_record = record + 16 * value.source_mesh_geometry
        assert u(raw, source_record + 8) == 0
        assert u(raw, source_record + 12) == expected['payload'] + 20
        source_mesh = Mesh()
        assert dll.dh2_mesh_open(c.byref(source_mesh), c.byref(data.view), value.source_mesh_geometry) == 0
        assert string(source_mesh.id) == expected['source_id']
        assert string(source_mesh.name) == expected['source_name']
        for field in ('vertices', 'stride', 'attributes', 'primitives', 'stream', 'buffers'):
            assert getattr(mesh, field) == getattr(source_mesh, field), (expected['file'], field)
        assert list(mesh.minimum) == list(source_mesh.minimum)
        assert list(mesh.maximum) == list(source_mesh.maximum)
        assert mesh.stride == 32 and mesh.attributes == 3 and mesh.primitives == 1
        assert mesh.vertices == expected['vertices']
        assert dll.dh2_type1_geometry_open(c.byref(Type1Geometry()), c.byref(data.view), 9999) == 2

        attributes = []
        for i, components in enumerate((3, 3, 2)):
            attr, source_attr = Attribute(), Attribute()
            assert dll.dh2_mesh_attribute(c.byref(mesh), i, c.byref(attr)) == 0
            assert dll.dh2_mesh_attribute(c.byref(source_mesh), i, c.byref(source_attr)) == 0
            assert (attr.data, attr.type, attr.components, attr.stride, attr.vertices) == (
                source_attr.data, source_attr.type, source_attr.components,
                source_attr.stride, source_attr.vertices)
            assert (attr.type, attr.components, attr.stride, attr.vertices) == (
                6, components, 32, mesh.vertices)
            attributes.append(attr)
            for k in range(mesh.vertices):
                expected_values = struct.unpack(
                    '<' + 'f' * components,
                    c.string_at(attr.data + k * attr.stride, components * 4))
                actual = (c.c_float * 16)()
                assert dll.dh2_attribute_read(c.byref(attr), k, actual)
                assert tuple(actual[:components]) == expected_values
                assert all(math.isfinite(x) for x in expected_values)
                if i == 0:
                    assert all(mesh.minimum[j] <= expected_values[j] <= mesh.maximum[j]
                               for j in range(3))
                scalar_checks += components
            assert not dll.dh2_attribute_read(
                c.byref(attr), mesh.vertices, (c.c_float * 16)())
        vertex_bytes = c.string_at(attributes[0].data, mesh.vertices * mesh.stride)
        assert hashlib.sha256(vertex_bytes).hexdigest() == expected['vertex_sha256']

        primitive, source_primitive = Primitive(), Primitive()
        assert dll.dh2_mesh_primitive(c.byref(mesh), 0, c.byref(primitive)) == 0
        assert dll.dh2_mesh_primitive(c.byref(source_mesh), 0, c.byref(source_primitive)) == 0
        assert primitive.indices == source_primitive.indices
        assert string(primitive.material) == string(source_primitive.material) == expected['material']
        assert primitive.collada_type == 0 and primitive.index_width == 2
        assert primitive.declared_count == expected['triangles']
        assert primitive.index_count == primitive.declared_count * 3
        assert list(primitive.attributes[:5]) == [0, 1, -1, -1, 2]
        index_bytes = c.string_at(primitive.indices, primitive.index_count * primitive.index_width)
        assert hashlib.sha256(index_bytes).hexdigest() == expected['index_sha256']
        for k in range(primitive.index_count):
            expected_index = struct.unpack_from('<H', index_bytes, k * 2)[0]
            actual = c.c_uint32()
            assert dll.dh2_index_read(c.byref(primitive), k, c.byref(actual))
            assert actual.value == expected_index
            assert primitive.minimum_index <= expected_index <= primitive.maximum_index < mesh.vertices
            index_checks += 1
        assert c.string_at(data.view.bytes, data.view.size) == raw
        vertices += mesh.vertices
        triangles += primitive.declared_count
        checked.append({
            'file': expected['file'], 'sha256': expected['sha256'],
            'type1_id': expected['id'], 'source_mesh_geometry': value.source_mesh_geometry,
            'source_mesh_id': expected['source_id'], 'source_mesh_name': expected['source_name'],
            'opaque_header': list(value.opaque_header), 'payload_offset': expected['payload'],
            'source_payload_offset': expected['payload'] + 20,
            'material': expected['material'], 'vertices': mesh.vertices,
            'triangles': primitive.declared_count,
            'vertex_sha256': expected['vertex_sha256'], 'index_sha256': expected['index_sha256'],
        })

    # Corrupt the owner's first record in memory. These are schema-boundary
    # checks; no modified game bytes are written or checked in.
    raw = bytearray((args.cache / CASES[0]['file']).read_bytes())
    root = u(raw, 32)
    record = u(raw, root + 108)
    payload = u(raw, record + 12)
    embedded = payload + 20
    stream = u(raw, embedded + 8)
    primitive = u(raw, embedded + 16)
    baseline = bytes(raw)
    struct.pack_into('<I', raw, record + 12, len(raw) - 8)
    reject(dll, raw, 2)  # Prefix / aliased mesh truncated.
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
    reject(dll, raw, 4)  # Unobserved prefix is outside the bounded contract.
    raw[:] = baseline
    raw[u(raw, record)] = ord('X')
    reject(dll, raw, 4)  # Only the two observed spline identifiers are admitted.
    raw[:] = baseline
    struct.pack_into('<I', raw, record + 16 + 8, 1)
    reject(dll, raw, 4)  # The source alias must be an ordinary type-0 mesh.
    raw[:] = baseline
    struct.pack_into('<I', raw, record + 16 + 12, embedded + 4)
    reject(dll, raw, 4)  # The adjacent payload must begin exactly at prefix+20.

    report = {
        'all_checks_passed': True,
        'scope': ('nine cache type-1 records; exact observed prefix/ID/index and adjacent '
                  'type-0 payload alias; original runtime type-1 rejection preserved'),
        'files': checked,
        'totals': {
            'type1_records': len(checked), 'source_mesh_aliases': len(checked),
            'embedded_vertices': vertices, 'embedded_triangles': triangles,
            'scalar_checks': scalar_checks, 'index_checks': index_checks,
            'malformed_cases': 8,
        },
        'host_library_sha256': hashlib.sha256(args.library.read_bytes()).hexdigest(),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report['totals']))


if __name__ == '__main__':
    main()
