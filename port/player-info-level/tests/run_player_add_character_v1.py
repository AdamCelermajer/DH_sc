"""Pinned _AddCharacter/GetBuffer ARM execution versus the selected native DLL.

Spawn/Save/Character/platform callbacks are declared fixtures. This proves the
caller and its shared record stores, not native Character initialization or
actual gameplay save loading.
"""
from __future__ import annotations
import argparse, hashlib, json, os, random, struct, subprocess
from pathlib import Path
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_PC, UC_ARM_REG_LR, UC_ARM_REG_SP
from player_locality_v1_original import image, get, put, signed, ELF_SHA
ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/player-info-level'
PIN=MODULE/'reference/player-add-character-v1/original-functions.json'
MANAGER=0x10001000;PLAYER=0x10003000;FRESH=0x10004000;HOST=0x10005000
CHARACTER=0x10008000;HOST_CHARACTER=0x1000b000;ROOM=0x1000e000;LEVEL=0x10010000
APP=0x99f72c;OBJECTS=0x10011000;HANDLE=0x10012000;ONLINE=0x10013000
STOP=0x30000000;SET_STATE=STOP+0x100
START=0x372220;SIZE=1180
def cases():
    base=[7,2,0,0,0,15,1,1,1,1,10,3,30,7,1,0,0,1,1,0,0,0,-1,0,0]
    rows=[]
    def add(**changes):
        row=base.copy()
        for k,v in changes.items():row[int(k[1:])]=v
        rows.append(row)
    for cl in [-2147483648,-1,0,1,2,2147483647]:
        for char in [0,1]:add(v1=cl,v2=char)
    for local in range(16):
        for active in [0,1]:add(v5=local,v6=active)
    for slot in [-2147483648,-1,0,9,2147483647]:add(v22=slot)
    for flag in [0,1,7,255]:add(v5=0,v14=flag)
    for count in [0,1,3,10,29,30,31,0xffffffff]:
        for present in [0,1]:add(v9=present,v10=count)
    for seed in [0,1,127,128,255]:
        for slots in [0,1,2,3]:
            for levels in [0,1,15,30]:add(v11=slots,v12=levels,v13=seed)
    for first,second in [(0,0),(0,1),(1,0),(1,1),(7,255)]:
        for host in [0,1]:
            for hosting in [0,1]:
                for room in [0,1]:
                    for accepted in [0,1]:add(v3=first,v4=second,v16=host,v17=hosting,v18=room,v19=accepted)
    for mismatch in [0,1]:add(v3=1,v4=1,v20=mismatch)
    for counter in [0,1,0xffffffff]:add(v21=counter)
    for mode in [-1,0,1,2,3]:add(v7=0,v23=mode);add(v10=31,v23=mode)
    for change in [-1,1,4]:add(v10=10,v24=change)
    rng=random.Random(START)
    for _ in range(30):add(v0=rng.choice([-2147483648,-9,0,9,2147483647]),v3=rng.randrange(2),v4=rng.randrange(2),v5=rng.randrange(16),v10=rng.randrange(31),v13=rng.randrange(256),v16=rng.randrange(2),v17=rng.randrange(2),v20=rng.randrange(2))
    return rows
def slot(index,seed):return (seed+index*83)%256
def learned(index,seed):return (seed+index*19)%256
def execute(data,row,words):
    u=Uc(UC_ARCH_ARM,UC_MODE_ARM);u.mem_map(0,len(data));u.mem_write(0,data)
    for base,size in [(0x10000000,0x40000),(0x20000000,0x10000),(STOP,0x1000)]:u.mem_map(base,size)
    stack=0x20008000;u.reg_write(UC_ARM_REG_SP,stack);u.reg_write(UC_ARM_REG_LR,STOP)
    u.mem_write(stack-0x270+0x20,bytes([0xf9]*3));u.mem_write(stack-0x270+0x224,bytes([0xfa]*30))
    put(u,APP+0x38,OBJECTS);put(u,0x99f914,row[23])
    for p in [PLAYER,FRESH,HOST]:put(u,p,0x10016000)
    put(u,0x10016000+0x50,0x80f1ec);put(u,0x10016000+0x5c,0x36d48c)
    put(u,PLAYER+0x380,row[1]);put(u,PLAYER+0x660,CHARACTER if row[2] else 0)
    put(u,PLAYER+0x664,row[22]);put(u,PLAYER+0x674,17);put(u,PLAYER+0x670,-42)
    u.mem_write(PLAYER+0x4e5,bytes([row[14]]))
    put(u,FRESH+0x660,HOST_CHARACTER if row[20] else CHARACTER);put(u,HOST+0x660,HOST_CHARACTER if row[17] else 0)
    for p in [CHARACTER,HOST_CHARACTER]:put(u,p,0x10017000)
    put(u,0x10017000+0x40,SET_STATE);put(u,CHARACTER+0x1f88,-99);put(u,CHARACTER+0x1f8c,-88)
    for i in range(3):put(u,CHARACTER+0x145c+i*4,101+i);put(u,HOST_CHARACTER+0x145c+i*4,901+i)
    put(u,HOST_CHARACTER+0x2f4,ROOM if row[18] else 0)
    put(u,CHARACTER+0x14e8,0x10018000 if row[9] else 0);put(u,0x10018000+0x84,row[10])
    put(u,MANAGER+0x6c4,row[21])
    for offset,length,method,pointer in [(0x3d8,row[11],slot,0x10019000),(0x400,row[12],learned,0x1001a000)]:
        put(u,PLAYER+offset+0x20,pointer if length else 0);put(u,PLAYER+offset+0x24,length)
        if length:u.mem_write(pointer,bytes(method(i,row[13]) for i in range(length)))
    trace=[];local_reads=online_reads=count_reads=0
    external={0x36dfb0,0x30eae4,0x34b724,0x33ff54,0x3b36b0,0x80f1ec,0x3bb740,0x3bb814,0x7fd794,0x80f23c,0x36e09c,0x393db4,0x3938a0,0x3a58f4,0x396a90,0x344184,0x38c710,0x3b35f0,0x36d48c,0x3b4bc4,0x3c1a00,0x3bbe54,0x3bbebc,SET_STATE,0x31f594,0x3f059c,0x36f0dc,0x371050,0x371d80,0x30e004,0x30e868}
    def ret(value=0):u.reg_write(UC_ARM_REG_R0,value&0xffffffff);u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def text(pointer):return bytes(u.mem_read(pointer,96)).split(b'\0',1)[0].decode()
    def note(op,a=0,b=0):trace.append([op,a,b])
    def hook(machine,address,size,context):
        nonlocal local_reads,online_reads,count_reads
        if address==STOP:machine.emu_stop();return
        # These direct source reads are projected as fresh native Save reads.
        if address==0x3723b4 or address==0x372410:
            put(u,0x10018000+0x84,row[10]+count_reads*row[24]);count_reads+=1
        a,b,c,d=[machine.reg_read(reg) for reg in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
        if (address in [0x3723e4,0x37260c] and row[23]==2) or (address==0x3722f0 and not a):machine.emu_stop();return
        if address not in external:
            assert START<=address<START+SIZE or 0x36d730<=address<0x36d7a8,hex(address)
            words.add(address);return
        if address==0x36dfb0:
            assert a==MANAGER and signed(b)==row[0];note(0,signed(b),c);ret(FRESH if c else PLAYER)
        elif address==0x30eae4:
            assert text(b)=='PlayerCharacter_%d';name=f'PlayerCharacter_{signed(c)}';u.mem_write(a,name.encode()+b'\0');ret(len(name))
        elif address==0x34b724:
            assert b==OBJECTS and text(c)=='Character' and text(d)==f'PlayerCharacter_{row[0]}'
            sp=machine.reg_read(UC_ARM_REG_SP);assert get(u,sp)==get(u,sp+4)==1
            note(1,row[0],3);put(u,a,HANDLE);ret(a)
        elif address==0x33ff54:assert get(u,a)==HANDLE;note(2,int(bool(row[7])));ret(CHARACTER if row[7] else 0)
        elif address==0x3b36b0:assert a==CHARACTER;note(3);ret()
        elif address==0x80f1ec:
            assert a==PLAYER;value=(row[5]>>local_reads)&1;local_reads+=1;note(4,value);ret(value)
        elif address==0x3bb740:assert a==CHARACTER;note(5,signed(b));ret()
        elif address==0x3bb814:assert a==CHARACTER;note(6,signed(b));ret()
        elif address==0x7fd794:
            value=row[4] if online_reads else row[3];online_reads+=1;note(7,value);u.mem_write(ONLINE+5,bytes([value]));ret(ONLINE)
        elif address==0x80f23c:assert a==PLAYER;note(8,row[16]);ret(row[16])
        elif address==0x36e09c:assert a==MANAGER;note(9);ret(HOST)
        elif address==0x393db4:assert a==CHARACTER and b==HOST_CHARACTER+0x160 and c==1;note(10,1);ret()
        elif address==0x3938a0:assert a==CHARACTER and b==HOST_CHARACTER+0x16c;note(11);ret()
        elif address==0x3a58f4:assert a==CHARACTER and b==HOST_CHARACTER+0x1450;note(12);ret()
        elif address==0x396a90:assert a==ROOM and b==CHARACTER;note(13,row[19]);ret(row[19])
        elif address==0x344184:assert a==OBJECTS and b==CHARACTER;note(14);ret()
        elif address==0x38c710:assert a==CHARACTER;note(15);ret()
        elif address==0x3b35f0:assert a==CHARACTER;note(16);ret()
        elif address==0x36d48c:assert a==PLAYER;note(17,row[6]);ret(row[6])
        elif address==0x3b4bc4:assert a==CHARACTER;note(18);ret()
        elif address==0x3c1a00:assert a==CHARACTER+0x4fc and b==0;note(19);ret()
        elif address==0x30e868:u.mem_write(a,bytes(u.mem_read(b,c)));ret(a)
        elif address==0x3bbe54:assert a==CHARACTER;note(21,signed(b),signed(c));ret()
        elif address==0x3bbebc:assert a==CHARACTER;note(22,b,signed(c));ret()
        elif address==SET_STATE:assert a==CHARACTER;note(23,b);u.mem_write(CHARACTER+0x2f0,bytes([b]));ret()
        elif address==0x31f594:assert a==APP;note(24,int(bool(row[8])));ret(LEVEL if row[8] else 0)
        elif address==0x3f059c:assert a==LEVEL and b==0;note(25);ret()
        elif address==0x36f0dc:assert a==MANAGER;note(26,signed(b));ret()
        elif address==0x371050:assert a==MANAGER;note(27,signed(b));ret()
        elif address==0x371d80:assert a==MANAGER and b==CHARACTER;note(28);ret()
        elif address==0x30e004:note(30,0 if get(u,machine.reg_read(UC_ARM_REG_SP))==0x464 else 1);ret()
    # Debug mode is a direct source read; inject its projection event at the
    # exact branch to compare ordering with explicit native providers.
    def debug_hook(machine,address,size,context):
        if address in [0x3725fc,0x3723d0]:note(29,row[23])
    u.hook_add(UC_HOOK_CODE,debug_hook);u.hook_add(UC_HOOK_CODE,hook)
    u.reg_write(UC_ARM_REG_R0,MANAGER);u.reg_write(UC_ARM_REG_R1,row[0]&0xffffffff)
    u.emu_start(START,STOP+4,count=30000)
    assert u.reg_read(UC_ARM_REG_PC) in [STOP,0x3723e4,0x37260c,0x3722f0]
    return dict(character=int(bool(get(u,PLAYER+0x660))),member=signed(get(u,CHARACTER+0x1f88)),id=signed(get(u,CHARACTER+0x1f8c)),position=[get(u,CHARACTER+0x145c+i*4) for i in range(3)],no_room=u.mem_read(CHARACTER+0x2ef,1)[0],state=u.mem_read(CHARACTER+0x2f0,1)[0],count=get(u,MANAGER+0x6c4),trace=trace)
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--original',type=Path,required=True);parser.add_argument('--output',type=Path,required=True);parser.add_argument('--compiler',required=True);parser.add_argument('--library',type=Path,required=True);args=parser.parse_args()
    data,names,_=image(args.original);pins=json.loads(PIN.read_text());words=set()
    assert pins['original_sha256']==ELF_SHA
    for pin in pins['functions']+pins['declared_callee_dependencies']:
        address=int(pin['elf_address'],0);symbol=names[pin['original_symbol']]
        assert (symbol['st_value'],symbol['st_size'])==(address,pin['size'])
        assert hashlib.sha256(data[address:address+pin['size']]).hexdigest()==pin['sha256']
    args.output.mkdir(parents=True,exist_ok=True);rows=cases();inputs=args.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    executable=args.output/'host.exe';library=args.library.resolve();command=[args.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-O2',str(MODULE/'tests/player_add_character_v1_host.cpp'),str(library),'-o',str(executable)]
    build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stderr
    env=os.environ.copy();dlls=sorted(library.parent.parent.rglob('*.dll'))
    env['PATH']=os.pathsep.join([*(str(p.parent) for p in dlls),str(Path(args.compiler).parent),env['PATH']])
    native=subprocess.run([str(executable),str(inputs)],env=env,capture_output=True,text=True);assert native.returncode==0,native.stderr
    actual=json.loads(native.stdout);expected=[execute(data,row,words) for row in rows]
    mismatches=[dict(index=i,input=row,expected=e,actual=n) for i,(row,e,n) in enumerate(zip(rows,expected,actual['results'])) if e!=n]
    (args.output/'comparison.json').write_text(json.dumps(dict(expected=expected,actual=actual),indent=2)+'\n')
    commands=json.loads((library.parent.parent/'compile_commands.json').read_text())
    selected=[entry for entry in commands if Path(entry['file']).name=='player_add_character_v1.cpp'];assert len(selected)==1
    sources=['player_add_character_v1.cpp','player_add_character_v1.hpp','tests/player_add_character_v1_host.cpp','tests/run_player_add_character_v1.py','reference/player-add-character-v1/original-functions.json']
    assert len(actual['results'])==len(rows)
    report=dict(status='PASS' if not mismatches else 'FAIL',arm_comparisons=len(rows),executed_pinned_words=len(words),native_failure_checks=actual['failure_checks'],mismatches=mismatches[:10],original_sha256=ELF_SHA,source_sha256={source:hashlib.sha256((MODULE/source).read_bytes()).hexdigest() for source in sources},binary_sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in [executable,*dlls]},selected_commands=selected,selected_library_sha256=hashlib.sha256(library.read_bytes()).hexdigest(),selected_library=str(library),scope=pins['scope'],compilation='host selected library',android_compilation=False,live_gameplay=False,native_policies=pins['native_policies'])
    (args.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','selected_commands','mismatches']},indent=2))
    if mismatches:print(json.dumps(mismatches[:1],indent=2))
    return 1 if mismatches else 0
if __name__=='__main__':raise SystemExit(main())
