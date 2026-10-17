"""Two source skill-state predicates, executing their original state getter."""
from __future__ import annotations
import argparse,hashlib,json,os,random,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-skill-state-queries/original-functions.json'
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_READ
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert(s['st_value'],s['st_size'])==(at,n)
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(load['p_offset'])+at-int(load['p_vaddr']);assert hashlib.sha256(raw[off:off+n]).hexdigest()==row['sha256']
    cpu=Cpu(original,False,manifest);machine=cpu.data+0x1000;states=[cpu.data+0x2000,cpu.data+0x2100]
    words=list(range(32))+[0xffffffff,0x80000000,0x7fffffff,0x80000006,0x80000007,0x10006,0x10007,0xfffffffe,0xdeadbeef,42,65535,0xffff0006,0xffff0007,0x3f800000,0x7fc00000]
    rng=random.Random(0x3c02e8);words+=list(dict.fromkeys(rng.getrandbits(32) for _ in range(24)))
    scenarios=[(1,w) for w in words]+[(0,0xdeadbeef)]
    # Same original machine with changing live state slot/pointee, no fabricated
    # callback inside the leaf. These final cases prove query-to-query freshness.
    scenarios += [(1,6),(1,7),(0,6),(1,3),(1,0xffffffff),(1,6)]
    instructions=set();records=[]
    def code_hook(_,at,__,___):
        if any(int(r['elf_address'],0)<=at<int(r['elf_address'],0)+r['size'] for r in manifest['functions']):instructions.add(at)
    hook=cpu.uc.hook_add(UC_HOOK_CODE,code_hook)
    try:
        for i,(present,word) in enumerate(scenarios):
            state=states[i%2]
            cpu.uc.mem_write(machine,bytes([0xa5])*64)
            cpu.pointer(machine+0x20,state if present else 0)
            cpu.uc.mem_write(state,struct.pack('<IIII',word,0x6,0x7,0xdeadbeef))
            snapshot_machine=bytes(cpu.uc.mem_read(machine,64));snapshot_state=bytes(cpu.uc.mem_read(state,16))
            for kind,entry in enumerate([0x3c02e8,0x3c0334]):
                reads=[]
                def memory_hook(_,__,at,size,___,____):
                    if machine<=at<machine+64 or any(s<=at<s+16 for s in states):reads.append([at,size])
                mem=cpu.uc.hook_add(UC_HOOK_MEM_READ,memory_hook)
                try:value=cpu.invoke(entry,[machine])
                finally:cpu.uc.hook_del(mem)
                expected_reads=[[machine+0x20,4]]+([[state,4]] if present else [])
                assert reads==expected_reads,(present,word,reads,expected_reads)
                assert bytes(cpu.uc.mem_read(machine,64))==snapshot_machine and bytes(cpu.uc.mem_read(state,16))==snapshot_state
                expected={'status':0,'state_word':word if present else 0xffffffff,'value':value}
                actual=json.loads(subprocess.check_output([str(exe),str(kind),str(present),str(word)],text=True));assert actual==expected,(present,word,kind,actual,expected)
                records.append({'query':kind,'current_present':bool(present),'input_word':word,'data_reads':[['machine+0x20',4]]+([['current+0',4]] if present else []),'matched':True,'original_result':expected,'compiled_result':actual})
    finally:cpu.uc.hook_del(hook)
    expected_instructions={at for r in manifest['functions'] for at in range(int(r['elf_address'],0),int(r['elf_address'],0)+r['size'],4)}
    assert instructions==expected_instructions and cpu.import_calls=={}
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'instructions_observed':len(instructions),'instruction_addresses':[hex(a) for a in sorted(instructions)],'imports_executed':cpu.import_calls,'results':records,'scope':'All instructions in both24B predicates and actual20B SM_GetState execute. No callee/import is modeled. Null current avoids its word read; malformed null this is rejected only by the port API. State/current pointers change between synchronous queries, not during a fabricated callback.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/character-skill-state-queries/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-skill-state-queries/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++');assert compiler
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'character_skill_state_queries.cpp',MODULE/'tests/character_skill_state_queries.cpp'];command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)];build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stdout+build.stderr
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':106,'guard_cases':13,'mismatches':0}
    comparison=oracle(a.original_elf.resolve(),exe);owned=[MODULE/'character_skill_state_queries.hpp',*sources,Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-skill-state-queries/NOTES.md'];reused=[ROOT/'port/engine-resources/tests/cpu.py']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':digest(a.original_elf),'compiler_command':command,'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in owned},'reused_source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in reused},'new_complete_caller_bodies':2,'new_complete_dependency_bodies':0,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['compiler_command','original_arm_comparison']},indent=2))
if __name__=='__main__':main()
