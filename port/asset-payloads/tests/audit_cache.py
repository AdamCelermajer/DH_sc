#!/usr/bin/env python3
"""Read every mesh/index and animation key through the compiled C++ loader."""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import json
import math
from pathlib import Path
import struct
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'tools'))
from api import Attribute, Input, Mesh, Primitive, Vector, bind

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--cache', type=Path, required=True)
    p.add_argument('--library', type=Path, default=ROOT/'build/libdh2_asset_payloads_host.so')
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    dll = bind(a.library)
    totals, types, states = Counter(), Counter(), Counter()
    skipped, hashes = [], []
    start = time.monotonic()
    for file in sorted(a.cache.rglob('*.bdae')):
        raw = file.read_bytes(); data = Input(dll, raw)
        hashes.append({'path': str(file.relative_to(a.cache)), 'sha256': hashlib.sha256(raw).hexdigest()})
        totals['files'] += 1; totals['file_bytes'] += len(raw)
        u = lambda o: struct.unpack_from('<I', raw, o)[0]
        root = data.view.root_offset
        if u(root+100):
            totals['files_with_deferred_mesh_buffers'] += 1
        for i in range(dll.dh2_bres_library_count(c.byref(data.view), 7)):
            mesh = Mesh(); error = dll.dh2_mesh_open(c.byref(mesh), c.byref(data.view), i)
            if error == 3:
                skipped.append({'file': str(file.relative_to(a.cache)), 'geometry': i, 'type': u(u(root+108)+16*i+8)})
                continue
            assert error == 0, (file, i, error)
            totals['decoded_meshes'] += 1; totals['vertices'] += mesh.vertices
            for j in range(mesh.attributes):
                attr = Attribute()
                assert dll.dh2_mesh_attribute(c.byref(mesh), j, c.byref(attr)) == 0
                types[f'vertex_{attr.type}_{attr.components}'] += 1
                # Decode every stored component in Python directly from the
                # pointer and stride returned by the validated C++ view, and
                # compare the actual C++ scalar reader at the first/last vertex.
                formats = ['b', 'B', 'h', 'H', 'i', 'I', 'f']
                fmt = '<'+formats[attr.type]*attr.components
                blob = c.string_at(attr.data, (attr.vertices-1)*attr.stride+struct.calcsize(fmt)) if attr.vertices else b''
                for k in range(attr.vertices):
                    values = struct.unpack_from(fmt, blob, k*attr.stride)
                    assert all(math.isfinite(v) for v in values), (file, i, j, k)
                    totals['finite_vertex_components'] += len(values)
                for k in sorted({0, attr.vertices-1}) if attr.vertices else []:
                    values = struct.unpack_from(fmt, blob, k*attr.stride)
                    out = (c.c_float*16)()
                    assert dll.dh2_attribute_read(c.byref(attr), k, out)
                    assert list(out)[:attr.components] == [c.c_float(v).value for v in values]
                    totals['vertex_scalar_reader_checks'] += 1
            for j in range(mesh.primitives):
                primitive = Primitive()
                assert dll.dh2_mesh_primitive(c.byref(mesh), j, c.byref(primitive)) == 0
                totals['primitives'] += 1; totals['indices'] += primitive.index_count
                types['primitive_'+str(primitive.collada_type)] += 1
                assert primitive.collada_type == 0 and primitive.index_count == primitive.declared_count*3
                totals['triangles'] += primitive.index_count//3
                indices = struct.unpack('<'+('H' if primitive.index_width == 2 else 'I')*primitive.index_count,
                                        c.string_at(primitive.indices, primitive.index_count*primitive.index_width))
                assert all(0 <= k < mesh.vertices and primitive.minimum_index <= k <= primitive.maximum_index for k in indices)
                for k in sorted({0, primitive.index_count-1}) if primitive.index_count else []:
                    out = c.c_uint32()
                    assert dll.dh2_index_read(c.byref(primitive), k, c.byref(out)) and out.value == indices[k]
                    totals['index_scalar_reader_checks'] += 1
        segments = dll.dh2_animation_segments(c.byref(data.view))
        totals['segments'] += segments
        if segments:
            lib = u(root+48)
            for s in range(segments):
                seg = u(lib+4)+24*s; states[str(u(seg+8))] += 1
                blob = u(seg+12) if u(seg+8) == 0 else u(seg+20)
                totals['animation_data_entries'] += u(blob)
        count = dll.dh2_bres_library_count(c.byref(data.view), 0)
        totals['animations'] += count
        for i in range(count):
            for segment in range(segments):
                anim = data.animation(i, segment)
                totals['animation_segment_views'] += 1
                for sampler in range(dll.dh2_animation_samplers(c.byref(anim))):
                    times, values = Vector(), Vector()
                    assert dll.dh2_animation_vector(c.byref(anim), sampler, False, c.byref(times))
                    assert dll.dh2_animation_vector(c.byref(anim), sampler, True, c.byref(values))
                    assert times.count == values.count
                    totals['sampler_segment_vectors'] += 1
                    types['time_'+str(times.type)] += 1; types[f'output_{values.type}_{values.components}'] += 1
                    keys = [dll.dh2_animation_key_time(c.byref(anim), sampler, k) for k in range(times.count)]
                    assert all(x <= y for x,y in zip(keys, keys[1:])), (file, i, segment, sampler)
                    assert not keys or dll.dh2_animation_start(c.byref(anim), sampler) == keys[0]
                    assert not keys or dll.dh2_animation_end(c.byref(anim), sampler) == keys[-1]
                    totals['animation_time_keys'] += len(keys)
                    fmt = '<'+['b','B','h','H','i','I','f'][values.type]*values.components
                    blob = c.string_at(values.data, struct.calcsize(fmt)*values.count)
                    decoded = list(struct.iter_unpack(fmt, blob))
                    assert all(math.isfinite(v) for row in decoded for v in row)
                    totals['finite_animation_components'] += values.components*values.count
                    for k in sorted({0, values.count-1}) if values.count else []:
                        out = (c.c_float*16)()
                        assert dll.dh2_vector_read(c.byref(values), k, out)
                        assert list(out)[:values.components] == [c.c_float(v).value for v in decoded[k]]
                        totals['animation_scalar_reader_checks'] += 1
        if totals['files'] % 500 == 0:
            print(json.dumps({'files_checked': totals['files'], 'seconds': round(time.monotonic()-start, 2)}), flush=True)
    report = {'all_checks_passed': True, 'scope': 'full recovered BRES cache; immutable payload decoding and data integrity',
              'complete_engine': False, 'totals': dict(totals), 'types': dict(types), 'segment_states': dict(states),
              'unsupported_geometry': skipped,
              'cache_manifest_sha256': hashlib.sha256(json.dumps(hashes, sort_keys=True).encode()).hexdigest(),
              'library_sha256': hashlib.sha256(a.library.read_bytes()).hexdigest(),
              'elapsed_seconds': round(time.monotonic()-start, 2)}
    a.report.parent.mkdir(parents=True, exist_ok=True)
    a.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))

if __name__ == '__main__':
    main()
