"""Original Value::getBool172B differential and isolated real Lua dependencies."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/script-value-boolean/original-functions.json'
FIELDS=('type','word','identity','text_mode','raw_lua_boolean','mutation')
LUA=ROOT/'port/adam-script-runtime/lua'
CORE='lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib'.split()
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def scenarios():
    rows=[]
    def add(**values):
        c=dict(zip(FIELDS,(4,0,17,2,17,0)));c.update(values);rows.append(c)
    for tag in list(range(32))+[0x80000000,0xffffffff]:add(type=tag)
    words=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc00000,0x7f800001,0xff800001,0xffffffff,0x7f7fffff,0xff7fffff,0x00000100,0x80000100]
    for tag in (1,3):
        for word in words:add(type=tag,word=word)
    for tag in (2,7):
        for identity in (0,17,0xffffffff):add(type=tag,identity=identity)
    for mode in range(5):
        for raw in (0,1,17,0xffffffff):
            for mutation in (0,1,2,4,8,16,32,64,128,255):add(text_mode=mode,raw_lua_boolean=raw,mutation=mutation)
    assert len(rows)==276;return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    sys.path.insert(0,str(MODULE/'tests'));from elf_import_identity import verify_imports
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']];assert(s['st_value'],s['st_size'])==(at,n)
            load=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(load['p_offset'])+at-int(load['p_vaddr'])
            assert hashlib.sha256(raw[off:off+n]).hexdigest()==row['sha256']
    imports=verify_imports(original,{0x30df8c:'__aeabi_fcmpeq'})
    class FloatCpu(Cpu):
        def external(self,uc,at,size,unused):
            if self.imports.get(at)=='__aeabi_fcmpeq':
                name=self.imports[at];self.import_calls[name]=self.import_calls.get(name,0)+1
                a,b=(struct.unpack('<f',struct.pack('<I',self.reg(i)))[0] for i in range(2))
                self.put(0,int(a==b));uc.reg_write(self.pc,uc.reg_read(self.lr))
            else:super().external(uc,at,size,unused)
    cpu=FloatCpu(original,False,manifest);value=cpu.data+0x1000;L=cpu.data+0x2000;texts=cpu.data+0x3000
    pointers=[0,texts,texts+32,texts+64,texts+96]
    for i,data in enumerate((b'\0',b'0\0',b'false\0',b'\0hidden\0')):cpu.uc.mem_write(texts+i*32,data)
    def word(at):return struct.unpack('<I',cpu.uc.mem_read(at,4))[0]
    def store(at,v):cpu.pointer(at,v)
    def text_tag(at):return 0 if not at else 1 if not cpu.uc.mem_read(at,1)[0] else 2 if cpu.uc.mem_read(at,1)[0]==ord('0') else 3
    def returned(v):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
    instructions=set();records=[]
    for c in scenarios():
        cpu.uc.mem_write(value,bytes(0x70));store(value+4,c['type']);store(value+8,c['word']);store(value+0x6c,c['identity']);store(value+0x20,pointers[c['text_mode']])
        trace=[];alive=[False]
        def observe(_,at,__,___):
            if 0x31bc80<=at<0x31bd2c:instructions.add(at)
            if at==0x84c7e0:
                assert not alive[0];alive[0]=True;trace.append([0,0,0,0])
                if c['mutation']&1:store(value+0x20,0)
                if c['mutation']&2:store(value+0x20,pointers[1])
                if c['mutation']&4:store(value+4,0)
                # Bits8/16 alter only the native service binding, not original
                # fixed call targets; caller control/trace remains identical.
                returned(L)
            elif at==0x84c04c:
                assert alive[0] and cpu.reg(0)==L and cpu.reg(1)==word(value+0x20)
                trace.append([1,1,text_tag(cpu.reg(1)),0]);returned(0xffffffff)
            elif at==0x84b320:
                assert alive[0] and cpu.reg(0)==L and cpu.reg(1)==0xffffffff
                trace.append([2,1,0,0xffffffff])
                if c['mutation']&32:store(value+0x20,0)
                returned(c['raw_lua_boolean'])
            elif at==0x85797c:
                assert alive[0] and cpu.reg(0)==L;alive[0]=False;trace.append([3,1,0,0])
                if c['mutation']&128:cpu.uc.mem_write(value,bytes(0x70))
                # Arbitrary close return cannot overwrite captured boolean.
                returned(0 if c['mutation']&64 and c['raw_lua_boolean'] else 17)
        hook=cpu.uc.hook_add(UC_HOOK_CODE,observe)
        try:result=cpu.invoke(0x31bc80,[value])
        finally:cpu.uc.hook_del(hook)
        assert result in (0,1) and not alive[0]
        expected={'status':0,'type':c['type'],'value':result,'raw':c['raw_lua_boolean'] if c['type']==4 else 0,'phase':5,'calls':len(trace),'closed':int(c['type']==4),'trace':trace}
        actual=json.loads(subprocess.check_output([str(exe),*(str(c[k]) for k in FIELDS)],text=True));assert actual==expected,(c,expected,actual)
        records.append({'input':c,'matched':True,'original_result':expected,'compiled_result':actual})
    assert instructions==set(range(0x31bc80,0x31bd2c,4))
    assert set(cpu.import_calls)=={'__aeabi_fcmpeq'}
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'caller_instructions_observed':len(instructions),'caller_instruction_addresses':[hex(a) for a in sorted(instructions)],'verified_import_identity':imports,'imports_executed':cpu.import_calls,'results':records,'scope':'All43 original getBool instructions run; float equality modeled only after actual relocated import proof. Four original Lua dependency entry points are explicit ordering/mutation fixtures, not dependency body reconstruction. Separate native adapter test executes real reused Lua C API.'}
def run(command):
    r=subprocess.run(command,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    return r.stdout
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--c-compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,default=MODULE/'build/script-value-boolean/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/script-value-boolean/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++');assert compiler
    cc=a.c_compiler or str(Path(compiler).with_name('gcc.exe' if Path(compiler).suffix=='.exe' else 'gcc'));assert Path(cc).exists() or shutil.which(cc)
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);sources=[MODULE/'script_value_boolean.cpp',MODULE/'tests/script_value_boolean.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)];run(command)
    host=json.loads(run([str(exe)]));assert host=={'validation':'PASS','host_cases':269,'failure_cases':11,'guard_cases':8,'mismatches':0}
    comparison=oracle(a.original_elf.resolve(),exe)
    lua_sources=[LUA/(name+'.c') for name in CORE];objects=[];lua_commands=[];object_dir=exe.parent/'lua-objects';object_dir.mkdir(exist_ok=True)
    for source in lua_sources:
        obj=object_dir/(source.stem+'.o');cmd=[cc,'-std=c99','-O1','-fno-fast-math','-ffp-contract=off','-I',str(LUA),'-c',str(source),'-o',str(obj)];run(cmd);objects.append(obj);lua_commands.append(cmd)
    adapter_exe=exe.with_name('real-lua'+exe.suffix);adapter_sources=[MODULE/'script_value_boolean.cpp',MODULE/'script_value_boolean_lua.cpp',MODULE/'tests/script_value_boolean_lua.cpp']
    link=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-I',str(LUA),*map(str,adapter_sources),*map(str,objects),'-lm','-o',str(adapter_exe)];run(link)
    actual=json.loads(run([str(adapter_exe)]));assert actual=={'validation':'PASS','real_lua_cases':42,'failure_cases':6,'temporary_resources_retained_on_error':True,'no_libraries_opened':True,'mismatches':0}
    owned=[MODULE/'script_value_boolean.hpp',MODULE/'script_value_boolean.cpp',MODULE/'script_value_boolean_lua.hpp',MODULE/'script_value_boolean_lua.cpp',MODULE/'tests/script_value_boolean.cpp',MODULE/'tests/script_value_boolean_lua.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/script-value-boolean/NOTES.md']
    # The retained Lua license is the original notice at the end of lua.h,
    # which is already hashed with every compiled dependency header below.
    reused=[ROOT/'port/engine-resources/tests/cpu.py',MODULE/'tests/elf_import_identity.py',*lua_sources,*sorted(LUA.glob('*.h'))]
    report={'validation':'PASS','host_report':host,'real_lua_report':actual,'original_arm_comparison':comparison,'original_sha256':digest(a.original_elf),'compiler_command':command,'lua_commands':lua_commands,'lua_link_command':link,'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in owned},'reused_source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in reused},'new_complete_caller_bodies':1,'new_complete_dependency_bodies':0,'real_lua_dependency_adapter':True,'whole_lua_vm_parity':False,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ['compiler_command','lua_commands','lua_link_command','original_arm_comparison','reused_source_sha256']},indent=2))
if __name__=='__main__':main()
