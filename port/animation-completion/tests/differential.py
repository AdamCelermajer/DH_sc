#!/usr/bin/env python3
"""Original completion setter/dispatch with controlled callback effects."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('value_cpu',ROOT/'../animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
Callback=c.CFUNCTYPE(None,c.c_size_t,c.c_void_p)
class State(c.Structure):_fields_=[('callback',c.c_void_p),('context',c.c_void_p),('pending',c.c_uint32)]
ROWS=[(0x364400,12,'SetCallback'),(0x36440c,56,'CheckCallback')]
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
    host.dh2_completion_set.argtypes=[c.POINTER(State),c.c_void_p,c.c_void_p]
    host.dh2_completion_check.argtypes=[c.POINTER(State),c.c_size_t]
    for name in ('set','check'):getattr(host,'dh2_completion_'+name).restype=c.c_uint32
    obj=old.data+0x400;np=new.data+0x400
    old_fn=old.stop+0x100;new_fn=new.stop+0x100
    traces={'old':[],'new':[],'host':[]};mode=0;s=State()
    def hook(machine,label):
        def called(uc,address,size,unused):
            traces[label].append((machine.reg(0),machine.reg(1)))
            if mode&1:
                if label=='old':uc.mem_write(obj+0x30,b'\xff')
                else:uc.mem_write(np+16,struct.pack('<I',255))
            if mode&2:
                if label=='old':uc.mem_write(obj+0x34,struct.pack('<II',0,0xabcdef))
                else:uc.mem_write(np,struct.pack('<QQ',0,0xabcdef))
            uc.reg_write(machine.pc_reg,uc.reg_read(machine.lr_reg))
        return called
    old.uc.hook_add(UC_HOOK_CODE,hook(old,'old'),begin=old_fn,end=old_fn)
    new.uc.hook_add(UC_HOOK_CODE,hook(new,'new'),begin=new_fn,end=new_fn)
    @Callback
    def callback(timeline,context):
        traces['host'].append((timeline,context or 0))
        if mode&1:s.pending=255
        if mode&2:s.callback=None;s.context=0xabcdef
    host_fn=c.cast(callback,c.c_void_p).value
    def projected_old():
        pending=old.uc.mem_read(obj+0x30,1)[0]
        fn,context=struct.unpack('<II',old.uc.mem_read(obj+0x34,8))
        return bool(fn),context,pending
    def projected_new():
        fn,context,pending=struct.unpack('<QQI',new.uc.mem_read(np,20))
        return bool(fn),context,pending
    def projected_host():return bool(s.callback),s.context or 0,s.pending
    checks=sets=dispatches=0
    for pending in (0,1,2,255):
        for enabled in (False,True):
            for context in (0,123,0xffffffff):
                for timeline in (0,123,0xffffffff,0x123456789abcdef0):
                    for mode in (0,1,2,3):
                        old.uc.mem_write(obj,b'\xa5'*64)
                        old.uc.mem_write(obj+0x30,bytes([pending]))
                        new.uc.mem_write(np,bytes(c.sizeof(State))+b'\xa5'*16)
                        s=State();s.pending=pending
                        new.uc.mem_write(np+16,struct.pack('<I',pending))
                        old.call(0x364400,[obj,old_fn if enabled else 0,context])
                        assert host.dh2_completion_set(c.byref(s),host_fn if enabled else None,context)==0
                        assert new.call('dh2_completion_set',[np,new_fn if enabled else 0,context])==0
                        assert projected_old()==projected_new()==projected_host()==(enabled,context,pending)
                        sets+=1
                        for trace in traces.values():trace.clear()
                        old.call(0x36440c,[obj,timeline&0xffffffff])
                        assert host.dh2_completion_check(c.byref(s),timeline)==0
                        assert new.call('dh2_completion_check',[np,timeline])==0
                        fired=bool(pending and enabled)
                        assert traces['old']==([(timeline&0xffffffff,context)]if fired else[])
                        assert traces['new']==traces['host']==([(timeline,context)]if fired else[])
                        assert projected_old()==projected_new()==projected_host()
                        if fired:
                            assert s.pending==0
                            if mode&2:assert not s.callback and s.context==0xabcdef
                            dispatches+=1
                        else:assert s.pending==pending
                        # A cleared notification does not fire twice; a missing
                        # callback keeps the pending flag for later registration.
                        before=projected_host()
                        old.call(0x36440c,[obj,timeline&0xffffffff])
                        assert host.dh2_completion_check(c.byref(s),timeline)==0
                        assert new.call('dh2_completion_check',[np,timeline])==0
                        assert projected_old()==projected_new()==projected_host()==before
                        assert len(traces['old'])==len(traces['new'])==len(traces['host'])==int(fired)
                        assert bytes(old.uc.mem_read(obj,0x30))==b'\xa5'*0x30
                        assert bytes(old.uc.mem_read(obj+0x3c,4))==b'\xa5'*4
                        assert bytes(new.uc.mem_read(np+c.sizeof(State),16))==b'\xa5'*16
                        checks+=2
    result={'complete_game':False,'android_integrated':False,'game_callbacks_tested':False,
        'comparison':'Original ARM32 dispatch/fields vs compiled ARM64 and host; pointer presence/context/pending projected across ABI widths',
        'setter_calls':sets,'check_calls':checks,'controlled_dispatches':dispatches,'mismatches':0,
        'controlled_callback_reentry_fields':True,'clear_after_callback':True,'missing_callback_retains_pending':True,
        'wide_timeline_handles':True,'wide_handle_mapping':'Original receives low 32 bits; ARM64/host receive full uintptr_t',
        'output_guards':True,'stack_restoration':True,'original_sha256':sha(a.original),'host_sha256':sha(a.host),
        'arm64_sha256':sha(a.arm64),'oracle_sha256':sha(a.oracle),'function_evidence':evidence,
        'source_sha256':{x.name:sha(x)for x in (ROOT/'completion.cpp',ROOT/'completion.hpp')},
        'test_sha256':sha(Path(__file__)),'dependency_model':'Actual original SetCallback/CheckCallback execute; callback bodies are controlled test effects, not the original game callbacks'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
