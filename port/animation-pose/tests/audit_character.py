#!/usr/bin/env python3
"""Classify a privately supplied character animation directory against one model.

This checks supported source poses; it does not validate game animation state,
root motion, combat semantics or complete original rendering.
"""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import json
import math
from pathlib import Path
import check as pose

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--library',type=Path,required=True)
    p.add_argument('--model',type=Path,required=True)
    p.add_argument('--animations',type=Path,required=True)
    p.add_argument('--report',type=Path,required=True)
    a=p.parse_args();dll=pose.bind(a.library)
    owner,model,model_raw=pose.image(dll,a.model)
    skin=pose.skin.Skin();assert dll.dh2_skin_open(c.byref(skin),c.byref(model),0)==0
    scene=pose.skin.scene.Scene();visual=pose.skin.scene.Visual()
    assert dll.dh2_scene_open(c.byref(scene),c.byref(model))==0
    assert dll.dh2_scene_visual(c.byref(scene),0,c.byref(visual))==0
    assets=pose.assets.bind(a.library)
    mesh=pose.assets.Mesh();primitive=pose.assets.Primitive();attribute=pose.assets.Attribute()
    assert assets.dh2_mesh_open(c.byref(mesh),c.cast(c.byref(model),c.POINTER(pose.assets.Bres)),skin.geometry_index)==0
    assert assets.dh2_mesh_primitive(c.byref(mesh),0,c.byref(primitive))==0
    assert assets.dh2_mesh_attribute(c.byref(mesh),primitive.attributes[0],c.byref(attribute))==0
    assert attribute.components>=3
    inputs={}
    for index in sorted({0,skin.vertices//2,skin.vertices-1}):
        values=(c.c_float*16)()
        assert assets.dh2_attribute_read(c.byref(attribute),index,values)
        inputs[index]=pose.skin.Vec(*list(values)[:3])
    records=[];summary=Counter();palettes=0;positions=0
    for path in sorted(a.animations.rglob('*.bdae')):
        raw=path.read_bytes();storage=c.create_string_buffer(raw);image=pose.skin.scene.Bres()
        entry={'path':path.relative_to(a.animations).as_posix(),'sha256':hashlib.sha256(raw).hexdigest()}
        e=dll.dh2_bres_open(c.byref(image),storage,len(raw))
        if e:
            entry.update(stage='bres',error=e);summary['bres_rejected']+=1
        else:
            clip=pose.Clip();e=dll.dh2_pose_clip_open(c.byref(clip),c.byref(image),0)
            if e:
                entry.update(stage='clip',error=e);summary[f'clip_error_{e}']+=1
            else:
                times=sorted({clip.start,(clip.start+clip.end)//2,clip.end})
                entry.update(tracks=clip.count,start_ms=clip.start,end_ms=clip.end)
                for time in times:
                    palette=(pose.skin.scene.Matrix*skin.joints)()
                    e=dll.dh2_pose_skin_palette(c.byref(clip),time,c.byref(skin),c.byref(visual),palette,skin.joints)
                    if e:break
                    assert all(math.isfinite(x) for matrix in palette for x in matrix.m)
                    palettes+=1
                    for index,value in inputs.items():
                        out=pose.skin.Vec()
                        assert dll.dh2_skin_position(c.byref(skin),index,palette,skin.joints,c.byref(value),c.byref(out))==0
                        assert all(math.isfinite(x)for x in (out.x,out.y,out.z));positions+=1
                if e:
                    entry.update(stage='model_binding',error=e);summary[f'model_binding_error_{e}']+=1
                else:
                    entry.update(stage='supported_pose',sampled_times_ms=times)
                    summary['supported_pose']+=1
                    summary['positive_duration']+=int(clip.end>clip.start)
        assert bytes(storage.raw[:len(raw)])==raw
        records.append(entry)
    assert records and summary['supported_pose']>0
    assert bytes(owner.raw[:len(model_raw)])==model_raw
    result={'complete_game':False,'original_animator_equivalence':False,
        'scope':'One locally resolved warrior skin; supported absolute-key poses at selected times',
        'model_sha256':hashlib.sha256(model_raw).hexdigest(),
        'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),
        'joints':skin.joints,'vertices':skin.vertices,'files':len(records),
        'summary':dict(summary),'successful_palette_checks':palettes,'successful_position_checks':positions,
        'input_preservation':True,'records':records}
    a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k!='records'}))
if __name__=='__main__':main()
