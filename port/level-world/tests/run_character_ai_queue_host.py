"""Compare the app-owned source AI queue advance with original ARM instructions."""
import argparse,hashlib,json,os,struct,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/level-world'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AI,A,B=0x10010000,0x10014000,0x10028000
def fixtures():
    base=[3,0,16,0,0,0,1,0,0,0,0,1,0]; rows=[]
    def add(name,changes):
        x=base.copy()
        for i,v in changes.items():x[i]=v
        rows.append((name,x))
    for n in (0,1,2,3,7,8):add('queue_'+str(n),{0:n})
    for t in (1,16,180,0x7fffffff,0xffffffff,0x80000000):
        for d in (0,16,0xffffffff):add('timer_'+str(t)+'_'+str(d),{1:t,2:d})
    for i in range(3,12):add('field_'+str(i),{i:0 if base[i] else 1})
    add('blocked_forced',{3:1,4:1});add('locked_forced',{4:1,5:1})
    add('outside_zone',{7:1,8:0});add('inside_zone',{7:1,8:1})
    for m in (1,2,3):add('owner_mutation_'+str(m),{7:1,12:m})
    add('timer_mutation',{1:10,2:16,12:4})
    add('raw_faerie',{9:0xffffffff});add('raw_follower',{10:7});add('raw_zonable',{11:0x80000000})
    return rows
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True);p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-ai-queue-host');a=p.parse_args()
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);exe=out/'host.exe'
    sources=[MODULE/'character_ai_queue.cpp',MODULE/'character_ai_queue.hpp',MODULE/'tests/character_ai_queue.cpp']
    cmd=[a.compiler,'-std=c++17','-O1','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-pedantic',str(sources[0]),str(sources[2]),'-o',str(exe)]
    subprocess.run(cmd,check=True)
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    assert hashlib.sha256(a.original_elf.read_bytes()).hexdigest()==SHA
    symbol='_ZN6CharAI14IncUpdateQueueEv';manifest={'functions':[{'original_symbol':symbol,'elf_address':'0x3ce9b8','size':568}]}
    old=Cpu(a.original_elf,False,manifest);old.uc.mem_map(0x10000000,0x50000)
    queue=old.symbols['_ZN6CharAI13s_updateQueueE'];timer=old.symbols['_ZN6CharAI13s_updateTimerE']
    # Both source queue readiness and the source CharAI frame use this byte.
    got=0x3ce9c8+8+struct.unpack('<I',old.uc.mem_read(0x3cebdc,4))[0]
    block=old.symbols.get('_ZN9Character12s_stopUpdateE')
    blocked_address=struct.unpack('<I',old.uc.mem_read(got+0x3650,4))[0]
    vtable=old.symbols['_ZTV9Character']+8
    targets={0x31f66c:0}
    for offset,op in ((0x34,1),(0x54,2),(0xc4,3)):
        target=struct.unpack('<I',old.uc.mem_read(vtable+offset,4))[0];targets[target]=op
    records=[]
    for name,x in fixtures():
        n,t,dt,blocked,forced,locked,visible,zoned,in_zone,faerie,follower,zonable,mutation=x
        slots,queue_map=0x10038000,0x10039000
        old.uc.mem_write(slots,bytes(128));old.pointer(queue_map,slots)
        begin=(slots,slots,slots+128,queue_map);end=(slots+n*4,slots,slots+128,queue_map)
        old.uc.mem_write(queue,struct.pack('<8I',*(begin+end)));old.pointer(timer,t)
        old.uc.mem_write(blocked_address,bytes([blocked]))
        for i in range(n):
            ai=AI+i*0x100;owner=A+i*0x1000;controller=0x10030000+i*0x100
            old.pointer(slots+i*4,ai);old.pointer(ai+4,owner);old.pointer(owner,vtable);old.pointer(owner+0x378,controller)
            old.uc.mem_write(controller+8,bytes([locked,forced]));old.uc.mem_write(owner+0x80,bytes([visible]))
            old.uc.mem_write(owner+0x2ee,bytes([zoned]));old.uc.mem_write(owner+0x2f0,bytes([in_zone]))
        old.pointer(B,vtable);old.pointer(B+0x378,0x10037000);old.uc.mem_write(0x10037008,bytes([locked,forced]))
        old.uc.mem_write(B+0x80,bytes([visible]));old.uc.mem_write(B+0x2ee,b'\0');old.uc.mem_write(B+0x2f0,bytes([in_zone]))
        calls=[];rotations=[0]
        def hook(uc,at,size,user):
            if at==0x3cea68:rotations[0]+=1
            if at not in targets:return
            op=targets[at];subject=0 if op==0 else old.reg(0);calls.append([op,subject])
            if op==0:
                value=dt
                if mutation==4:old.pointer(timer,77)
            else:
                value=(faerie,follower,zonable)[op-1]
                if mutation==op:
                    front_slot=struct.unpack('<I',old.uc.mem_read(queue,4))[0]
                    front=struct.unpack('<I',old.uc.mem_read(front_slot,4))[0];old.pointer(front+4,B)
            old.put(0,value);uc.reg_write(old.pc,uc.reg_read(old.lr))
        h=old.uc.hook_add(UC_HOOK_CODE,hook);old.invoke(symbol,[]);old.uc.hook_del(h)
        front_slot=struct.unpack('<I',old.uc.mem_read(queue,4))[0]
        observed_queue=list(struct.unpack('<'+'I'*n,old.uc.mem_read(front_slot,n*4))) if n else []
        observed_timer=struct.unpack('<i',old.uc.mem_read(timer,4))[0]
        run=subprocess.run([str(exe),*(str(v) for v in x)],capture_output=True,text=True,check=True)
        host=json.loads(run.stdout)
        assert host['status']==0 and host['timer']==observed_timer and host['calls']==calls and host['queue']==observed_queue and host['rotations']==rotations[0],(name,host,observed_queue,observed_timer,calls,rotations)
        records.append({'name':name,'input':x,'host':host,'original_calls':calls,'original_queue':observed_queue,'original_timer':observed_timer,'original_rotations':rotations[0]})
    with a.original_elf.open('rb') as f:
        e=ELFFile(f);syms={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()};s=syms[symbol]
        assert (int(s['st_value']),int(s['st_size']))==(0x3ce9b8,568)
    manifest['functions'][0]['sha256']=hashlib.sha256(bytes(old.uc.mem_read(0x3ce9b8,568))).hexdigest();manifest['original_sha256']=SHA
    report={'validation':'PASS','original_arm_cases':len(records),'mismatches':0,'original':manifest,'cases':records,
      'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+[Path(__file__).resolve()]},
      'scope':'Original568B queue body plus actual deque subtraction and no-allocation rotation instructions. Character virtual predicates/GetDt are fixture providers; original deque allocation boundaries, registration/destruction and native binding are not claimed.'}
    (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','original_arm_cases','mismatches')}))
if __name__=='__main__':main()
