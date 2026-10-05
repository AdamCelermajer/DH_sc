"""Build a separate compatibility SWF for the supplied, mismatched menu atlas.

Original cache files are preserved. Four mappings come from corresponding key
states in dqmenus.swf. Shift/Space use identified artwork in its embedded atlas;
those two mappings are compatibility choices, not recovered Android coordinates.
Five key shapes and the Exit power icon change only fill matrices. Space's two states use three adjacent
bitmap rectangles to preserve the original capsule caps. Layout/actions stay intact.
"""
import argparse, hashlib, json, struct, zipfile, zlib
from pathlib import Path
from inspect_front_swf import Bits

PREFIX = 'com.gameloft.android.GAND.GloftD2SS/files/data/menus/'
ANDROID_SHA = 'c114c7d39b7fe0351a93a515c78f9457dda715d78aed6e3bcc3f4895120de421'

def unpack(raw):
    d = raw[:8] + zlib.decompress(raw[8:]) if raw[:3] == b'CWS' else raw
    assert len(d) == struct.unpack_from('<I', d, 4)[0]
    return d

def tags(d):
    b=Bits(d,8);n=b.read(5);b.read(n*4);p=b.offset()+4
    while p+2<=len(d):
        start=p;v=struct.unpack_from('<H',d,p)[0];p+=2;t,l=v>>6,v&63
        if l==63:l=struct.unpack_from('<I',d,p)[0];p+=4
        assert p+l<=len(d)
        yield t,start,p,l
        p+=l
        if t==0:break

def matrix(d,p):
    b=Bits(d,p);scale=[1.,1.];skew=[0.,0.]
    if b.read(1):n=b.read(5);scale=[b.signed(n)/65536 for _ in range(2)]
    if b.read(1):n=b.read(5);skew=[b.signed(n)/65536 for _ in range(2)]
    n=b.read(5);shift=[b.signed(n) for _ in range(2)]
    return dict(scale=scale,skew=skew,shift=shift),b.offset()

def shape(d,p,tag=2):
    b=Bits(d,p+2);n=b.read(5);bounds=[b.signed(n) for _ in range(4)]
    q=b.offset();fills=[]
    def styles(q):
        count=d[q];q+=1
        if count==255:count=struct.unpack_from('<H',d,q)[0];q+=2
        for _ in range(count):
            assert d[q] in (64,65,66,67)
            bitmap=struct.unpack_from('<H',d,q+1)[0];start=q+3
            m,q=matrix(d,start);fills.append((start,q,bitmap,m))
        lines=d[q];q+=1
        if lines==255:lines=struct.unpack_from('<H',d,q)[0];q+=2
        q+=lines*(2+(4 if tag==32 else 3))
        return q
    q=styles(q);b=Bits(d,q);nf,nl=b.read(4),b.read(4)
    while True:
        edge=b.read(1)
        if edge:
            straight=b.read(1);n=b.read(4)+2
            if straight:
                general=b.read(1)
                if general:b.read(n*2)
                else:b.read(1);b.read(n)
            else:b.read(n*4)
        else:
            flags=b.read(5)
            if not flags:break
            if flags&1:n=b.read(5);b.read(n*2)
            if flags&2:b.read(nf)
            if flags&4:b.read(nf)
            if flags&8:b.read(nl)
            if flags&16:
                q=styles(b.offset());b=Bits(d,q);nf,nl=b.read(4),b.read(4)
    return bounds,fills

def encode_matrix(scale,shift):
    bits=[]
    def put(v,n):bits.extend((v>>k)&1 for k in range(n-1,-1,-1))
    def signed(values):
        n=max(1,max(abs(v).bit_length()+1 for v in values));assert n<=31
        put(n,5)
        for v in values:put(v & ((1<<n)-1),n)
    put(1,1);signed([round(x*65536) for x in scale]);put(0,1);signed([round(x) for x in shift])
    bits.extend([0]*((-len(bits))%8))
    return bytes(sum(bits[p+k]<<(7-k) for k in range(8)) for p in range(0,len(bits),8))

def space_slices(original,p,bounds,uv):
    """Stretch the capsule's middle while scaling each end uniformly by height."""
    x0,x1,y0,y1=bounds;u0,u1,v0,v1=uv
    sy=(y1-y0)/(v1-v0)
    # Leave a two-pixel strip for the stretch; preserve the curved original ends.
    middle=(u0+u1)/2
    cuts=[u0,middle-1,middle+1,u1]
    xs=[x0,round(x0+(cuts[1]-u0)*sy),round(x1-(u1-cuts[2])*sy),x1]
    assert xs==sorted(xs) and xs[1]<xs[2]
    b=Bits(original,p+2);n=b.read(5);b.read(n*4)
    prefix=bytearray(original[p:b.offset()]);styles=[];matrices=[]
    for i in range(3):
        sx=(xs[i+1]-xs[i])/(cuts[i+1]-cuts[i])
        shift=[xs[i]-cuts[i]*sx,y0-v0*sy]
        styles.append(b'\x01\x43\x01\x00'+encode_matrix([sx,sy],shift)+b'\x00')
        matrices.append(dict(scale=[sx,sy],shift=shift))
    payload=prefix+styles[0]
    bits=[]
    def put(v,n):bits.extend((v>>k)&1 for k in range(n-1,-1,-1))
    def signed(v,n):put(v & ((1<<n)-1),n)
    put(1,4);put(0,4) # One fill in each separate tessellation layer.
    for i in range(3):
        if i:
            put(0,1);put(16,5) # NewStyles starts a separate shape layer.
            bits.extend([0]*((-len(bits))%8))
            for byte in styles[i]:put(byte,8)
            put(1,4);put(0,4)
        put(0,1);put(7,5) # MoveTo; explicitly clear FillStyle0 and set FillStyle1.
        n=max(abs(xs[i]).bit_length(),abs(y0).bit_length())+1
        put(n,5);signed(xs[i],n);signed(y0,n);put(0,1);put(1,1)
        for dx,dy in [(xs[i+1]-xs[i],0),(0,y1-y0),(xs[i]-xs[i+1],0),(0,y0-y1)]:
            delta=dx or dy;n=max(2,abs(delta).bit_length()+1)
            put(1,1);put(1,1);put(n-2,4);put(0,1);put(int(dx==0),1);signed(delta,n)
    put(0,1);put(0,5)
    bits.extend([0]*((-len(bits))%8))
    payload.extend(bytes(sum(bits[p+k]<<(7-k) for k in range(8)) for p in range(0,len(bits),8)))
    return payload,dict(mode='three-slice capsule',x_cuts_twips=xs,u_cuts_pixels=cuts,matrices=matrices)

def artwork_bounds(generic,box):
    # Inspect pixels only; the original atlas is never edited or regenerated.
    for t,_,p,l in tags(generic):
        if t in (20,36) and struct.unpack_from('<H',generic,p)[0]==1:
            _,fmt,w,h=struct.unpack_from('<HBHH',generic,p);assert fmt==5
            argb=zlib.decompress(generic[p+7:p+l]);assert len(argb)==w*h*4
            pts=[(x,y) for y in range(box[1],box[3]) for x in range(box[0],box[2])
                 if max(argb[(y*w+x)*4+1:(y*w+x)*4+4])>8]
            assert pts
            return [min(x for x,y in pts),max(x for x,y in pts)+1,min(y for x,y in pts),max(y for x,y in pts)+1]
    raise ValueError('Missing original embedded atlas')

def build(archive,destination):
    with zipfile.ZipFile(archive) as z:
        raw=z.read(PREFIX+'dqmenus_droid.swf');generic_raw=z.read(PREFIX+'dqmenus.swf')
    assert hashlib.sha256(raw).hexdigest()==ANDROID_SHA
    original=unpack(raw);generic=unpack(generic_raw)
    source_shapes={struct.unpack_from('<H',generic,p)[0]:shape(generic,p,t)
                   for t,_,p,l in tags(generic) if t in (2,22,32) and struct.unpack_from('<H',generic,p)[0] in [73,75,78,80,494]}
    mappings={}
    for target,source in [(70,73),(72,75),(75,78),(77,80),(507,494)]:
        b,f=source_shapes[source];m=f[0][3];assert f[0][2]==1 and m['skew']==[0.,0.]
        sx,sy=m['scale'];tx,ty=m['shift']
        mappings[target]=([ (b[0]-tx)/sx,(b[1]-tx)/sx,(b[2]-ty)/sy,(b[3]-ty)/sy ],f'dqmenus.swf shape {source}')
    for target,box,label in [(85,(300,960,380,1024),'Shift up-arrow'),(80,(850,978,930,1024),'Space blank key'),(82,(930,978,1024,1024),'Space glow')]:
        mappings[target]=(artwork_bounds(generic,box),'Identified original atlas artwork: '+label)
    pieces=[];cursor=0;records=[]
    for t,start,p,l in tags(original):
        if t not in (2,22,32):continue
        identity=struct.unpack_from('<H',original,p)[0]
        if identity not in mappings:continue
        bounds,fills=shape(original,p,t);uv,source=mappings[identity]
        sx=(bounds[1]-bounds[0])/(uv[1]-uv[0]);sy=(bounds[3]-bounds[2])/(uv[3]-uv[2])
        shift=[bounds[0]-uv[0]*sx,bounds[2]-uv[2]*sy]
        slicing=None
        if identity in (80,82):
            payload,slicing=space_slices(original,p,bounds,uv)
        else:
            replacement=encode_matrix([sx,sy],shift);payload=bytearray();q=p
            for a,b,bitmap,m in fills:
                assert bitmap==1
                payload.extend(original[q:a]);payload.extend(replacement);q=b
            payload.extend(original[q:p+l])
        tag=struct.pack('<HI',(t<<6)|63,len(payload))+payload
        pieces.extend([original[cursor:start],tag]);cursor=p+l
        records.append(dict(shape=identity,bounds_twips=bounds,source=source,atlas_pixel_bounds=uv,fill_count=3 if slicing else len(fills),slicing=slicing))
    assert {r['shape'] for r in records}==set(mappings)
    pieces.append(original[cursor:]);patched=bytearray(b''.join(pieces));patched[:3]=b'FWS';struct.pack_into('<I',patched,4,len(patched))
    destination.mkdir(parents=True,exist_ok=True);(destination/'dqmenus_droid.swf').write_bytes(patched)
    report=dict(scope='Keyboard and Exit icon atlas compatibility repair; original Android mapping unresolved',original_sha256=ANDROID_SHA,
                generic_sha256=hashlib.sha256(generic_raw).hexdigest(),patched_sha256=hashlib.sha256(patched).hexdigest(),shapes=records)
    (destination/'keyboard-atlas-repair.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--archive',type=Path,required=True);p.add_argument('--destination',type=Path,required=True)
    a=p.parse_args();build(a.archive,a.destination)
