"""Compare both source sight callers with original ARM and modeled soft-float imports."""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

MODULE=Path(__file__).resolve().parents[1]
ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-sight/original-functions.json'
ORIGINAL_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AI,OWNER,OWNER_B,TARGET=0x10010000,0x10014000,0x10018000,0x1001c000
TABLE,NEW_TABLE,TABLE_VAR,COUNT_VAR=0x10020000,0x10024000,0x10028000,0x10028004


def number(word): return struct.unpack('<f',struct.pack('<I',word))[0]
def word(value):
    try: return struct.unpack('<I',struct.pack('<f',value))[0]
    except OverflowError: return 0xff800000 if value<0 else 0x7f800000
def is_nan(value): return value&0x7f800000==0x7f800000 and value&0x7fffff!=0
def equivalent_word(a,b): return a==b or is_nan(a) and is_nan(b)


def scenarios():
    cases=[]
    base=[1,1,1,word(6),word(25),0,0,0,word(3),word(4),0,0,word(2)]
    def add(name,changes=None):
        values=base.copy()
        for key,value in (changes or {}).items(): values[key]=value
        cases.append((name,values))
    add('object_inside')
    add('object_strict_boundary',{3:word(5)})
    add('object_negative_radius',{3:word(-6)})
    add('null_argument_uses_ai40',{1:0})
    add('no_target_no_queries',{1:0,2:0})
    add('coincident_zero_radius',{3:0,8:0,9:0})
    add('point_nan',{8:0x7fc00001})
    add('point_infinite_distance',{8:0x7f800000})
    add('both_infinite_coordinates_nan',{5:0x7f800000,8:0x7f800000})
    add('binary32_square_overflow',{8:word(1e20),3:0x7f800000})
    add('binary32_subnormal_square_underflow',{5:1,8:2,9:0,3:1})
    add('binary32_three_axis_rounding',{5:0x3f800001,6:0x47800001,7:0xb7800001,
                                     8:0x3f7fffff,9:0x47800000,10:0x37800000})
    for mutation in range(1,8): add('callback_mutation_'+str(mutation),{11:mutation})
    add('late_owner_point_change_crosses_boundary',{3:word(5),11:3})
    for name,radius,distance in [
        ('strict_boundary',word(5),word(25)),('inside',word(5),word(24)),
        ('outside',word(5),word(26)),('negative_radius',word(-5),word(24)),
        ('plus_zero',0,0),('minus_zero',0x80000000,0x80000000),
        ('negative_distance',0,word(-1)),('nan_radius',0x7fc00001,0),
        ('nan_distance',word(6),0x7fc00001),('positive_infinity',0x7f800000,word(100)),
        ('negative_infinity',0xff800000,0x7f800000),('overflow_radius',word(1e20),0x7f800000),
        ('subnormal_radius',1,0)]:
        add('scalar_'+name,{0:0,3:radius,4:distance})
    add('scalar_captured_props_row',{0:0,11:5})
    add('scalar_live_radius_read',{0:0,11:6})
    return cases


def oracle(original:Path,executable:Path):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
    from cpu import Cpu
    raw=original.read_bytes(); assert hashlib.sha256(raw).hexdigest()==ORIGINAL_SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as stream:
        elf=ELFFile(stream); symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def source(address,size):
            s=next(s for s in segments if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            at=int(s['p_offset'])+address-int(s['p_vaddr']); return raw[at:at+size]
        for row in manifest['functions']:
            at,size=int(row['elf_address'],0),row['size']; symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,size)
            assert hashlib.sha256(source(at,size)).hexdigest()==row['sha256']
    old=Cpu(original,False,manifest); old.uc.mem_map(0x10000000,0x40000)
    def get(address): return struct.unpack('<I',old.uc.mem_read(address,4))[0]
    results=[]
    for name,values in scenarios():
        mode,explicit,fallback,radius,distance,*_=values; mutation=values[11]
        old.pointer(AI+4,OWNER); old.pointer(AI+0x40,TARGET if fallback else 0)
        for actor,index in ((OWNER,0),(OWNER_B,1),(TARGET,0)):
            old.pointer(actor+0x180,0); old.uc.mem_write(actor+0x80,b'\0'); old.pointer(actor+0xffc,index)
            coords=values[8:11] if actor==TARGET else values[5:8]
            old.uc.mem_write(actor+0x160,struct.pack('<III',*coords))
            old.uc.mem_write(actor+0x184,struct.pack('<III',word(99),0,0))
        for index in range(16):
            old.pointer(TABLE+index*0x44+0x3c,radius if index==0 else values[12])
            old.pointer(NEW_TABLE+index*0x44+0x3c,values[12])
        old.pointer(TABLE_VAR,TABLE); old.pointer(COUNT_VAR,16)
        old.pointer(0x994a98+0x758,TABLE_VAR); old.pointer(0x994a98+0x112c,COUNT_VAR)
        calls,fp=[],[]; positions=[0]; measured_distance=[0]; squared_radius=[0]
        def returned(value): old.put(0,value); old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
        def observe(_,address,__,___):
            if address==0x3935dc:
                actor=old.reg(0); positions[0]+=1; calls.append([0,actor])
                if positions[0]==1 and mutation==1:
                    old.pointer(AI+4,OWNER_B); old.pointer(AI+0x40,0)
                if positions[0]==2:
                    if mutation==2: old.pointer(AI+4,OWNER_B)
                    if mutation in (3,4): old.pointer(OWNER+0x160,word(7))
                    if mutation==7: old.pointer(AI+0x40,OWNER_B)
                # Execute real GetTargetPosition leaf; no coordinate snapshot.
            elif address==0x3d4ef0 and mutation==4:
                # Source first returned point stays fixed in R0/R4 after this
                # backing-pointer selection changes for future getters.
                old.pointer(OWNER+0x180,1); old.uc.mem_write(OWNER+0x80,b'\1')
            elif address==0x3a3024:
                calls.append([1,old.reg(0)])
                # Execute original table capture/GetCharAIId/row selection.
            elif address==0x3a303c and mutation==5:
                assert old.reg(4)==TABLE
                old.pointer(AI+4,OWNER_B); old.pointer(TABLE_VAR,NEW_TABLE)
            elif address==0x3d4eb0 and mutation==6:
                old.pointer(old.reg(0)+0x3c,word(2))
            elif address==0x3d4ea0:
                measured_distance[0]=old.reg(1)
            elif address==0x3d4ec0:
                squared_radius[0]=old.reg(0)
            elif address in (0x30e3ac,0x30ed6c,0x30eba4,0x30e2f8):
                a,b=old.reg(0),old.reg(1); x,y=number(a),number(b)
                operation={0x30e3ac:'sub',0x30ed6c:'mul',0x30eba4:'add',0x30e2f8:'gt'}[address]
                if operation=='sub': output=word(x-y)
                elif operation=='mul': output=word(x*y)
                elif operation=='add': output=word(x+y)
                else: output=int(x>y)
                fp.append([operation,a,b,output]); returned(output)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        value=old.invoke(0x3d4ed8 if mode else 0x3d4ea0,[AI,TARGET if mode and explicit else 0 if mode else distance])
        old.uc.hook_del(hook)
        if calls:
            expected_order=['sub','sub','sub','mul','mul','add','mul','add','mul','gt'] if mode else ['mul','gt']
            assert [c[0] for c in fp]==expected_order,(name,fp)
        expected={'status':0,'value':value,'distance':measured_distance[0],'radius_squared':squared_radius[0],
                  'owner':get(AI+4),'target40':get(AI+0x40),'calls':calls}
        actual=json.loads(subprocess.check_output([str(executable),*map(str,values)],text=True))
        numeric_keys=('distance','radius_squared')
        assert all(equivalent_word(actual[k],expected[k]) for k in numeric_keys),(name,actual,expected)
        assert {k:v for k,v in actual.items() if k not in numeric_keys}=={k:v for k,v in expected.items() if k not in numeric_keys},(name,actual,expected)
        results.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'source_soft_float_trace':fp})
    return {'validation':'PASS','comparisons':len(results),'mismatches':0,'results':results,
            'executed_scope':'Both original sight caller bodies, GetTargetPosition leaf, GetCharAI and GetCharAIId table/row instructions execute.',
            'external_soft_float':'Original external fsub/fmul/fadd/fcmpgt PLT imports modeled as separately rounded IEEE binary32 operations. Finite/non-NaN words compare exactly; NaN outputs compare classification, not payload/sign propagation.',
            'native_wired':False}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler'); parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-ai-sight/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/character-ai-sight/validation.json')
    args=parser.parse_args(); compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler: parser.error('pass --compiler')
    output=args.output.resolve(); output.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_ai_sight.cpp',MODULE/'tests/character_ai_sight.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-ffp-contract=off',*map(str,sources),'-o',str(output)]
    subprocess.run(command,cwd=ROOT,check=True,capture_output=True,text=True)
    host=json.loads(subprocess.check_output([str(output)],text=True))
    assert host['validation']=='PASS' and host['sight_cases']==32
    for flag in ('borrowed_points_read_after_target_callback','fresh_scalar_owner','strict_binary32_predicates','failure_alias_contracts'):
        assert host[flag] is True
    comparison=oracle(args.original_elf.resolve(),output)
    paths=sources+[MODULE/'character_ai_sight.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,
            'original_sha256':ORIGINAL_SHA,'compiler_command':command,'native_wired':False,
            'source_sha256':{p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
            'scope':'Two bounded original sight caller orchestrations with external owning providers; existing snapshot API unchanged; no current actor/native behavior claim.'}
    args.report.parent.mkdir(parents=True,exist_ok=True); args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['sight_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))


if __name__=='__main__': main()
