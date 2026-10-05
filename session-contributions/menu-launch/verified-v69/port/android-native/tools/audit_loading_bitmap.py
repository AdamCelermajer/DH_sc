"""Compare the authored loading fill with the real native bitmap draw."""
import argparse, hashlib, json, re, struct, zlib
from pathlib import Path
from PIL import Image
from inspect_front_swf import Bits

def audit(swf,atlas,native_log):
    raw=swf.read_bytes();data=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
    bits=Bits(data,8);n=bits.read(5);movie=[bits.signed(n) for _ in range(4)];pos=bits.offset()+4
    shape=None;export=None
    while pos<len(data):
        head=struct.unpack_from('<H',data,pos)[0];pos+=2;tag,size=head>>6,head&63
        if size==63:size=struct.unpack_from('<I',data,pos)[0];pos+=4
        if tag==56:
            count,identity=struct.unpack_from('<HH',data,pos)
            if count!=1:raise ValueError('Audit expects the original single loading atlas export')
            export=data[pos+4:pos+size].rstrip(b'\0').decode()
        if tag==2:
            bits=Bits(data,pos+2);n=bits.read(5);bounds=[bits.signed(n) for _ in range(4)];q=bits.offset()
            if data[q]!=1 or data[q+1] not in (64,65,66,67):raise ValueError('Original loading shape fill changed')
            bits=Bits(data,q+4);scale=[1.,1.];skew=[0.,0.]
            if bits.read(1):n=bits.read(5);scale=[bits.signed(n)/65536 for _ in range(2)]
            if bits.read(1):n=bits.read(5);skew=[bits.signed(n)/65536 for _ in range(2)]
            n=bits.read(5);shift=[bits.signed(n) for _ in range(2)]
            a,d=scale;c,b=skew;tx,ty=shift;det=a*d-b*c
            if not det:raise ValueError('Singular original fill matrix')
            inverse=[d/det,-b/det,(b*ty-d*tx)/det,-c/det,a/det,(c*tx-a*ty)/det]
            xmin,xmax,ymin,ymax=bounds
            points=[(inverse[0]*x+inverse[1]*y+inverse[2],inverse[3]*x+inverse[4]*y+inverse[5]) for x in (xmin,xmax) for y in (ymin,ymax)]
            shape=dict(bounds_twips=bounds,source_matrix=[a,b,tx,c,d,ty],inverse_matrix=inverse,texture_pixel_bounds=[min(x for x,y in points),max(x for x,y in points),min(y for x,y in points),max(y for x,y in points)])
        pos+=size
        if tag==0:break
    if shape is None or export is None:raise ValueError('Required original shape/export absent')
    with Image.open(atlas) as image:dimensions=image.size
    log=native_log.read_text()
    row=re.search(r'Original loading bitmap draw \| texture (\d+) (\d+) \| matrix (.*?) \| vertex ([\d.-]+) ([\d.-]+) \| texture pixel ([\d.-]+) ([\d.-]+)',log)
    if not row:raise ValueError('Required actual native draw absent')
    native_matrix=list(map(float,row[3].split()))
    if tuple(map(int,row.group(1,2)))!=dimensions:raise AssertionError('Native dimensions differ from the bundled atlas')
    if any(abs(a-b)>.0001 for a,b in zip(shape['inverse_matrix'],native_matrix)):raise AssertionError('Native matrix differs from authored inverse')
    return dict(scope='Authored loading bitmap matrix and actual draw correspondence; appearance incomplete',swf_sha256=hashlib.sha256(raw).hexdigest(),atlas_sha256=hashlib.sha256(atlas.read_bytes()).hexdigest(),movie_bounds=movie,export_name=export,atlas_dimensions=dimensions,shape=shape,native_draw=row[0],matrix_correspondence='PASS',full_loading_screen=False)

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--swf',type=Path,required=True);p.add_argument('--atlas',type=Path,required=True);p.add_argument('--native-log',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.write_text(json.dumps(audit(a.swf,a.atlas,a.native_log),indent=2)+'\n');print('PASS actual loading matrix matches authored data; appearance incomplete')
