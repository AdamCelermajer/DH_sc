"""Compare both complete visibility callers with original ARM instructions."""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys

MODULE=Path(__file__).resolve().parents[1]
REPO=MODULE.parents[1]
sys.path.insert(0,str(REPO/'port/engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE

OBJ,VA,VB=0x02110000,0x02220000,0x02230000
NA,NB,MA,MB=0x02310000,0x02320000,0x02410000,0x02420000
SA,SB,VTA,VTB=0x02510000,0x02510010,0x02610000,0x02620000
APP,APPB,DEVICE,DEVICEB=0x02710000,0x02720000,0x02730000,0x02740000
MANIFEST=MODULE/'reference/game-object-set-visible/original-functions.json'
INPUTS=[MODULE/'game_object_set_visible.hpp',MODULE/'game_object_set_visible.cpp',
        MODULE/'tests/game_object_set_visible.cpp',Path(__file__).resolve(),MANIFEST,
        MODULE/'reference/game-object-set-visible/NOTES.md',REPO/'port/engine-resources/tests/cpu.py']

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def word(cpu,p):return struct.unpack('<I',cpu.uc.mem_read(p,4))[0]
def byte(cpu,p):return cpu.uc.mem_read(p,1)[0]
def put_byte(cpu,p,x):cpu.uc.mem_write(p,bytes([x&255]))

def fixtures():
    rows=[]
    # Full byte domain, zero/nonzero raw input and null/non-null visual paths.
    for enabled in range(256):
        for arg in (0,1):
            for visual in (0,1):rows.append([0,arg,enabled,77,visual,1,0,0,0])
    for arg in (0,1,2,255,0x80000000,0xffffffff):
        for flags in (0,1,2,3,0x12345678,0x12345679,0xffffffff):
            for root in (0,1):
                for owner in (0,OBJ,1):rows.append([1,arg,255,77,1,root,flags,0,owner])
    for mutation in (1,2,4,8,16,32,256,1|2,1|4,1|8,2|8,1|2|4|8|16|32|256):
        for arg,flags in ((0,0),(0,1),(1,0),(1,1),(255,1)):
            rows.append([1,arg,255,77,1,1,flags,mutation,OBJ])
    for mutation in (1,2,4,1|2|4):
        for enabled in (0,1,128,255):
            for arg in (0,1,0xffffffff):rows.append([0,arg,enabled,77,1,1,0,mutation,0])
    return rows

def original_cases(path,manifest,rows):
    cpu=Cpu(path,False,manifest)
    singleton=cpu.symbols['_ZN9SingletonI11ApplicationE6s_instE']
    calls=[];coverage=set();current=[None];writes=[0]
    def ret():cpu.put(0,0xaabbccdd);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def hook(uc,address,size,unused):
        if 0x38b0f0<=address<0x38b110 or 0x471368<=address<0x4713c8:coverage.add(address)
        if current[0] is None:return
        kind,arg,enabled,visible,visual,root,flags,mutation,owner=current[0]
        if address==0x38b100:writes[0]+=1
        if address==0x4713d0:
            assert cpu.reg(0)==VA
            calls.append([0,VA,0,0])
            if mutation&1:cpu.pointer(OBJ+0x2d8,VB)
            if mutation&2:put_byte(cpu,OBJ+0x8a,97)
            if mutation&4:put_byte(cpu,OBJ+0x80,201)
            ret()
        elif address==0x350ee0:
            assert cpu.reg(0)==MA
            calls.append([1,MA,0,0])
            if mutation&1:cpu.pointer(VA+8,NB)
            if mutation&2:cpu.pointer(VTA+0x48,SB)
            if mutation&4:cpu.pointer(NA+0x11c,word(cpu,NA+0x11c)^1)
            if mutation&8:cpu.pointer(singleton+0x10,DEVICEB)
            if mutation&256:cpu.pointer(VA+4,1)
            ret()
        elif address in (SA,SB):
            node,value=cpu.reg(0),cpu.reg(1)
            assert node in (NA,NB)
            calls.append([2,node,address,value])
            cpu.pointer(node+0x11c,(word(cpu,node+0x11c)&~1)|(1 if value else 0))
            if mutation&16:cpu.pointer(VA+8,NB)
            if mutation&32:put_byte(cpu,OBJ+0x80,211);put_byte(cpu,OBJ+0x8a,71)
            ret()
    cpu.uc.hook_add(UC_HOOK_CODE,hook)
    results=[]
    for row in rows:
        kind,arg,enabled,visible,visual,root,flags,mutation,owner=row
        current[0]=None;calls.clear();writes[0]=0
        put_byte(cpu,OBJ+0x80,visible);put_byte(cpu,OBJ+0x8a,enabled)
        cpu.pointer(OBJ+0x2d8,VA if visual else 0);cpu.pointer(VA+8,NA if root else 0);cpu.pointer(VA+4,owner)
        cpu.pointer(VB+8,NB);cpu.pointer(VB+4,OBJ)
        cpu.pointer(NA,VTA);cpu.pointer(NB,VTB);cpu.pointer(NA+0x11c,flags);cpu.pointer(NB+0x11c,0x12340000)
        cpu.pointer(VTA+0x48,SA);cpu.pointer(VTB+0x48,SB)
        cpu.pointer(singleton+0x10,DEVICE)
        cpu.pointer(DEVICE+0x1c,MA);cpu.pointer(DEVICEB+0x1c,MB)
        current[0]=row
        cpu.invoke('_ZN10GameObject10SetVisibleEb' if kind==0 else '_ZN12VisualObject10SetVisibleEb',
                 [OBJ if kind==0 else VA,arg],budget=1000)
        device=word(cpu,singleton+0x10)
        results.append({'status':0,'writes':writes[0],'calls':len(calls),
                        'state':[byte(cpu,OBJ+0x80),byte(cpu,OBJ+0x8a),word(cpu,OBJ+0x2d8),word(cpu,VA+8),
                                 word(cpu,NA+0x11c),word(cpu,NB+0x11c),word(cpu,device+0x1c),word(cpu,VA+4)],
                        'trace':[x[:] for x in calls]})
    expected=set(range(0x38b0f0,0x38b110,4))|set(range(0x471368,0x4713c8,4))
    if coverage!=expected:raise AssertionError(('incomplete original instruction coverage',sorted(expected-coverage)))
    return results,sorted(coverage)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf',required=True,type=Path)
    parser.add_argument('--compiler',required=True,type=Path)
    parser.add_argument('--build-dir',type=Path,default=MODULE/'build/game-object-set-visible')
    parser.add_argument('--report',type=Path)
    args=parser.parse_args();build=args.build_dir.resolve();build.mkdir(parents=True,exist_ok=True)
    original=args.original_elf.resolve(strict=True);compiler=args.compiler.resolve(strict=True)
    manifest=json.loads(MANIFEST.read_bytes());before={str(p.relative_to(REPO)).replace('\\','/'):digest(p) for p in INPUTS}
    if digest(original)!=manifest['original_sha256']:raise ValueError('original ELF identity mismatch')
    from elftools.elf.elffile import ELFFile
    with original.open('rb') as f:
        elf=ELFFile(f)
        for row in manifest['functions']+manifest['supporting_functions']:
            address=int(row['elf_address'],16);size=row['size']
            segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and
                         s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(segment['p_offset']+address-segment['p_vaddr'])
            if hashlib.sha256(f.read(size)).hexdigest()!=row['sha256']:raise ValueError('original range identity mismatch')
    exe=build/('game_object_set_visible.exe' if sys.platform=='win32' else 'game_object_set_visible')
    command=[str(compiler),'-std=c++17','-O2','-Wall','-Wextra','-Werror','-pedantic',
             str(MODULE/'game_object_set_visible.cpp'),str(MODULE/'tests/game_object_set_visible.cpp'),'-o',str(exe)]
    subprocess.run(command,check=True)
    guards=json.loads(subprocess.check_output([str(exe),'--guards'],text=True))
    if guards['status']!='PASS' or guards['mismatches'] or guards['guard_checks']<40:raise ValueError('guards incomplete')
    rows=fixtures();fixture=build/'cases.tsv'
    fixture.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n',encoding='utf-8',newline='\n')
    compiled=[json.loads(line) for line in subprocess.check_output([str(exe),'--cases',str(fixture)],text=True).splitlines()]
    expected,coverage=original_cases(original,manifest,rows)
    if len(compiled)!=len(rows):raise ValueError('compiled fixture count mismatch')
    mismatches=[{'case':i,'input':rows[i],'original':a,'compiled':b} for i,(a,b) in enumerate(zip(expected,compiled)) if a!=b]
    after={str(p.relative_to(REPO)).replace('\\','/'):digest(p) for p in INPUTS}
    if before!=after:raise ValueError('source changed during compile/test')
    report={'schema':'dh2-game-object-set-visible-original-v1','status':'PASS' if not mismatches else 'FAIL',
            'complete_original_bodies':2,'original_elf_sha256':digest(original),'source_sha256':before,
            'compiler':str(compiler),'compile_command':command,'executable':{'path':str(exe),'sha256':digest(exe)},
            'fixture':{'path':str(fixture),'sha256':digest(fixture)},'original_arm_cases':len(rows),
            'original_instruction_count':32,'covered_original_instructions':[hex(a) for a in coverage],
            'guard_suite':guards,'mismatch_count':len(mismatches),'mismatches':mismatches,
            'providers':'SyncVisibility, ForceRegister and SceneNode virtual+48 intercepted as named fixture services; their bodies are not credited',
            'native_wired':False,'cases':[{'input':r,'original':a,'compiled':b} for r,a,b in zip(rows,expected,compiled)]}
    output=(args.report or build/'validation.json').resolve();output.parent.mkdir(parents=True,exist_ok=True)
    output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps({k:report[k] for k in ['status','complete_original_bodies','original_arm_cases','original_instruction_count','guard_suite','mismatch_count']}))
    if mismatches:raise SystemExit(1)

if __name__=='__main__':main()
