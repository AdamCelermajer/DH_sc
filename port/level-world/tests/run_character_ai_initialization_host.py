"""Compare native CharAI initialization with both original 352-byte constructors."""
import argparse, csv, hashlib, json, random, struct, subprocess, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / 'port/level-world'
SHA = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
MANIFEST = MODULE / 'reference/character-ai-initialization/original-functions.json'
AI, SLOTS, MAP = 0x10010000, 0x10020000, 0x10021000
BYTE_OFFSETS = {0x18,0x24,0x2c,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x54,0x55,0x5c,0x7c,0x94,0xd0,0xd1}
OFFSETS = [0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,73,74,75,76,77,80,84,85,88,
           0x5c,0x60,0x64,0x68,0x6c,0x7c,0x80,0x84,0x88,0x8c,0x94,0x98,0x9c,0xa0,0xa4,
           0xac,0xb0,0xb4,0xb8,0xbc,0xc0,0xc4,0xc8,0xcc,0xd0,0xd1]

def digest(path): return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',required=True)
    parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-ai-initialization-host')
    a=parser.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);exe=out/'host.exe'
    sources=[MODULE/'character_ai_initialization.cpp',MODULE/'character_ai_initialization.hpp',MODULE/'tests/character_ai_initialization.cpp']
    command=[a.compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',str(sources[0]),str(sources[2]),'-o',str(exe)]
    subprocess.run(command,check=True)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS'
    assert digest(a.original_elf)==SHA
    manifest=json.loads(MANIFEST.read_text(encoding='utf-8'))
    from elftools.elf.elffile import ELFFile
    with a.original_elf.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        raw=a.original_elf.read_bytes();loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for f in manifest['functions']:
            at=int(f['elf_address'],0);n=f['size'];s=symbols[f['original_symbol']]
            assert (s['st_value'],s['st_size'])==(at,n)
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            start=load['p_offset']+at-load['p_vaddr'];assert hashlib.sha256(raw[start:start+n]).hexdigest()==f['sha256']
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    from unicorn import UC_HOOK_CODE
    old=Cpu(a.original_elf,False,manifest);old.uc.mem_map(0x10000000,0x30000)
    queue=old.symbols['_ZN6CharAI13s_updateQueueE'];table=old.symbols['_ZTV6CharAI']+8
    def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def fields():
        return [[offset,old.uc.mem_read(AI+offset,1)[0] if offset in BYTE_OFFSETS else word(AI+offset)] for offset in OFFSETS]
    records=[]
    rng=random.Random(3704);fills=[0,165,255]+[rng.randrange(256) for _ in range(5)]
    for constructor,at in (('C2',0x3cebf0),('C1',0x3ced50)):
        for fill in fills:
            for count,grow in ((0,False),(4,False),(31,True)):
                for mutation in (0,1):
                    initial=bytes([fill])*0xd4;old.uc.mem_write(AI,initial)
                    old.uc.mem_write(SLOTS,bytes(128));old.pointer(MAP,SLOTS)
                    for i in range(count):old.pointer(SLOTS+i*4,0x10022000+i*4)
                    old.uc.mem_write(queue,struct.pack('<8I',SLOTS,SLOTS,SLOTS+128,MAP,SLOTS+count*4,SLOTS,SLOTS+128,MAP))
                    trace=[];before=[]
                    def observe(uc,address,size,user):
                        if address not in (at+0x12c,0x3ce810):return
                        if address==0x3ce810:
                            appended=word(old.reg(0));assert grow
                        else:
                            appended=old.reg(4);assert not grow
                        assert appended==AI;trace.append(appended);before.extend(fields())
                        if mutation:
                            old.uc.mem_write(AI+0x4d,bytes([17]));old.pointer(AI+0x1c,0x10018000);old.pointer(AI+4,0x1001a000)
                        if address==0x3ce810:
                            # Explicit original deque allocation boundary. Its
                            # own allocator/blocks are not reconstructed here.
                            end=word(queue+0x10);old.pointer(end,AI);old.pointer(queue+0x10,end+4)
                            uc.reg_write(old.pc,uc.reg_read(old.lr))
                    hook=old.uc.hook_add(UC_HOOK_CODE,observe)
                    returned=old.invoke(at,[AI]);old.uc.hook_del(hook)
                    expected={'status':0,'queue_calls':1,'queued':1,'appended':AI,'fields':fields(),'before_queue':before}
                    actual=json.loads(subprocess.check_output([str(exe),str(fill),str(table),str(mutation),'0'],text=True))
                    assert actual==expected,(constructor,fill,count,mutation,actual,expected)
                    assert returned==AI and trace==[AI] and word(queue+0x10)==SLOTS+(count+1)*4
                    assert word(SLOTS+count*4)==AI
                    # The ctor never stores owner+4/alive+48. Observe every
                    # unmodeled byte too so new writes cannot hide in padding.
                    written=set()
                    for offset in OFFSETS:
                        if offset in (4,0x48):continue
                        written.update(range(offset,offset+(1 if offset in BYTE_OFFSETS else 4)))
                    if mutation:written.update(range(4,8));written.update(range(0x1c,0x20));written.add(0x4d)
                    final=bytes(old.uc.mem_read(AI,len(initial)))
                    assert all(final[i]==initial[i] for i in range(len(initial)) if i not in written)
                    records.append({'constructor':constructor,'initial_fill':fill,'prior_queue_count':count,
                        'allocation_boundary_modeled':grow,'registration_callback_mutation':bool(mutation),
                        'matched':True,'source_result':expected})
    paths=sources+[Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_cases':len(records),'mismatches':0,
            'original_sha256':SHA,'compiler_command':command,'cases':records,
            'native_wired':False,'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in paths},
            'scope':'Both complete original352B CharAI constructors execute. Scalar writes, unchanged owner/alive and every untouched byte, three native empty-tree headers, native circular-list header and queue append match. The growth helper is an explicit fixture allocator boundary; active AIS/Character association and full lifecycle are separate.'}
    (out/'validation.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':len(records),'mismatches':0}))

if __name__=='__main__':main()
