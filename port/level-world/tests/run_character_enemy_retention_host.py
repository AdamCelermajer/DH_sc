"""Verify pinned ARM character gates and existing-enemy retention continuation."""
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
MANIFEST=MODULE/'reference/character-enemy-retention/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
A,B,P,Q=0x10014000,0x10018000,0x10020000,0x10024000
AI=A+0x3c8; M,N=0x10030000,0x10031000
LIST,HEAP,VT,DEAD,PLAYER=0x10040000,0x10042000,0x10044000,0x10045000,0x10045010
APP,LEVEL,TABLE,NEW_TABLE,VARIABLE,INFO,DESIGN=0x10046000,0x10047000,0x10048000,0x10049000,0x1004a000,0x1004b000,0x1004c000
NODE,STACK=0x1004d000,0x1005e000
def word(x): return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x): return struct.unpack('<f',struct.pack('<I',x))[0]

def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes(); assert hashlib.sha256(raw).hexdigest()==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        e=ELFFile(f);symbols={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in e.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,size):
            p=next(p for p in loads if p['p_vaddr']<=at and at+size<=p['p_vaddr']+p['p_filesz'])
            off=int(p['p_offset'])+at-int(p['p_vaddr']);return raw[off:off+size]
        for row in manifest['functions']:
            at,size=int(row['elf_address'],0),row['size'];s=symbols[row['original_symbol']]
            assert (int(s['st_value']),int(s['st_size']))==(at,size)
            assert hashlib.sha256(data(at,size)).hexdigest()==row['sha256']
        got=0x3cf408+struct.unpack('<I',data(0x3cfbb0,4))[0]
        table_offset=struct.unpack('<I',data(0x3cfbc4,4))[0]
        app_offset=struct.unpack('<I',data(0x3cfbc8,4))[0]
        design_offset=struct.unpack('<I',data(0x3cfbb8,4))[0]
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[]
    gates=[]
    base=[10,0,0,1,1,0,0]
    def gate(name,changes):
        x=base.copy()
        for i,v in changes.items():x[i]=v
        gates.append((name,x))
    gate('alive_enemy_player_nonplayer_owner',{})
    gate('signed_bound_reject',{0:0xffffffff,1:0})
    gate('signed_bound_allows',{0:0,1:0xffffffff})
    gate('equal_bound_allows',{0:4,1:4})
    gate('dead_reject',{2:7})
    gate('not_enemy_reject',{3:0})
    gate('enemy_noncanonical',{3:0xffffffff})
    gate('nonplayer_candidate',{4:0,5:1})
    gate('both_players_reject',{5:1})
    gate('noncanonical_owner_player_reject',{5:0xffffffff})
    gate('owner_replacement_after_dead',{6:12})
    gate('owner_replacement_after_candidate_player',{6:13})
    for name,x in gates:
        limit,needed,dead,enemy,player,owner_player,mutation=x
        old.uc.mem_write(LIST,bytes(0x80));old.pointer(LIST+0x30,A);old.pointer(LIST+0x34,1);old.pointer(LIST+0x38,2)
        old.pointer(A+0x1314,limit);old.pointer(P+0x1310,needed)
        for actor in (A,B,P):old.pointer(actor,VT)
        old.pointer(VT+0x34,DEAD);old.pointer(VT+0x28,PLAYER)
        calls=[]
        def observe(_,at,__,___):
            if at==DEAD:
                calls.append([8,0,old.reg(0),0,0,0])
                if mutation==12:old.pointer(LIST+0x30,B)
                returned(dead)
            elif at==0x3d574c:
                calls.append([9,0,old.reg(0)-0x3c8,old.reg(1),0,0]);returned(enemy)
            elif at==PLAYER:
                actor=old.reg(0);calls.append([10,0,actor,0,0,0])
                if actor==P and mutation==13:old.pointer(LIST+0x30,B)
                returned(player if actor==P else owner_player)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        value=old.invoke(0x4a1ab8,[LIST,P]);old.uc.hook_del(hook)
        expected={'status':0,'value':value,'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),'gate',*map(str,x)],text=True))
        assert actual==expected,(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual})
    # Execute the actual filter2 method: no object virtual+90 call is reachable.
    assert old.invoke(0x4a1950,[LIST,Q])==0
    retention=[]
    base=[3,0xffffffffffffffff,P,1,0,word(7),word(10)]
    def case(name,changes):
        x=base.copy()
        for i,v in changes.items():x[i]=v
        retention.append((name,x))
    case('add_player_after_nonplayer_pop',{})
    case('empty_search_clears_original_ai',{0:0})
    case('known_found_keeps',{1:P})
    case('known_not_found_still_keeps',{0:2,1:P})
    case('unknown_player_absent',{0:2})
    case('player_first_no_pops',{0:1})
    case('null_player_character_no_match',{2:0})
    case('null_player_known_zero',{1:0,2:0})
    case('current_local_player_is_second_candidate',{2:Q})
    case('other_tree_key_not_player',{1:Q})
    case('unsigned_high_tree_key',{1:0xf0000000})
    case('fresh_relation_rejects',{3:0})
    case('fresh_relation_noncanonical',{3:0xffffffff})
    case('radius_excludes_player',{6:word(1)})
    case('radius_empty',{6:0})
    case('radius_strict_boundary_includes_player',{6:word(2)})
    for name,value in [('plus_zero',0),('minus_zero',0x80000000),('negative',word(-7)),('nan',0x7fc00001),('infinity',0x7f800000)]:case('raw_design_'+name,{5:value})
    for i in range(1,10):case('live_mutation_'+str(i),{4:i,**({0:0} if i==9 else {})})
    for name,x in retention:
        mask,key,player,enemy,mutation,design,radius=x
        old.uc.mem_write(STACK-0x2000,bytes(0x2000));old.pointer(AI+4,A)
        old.pointer(got+app_offset,APP);old.pointer(APP+0x38,LEVEL);old.pointer(APP+0x40,M)
        old.pointer(LEVEL+0x70,LEVEL+0x70);old.pointer(got+table_offset,VARIABLE);old.pointer(VARIABLE,TABLE)
        old.pointer(TABLE+8*0x44+0x40,radius);old.pointer(NEW_TABLE+8*0x44+0x40,word(1))
        old.pointer(got+design_offset,VARIABLE+4);old.pointer(VARIABLE+4,DESIGN);old.pointer(DESIGN+0x30,design)
        old.pointer(INFO+0x660,player)
        for owner in (A,B):old.pointer(owner+0x448,0 if key==0xffffffffffffffff else NODE)
        old.uc.mem_write(NODE,bytes(0x24));old.pointer(NODE+0x10,key&0xffffffff)
        calls=[];q=[];captured_radius=[0];pops=[0];known=[0];found=[0];selected_player=[0];added=[0];ended=[]
        local_list=STACK+0x6c
        def call(op,kind,subject,peer=0,w=0,extra=0):calls.append([op,kind,subject,peer,w,extra])
        def install_queue():
            old.pointer(local_list,HEAP);old.pointer(local_list+0x10,HEAP+len(q)*20)
            for i,identity in enumerate(q):old.pointer(HEAP+i*20,identity)
        def observe(_,at,__,___):
            if at==0x4a2730:
                assert old.reg(2)==1 and old.reg(3)==2 and get(STACK)==1
                old.uc.mem_write(local_list,bytes(0x50));old.pointer(local_list+0x2c,old.reg(1))
                if mutation==1:old.pointer(AI+4,B)
                returned(local_list)
            elif at==0x3cf9d0:call(0,0,0)
            elif at==0x3cfa00:
                call(1,0,0)
                if mutation==2:old.pointer(AI+4,B)
            elif at==0x3a2fec:
                call(2,1,old.reg(0))
                if mutation==3:
                    old.pointer(VARIABLE,NEW_TABLE);old.pointer(TABLE+8*0x44+0x40,word(4));old.pointer(AI+4,B)
                returned(8)
            elif at==0x4a3428:
                captured_radius[0]=old.reg(1);assert old.reg(2)==0x40c90fdb
                if mutation==4:old.pointer(APP+0x40,N);old.pointer(AI+4,B)
                # Source SearchEff is a separately tested borrowed boundary here.
                if mask&2 and 1<=number(old.reg(1)):q.append(Q)
                if mask&1 and 2<=number(old.reg(1)):q.append(P)
                install_queue();returned()
            elif at==0x36e478:
                call(3,3,old.reg(0),0,old.reg(1),old.reg(2))
                if mutation==5:old.pointer(AI+4,B)
                if mutation==6:old.pointer(INFO+0x660,Q)
                returned(INFO)
            elif at==0x3cfa54:selected_player[0]=old.reg(7)
            elif at==0x3cfaa8:known[0]=int(old.reg(10)!=old.reg(9))
            elif at==0x38fb18:
                q.pop(0);pops[0]+=1;install_queue();returned()
            elif at==0x3cfae4:found[0]=old.reg(3)
            elif at==0x3d574c:
                call(4,1,old.reg(0)-0x3c8,old.reg(1))
                if mutation==7:old.pointer(AI+4,B)
                returned(enemy)
            elif at==0x3cfb74:
                call(5,0,0)
                if mutation==8:old.pointer(AI+4,B)
            elif at==0x3d7c68:
                added[0]=old.reg(2);call(6,1,old.reg(0)-0x3c8,old.reg(1),old.reg(2));returned()
            elif at==0x3d6d68:
                call(7,2,old.reg(0),old.reg(1))
                if mutation==9:old.pointer(AI+4,B)
                returned()
            elif at==0x3d6890:call(8,2,old.reg(0),old.reg(1),old.reg(2));returned()
            elif at==0x3d49c4:call(9,2,old.reg(0));returned()
            elif at==0x38d18c:returned() # local container destructor boundary
            elif at==0x3cf430:ended.append(True);old.uc.emu_stop()
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        old.put(4,got);old.put(5,AI);old.put(7,Q);old.uc.reg_write(old.sp,STACK);old.uc.reg_write(old.lr,old.stop)
        old.uc.emu_start(0x3cf9a8,old.stop,count=2000);old.uc.hook_del(hook);assert ended
        count=int(bool(mask&2 and 1<=number(captured_radius[0])))+int(bool(mask&1 and 2<=number(captured_radius[0])))
        decision=1 if not count else 2 if known[0] else 3 if not found[0] else 4 if not enemy else 5
        expected={'status':0,'decision':decision,'owner':get(AI+4),'player':selected_player[0],
          'known':known[0],'found':found[0],'pops':pops[0],'count':count,'radius':captured_radius[0],'added':added[0],'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),'retain',*map(str,x)],text=True))
        assert actual==expected,(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
      'executed_scope':'Original flags1 IsCharacterValid body/filter2 IsGameObjectValid and _UpdateAggro 3cf9a8 continuation through original unsigned tree lookup, scan/pop branch/empty cleanup or AddAggro. Constructor, SearchEff, PlayerManager, queue pop, destructor and action callees are explicit modeled providers in the caller comparison; no full-body claim for those dependencies.',
      'search_scope':'Host tests execute real specialized intrusive enumeration, fresh gates, live point reads, binary32 distance and existing source queue pop; ARM comparison of common Search numerical body/queue internals is not claimed.',
      'native_wired':False}

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-enemy-retention/host.exe')
    p.add_argument('--report',type=Path,default=MODULE/'build/character-enemy-retention/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_enemy_retention.cpp',MODULE/'character_target_search.cpp',MODULE/'tests/character_enemy_retention.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-ffp-contract=off',*map(str,sources),'-o',str(exe)]
    run=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if run.returncode:raise RuntimeError(run.stdout+run.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==44
    comparison=oracle(a.original_elf.resolve(),exe)
    paths=sources+[MODULE/'character_enemy_retention.hpp',MODULE/'character_target_search.hpp',ROOT/'port/game-data/aggro.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,
      'compiler_command':command,'native_wired':False,'source_sha256':{x.relative_to(ROOT).as_posix():hashlib.sha256(x.read_bytes()).hexdigest() for x in paths}}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
