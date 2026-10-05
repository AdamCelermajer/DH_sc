"""Verify menu-atlas compatibility preserves actions and non-Space shape edges."""
import argparse,hashlib,json,zipfile,struct
from pathlib import Path
from repair_keyboard_atlas import unpack,tags,shape,PREFIX
from inspect_front_swf import inspect, Bits

def verify_space(d,p,t,record):
    bounds,fills=shape(d,p,t);assert bounds==record['bounds_twips']
    assert len(fills)==3 and all(f[2]==1 for f in fills)
    x0,x1,y0,y1=bounds;uv=record['atlas_pixel_bounds']
    cuts=record['slicing']['x_cuts_twips'];us=record['slicing']['u_cuts_pixels']
    assert cuts[0]==x0 and cuts[-1]==x1 and all(cuts[i]<cuts[i+1] for i in range(3))
    assert us[0]==uv[0] and us[-1]==uv[1]
    sy=(y1-y0)/(uv[3]-uv[2])
    for i,(_,_,_,m) in enumerate(fills):
        sx,my=m['scale'];tx,ty=m['shift']
        assert m['skew']==[0.,0.] and abs(my-sy)<1/65536
        assert abs(us[i]*sx+tx-cuts[i])<1.1
        assert abs(us[i+1]*sx+tx-cuts[i+1])<1.1
        assert abs(uv[2]*my+ty-y0)<1.1 and abs(uv[3]*my+ty-y1)<1.1
        if i!=1:assert abs(sx-sy)<1/(us[i+1]-us[i])+1/65536
    for i in range(3):
        q=fills[i][1];assert d[q]==0
        b=Bits(d,q+1);assert b.read(4)==1 and b.read(4)==0
        assert b.read(1)==0 and b.read(5)==7
        n=b.read(5);x,y=b.signed(n),b.signed(n);assert b.read(1)==0 and b.read(1)==1
        points=[(x,y)]
        for _ in range(4):
            assert b.read(1)==1 and b.read(1)==1
            n=b.read(4)+2;assert b.read(1)==0
            vertical=b.read(1);delta=b.signed(n)
            if vertical:y+=delta
            else:x+=delta
            points.append((x,y))
        assert points==[(cuts[i],y0),(cuts[i+1],y0),(cuts[i+1],y1),(cuts[i],y1),(cuts[i],y0)]
        assert b.read(1)==0 and b.read(5)==(16 if i<2 else 0)

def verify(archive,folder):
    with zipfile.ZipFile(archive) as z:original=unpack(z.read(PREFIX+'dqmenus_droid.swf'))
    path=folder/'dqmenus_droid.swf';patched=unpack(path.read_bytes())
    report=json.loads((folder/'keyboard-atlas-repair.json').read_text());ids={x['shape'] for x in report['shapes']}
    assert hashlib.sha256(path.read_bytes()).hexdigest()==report['patched_sha256']
    a=list(tags(original));b=list(tags(patched));assert len(a)==len(b);changed=[]
    def without_matrices(d,p,l,t):
        bounds,fills=shape(d,p,t);chunks=[];q=p
        for start,end,bitmap,m in fills:chunks.append(d[q:start]);q=end
        chunks.append(d[q:p+l]);return bounds,b''.join(chunks)
    for (ta,sa,pa,la),(tb,sb,pb,lb) in zip(a,b):
        assert ta==tb
        identity=struct.unpack_from('<H',original,pa)[0] if ta in (2,22,32) else None
        if identity in ids:
            record=next(x for x in report['shapes'] if x['shape']==identity)
            if identity in (80,82) and record.get('slicing'):
                assert shape(original,pa,ta)[0]==shape(patched,pb,tb)[0]
                verify_space(patched,pb,tb,record)
            else:assert without_matrices(original,pa,la,ta)==without_matrices(patched,pb,lb,tb)
            changed.append(identity)
        else:assert original[pa:pa+la]==patched[pb:pb+lb]
    assert set(changed)==ids
    graph=inspect(path);assert graph['bounds_twips']==[0,9600,0,6400]
    return dict(status='PASS',scope='Five keyboard shapes and Exit shape507 change matrices only; two Space states use verified adjacent capsule slices within original bounds. Every action, placement and other tag is byte preserved',shapes=changed,tag_count=len(a),patched_sha256=report['patched_sha256'])

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--archive',type=Path,required=True);p.add_argument('--folder',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();r=verify(a.archive,a.folder);a.output.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
