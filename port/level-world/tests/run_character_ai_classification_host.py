"""Execute original Character classification/getters and compare native source."""
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

MODULE=Path(__file__).resolve().parents[1]
ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-classification/original-functions.json'
ORIGINAL_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
OWNER,TABLE,NEW_TABLE=0x10014000,0x10020000,0x10024000
TABLE_VAR,COUNT_VAR,FACTION_COUNT_VAR=0x10028000,0x10028004,0x10028008
NAME,ALT_NAME=0x1002a000,0x1002b000
QUERIES=['ai_id','faction_id','ai_row','type','monster','follower','faerie','summoned',
         'merchant','invisible_man','cleaner','npc','player','miniboss','boss','sitting','dead']
SYMBOLS=['_ZNK9Character11GetCharAIIdEv','_ZNK9Character18GetCharAIFactionIdEv',
         '_ZNK9Character9GetCharAIEv','_ZNK9Character11GetCharTypeEv',
         '_ZNK9Character9IsMonsterEv','_ZNK9Character10IsFollowerEv',
         '_ZNK9Character8IsFaerieEv','_ZNK9Character10IsSummonedEv',
         '_ZNK9Character10IsMerchantEv','_ZNK9Character14IsInvisibleManEv',
         '_ZNK9Character9IsCleanerEv','_ZNK9Character5IsNPCEv','_ZNK9Character8IsPlayerEv',
         '_ZNK9Character10IsMiniBossEv','_ZNK9Character6IsBossEv',
         '_ZNK9Character13IsCharSittingEv','_ZNK9Character6IsDeadEv']

def cases():
    result=[]
    def add(label,query,**changes):
        value=[query,0,0,16,20,4,0,'Ghost',0,0,0]
        fields={'id':1,'faction':2,'count':3,'faction_count':4,'type':5,'flags':6,
                'name':7,'dead':8,'mutation':9}
        for key,item in changes.items():value[fields[key]]=item
        result.append((label,value))
    for query in range(17):
        for kind in [-1,0,1,2,3,4,5,6,7,8,9,10,0x7fffffff,-0x80000000]:
            add(QUERIES[query]+'_type_'+str(kind),query,type=kind,name='PlayerCharacter_01')
    for query in range(4):
        for identifier in [-0x80000000,-1,0,1,8,15,16,0x7fffffff]:
            for count in [-0x80000000,-1,0,1,8,16,0x7fffffff]:
                add(QUERIES[query]+'_id_'+str(identifier)+'_count_'+str(count),query,
                    id=identifier,faction=identifier,count=count,faction_count=count)
    for query in [13,14,15]:
        for flag in [0,1,2,4,8,15,0xffffffff,0x80000000]:add(QUERIES[query]+'_flags_'+str(flag),query,flags=flag)
    for name in ['','PlayerCharacter','PlayerCharacter_01','XPlayerCharacter',
                 'Player','playercharacter','PlayerCharacterPlayerCharacter']:
        add('player_name_'+name,12,type=0,name=name)
    for dead in [0,1,2,127,128,255]:add('raw_dead_'+str(dead),16,dead=dead)
    for mutation in [1,2,3]:
        for query in [2,3,4,6,12]:add(QUERIES[query]+'_mutation_'+str(mutation),query,mutation=mutation)
    add('npc_fresh_types_second_is_merchant',11,mutation=4)
    add('player_captured_name_after_import',12,type=0,name='PlayerCharacter_01',mutation=5)
    return result

def original_compare(original:Path,exe:Path):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
    from cpu import Cpu
    assert hashlib.sha256(original.read_bytes()).hexdigest()==ORIGINAL_SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for item in manifest['functions']:
            symbol=symbols[item['original_symbol']];at,size=int(item['elf_address'],0),item['size']
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,size)
            segment=next(s for s in segments if s['p_vaddr']<=at and at+size<=s['p_vaddr']+s['p_filesz'])
            raw=segment.data()[at-segment['p_vaddr']:at-segment['p_vaddr']+size]
            assert hashlib.sha256(raw).hexdigest()==item['sha256']
    cpu=Cpu(original,False,manifest);cpu.uc.mem_map(0x10000000,0x40000)
    report=[];success_cases=[];bounded_rejects=[]
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def signed(at):return struct.unpack('<i',cpu.uc.mem_read(at,4))[0]
    def index(row):
        if TABLE<=row<TABLE+16*0x44:return (row-TABLE)//0x44
        if NEW_TABLE<=row<NEW_TABLE+16*0x44:return 100+(row-NEW_TABLE)//0x44
        raise AssertionError(('invalid source row',hex(row)))
    for name,values in cases():
        query,identifier,faction,count,fcount,kind,flags,title,dead,mutation,_=values
        # Source can address outside the supplied table if a count exceeds its
        # real capacity. The bounded port must reject instead of reproducing an
        # unsafe read; do not call these ARM cases valid parity comparisons.
        selected=identifier if 0<=identifier<count else 8
        if query not in (0,1,16) and selected>=16:
            actual=json.loads(subprocess.check_output([str(exe),*map(str,values)],text=True))
            assert actual['status']==4
            bounded_rejects.append({'case':name,'compiled':actual});continue
        cpu.pointer(OWNER+0xffc,identifier&0xffffffff);cpu.pointer(OWNER+0xff8,faction&0xffffffff)
        cpu.pointer(OWNER+0x44,NAME);cpu.uc.mem_write(OWNER+0x1449,bytes([dead]))
        cpu.uc.mem_write(NAME,title.encode()+b'\0');cpu.uc.mem_write(ALT_NAME,b'changed\0')
        for i in range(16):
            cpu.pointer(TABLE+i*0x44+0x38,(2 if i==1 else kind)&0xffffffff)
            cpu.pointer(TABLE+i*0x44+0x14,flags)
            cpu.pointer(NEW_TABLE+i*0x44+0x38,9);cpu.pointer(NEW_TABLE+i*0x44+0x14,0xffffffff)
        cpu.pointer(TABLE_VAR,TABLE);cpu.pointer(COUNT_VAR,count&0xffffffff);cpu.pointer(FACTION_COUNT_VAR,fcount&0xffffffff)
        cpu.pointer(0x994a98+0x758,TABLE_VAR);cpu.pointer(0x994a98+0x112c,COUNT_VAR)
        cpu.pointer(0x994a98+0x2244,FACTION_COUNT_VAR)
        trace=[];last_row=[-1];captures=[0]
        def observe(_,address,__,___):
            if address==0x3a303c:
                trace.append(2);captures[0]+=1
                if mutation==1:cpu.pointer(OWNER+0xffc,1);cpu.pointer(TABLE_VAR,NEW_TABLE)
                if mutation==4:cpu.pointer(TABLE+0x38,5 if captures[0]==1 else 7 if captures[0]==2 else 8)
            elif address==0x3a3008:
                trace.append(0)
                if mutation==2:cpu.pointer(OWNER+0xffc,1)
                if mutation==3:cpu.pointer(TABLE+0x38,9)
            elif address==0x3a319c:trace.append(1)
            elif address==0x3a3048:last_row[0]=index(cpu.reg(0))
            elif address==0x30ebd4:
                trace.append(3);captured=cpu.reg(0);needle=cpu.reg(1)
                def string(at):
                    data=bytes(cpu.uc.mem_read(at,256));return data.split(b'\0',1)[0]
                assert string(needle)==b'PlayerCharacter'
                offset=string(captured).find(string(needle));cpu.put(0,captured+offset if offset>=0 else 0)
                if mutation==5:cpu.pointer(OWNER+0x44,ALT_NAME)
                cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
        hook=cpu.uc.hook_add(UC_HOOK_CODE,observe)
        value=cpu.invoke(SYMBOLS[query],[OWNER]);cpu.uc.hook_del(hook)
        expected={'status':0,'word':0 if query==2 else value,'row':last_row[0],
                  'ai_id':signed(OWNER+0xffc),'trace':trace}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,values)],text=True))
        assert actual==expected,(name,actual,expected)
        report.append({'case':name,'source':expected,'compiled':actual})
        success_cases.append((name,values,actual))
    coverage=[]
    for item in manifest['functions']:
        at=int(item['elf_address'],0);executable=item['executable_bytes']
        instructions=set(range(at,at+executable,4));seen=cpu.seen&instructions
        assert seen==instructions,(item['original_symbol'],sorted(instructions-seen))
        coverage.append({'symbol':item['original_symbol'],'caller_instructions':len(instructions),
                         'executed_instructions':len(seen)})
    failures=[]
    # Taken dependency failures preserve their call prefix and any earlier ID
    # mutation. They never turn unavailable table/import work into false/true.
    for name,values,success in success_cases:
        if name not in ['npc_type_4','player_name_PlayerCharacter_01','type_mutation_1']:continue
        for failure in range(1,len(success['trace'])+1):
            failed=values.copy();failed[-1]=failure
            actual=json.loads(subprocess.check_output([str(exe),*map(str,failed)],text=True))
            assert actual['status']==3 and actual['trace']==success['trace'][:failure]
            failures.append({'case':name,'failed_call':failure,'compiled':actual})
    return {'validation':'PASS','original_arm_cases':len(report),'mismatches':0,'results':report,
            'coverage':coverage,'executed_caller_instructions':sum(r['caller_instructions'] for r in coverage),
            'bounded_unsafe_table_rejections':bounded_rejects,'dependency_failures':failures,
            'dependency_boundary':'libc strstr modeled as exact byte substring and returned address. No libc implementation claim.'}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-elf',type=Path,required=True);parser.add_argument('--compiler')
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-ai-classification/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/character-ai-classification/validation.json')
    args=parser.parse_args();compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:parser.error('pass --compiler')
    args.output.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_ai_classification.cpp',MODULE/'tests/character_ai_classification.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(args.output)]
    subprocess.run(command,cwd=ROOT,check=True)
    guards=json.loads(subprocess.check_output([str(args.output),'--guards'],text=True))
    assert guards['guard_checks']==9
    comparison=original_compare(args.original_elf.resolve(),args.output.resolve())
    paths=sources+[MODULE/'character_ai_classification.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','original_sha256':ORIGINAL_SHA,'original_arm_comparison':comparison,
            'guard_checks':guards,'compiler_command':command,'native_runtime_tested':False,
            'scope':'Standalone caller differential/guard proof. Native wiring and live producers are verified separately by build-specific checkpoint evidence.',
            'source_sha256':{p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','original_arm_cases':comparison['original_arm_cases'],
                     'caller_instructions':comparison['executed_caller_instructions'],'mismatches':0,'guard_checks':9}))

if __name__=='__main__':main()
