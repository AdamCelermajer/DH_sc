"""Complete original _InitHpMp32B caller with explicit real regen boundaries."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-init-hp-mp/original-functions.json'
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert (s['st_value'],s['st_size'])==(at,n)
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(load['p_offset'])+at-int(load['p_vaddr']);assert hashlib.sha256(raw[off:off+n]).hexdigest()==row['sha256']
    old=Cpu(original,False,manifest);A,B,OWNER=old.data+0x1000,old.data+0x2000,old.data+0x3000
    def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(value):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[];instructions=set()
    for identity in [101,202]:
        character=A if identity==101 else B
        for mutation in range(16):
            old.pointer(OWNER,character);trace=[];effects=[0,0];tail=0x80000000|mutation
            def observe(_,at,__,___):
                if at in (0x3bdca4,0x3bdbb8):
                    phase=int(at==0x3bdbb8);assert old.reg(0)==character and old.reg(1)==0xffffffff
                    trace.append([phase,identity,old.reg(1)]);effects[phase]+=1
                    if phase==0:
                        if mutation&1:old.pointer(OWNER,B)
                        if mutation&2:old.pointer(OWNER,0)
                        # Arbitrary raw r0 left by a VOID provider is discarded.
                        # Binding/context mutations in host have no ARM dynamic
                        # dispatch equivalent: these two direct callees remain
                        # fixed. Caller instructions verify that same rule.
                        returned(0xffffffff if mutation&4 else 7)
                    else:returned(tail)
                elif 0x3b3a70<=at<0x3b3a90:instructions.add(at)
            hook=old.uc.hook_add(UC_HOOK_CODE,observe)
            try:raw_return=old.invoke(0x3b3a70,[character])
            finally:old.uc.hook_del(hook)
            assert raw_return==tail
            selected=word(OWNER);owner=0 if selected==0 else 202 if selected==B else 101
            expected={'status':0,'captured':identity,'calls':2,'last':1,'hp':1,'mp':1,'owner':owner,'effects':effects,'trace':trace}
            actual=json.loads(subprocess.check_output([str(exe),str(identity),str(mutation)],text=True));assert expected==actual,(identity,mutation,expected,actual)
            records.append({'identity':identity,'mutation':mutation,'raw_original_void_tail_r0_discarded':raw_return,'matched':True,'source_result':expected,'compiled_result':actual})
    assert instructions==set(range(0x3b3a70,0x3b3a90,4)) and old.import_calls=={}
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'caller_instructions_observed':len(instructions),'instruction_addresses':[hex(a) for a in sorted(instructions)],'imports_executed':old.import_calls,'results':records,'scope':'All eight original instructions execute, including pop-before-tail-B to RegenMP. HP/MP provider bodies are explicit named observing fixtures and not credited here. Owner-projection mutation does not replace captured this; source raw void r0 is discarded, not misused as a port error. No numeric helper or guessed import is modeled.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/character-init-hp-mp/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-init-hp-mp/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++');assert compiler
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'character_init_hp_mp.cpp',MODULE/'tests/character_init_hp_mp.cpp'];command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)];build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stdout+build.stderr
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':17,'guard_cases':10,'failure_cases':6,'mismatches':0}
    comparison=oracle(a.original_elf.resolve(),exe);owned=[MODULE/'character_init_hp_mp.hpp',*sources,Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-init-hp-mp/NOTES.md'];reused=[ROOT/'port/engine-resources/tests/cpu.py']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':digest(a.original_elf),'compiler_command':command,'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in owned},'reused_source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in reused},'new_complete_caller_bodies':1,'new_complete_dependency_bodies':0,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['compiler_command','original_arm_comparison']},indent=2))
if __name__=='__main__':main()
