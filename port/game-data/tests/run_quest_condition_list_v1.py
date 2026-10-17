"""Whole original ConditionList callers versus native selected/direct kernels."""
from __future__ import annotations
import argparse,hashlib,json,os,random,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R5,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
from quest_table_bindings_v1_original import Original
PIN=MODULE/'reference/quest-condition-list-v1/original-functions.json'
LIST=0x10000100;STOP=0x30000000;DEFS=0x10030000
STARTS=[0x4786f0,0x4786dc,0x478914,0x478eac,0x478f20,0x478704]
RANGES=[(0x4786f0,20),(0x4786dc,20),(0x478914,136),(0x478eac,116),(0x478f20,116),(0x478704,88)]
FACTORIES=[0x478b5c,0x478b14,0x478acc,0x478a80,0x478a34,0x4789e8,0x47899c]

def cases(rows):
    base=[2,0,len(rows[0]['lists'][0]),0,0,-1,0,-1,1,0,-1,0];out=[]
    def add(**changes):
        row=base.copy()
        for k,v in changes.items():row[int(k[1:])]=v
        out.append(row)
    for mode in [0,1]:
        for count,length in [(0,0),(3,3),(-1,3)]:add(v0=mode,v3=count,v4=length)
    for r,definition in enumerate(rows):
        count=len(definition['lists'][0]);add(v1=r,v2=count);add(v1=r,v2=count,v11=1)
    for count in [-3,-1,0,1,3,5]:
        for length in [0,1,3,5]:
            for mode in [3,4,5]:add(v0=mode,v3=count,v4=length)
    for count in [-3,-1,0]:add(v2=count,v3=3,v4=3)
    for mutation in [1,2,3]:add(v6=mutation,v4=5)
    for mode,mutations in [(3,[4,5,6]),(4,[4,5,6]),(5,[7,8])]:
        for mutation in mutations:add(v0=mode,v3=3,v4=5,v6=mutation)
    for null in [-1,0,1,2]:
        for mode in [3,4,5]:add(v0=mode,v3=3,v4=3,v5=null)
    for zero in [-1,0,1,2]:add(v0=5,v3=3,v4=3,v10=zero)
    for op in range(5):
        mode=2 if op<=1 else 3 if op<=3 else 5
        for occurrence in [1,2,3]:
            for throwing in [0,1]:add(v0=mode,v3=3,v4=3,v7=op,v8=occurrence,v9=throwing)
    rng=random.Random(0x478914)
    for _ in range(60):
        mode=rng.choice([3,4,5]);add(v0=mode,v3=rng.randrange(-2,6),v4=rng.randrange(6),v5=rng.randrange(-1,5),v10=rng.randrange(-1,5))
    return out

def execute(data,row,definitions,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x80000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    trace=[];arrays={};objects={};allocated=created=evals=fail_seen=0;status=0;evaluation=1
    for r,definition in enumerate(definitions):
        for i,fields in enumerate(definition['lists'][0]):
            for j,value in enumerate(fields):put(u,DEFS+r*0x100+i*0x10+4+j*4,value)
    put(u,LIST,row[3]);put(u,LIST+4,0x10001000);put(u,LIST+8,DEFS)
    for which in range(2):
        base=0x10001000+which*0x1000;arrays[base]=row[4]
        for i in range(row[4]):
            child=0x10010000+which*0x1000+i*0x100;objects[child]=0
            put(u,base+i*4,0 if i==row[5] else child);put(u,child,0x10050000);put(u,child+4,0)
    delete=STOP+0x100;evaluate=STOP+0x200
    put(u,0x10050000+4,delete);put(u,0x10050000+8,evaluate)
    def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def event(op,a=0,b=0,c=0):
        nonlocal status,fail_seen
        trace.append([op,a,b,c])
        if op==row[7]:
            fail_seen+=1
            if fail_seen==row[8]:status=3;u.emu_stop();return False
        return True
    def valid_slot(base,index):return base in arrays and index<arrays[base]
    def hook(machine,address,size,context):
        nonlocal allocated,created,evals,status,evaluation
        if address==STOP:machine.emu_stop();return
        a,b,c=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
        if any(start<=address<start+length for start,length in RANGES):
            words.add(address)
            # Explicit comparison-domain guards stop only source accesses beyond
            # retained fixtures. They are not original source error handling.
            from unicorn.arm_const import UC_ARM_REG_R3,UC_ARM_REG_R4,UC_ARM_REG_R6,UC_ARM_REG_R7
            if address==0x478964:
                pointer=machine.reg_read(UC_ARM_REG_R5);relative=pointer-DEFS;r=relative//0x100;i=(relative%0x100)//0x10
                if relative<0 or r>=len(definitions) or i>=len(definitions[r]['lists'][0]):status=4;machine.emu_stop()
            elif address==0x478970:
                if not valid_slot(machine.reg_read(UC_ARM_REG_R7),machine.reg_read(UC_ARM_REG_R6)):status=4;machine.emu_stop()
            elif address==0x478978:
                if not valid_slot(machine.reg_read(UC_ARM_REG_R3),machine.reg_read(UC_ARM_REG_R6)):status=4;machine.emu_stop()
            elif address==0x478980:
                if not machine.reg_read(UC_ARM_REG_R3):status=4;machine.emu_stop()
            elif address in [0x478ed0,0x478f44]:
                if not valid_slot(machine.reg_read(UC_ARM_REG_R5),machine.reg_read(UC_ARM_REG_R4)):status=4;machine.emu_stop()
            elif address==0x478730:
                if not valid_slot(machine.reg_read(UC_ARM_REG_R3),machine.reg_read(UC_ARM_REG_R4)):status=4;machine.emu_stop()
            elif address==0x47873c:
                if not machine.reg_read(UC_ARM_REG_R3):status=4;machine.emu_stop()
            return
        if address==0x31056c:
            identity=0x10004000+allocated*0x100;allocated+=1
            if not event(0,a,b,identity):return
            assert a%4==0 and a<4096;arrays[identity]=a//4;u.mem_write(identity,bytes(a))
            if row[6]==1:put(u,LIST,0)
            ret(identity)
        elif address in FACTORIES:
            kind=FACTORIES.index(address);identity=0x10006000+created*0x100;created+=1
            if not event(1,kind,identity):return
            objects[identity]=1;put(u,identity,0x10050000);put(u,identity+4,0)
            if row[6]==2:put(u,LIST,1)
            if row[6]==3 and created==1:put(u,LIST+4,0x10002000)
            ret(identity)
        elif address==delete:
            if not event(2,a):return
            objects[a]=2
            if row[6]==4:put(u,LIST,1)
            if row[6]==5:put(u,LIST+4,0x10002000)
            ret()
        elif address==0x310440:
            if not event(3,a):return
            if row[6]==6:put(u,LIST+4,0x10002000)
            ret()
        elif address==evaluate:
            value=0 if evals==row[10] else 7;evals+=1
            if not event(4,a,value):return
            if row[6]==7:put(u,LIST,1)
            if row[6]==8:put(u,LIST+4,0x10002000)
            if not value:evaluation=0
            ret(value)
        else:raise AssertionError(hex(address))
    u.hook_add(UC_HOOK_CODE,hook)
    def invoke(mode):
        u.reg_write(UC_ARM_REG_R0,LIST);u.reg_write(UC_ARM_REG_R1,DEFS+row[1]*0x100);u.reg_write(UC_ARM_REG_R2,row[2]&0xffffffff)
        u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(STARTS[mode],STOP+4,count=50000)
    invoke(row[0])
    if row[11] and not status:invoke(3);evaluation=1
    def definition(pointer):
        if not pointer:return [-1,-1]
        relative=pointer-DEFS;return [relative//0x100,(relative%0x100)//0x10]
    return dict(status=status,count=signed(get(u,LIST)),array=get(u,LIST+4),definition=definition(get(u,LIST+8)),evaluation=evaluation,
                arrays=[[p,[get(u,p+i*4) for i in range(n)]] for p,n in sorted(arrays.items())],
                objects=[[p,phase,definition(get(u,p+4))] for p,phase in sorted(objects.items())],trace=trace)

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--direct',action='store_true');args=p.parse_args()
    data,symbols,_=image(args.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA
    for pin in pins['functions']:
        address=int(pin['elf_address'],0);symbol=symbols[pin['original_symbol']]
        assert (symbol['st_value'],symbol['st_size'])==(address,pin['size']);assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
    dispatch=pins['factory_dispatch'];address=int(dispatch['elf_address'],0)
    assert hashlib.sha256(data[address:address+28]).hexdigest()==dispatch['sha256'];assert [hex(a) for a in FACTORIES]==dispatch['entries']
    original=Original(args.original,(args.cache/'v2quests_pyarray.bin').read_bytes(),(args.cache/'v2quests_pyarraynames.bin').read_bytes())
    definitions=[original.record(i) for i in range(original.count)];rows=cases(definitions);words=set();args.output.mkdir(parents=True,exist_ok=True)
    inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    library=args.library.resolve();exe=args.output/'host.exe';command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/quest_condition_list_v1_host.cpp')]
    if args.direct:command.append(str(MODULE/'quest_condition_list_v1.cpp'))
    command.extend([str(library),'-o',str(exe)]);build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stderr
    dlls=sorted(library.parent.parent.rglob('*.dll'));env=os.environ.copy();env['PATH']=os.pathsep.join([*(str(path.parent) for path in dlls),str(Path(args.compiler).parent),env['PATH']])
    native=subprocess.run([str(exe),str(args.cache.resolve()),str(inputs)],capture_output=True,text=True,env=env);assert native.returncode==0,native.stderr
    actual=json.loads(native.stdout);expected=[execute(data,row,definitions,words) for row in rows];assert len(actual['results'])==len(expected)
    mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    commands=next(path/'compile_commands.json' for path in library.parents if (path/'compile_commands.json').is_file())
    selected=[e for e in json.loads(commands.read_text()) if Path(e['file']).name=='quest_condition_list_v1.cpp']
    if not args.direct:assert len(selected)==1
    sources=['quest_condition_list_v1.cpp','quest_condition_list_v1.hpp','tests/quest_condition_list_v1_host.cpp','tests/run_quest_condition_list_v1.py','reference/quest-condition-list-v1/original-functions.json','quest_table_bindings_v1.cpp','quest_table_bindings_v1.hpp']
    report=dict(status='PASS' if not mismatches else 'FAIL',arm_comparisons=len(rows),executed_pinned_words=len(words),native_checks=actual['native_checks'],mismatches=mismatches[:10],original_sha256=ELF_SHA,
                source_sha256={s:sha(MODULE/s) for s in sources},selected_kernel=not args.direct,selected_commands=selected,selected_library=str(library),selected_library_sha256=sha(library),binary_sha256={path.name:sha(path) for path in [exe,*dlls]},scope=pins['scope'],source_policies=pins['source_policies'],android_compilation=False,live_gameplay=False)
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return bool(mismatches)
if __name__=='__main__':raise SystemExit(main())
