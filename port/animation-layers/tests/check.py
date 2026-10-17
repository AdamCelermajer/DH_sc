#!/usr/bin/env python3
"""Check layered source poses with real owner fixtures; no animator equivalence claim."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import struct

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('pose_api',ROOT/'../animation-pose/tests/check.py')
pose=importlib.util.module_from_spec(spec);spec.loader.exec_module(pose)
class Layer(c.Structure):
    _fields_=[('clip',c.POINTER(pose.Clip)),('time',c.c_int32),('weight',c.c_float)]
class Layers(c.Structure):
    _fields_=[('items',Layer*8),('count',c.c_uint32)]

def main():
    p=argparse.ArgumentParser()
    for key in ('library','model','first','second','report'):p.add_argument('--'+key,type=Path,required=True)
    a=p.parse_args();dll=pose.bind(a.library)
    dll.dh2_layers_skin_palette.argtypes=[c.POINTER(Layers),c.POINTER(pose.skin.Skin),c.POINTER(pose.skin.scene.Visual),c.POINTER(pose.skin.scene.Matrix),c.c_size_t]
    dll.dh2_layers_skin_palette.restype=c.c_uint32
    keep=[];clips=[]
    for path in (a.first,a.second):
        storage,image,raw=pose.image(dll,path);clip=pose.Clip()
        assert dll.dh2_pose_clip_open(c.byref(clip),c.byref(image),0)==0
        keep.append((storage,image,raw));clips.append(clip)
    storage,model,raw=pose.image(dll,a.model)
    skin=pose.skin.Skin();assert dll.dh2_skin_open(c.byref(skin),c.byref(model),0)==0
    scene=pose.skin.scene.Scene();assert dll.dh2_scene_open(c.byref(scene),c.byref(model))==0
    visual=pose.skin.scene.Visual();assert dll.dh2_scene_visual(c.byref(scene),0,c.byref(visual))==0
    n=skin.joints;checks=0;endpoints=0;results=[]
    def same(x,y):
        for aa,bb in zip(x,y):
            for v,w in zip(aa.m,bb.m):
                assert v==w or math.isclose(v,w,rel_tol=3e-6,abs_tol=3e-5),(v,w)
    for phase in (0,.25,.5,.75,1):
        times=[clip.start+int((clip.end-clip.start)*phase)for clip in clips]
        originals=[]
        for clip,time in zip(clips,times):
            palette=(pose.skin.scene.Matrix*n)()
            assert dll.dh2_pose_skin_palette(c.byref(clip),time,c.byref(skin),c.byref(visual),palette,n)==0
            originals.append(palette)
        phase_results=[]
        for weight in (0,.25,.5,.75,1):
            layers=Layers();layers.count=2
            layers.items[0]=Layer(c.pointer(clips[0]),times[0],1-weight)
            layers.items[1]=Layer(c.pointer(clips[1]),times[1],weight)
            palette=(pose.skin.scene.Matrix*n)()
            assert dll.dh2_layers_skin_palette(c.byref(layers),c.byref(skin),c.byref(visual),palette,n)==0
            assert all(math.isfinite(v)for matrix in palette for v in matrix.m)
            if weight in (0,1):same(palette,originals[int(weight)]);endpoints+=1
            phase_results.append(bytes(palette));checks+=1
        assert phase_results[0]!=phase_results[-1] and phase_results[0]!=phase_results[2] and phase_results[2]!=phase_results[-1]
        results.append({'phase':phase,'times_ms':times,'palette_sha256':[hashlib.sha256(x).hexdigest()for x in phase_results]})
    layers=Layers();layers.count=2
    layers.items[0]=Layer(c.pointer(clips[0]),clips[0].start,0)
    layers.items[1]=Layer(c.pointer(clips[1]),clips[1].start,0)
    palette=(pose.skin.scene.Matrix*n)();first=(pose.skin.scene.Matrix*n)()
    assert dll.dh2_layers_skin_palette(c.byref(layers),c.byref(skin),c.byref(visual),palette,n)==0
    assert dll.dh2_pose_skin_palette(c.byref(clips[0]),clips[0].start,c.byref(skin),c.byref(visual),first,n)==0
    same(palette,first)
    # Count/weight/capacity failures must not expose a partially written palette.
    for count,weight,capacity in ((0,1,n),(9,1,n),(2,-1,n),(2,float('nan'),n),(2,1,n-1)):
        layers.count=count;layers.items[0].weight=weight
        c.memset(c.byref(palette),0xa5,c.sizeof(palette));before=bytes(palette)
        assert dll.dh2_layers_skin_palette(c.byref(layers),c.byref(skin),c.byref(visual),palette,capacity)!=0
        assert bytes(palette)==before
    assert storage.raw[:len(raw)]==raw
    for buf,_,data in keep:assert buf.raw[:len(data)]==data
    sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    report={'complete_game':False,'original_animator_equivalence':False,'absolute_layer_diagnostic':True,
            'library_sha256':sha(a.library),'model_sha256':sha(a.model),'first_sha256':sha(a.first),'second_sha256':sha(a.second),
            'tracks':[x.count for x in clips],'joints':n,'palettes_checked':checks,'endpoint_checks':endpoints,
            'zero_sum_selects_first':True,'rejections_unchanged':5,'palettes_change_with_weight':True,
            'input_preservation':True,'phase_results':results,'test_sha256':sha(Path(__file__))}
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
