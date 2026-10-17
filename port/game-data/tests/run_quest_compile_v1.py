"""Whole Quest Compile316B, log CompileQuests140B and reached list wrappers.

The original noarg LoopOnAll104B, Objective invalidation16B, list invalidation /
Compile / Register / markers, Reward invalidation84B / Compile68B and Gold20B
Compile execute. Full gameplay Objective member bodies are explicit observing
fixture providers: their callback order, mutation, failure and partial-store
prefixes are compared, not their missing world/event/marker/script behavior.
Native reward Compile reuses the selected RewardExecution body. Existing one
Instance/list/factory/log owners supply all state. Difficulty is a mandatory
observing fixture for SG_GetGameDifficulty; it can change between each source
query. Captured vector length and fresh vector selection/backing are verified.
Native slots beyond a vector's live element range are a separately scoped
container-lifetime guard, even when source allocation capacity still remains.
No Android compilation, native hookup or live gameplay claim.
"""
from __future__ import annotations
import argparse,hashlib,itertools,json,os,struct,subprocess,sys
from pathlib import Path
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_ARM,UC_HOOK_CODE
from unicorn.arm_const import *
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/game-data'
sys.path.insert(0,str(ROOT/'port/player-info-level/tests'))
from player_locality_v1_original import image,get,put,signed,ELF_SHA
PIN=MODULE/'reference/quest-compile-v1/original-functions.json'
FUNCTIONS=[('_ZN5Quest7CompileEv',0x480178,316),('_ZN13QuestSavegame13CompileQuestsEb',0x46b824,140),
 ('_ZN13ObjectiveList17InvalidateCompileEv',0x47a638,52),('_ZN13ObjectiveList7CompileEv',0x47a66c,36),
 ('_ZN13ObjectiveList8RegisterEv',0x47a614,36),('_ZN13ObjectiveList23InstallObjectiveMarkersEii',0x47a4ec,84),
 ('_ZN13ObjectiveList9LoopOnAllEM9ObjectiveFvvE',0x47a540,104),('_ZN9Objective17InvalidateCompileEv',0x479f70,16),
 ('_ZN10RewardList17InvalidateCompileEv',0x4838fc,84),('_ZN10RewardList7CompileEv',0x482978,68),('_ZN11Reward_Gold7CompileEv',0x482880,20)]
Q=0x10001000;OA=0x10003000;RA=0x10003500;OBJECTS=0x10010000;REWARDS=0x10015000;VT=0x10018000;LOG=0x1001a000;VECTORS=0x1001b000;ROW=0x1001c000;STOP=0x30000000
def word(v):return struct.pack('<I',v&0xffffffff)
def pack(v):return b''.join(word(x) for x in v)
def evidence(data,names):
    pins=[]
    for name,address,size in FUNCTIONS:
        assert (names[name]['st_value'],names[name]['st_size'])==(address,size)
        pins.append(dict(original_symbol=name,elf_address=hex(address),size=size,sha256=hashlib.sha256(data[address:address+size]).hexdigest()))
    return dict(original_sha256=ELF_SHA,functions=pins,remaining_external=['Objective full member bodies','std::string empty assignment implementation','Character::SG_GetGameDifficulty52B'],scope=__doc__)
def cases():
    rows=[]
    def add(**changes):
        r=[0,0,1,3,3,0,0,0,0,1,0,0,0,0,6,0,1]
        for key,value in changes.items():r[int(key[1:])]=value
        rows.append(r)
    for mode,state,enabled,oc,rc in itertools.product(range(9),[-1,0,3,6,9,13],[0,1,255],[-2,0,1,3],[-1,0,2,3]):add(v0=mode,v1=state,v2=enabled,v3=oc,v4=rc)
    for mode,mutation,state in itertools.product([0,2,3,4,7],range(1,9),[3,6,9]):add(v0=mode,v1=state,v5=mutation,v14=state+1)
    for mode,fail,partial,throws in itertools.product([0,2,3,4,7],range(1,14),range(2),range(2)):add(v0=mode,v1=6,v6=fail,v7=partial,v8=throws)
    for mode,null,backing,count in itertools.product(range(9),range(5),range(4),[0,1,3,4]):add(v0=mode,v1=6,v12=null,v13=backing,v3=count,v4=count)
    for force,flag,difficulty,count,mutation in itertools.product(range(2),[0,1,255],[-1,0,1,2,3,4],range(3),[0,8]):add(v0=7,v15=force,v10=flag,v11=difficulty,v9=count,v5=mutation)
    for encoded in [0,1,128,129,-128,-127]:add(v0=8,v16=encoded)
    return rows
class Original:
    def __init__(self,path):
        data,names,_=image(path);self.pins=evidence(data,names);assert self.pins==json.loads(PIN.read_text())
        self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM);self.u.mem_map(0,len(data));self.u.mem_write(0,data)
        for base,size in [(0x10000000,0x30000),(0x20000000,0x10000),(STOP,0x1000)]:self.u.mem_map(base,size)
        self.words=set();self.coverage={a:set() for _,a,_ in FUNCTIONS};self.u.hook_add(UC_HOOK_CODE,self.code)
    def ret(self,value=0):self.u.reg_write(UC_ARM_REG_R0,value&0xffffffff);self.u.reg_write(UC_ARM_REG_PC,self.u.reg_read(UC_ARM_REG_LR))
    def stop(self):self.failed=True;self.u.emu_stop()
    def oid(self,p):return (p-OBJECTS)//0x100+1 if p else 0
    def rid(self,p):return (p-REWARDS)//0x100+1 if p else 0
    def code(self,u,a,size,context):
        if a==STOP:u.emu_stop();return
        r0,r1,r2,r3=[u.reg_read(x) for x in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]]
        if a==0x3bb8e4:
            d=self.row[11] if self.row[11]!=3 else self.difficulties%3;self.difficulties+=1
            self.events += [2,d&0xffffffff,0,0,0,0]
            if d not in range(3):self.stop();return
            self.ret(d);return
        if a==0x3109e0:assert r0==Q+0x40;self.text_empty=True;self.ret();return
        if a in [STOP+0x108,STOP+0x118,STOP+0x110,0x18]:
            function={STOP+0x108:8,STOP+0x118:0x18,STOP+0x110:0x10,0x18:0x18}[a]
            adjustment=self.row[16]//2 if self.row[0]==8 else 0
            obj=r0-adjustment;oid=self.oid(obj)
            assert oid in range(1,6)
            priority=r1 if function==0x10 else 0;state=r2 if function==0x10 else 0
            self.events += [1,oid,function,adjustment&0xffffffff,priority,state];self.calls+=1
            if self.row[6]>0 and self.calls==self.row[6]:
                if self.row[7]:put(u,obj+0x20,0xf0000000+self.calls);u.mem_write(obj+0x14,b'\x05')
                self.stop();return
            if a==STOP+0x108:u.mem_write(obj+8,b'\x01');u.mem_write(obj+0x14,b'\x01')
            if not self.changed:
                self.changed=True
                m=self.row[5]
                if m==1:put(u,Q+0x2c,0)
                if m==2:put(u,Q+0x30,OA+0x100)
                if m==3:put(u,Q+0x18,OBJECTS+0x300);put(u,Q+0x1c,OBJECTS+0x400)
                if m==4:put(u,Q,self.row[14])
                if m==5:u.mem_write(Q+0x5c,bytes([0 if u.mem_read(Q+0x5c,1)[0] else 1]))
                if m==6:put(u,Q+0x38,0)
                if m==7:put(u,Q+0x3c,RA+0x100)
                if m==8:put(u,LOG+8,VECTORS)
            self.ret();return
        assert any(start<=a<start+n for _,start,n in FUNCTIONS),f'unscoped {a:#x}'
        self.words.add(a)
        for _,start,n in FUNCTIONS:
            if start<=a<start+n:self.coverage[start].add(a)
        if a in [0x48018c,0x4801a8] and not r3:self.stop();return
        if a in [0x479f74,0x47a588] and not r0:self.stop();return
        if a in [0x4801bc,0x4801d8,0x480218,0x480234,0x480284,0x4802a0,0x47a524] and not r3:self.stop();return
        if a in [0x47a578,0x47a518,0x483930,0x482994]:
            if a==0x47a578:base=r2;index=u.reg_read(UC_ARM_REG_R5)
            else:base=r3 if a in [0x47a518,0x482994] else r2;index=u.reg_read(UC_ARM_REG_R4) if a in [0x47a518,0x482994] else r3
            if not base or index>=3:self.stop();return
        if a==0x483938 and not r2:self.stop();return
        if a==0x4829a0 and not r3:self.stop();return
        if a==0x46b87c:
            index=u.reg_read(UC_ARM_REG_R4);d=(r3-VECTORS)//0x100
            if not r3 or d not in range(3):self.stop();return
            live_size=(get(u,LOG+d*12+8)-get(u,LOG+d*12+4))//4
            if index>=live_size:self.stop();return
    def execute(self,row):
        self.row=row;self.events=[];self.failed=self.changed=False;self.text_empty=False;self.calls=self.difficulties=0;u=self.u
        u.mem_write(Q,bytes(0x100));u.mem_write(LOG,bytes(0x100));u.mem_write(OBJECTS,bytes(0x1000));u.mem_write(REWARDS,bytes(0x400));u.mem_write(VT,bytes(0x1000))
        put(u,Q,row[1]);u.mem_write(Q+0x5c,bytes([row[2]&255]));put(u,Q+0x68,ROW);put(u,ROW+0x114,0)
        for i in range(5):
            q=OBJECTS+i*0x100;put(u,q,VT);u.mem_write(q+0x14,bytes([19+i]));u.mem_write(q+8,bytes([29+i]));put(u,q+0x20,100+i)
            if row[0]==8 and row[16]&1:put(u,q+row[16]//2,VT)
        for offset in [8,0x18,0x10]:put(u,VT+offset,STOP+0x100+offset)
        put(u,Q+0x18,0 if row[12]==1 else OBJECTS);put(u,Q+0x1c,0 if row[12]==2 else OBJECTS+0x100)
        for i in range(3):
            put(u,OA+i*4,0 if row[12]==3 and i==0 else OBJECTS+(2+i)*0x100);put(u,OA+0x100+i*4,OBJECTS+(4-i)*0x100)
            q=REWARDS+i*0x100;put(u,q,VT+0x100);u.mem_write(q+8,bytes([39+i]));put(u,q+0xc,ROW+0x200+i*0x10)
            put(u,RA+i*4,0 if row[12]==4 and i==0 else q);put(u,RA+0x100+i*4,REWARDS+(2-i)*0x100)
        put(u,VT+0x100+0xc,0x482880)
        put(u,Q+0x2c,row[3]);put(u,Q+0x30,0 if row[13]&1 else OA);put(u,Q+0x38,row[4]);put(u,Q+0x3c,0 if row[13]&2 else RA)
        self.vector_sizes=[max(row[9]+i,0) for i in range(3)]
        for i,n in enumerate(self.vector_sizes):
            put(u,LOG+i*12+4,VECTORS+i*0x100);put(u,LOG+i*12+8,VECTORS+i*0x100+n*4);u.mem_write(LOG+0x28+i,bytes([row[10]&255]))
            for j in range(n):put(u,VECTORS+i*0x100+j*4,Q)
        put(u,LOG+0x5c,0x10019000)
        starts=[0x480178,0x47a638,0x47a66c,0x47a614,0x47a4ec,0x4838fc,0x482978,0x46b824,0x47a540]
        u.reg_write(UC_ARM_REG_SP,0x20008000);u.reg_write(UC_ARM_REG_LR,STOP);u.reg_write(UC_ARM_REG_R0,Q if row[0]==0 else LOG if row[0]==7 else Q+0x38 if row[0] in [5,6] else Q+0x2c)
        u.reg_write(UC_ARM_REG_R1,row[15] if row[0]==7 else 7 if row[0]==4 else 0x18);u.reg_write(UC_ARM_REG_R2,-9&0xffffffff if row[0]==4 else row[16]&0xffffffff)
        try:u.emu_start(starts[row[0]],STOP+4,count=100000)
        except Exception as error:raise AssertionError(dict(row=row,pc=hex(u.reg_read(UC_ARM_REG_PC)),registers=[hex(u.reg_read(x)) for x in [UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3]])) from error
        assert self.failed or u.reg_read(UC_ARM_REG_PC)==STOP
        out=[int(self.failed),get(u,Q),u.mem_read(Q+0x5c,1)[0],self.oid(get(u,Q+0x18)),self.oid(get(u,Q+0x1c)),get(u,Q+0x2c),1 if get(u,Q+0x30)==OA else 2 if get(u,Q+0x30)==OA+0x100 else 0,get(u,Q+0x38),1 if get(u,Q+0x3c)==RA else 2 if get(u,Q+0x3c)==RA+0x100 else 0,int(self.text_empty)]
        for i in range(5):q=OBJECTS+i*0x100;out += [u.mem_read(q+0x14,1)[0],u.mem_read(q+8,1)[0],get(u,q+0x20)]
        for i in range(3):q=REWARDS+i*0x100;out += [u.mem_read(q+8,1)[0],int(bool(get(u,q+0x14)))]
        out += list(u.mem_read(LOG+0x28,3))
        out += [(get(u,LOG+i*12+8)-get(u,LOG+i*12+4))//4 for i in range(3)]
        out += [len(self.events)//6,*self.events];return word(len(out))+pack(out)
def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['original','cache','compiler','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--library',type=Path);p.add_argument('--dependencies-library',type=Path);p.add_argument('--write-pins',action='store_true');a=p.parse_args()
    if a.write_pins:
        data,names,_=image(a.original);PIN.parent.mkdir(parents=True,exist_ok=True);PIN.write_text(json.dumps(evidence(data,names),indent=2)+'\n');return
    a.output.mkdir(parents=True,exist_ok=True);oracle=Original(a.original);rows=cases();expected=[oracle.execute(row) for row in rows]
    fixture=a.output/'fixtures.bin';native=a.output/'native.bin';exe=a.output/'quest_compile_v1_host.exe';fixture.write_bytes(word(len(rows))+b''.join(pack(row) for row in rows))
    cmd=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror',MODULE/'tests/quest_compile_v1_host.cpp']
    if a.library:cmd.append(a.library)
    else:assert a.dependencies_library;cmd += [MODULE/'quest_compile_v1.cpp',a.dependencies_library]
    cmd += ['-o',exe];subprocess.run(list(map(str,cmd)),check=True,cwd=ROOT)
    dependency=a.library or a.dependencies_library;dlls=sorted(dependency.parent.parent.rglob('*.dll'));before={str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in [dependency,*dlls]}
    env=os.environ.copy();env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),str(a.compiler.parent),env.get('PATH','')])
    run=subprocess.run(list(map(str,[exe,fixture,native,a.cache.resolve()])),env=env,capture_output=True,text=True);assert run.returncode==0,run.stderr
    actual=native.read_bytes();cursor=0
    for i,want in enumerate(expected):
        size=4+struct.unpack_from('<I',actual,cursor)[0]*4;got=actual[cursor:cursor+size]
        if got!=want:raise AssertionError(dict(case=i,row=rows[i],native=list(struct.unpack('<'+'I'*(len(got)//4),got)),original=list(struct.unpack('<'+'I'*(len(want)//4),want))))
        cursor+=size
    assert cursor==len(actual);assert before=={str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in [dependency,*dlls]}
    coverage={hex(start):len(oracle.coverage[start]) for _,start,_ in FUNCTIONS}
    expected_words={hex(start):(size//4-(2 if start==0x47a638 else 1 if start==0x4838fc else 0)) for _,start,size in FUNCTIONS}
    assert coverage==expected_words,(coverage,expected_words)
    report=dict(validation='PASS',cases=len(rows),native_policy_checks=int(run.stdout.strip()),original_reached_words=len(oracle.words),function_reached_words=coverage,scope=__doc__,implementation='selected library' if a.library else 'new TU with selected dependencies',selected_library=str(a.library) if a.library else None,binary_sha256={**before,str(exe):hashlib.sha256(exe.read_bytes()).hexdigest()},source_sha256={str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in [MODULE/'quest_compile_v1.cpp',MODULE/'quest_compile_v1.hpp',MODULE/'tests/quest_compile_v1_host.cpp',Path(__file__).resolve(),PIN]},android_compilation=False,live_gameplay=False)
    if a.library:
        build=next(parent for parent in a.library.parents if(parent/'compile_commands.json').is_file());entries=json.loads((build/'compile_commands.json').read_text());paths={MODULE/'quest_compile_v1.cpp',MODULE/'quest_reward_execution_v1.cpp',MODULE/'quest_instance_v1.cpp'};report['selected_commands']=[entry for entry in entries if Path(entry['file']).resolve() in {x.resolve() for x in paths}];assert len(report['selected_commands'])==len(paths)
        objdump=subprocess.run([str(a.compiler.with_name('objdump.exe')),'-p',str(exe)],capture_output=True,text=True,check=True).stdout
        assert 'DLL Name: libdh2_game_data.dll' in objdump;report['actual_game_data_import']=True
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','binary_sha256','scope','selected_commands']}))
if __name__=='__main__':main()
