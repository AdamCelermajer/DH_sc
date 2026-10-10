"""Recover original PlaceObject2 ClipActions (not covered by DoAction decoder)."""
from pathlib import Path
import importlib.util,hashlib,json,struct,zlib
ROOT=Path(__file__).resolve().parents[5]
def module(name,path):
    s=importlib.util.spec_from_file_location(name,ROOT/path);m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
d=module('decoder','port/engine-ui/tools/swf_action_decoder_v1.py')
swf=module('geometry','port/windows-foundation/tools/export_hud_geometry.py')
p=ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqmenus.swf'
raw=p.read_bytes();data=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
b=d.Bits(data,8);b.skip(b.u(5)*4);out=[]
def tags(start,end,sprite='root'):
    at=start;frame=0
    while at+2<=end:
        h=struct.unpack_from('<H',data,at)[0];at+=2;k,n=h>>6,h&63
        if n==63:n=struct.unpack_from('<I',data,at)[0];at+=4
        stop=at+n
        if k==39:tags(at+4,stop,'sprite'+str(struct.unpack_from('<H',data,at)[0]))
        if k==1:frame+=1
        if k==26 and data[at]&128:
            t=data[at:stop];flags=t[0];q=3;cid=None;name=''
            if flags&2:cid=struct.unpack_from('<H',t,q)[0];q+=2
            if flags&4:_,q=swf.matrix(t,q)
            if flags&8:_,q=swf.cxform(t,q)
            if flags&16:q+=2
            if flags&32:e=t.index(0,q);name=t[q:e].decode();q=e+1
            if flags&64:q+=2
            q+=6 # reserved UI16, AllEventFlags UI32 for version8
            while q+4<=len(t):
                event=struct.unpack_from('<I',t,q)[0];q+=4
                if event==0:break
                size=struct.unpack_from('<I',t,q)[0];q+=4;e=q+size
                if event&0x20000:q+=1 # KeyPress code
                rows=d.decode(data,at+q,at+e,label='clip')
                out.append(dict(sprite=sprite,frame=frame,placement=name,character=cid,event_flags=event,offset=at+q,actions=rows))
                q=e
        at=stop
tags(b.end()+4,len(data))
Path(__file__).with_name('clip_actions_evidence.json').write_text(json.dumps(dict(source=str(p.relative_to(ROOT)),sha256=hashlib.sha256(raw).hexdigest(),clip_actions=out),indent=2)+'\n',encoding='utf-8')
for t in out:
    values=[v.get('text') for r in t['actions'] for v in r.get('values',[]) if isinstance(v,dict) and v.get('text')]
    print(t['sprite'],t['placement'],t['character'],t['event_flags'],t['offset'],values)
