"""Whole original shipped condition Eval callers with declared owner services."""
from __future__ import annotations
import argparse,hashlib,json,os,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_PC,UC_ARM_REG_LR,UC_ARM_REG_SP
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
from quest_table_bindings_v1_original import Original
PIN=MODULE/'reference/quest-condition-eval-v1/original-functions.json'
OBJECT=0x10010000;MANAGER=0x10020000;DEFS=0x10030000;PLAYER=0x10040000
LEVEL=0x10050000;CHARACTER=0x10060000;QUEST=0x10070000;APP=0x99f72c;STOP=0x30000000

def cases(definitions):
    rows=[];shipped=[(r,i,c) for r,d in enumerate(definitions) for i,c in enumerate(d['lists'][0])]
    def add(r,i,c,**changes):
        row=[r,i,c[0] if c[0]<3 else 0,c[2],c[1],1,1,-1,0,0,0,-1,0,0,0,0,0]
        for k,v in changes.items():row[int(k[1:])]=v
        rows.append(row)
    for r,i,c in shipped:
        for state in sorted(set([-2147483648,-1,0,1,c[2]-1,c[2],c[2]+1,2147483647])):
            add(r,i,c,v3=state)
        for level in sorted(set([-2147483648,-1,0,1,c[1]-1,c[1],c[1]+1,2147483647])):
            add(r,i,c,v4=level)
        add(r,i,c,v5=0);add(r,i,c,v6=0)
    representative=[next(p for p in shipped if p[2][0]==kind) for kind in range(4)]
    replacement=next((r,i,c) for r,i,c in shipped if c[0]==3 and c[1]!=0)
    for r,i,c in representative:
        for comparator in [-2147483648,-1,0,1,2,3,2147483647]:add(r,i,c,v2=comparator)
        ops=[0,1,2,3] if c[0]<3 else [0,4]
        for op in ops:
            for comparator in [0,1,2,3]:
                add(r,i,c,v7=op,v8=replacement[0],v9=replacement[1],v10=comparator,v16=9)
            for throwing in [0,1]:
                add(r,i,c,v11=op,v12=throwing)
                add(r,i,c,v7=op,v8=replacement[0],v9=replacement[1],v10=2,v16=9,v11=op,v12=throwing)
    return rows

def execute(data,row,definitions,ranges,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x80000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    for r,d in enumerate(definitions):
        for i,c in enumerate(d['lists'][0]):
            for j,value in enumerate(c):put(u,DEFS+r*0x100+i*0x10+4+j*4,value)
    put(u,OBJECT+4,DEFS+row[0]*0x100+row[1]*0x10);put(u,OBJECT+8,row[2])
    put(u,QUEST,row[3]);put(u,LEVEL+0x3c,row[4]);put(u,PLAYER+0x660,CHARACTER if row[5] else 0);put(u,APP+0x40,MANAGER)
    trace=[];status=0;result=0
    def event(op,a=0,b=0,c=0):
        nonlocal status
        trace.append([op,a,b,c])
        if op==row[7]:
            put(u,OBJECT+4,DEFS+row[8]*0x100+row[9]*0x10);put(u,OBJECT+8,row[10]);put(u,QUEST,row[16]);put(u,LEVEL+0x3c,row[16])
        if op==row[11]:status=3;u.emu_stop();return False
        return True
    def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def hook(machine,address,size,context):
        if address==STOP:machine.emu_stop();return
        if any(start<=address<start+length for start,length in ranges):
            words.add(address)
            if address in [0x478ffc,0x478d8c]:event(0)
            elif address==0x479008:event(1,machine.reg_read(UC_ARM_REG_R0))
            return
        a,b,c=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2]]
        if address==0x36e478:
            if event(2,a,signed(b),c):ret(PLAYER)
        elif address==0x3bc2f8:
            if event(3,a,signed(b),signed(c)):ret(QUEST if row[6] else 0)
        elif address==0x31f594:
            if event(4,a):ret(LEVEL)
        else:raise AssertionError(hex(address))
    u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_R0,OBJECT);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
    kind=definitions[row[0]]['lists'][0][row[1]][0]
    u.emu_start(0x478d78 if kind==3 else 0x478fe8,STOP+4,count=10000)
    if not status:result=u.reg_read(UC_ARM_REG_R0)
    pointer=get(u,OBJECT+4)-DEFS
    return dict(status=status,value=result,calls=len(trace),definition=[pointer//0x100,(pointer%0x100)//0x10],comparator=signed(get(u,OBJECT+8)),state=signed(get(u,QUEST)),level=signed(get(u,LEVEL+0x3c)),character=get(u,PLAYER+0x660),effects=len(trace),trace=trace)

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--direct',action='store_true');args=p.parse_args()
    data,symbols,_=image(args.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA
    ranges=[]
    for pin in pins['functions']+pins['dependencies']:
        address=int(pin['elf_address'],0);symbol=symbols[pin['original_symbol']]
        assert (symbol['st_value'],symbol['st_size'])==(address,pin['size']);assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
        if pin in pins['functions']:ranges.append((address,pin['size']))
    assert symbols['_ZN9SingletonI11ApplicationE6s_instE']['st_value']==APP
    table_pins=json.loads((MODULE/'reference/quest-table-bindings-v1/original-functions.json').read_text())
    for name,pin in table_pins['cache'].items():assert (args.cache/name).stat().st_size==pin['bytes'] and sha(args.cache/name)==pin['sha256']
    original=Original(args.original,(args.cache/'v2quests_pyarray.bin').read_bytes(),(args.cache/'v2quests_pyarraynames.bin').read_bytes())
    definitions=[original.record(i) for i in range(original.count)]
    from collections import Counter
    assert {str(k):v for k,v in Counter(c[0] for r in definitions for c in r['lists'][0]).items()}==pins['shipped_conditions']
    rows=cases(definitions);words=set();args.output.mkdir(parents=True,exist_ok=True);inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    library=args.library.resolve();library_hash=sha(library);dlls=sorted(library.parent.parent.rglob('*.dll'));dll_hashes={str(path):sha(path) for path in dlls}
    exe=args.output/'host.exe';command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/quest_condition_eval_v1_host.cpp')]
    if args.direct:command.append(str(MODULE/'quest_condition_eval_v1.cpp'))
    command.extend([str(library),'-o',str(exe)]);build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stderr
    env=os.environ.copy();env['PATH']=os.pathsep.join([*(str(path.parent) for path in dlls),str(Path(args.compiler).parent),env['PATH']])
    native=subprocess.run([str(exe),str(args.cache.resolve()),str(inputs)],capture_output=True,text=True,env=env);assert native.returncode==0,native.stderr
    actual=json.loads(native.stdout);expected=[execute(data,row,definitions,ranges,words) for row in rows];assert len(actual['results'])==len(expected)
    assert words==set(range(0x478fe8,0x479098,4))|set(range(0x478d78,0x478dac,4)),[hex(word) for word in sorted(words)]
    assert library_hash==sha(library) and dll_hashes=={str(path):sha(path) for path in dlls},'selected binaries changed during gate'
    mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    command_file=next(path/'compile_commands.json' for path in library.parents if (path/'compile_commands.json').is_file());commands=json.loads(command_file.read_text())
    selected=[e for e in commands if Path(e['file']).name=='quest_condition_eval_v1.cpp']
    if not args.direct:assert len(selected)==1
    dependencies=[e for e in commands if Path(e['file']).name in ['quest_condition_factory_v1.cpp','quest_condition_list_v1.cpp','quest_table_bindings_v1.cpp','quests.c']]
    for name in ['quest_condition_factory_v1.cpp','quest_condition_list_v1.cpp','quest_table_bindings_v1.cpp','quests.c']:assert sum(Path(e['file']).name==name for e in dependencies)==1
    sources=['quest_condition_eval_v1.cpp','quest_condition_eval_v1.hpp','tests/quest_condition_eval_v1_host.cpp','tests/run_quest_condition_eval_v1.py','reference/quest-condition-eval-v1/original-functions.json','quest_condition_factory_v1.cpp','quest_condition_factory_v1.hpp','quest_condition_list_v1.cpp','quest_condition_list_v1.hpp','quest_table_bindings_v1.cpp','quest_table_bindings_v1.hpp','quest_runtime_fields_v1.hpp']
    source_hashes={s:sha(MODULE/s) for s in sources};source_hashes.update({s:sha(ROOT/s) for s in ['port/quest-data/quests.c','port/quest-data/quests.h','port/game-data/tests/quest_table_bindings_v1_original.py','port/player-info-level/tests/player_locality_v1_original.py','port/level-world/level_construction_fields.hpp']})
    report=dict(status='PASS' if not mismatches else 'FAIL',arm_comparisons=len(rows),executed_pinned_words=len(words),native_checks=actual['native_checks'],shipped_types=pins['shipped_conditions'],composed_shipped_rows=64,mismatches=mismatches[:10],original_sha256=ELF_SHA,cache=table_pins['cache'],source_sha256=source_hashes,selected_kernel=not args.direct,selected_commands=selected,selected_dependencies=dependencies,selected_library=str(library),selected_library_sha256=sha(library),binary_sha256={path.name:sha(path) for path in [exe,*dlls]},scope=pins['scope'],source_policies=pins['source_policies'],android_compilation=False,live_gameplay=False)
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','selected_dependencies','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return bool(mismatches)
if __name__=='__main__':raise SystemExit(main())
