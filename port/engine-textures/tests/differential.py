"""Execute original ARM32 header/pixel routines and the compiled ARM64 port.

Unicorn is a test oracle only. The APK never includes it or the original ELF.
The caller's IReadFile and logger, and imported libc/arithmetic helpers, are
modeled explicitly. Unknown imports fail. Native GPU behavior is not tested.
"""
import argparse
import hashlib
import json
from pathlib import Path
import random
import struct
import sys
import zipfile
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_INVALID
from unicorn.arm64_const import UC_ARM64_REG_TPIDR_EL0

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu as BaseCpu,u32,i32

class Cpu(BaseCpu):
    def __init__(self,path,arm64,provenance):
        super().__init__(path,arm64,provenance)
        self.file=b'';self.cursor=0;self.logs=0
        self.heap=self.data+0x400000
        if arm64:
            # Android TLS contains the stack canary at offset 0x28. Model the
            # caller's TLS, not engine logic; retain stack-protector checks.
            self.uc.reg_write(UC_ARM64_REG_TPIDR_EL0,self.data+0x8000)
            self.uc.mem_write(self.data+0x8028,struct.pack('<Q',0xD22026))
            with path.open('rb') as f:
                elf=ELFFile(f)
                for sec in elf.iter_sections():
                    if sec['sh_type']!='SHT_RELA':continue
                    syms=elf.get_section(sec['sh_link'])
                    for rel in sec.iter_relocations():
                        if rel['r_info_type']==1025:
                            sym=syms.get_symbol(rel['r_info_sym'])
                            if sym['st_shndx']!='SHN_UNDEF':self.pointer(self.base+rel['r_offset'],self.symbols[sym.name]+rel['r_addend'])
        self.uc.hook_add(UC_HOOK_MEM_INVALID,self.invalid)
        if not arm64:
            with path.open('rb') as f:
                elf=ELFFile(f)
                for sec in elf.iter_sections():
                    if sec['sh_type']!='SHT_REL':continue
                    syms=elf.get_section(sec['sh_link'])
                    for rel in sec.iter_relocations():
                        if rel['r_info_type']==21:
                            sym=syms.get_symbol(rel['r_info_sym'])
                            if sym['st_shndx']!='SHN_UNDEF':self.pointer(rel['r_offset'],sym['st_value'])
                            elif sym.name=='__stack_chk_guard':self.pointer(rel['r_offset'],self.data+0x9000)
            self.uc.hook_add(UC_HOOK_CODE,self.logger,begin=0x60b034,end=0x60b034)
            obj,table=self.data+0x1000,self.data+0x2000
            self.pointer(obj,table)
            for slot,name in ((12,'fixture_read'),(24,'fixture_seek'),(32,'fixture_size'),(36,'fixture_position'),(40,'fixture_name')):
                addr=self.extern+0xf000+slot*16;self.imports[addr]=name;self.pointer(table+slot,addr)
            self.uc.mem_write(self.data+0x3000,b'texture-fixture\0')

    def invalid(self,uc,access,address,size,value,unused):
        raise AssertionError(f'Invalid memory access at PC {uc.reg_read(self.pc):#x}: {address:#x}, size {size}, ARM64={self.arm64}')

    def logger(self,uc,address,size,unused):
        self.logs+=1;self.put(0,0);self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))

    def external(self,uc,address,size,unused):
        name=self.imports.get(address,'')
        if name.startswith('fixture_'):
            if name=='fixture_read':
                chunk=self.file[self.cursor:self.cursor+self.reg(2)];self.cursor+=len(chunk)
                if chunk:self.uc.mem_write(self.reg(1),chunk)
                self.put(0,len(chunk))
            elif name=='fixture_seek':
                pos=i32(self.reg(1))+(self.cursor if self.reg(2) else 0)
                valid=0<=pos<=len(self.file)
                if valid:self.cursor=pos
                self.put(0,int(valid))
            elif name=='fixture_size':self.put(0,len(self.file))
            elif name=='fixture_position':self.put(0,self.cursor)
            else:self.put(0,self.data+0x3000)
        elif name in ('_Znwm','_Znam','malloc'):
            size=self.reg(0);assert size<=0x1000000
            address=self.heap;self.heap+=(size+15)&~15
            assert self.heap<self.data+0x2000000
            self.put(0,address)
        elif name in ('_ZdlPv','_ZdaPv','_ZdlPvm','_ZdaPvm','free'):
            pass
        elif name in ('memcmp','strncmp'):
            a,b,n=self.reg(0),self.reg(1),self.reg(2)
            aa,bb=bytes(self.uc.mem_read(a,n)),bytes(self.uc.mem_read(b,n))
            if name=='strncmp':aa,bb=aa.split(b'\0',1)[0],bb.split(b'\0',1)[0]
            self.put(0,0 if aa==bb else -1 if aa<bb else 1)
        elif name in ('__aeabi_uidiv','__aeabi_uidivmod'):
            a,b=u32(self.reg(0)),u32(self.reg(1));assert b
            self.put(0,a//b)
            if name.endswith('mod'):self.put(1,a%b)
        elif name in ('__aeabi_idiv','__aeabi_idivmod'):
            a,b=i32(self.reg(0)),i32(self.reg(1));assert b
            quotient=abs(a)//abs(b)*(1 if (a<0)==(b<0) else -1)
            self.put(0,quotient)
            if name.endswith('mod'):self.put(1,a-quotient*b)
        else:return super().external(uc,address,size,unused)
        self.import_calls[name]=self.import_calls.get(name,0)+1
        self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))

def main():
    p=argparse.ArgumentParser()
    p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True)
    p.add_argument('--cache',type=Path,required=True);p.add_argument('--report',type=Path,required=True)
    a=p.parse_args();manifest=json.loads((ROOT/'original-functions.json').read_text())
    assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
    old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
    checks=0;valid=0;pixel_cases=[];mismatches=[];safety_differences=[]
    def header(raw,label,safer=False):
        nonlocal checks,valid
        old.file=raw;old.cursor=0
        output_old=old.data+0x4000;output_new=new.data+0x4000;input_new=new.data+0x10000
        old.uc.mem_write(output_old,b'\0'*32);new.uc.mem_write(output_new,b'\0'*32);new.uc.mem_write(input_new,raw)
        original=old.invoke(0x605b10,[0,old.data+0x1000,output_old])
        rebuilt=new.invoke('dh2_pvr_describe',[input_new,len(raw),output_new])
        if safer:
            assert original and not rebuilt,label
            safety_differences.append({'case':label,'original_accepts':True,'rebuilt_accepts':False,'reason':'Zero dimensions are rejected before decoding'})
            return
        assert bool(original)==bool(rebuilt),(label,original,rebuilt)
        if original:
            assert bytes(old.uc.mem_read(output_old,32))==bytes(new.uc.mem_read(output_new,32)),label
            valid+=1
        checks+=1
    def pixels(raw,label):
        h=struct.unpack_from('<13I',raw,8);height,width=h[1:3];two=int((h[4]&255)==24)
        source_old=old.data+0x10000;output_old=old.data+0x100000
        source_new=new.data+0x10000;output_new=new.data+0x100000
        payload=raw[60:];n=width*height*4
        old.uc.mem_write(source_old,payload);new.uc.mem_write(source_new,payload)
        old.uc.mem_write(output_old,b'\xcc'*n);new.uc.mem_write(output_new,b'\xcc'*n)
        old.invoke('_Z15PVRTCDecompressPKviiiPh',[source_old,two,width,height,output_old],budget=100000000)
        assert new.invoke('dh2_pvrtc_decompress',[source_new,len(payload),two,width,height,output_new,n],budget=100000000)
        original=bytes(old.uc.mem_read(output_old,n));rebuilt=bytes(new.uc.mem_read(output_new,n))
        different=sum(x!=y for x,y in zip(original,rebuilt))
        row={'case':label,'width':width,'height':height,'two_bpp':bool(two),'compared_bytes':n,'different_bytes':different,
             'original_sha256':hashlib.sha256(original).hexdigest(),'rebuilt_sha256':hashlib.sha256(rebuilt).hexdigest()}
        pixel_cases.append(row)
        if different:mismatches.append(row)
    with zipfile.ZipFile(a.cache) as archive:
        textures=[(e.filename,archive.read(e)) for e in archive.infolist() if e.filename.endswith('.tga')]
    pvrs=[(name,raw) for name,raw in textures if raw.startswith(b'BTEXpvr\0')]
    for name,raw in pvrs:header(raw,name)
    fixture=pvrs[0][1]
    for length in (0,1,7,8,12,30,59):header(fixture[:length],f'truncate-{length}')
    for offset,value in ((8,51),(52,0),(48,0),(12,0),(16,0),(28,1),(24,0xff),(56,6)):
        raw=bytearray(fixture);struct.pack_into('<I',raw,offset,value);header(bytes(raw),f'change-{offset}-{value}',offset in (12,16))
    for kind in (0,1,2,4,5,7,8,12,13,16,17,18,19,21,22,23,24,25,26,32,33,34,35,36,42,57,59,79,80,82,83,86,255):
        for flags in (kind,kind|0x8000,kind|0x200):
            h=(52,8,8,0,flags,32,4,0,0,0,1,0x21525650,1)
            sample=struct.pack('<13I',*h)+bytes(32)
            header(sample,f'raw-format-{flags}')
    for flags,surfaces,mips in ((0x1019,6,0),(0x1019,1,0),(0x4019,4,0),(0x119,1,3),(0x119,1,2),(0x119,1,0),(0x4119,4,3)):
        h=(52,8,8,mips,flags,32,4,0,0,0,0,0x21525650,surfaces)
        sample=b'BTEXpvr\0'+struct.pack('<13I',*h)+bytes(32*(6 if flags&0x1000 else 1))
        header(sample,f'features-{flags}-{surfaces}-{mips}')
    for name,raw in sorted(pvrs,key=lambda r:len(r[1]))[:4]:pixels(raw,name)
    # Real 2bpp assets are all 1024 square. Use their leading compressed words
    # in a small labelled fixture; the corpus audit decodes the full images.
    name,raw=next((name,raw) for name,raw in pvrs if (struct.unpack_from('<I',raw,24)[0]&255)==24)
    h=list(struct.unpack_from('<13I',raw,8));h[1]=h[2]=16;h[5]=64
    sample=b'BTEXpvr\0'+struct.pack('<13I',*h)+raw[60:124]
    header(sample,'real-pvrtc2-payload-miniature');pixels(sample,'real-pvrtc2-payload-miniature')
    for width,height,two,seed in ((16,8,True,1),(32,16,True,2),(8,8,False,3),(16,8,False,4),(8,16,False,5)):
        n=width*height*(2 if two else 4)//8;rng=random.Random(seed)
        payload=bytes(rng.randrange(256) for _ in range(n))
        h=(52,height,width,0,0x8200|(24 if two else 25),n,2 if two else 4,0,0,0,1,0x21525650,1)
        sample=b'BTEXpvr\0'+struct.pack('<13I',*h)+payload
        header(sample,f'synthetic-{width}-{height}-{two}-{seed}');pixels(sample,f'synthetic-{width}-{height}-{two}-{seed}')
    for name,raw in pvrs:
        h=struct.unpack_from('<13I',raw,8)
        if (h[4]&255)==24:
            # Small synthetic input follows the real texture's header/format.
            h=list(h);h[1]=h[2]=16;h[5]=64
            rng=random.Random(0xD22026);data=bytes(rng.randrange(256) for _ in range(64))
            sample=b'BTEXpvr\0'+struct.pack('<13I',*h)+data
            header(sample,'synthetic-pvrtc2');pixels(sample,'synthetic-pvrtc2');break
    report={'header_comparisons':checks,'valid_descriptions':valid,'pixel_comparisons':pixel_cases,
            'pixel_mismatches':len(mismatches),'intentional_safety_differences':safety_differences,'original_sha256':manifest['original_sha256'],
            'compiled_arm64_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),
            'original_instructions_seen':len(old.seen),'import_calls':old.import_calls,'gpu_execution':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='pixel_comparisons'}))
    assert not mismatches,'Rebuilt pixel decoder differs from original; inspect recorded cases'
if __name__=='__main__':main()
