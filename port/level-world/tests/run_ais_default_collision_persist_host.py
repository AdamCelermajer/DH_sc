"""Execute original collision caller and compare live producer/order with maintained C++."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/ais-default-collision-persist/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AIS,A,B,P,Q,APP=0x10010000,0x10014000,0x10018000,0x10020000,0x10024000,0x10028000
CHARVT,PEERVT,CHAR,PLAYER,FSMSTATE=0x10030000,0x10031000,0x10033000,0x10033010,0x10035000
def identity(kind):return 0 if kind==0 else P if kind==1 else Q if kind==2 else 1
def fixtures():
    rows=[];base=[4,0,0,1,2,0,0,0,0,2,0,0,1,2,180,25,0]
    def add(name,changes=None):
        x=base.copy()
        for i,v in (changes or {}).items():x[i]=v
        rows.append((name,x))
    add('noncharacter_type2_counts');add('type21_counts',{4:21});add('other_type_no_count',{4:3})
    add('unknown_type_no_count',{4:0xffffffff});add('second_character_counts',{2:7,4:0})
    for state in [0,3,4,19,20,0xffffffff]:add('moving_state_'+str(state),{0:state})
    add('same_target_skip',{9:1});add('no_collision_skip',{3:0});add('raw_collision_truthy',{3:0xffffffff})
    add('same_frame_no_dt',{12:2});add('owner_flag_no_dt',{11:255})
    add('frame_wrap_difference',{12:0xffffffff,13:0});add('frame_equal_zero',{12:0,13:0})
    for dt in [0,1,0xffffffff,0x80000000,0x7fffffff]:add('raw_dt_'+str(dt),{15:dt})
    add('counter_wrap',{14:0xfffffff0,15:32})
    add('character_enemy_sets_and_tail_returns',{1:1,7:1})
    add('character_no_collision_enemy_still_sets',{1:1,3:0,7:1})
    add('character_non_enemy_qualifies_counter',{1:1,2:1})
    add('master_equal_nonnull_skips_first_enemy',{1:1,2:1,10:2,7:1})
    add('master_different_allows_set',{1:1,10:1,7:1})
    add('null_master_null_target_allows_set',{1:1,9:0,10:0,7:1})
    add('pointer_one_equal_blocks_set',{1:1,9:3,10:3,7:1})
    add('first_player_raw_bypasses_set',{1:1,5:0xffffffff,6:0,2:1})
    add('second_player_enemy_cancels_sneaking',{1:1,5:1,6:1,7:1})
    add('second_player_non_enemy_no_cancel',{1:1,5:1,6:1,7:0})
    add('cancel_before_collision_false',{1:1,5:1,6:1,7:1,3:0})
    add('two_enemy_queries',{1:1,5:0,6:1,7:0,8:1})
    add('peer_character_fresh_changes_to_noncharacter',{1:1,2:0,10:2,4:2})
    add('peer_noncharacter_fresh_changes_to_character',{1:0,2:1,4:0})
    for mutation in range(1,14):
        changes={16:mutation}
        if 3<=mutation<=7 or mutation in (11,12):changes.update({1:1,5:1,6:1,7:1,8:1})
        if mutation in (4,11,12):changes.update({5:0,7:1})
        if mutation==6:changes.update({5:0,7:0})
        add('genuine_callee_fixture_mutation_'+str(mutation),changes)
    return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    m=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        e=ELFFile(f);syms={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in e.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[off:off+n]
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
        got=(0x3dbfcc+8+struct.unpack('<I',data(0x3dc124,4))[0])&0xffffffff
        app_offset=struct.unpack('<I',data(0x3dc128,4))[0]
        frame_fragment=m['data_producers'][0];at,n=int(frame_fragment['elf_address'],0),frame_fragment['size']
        assert hashlib.sha256(data(at,n)).hexdigest()==frame_fragment['sha256']
    old=Cpu(original,False,m);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def byte(at,x):old.uc.mem_write(at,bytes([x&255]))
    def returned(x=0):old.put(0,x);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[]
    for name,x in fixtures():
        state_id,char1,char2,flag,type_f4,player1,player2,enemy1,enemy2,target,master,ownerflag,lastframe,frame,counter,dt,mutation=x
        old.uc.mem_write(old.stack,bytes(0x10000));old.pointer(AIS+0x98,A);old.pointer(AIS+0xbc,counter);old.pointer(AIS+0xc0,lastframe)
        old.pointer(A+0x408,identity(target));old.pointer(A+0x418,identity(master));byte(A+0x3e0,ownerflag)
        old.pointer(B+0x408,Q);old.pointer(B+0x418,Q);byte(B+0x3e0,0)
        for actor in (A,B):
            old.pointer(actor,CHARVT);old.pointer(actor+0x4fc+0x20,FSMSTATE);old.pointer(FSMSTATE,state_id)
        old.pointer(P,PEERVT);old.pointer(P+0xf4,type_f4)
        old.pointer(PEERVT+0x24,CHAR);old.pointer(CHARVT+0x28,PLAYER)
        old.pointer(got+app_offset,APP);old.pointer(APP+0x74,frame);old.pointer(APP+0x8c,dt)
        calls=[];views=[];chars=[0];players=[0];enemies=[0];counter_at_dt=[]
        def call(op,subject,peer=0,argument=0):calls.append([op,subject,peer,argument])
        def observe(_,at,__,___):
            if at==0x3c029c:
                call(0,old.reg(0)-0x4fc,0,old.reg(1));assert old.reg(1)==0
                if mutation==1:old.pointer(AIS+0x98,B)
                # Actual52B moving helper +20B state getter execute.
            elif at==CHAR:
                chars[0]+=1;call(1,old.reg(0))
                if chars[0]==1 and mutation==2:old.pointer(AIS+0x98,B)
                if chars[0]==1 and mutation==8:old.pointer(AIS+0xbc,42)
                if chars[0]==2 and mutation==10:old.pointer(AIS+0x98,B);byte(B+0x3e0,1)
                returned(char1 if chars[0]==1 else char2)
            elif at==PLAYER:
                players[0]+=1;call(2,old.reg(0))
                if players[0]==1 and mutation==3:old.pointer(AIS+0x98,B)
                if players[0]==2 and mutation==5:old.pointer(AIS+0x98,B)
                if players[0]==1 and mutation==11:old.pointer(A+0x418,Q)
                returned(player1 if players[0]==1 else player2)
            elif at==0x3d574c:
                enemies[0]+=1;call(3,old.reg(0)-0x3c8,old.reg(1))
                if enemies[0]==1 and mutation==4:old.pointer(AIS+0x98,B)
                if enemies[0]==2 and mutation==6:old.pointer(AIS+0x98,B)
                if enemies[0]==1 and mutation==12:old.pointer(A+0x408,0)
                returned(enemy1 if enemies[0]==1 else enemy2)
            elif at==0x3d6890:
                call(4,old.reg(0)-0x3c8,old.reg(1),old.reg(2));assert old.reg(2)==0;returned()
            elif at==0x3bc6b8:
                call(5,old.reg(0))
                if mutation==7:old.pointer(AIS+0x98,B)
                returned()
            elif at==0x31f66c:
                call(6,old.reg(0));assert old.reg(0)==APP and get(AIS+0xc0)==frame
                counter_at_dt.append([get(AIS+0xc0),old.reg(5)])
                if mutation==9:
                    old.pointer(AIS+0xbc,999);old.pointer(AIS+0xc0,999);old.pointer(APP+0x74,99);old.pointer(AIS+0x98,B)
                if mutation==13:old.pointer(AIS+0x98,B)
                # Actual8B GetDt executes; captured R5 counter wins over mutation.
            elif at==0x3dbfd8:views.append([0,old.reg(3)])
            elif at==0x3dc06c:views.append([0,old.reg(2)])
            elif at==0x3dc0dc:views.append([1,P])
            elif at==0x3dc02c:views.append([2,0])
            elif at==0x3dc03c:views.append([0,old.reg(3)])
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        old.invoke(0x3dbfa0,[AIS,P,flag]);old.uc.hook_del(hook)
        expected={'status':0,'counter':get(AIS+0xbc),'frame':get(AIS+0xc0),'owner':get(AIS+0x98),
            'target':get(A+0x408),'master':get(A+0x418),'app_frame':get(APP+0x74),'calls':calls,'views':views}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True))
        assert actual==expected,(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'counter_captured_at_dt':counter_at_dt})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
        'executed_scope':'Complete original396B OnCollisionPersist,52B SM_IsMoving(false),20B SM_GetState and8B GetDt. VirtualCharacter/Player,AI_IsEnemy/SetTarget andCancelSneaking are explicit fixtureproviders; mutation hooks check caller freshness, not original dependency-side-effect claims.',
        'native_wired':False}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/ais-default-collision-persist/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/ais-default-collision-persist/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'ais_default_collision_persist.cpp',MODULE/'ais_external_update.cpp',MODULE/'tests/ais_default_collision_persist.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if build.returncode:raise RuntimeError(build.stdout+build.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==54
    comparison=oracle(a.original_elf.resolve(),exe)
    paths=sources+[MODULE/'ais_default_collision_persist.hpp',MODULE/'ais_external_update.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,'native_wired':False,
      'source_sha256':{x.relative_to(ROOT).as_posix():hashlib.sha256(x.read_bytes()).hexdigest() for x in paths}}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
