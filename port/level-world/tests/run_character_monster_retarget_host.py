"""Compare bounded local-Monster retarget caller branches with original ARM."""
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

MODULE=Path(__file__).resolve().parents[1]; ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-monster-retarget/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
OWNER,OWNER_B=0x10014000,0x10018000
AI,PEER,CURRENT=OWNER+0x3c8,0x10020000,0x10024000
DESIGN,DESIGN_VAR,STACK=0x10030000,0x10031000,0x1005e000
def word(value):
    try: return struct.unpack('<I',struct.pack('<f',value))[0]
    except OverflowError: return 0xff800000 if value<0 else 0x7f800000
def number(value): return struct.unpack('<f',struct.pack('<I',value))[0]
def nan(value): return value&0x7f800000==0x7f800000 and value&0x7fffff!=0
def cases():
    base=[PEER,CURRENT,word(10),word(20),word(1.5),0,0,0]; rows=[]
    def add(name,changes=None):
        values=base.copy()
        for i,v in (changes or {}).items(): values[i]=v
        rows.append((name,values))
    add('switch_highest_strictly_greater'); add('strict_boundary_keeps',{3:word(15)}); add('lower_keeps',{3:word(14)})
    add('equal_identities_clear',{0:CURRENT}); add('no_highest_clear',{0:0}); add('unresolved_current_clear',{1:0}); add('both_null_clear',{0:0,1:0})
    add('equal_current_enemy_search_boundary',{0:CURRENT,5:7}); add('no_highest_enemy_search_boundary',{0:0,5:1}); add('null_current_enemy_fallback_boundary',{1:0,5:1})
    for name,v in [('plus_zero',0),('minus_zero',0x80000000),('negative',word(-10)),('nan',0x7fc00001),('infinity',0x7f800000)]: add('current_threat_'+name,{2:v})
    for name,v in [('plus_zero',0),('minus_zero',0x80000000),('negative',word(-1)),('nan',0x7fc00001),('infinity',0x7f800000)]: add('factor_'+name,{4:v})
    add('highest_nan',{3:0x7fc00001}); add('infinity_equal_keeps',{2:0x7f800000,3:0x7f800000})
    add('multiply_overflow_keeps',{2:word(1e20),4:word(1e20),3:0x7f800000})
    add('debug_noncanonical_return_ignored',{7:0xffffffff})
    for mutation in range(1,10): add('source_ownership_mutation_'+str(mutation),{6:mutation,**({0:0} if mutation>=7 else {})})
    return rows

def oracle(original,executable):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests')); from cpu import Cpu
    raw=original.read_bytes(); assert hashlib.sha256(raw).hexdigest()==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        e=ELFFile(f); symbols={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()}; loads=[s for s in e.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,size):
            s=next(s for s in loads if s['p_vaddr']<=at and at+size<=s['p_vaddr']+s['p_filesz']); offset=int(s['p_offset'])+at-int(s['p_vaddr']); return raw[offset:offset+size]
        for row in manifest['functions']:
            at,size=int(row['elf_address'],0),row['size']; s=symbols[row['original_symbol']]
            assert (int(s['st_value']),int(s['st_size']))==(at,size)
            assert hashlib.sha256(data(at,size)).hexdigest()==row['sha256']
        got=0x3cf408+struct.unpack('<I',data(0x3cfbb0,4))[0]
        factor_offset=struct.unpack('<I',data(0x3cfbb8,4))[0]
        debug_offset=struct.unpack('<I',data(0x3cfbbc,4))[0]
    old=Cpu(original,False,manifest); old.uc.mem_map(0x10000000,0x60000)
    def get(at): return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    records=[]
    for name,values in cases():
        highest,resolved,current_threat,highest_threat,factor,enemy,mutation,debug=values
        old.uc.mem_write(STACK-0x2000,bytes(0x2000)); old.pointer(AI+4,OWNER)
        old.pointer(OWNER+0x408,CURRENT); old.pointer(OWNER_B+0x408,CURRENT); old.pointer(AI+0x44,0x10028000)
        old.pointer(got+factor_offset,DESIGN_VAR); old.pointer(DESIGN_VAR,DESIGN); old.pointer(DESIGN+8,factor)
        old.pointer(got+debug_offset,0x10032000)
        calls,end=[],[]; aggro_count=[0]; captured_target_owner=[0]; threshold=[0]; floats=[]
        def returned(value=0): old.put(0,value); old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
        def call(op,kind,subject,peer=0,force=0):
            calls.append([op,kind,subject,peer,force])
            if ((mutation==1 and op==0) or (mutation==2 and op==1) or
                (mutation==3 and op==2 and aggro_count[0]==1) or (mutation==4 and op==2 and aggro_count[0]==2) or
                (mutation==5 and op==3) or (mutation==6 and op==4) or (mutation==7 and op==6) or (mutation==8 and op==7)):
                old.pointer(AI+4,OWNER_B)
        def observe(_,address,__,___):
            if address==0x3cf430:
                decision=3 if any(c[0]==8 for c in calls) else 2 if any(c[0]==5 for c in calls) else 1
                end.append((0,decision)); old.uc.emu_stop()
            elif address==0x3cf9a8:
                end.append((5,4)); old.uc.emu_stop()
            elif address==0x3d4a18:
                call(0,0,old.reg(0)-0x3c8); returned(highest)
            elif address==0x3cf4c8: captured_target_owner[0]=old.reg(3)
            elif address==0x33dd2c:
                assert old.reg(1)==CURRENT
                # Genuine nonconst GetHandle struct-return boundary is observed.
                returned(old.reg(0))
            elif address==0x33ff54:
                call(1,0,captured_target_owner[0]); returned(resolved)
            elif address==0x3d4ac8:
                aggro_count[0]+=1; call(2,0,old.reg(0)-0x3c8,old.reg(1))
                returned(current_threat if aggro_count[0]==1 else highest_threat)
            elif address==0x3cf52c: call(3,2,0)
            elif address in (0x30ed6c,0x30e2f8):
                a,b=old.reg(0),old.reg(1)
                output=word(number(a)*number(b)) if address==0x30ed6c else int(number(a)>number(b))
                floats.append(['mul' if address==0x30ed6c else 'gt',a,b,output]); returned(output)
            elif address==0x3cf534: threshold[0]=old.reg(0)
            elif address in (0x337888,0x3140ec,0x318254): returned()
            elif address==0x337a88: call(4,2,0); returned(debug)
            elif address==0x3d6890:
                original=old.uc.reg_read(old.lr)==0x3cfb2c
                subject=old.reg(0) if original else old.reg(0)-0x3c8
                call(5,1 if original else 0,subject,old.reg(1),old.reg(2))
                old.pointer(old.reg(0)+0x40,old.reg(1))
                if mutation==9: old.pointer(AI+0x40,PEER)
                returned()
            elif address==0x3d574c: call(6,0,old.reg(0)-0x3c8,old.reg(1)); returned(enemy)
            elif address==0x3d6d68: call(7,1,old.reg(0),old.reg(1)); returned()
            elif address==0x3d49c4:
                call(8,1,old.reg(0))
                # Execute real AI_SyncLastTarget ldr40/str44/bx instructions.
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        old.put(0,OWNER); old.put(4,got); old.put(5,AI); old.put(10,0)
        old.uc.reg_write(old.sp,STACK); old.uc.reg_write(old.lr,old.stop)
        old.uc.emu_start(0x3cf4b4,old.stop,count=1500); old.uc.hook_del(hook)
        assert len(end)==1,(name,end)
        expected={'status':end[0][0],'decision':end[0][1],'threshold':threshold[0],'owner':get(AI+4),
                  'target_a':get(OWNER+0x408),'target_b':get(OWNER_B+0x408),'last_a':get(AI+0x44),'calls':calls}
        actual=json.loads(subprocess.check_output([str(executable),*map(str,values)],text=True))
        assert actual['threshold']==expected['threshold'] or nan(actual['threshold']) and nan(expected['threshold']),(name,actual,expected)
        assert {k:v for k,v in actual.items() if k!='threshold'}=={k:v for k,v in expected.items() if k!='threshold'},(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'modeled_float_trace':floats})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
            'executed_scope':'Original local-Monster target branch through threat-switch return, non-enemy clear/SetTarget/real SyncLastTarget, or first enemy-retention search boundary. No retention TargetList/property-registry enumeration.',
            'external_services':'HighestAggro, GetHandle/Character conversion, GetAggro, diagnostic switch, IsEnemy, ClearAggro and SetTarget observed at exact call boundaries; fmul/fcmpgt PLT modeled with IEEE binary32; NaN payload/sign excluded.'}

def main():
    p=argparse.ArgumentParser(description=__doc__); p.add_argument('--compiler'); p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-monster-retarget/host.exe'); p.add_argument('--report',type=Path,default=MODULE/'build/character-monster-retarget/validation.json'); a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler: p.error('pass --compiler')
    output=a.output.resolve(); output.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_monster_retarget.cpp',MODULE/'tests/character_monster_retarget.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-ffp-contract=off',*map(str,sources),'-o',str(output)]
    subprocess.run(command,cwd=ROOT,check=True,text=True,capture_output=True); host=json.loads(subprocess.check_output([str(output)],text=True))
    assert host['validation']=='PASS' and host['monster_retarget_cases']==28
    for key in ('fresh_owner_and_original_ai_preserved','strict_float_threshold','sync_last_target_after_clear','unsupported_retention_search_explicit'): assert host[key] is True
    comparison=oracle(a.original_elf.resolve(),output)
    paths=sources+[MODULE/'character_monster_retarget.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'native_wired':False,'compiler_command':command,
            'source_sha256':{p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
            'scope':'One bounded already-target branch caller with external genuine owning services. Enemy-retention list/property domain and native integration remain unsupported/external.'}
    a.report.parent.mkdir(parents=True,exist_ok=True); a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['monster_retarget_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__': main()
