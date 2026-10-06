"""Repair the supplied loading SWF's mismatched packed-atlas fill.

Uses original ring pixels, retaining SWF geometry/actions/timeline. This is a
documented compatibility mapping; original Android packing coordinates remain
unresolved. The original cache and texture bytes are never modified.
"""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
from repair_keyboard_atlas import unpack,tags,shape,encode_matrix,artwork_bounds

def build(archive,destination):
    with zipfile.ZipFile(archive) as z:
        prefix='com.gameloft.android.GAND.GloftD2SS/files/data/menus/'
        raw=z.read(prefix+'loadanims_droid.swf');genericraw=z.read(prefix+'dqmenus.swf')
    assert hashlib.sha256(raw).hexdigest()=='ee12d5c1dd9791fe2d438eb931794ff78ee53618c084bf350b90ad33838f34d5'
    generic=unpack(genericraw);data=unpack(raw)
    # First complete original ring; the second cell begins beyond x76.
    uv=artwork_bounds(generic,(16,786,76,852))
    pieces=[];cursor=0;records=[]
    for t,start,q,length in tags(data):
        if t!=2:continue
        identity=struct.unpack_from('<H',data,q)[0];assert identity==2
        bounds,fills=shape(data,q,t);assert len(fills)==1 and fills[0][2]==1
        sx=(bounds[1]-bounds[0])/(uv[1]-uv[0]);sy=(bounds[3]-bounds[2])/(uv[3]-uv[2])
        shift=[bounds[0]-uv[0]*sx,bounds[2]-uv[2]*sy]
        a,b,bitmap,old=fills[0];replacement=encode_matrix([sx,sy],shift)
        payload=data[q:a]+replacement+data[b:q+length]
        pieces.extend([data[cursor:start],struct.pack('<HI',(t<<6)|63,len(payload))+payload]);cursor=q+length
        records.append({'shape':identity,'bounds_twips':bounds,'old_matrix':old,'ring_pixel_bounds':uv,'new_scale':[sx,sy],'new_shift':shift})
    assert len(records)==1
    pieces.append(data[cursor:]);patched=bytearray(b''.join(pieces));patched[:3]=b'FWS';struct.pack_into('<I',patched,4,len(patched))
    original_tags=list(tags(data));patched_tags=list(tags(patched));assert len(original_tags)==len(patched_tags)
    for (t,_,q,n),(u,_,r,m) in zip(original_tags,patched_tags):
        assert t==u
        if t!=2:assert data[q:q+n]==patched[r:r+m], 'Non-shape tag changed'
        else:
            oldbounds,oldfills=shape(data,q,t);newbounds,newfills=shape(patched,r,t)
            assert oldbounds==newbounds
            oa,ob,_,_=oldfills[0];na,nb,_,_=newfills[0]
            assert data[q:oa]==patched[r:na] and data[ob:q+n]==patched[nb:r+m], 'Geometry changed'
    destination.mkdir(parents=True,exist_ok=True);(destination/'loadanims_droid.swf').write_bytes(patched)
    report={'scope':'Loading ring atlas compatibility correction; supplied Android fill mapping unresolved','original_sha256':hashlib.sha256(raw).hexdigest(),'original_atlas_source':'dqmenus.swf embedded bitmap1; original packed splash ring artwork','source_movie_sha256':hashlib.sha256(genericraw).hexdigest(),'patched_sha256':hashlib.sha256(patched).hexdigest(),'geometry_actions_timeline_unchanged':True,'shapes':records}
    (destination/'loading-atlas-repair.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--archive',type=Path,required=True);p.add_argument('--destination',type=Path,required=True)
    a=p.parse_args();build(a.archive,a.destination)
