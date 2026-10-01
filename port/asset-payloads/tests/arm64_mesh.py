#!/usr/bin/env python3
"""Execute compiled ARM64 mesh decoding and compare with the host decoder.

This is a target-ABI/code-generation check, not an original GPU constructor
test. Mesh layout evidence and full-cache data integrity are reported separately.
"""
import argparse
import ctypes as c
import hashlib
import json
from pathlib import Path
import struct
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'tools'))
sys.path.insert(0, str(ROOT/'../engine-resources/tests'))
from api import Attribute, Input, Mesh, Primitive, bind
from cpu import Cpu

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--cache', required=True, type=Path)
    p.add_argument('--ported', type=Path, default=ROOT/'build/libdh2_asset_payloads_arm64.so')
    p.add_argument('--host', type=Path, default=ROOT/'build/libdh2_asset_payloads_host.so')
    p.add_argument('--report', required=True, type=Path)
    a = p.parse_args()
    start = time.monotonic()
    cpu = Cpu(a.ported, True, {'functions': []}); dll = bind(a.host)
    files = {a.cache/'data/3d/menu/main_menu_charactere_swamp.bdae',
             a.cache/'data/3d/characters/prince/prince_modular.bdae',
             a.cache/'data/3d/animateddecors/candle_flame.bdae'}
    # Include the largest geometry-bearing BRES file, chosen from the corpus.
    candidates = []
    for f in a.cache.rglob('*.bdae'):
        raw = f.read_bytes(); root = struct.unpack_from('<I',raw,32)[0]
        if struct.unpack_from('<I',raw,root+104)[0]:
            candidates.append((len(raw), f))
    if candidates:
        files.add(max(candidates)[1])
    checks = meshes = vertices = indices = 0
    def same(old, new, context):
        nonlocal checks
        assert old == new, (context,old,new)
        checks += 1
    provenance = []
    for f in sorted(files):
        raw = f.read_bytes(); host = Input(dll,raw)
        image,view,mesh,attr,primitive,result = [cpu.data+x for x in [0x10000,0x1000,0x2000,0x3000,0x4000,0x5000]]
        cpu.uc.mem_write(image,raw)
        assert cpu.invoke('dh2_bres_open',[view,image,len(raw)],budget=10000000) == 0
        host_base = c.addressof(host.bytes)
        def offset(pointer, base):
            return pointer-base if pointer else None
        decoded = 0
        for i in range(dll.dh2_bres_library_count(c.byref(host.view),7)):
            m = Mesh(); error = dll.dh2_mesh_open(c.byref(m),c.byref(host.view),i)
            native_error = cpu.invoke('dh2_mesh_open',[mesh,view,i],budget=10000000)
            same(error,native_error,(f.name,i,'open'))
            if error:
                continue
            n = Mesh.from_buffer_copy(bytes(cpu.uc.mem_read(mesh,c.sizeof(Mesh))))
            same((offset(m.id,host_base),offset(m.name,host_base)),(offset(n.id,image),offset(n.name,image)),(f.name,i,'names'))
            same((m.vertices,m.stride,m.attributes,m.primitives,m.stream,m.buffers),
                 (n.vertices,n.stride,n.attributes,n.primitives,n.stream,n.buffers),(f.name,i,'mesh'))
            same(bytes(m.minimum)+bytes(m.maximum),bytes(n.minimum)+bytes(n.maximum),(f.name,i,'bounds'))
            decoded += 1; meshes += 1; vertices += m.vertices
            for j in range(m.attributes):
                h = Attribute(); assert dll.dh2_mesh_attribute(c.byref(m),j,c.byref(h)) == 0
                assert cpu.invoke('dh2_mesh_attribute',[mesh,j,attr]) == 0
                v = Attribute.from_buffer_copy(bytes(cpu.uc.mem_read(attr,c.sizeof(Attribute))))
                same((offset(h.data,host_base),h.type,h.components,h.stride,h.vertices),
                     (offset(v.data,image),v.type,v.components,v.stride,v.vertices),(f.name,i,j,'attribute'))
                for k in sorted({0,m.vertices-1}) if m.vertices else []:
                    out = (c.c_float*16)(); assert dll.dh2_attribute_read(c.byref(h),k,out)
                    assert cpu.invoke('dh2_attribute_read',[attr,k,result]) == 1
                    same(bytes(out)[:h.components*4],bytes(cpu.uc.mem_read(result,h.components*4)),(f.name,i,j,k,'vertex'))
            for j in range(m.primitives):
                h = Primitive(); assert dll.dh2_mesh_primitive(c.byref(m),j,c.byref(h)) == 0
                assert cpu.invoke('dh2_mesh_primitive',[mesh,j,primitive]) == 0
                v = Primitive.from_buffer_copy(bytes(cpu.uc.mem_read(primitive,c.sizeof(Primitive))))
                same((offset(h.material,host_base),offset(h.indices,host_base)),
                     (offset(v.material,image),offset(v.indices,image)),(f.name,i,j,'primitive pointers'))
                same((h.collada_type,h.engine_type,h.declared_count,h.index_count,h.index_width,h.minimum_index,h.maximum_index,list(h.attributes)),
                     (v.collada_type,v.engine_type,v.declared_count,v.index_count,v.index_width,v.minimum_index,v.maximum_index,list(v.attributes)),(f.name,i,j,'primitive fields'))
                indices += h.index_count
                for k in sorted({0,h.index_count-1}) if h.index_count else []:
                    out = c.c_uint32(); assert dll.dh2_index_read(c.byref(h),k,c.byref(out))
                    assert cpu.invoke('dh2_index_read',[primitive,k,result]) == 1
                    same(out.value,struct.unpack('<I',cpu.uc.mem_read(result,4))[0],(f.name,i,j,k,'index'))
        provenance.append({'file': str(f.relative_to(a.cache)), 'sha256': hashlib.sha256(raw).hexdigest(), 'decoded_meshes': decoded})
    report = {'scope': 'host versus executed compiled ARM64 mesh decoder; original constructor is not executed',
              'complete_engine': False, 'comparisons': checks, 'mismatches': 0, 'meshes': meshes, 'vertices': vertices, 'indices': indices,
              'arm64_pointers_above_4gib': True, 'files': provenance,
              'ported_sha256': hashlib.sha256(a.ported.read_bytes()).hexdigest(), 'host_sha256': hashlib.sha256(a.host.read_bytes()).hexdigest(),
              'elapsed_seconds': round(time.monotonic()-start,2)}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))

if __name__ == '__main__':
    main()
