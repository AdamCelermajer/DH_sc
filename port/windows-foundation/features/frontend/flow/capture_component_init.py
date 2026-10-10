from pathlib import Path
import importlib.util,hashlib,json,struct,zlib
ROOT=Path(__file__).resolve().parents[5]
spec=importlib.util.spec_from_file_location('decoder',ROOT/'port/engine-ui/tools/swf_action_decoder_v1.py')
d=importlib.util.module_from_spec(spec);spec.loader.exec_module(d)
p=ROOT/'port/android-native/app/src/main/assets/original-cache/data/menus/dqmenus.swf'
raw=p.read_bytes();data=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
b=d.Bits(data,8);b.skip(b.u(5)*4);at=b.end()+4;out=[]
while at+2<=len(data):
    h=struct.unpack_from('<H',data,at)[0];at+=2;k,n=h>>6,h&63
    if n==63:n=struct.unpack_from('<I',data,at)[0];at+=4
    if k==59:out.append(dict(offset=at,sprite=struct.unpack_from('<H',data,at)[0],actions=d.decode(data,at+2,at+n,label='init')))
    at+=n
Path(__file__).with_name('component_init_evidence.json').write_text(json.dumps(dict(source=str(p.relative_to(ROOT)),sha256=hashlib.sha256(raw).hexdigest(),init_actions=out),indent=2)+'\n',encoding='utf-8')
for t in out:
    print(t['sprite'],t['offset'],[r['constants'] for r in t['actions'] if 'constants' in r])
