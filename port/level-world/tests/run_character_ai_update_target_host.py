"""Replay original _UpdateTarget caller with explicit synchronous callee boundaries."""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-update-target/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
A,B,P,Q=0x10014000,0x10018000,0x10020000,0x10024000
AI=A+0x3c8;VT,INTERACTIVE,DEAD,CAN_RANGE=0x10040000,0x10041000,0x10041010,0x10041020
FSM,ACTIVE=0x10042000,0x10043000
def cases():
    rows=[];base=[0,0,1,0,1,1,0,0,0,1,0,0,0,1,1]
    def add(name,changes=None):
        x=base.copy()
        for i,v in (changes or {}).items():x[i]=v
        rows.append((name,x))
    add('alive_sight_melee_edges')
    add('awaiting_skips',{0:1});add('limbus_skips',{1:1});add('null_current_keeps_independent_last',{13:0})
    add('null_current_and_last',{13:0,14:0});add('untargetable_clears_both',{2:0})
    add('dead_stays_without_event',{3:1});add('death_edge',{3:1,11:1})
    add('revival_edge_noncanonical_previous',{11:7,3:1})
    add('alive_noncanonical_previous_no_false_edge',{11:7})
    add('no_sight_no_range',{4:0});add('sight_lost_edge',{4:0,12:1})
    add('sight_remains_noncanonical_previous',{12:7});add('second_interactive_reject',{5:0})
    add('close_event',{6:1,7:1});add('ranged_event',{6:1,8:1});add('ranged_out_event',{6:1,8:0})
    add('melee_out_event',{9:0});add('noncanonical_ranged_close',{6:7,7:0xffffffff})
    for name,v in [('two',2),('byte_255',255),('word_256',256),('word_minus_one',0xffffffff)]:add('dead_raw_'+name,{3:v})
    for name,v in [('word_256',256),('word_minus_one',0xffffffff),('word_256_previous_truthy',256)]:add('sight_raw_'+name,{4:v,**({12:1} if name.endswith('truthy') else {})})
    add('null_last_does_not_get_synchronized',{14:0})
    for i in range(1,24):
        changes={10:i,6:1,8:1}
        if i==11:changes.update({3:1,11:1})
        if i==12:changes.update({4:0,12:1})
        add('source_callback_mutation_'+str(i),changes)
    add('range_provider_clears_current',{10:25,6:1})
    return rows

def original_comparison(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    m=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        e=ELFFile(f);syms={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()};loads=[p for p in e.iter_segments() if p['p_type']=='PT_LOAD']
        def data(at,n):
            seg=next(p for p in loads if p['p_vaddr']<=at and at+n<=p['p_vaddr']+p['p_filesz']);offset=int(seg['p_offset'])+at-int(seg['p_vaddr']);return raw[offset:offset+n]
        for row in m['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']]
            assert (int(s['st_value']),int(s['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in m['vtable_ranges']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['symbol']]
            assert at==int(s['st_value']) and n<=int(s['st_size'])
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
            for slot in row['slots']:
                assert struct.unpack('<I',data(at+8+int(slot['byte_offset'],0),4))[0]==int(slot['target'],0)
                assert int(syms[slot['symbol']]['st_value'])==int(slot['target'],0)
    old=Cpu(original,False,m);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def byte(at):return old.uc.mem_read(at,1)[0]
    def setbyte(at,n):old.uc.mem_write(at,bytes([n&255]))
    def returned(n):old.put(0,n);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    # Execute the actual named FSM leaves; the subtraction returns ZERO at state1.
    leaves=[]
    for state_id in (0,1,2,17,18,0xffffffff):
        old.pointer(FSM+0x20,ACTIVE);old.pointer(ACTIVE,state_id)
        await_value=old.invoke(0x3c0230,[FSM]);limbus_value=old.invoke(0x3c01c0,[FSM])
        assert await_value==int(state_id==17) and limbus_value==int(state_id==0),(state_id,await_value,limbus_value)
        leaves.append({'state_id':state_id,'awaiting':await_value,'limbus':limbus_value})
    records=[]
    for name,x in cases():
        awaiting,limbus,interactive1,dead,sight,interactive2,ranged,close,range_value,melee,mutation,alive_prev,sight_prev,present,last=x
        old.pointer(AI+4,A);old.pointer(AI+0x40,P if present else 0);old.pointer(AI+0x44,Q if last else 0)
        setbyte(AI+0x48,alive_prev);setbyte(AI+0x49,sight_prev)
        for actor in (A,B,P,Q):old.pointer(actor,VT)
        old.pointer(VT+0x88,INTERACTIVE);old.pointer(VT+0x34,DEAD);old.pointer(VT+0x124,CAN_RANGE)
        calls=[];interactive_calls=[0]
        def service(op,kind,subject,other=0,event=0):
            calls.append([op,kind,subject,other,event,byte(AI+0x48),byte(AI+0x49)])
            if ((mutation==1 and op==0) or (mutation==2 and op==2 and interactive_calls[0]==1) or
                (mutation==3 and op==3) or (mutation==4 and op==4) or (mutation==5 and op==5) or
                (mutation==6 and op==10 and event==11) or (mutation==7 and op==10 and event==13) or
                (mutation==8 and op==6) or (mutation==9 and op==7)):old.pointer(AI+4,B)
            if ((mutation==10 and op==2 and interactive_calls[0]==1) or
                (mutation==11 and op==10 and event==10) or (mutation==12 and op==10 and event==12)):
                old.pointer(AI+0x40,0);old.pointer(AI+0x44,0)
            if ((mutation==13 and op==3) or (mutation==14 and op==4) or
                (mutation==15 and op==10 and event==11) or (mutation==16 and op==10 and event==13) or
                (mutation==17 and op==2 and interactive_calls[0]==2) or
                (mutation==18 and op==6) or (mutation==19 and op==7)):old.pointer(AI+0x40,Q)
            if mutation==20 and op==10 and event==11:setbyte(AI+0x48,7)
            if mutation==21 and op==10 and event==13:setbyte(AI+0x49,7)
            if mutation==22 and op==4:setbyte(AI+0x48,1)
            if mutation==23 and op==5:setbyte(AI+0x49,1)
            if mutation==25 and op==7:old.pointer(AI+0x40,0)
        def observe(_,at,__,___):
            if at==0x3c0230:service(0,0,old.reg(0)-0x4fc);returned(awaiting)
            elif at==0x3c01c0:service(1,0,old.reg(0)-0x4fc);returned(limbus)
            elif at==INTERACTIVE:
                interactive_calls[0]+=1;service(2,1,old.reg(0),old.reg(1));returned(interactive1 if interactive_calls[0]==1 else interactive2)
            elif at==0x3a2fec:service(3,0,old.reg(0));returned(0xffffffff)
            elif at==DEAD:service(4,1,old.reg(0));returned(dead)
            elif at==0x3d4ed8:service(5,2,old.reg(0),old.reg(1));returned(sight)
            elif at==CAN_RANGE:service(6,0,old.reg(0));returned(ranged)
            elif at==0x3d63d8:service(7,2,old.reg(0),old.reg(1));returned(close)
            elif at==0x3d6604:service(8,2,old.reg(0),old.reg(1));returned(range_value)
            elif at==0x3d6188:service(9,2,old.reg(0),old.reg(1));returned(melee)
            elif at==0x3a4d5c:service(10,0,old.reg(0),old.reg(2),old.reg(1));returned(0xffffffff)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        old.invoke(0x3cb908,[AI]);old.uc.hook_del(hook)
        expected={'status':0,'owner':get(AI+4),'target':get(AI+0x40),'last':get(AI+0x44),'alive':byte(AI+0x48),'sight':byte(AI+0x49),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True))
        assert actual==expected,(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
      'executed_scope':'Entire original 556B _UpdateTarget caller, including tail-event calls, fresh owner/target loads and direct target/snapshot writes. FSM/virtual/getter/sight/range/RaiseEvent callees are explicit synchronous modeled providers. Separate original FSM leaf checks execute GetState and both named predicates.',
      'fsm_original_leaf_cases':leaves,'vtable_address_point_delta':8,'native_wired':False}

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-ai-update-target/host.exe')
    p.add_argument('--report',type=Path,default=MODULE/'build/character-ai-update-target/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_ai_update_target.cpp',MODULE/'character_ai_sight.cpp',MODULE/'tests/character_ai_update_target.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    run=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if run.returncode:raise RuntimeError(run.stdout+run.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==59
    comparison=original_comparison(a.original_elf.resolve(),exe)
    paths=sources+[MODULE/'character_ai_update_target.hpp',MODULE/'character_ai_set_target.hpp',MODULE/'character_ai_sight.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,
      'compiler_command':command,'native_wired':False,'source_sha256':{x.relative_to(ROOT).as_posix():hashlib.sha256(x.read_bytes()).hexdigest() for x in paths}}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'fsm_leaf_cases':len(comparison['fsm_original_leaf_cases']),'mismatches':0}))
if __name__=='__main__':main()
