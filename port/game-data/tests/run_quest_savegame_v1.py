"""Original ARM QuestSavegame callers versus the actual selected game-data DLL.

Quest/factory/platform dependencies are declared fixtures. No production Quest
body, Android compilation or live gameplay is inferred from these comparisons.
"""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-savegame-v1/original-functions.json'
SAVE=0x10000100;STOP=0x30000000;TABLE=0x10030000
STARTS=[0x46b0a4,0x46b004,0x46c1a8,0x46be6c]
RANGES=[(0x46b0a4,160),(0x46b004,160),(0x46c1a8,412),(0x46be6c,224)]
def cases():
    base=[2,3,3,3,0,0,0,123,0,-1,-1,1,0,0]
    rows=[]
    def add(**changes):
        row=base.copy()
        for k,v in changes.items():row[int(k[1:])]=v
        rows.append(row)
    for mode in [0,1]:
        for character in [0,123,0xffffffff]:add(v0=mode,v7=character)
    for count in [0,1,3,5,64]:
        for character in [0,123,0xffffffff]:add(v1=count,v2=count,v3=count,v7=character)
    for n0,n1,n2 in [(0,0,0),(0,1,3),(1,0,5),(3,5,0),(1,1,1),(3,3,3),(5,5,5)]:
        for c0,c1,c2 in [(0,0,0),(1,1,1),(3,3,3),(5,5,5),(1,3,5),(5,1,0)]:add(v1=c0,v2=c1,v3=c2,v4=n0,v5=n1,v6=n2)
    for mutate in [1,2,3,4,6]:
        for lengths in [(0,0,0),(3,3,3),(3,0,3)]:add(v8=mutate,v4=lengths[0],v5=lengths[1],v6=lengths[2])
    for empty in [-1,0,1,2,100,101,102,200,201,202]:add(v4=3,v5=3,v6=3,v9=empty)
    add(v13=1);add(v4=3,v5=3,v6=3,v13=1)
    for op in range(8):
        for occurrence in ([1,2,3] if op==0 else [1,2,5]):
            for throwing in [0,1]:add(v10=op,v11=occurrence,v12=throwing)
    for lengths in [(0,0,0),(1,3,5),(3,3,3),(33,0,0),(64,0,0)]:
        for empty in [-1,0,1,100,102,200,204]:add(v0=3,v4=lengths[0],v5=lengths[1],v6=lengths[2],v9=empty)
    for op in [8,9]:
        for occurrence in [1,2,5]:
            for throwing in [0,1]:add(v0=3,v4=3,v5=3,v6=3,v10=op,v11=occurrence,v12=throwing)
    rng=random.Random(0x46c1a8)
    for _ in range(35):add(v1=rng.randrange(6),v2=rng.randrange(6),v3=rng.randrange(6),v4=rng.randrange(6),v5=rng.randrange(6),v6=rng.randrange(6),v8=rng.choice([0,1,2,3,4,6]))
    return rows
def execute(data,row,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x80000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    counts=row[1:4].copy();vectors=[];objects={};trace=[];table=TABLE
    difficulty=0;allocated=0;reinit_calls=0;status=0;fail_seen=0
    got=0x994a98
    count_pointer=get(u,got+0x4424);rows_pointer=get(u,got+0x46c8);names_pointer=get(u,got+0x20cc)
    put(u,rows_pointer,TABLE);put(u,names_pointer,0x10050000)
    for i in range(70):put(u,0x10050000+i*4,0x10020000+i*0x20)
    for d in range(3):
        vector=0x10001000+d*0x1000;length=row[4+d]
        put(u,SAVE+4+d*12,vector if length else 0);put(u,SAVE+8+d*12,vector+length*4 if length else 0);put(u,SAVE+12+d*12,vector+length*4 if length else 0)
        vectors.append(vector)
        for i in range(length):
            identity=0x10010000+d*0x1000+i*0x100;objects[identity]=4
            put(u,identity+8,i);put(u,identity+0x14,0x10020000+i*0x20);put(u,identity+0x60,row[7])
            put(u,vector+i*4,0 if d*100+i==row[9] else identity)
        u.mem_write(SAVE+0x28+d,bytes([91+d]));put(u,SAVE+0x2c+d*4,71+d);put(u,SAVE+0x38+d*4,81+d);put(u,SAVE+0x44+d*4,21+d);put(u,SAVE+0x50+d*4,31+d)
    put(u,SAVE+0x5c,row[7])
    def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def event(op,a=0,b=0,c=0):
        nonlocal status,fail_seen
        trace.append([op,a,b,c])
        if op==row[10]:
            fail_seen+=1
            if fail_seen==row[11]:status=3;u.emu_stop();return False
        return True
    external={0x46c164,0x310570,0x4808e4,0x4807d8,0x48081c,0x480060,0x4809a4,0x310440,0x708f00}
    def hook(machine,address,size,context):
        nonlocal difficulty,allocated,reinit_calls,table,status
        if address==STOP:machine.emu_stop();return
        a,b,c=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
        if address not in external:
            assert any(start<=address<start+length for start,length in RANGES),hex(address)
            words.add(address)
            if address==0x46c200:
                count=counts[difficulty]
                if not event(0,difficulty,count):return
                put(u,count_pointer,count);difficulty+=1
            elif address==0x46c2a0:
                if not event(1,table):return
                put(u,rows_pointer,table)
            elif address==0x46c2e4:
                # r8 carries the row; the name itself is a direct table load.
                from unicorn.arm_const import UC_ARM_REG_R8
                index=machine.reg_read(UC_ARM_REG_R8)
                name=0x10020000+index*0x20
                if not event(5,index,name):return
                put(u,0x10050000+index*4,name)
                if row[8]==3:table+=0x100
            return
        if address==0x46c164:
            assert a in [SAVE+4+i*12 for i in range(3)] and get(u,c)==0
            d=(a-SAVE-4)//12;vector=vectors[d]
            u.mem_write(vector,bytes(b*4));put(u,a,vector if b else 0);put(u,a+4,vector+b*4 if b else 0);put(u,a+8,vector+b*4 if b else 0);ret()
        elif address==0x310570:
            identity=0x10006000+allocated*0x100
            assert a==0x6c and b==0
            if not event(2,a,b,identity):return
            objects[identity]=0;u.mem_write(identity,bytes([0xcc])*0x6c);allocated+=1;ret(identity)
        elif address==0x4808e4:
            if not event(3,a,signed(b)):return
            objects[a]=1;put(u,a+8,-1);put(u,a+0x14,0)
            if row[8]==1:put(u,SAVE+0x5c,200+allocated)
            ret(a)
        elif address==0x4807d8:
            if not event(4,a,get(u,a+0x60)):return
            objects[a]=2
            if row[8]==2:put(u,SAVE+0x5c,get(u,SAVE+0x5c)+7)
            if row[8]==6:put(u,a+8,999);put(u,a+0x14,999)
            ret()
        elif address==0x48081c:
            if not event(6,a,b):return
            objects[a]=3;ret()
        elif address==0x480060:
            if not a:status=5;machine.emu_stop();return
            if not event(7,a,signed(get(u,a+8)),get(u,a+0x60)):return
            objects[a]=4
            if row[8]==4 and reinit_calls==0:counts[1]=1;counts[2]=0
            reinit_calls+=1;ret()
        elif address==0x4809a4:
            if not event(8,a):return
            objects[a]=5;ret(a)
        elif address==0x310440:
            if a in objects:
                if not event(9,a):return
                objects[a]=6
            ret()
        elif address==0x708f00:ret()
    u.hook_add(UC_HOOK_CODE,hook)
    def invoke(mode):
        u.reg_write(UC_ARM_REG_R0,SAVE);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
        u.emu_start(STARTS[mode],STOP+4,count=100000)
    invoke(row[0])
    if row[13] and not status:difficulty=0;invoke(2)
    released=row[0]==3 and status==0
    result_vectors=[]
    for d in range(3):
        start=get(u,SAVE+4+d*12);end=get(u,SAVE+8+d*12)
        result_vectors.append([] if released else [get(u,start+i*4) for i in range((end-start)//4)])
    return dict(status=status,character=get(u,SAVE+0x5c),fields=[[u.mem_read(SAVE+0x28+i,1)[0],signed(get(u,SAVE+0x2c+i*4)),signed(get(u,SAVE+0x38+i*4)),signed(get(u,SAVE+0x44+i*4)),signed(get(u,SAVE+0x50+i*4))] for i in range(3)],vectors=result_vectors,objects=[[id,signed(get(u,id+8)),get(u,id+0x14),get(u,id+0x60),phase] for id,phase in sorted(objects.items())],trace=trace)
def blank_save_act_words(data,words):
    # Whole real blank Save C1, including both real embedded Quest C1 bodies.
    # String reserve and empty-map insertion are declared platform fixtures.
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x10000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    u.mem_write(SAVE,bytes([0xcc])*0x198)
    def hook(machine,address,size,context):
        if address==STOP:machine.emu_stop();return
        if address in [0x31167c,0x465a48]:
            machine.reg_write(UC_ARM_REG_R0,0);machine.reg_write(UC_ARM_REG_PC,machine.reg_read(UC_ARM_REG_LR));return
        assert 0x465ae0<=address<0x465ae0+352 or 0x46b0a4<=address<0x46b0a4+160,hex(address)
        words.add(address)
    u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,SAVE);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
    u.emu_start(0x465ae0,STOP+4,count=2000)
    assert u.reg_read(UC_ARM_REG_PC)==STOP
    return [[signed(get(u,SAVE+base+i*4)) for i in range(3)] for base in [0xfc,0x15c]]
def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);args=p.parse_args()
    data,names,_=image(args.original);pins=json.loads(PIN.read_text());words=set();assert pins['original_sha256']==ELF_SHA
    for pin in pins['functions']+pins['declared_callee_dependencies']+pins['blank_save_ctor_evidence']+pins['related_character_setter_evidence']:
        address=int(pin['elf_address'],0);symbol=names[pin['original_symbol']]
        assert (symbol['st_value'],symbol['st_size'])==(address,pin['size'])
        assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
    args.output.mkdir(parents=True,exist_ok=True);rows=cases();inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    library=args.library.resolve();exe=args.output/'host.exe';command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/quest_savegame_v1_host.cpp'),str(library),'-o',str(exe)]
    build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stderr
    env=os.environ.copy();dlls=sorted(library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(path.parent) for path in dlls),str(Path(args.compiler).parent),env['PATH']])
    native=subprocess.run([str(exe),str(inputs)],capture_output=True,text=True,env=env);assert native.returncode==0,native.stderr
    actual=json.loads(native.stdout);expected=[execute(data,row,words) for row in rows]
    original_blank=blank_save_act_words(data,words);assert original_blank==actual['blank_quest_act_words']==[[1,1,1],[1,1,1]]
    setters=[]
    for present in [False,True]:
        for identity in [0,1,123,0x7fffffff,0xffffffff]:
            u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
            u.mem_map(0x10000000,0x10000);u.mem_map(STOP,0x1000)
            character=0x10004000;put(u,character+0x14e8,SAVE if present else 0)
            for offset in [0x10,0x114,0x174]:put(u,SAVE+offset,99)
            def hook(machine,address,size,context):
                if address==STOP:machine.emu_stop();return
                assert 0x3bb754<=address<0x3bb770;words.add(address)
            u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,character);u.reg_write(UC_ARM_REG_R1,identity);u.reg_write(UC_ARM_REG_LR,STOP)
            u.emu_start(0x3bb754,STOP+4,count=20)
            setters.append([get(u,SAVE+offset) for offset in [0x10,0x114,0x174]])
    assert setters==actual['set_player_projections']
    assert len(actual['results'])==len(rows)
    mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    command_file=next(path for path in library.parents if (path/'compile_commands.json').is_file())/'compile_commands.json'
    selected=[entry for entry in json.loads(command_file.read_text()) if Path(entry['file']).name=='quest_savegame_v1.cpp'];assert len(selected)==1
    sources=['quest_savegame_v1.cpp','quest_savegame_v1.hpp','player_savegame_v1.hpp','tests/quest_savegame_v1_host.cpp','tests/run_quest_savegame_v1.py','reference/quest-savegame-v1/original-functions.json']
    report=dict(status='PASS' if not mismatches else 'FAIL',arm_comparisons=len(rows),set_player_store_comparisons=len(setters),blank_save_act_words=original_blank,executed_pinned_words=len(words),native_failure_checks=actual['failure_checks'],mismatches=mismatches[:10],original_sha256=ELF_SHA,source_sha256={s:hashlib.sha256((MODULE/s).read_bytes()).hexdigest() for s in sources},selected_commands=selected,selected_library=str(library),selected_library_sha256=hashlib.sha256(library.read_bytes()).hexdigest(),binary_sha256={path.name:hashlib.sha256(path.read_bytes()).hexdigest() for path in [exe,*dlls]},scope=pins['scope'],native_policies=pins['native_policies'],android_compilation=False,live_gameplay=False)
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return bool(mismatches)
if __name__=='__main__':raise SystemExit(main())
