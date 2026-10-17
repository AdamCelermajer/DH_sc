#!/usr/bin/env python3
"""Original animation extra-time/ending field calculations vs ARM64 and host."""
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
class Sample(c.Structure):_fields_=[('current_ms',c.c_int32),('delta_seconds',c.c_float),('current_seconds',c.c_float)]
class Notice(c.Structure):_fields_=[('extra_ms',c.c_int32),('pending',c.c_uint32)]
ROWS=[(0x3c90f8,112,'CharAnimator::CalculateExtraTime'),(0x366628,160,'AnimatorBlender::_HandleAnimEnding'),(0x366224,112,'Animator::_HandleAnimEnding')]
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
    deps=cpu.TimelineDependencies(a.oracle);provenance={'functions':evidence}
    old=cpu.EngineCpu(a.original,False,deps,provenance);new=cpu.EngineCpu(a.arm64,True,deps,provenance)
    host=c.CDLL(str(a.host.resolve()))
    host.dh2_animation_extra_time.argtypes=[c.POINTER(c.c_int32),c.POINTER(Sample)]
    host.dh2_animation_end_notice.argtypes=[c.POINTER(Notice),c.c_size_t,c.c_size_t,c.POINTER(Sample)]
    host.dh2_animation_extra_time.restype=host.dh2_animation_end_notice.restype=c.c_uint32
    obj=old.data+0x400;timeline=old.data+0x6000;np=new.data+0x400;sampleptr=new.data+0x800
    getter=old.stop+0x100;child=old.data+0x2000;table=old.data+0x1000;children=old.data+0x1800
    word=lambda address,value:old.uc.mem_write(address,struct.pack('<I',value&0xffffffff))
    word(child,table);word(table+0x44,getter);word(children,child)
    active=0;getters=0
    def get(uc,address,size,unused):
        nonlocal getters
        assert old.reg(0)==child
        old.write_reg(0,active);getters+=1
        uc.reg_write(old.pc_reg,uc.reg_read(old.lr_reg))
    old.uc.hook_add(UC_HOOK_CODE,get,begin=getter,end=getter)
    f32=lambda value:c.c_float(value).value
    signed=lambda bits:c.c_int32(bits).value
    samples=[]
    for delta in (-.1,0,.001,.016,.1,1,64):
        for seconds in (-123.456,-.001,-0.0,.001,1.099,100000.03125):
            elapsed=int(f32(f32(delta)*1000));millis=int(f32(f32(seconds)*1000))
            for difference in (-2,-1,0,1,elapsed-1,elapsed,elapsed+1):
                samples.append(Sample(signed(millis-difference),delta,seconds))
    rng=random.Random(20261002)
    for _ in range(200):
        samples.append(Sample(signed(rng.getrandbits(32)),rng.uniform(-1,100),rng.uniform(-100000,100000)))
    extra_checks=notice_checks=direct_checks=0
    for sample in [*samples,None]:
        old.uc.mem_write(timeline,bytes(64))
        if sample:
            word(timeline+4,sample.current_ms)
            old.uc.mem_write(timeline+0x1c,struct.pack('<f',sample.delta_seconds))
            old.uc.mem_write(timeline+0x2c,struct.pack('<f',sample.current_seconds))
            new.uc.mem_write(sampleptr,bytes(sample)+b'\xa5'*16)
        timeline_before=bytes(old.uc.mem_read(timeline,64))
        old.uc.mem_write(obj,b'\xa5'*0xd0);word(obj+0x44,987)
        new.uc.mem_write(np,struct.pack('<i',987)+b'\xa5'*16)
        extra=c.c_int32(987)
        old.call(0x3c90f8,[obj,timeline if sample else 0])
        assert host.dh2_animation_extra_time(c.byref(extra),c.byref(sample)if sample else None)==0
        assert new.call('dh2_animation_extra_time',[np,sampleptr if sample else 0])==0
        expected=struct.unpack('<i',old.uc.mem_read(obj+0x44,4))[0]
        assert extra.value==expected==struct.unpack('<i',new.uc.mem_read(np,4))[0]
        assert bytes(new.uc.mem_read(np+4,16))==b'\xa5'*16;extra_checks+=1
        for match in ('matching','different','matching-null','different-null'):
            active=timeline if sample and not match.endswith('null') else 0
            sender=active if match.startswith('matching')else timeline+64
            word(obj+0x28,children);word(obj+0x70,0);word(obj+0x98,987)
            old.uc.mem_write(obj+0xb8,b'\xff')
            notice=Notice(987,255);new.uc.mem_write(np,bytes(notice)+b'\xa5'*16)
            old.call(0x366628,[obj,sender])
            assert host.dh2_animation_end_notice(c.byref(notice),active,sender,c.byref(sample)if sample else None)==0
            assert new.call('dh2_animation_end_notice',[np,active,sender,sampleptr if sample else 0])==0
            expected=(struct.unpack('<i',old.uc.mem_read(obj+0x98,4))[0],old.uc.mem_read(obj+0xb8,1)[0])
            assert expected==(notice.extra_ms,notice.pending)==struct.unpack('<iI',new.uc.mem_read(np,8))
            assert bytes(new.uc.mem_read(np+8,16))==b'\xa5'*16;notice_checks+=1
        word(obj+0x68,987);old.uc.mem_write(obj+0x88,b'\xff')
        direct=Notice(987,255);new.uc.mem_write(np,bytes(direct)+b'\xa5'*16)
        sender=timeline if sample else 0
        old.call(0x366224,[obj,sender])
        assert host.dh2_animation_end_notice(c.byref(direct),sender,sender,c.byref(sample)if sample else None)==0
        assert new.call('dh2_animation_end_notice',[np,sender,sender,sampleptr if sample else 0])==0
        expected=(struct.unpack('<i',old.uc.mem_read(obj+0x68,4))[0],old.uc.mem_read(obj+0x88,1)[0])
        assert expected==(direct.extra_ms,direct.pending)==struct.unpack('<iI',new.uc.mem_read(np,8))
        assert bytes(new.uc.mem_read(np+8,16))==b'\xa5'*16;direct_checks+=1
        assert bytes(old.uc.mem_read(timeline,64))==timeline_before
        if sample:assert bytes(new.uc.mem_read(sampleptr,28))==bytes(sample)+b'\xa5'*16
    assert getters==notice_checks
    result={'complete_game':False,'android_integrated':False,'character_callback_effects_tested':False,
        'comparison':'Original ARM32 vs compiled ARM64/host extra-time and pending fields: exact integers',
        'extra_time_checks':extra_checks,'blender_notice_checks':notice_checks,'direct_animator_notice_checks':direct_checks,'active_getter_calls':getters,'mismatches':0,
        'input_preservation':True,'output_guards':True,'stack_restoration':True,'seed':20261002,
        'original_sha256':sha(a.original),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'oracle_sha256':sha(a.oracle),
        'function_evidence':evidence,'source_sha256':{x.name:sha(x)for x in (ROOT/'ending.cpp',ROOT/'ending.hpp')},
        'test_sha256':sha(Path(__file__)),'import_calls':old.import_calls,
        'dependency_model':'Actual original extra-time and ending bodies execute; active timeline getter is a controlled pointer-return stub; bounded float arithmetic/casts use the existing host dependency model'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
