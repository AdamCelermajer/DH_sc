"""Original full condition factories/destructors and real list composition."""
from __future__ import annotations
import argparse,hashlib,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
from quest_table_bindings_v1_original import Original
PIN=MODULE/'reference/quest-condition-factory-v1/original-functions.json'
FACTORIES=[0x478b5c,0x478b14,0x478acc,0x478a80,0x478a34,0x4789e8,0x47899c]
D1=[0x4788e0,0x4788ac,0x478878,0x4787dc,0x478844,0x478810,0x4787a8,0x4786a4]
D0=[0x479320,0x4792e4,0x4792a8,0x479230,0x47917c,0x47926c,0x4791f4,0x4791b8]
VTABLES=[0,0x969280,0x969200,0x969240,0x969220,0x9691e0,0x9691c0,0x9691a0,0x969180,0x969160]
LIST=0x10000100;OBJECT=0x10010000;DEFS=0x10030000;STOP=0x30000000

def cases():
    base=[0,0,0,-1,1,0];rows=[]
    def add(**changes):
        row=base.copy()
        for k,v in changes.items():row[int(k[1:])]=v
        rows.append(row)
    for kind in range(7):
        for mode in range(5):add(v0=mode,v1=kind)
    for mode in [3,4]:add(v0=mode,v1=7)
    for mode in [7,8,9]:add(v0=mode)
    for r in range(64):
        for mode in [5,6]:add(v0=mode,v2=r)
    for kind in range(7):
        for throwing in [0,1]:add(v1=kind,v3=0,v5=throwing);add(v0=4,v1=kind,v3=1,v5=throwing)
    for r in [0,1,2,3,16,28,51,52,63]:
        for op in range(4):
            for occurrence in [1,2,3]:
                for throwing in [0,1]:add(v0=5,v2=r,v3=op,v4=occurrence,v5=throwing)
    return rows

def execute(data,row,definitions,ranges,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x80000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    objects={};arrays={};trace=[];allocated=array_allocated=seen=0;status=0
    for r,definition in enumerate(definitions):
        for i,fields in enumerate(definition['lists'][0]):
            for j,value in enumerate(fields):put(u,DEFS+r*0x100+i*0x10+4+j*4,value)
    if 1<=row[0]<=4 or row[0]>=7:
        objects[OBJECT]=[12 if row[1]<3 or row[1]==7 else 8,0]
        dispatch=1 if row[0]>=7 else 2 if row[1]==7 else row[1]+3
        put(u,OBJECT,VTABLES[dispatch]);put(u,OBJECT+4,DEFS);put(u,OBJECT+8,row[1] if row[1]<3 else 0xcccccccc)
    def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def event(op,a=0,b=0,c=0):
        nonlocal seen,status
        trace.append([op,a,b,c])
        if op==row[3]:
            seen+=1
            if seen==row[4]:status=3;u.emu_stop();return False
        return True
    def hook(machine,address,size,context):
        nonlocal allocated,array_allocated
        if address==STOP:machine.emu_stop();return
        if any(start<=address<start+length for start,length in ranges):words.add(address);return
        a,b=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1]]
        if address in [0x310570,0x31056c]:
            is_object=address==0x310570
            identity=0x10006000+allocated*0x100 if is_object else 0x10004000+array_allocated*0x100
            if is_object:allocated+=1
            else:array_allocated+=1
            if not event(0 if is_object else 2,a,b,identity):return
            if is_object:
                assert a in [8,12] and b==0;objects[identity]=[a,1];u.mem_write(identity,bytes([0xcc])*32);put(u,identity+4,DEFS)
            else:
                assert a%4==0 and b==0;arrays[identity]=a//4;u.mem_write(identity,bytes(a))
            ret(identity)
        elif address==0x310440:
            op=1 if a in objects else 3
            if not event(op,a):return
            if op==1:objects[a][1]=2
            ret()
        else:raise AssertionError(hex(address))
    u.hook_add(UC_HOOK_CODE,hook)
    def invoke(address,args):
        for reg,value in zip([UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2],args):u.reg_write(reg,value&0xffffffff)
        u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.emu_start(address,STOP+4,count=100000)
    mode=row[0]
    if mode==0:invoke(FACTORIES[row[1]],[])
    elif mode in [1,2]:invoke(0x478674 if mode==1 else 0x478648,[OBJECT])
    elif mode in [3,4]:invoke((D1 if mode==3 else D0)[row[1]],[OBJECT])
    elif mode>=7:invoke({7:0x4786d8,8:0x4786a0,9:0x478e90}[mode],[OBJECT])
    else:
        invoke(0x4786f0,[LIST]);invoke(0x478914,[LIST,DEFS+row[2]*0x100,len(definitions[row[2]]['lists'][0])])
        if not status and mode==5:invoke(0x478eac,[LIST])
    def definition(pointer):
        if not pointer:return [-1,-1]
        relative=pointer-DEFS;return [relative//0x100,(relative%0x100)//0x10]
    def dispatch(word):return VTABLES.index(word) if word in VTABLES else 0xffffffff
    return dict(status=status,count=signed(get(u,LIST)),array=get(u,LIST+4),definition=definition(get(u,LIST+8)),
                objects=[[p,bytes,phase,dispatch(get(u,p)),signed(get(u,p+8)),definition(get(u,p+4))] for p,(bytes,phase) in sorted(objects.items())],
                arrays=[[p,[get(u,p+i*4) for i in range(n)]] for p,n in sorted(arrays.items())],trace=trace)

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--direct',action='store_true');args=p.parse_args()
    data,symbols,_=image(args.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA
    caller_pins=json.loads((MODULE/'reference/quest-condition-list-v1/original-functions.json').read_text())['functions']
    ranges=[]
    for pin in pins['functions']+caller_pins:
        address=int(pin['elf_address'],0);symbol=symbols[pin['original_symbol']]
        assert (symbol['st_value'],symbol['st_size'])==(address,pin['size']);assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256'];ranges.append((address,pin['size']))
    for name,address in pins['source_vtables'].items():assert symbols[name]['st_value']+8==int(address,0)
    table_pins=json.loads((MODULE/'reference/quest-table-bindings-v1/original-functions.json').read_text())
    for name,pin in table_pins['cache'].items():assert (args.cache/name).stat().st_size==pin['bytes'] and sha(args.cache/name)==pin['sha256']
    original=Original(args.original,(args.cache/'v2quests_pyarray.bin').read_bytes(),(args.cache/'v2quests_pyarraynames.bin').read_bytes())
    definitions=[original.record(i) for i in range(original.count)]
    from collections import Counter
    assert {str(k):v for k,v in Counter(c[0] for r in definitions for c in r['lists'][0]).items()}==pins['shipped_conditions']
    rows=cases();words=set();args.output.mkdir(parents=True,exist_ok=True);inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    library=args.library.resolve();exe=args.output/'host.exe';command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/quest_condition_factory_v1_host.cpp')]
    if args.direct:command.append(str(MODULE/'quest_condition_factory_v1.cpp'))
    command.extend([str(library),'-o',str(exe)]);build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stderr
    dlls=sorted(library.parent.parent.rglob('*.dll'));env=os.environ.copy();env['PATH']=os.pathsep.join([*(str(path.parent) for path in dlls),str(Path(args.compiler).parent),env['PATH']])
    native=subprocess.run([str(exe),str(args.cache.resolve()),str(inputs)],capture_output=True,text=True,env=env);assert native.returncode==0,native.stderr
    actual=json.loads(native.stdout);expected=[execute(data,row,definitions,ranges,words) for row in rows];assert len(actual['results'])==len(expected)
    mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    command_file=next(path/'compile_commands.json' for path in library.parents if (path/'compile_commands.json').is_file())
    selected=[e for e in json.loads(command_file.read_text()) if Path(e['file']).name=='quest_condition_factory_v1.cpp']
    if not args.direct:assert len(selected)==1
    dependencies=[e for e in json.loads(command_file.read_text()) if Path(e['file']).name in ['quest_condition_list_v1.cpp','quest_table_bindings_v1.cpp','quests.c']]
    for name in ['quest_condition_list_v1.cpp','quest_table_bindings_v1.cpp','quests.c']:assert sum(Path(e['file']).name==name for e in dependencies)==1
    sources=['quest_condition_factory_v1.cpp','quest_condition_factory_v1.hpp','tests/quest_condition_factory_v1_host.cpp','tests/run_quest_condition_factory_v1.py','reference/quest-condition-factory-v1/original-functions.json','quest_condition_list_v1.cpp','quest_condition_list_v1.hpp','quest_table_bindings_v1.cpp','quest_table_bindings_v1.hpp']
    source_hashes={s:sha(MODULE/s) for s in sources}
    source_hashes.update({s:sha(ROOT/s) for s in ['port/quest-data/quests.c','port/quest-data/quests.h','port/game-data/tests/quest_table_bindings_v1_original.py','port/player-info-level/tests/player_locality_v1_original.py']})
    report=dict(status='PASS' if not mismatches else 'FAIL',arm_comparisons=len(rows),executed_pinned_words=len(words),native_checks=actual['native_checks'],shipped_types=pins['shipped_conditions'],composed_shipped_rows=64,mismatches=mismatches[:10],original_sha256=ELF_SHA,cache=table_pins['cache'],source_sha256=source_hashes,selected_kernel=not args.direct,selected_commands=selected,selected_dependencies=dependencies,selected_library=str(library),selected_library_sha256=sha(library),binary_sha256={path.name:sha(path) for path in [exe,*dlls]},scope=pins['scope'],source_policies=pins['source_policies'],android_compilation=False,live_gameplay=False)
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','selected_dependencies','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return bool(mismatches)
if __name__=='__main__':raise SystemExit(main())
