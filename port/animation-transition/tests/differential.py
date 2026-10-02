#!/usr/bin/env python3
"""Original transition state and dispatch ordering, with explicit object stubs."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import random
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('timeline_cpu',ROOT/'../animation-timeline/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
class State(c.Structure):
    _fields_=[('weights',c.c_float*8)]+[(x,c.c_uint32)for x in ('count','current','previous')]+[
        ('requested_duration',c.c_int32),('remaining',c.c_int32),('inverse_duration',c.c_float),('last_timestamp',c.c_uint32)]
class Dependencies(cpu.TimelineDependencies):
    def call(self,machine,name):
        if name in ('__aeabi_idivmod','__aeabi_uidivmod'):
            a,b=(c.c_int32(machine.reg(i)).value if name=='__aeabi_idivmod' else machine.reg(i) for i in range(2))
            assert b!=0
            q=abs(a)//abs(b)*(1 if (a<0)==(b<0) else -1)
            machine.write_reg(0,q&0xffffffff);machine.write_reg(1,(a-q*b)&0xffffffff)
            machine.uc.reg_write(machine.pc_reg,machine.uc.reg_read(machine.lr_reg))
        else:super().call(machine,name)
ROWS=[(0x36679c,236,'Blend'),(0x366d90,296,'updateTime'),(0x366594,140,'normalizeWeights')]
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
    deps=Dependencies(a.oracle);provenance={'functions':evidence}
    old=cpu.EngineCpu(a.original,False,deps,provenance);new=cpu.EngineCpu(a.arm64,True,deps,provenance)
    host=c.CDLL(str(a.host.resolve()))
    host.dh2_transition_begin.argtypes=[c.POINTER(State),c.c_int32]
    host.dh2_transition_update.argtypes=[c.POINTER(State),c.c_uint32,c.POINTER(c.c_uint32)]
    for name in ('begin','update'):getattr(host,'dh2_transition_'+name).restype=c.c_uint32
    obj=old.data+0x400;np=new.data+0x400;maskptr=new.data+0x800
    weights=old.data+0x3000;children=old.data+0x1800;vtable=old.data+0x1000
    update_stub=old.stop+0x100;getter_stub=old.stop+0x104
    trace=[]
    def stub(uc,address,size,unused):
        if address not in (update_stub,getter_stub,0x36440c):return
        if address==update_stub:
            index=(old.reg(0)-(old.data+0x2000))//64
            trace.append(('update',index,old.reg(1)))
        elif address==getter_stub:
            index=(old.reg(0)-(old.data+0x2000))//64
            trace.append(('get',index));old.write_reg(0,old.data+0x6000+64*index)
        else:trace.append(('check_callback',old.reg(1)))
        uc.reg_write(old.pc_reg,uc.reg_read(old.lr_reg))
    old.uc.hook_add(UC_HOOK_CODE,stub,begin=update_stub,end=getter_stub)
    old.uc.hook_add(UC_HOOK_CODE,stub,begin=0x36440c,end=0x36440c)
    def word(address,value):old.uc.mem_write(address,struct.pack('<I',value&0xffffffff))
    word(vtable+0x14,update_stub);word(vtable+0x44,getter_stub)
    for i in range(8):
        child=old.data+0x2000+64*i;word(child,vtable);word(children+4*i,child)
    def install(s):
        old.uc.mem_write(obj,bytes(0xd0));word(obj+0x28,children);word(obj+0x2c,children+4*s.count)
        word(obj+0x34,weights);word(obj+0x38,weights+4*s.count)
        old.uc.mem_write(weights,bytes(s.weights)+b'\xa5'*16)
        old.uc.mem_write(obj+0x70,bytes(s)[36:])
        new.uc.mem_write(np,bytes(s)+b'\xa5'*16)
    def projection(s):
        return bytes(old.uc.mem_read(weights,32))+struct.pack('<I',s.count)+bytes(old.uc.mem_read(obj+0x70,24))
    def equal(expected,actual,label):
        x=struct.unpack('<15I',expected);y=struct.unpack('<15I',actual)
        assert all(v==w or ((i<8 or i==13)and v&0x7fffffff==w&0x7fffffff==0)
                   for i,(v,w)in enumerate(zip(x,y))),(label,x,y)
    rng=random.Random(20261002);sequences=begins=updates=0
    for count in (1,2,3,8):
        for case in range(100):
            s=State();s.count=count;s.current=case%count;s.previous=(case+1)%count
            for i in range(count):s.weights[i]=rng.choice((0,0,.25,.75,1,-1))
            s.requested_duration=rng.choice((0,1,80,500,10000,2147483647))
            s.remaining=rng.choice((-1,0,1,50,500));s.inverse_duration=c.c_float(1/80).value
            s.last_timestamp=rng.choice((0,100,0xfffffff0))
            install(s)
            for step,now in enumerate((0,1,20,40,40,80,79,500,10000,0xfffffffe,4,100)):
                if count==2 and step in (0,4,8):
                    duration=rng.choice((-500,0,1,80,500,10000,2147483647))
                    old.call(0x36679c,[obj,duration&0xffffffff])
                    assert host.dh2_transition_begin(c.byref(s),duration)==0
                    assert new.call('dh2_transition_begin',[np,duration&0xffffffff])==0
                    equal(projection(s),bytes(s),('begin host',case,step))
                    equal(projection(s),bytes(new.uc.mem_read(np,60)),('begin arm64',case,step));begins+=1
                trace.clear();old.call(0x366d90,[obj,now])
                active=c.c_uint32(0xa5a5a5a5)
                assert host.dh2_transition_update(c.byref(s),now,c.byref(active))==0
                new.uc.mem_write(maskptr,b'\xa5'*20)
                assert new.call('dh2_transition_update',[np,now,maskptr])==0
                expected=projection(s)
                equal(expected,bytes(s),('update host',count,case,step))
                equal(expected,bytes(new.uc.mem_read(np,60)),('update arm64',count,case,step))
                expected_trace=[('update',i,now)for i in range(count)if active.value&(1<<i)]+[
                    ('get',s.current),('check_callback',old.data+0x6000+64*s.current)]
                assert trace==expected_trace,(trace,expected_trace)
                assert struct.unpack('<I',new.uc.mem_read(maskptr,4))[0]==active.value
                assert bytes(new.uc.mem_read(maskptr+4,16))==b'\xa5'*16
                assert bytes(new.uc.mem_read(np+60,16))==b'\xa5'*16
                assert bytes(old.uc.mem_read(weights+32,16))==b'\xa5'*16
                updates+=1
            sequences+=1
    result={'complete_game':False,'android_integrated':False,'full_animator_equivalence':False,
        'comparison':'Original ARM32 vs compiled ARM64 and host projected state: exact bits except signed zero',
        'sequences':sequences,'begin_calls':begins,'updates':updates,'dispatch_order_checks':updates,
        'mismatches':0,'output_guards':True,'stack_restoration':True,'seed':20261002,
        'original_sha256':sha(a.original),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'oracle_sha256':sha(a.oracle),
        'function_evidence':evidence,'source_sha256':{x.name:sha(x)for x in (ROOT/'transition.cpp',ROOT/'transition.hpp')},
        'test_sha256':sha(Path(__file__)),'import_calls':old.import_calls,
        'dependency_model':'Actual original Blend/updateTime/normalization execute; host arithmetic and signed/unsigned division; child update/getter and CheckCallback explicitly stubbed without mutation',
        'child_timelines_tested':False,'callback_check_implementation_tested':False,'events_tested':False}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
