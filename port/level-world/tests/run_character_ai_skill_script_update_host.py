"""Original OnSkillUpdate caller with explicit typed Lua/resource providers."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-skill-script-update/original-functions.json'
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def read(at,n):
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(load['p_offset'])+at-int(load['p_vaddr']);return raw[off:off+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert(s['st_value'],s['st_size'])==(at,n);assert hashlib.sha256(read(at,n)).hexdigest()==row['sha256']
        for row in manifest['literal_strings']:
            data=read(int(row['elf_address'],0),row['size']);assert data==row['text'].encode()+b'\0' and hashlib.sha256(data).hexdigest()==row['sha256']
    for row in manifest['script_dependencies']:
        source=ROOT/row['path'];assert source.stat().st_size==row['size'] and digest(source)==row['sha256']
    imports=verify_imports(original,{0x30e310:'__stack_chk_fail'})
    cpu=Cpu(original,False,manifest)
    skill=cpu.data+0x1000;A=cpu.data+0x2000;B=cpu.data+0x3000;SA=cpu.data+0x4000;SB=cpu.data+0x5000;vector=cpu.data+0x6000;data=cpu.data+0x7000
    provider_addresses={0x31b434:0,0x37c390:1,0x31c3cc:2,0x37c494:3,0x31b398:4}
    instructions=set();records=[]
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def store(at,value):cpu.pointer(at,value)
    def script_tag(script):assert script in (SA,SB);return 113 if script==SA else 224
    def text(at):return bytes(cpu.uc.mem_read(at,32)).split(b'\0')[0].decode()
    def return_raw(value):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    for initial in range(3):
      for error in [0,7,0xffffffff]:
       for count in [0,1,3]:
        for mutation in [0,1,2,4,8,16,31]:
            store(skill+4,A);store(A+0x3e4,0 if initial==0 else SA if initial==1 else SB);store(B+0x3e4,SB)
            store(vector,data);store(vector+4,data);trace=[];temp=[None];erased=[0];updated=[0]
            def swap_script():
                owner=word(skill+4);old=word(owner+0x3e4);store(owner+0x3e4,SB if old==SA else SA)
            def observe(_,at,__,___):
                if at not in provider_addresses:
                    if 0x3dabd0<=at<0x3dac90:instructions.add(at)
                    return
                op=provider_addresses[at]
                if op==0:
                    assert temp[0] is None;temp[0]=cpu.reg(0);store(temp[0]+8,0);store(temp[0]+0x24,vector)
                    trace.append([0,0,0,0,0])
                    if mutation&1:store(skill+4,B)
                    return_raw(0x80000001)
                elif op==1:
                    assert cpu.reg(2)==skill+0xc and cpu.reg(3)==temp[0] and text(cpu.reg(1))=='SetSkill'
                    assert cpu.reg(0)==word(word(skill+4)+0x3e4)
                    trace.append([1,script_tag(cpu.reg(0)),12,0,1]);store(temp[0]+8,error);store(vector+4,data+count*0x70)
                    if mutation&2:store(skill+4,B)
                    if mutation&4:swap_script()
                    # Raw Call return is nonzero even when Error.word==0;
                    # caller must inspect the Error word rather than r0.
                    return_raw(0xffffffff)
                elif op==2:
                    assert cpu.reg(0)==vector and cpu.reg(1)==data and cpu.reg(2)==data+count*0x70 and count
                    trace.append([2,0,0,count,0]);store(vector+4,data);erased[0]=1
                    if mutation&8:store(skill+4,B)
                    if mutation&16:swap_script()
                    return_raw(0xffffffff)
                elif op==3:
                    assert cpu.reg(2)==temp[0] and text(cpu.reg(1))=='OnSkillUpdate'
                    assert word(vector)==word(vector+4) and cpu.reg(0)==word(word(skill+4)+0x3e4)
                    trace.append([3,script_tag(cpu.reg(0)),0,0,2]);updated[0]=1
                    store(temp[0]+8,0x80000007);store(vector+4,data+0x70);return_raw(0xffffffff)
                else:
                    assert cpu.reg(0)==temp[0];trace.append([4,0,0,0,0]);store(vector+4,data);return_raw(0xdeadffff)
            hook=cpu.uc.hook_add(UC_HOOK_CODE,observe)
            try:raw_return=cpu.invoke(0x3dabd0,[skill])
            finally:cpu.uc.hook_del(hook)
            has_script=initial!=0 or bool(mutation&1);expected={'status':0,'phase':6,'decision':0 if updated[0] else 2 if has_script else 1,'calls':len(trace),'set_error':error if has_script else 0,'erased':erased[0],'updated':updated[0],'constructed':1,'destroyed':1,'trace':trace}
            actual=json.loads(subprocess.check_output([str(exe),str(initial),str(error),str(count),str(mutation)],text=True));assert actual==expected,(initial,error,count,mutation,expected,actual)
            assert raw_return==0xdeadffff and word(vector)==word(vector+4)
            records.append({'initial_script':initial,'error_word':error,'value_count':count,'mutation':mutation,'matched':True,'discarded_original_void_return':raw_return,'original_result':expected,'compiled_result':actual})
    assert instructions==set(range(0x3dabd0,0x3dac90,4)),sorted(hex(a) for a in set(range(0x3dabd0,0x3dac90,4))-instructions)
    assert cpu.import_calls=={}
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'caller_instructions_observed':len(instructions),'instruction_addresses':[hex(a) for a in sorted(instructions)],'verified_import_identity':imports,'imports_executed':cpu.import_calls,'results':records,'scope':'All48 normal caller instructions execute; compiler stack-corruption trap3dac90 excluded. Five named provider bodies are controlled observing fixtures, not original callee-body comparisons. SetSkill/error/vector and current owner/script are mutated in source order. Actual Lua skill bodies and resource allocator/destructors remain unimplemented native dependencies; no whole skill update pipeline claim.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/character-ai-skill-script-update/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-ai-skill-script-update/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++');assert compiler
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'character_ai_skill_script_update.cpp',MODULE/'tests/character_ai_skill_script_update.cpp'];command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)];build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stdout+build.stderr
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':229,'failure_cases':13,'source_boundary_cases':16,'guard_cases':11,'mismatches':0}
    comparison=oracle(a.original_elf.resolve(),exe);owned=[MODULE/'character_ai_skill_script_update.hpp',*sources,Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-ai-skill-script-update/NOTES.md'];reused=[ROOT/'port/engine-resources/tests/cpu.py',MODULE/'tests/elf_import_identity.py',ROOT/'recovered/scripts/original/data/scripts/skills/_commons.luac']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':digest(a.original_elf),'compiler_command':command,'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in owned},'reused_source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in reused},'new_complete_caller_bodies':1,'new_complete_dependency_bodies':0,'new_lua_callback_bodies':0,'native_wired':True,'native_wiring_compiled_by_this_runner':False,'scope':'Native VM-backed update and cleanup wiring is present in the Android renderer; this focused runner compiles/tests the shared source kernel and ARM-compares OnSkillUpdate only.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['compiler_command','original_arm_comparison']},indent=2))
if __name__=='__main__':main()
