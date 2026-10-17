"""Differentially exercise Objective save virtuals against the original ARM ELF."""
from __future__ import annotations
import argparse, hashlib, json, os, struct, subprocess
from pathlib import Path
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_SP

ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/game-data'
sys_path=ROOT/'port/player-info-level/tests'
import sys
sys.path.insert(0,str(sys_path))
from player_locality_v1_original import image, ELF_SHA
PIN=MODULE/'reference/quest-objective-save-data-v1/original-functions.json'
Q=0x10001000; STREAM=0x10002000; VTABLE=0x10003000
STOP=0x30000000; WRITE=STOP+0x100
BASE=0x47ab74; SAVED_QTY=0x47ab84
BASE_DISPATCH={7,9,15}; QTY_DISPATCH={3,4,5,6,8,10,11,12,13,14}

def cases():
    rows=[]
    for dispatch in sorted(BASE_DISPATCH|QTY_DISPATCH):
        for done in [0,1,9,128,255]:
            for quantity in [0,0x12345678,0xffffffff]:
                rows.append([dispatch,done,quantity,0,0])
    # Inject incomplete byte counts at every virtual stream write. The source
    # derived virtual ignores the base result and still issues its qty write.
    for dispatch in sorted(BASE_DISPATCH|QTY_DISPATCH):
        rows.append([dispatch,0x80,0x87654321,1,0])
        if dispatch in QTY_DISPATCH:
            rows.extend([[dispatch,0x80,0x87654321,2,0],
                         [dispatch,0x80,0x87654321,2,2]])
    return rows

class Original:
    def __init__(self,data):
        self.u=Uc(UC_ARCH_ARM,UC_MODE_ARM)
        self.u.mem_map(0,len(data));self.u.mem_write(0,data)
        for start,size in [(0x10000000,0x10000),(STOP,0x1000)]:self.u.mem_map(start,size)
        self.row=None;self.trace=[]
        self.u.hook_add(UC_HOOK_CODE,self.hook)
        self.u.mem_write(STREAM,struct.pack('<I',VTABLE))
        self.u.mem_write(VTABLE+0x1c,struct.pack('<I',WRITE))
    def hook(self,u,address,size,user_data):
        if address==STOP:u.emu_stop();return
        if address!=WRITE:return
        stream=u.reg_read(UC_ARM_REG_R0);ptr=u.reg_read(UC_ARM_REG_R1);length=u.reg_read(UC_ARM_REG_R2)
        assert stream==STREAM and length in (1,4)
        payload=bytes(u.mem_read(ptr,length));ordinal=len(self.trace)+1
        fail_call,accepted=self.row[3],self.row[4]
        count=min(length,max(0,accepted)) if ordinal==fail_call else length
        self.trace.append([length,list(payload[:count])])
        # StreamReader::writeAs returns bytes written in R0. Clearing R1 keeps
        # its source assertion's error flag false; default assert level is 0.
        u.reg_write(UC_ARM_REG_R0,count);u.reg_write(UC_ARM_REG_R1,0)
        u.reg_write(UC_ARM_REG_PC,u.reg_read(UC_ARM_REG_LR))
    def execute(self,row):
        self.row=row;self.trace=[];u=self.u
        u.mem_write(Q,bytes([0xcc])*48)
        u.mem_write(Q+0x14,bytes([row[1]&0xff]))
        u.mem_write(Q+0x20,struct.pack('<I',row[2]&0xffffffff))
        fn=BASE if row[0] in BASE_DISPATCH else SAVED_QTY
        u.reg_write(UC_ARM_REG_R0,Q);u.reg_write(UC_ARM_REG_R1,STREAM)
        u.reg_write(UC_ARM_REG_SP,0x10008000);u.reg_write(UC_ARM_REG_LR,STOP)
        u.emu_start(fn,STOP+4,count=1000)
        assert u.reg_read(UC_ARM_REG_PC)==STOP
        return {"calls":[x[0] for x in self.trace],"bytes":[b for x in self.trace for b in x[1]]}

def main():
    p=argparse.ArgumentParser();p.add_argument('--original',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--build',type=Path,required=True);a=p.parse_args()
    data,_,_=image(a.original);pins=json.loads(PIN.read_text())
    assert pins['original_sha256']==ELF_SHA
    for entry in pins['functions']:
        start=int(entry['address'],0);size=entry['size']
        assert hashlib.sha256(data[start:start+size]).hexdigest()==entry['sha256'],entry['name']
    rows=cases();oracle=Original(data);expected=[oracle.execute(row) for row in rows]
    a.output.mkdir(parents=True,exist_ok=True)
    inputs=a.output/'objective-save-cases.txt'
    inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
    build=a.build.resolve();exe=build/'quest_objective_save_data_v1_audit.exe'
    library=build/'libdh2_game_data.dll';compile_db=json.loads((build/'compile_commands.json').read_text())
    selected_source=[entry for entry in compile_db if Path(entry['file']).resolve()==(MODULE/'quest_objective_factory_v1.cpp').resolve()]
    selected_host=[entry for entry in compile_db if Path(entry['file']).resolve()==(MODULE/'tests/quest_objective_save_data_v1_host.cpp').resolve()]
    assert len(selected_source)==len(selected_host)==1 and exe.is_file() and library.is_file()
    env=os.environ.copy();env['PATH']=os.pathsep.join([str(build),env.get('PATH','')])
    binary_before=hashlib.sha256(library.read_bytes()).hexdigest()
    run=subprocess.run([str(exe),str(inputs)],capture_output=True,text=True,env=env)
    assert run.returncode==0,run.stderr
    actual=json.loads(run.stdout);assert len(actual)==len(rows)
    mismatches=[{"row":row,"original":want,"native":got} for row,want,got in zip(rows,expected,actual)
                if want['calls']!=got['calls'] or want['bytes']!=got['bytes']]
    (a.output/'mismatches.json').write_text(json.dumps(mismatches,indent=2)+'\n')
    assert not mismatches,f'{len(mismatches)} differential mismatches'
    # Successful writes return complete; a false native sink callback maps to
    # failure, while the source-derived method still attempts its next write.
    expected_failures=[int(row[3]>0) for row in rows]
    assert [int(item['status']==3) for item in actual]==expected_failures
    report={"validation":"PASS","cases":len(rows),"mismatches":0,
            "original_sha256":ELF_SHA,"original_functions":pins['functions'],
            "covered_dispatches":sorted(BASE_DISPATCH|QTY_DISPATCH),
            "covered_raw_done_bytes":[0,1,9,128,255],
            "failure_cases":sum(expected_failures),
            "derived_continues_after_base_write_failure":True,
            "implementation":"selected dh2_game_data library and CMake host executable",
            "selected_source_command":selected_source[0]['command'],
            "selected_test_command":selected_host[0]['command'],
            "library_sha256":binary_before,
            "qest_writer":False,"native_savegame_wired":False,
            "android_compilation":False,"live_gameplay":False}
    assert hashlib.sha256(library.read_bytes()).hexdigest()==binary_before
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ['validation','cases','mismatches','failure_cases','implementation']}))

if __name__=='__main__':main()
