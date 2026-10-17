"""Whole Objective payload and returned-value StreamReader protocols versus ARM.

Unmodified base/SavedQty/readAs<bool>/readAs<int> bodies execute through actual
source vtable slots and direct assertion-global reads. Only actual stream read
and variadic logger are observing external leaves. Provider failures interrupt
at declared delivery boundaries. Native error/status/reentry/alias controls are
not an original exception ABI. Explicit caller scratch pins original otherwise
uninitialized stack residue; incomplete reads must not fabricate default words.
"""
from __future__ import annotations
import argparse,hashlib,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
from quest_table_bindings_v1_original import Original
PIN=MODULE/'reference/quest-objective-payload-v1/original-functions.json'
OBJECT=0x10010000;STREAM=0x10020000;VTABLE=0x10021000;STOP=0x30000000;READ=STOP+0x100

def cases():
    rows=[]
    def add(**changes):
        row=[0,0,255,-1,5,-9,0,-9,0,0,0xa5,-1515870811,-1,1,0,-1,55,99,7,128,42]
        for k,v in changes.items():row[int(k[1:])]=v
        rows.append(row)
    for kind in range(13):
        for byte in [0,1,2,127,128,255]:
            for word in [-2147483648,-1,0,1,2147483647]:add(v1=kind,v2=byte,v3=word)
        for mode in [-2,0,1,2,3]:
            for available in range(6):add(v1=kind,v4=available,v9=mode)
    for byte in range(256):add(v0=3,v2=byte)
    for word in [-2147483648,-1,0,1,127,128,255,256,65535,65536,2147483647]:add(v0=4,v3=word,v4=4)
    for method in [1,2,3,4]:
        for mode in [-2,0,1,2,3]:
            for available in range(6):add(v0=method,v4=available,v9=mode)
        for low,high in [(0,0),(1,0),(2,0),(4,0),(-1,0),(1,1),(4,1),(1,-1),(4,-1),(-1,-1)]:
            for mode in [0,1,2]:add(v0=method,v5=low,v6=high,v7=low,v8=high,v9=mode)
    for kind in [0,4,5,6,7,10]:
        for op in range(4):
            for available in [0,3,5]:
                for throwing in [0,1]:
                    for occurrence in [1,2]:add(v1=kind,v4=available,v9=1,v12=op,v13=occurrence,v14=throwing)
        for op in range(4):
            for available in [0,3,5]:
                add(v1=kind,v4=available,v9=1,v15=op)
                add(v1=kind,v4=available,v9=1,v15=op,v12=op,v13=1)
    return rows

def execute(data,row,vtables,ranges,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    put(u,OBJECT,vtables[row[1]]);u.mem_write(OBJECT+0x14,b'\x7f');put(u,OBJECT+0x20,0xcccccccc);put(u,STREAM,VTABLE);put(u,VTABLE+0x18,READ);put(u,0x99f914,row[9])
    packet=(b'' if row[0]==4 else bytes([row[2]&255]))+struct.pack('<I',row[3]&0xffffffff);packet=packet[:row[4]]
    trace=[];status=0;cursor=seen=kind=reads=assertions=logs=stores=value=0;dispatch=row[1]+3
    scratch=[row[10]&255,row[11]&0xffffffff];pointers=[0,0]
    def refresh():
        for k,pointer in enumerate(pointers):
            if pointer:scratch[k]=bytes(u.mem_read(pointer,1))[0] if k==0 else get(u,pointer)
    def event(op,a=0,b=0,c=0):
        nonlocal status,seen,dispatch
        refresh();trace.append([op,a,b,c])
        if op==row[15]:
            u.mem_write(OBJECT+0x14,bytes([row[16]&255]));put(u,OBJECT+0x20,row[17]);dispatch=row[18];scratch[:]=[row[19]&255,row[20]&0xffffffff]
            for k,pointer in enumerate(pointers):
                if pointer:u.mem_write(pointer,bytes([scratch[k]]) if k==0 else struct.pack('<I',scratch[k]))
        if op==row[12]:
            seen+=1
            if seen==row[13]:status=3;u.emu_stop();return False
        return True
    def ret(low=0,high=0):u.reg_write(UC_ARM_REG_R0,low&0xffffffff);u.reg_write(UC_ARM_REG_R1,high&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def text(pointer):return bytes(u.mem_read(pointer,180)).split(b'\0')[0].decode('ascii')
    def hook(machine,address,size,context):
        nonlocal cursor,kind,reads,assertions,logs,stores,value,status
        if address==STOP:machine.emu_stop();return
        if any(start<=address<start+length for start,length in ranges):
            words.add(address)
            if address in [0x429c1c,0x3364ec]:
                k=0 if address==0x429c1c else 1;p=machine.reg_read(UC_ARM_REG_SP)-24+(15 if k==0 else 12);pointers[k]=p;machine.mem_write(p,bytes([scratch[k]]) if k==0 else struct.pack('<I',scratch[k]))
            elif address in [0x429c54,0x336524]:assertions+=1;event(2,kind,row[9])
            elif address in [0x429c60,0x336530] and row[9]==2:status=4;machine.emu_stop()
            elif address in [0x47ab4c,0x47ab6c]:stores+=1;value=machine.reg_read(UC_ARM_REG_R0)&(255 if address==0x47ab4c else 0xffffffff)
            return
        if address==READ:
            a,destination,n,high=[machine.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]];assert a==STREAM and high==0 and n in [1,4]
            kind=0 if n==1 else 1;reads+=1;count=min(n,len(packet)-cursor)
            if count:machine.mem_write(destination,packet[cursor:cursor+count]);cursor+=count
            low=row[7 if kind else 5];hi=row[8 if kind else 6];low=count if low==-9 else low;hi=0 if row[7 if kind else 5]==-9 else hi
            if event(kind,n,low&0xffffffff,hi&0xffffffff):ret(low,hi)
        elif address==0x30e004:
            assert text(machine.reg_read(UC_ARM_REG_R1))=='ASSERT(%s) FAILED: %s:%d\n'
            assert text(machine.reg_read(UC_ARM_REG_R2))=='bytesRead == sizeof(T)'
            assert text(machine.reg_read(UC_ARM_REG_R3))=='..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/StreamReader.h'
            line=get(machine,machine.reg_read(UC_ARM_REG_SP));assert line==0x44;logs+=1
            if event(3,kind,0x429c1c if kind==0 else 0x3364ec,line):ret()
        else:raise AssertionError(hex(address))
    u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP)
    method=row[0];function=get(u,vtables[row[1]]+0x28) if method==0 else {1:0x47ab3c,2:0x47ab54,3:0x429c1c,4:0x3364ec}[method]
    u.reg_write(UC_ARM_REG_R0,STREAM if method>=3 else OBJECT);u.reg_write(UC_ARM_REG_R1,STREAM)
    u.emu_start(function,STOP+4,count=10000)
    refresh()
    if not status and method>=3:value=u.reg_read(UC_ARM_REG_R0)
    return dict(status=status,value=value,done=bytes(u.mem_read(OBJECT+0x14,1))[0],quantity=get(u,OBJECT+0x20),dispatch=dispatch,scratch=scratch,cursor=cursor,calls=len(trace),reads=reads,assertions=assertions,logs=logs,stores=stores,effects=len(trace),trace=trace)

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--world-library',type=Path,required=True);p.add_argument('--direct',action='store_true');args=p.parse_args()
    data,symbols,_=image(args.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA;ranges=[]
    for pin in pins['functions']:
        address=int(pin['elf_address'],0);symbol=symbols[pin['original_symbol']];assert (symbol['st_value'],symbol['st_size'])==(address,pin['size']);assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256'];ranges.append((address,pin['size']))
    vtables=[]
    for pin in pins['vtables']:
        symbol=symbols[pin['original_symbol']];address=int(pin['elf_address'],0);assert symbol['st_value']+8==address;target=struct.unpack_from('<I',data,address+0x28)[0];assert target==int(pin['slot28'],0);vtables.append(address)
    assert symbols['gAssertLevel']['st_value']==0x99f914
    table_pins=json.loads((MODULE/'reference/quest-table-bindings-v1/original-functions.json').read_text())
    for name,pin in {**table_pins['cache'],**pins['cache']}.items():assert (args.cache/name).stat().st_size==pin['bytes'] and sha(args.cache/name)==pin['sha256']
    original=Original(args.original,(args.cache/'v2quests_pyarray.bin').read_bytes(),(args.cache/'v2quests_pyarraynames.bin').read_bytes());definitions=[original.record(i) for i in range(original.count)]
    from collections import Counter
    assert {str(k):v for k,v in Counter(c['common'][0] for d in definitions for c in d['lists'][1]).items()}==pins['shipped']['list']
    for field in ['accept','end']:assert {str(k):v for k,v in Counter(d[field]['common'][0] for d in definitions).items()}==pins['shipped'][field]
    rows=cases();words=set();args.output.mkdir(parents=True,exist_ok=True);inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    library=args.library.resolve();world=args.world_library.resolve();dlls=sorted(library.parent.parent.rglob('*.dll'));binary_before={str(path):sha(path) for path in [library,world,*dlls]}
    exe=args.output/'host.exe';command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/quest_objective_payload_v1_host.cpp')]
    if args.direct:command.append(str(MODULE/'quest_objective_payload_v1.cpp'))
    command.extend([str(library),str(world),'-o',str(exe)]);built=subprocess.run(command,capture_output=True,text=True);assert built.returncode==0,built.stderr
    env=os.environ.copy();env['PATH']=os.pathsep.join([*(str(path.parent) for path in dlls),str(Path(args.compiler).parent),env['PATH']]);run=subprocess.run([str(exe),str(args.cache.resolve()),str(inputs)],capture_output=True,text=True,env=env);assert run.returncode==0,run.stderr
    actual=json.loads(run.stdout);expected=[execute(data,row,vtables,ranges,words) for row in rows];assert len(actual['results'])==len(expected)
    mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    assert binary_before=={str(path):sha(path) for path in [library,world,*dlls]},'selected DLL changed during gate'
    expected_words=set(range(0x47ab3c,0x47ab74,4))|set(range(0x429c1c,0x429cbc,4))|set(range(0x3364ec,0x33658c,4));assert words==expected_words,[hex(a) for a in sorted(expected_words-words)]
    command_file=next(path/'compile_commands.json' for path in library.parents if (path/'compile_commands.json').is_file());commands=json.loads(command_file.read_text());selected=[e for e in commands if Path(e['file']).name=='quest_objective_payload_v1.cpp']
    if not args.direct:assert len(selected)==1
    dependencies=[e for e in commands if Path(e['file']).name in ['quest_objective_factory_v1.cpp','quest_objective_list_v1.cpp','quest_table_bindings_v1.cpp','quests.c','constants.c']]
    for name in ['quest_objective_factory_v1.cpp','quest_objective_list_v1.cpp','quest_table_bindings_v1.cpp','quests.c','constants.c']:assert sum(Path(e['file']).name==name for e in dependencies)==1
    sources=['quest_objective_payload_v1.cpp','quest_objective_payload_v1.hpp','tests/quest_objective_payload_v1_host.cpp','tests/run_quest_objective_payload_v1.py','reference/quest-objective-payload-v1/original-functions.json','quest_objective_factory_v1.cpp','quest_objective_factory_v1.hpp','quest_objective_list_v1.cpp','quest_objective_list_v1.hpp','quest_table_bindings_v1.cpp','quest_table_bindings_v1.hpp']
    hashes={s:sha(MODULE/s) for s in sources};hashes.update({s:sha(ROOT/s) for s in ['port/quest-data/quests.c','port/quest-data/quests.h','port/pydata-constants/constants.c','port/pydata-constants/constants.h','port/game-data/tests/quest_table_bindings_v1_original.py','port/player-info-level/tests/player_locality_v1_original.py']})
    report=dict(status='PASS' if not mismatches else 'FAIL',arm_comparisons=len(rows),executed_pinned_words=len(words),native_checks=actual['native_checks'],shipped=pins['shipped'],composed_shipped_objects=194,mismatches=mismatches[:10],original_sha256=ELF_SHA,cache={**table_pins['cache'],**pins['cache']},source_sha256=hashes,selected_kernel=not args.direct,selected_commands=selected,selected_dependencies=dependencies,selected_library=str(library),selected_library_sha256=sha(library),selected_world_library=str(world),selected_world_library_sha256=sha(world),binary_sha256={path.name:sha(path) for path in [exe,*dlls]},scope=pins['scope'],source_policies=pins['source_policies'],android_compilation=False,live_gameplay=False)
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','selected_dependencies','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return bool(mismatches)
if __name__=='__main__':raise SystemExit(main())
