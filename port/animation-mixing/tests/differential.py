#!/usr/bin/env python3
"""Original ARM32 normalization/mixing instructions vs compiled ARM64 and host."""
import argparse
from collections import Counter
import ctypes as c
import hashlib
import importlib.util
import json
import math
from pathlib import Path
import random
import struct
from elftools.elf.elffile import ELFFile

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('animation_value_cpu',ROOT/'../animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
ROWS=[(0x366594,140,'normalize'),(0x626b4c,228,'position blend'),
      (0x62a754,228,'position add'),(0x6275fc,228,'scale blend'),(0x62b204,228,'scale add'),
      (0x6e3124,96,'scalar blend'),(0x6e3184,96,'scalar add'),(0x613378,528,'quaternion add')]

def main():
    p=argparse.ArgumentParser()
    for key in ('original','host','arm64','oracle','report'):p.add_argument('--'+key,type=Path,required=True)
    a=p.parse_args();sha=lambda x:hashlib.sha256(x.read_bytes()).hexdigest()
    assert sha(a.original)==cpu.ORIGINAL_SHA256
    evidence=[]
    with a.original.open('rb')as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        syms={s['st_value']:s.name for s in elf.get_section_by_name('.dynsym').iter_symbols()if s['st_shndx']!='SHN_UNDEF'}
        for address,size,name in ROWS:
            s=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(s['p_offset']+address-s['p_vaddr'])
            evidence.append({'elf_address':f'0x{address:08x}','size':size,'name':name,'symbol':syms[address],
                             'sha256':hashlib.sha256(f.read(size)).hexdigest()})
    deps=cpu.Dependencies(a.oracle);provenance={'functions':evidence}
    old=cpu.EngineCpu(a.original,False,deps,provenance);new=cpu.EngineCpu(a.arm64,True,deps,provenance)
    host=c.CDLL(str(a.host.resolve()))
    host.dh2_animation_weights_normalize.argtypes=[c.c_void_p,c.c_uint32]
    for kind in ('vector_mix','scalar_mix','quaternion_add'):
        fn=getattr(host,'dh2_animation_'+kind);fn.argtypes=[c.c_void_p,c.c_void_p,c.c_uint32,c.c_void_p];fn.restype=c.c_uint32
    host.dh2_animation_weights_normalize.restype=c.c_uint32
    rng=random.Random(20261002);calls=Counter()
    def equal(x,y,label):
        xx=struct.unpack('<'+'I'*(len(x)//4),x);yy=struct.unpack('<'+'I'*(len(y)//4),y)
        assert all(v==w or v&0x7fffffff==w&0x7fffffff==0 for v,w in zip(xx,yy)),(label,xx,yy)
    for count in (0,1,2,3,8,32,256):
        for case in range(20 if count<=8 else 3):
            weights=[rng.choice([0,-1,-.5,.25,.75,1,1.5])for _ in range(count)]
            if case==0:weights=[0]*count
            if case==1 and count>=2:weights=[1,-1]+[0]*(count-2)
            cw=(c.c_float*count)(*weights)
            for machine in (old,new):
                machine.uc.mem_write(machine.data+0x3000,bytes(cw)+b'\xa5'*16)
            obj=old.data+0x100
            old.uc.mem_write(obj,bytes(64))
            old.uc.mem_write(obj+0x34,struct.pack('<II',old.data+0x3000,old.data+0x3000+count*4))
            old.call(0x366594,[obj])
            assert host.dh2_animation_weights_normalize(cw,count)==0
            assert new.call('dh2_animation_weights_normalize',[new.data+0x3000,count])==0
            expected=bytes(old.uc.mem_read(old.data+0x3000,count*4))
            equal(expected,bytes(cw),('normalize host',count,case))
            equal(expected,bytes(new.uc.mem_read(new.data+0x3000,count*4)),('normalize arm64',count,case))
            for machine in (old,new):assert bytes(machine.uc.mem_read(machine.data+0x3000+count*4,16))==b'\xa5'*16
            calls['normalize']+=1
            for address,size,name in ROWS[1:]:
                quaternion=name=='quaternion add'
                if quaternion and count>8:continue
                components=4 if quaternion else 1 if name.startswith('scalar') else 3
                values=[]
                for _ in range(count):
                    v=[rng.uniform(-1,1)for _ in range(components)]
                    if quaternion:
                        norm=math.sqrt(sum(x*x for x in v));v=[x/norm for x in v]
                    values.extend(v)
                cv=(c.c_float*(components*count))(*values);cw=(c.c_float*count)(*weights)
                co=(c.c_float*components)();nbytes=components*4
                for machine in (old,new):
                    machine.uc.mem_write(machine.data+0x1000,bytes(cv))
                    machine.uc.mem_write(machine.data+0x3000,bytes(cw))
                    machine.uc.mem_write(machine.data+0x100,b'\xa5'*32)
                kind='quaternion_add' if quaternion else 'scalar_mix' if components==1 else 'vector_mix'
                fn=getattr(host,'dh2_animation_'+kind);assert fn(cv,cw,count,co)==0
                args=[old.data+0x1000,old.data+0x3000,count,old.data+0x100]
                old.call(address,args if quaternion else [0,*args])
                assert new.call('dh2_animation_'+kind,[new.data+0x1000,new.data+0x3000,count,new.data+0x100])==0
                expected=bytes(old.uc.mem_read(old.data+0x100,nbytes))
                equal(expected,bytes(co),(name,'host',count,case))
                equal(expected,bytes(new.uc.mem_read(new.data+0x100,nbytes)),(name,'arm64',count,case))
                for machine in (old,new):
                    assert bytes(machine.uc.mem_read(machine.data+0x100+nbytes,16))==b'\xa5'*16
                    assert bytes(machine.uc.mem_read(machine.data+0x1000,len(bytes(cv))))==bytes(cv)
                    assert bytes(machine.uc.mem_read(machine.data+0x3000,len(bytes(cw))))==bytes(cw)
                calls[name]+=1
    r={'complete_game':False,'android_integrated':False,'state_machine':False,
       'comparison':'Original ARM32 vs compiled ARM64 and host: exact float bits except signed zero',
       'original_sha256':sha(a.original),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'oracle_sha256':sha(a.oracle),
       'seed':20261002,'calls':dict(calls),'comparisons':sum(calls.values()),'mismatches':0,
       'source_sha256':{x.name:sha(x)for x in (ROOT/'mixing.cpp',ROOT/'mixing.hpp',ROOT/'../engine-math/math.cpp',ROOT/'../engine-math/math.hpp')},
       'test_sha256':sha(Path(__file__)),'function_evidence':evidence,'main_instruction_addresses_seen':len(old.seen),
       'input_preservation':True,'output_guards':True,'stack_restoration':True,'import_calls':old.import_calls,
       'dependency_model':'Host C arithmetic/libm; actual original normalization/mixing and quaternion math execute'}
    a.report.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
if __name__=='__main__':main()
