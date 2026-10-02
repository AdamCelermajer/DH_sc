#!/usr/bin/env python3
"""Compare range-only, null-callback original timeline state with ARM64/host."""
import argparse, ctypes as c, hashlib, json, math, struct, sys
import importlib.util
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('animation_value_cpu',ROOT/'../animation-values/tests/differential.py')
value_cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(value_cpu)
EngineCpu,Dependencies,ORIGINAL_SHA256=value_cpu.EngineCpu,value_cpu.Dependencies,value_cpu.ORIGINAL_SHA256

class State(c.Structure):
    _fields_=[(x,c.c_int32)for x in ('current_ms','start_ms','end_ms')]+[('loop',c.c_uint32)]+[
        (x,c.c_float)for x in ('delta_magnitude','start_seconds','length_seconds','last_input_seconds','current_seconds','scale')]+[
        ('finished',c.c_uint32),('started',c.c_uint32)]

class TimelineDependencies(Dependencies):
    def __init__(self,path):
        super().__init__(path)
        libm=c.CDLL('libm.so.6');fn=libm.fmodf
        fn.argtypes=[c.c_float,c.c_float];fn.restype=c.c_float
        self.libm=libm;self.functions['fmodf']=(fn,'f',2)
    def call(self,cpu,name):
        short=name.removeprefix('__aeabi_')
        if short=='i2f':cpu.put_float(c.c_float(c.c_int32(cpu.reg(0)).value).value,'f')
        elif short=='f2iz':cpu.write_reg(0,int(cpu.get_float('f',0))&0xffffffff)
        else:return super().call(cpu,name)
        cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg))

ROWS=[(0x666c20,8,'setScale'),(0x666c28,8,'setLoop'),(0x666c38,140,'setRange'),
      (0x666f10,52,'jumpTo'),(0x667104,508,'update')]
def main():
    p=argparse.ArgumentParser()
    for key in ('original','arm64','host','oracle','report'):p.add_argument('--'+key,type=Path,required=True)
    a=p.parse_args();sha=lambda x:hashlib.sha256(x.read_bytes()).hexdigest()
    assert sha(a.original)==ORIGINAL_SHA256
    evidence=[]
    with a.original.open('rb')as f:
        e=ELFFile(f);segments=[s for s in e.iter_segments()if s['p_type']=='PT_LOAD']
        syms={s['st_value']:s.name for s in e.get_section_by_name('.dynsym').iter_symbols()if s['st_shndx']!='SHN_UNDEF'}
        for address,size,name in ROWS:
            s=next(s for s in segments if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(s['p_offset']+address-s['p_vaddr'])
            evidence.append({'elf_address':f'0x{address:08x}','size':size,'name':name,
                             'symbol':syms[address],'sha256':hashlib.sha256(f.read(size)).hexdigest()})
    dep=TimelineDependencies(a.oracle);provenance={'functions':evidence}
    old=EngineCpu(a.original,False,dep,provenance);new=EngineCpu(a.arm64,True,dep,provenance)
    host=c.CDLL(str(a.host.resolve()))
    host.dh2_timeline_init.argtypes=[c.POINTER(State),c.c_int32,c.c_int32,c.c_float,c.c_bool]
    host.dh2_timeline_jump.argtypes=[c.POINTER(State),c.c_int32]
    host.dh2_timeline_update.argtypes=[c.POINTER(State),c.c_int32]
    for name in ('init','jump','update'):getattr(host,'dh2_timeline_'+name).restype=c.c_uint32
    op=old.data+0x400;np=new.data+0x400;cases=0;updates=0;jumps=0
    def original_state():
        data=bytes(old.uc.mem_read(op,64));w=lambda off:struct.unpack_from('<I',data,off)[0]
        values=[w(4),w(16),w(20),data[24],w(28),w(32),w(36),w(40),w(44),w(48),data[60],data[61]]
        return struct.pack('<12I',*values)
    def equal(x,y,label):
        xx=struct.unpack('<12I',x);yy=struct.unpack('<12I',y)
        assert all(v==w or (4<=i<=9 and v&0x7fffffff==w&0x7fffffff==0)
                   for i,(v,w)in enumerate(zip(xx,yy))), (label,xx,yy)
    for start,end in ((0,799),(-333,466),(100,1099),(0,1),(100001,300017)):
        for scale in (-3,-1,-.1,0,.1,1,3,64):
            for loop in (False,True):
                for jump in (start,start+(end-start)//2,end,end+100,start-100):
                    old.uc.mem_write(op,bytes(64));new.uc.mem_write(np,b'\xa5'*64)
                    state=State()
                    old.call(0x666c38,[op,start&0xffffffff,end&0xffffffff,0])
                    old.call(0x666f10,[op,start&0xffffffff])
                    old.call(0x666c20,[op,struct.unpack('<I',struct.pack('<f',scale))[0]])
                    old.call(0x666c28,[op,int(loop)])
                    assert host.dh2_timeline_init(c.byref(state),start,end,scale,loop)==0
                    assert new.call('dh2_timeline_init',[np,start&0xffffffff,end&0xffffffff,int(loop)],scale)==0
                    equal(original_state(),bytes(state),('init',start,end,scale,loop))
                    equal(original_state(),bytes(new.uc.mem_read(np,48)),'arm64 init')
                    old.call(0x666f10,[op,jump&0xffffffff])
                    assert host.dh2_timeline_jump(c.byref(state),jump)==0
                    assert new.call('dh2_timeline_jump',[np,jump&0xffffffff])==0
                    equal(original_state(),bytes(state),'jump');equal(original_state(),bytes(new.uc.mem_read(np,48)),'arm64 jump')
                    jumps+=1
                    for now in (0,17,333,800,1000,2500,2490,7000,7000,10000):
                        old.call(0x667104,[op,now])
                        assert host.dh2_timeline_update(c.byref(state),now)==0
                        assert new.call('dh2_timeline_update',[np,now])==0
                        equal(original_state(),bytes(state),('host update',start,end,scale,loop,jump,now))
                        equal(original_state(),bytes(new.uc.mem_read(np,48)),('arm64 update',start,end,scale,loop,jump,now))
                        assert bytes(new.uc.mem_read(np+48,16))==b'\xa5'*16
                        updates+=1
                    cases+=1
    r={'complete_game':False,'android_integrated':False,'callbacks_tested':False,'clip_library_tested':False,
       'scope':'Range-only timing, null original callback: forward/reverse, loop/clamp, jumps and repeated/backward timestamps',
       'comparison':'Original ARM32 vs compiled ARM64 and host state: exact bits except signed zero',
       'sequences':cases,'updates':updates,'jumps':jumps,'mismatches':0,'output_guards':True,
       'original_sha256':sha(a.original),'arm64_sha256':sha(a.arm64),'host_sha256':sha(a.host),
       'oracle_sha256':sha(a.oracle),'function_evidence':evidence,
       'main_instruction_addresses_seen':len(old.seen),'import_calls':old.import_calls,
       'test_sha256':sha(Path(__file__)),
       'source_sha256':{x.name:sha(x)for x in (ROOT/'timeline.cpp',ROOT/'timeline.hpp')},
       'dependency_model':'Host C arithmetic/libm; integer casts modeled over bounded valid values; actual original timeline executes'}
    a.report.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
if __name__=='__main__':main()
