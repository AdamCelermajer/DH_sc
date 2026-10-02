#!/usr/bin/env python3
"""Explicit-position original AnimApplicator delta state vs ARM64 and host."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import random
import struct
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('value_cpu',ROOT/'../animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
class Vector(c.Structure):_fields_=[(x,c.c_float)for x in ('x','y','z')]
class State(c.Structure):_fields_=[('timestamp',c.c_uint32),('previous',Vector),('delta',Vector)]
ROWS=[(0x3644cc,48,'ResetDelta(position)'),(0x364444,136,'CalculateDelta(position)')]
def main():
    p=argparse.ArgumentParser()
    for key in ('original','host','arm64','oracle','report'):p.add_argument('--'+key,type=Path,required=True)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    assert sha(a.original)==cpu.ORIGINAL_SHA256
    evidence=[]
    with a.original.open('rb')as stream:
        elf=ELFFile(stream);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        symbols={s['st_value']:s.name for s in elf.get_section_by_name('.dynsym').iter_symbols()if s['st_shndx']!='SHN_UNDEF'}
        for address,size,name in ROWS:
            segment=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            stream.seek(segment['p_offset']+address-segment['p_vaddr'])
            evidence.append({'elf_address':f'0x{address:08x}','size':size,'name':name,'symbol':symbols[address],
                             'sha256':hashlib.sha256(stream.read(size)).hexdigest()})
    deps=cpu.Dependencies(a.oracle);provenance={'functions':evidence}
    old=cpu.EngineCpu(a.original,False,deps,provenance);new=cpu.EngineCpu(a.arm64,True,deps,provenance)
    host=c.CDLL(str(a.host.resolve()))
    for name in ('reset','calculate'):
        fn=getattr(host,'dh2_motion_'+name);fn.argtypes=[c.POINTER(State),c.c_uint32,c.POINTER(Vector)];fn.restype=c.c_uint32
    obj=old.data+0x400;np=new.data+0x400
    def equal(expected,actual,label):
        x=struct.unpack('<7I',expected);y=struct.unpack('<7I',actual)
        assert all(v==w or (i>0 and v&0x7fffffff==w&0x7fffffff==0)
                   for i,(v,w)in enumerate(zip(x,y))),(label,x,y)
    rng=random.Random(20261002);resets=updates=same_timestamp=0
    for case in range(400):
        s=State();old.uc.mem_write(obj,b'\xa5'*64);new.uc.mem_write(np,b'\xa5'*44)
        for step,timestamp in enumerate((0,1,1,1,4,4,0xfffffffe,0xffffffff,0,0,10,9)):
            position=Vector(*(rng.uniform(-100000,100000)for _ in range(3)))
            if case==0:position=Vector(float(step),float(-step),-0.0)
            reset=step in (0,7);name='reset'if reset else'calculate'
            repeated=not reset and s.timestamp==timestamp
            for machine in (old,new):machine.uc.mem_write(machine.data+0x800,bytes(position)+b'\xa5'*16)
            old.call(ROWS[0 if reset else 1][0],[obj,timestamp,old.data+0x800])
            assert getattr(host,'dh2_motion_'+name)(c.byref(s),timestamp,c.byref(position))==0
            assert new.call('dh2_motion_'+name,[np,timestamp,new.data+0x800])==0
            expected=bytes(old.uc.mem_read(obj+0x14,28))
            equal(expected,bytes(s),('host',case,step));equal(expected,bytes(new.uc.mem_read(np,28)),('arm64',case,step))
            if repeated:
                assert all(v==0 for v in (s.delta.x,s.delta.y,s.delta.z));same_timestamp+=1
                assert bytes(s.previous)==bytes(position)
            assert bytes(old.uc.mem_read(obj,0x14))==b'\xa5'*0x14
            assert bytes(old.uc.mem_read(obj+0x30,16))==b'\xa5'*16
            assert bytes(new.uc.mem_read(np+28,16))==b'\xa5'*16
            for machine in (old,new):assert bytes(machine.uc.mem_read(machine.data+0x800,28))==bytes(position)+b'\xa5'*16
            if reset:resets+=1
            else:updates+=1
    result={'complete_game':False,'android_integrated':False,'root_track_sampling_tested':False,
        'comparison':'Original ARM32 vs compiled ARM64 and host state: exact bits except signed zero',
        'scope':'Explicit-position ResetDelta and CalculateDelta, not node/track overloads',
        'sequences':400,'reset_calls':resets,'calculate_calls':updates,'same_timestamp_checks':same_timestamp,
        'mismatches':0,'input_preservation':True,'output_guards':True,'stack_restoration':True,'seed':20261002,
        'original_sha256':sha(a.original),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'oracle_sha256':sha(a.oracle),
        'function_evidence':evidence,'source_sha256':{x.name:sha(x)for x in (ROOT/'motion.cpp',ROOT/'motion.hpp',ROOT/'../engine-math/math.hpp')},
        'test_sha256':sha(Path(__file__)),'import_calls':old.import_calls,
        'dependency_model':'Actual original explicit-position methods execute; float subtraction uses existing host C arithmetic dependency model'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
