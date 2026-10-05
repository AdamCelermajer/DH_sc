"""Record original loading atlas variants, fill tables and startup draw inputs.

This reads source resources only. It does not claim correct spinner rendering.
"""
import argparse
import hashlib
import json
import struct
import zipfile
import zlib
from pathlib import Path
from inspect_front_swf import Bits


def fills(raw, selected=None):
    data=raw[:8]+zlib.decompress(raw[8:]) if raw[:3]==b'CWS' else raw
    b=Bits(data,8);n=b.read(5);b.read(n*4);pos=b.offset()+4;out=[]
    while pos+2<=len(data):
        head=struct.unpack_from('<H',data,pos)[0];pos+=2
        tag,size=head>>6,head&63
        if size==63:size=struct.unpack_from('<I',data,pos)[0];pos+=4
        end=pos+size
        if end>len(data):raise ValueError('Tag outside original movie')
        if tag==2:
            identity=struct.unpack_from('<H',data,pos)[0]
            if selected is None or identity==selected:
                b=Bits(data,pos+2);n=b.read(5);bounds=[b.signed(n) for _ in range(4)]
                q=b.offset();count=data[q];q+=1;styles=[]
                for _ in range(count):
                    kind=data[q];q+=1
                    if kind not in (64,65,66,67):raise ValueError('Non-bitmap initial fill')
                    bitmap=struct.unpack_from('<H',data,q)[0];q+=2
                    b=Bits(data,q);scale=[1.,1.];skew=[0.,0.]
                    if b.read(1):n=b.read(5);scale=[b.signed(n)/65536 for _ in range(2)]
                    if b.read(1):n=b.read(5);skew=[b.signed(n)/65536 for _ in range(2)]
                    n=b.read(5);shift=[b.signed(n) for _ in range(2)];q=b.offset()
                    styles.append(dict(bitmap_id=bitmap,kind=kind,scale=scale,skew=skew,shift=shift))
                out.append(dict(shape=identity,bounds_twips=bounds,initial_fill_styles=styles))
        pos=end
        if tag==0:break
    return out


def main():
    p=argparse.ArgumentParser();p.add_argument('--archive',type=Path,required=True)
    p.add_argument('--elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();prefix='com.gameloft.android.GAND.GloftD2SS/files/data/'
    resources={}
    with zipfile.ZipFile(a.archive) as z:
        for name in ('3d/textures/splash_final.tga','3d/textures/splash_final_droid.tga',
                     'menus/loadanims.swf','menus/loadanims_droid.swf','menus/dqmenus_droid.swf'):
            resources[name]=z.read(prefix+name)
    raw=a.elf.read_bytes();ph=struct.unpack_from('<I',raw,28)[0]
    size,count=struct.unpack_from('<HH',raw,42)
    segments=[struct.unpack_from('<8I',raw,ph+i*size) for i in range(count)]
    def read(address,length):
        for s in segments:
            if s[0]==1 and s[2]<=address and address+length<=s[2]+s[4]:
                return raw[s[1]+address-s[2]:s[1]+address-s[2]+length]
        raise ValueError('Unmapped original address')
    word=lambda address:struct.unpack('<I',read(address,4))[0]
    assert word(0x384ab4)==0xe3a0cc05  # mov ip,#1280
    assert word(0x384ad0)==0xe3a0ce2f  # mov ip,#752
    assert read(0x381750,8)==bytes.fromhex('0100a0e31eff2fe1')
    name_address=(0x384fb8+8+word(0x3850bc))&0xffffffff
    source_name=read(name_address,150).split(b'\0')[0].decode('ascii')
    report=dict(scope='Source resource/provenance audit; spinner appearance incomplete',
                elf_sha256=hashlib.sha256(raw).hexdigest(),startup_source_rect=[0,0,1280,752],
                startup_default_texture=source_name,source_high_resolution_device=True,
                atlas_base_and_droid_identical=resources['3d/textures/splash_final.tga']==resources['3d/textures/splash_final_droid.tga'],
                resources={name:dict(bytes=len(b),sha256=hashlib.sha256(b).hexdigest()) for name,b in resources.items()},
                loading_fill_tables={name:fills(resources[name]) for name in ('menus/loadanims.swf','menus/loadanims_droid.swf')},
                exit_fill_table=fills(resources['menus/dqmenus_droid.swf'],507))
    a.output.write_text(json.dumps(report,indent=2)+'\n');print(source_name)


if __name__=='__main__':main()
