#!/usr/bin/env python3
"""Export local-space triangle meshes and raw animation keys from BRES."""
import argparse
import ctypes as c
import hashlib
import json
from pathlib import Path
import re
from api import Attribute, Input, Primitive, Vector, bind, string

ROOT = Path(__file__).resolve().parents[1]

def checked(code, message):
    if code:
        raise ValueError(f'{message}: error {code}')

def label(value):
    return re.sub(r'[^\w.-]+', '_', value or 'unnamed')

def attribute(dll, mesh, index):
    if index < 0:
        return None
    value = Attribute()
    checked(dll.dh2_mesh_attribute(c.byref(mesh), index, c.byref(value)), 'Attribute')
    return value

def row(dll, attribute, index):
    out = (c.c_float*16)()
    if not dll.dh2_attribute_read(c.byref(attribute), index, out):
        raise ValueError('Vertex read rejected')
    return list(out)[:attribute.components]

def export_obj(data, path):
    dll = data.dll
    meshes = dll.dh2_bres_library_count(c.byref(data.view), 7)
    totals = {'meshes': 0, 'faces': 0, 'unsupported_geometries': []}
    vertex_base = uv_base = normal_base = 0
    with path.open('w', encoding='utf-8') as out:
        out.write('# DH2 BRES: local geometry only; no scene transforms, skinning or textures\n')
        for i in range(meshes):
            try:
                mesh = data.mesh(i)
            except ValueError as error:
                if 'code 3' not in str(error):
                    raise
                totals['unsupported_geometries'].append(i)
                continue
            totals['meshes'] += 1
            for j in range(mesh.primitives):
                p = Primitive()
                checked(dll.dh2_mesh_primitive(c.byref(mesh), j, c.byref(p)), 'Primitive')
                if p.collada_type != 0 or p.index_count % 3:
                    raise ValueError('OBJ exporter currently requires triangle lists')
                position = attribute(dll, mesh, p.attributes[0])
                normal = attribute(dll, mesh, p.attributes[1])
                uv = attribute(dll, mesh, p.attributes[4])
                if not position or position.components < 3:
                    raise ValueError('Triangle primitive has no 3D positions')
                out.write(f'o {label(string(mesh.id))}_{j}\n')
                out.write(f'usemtl {label(string(p.material))}\n')
                for k in range(mesh.vertices):
                    out.write('v '+' '.join(format(v, '.9g') for v in row(dll, position, k)[:3])+'\n')
                if uv:
                    for k in range(mesh.vertices):
                        out.write('vt '+' '.join(format(v, '.9g') for v in row(dll, uv, k)[:2])+'\n')
                if normal:
                    for k in range(mesh.vertices):
                        out.write('vn '+' '.join(format(v, '.9g') for v in row(dll, normal, k)[:3])+'\n')
                for k in range(0, p.index_count, 3):
                    indices = []
                    for offset in range(3):
                        value = c.c_uint32()
                        if not dll.dh2_index_read(c.byref(p), k+offset, c.byref(value)):
                            raise ValueError('Index read rejected')
                        v = value.value + 1
                        indices.append(f'{vertex_base+v}/{uv_base+v if uv else ""}/{normal_base+v if normal else ""}' if uv or normal else str(vertex_base+v))
                    out.write('f '+' '.join(indices)+'\n')
                    totals['faces'] += 1
                vertex_base += mesh.vertices
                if uv:
                    uv_base += mesh.vertices
                if normal:
                    normal_base += mesh.vertices
    return totals

def export_animation(data, source_hash):
    dll = data.dll
    result = {'schema': 'dh2-raw-animation-keys-v1', 'input_sha256': source_hash,
              'value_space': 'raw stored components; no track-specific normalization or transforms',
              'animations': []}
    for i in range(dll.dh2_bres_library_count(c.byref(data.view), 0)):
        animation = {'index': i, 'segments': []}
        for segment in range(dll.dh2_animation_segments(c.byref(data.view))):
            a = data.animation(i, segment)
            entry = {'index': segment, 'range_ms': [a.segment_start, a.segment_end],
                     'target': string(dll.dh2_animation_target(c.byref(a))),
                     'channel_types': [dll.dh2_animation_type(c.byref(a), j) for j in range(dll.dh2_animation_channels(c.byref(a)))],
                     'samplers': []}
            for sampler in range(dll.dh2_animation_samplers(c.byref(a))):
                times, values = Vector(), Vector()
                if not dll.dh2_animation_vector(c.byref(a), sampler, False, c.byref(times)) or not dll.dh2_animation_vector(c.byref(a), sampler, True, c.byref(values)):
                    raise ValueError('Animation vector rejected')
                out = (c.c_float*16)()
                keys = []
                for k in range(values.count):
                    if not dll.dh2_vector_read(c.byref(values), k, out):
                        raise ValueError('Animation value rejected')
                    keys.append(list(out)[:values.components])
                entry['samplers'].append({'index': sampler,
                    'interpolation': dll.dh2_animation_interpolation(c.byref(a), sampler),
                    'time_type': times.type, 'value_type': values.type,
                    'components': values.components,
                    'time_ms': [dll.dh2_animation_key_time(c.byref(a), sampler, k) for k in range(times.count)],
                    'values': keys})
            animation['segments'].append(entry)
        result['animations'].append(animation)
    return result

def main():
    p = argparse.ArgumentParser()
    p.add_argument('input', type=Path)
    p.add_argument('--library', type=Path, default=ROOT/'build/libdh2_asset_payloads_host.so')
    p.add_argument('--obj', type=Path)
    p.add_argument('--animation-json', type=Path)
    a = p.parse_args()
    if not a.obj and not a.animation_json:
        p.error('Choose --obj and/or --animation-json')
    raw = a.input.read_bytes()
    data = Input(bind(a.library), raw)
    report = {'input': str(a.input), 'sha256': hashlib.sha256(raw).hexdigest()}
    if a.obj:
        a.obj.parent.mkdir(parents=True, exist_ok=True)
        report['obj'] = export_obj(data, a.obj)
    if a.animation_json:
        a.animation_json.parent.mkdir(parents=True, exist_ok=True)
        result = export_animation(data, report['sha256'])
        a.animation_json.write_text(json.dumps(result, indent=2, allow_nan=False)+'\n')
        report['animation_count'] = len(result['animations'])
    print(json.dumps(report))

if __name__ == '__main__':
    main()
