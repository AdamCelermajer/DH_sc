"""Whole three direct-destination typed readers against the private ARM image.

Only actual stream virtual18 and logger leaves are supplied. Assertion mode is
read by the unmodified body from the pinned source global. Callback failures
stop the oracle at delivery boundaries; native errors are not a source ABI.
"""
from __future__ import annotations
import argparse,hashlib,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,ELF_SHA
PIN=MODULE/'reference/quest-stream-read-v1/original-functions.json'
STREAM=0x10020000;VTABLE=0x10021000;DESTINATION=0x10010000;STOP=0x30000000;READ=STOP+0x100
FUNCTIONS=[0x313b48,0x38b758,0x459090]
def cases():
    rows=[]
    def add(**changes):
        row=[0,-1515870811,0x12345678,4,-9,0,0,-1,0,-1,55]
        for key,value in changes.items():row[int(key[1:])]=value
        rows.append(row)
    for method in range(3):
        for residue in [0,-1,-1515870811,-2147483648]:
            for word in [-2147483648,-1,0,1,0x12345678,2147483647]:
                for available in range(5):
                    for mode in [-2,0,1,2,3]:add(v0=method,v1=residue,v2=word,v3=available,v6=mode)
        for low,high in [(0,0),(1,0),(2,0),(3,0),(4,0),(5,0),(-1,0),(4,1),(4,-1),(0,1),(-1,-1)]:
            for mode in [-2147483648,0,1,2,2147483647]:add(v0=method,v4=low,v5=high,v6=mode)
        for op in range(3):
            for available in [0,2,4]:
                for mode in [0,1,2]:
                    for throwing in [0,1]:
                        add(v0=method,v3=available,v6=mode,v7=op,v8=throwing)
                        add(v0=method,v3=available,v6=mode,v7=op,v8=throwing,v9=op,v10=-1)
            for available in range(5):
                for mode in [-2,0,1,2,3]:add(v0=method,v3=available,v6=mode,v9=op,v10=-1985229329)
    return rows
def execute(data,row,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    put(u,STREAM,VTABLE);put(u,VTABLE+0x18,READ);put(u,DESTINATION,row[1]);put(u,0x99f914,row[6])
    source=FUNCTIONS[row[0]];packet=struct.pack('<I',row[2]&0xffffffff)[:row[3]]
    trace=[];status=0;operation=4;cursor=reads=assertions=logs=returned=0;caller=source+0x18
    def event(op,a=0,b=0,c=0):
        nonlocal status,operation
        trace.append([op,a,b,c])
        if op==row[9]:put(u,DESTINATION,row[10])
        if op==row[7]:status=3;operation=op+1;u.emu_stop();return False
        return True
    def ret(low=0,high=0):u.reg_write(UC_ARM_REG_R0,low&0xffffffff);u.reg_write(UC_ARM_REG_R1,high&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def text(pointer):return bytes(u.mem_read(pointer,160)).split(b'\0',1)[0].decode('ascii')
    def hook(machine,address,size,context):
        nonlocal cursor,reads,assertions,logs,returned,caller,status,operation
        if address==STOP:machine.emu_stop();return
        if source<=address<source+152:
            words.add(address)
            if address==source+0x38:
                caller=source+0x34;assertions+=1;assert machine.reg_read(UC_ARM_REG_R2)==row[6]&0xffffffff;event(1,row[0],row[6])
            elif address==source+0x40 and row[6]==2:
                caller=source+0x40;status=4;operation=2;machine.emu_stop()
            return
        if address==READ:
            stream,destination,low,high=[machine.reg_read(r) for r in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
            assert (stream,destination,low,high)==(STREAM,DESTINATION,4,0);reads+=1;count=min(4,len(packet)-cursor)
            if count:machine.mem_write(destination,packet[cursor:cursor+count]);cursor+=count
            low=count if row[4]==-9 else row[4]&0xffffffff;high=0 if row[4]==-9 else row[5]&0xffffffff;returned=(high<<32)|low
            if event(0,4,low,high):ret(low,high)
        elif address==0x30e004:
            caller=source+0x90;logs+=1
            assert text(machine.reg_read(UC_ARM_REG_R1))=='ASSERT(%s) FAILED: %s:%d\n'
            assert text(machine.reg_read(UC_ARM_REG_R2))=='bytesRead == sizeof(T)'
            assert text(machine.reg_read(UC_ARM_REG_R3))==('..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/StreamReader.h' if row[0]==2 else '..\\..\\project_vs2005\\Game/..\\..\\sources/Utils/IStream.h')
            line=get(machine,machine.reg_read(UC_ARM_REG_SP));assert line==(0x50 if row[0]==2 else 0x45)
            if event(2,row[0],source,line):ret()
        else:raise AssertionError(hex(address))
    u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,STREAM);u.reg_write(UC_ARM_REG_R1,DESTINATION)
    u.emu_start(source,STOP+4,count=1000)
    return dict(status=status,operation=operation,destination=get(u,DESTINATION),cursor=cursor,calls=len(trace),reads=reads,assertions=assertions,logs=logs,source_function=source,source_caller=caller,requested=4,returned=returned,trace=trace)
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--direct',action='store_true');args=p.parse_args()
    data,symbols,_=image(args.original);pins=json.loads(PIN.read_text());assert pins['original_sha256']==ELF_SHA
    for pin in pins['functions']:
        address=int(pin['elf_address'],0);symbol=symbols[pin['original_symbol']];assert (symbol['st_value'],symbol['st_size'])==(address,pin['size']);assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
    for pin in pins['source_strings']:
        value=pin['text'].encode()+b'\0';address=int(pin['elf_address'],0);assert data[address:address+len(value)]==value and hashlib.sha256(value).hexdigest()==pin['sha256']
    assert symbols['gAssertLevel']['st_value']==int(pins['assert_global'],0)
    source_paths=[MODULE/s for s in ['quest_stream_read_v1.cpp','quest_stream_read_v1.hpp','tests/quest_stream_read_v1_host.cpp','tests/run_quest_stream_read_v1.py','reference/quest-stream-read-v1/original-functions.json','quest_runtime_fields_v1.cpp','quest_runtime_fields_v1.hpp','player_saved_quests_v1.hpp']]+[ROOT/'port/player-info-level/tests/player_locality_v1_original.py']
    source_before={str(path.relative_to(ROOT)):sha(path) for path in source_paths}
    library=args.library.resolve();command_file=next(path/'compile_commands.json' for path in library.parents if (path/'compile_commands.json').is_file());command_before=sha(command_file);commands=json.loads(command_file.read_text());selected=[e for e in commands if Path(e['file']).name=='quest_stream_read_v1.cpp'];dependencies=[e for e in commands if Path(e['file']).name=='quest_runtime_fields_v1.cpp']
    if not args.direct:assert len(selected)==1
    assert len(dependencies)==1
    dlls=sorted(library.parent.parent.rglob('*.dll'));binary_before={str(path):sha(path) for path in [library,*dlls]}
    rows=cases();args.output.mkdir(parents=True,exist_ok=True);inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    exe=args.output/'host.exe';command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/quest_stream_read_v1_host.cpp')]
    if args.direct:command.append(str(MODULE/'quest_stream_read_v1.cpp'))
    command.extend([str(library),'-o',str(exe)]);built=subprocess.run(command,capture_output=True,text=True);assert built.returncode==0,built.stderr
    env=os.environ.copy();env['PATH']=os.pathsep.join([*(str(path.parent) for path in dlls),str(Path(args.compiler).parent),env['PATH']]);run=subprocess.run([str(exe),str(inputs)],capture_output=True,text=True,env=env);assert run.returncode==0,run.stderr
    actual=json.loads(run.stdout);words=set();expected=[execute(data,row,words) for row in rows];assert len(actual['results'])==len(expected)
    mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    assert source_before=={str(path.relative_to(ROOT)):sha(path) for path in source_paths},'source changed during gate'
    assert command_before==sha(command_file),'build selections changed during gate'
    assert binary_before=={str(path):sha(path) for path in [library,*dlls]},'selected binaries changed during gate'
    assert words=={word for function in FUNCTIONS for word in range(function,function+152,4)}
    dump=Path(args.compiler).with_name('objdump.exe');imports=subprocess.run([str(dump),'-p',str(exe)],capture_output=True,text=True);assert imports.returncode==0
    lines=[line.strip() for line in imports.stdout.splitlines() if 'quest_stream_read_v1' in line or 'quest_runtime_fields_v1' in line]
    assert any('load_quest_data' in line for line in lines),'selected Quest caller not imported'
    if not args.direct:assert all(any(method in line and 'quest_stream_read_v1' in line for line in lines) for method in ['read_unsigned','read_signed','read_quest_signed'])
    (args.output/'imports.txt').write_text(imports.stdout)
    report=dict(status='PASS' if not mismatches else 'FAIL',arm_comparisons=len(rows),whole_reader_words=len(words),native_checks=actual['native_checks'],mismatches=mismatches[:10],original_sha256=ELF_SHA,source_sha256=source_before,source_unchanged=True,selected_commands=selected,selected_dependencies=dependencies,compile_commands_sha256=command_before,selected_library=str(library),selected_library_sha256=sha(library),binary_sha256={path.name:sha(path) for path in [exe,*dlls]},binaries_unchanged=True,selected_kernel=not args.direct,imports=lines,scope=pins['scope'],source_policies=pins['source_policies'],android_compilation=False,live_gameplay=False)
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','selected_dependencies','imports','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return bool(mismatches)
if __name__=='__main__':raise SystemExit(main())
