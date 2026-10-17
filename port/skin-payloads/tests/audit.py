#!/usr/bin/env python3
"""Validate borrowed skin records throughout a supplied private BRES cache."""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('scene_api',ROOT/'../scene-payloads/tests/audit_cache.py')
scene = importlib.util.module_from_spec(spec); spec.loader.exec_module(scene)
U, I, P = c.c_uint32, c.c_int32, c.c_void_p
class Skin(c.Structure):
    _fields_ = [('image',scene.Bres),('id',P),('geometry_url',P)]+[(n,U) for n in
        ['record','joints','joint_names','inverse_matrices','vertices','influences','weights','weight_stride']]+[('geometry_index',I),('bind_shape',scene.Matrix)]
class Influence(c.Structure):
    _fields_ = [('count',U),('joints',c.c_uint8*4),('weights',c.c_float*4)]
class Vec(c.Structure):
    _fields_ = [('x',c.c_float),('y',c.c_float),('z',c.c_float)]

def bind(path):
    dll=scene.bind(path)
    signatures={
        'dh2_skin_open':(U,[c.POINTER(Skin),c.POINTER(scene.Bres),I]),
        'dh2_skin_joint_name':(P,[c.POINTER(Skin),I]),
        'dh2_skin_inverse_bind':(U,[c.POINTER(Skin),I,c.POINTER(scene.Matrix)]),
        'dh2_skin_influence':(U,[c.POINTER(Skin),U,c.POINTER(Influence)]),
        'dh2_skin_palette':(U,[c.POINTER(Skin),c.POINTER(scene.Matrix),c.c_size_t,c.POINTER(scene.Matrix),c.c_size_t]),
        'dh2_skin_scene_palette':(U,[c.POINTER(Skin),c.POINTER(scene.Visual),c.POINTER(scene.Matrix),c.c_size_t]),
        'dh2_skin_position':(U,[c.POINTER(Skin),U,c.POINTER(scene.Matrix),c.c_size_t,c.POINTER(Vec),c.POINTER(Vec)])}
    for name,(restype,args) in signatures.items():
        f=getattr(dll,name);f.restype=restype;f.argtypes=args
    return dll

def mul(a,b):
    # Round each product and addition to binary32, avoiding false failures on
    # scene translations where large products cancel near zero.
    f=lambda x:struct.unpack('<f',struct.pack('<f',x))[0]
    result=[]
    for col in range(4):
        for r in range(4):
            v=[f(a[k*4+r]*b[col*4+k]) for k in range(4)]
            result.append(f(f(f(v[0]+v[1])+v[2])+v[3]))
    return result

def check(dll,path,root,counts,hist,errors,samples):
    raw=path.read_bytes();storage=c.create_string_buffer(raw); image=scene.Bres()
    assert dll.dh2_bres_open(c.byref(image),storage,len(raw))==0
    word=lambda o:struct.unpack_from('<I',raw,o)[0]
    n=word(image.root_offset+0x70);ptr=word(image.root_offset+0x74)
    if not n:return
    counts['files_with_controllers']+=1
    for i in range(n):
        counts['controllers']+=1;s=Skin();error=dll.dh2_skin_open(c.byref(s),c.byref(image),i)
        if error:
            errors.append({'file':str(path.relative_to(root)),'controller':i,'type':word(ptr+i*12),'error':error});continue
        counts['decoded_skins']+=1;counts['joints']+=s.joints;counts['vertices']+=s.vertices
        hist[s.influences]+=1
        worlds=(scene.Matrix*s.joints)();palette=(scene.Matrix*s.joints)()
        inverse=[]
        for j in range(s.joints):
            name=dll.dh2_skin_joint_name(c.byref(s),j);assert name and c.string_at(name)
            m=scene.Matrix();assert dll.dh2_skin_inverse_bind(c.byref(s),j,c.byref(m))==0
            assert tuple(m.m)==struct.unpack_from('<16f',raw,s.inverse_matrices+j*64)
            inverse.append(list(m.m))
            # Noncommuting affine world fixture verifies multiply order, plus
            # actual inverse/bind/weight bytes, using independent double math.
            worlds[j].m[:]=[0,1,0,0,-1,0,0,0,0,0,1,0,float(j),2,-3,1]
        assert dll.dh2_skin_palette(c.byref(s),worlds,s.joints,palette,s.joints)==0
        expected=[mul(mul(list(worlds[j].m),inverse[j]),list(s.bind_shape.m)) for j in range(s.joints)]
        for j in range(s.joints):
            assert all(math.isclose(x,y,rel_tol=3e-5,abs_tol=3e-4) for x,y in zip(palette[j].m,expected[j])),(path,i,j,list(palette[j].m),expected[j])
        max_sum_error=0
        for k in range(s.vertices):
            v=Influence();assert dll.dh2_skin_influence(c.byref(s),k,c.byref(v))==0
            p=s.weights+k*s.weight_stride
            assert bytes(v.joints)[:v.count]==raw[p:p+v.count]
            assert list(v.weights)[:v.count]==list(struct.unpack_from('<'+'f'*v.count,raw,p+4))
            total=sum(v.weights[:v.count])
            if total==0: counts['zero_weight_vertices']+=1
            else: max_sum_error=max(max_sum_error,abs(total-1))
            if k in [0,s.vertices//2,s.vertices-1]:
                start=Vec(1.25,-2,3.5);out=Vec()
                assert dll.dh2_skin_position(c.byref(s),k,palette,s.joints,c.byref(start),c.byref(out))==0
                xyz=[1.25,-2,3.5,1]; e=[0.0]*3
                for j,w in zip(v.joints[:v.count],v.weights[:v.count]):
                    if w:
                        for axis in range(3):e[axis]+=sum(expected[j][q*4+axis]*xyz[q] for q in range(4))*w
                assert all(math.isclose(x,y,rel_tol=4e-5,abs_tol=5e-4) for x,y in zip((out.x,out.y,out.z),e))
                counts['checked_positions']+=1
        assert max_sum_error<2e-5,(path,i,max_sum_error)
        if 'prince_low_poly_warrior' in path.name or 'prince_modular' in path.name:
            samples.append({'file':str(path.relative_to(root)),'skin':c.string_at(s.id).decode(),
                            'joints':s.joints,'vertices':s.vertices,'influences':s.influences,
                            'max_weight_sum_error':max_sum_error,'sha256':hashlib.sha256(raw).hexdigest()})

def main():
    p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True)
    p.add_argument('--cache',type=Path,required=True);p.add_argument('--report',type=Path,required=True)
    a=p.parse_args();dll=bind(a.library);counts=Counter();hist=Counter();errors=[];samples=[]
    for path in sorted(a.cache.rglob('*.bdae')):
        counts['files']+=1;check(dll,path,a.cache,counts,hist,errors,samples)
    result={'complete_engine':False,'arm_differential':False,'totals':dict(counts),
            'influences_per_skin':dict(hist),'unsupported':errors,'samples':samples,
            'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest()}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'totals':dict(counts),'unsupported':len(errors),'histogram':dict(hist)}))
    unexpected=[e for e in errors if e['error']!=6 or '/animations/' not in e['file']]
    assert not unexpected,unexpected[:4]

if __name__=='__main__':main()
