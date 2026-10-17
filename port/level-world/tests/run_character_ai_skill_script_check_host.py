"""Both original skill-check callers with real index/getBool instructions."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-skill-script-check/original-functions.json'
FIELDS=('kind','script','set_error','set_count','check_error','count','type','word','identity','string_boolean','mutation')
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def scenarios():
    rows=[]
    def add(**values):
        c=dict(zip(FIELDS,(0,1,0,2,0,2,3,0x3f800000,17,1,0)));c.update(values);rows.append(c)
    for kind in range(2):
      for script in range(3):
       for se in (0,7):
        for sn in (0,2):
         for ce in (0,9):
          for count in range(3):add(kind=kind,script=script,set_error=se,set_count=sn,check_error=ce,count=count)
    profiles=[(0,0,0,1),(1,0,0,1),(1,0x3f800000,0,1),(3,0,0,1),(3,0x80000000,0,1),(3,0x3f800000,0,1),(3,0xbf800000,0,1),(3,0x7f800000,0,1),(3,0xff800000,0,1),(3,0x7fc00000,0,1),(3,1,0,1),(2,0,0,1),(2,0,17,1),(7,0,0,1),(7,0,17,1),(4,0,0,1),(4,1,0,1),(4,0,0,0),(5,0,0,1),(6,0,0,1),(8,0,0,1),(0xffffffff,0,0,1)]
    for kind in range(2):
        for typ,word,identity,string in profiles:add(kind=kind,type=typ,word=word,identity=identity,string_boolean=string)
        for mutation in (1,2,4,8,16,31,32,64,128):add(kind=kind,mutation=mutation,type=4 if mutation==128 else 3)
    assert len(rows)==206;return rows
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
        p=ROOT/row['path'];assert p.stat().st_size==row['size'] and digest(p)==row['sha256']
    imports=verify_imports(original,{0x30df8c:'__aeabi_fcmpeq',0x30e310:'__stack_chk_fail'})
    class FloatCpu(Cpu):
        def external(self,uc,at,size,unused):
            if self.imports.get(at)=='__aeabi_fcmpeq':
                name=self.imports[at];self.import_calls[name]=self.import_calls.get(name,0)+1
                a,b=(struct.unpack('<f',struct.pack('<I',self.reg(i)))[0] for i in range(2));self.put(0,int(a==b));uc.reg_write(self.pc,uc.reg_read(self.lr))
            else:super().external(uc,at,size,unused)
    cpu=FloatCpu(original,False,manifest)
    skill=cpu.data+0x1000;A=cpu.data+0x2000;B=cpu.data+0x3000;SA=cpu.data+0x4000;SB=cpu.data+0x5000;vector=cpu.data+0x6000;data=cpu.data+0x7000;L=cpu.data+0x8000;string=cpu.data+0x9000
    instructions=set();support=set();records=[];total_index=0;total_bool=0
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def store(at,value):cpu.pointer(at,value)
    def text(at):return bytes(cpu.uc.mem_read(at,32)).split(b'\0')[0].decode()
    def returned(value):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    def tag(ptr):assert ptr in (SA,SB);return 113 if ptr==SA else 224
    for c in scenarios():
        store(skill+4,A);store(A+0x3e4,0 if c['script']==0 else SA if c['script']==1 else SB);store(B+0x3e4,SB)
        store(vector,data);store(vector+4,data);cpu.uc.mem_write(string,b'0\0' if c['word'] else b'\0')
        trace=[];string_trace=[];temp=[None];erased=[0];script_bool=[False];selected=[None];calls_index=[0];calls_bool=[0]
        def swap():
            owner=word(skill+4);store(owner+0x3e4,SB if word(owner+0x3e4)==SA else SA)
        def fill(n,final):
            store(vector,data);store(vector+4,data+n*0x70)
            for i in range(n):
                cpu.uc.mem_write(data+i*0x70,bytes(0x70));store(data+i*0x70+4,3);store(data+i*0x70+8,0x3f800000)
            if final and c['kind']<n:
                value=data+c['kind']*0x70;store(value+4,c['type']);store(value+8,c['word']);store(value+0x6c,c['identity']);store(value+0x20,string if c['string_boolean'] else 0)
        def observe(_,at,__,___):
            if 0x3da9dc<=at<0x3daae8 or 0x3db16c<=at<0x3db274:instructions.add(at)
            if 0x3da43c<=at<0x3da490 or 0x31bc80<=at<0x31bd2c:support.add(at)
            if at==0x31b434:
                assert temp[0] is None;temp[0]=cpu.reg(0);store(temp[0]+8,0);store(temp[0]+0x24,vector);trace.append([0,0,0,0,0])
                if c['mutation']&1:store(skill+4,B)
                returned(0xffffffff)
            elif at==0x37c390:
                assert cpu.reg(2)==skill+0xc and cpu.reg(3)==temp[0] and text(cpu.reg(1))=='SetSkill';assert cpu.reg(0)==word(word(skill+4)+0x3e4)
                trace.append([1,tag(cpu.reg(0)),12,0,1]);store(temp[0]+8,c['set_error']);fill(c['set_count'],False)
                if c['mutation']&2:store(skill+4,B)
                if c['mutation']&4:swap()
                returned(0xffffffff)
            elif at==0x31c3cc:
                assert cpu.reg(0)==vector and cpu.reg(1)==data and cpu.reg(2)==data+c['set_count']*0x70 and c['set_count']
                trace.append([2,0,0,c['set_count'],0]);fill(0,False);erased[0]=1
                if c['mutation']&8:store(skill+4,B)
                if c['mutation']&16:swap()
                returned(0xffffffff)
            elif at==0x37c494:
                assert cpu.reg(2)==temp[0] and text(cpu.reg(1))=='OnSkillCheck';assert word(vector)==word(vector+4) and cpu.reg(0)==word(word(skill+4)+0x3e4)
                trace.append([3,tag(cpu.reg(0)),0,0,2]);store(temp[0]+8,c['check_error']);fill(c['count'],True)
                if c['mutation']&32:store(skill+4,0)
                returned(0xffffffff)
            elif at==0x3da43c:
                assert c['kind']==0 and cpu.reg(0)==temp[0] and cpu.reg(1)==0;trace.append([4,0,0,0,0]);calls_index[0]+=1
                # Real original operator[](0) executes: no replacement return.
            elif at==0x31bc80:
                assert cpu.reg(0)==data+c['kind']*0x70;selected[0]=cpu.reg(0);trace.append([5,0,0,0,0]);calls_bool[0]+=1
                # Real original getBool executes, including numeric import and
                # pointer/type branches. Only its four named string Lua callees
                # are explicit controlled dependency fixtures below.
            elif at==0x84c7e0:
                assert selected[0] is not None;string_trace.append(0)
                if c['mutation']&128:store(selected[0]+0x20,0 if word(selected[0]+0x20) else string)
                returned(L)
            elif at==0x84c04c:
                assert cpu.reg(0)==L and cpu.reg(1)==word(selected[0]+0x20);script_bool[0]=bool(cpu.reg(1));string_trace.append(int(script_bool[0]));returned(0xffffffff)
            elif at==0x84b320:
                assert cpu.reg(0)==L and cpu.reg(1)==0xffffffff;string_trace.append(1);returned(17 if script_bool[0] else 0)
            elif at==0x85797c:
                assert cpu.reg(0)==L;string_trace.append(2);returned(0xffffffff)
            elif at==0x31b398:
                assert cpu.reg(0)==temp[0];trace.append([6,0,0,0,0])
                if c['mutation']&64:
                    for i in range(c['count']):store(data+i*0x70+4,0);store(data+i*0x70+8,0)
                fill(0,False);returned(0xdeadffff)
        hook=cpu.uc.hook_add(UC_HOOK_CODE,observe)
        try:source_bool=cpu.invoke(0x3da9dc if c['kind']==0 else 0x3db16c,[skill])
        finally:cpu.uc.hook_del(hook)
        assert source_bool in (0,1);has_script=c['script']!=0 or bool(c['mutation']&1);second=has_script and c['set_error']==0
        decision=1 if not has_script else 2 if c['set_error'] else 3 if c['check_error'] else 4 if c['count']<=c['kind'] else 0
        expected={'status':0,'phase':8,'decision':decision,'calls':len(trace),'set_error':c['set_error'] if has_script else 0,'check_error':c['check_error'] if second else 0,'count':c['count'] if second and not c['check_error'] else 0,'boolean':source_bool,'erased':erased[0],'constructed':1,'destroyed':1,'trace':trace,'string_trace':string_trace}
        actual=json.loads(subprocess.check_output([str(exe),*(str(c[k]) for k in FIELDS)],text=True));assert actual==expected,(c,expected,actual)
        total_index+=calls_index[0];total_bool+=calls_bool[0];records.append({'input':c,'matched':True,'original_result':expected,'compiled_result':actual})
    caller_expected=set(range(0x3da9dc,0x3daae8,4))|set(range(0x3db16c,0x3db274,4));assert instructions==caller_expected,sorted(hex(a) for a in caller_expected-instructions)
    # Indexed getter's normal bounds path executes; its assertion branch is
    # not claimed. getBool's complete type dispatch executes across profiles.
    assert set(range(0x31bc80,0x31bd2c,4))<=support
    assert all(n=='__aeabi_fcmpeq' for n in cpu.import_calls)
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'caller_instructions_observed':len(instructions),'caller_instruction_addresses':[hex(a) for a in sorted(instructions)],'support_instruction_addresses':[hex(a) for a in sorted(support)],'actual_index_calls':total_index,'actual_get_bool_calls':total_bool,'verified_import_identity':imports,'imports_executed':cpu.import_calls,'results':records,'scope':'Complete normal Usable288B/Active284B caller instructions; actual indexed getter normal path and actual172B getBool type dispatch run. Resource/Lua Call/erase bodies and four string Lua dependencies are named fixtures; numerical equality is modeled only after actual relocated import identity. No new getBool/index/VM body credit. Compiler canary traps/literal pools and indexed assertion branch excluded.'}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/character-ai-skill-script-check/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-ai-skill-script-check/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++');assert compiler
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'character_ai_skill_script_check.cpp',MODULE/'tests/character_ai_skill_script_check.cpp'];command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)];build=subprocess.run(command,capture_output=True,text=True);assert build.returncode==0,build.stdout+build.stderr
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':212,'failure_cases':27,'source_boundary_cases':16,'guard_cases':14,'mismatches':0}
    comparison=oracle(a.original_elf.resolve(),exe);owned=[MODULE/'character_ai_skill_script_check.hpp',*sources,Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-ai-skill-script-check/NOTES.md'];reused=[MODULE/'character_ai_skill_script_update.hpp',ROOT/'port/engine-resources/tests/cpu.py',MODULE/'tests/elf_import_identity.py',ROOT/'recovered/scripts/original/data/scripts/skills/_commons.luac']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':digest(a.original_elf),'compiler_command':command,'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in owned},'reused_source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in reused},'new_complete_caller_bodies':2,'new_complete_dependency_bodies':0,'new_lua_callback_bodies':0,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['compiler_command','original_arm_comparison']},indent=2))
if __name__=='__main__':main()
