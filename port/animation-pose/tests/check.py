#!/usr/bin/env python3
"""Check absolute float transform sampling and animated bone world traversal."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import sys

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../asset-payloads/tools'))
import api as assets
spec=importlib.util.spec_from_file_location('skin_api',ROOT/'../skin-payloads/tests/audit.py')
skin=importlib.util.module_from_spec(spec);spec.loader.exec_module(skin)
class Clip(c.Structure):
    _fields_=[('tracks',assets.Animation*128),('count',c.c_uint32),('start',c.c_int32),('end',c.c_int32)]

def bind(path):
    dll=skin.bind(path)
    for name,parameters in {
        'dh2_pose_clip_open':[c.POINTER(Clip),c.POINTER(skin.scene.Bres),c.c_int32],
        'dh2_pose_sample':[c.POINTER(Clip),c.c_uint32,c.c_int32,c.POINTER(c.c_float)],
        'dh2_pose_node':[c.POINTER(Clip),c.c_int32,c.POINTER(skin.scene.Node),c.POINTER(skin.scene.Node)],
        'dh2_pose_skin_palette':[c.POINTER(Clip),c.c_int32,c.POINTER(skin.Skin),c.POINTER(skin.scene.Visual),c.POINTER(skin.scene.Matrix),c.c_size_t],
    }.items():
        f=getattr(dll,name);f.restype=c.c_uint32;f.argtypes=parameters
    return dll

def image(dll,path):
    raw=path.read_bytes();storage=c.create_string_buffer(raw);b=skin.scene.Bres()
    assert dll.dh2_bres_open(c.byref(b),storage,len(raw))==0
    return storage,b,raw

def main():
    p=argparse.ArgumentParser();p.add_argument('--library',required=True,type=Path)
    p.add_argument('--model',required=True,type=Path);p.add_argument('--animation',required=True,type=Path)
    p.add_argument('--report',required=True,type=Path);a=p.parse_args()
    dll=bind(a.library);asset_dll=assets.bind(a.library)
    storage,b,raw=image(dll,a.animation);clip=Clip()
    error=dll.dh2_pose_clip_open(c.byref(clip),c.byref(b),0)
    assert error==0,error
    assert clip.count==27,clip.count
    key_checks=0;interpolation_checks=0
    for i in range(clip.count):
        track=clip.tracks[i];v=assets.Vector()
        assert asset_dll.dh2_animation_vector(c.byref(track),0,True,c.byref(v))
        for k in range(v.count):
            time=asset_dll.dh2_animation_key_time(c.byref(track),0,k)
            expected=(c.c_float*16)();actual=(c.c_float*4)()
            assert asset_dll.dh2_vector_read(c.byref(v),k,expected)
            assert dll.dh2_pose_sample(c.byref(clip),i,time,actual)==0
            selected=c.c_int32();fraction=c.c_float()
            active=asset_dll.dh2_animation_find(c.byref(track),0,time,c.byref(selected),c.byref(fraction))
            assert asset_dll.dh2_vector_read(c.byref(v),selected.value,expected)
            if not active:
                assert list(actual)[:v.components]==list(expected)[:v.components]
                key_checks+=1
            else:
                other=(c.c_float*16)();assert asset_dll.dh2_vector_read(c.byref(v),selected.value+1,other)
                t=fraction.value
                if v.components==4:
                    first=list(expected)[:4];second=list(other)[:4]
                    dot=sum(x*y for x,y in zip(first,second))
                    if dot<0:first=[-x for x in first];dot=-dot
                    if 1-dot<0.05:
                        ref=[(1-t)*x+t*y for x,y in zip(first,second)]
                        length=math.sqrt(sum(x*x for x in ref));ref=[x/length for x in ref]
                    else:
                        angle=math.acos(min(1,dot));denom=math.sin(angle)
                        ref=[(math.sin(angle*(1-t))*x+math.sin(angle*t)*y)/denom for x,y in zip(first,second)]
                else:ref=[x+(y-x)*t for x,y in zip(list(expected)[:3],list(other)[:3])]
                assert all(math.isclose(x,y,rel_tol=3e-5,abs_tol=3e-5)for x,y in zip(actual,ref)),(i,k,time,list(actual),ref)
                interpolation_checks+=1
    model_storage,model,model_raw=image(dll,a.model);s=skin.Skin()
    assert dll.dh2_skin_open(c.byref(s),c.byref(model),0)==0
    scene=skin.scene.Scene();assert dll.dh2_scene_open(c.byref(scene),c.byref(model))==0
    visual=skin.scene.Visual();assert dll.dh2_scene_visual(c.byref(scene),0,c.byref(visual))==0
    values=[]
    for t in (0,clip.end//4,clip.end//2,clip.end*3//4,clip.end):
        palette=(skin.scene.Matrix*s.joints)()
        assert dll.dh2_pose_skin_palette(c.byref(clip),t,c.byref(s),c.byref(visual),palette,s.joints)==0
        values.append([list(m.m)for m in palette])
    assert values[0]!=values[1] and values[1]!=values[2]
    assert storage.raw[:len(raw)]==raw and model_storage.raw[:len(model_raw)]==model_raw
    assert dll.dh2_pose_sample(c.byref(clip),clip.count,0,(c.c_float*4)())!=0
    result={'complete_engine':False,'absolute_preview':True,'original_animator_equivalence':False,
            'tracks':clip.count,'start_ms':clip.start,'end_ms':clip.end,'exact_selected_key_checks':key_checks,
            'interpolated_value_checks':interpolation_checks,
            'palette_times_checked':5,'joint_count':s.joints,'palettes_change':True,
            'input_preservation':True,'animation_sha256':hashlib.sha256(raw).hexdigest(),
            'model_sha256':hashlib.sha256(model_raw).hexdigest(),
            'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest()}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
