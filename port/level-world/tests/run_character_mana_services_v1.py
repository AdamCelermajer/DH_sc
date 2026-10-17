"""Host regression and original ARM comparison for the borrowed mana callers."""
from __future__ import annotations
import argparse,hashlib,json,math,os,shutil,struct,subprocess,sys
from pathlib import Path

MODULE=Path(__file__).resolve().parents[1]
ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-mana-services-v1/original-functions.json'
SOURCE_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
C,P,S=0x10014000,0x10014560,0x10014ff4
APP,DEBUG,ALT_DEBUG=0x99f72c,0x9a1d18,0x9a1e18
O1,O2,VTABLE=0x10033000,0x10033100,0x10030000
GOD='GOD_MANA';TRACE='isTracingChar_Stats'

def digest(path:Path)->str:return hashlib.sha256(path.read_bytes()).hexdigest()
def signed(value:int)->int:return value if value<0x80000000 else value-0x100000000
def bits_float(value:float)->int:return struct.unpack('<I',struct.pack('<f',value))[0]
def float_bits(word:int)->float:return struct.unpack('<f',struct.pack('<I',word))[0]

def direct_cases():
    # mode,online-first,online-second,remote,saved-option,GOD_MANA,trace,+0x14f0 byte,MP,amount
    return [
        [0,0,0,0,0,0,0,0,80,50], [0,0,0,0,0,0,0,0,49,50],
        [0,1,0,1,0,0,0,0,0,99], [0,1,0,0,0,0,0,0,100,100],
        [1,1,0,1,0,0,0,0,12,900], [1,0,0,0,1,0,0,0,12,900],
        [1,0,0,0,0,1,0,0,12,900], [1,0,0,0,0,0,0,7,12,900],
        [1,0,0,0,0,0,0,0,12,13], [1,0,0,0,0,0,1,0,100,30],
        [1,2,0,0,0,0,0,0,100,30], [1,0,1,0,0,0,0,0,100,30],
        [1,0,0,0,0,0,0,0,100,30], [1,0,0,0,0,0,0,0,0,0],
        [0,255,0,0,0,0,0,0,0,0],
        # The DebugSwitches GOT changes inside the first load provider. UseMana
        # must keep its original selected receiver for both GetSwitch calls.
        [6,0,0,0,0,0,0,0,100,30],
    ]

def wrapper_cases():
    # Valid wrappers exercise the original getNumber + __aeabi_f2iz path.
    return [
        [2,0,0,0,0,0,0,0,80,'0.9'],
        [2,1,0,1,0,0,0,0,0,'25.99'],
        [3,0,0,0,0,0,0,0,100,'30.9'],
        [3,0,0,0,1,0,0,0,0,'214.0'],
        [4,0,0,0,0,0,0,0,80,'8.0'],
        [5,0,0,0,0,0,0,0,80,'8.0'],
        # Original wrappers only reject an empty vector; trailing values are ignored.
        [7,0,0,0,0,0,0,0,100,'30.9'],
        [8,0,0,0,0,0,0,0,80,'8.0'],
        # __aeabi_f2iz runs before the integer caller assertion: negative
        # fractions truncate to supported zero, while negative integers do not.
        [3,0,0,0,0,0,0,0,100,'-0.9'],
        [2,0,0,0,0,0,0,0,80,'-0.0'],
    ]

class ArmCpu:
    def __init__(self,path:Path,manifest:dict):
        from unicorn import UC_HOOK_CODE
        from unicorn.arm_const import UC_ARM_REG_PC,UC_ARM_REG_LR
        sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
        from cpu import Cpu
        sys.path.insert(0,str(MODULE/'tests'))
        from elf_import_identity import verify_imports
        self.imports=verify_imports(path,{0x30e4cc:'__aeabi_f2iz'})
        class Wrapped(Cpu):
            def external(cpu,uc,address,size,unused):
                if cpu.imports.get(address)=='__aeabi_f2iz':
                    value=float_bits(cpu.reg(0))
                    if math.isnan(value):cpu.put(0,0x80000000)
                    elif value>=2147483648.0:cpu.put(0,0x7fffffff)
                    elif value< -2147483648.0:cpu.put(0,0x80000000)
                    else:cpu.put(0,int(value)&0xffffffff)
                    uc.reg_write(cpu.pc,uc.reg_read(cpu.lr));return
                return super().external(uc,address,size,unused)
        self.cpu=Wrapped(path,False,manifest);self.cpu.uc.mem_map(0x10000000,0x60000)
        self.covered=set();self.events=[];self.debug=[];self.strings={};self.online_calls=0
        self.remote=False;self.saved=False;self.switches={};self.mp=0;self.saved_mp=0;self.last_read=0
        self.pushes=[];self.operator_calls=0;self.number_calls=0;self.prop_reads=[];self.adds=[];self.has_calls=0;self.mana_exempt_reads=[];self.wrapper_mode=False;self.mode=0;self.rebound=False
        self.cpu.pointer(0x99531c,DEBUG);self.cpu.pointer(0x99828c,APP)
        self.hook=self.cpu.uc.hook_add(UC_HOOK_CODE,self.observe)
        self.PC=UC_ARM_REG_PC;self.LR=UC_ARM_REG_LR

    def word(self,address):return struct.unpack('<I',self.cpu.uc.mem_read(address,4))[0]
    def read_cstr(self,address):
        raw=bytearray()
        for _ in range(512):
            byte=self.cpu.uc.mem_read(address,1)[0]
            if byte==0:return raw.decode('ascii')
            raw.append(byte);address+=1
        raise AssertionError('unterminated provider string')
    def ret(self,value=0):
        self.cpu.put(0,value);self.cpu.uc.reg_write(self.PC,self.cpu.uc.reg_read(self.LR))
    def observe(self,uc,address,size,unused):
        if address==0x3bd40c:self.has_calls+=1
        if address==0x3be034:self.mana_exempt_reads.append(uc.mem_read(C+0x14f0,1)[0])
        if 0x3bd40c<=address<0x3bd40c+236 or 0x3bdef4<=address<0x3bdef4+476 or 0x3b7864<=address<0x3b78ec+136:
            self.covered.add(address)
        if address==0x7fd794:
            ident=O1 if self.online_calls==0 else O2
            byte=(self.online_first if self.online_calls==0 else self.online_second)
            self.online_calls+=1;uc.mem_write(ident+5,bytes([byte&255]));self.events.append([0,0,'',byte]);self.ret(ident)
        elif address==0x33dd10:
            assert self.cpu.reg(0)==C
            value=1 if self.remote else 0;self.events.append([1,C,'',value]);self.ret(value)
        elif address==0x320e14:
            assert self.cpu.reg(0)==APP
            key=self.read_cstr(self.cpu.reg(1));assert key==GOD
            value=1 if self.saved else 0;self.events.append([2,APP,key,value]);self.ret(value)
        elif address==0x337888:
            assert self.cpu.reg(0)==DEBUG
            if self.mode==6 and not self.rebound:
                self.cpu.pointer(0x99531c,ALT_DEBUG);self.rebound=True
            self.debug.append(['load']);self.ret()
        elif address==0x3140ec:
            object_at,key_at=self.cpu.reg(0),self.cpu.reg(1)
            key=self.read_cstr(key_at);self.strings[object_at]=key;self.ret(object_at)
        elif address==0x337a88:
            assert self.cpu.reg(0)==DEBUG,hex(self.cpu.reg(0))
            key=self.strings.get(self.cpu.reg(1));assert key in (GOD,TRACE,'isTracingDebugSwitches')
            value=1 if self.switches.get(key,False) else 0
            self.debug.append(['get',key,value]);self.ret(value)
        elif address==0x318254:
            object_at=self.cpu.reg(0);assert object_at in self.strings;del self.strings[object_at];self.ret()
        elif address==0x3dedb4:
            assert self.cpu.reg(0)==P and self.cpu.reg(1)==S and self.cpu.reg(2)==41
            self.last_read=self.mp;self.prop_reads.append([P,S,41,self.mp]);self.ret(self.mp&0xffffffff)
        elif address==0x3e0708:
            prop,delta=self.cpu.reg(1),signed(self.cpu.reg(2));assert self.cpu.reg(0)==P and prop==41
            self.adds.append([P,prop,delta]);self.mp=(self.mp+delta)&0xffffffff;self.saved_mp=(self.saved_mp+delta)&0xffffffff;self.ret()
        elif address==0x37baf8:
            self.operator_calls+=1;args=self.cpu.reg(0);index=self.cpu.reg(1);assert index==0
            begin=self.word(self.word(args+4));self.ret(begin)
        elif address==0x31bbf0:
            self.number_calls+=1;value_at=self.cpu.reg(0);self.ret(self.word(value_at+8))
        elif address==0x37c7e4:
            self.pushes.append(self.cpu.reg(1));self.ret()

    def prepare(self,case):
        mode,on1,on2,remote,saved,god,trace,move,mp,amount=case
        self.mode=mode;self.rebound=False
        self.online_first=on1;self.online_second=on2;self.remote=bool(remote);self.saved=bool(saved)
        self.switches={GOD:bool(god),TRACE:bool(trace),'isTracingDebugSwitches':False}
        self.online_calls=0;self.events=[];self.debug=[];self.strings={};self.pushes=[];self.prop_reads=[];self.adds=[]
        self.operator_calls=0;self.number_calls=0;self.has_calls=0;self.mana_exempt_reads=[];self.mp=mp&0xffffffff;self.saved_mp=0;self.last_read=0
        self.covered=set();self.cpu.uc.mem_write(self.cpu.stack,bytes(0x10000));self.cpu.uc.mem_write(C,bytes(0x3000))
        self.cpu.pointer(C,VTABLE);self.cpu.pointer(VTABLE+0x54,0x33dd10)
        self.cpu.uc.mem_write(O1,bytes(16));self.cpu.uc.mem_write(O2,bytes(16))
        self.cpu.uc.mem_write(C+0x14f0,bytes([move&255]))
        self.cpu.pointer(0x99531c,DEBUG);self.cpu.pointer(0x99828c,APP)

    def run(self,case):
        self.prepare(case);mode,*_,amount=case
        if mode<2 or mode==6:
            self.cpu.invoke(0x3bdef4 if mode else 0x3bd40c,[C,int(amount)])
        else:
            argobj,vec,value,retvals=0x10018000,0x10018100,0x10018200,0x10018300
            bits=bits_float(float(amount))
            count=0 if mode==4 else (2 if mode in (7,8) else 1);self.cpu.pointer(argobj+4,vec);self.cpu.pointer(vec,value);self.cpu.pointer(vec+4,value+16*count)
            self.cpu.pointer(value+4,1 if mode==5 else 3);self.cpu.pointer(value+8,bits)
            self.cpu.invoke(0x3b7864 if mode in (3,5,7) else 0x3b78ec,[argobj,retvals,C])
        return {'decision':self.cpu.reg(0)&1,'mp':signed(self.mp),'saved_mp':signed(self.saved_mp),
                'calls':self.events,'online_queries':self.online_calls,'remote_queries':sum(c[0]==1 for c in self.events),
                'application_is_saved_option_on_queries':sum(c[0]==2 for c in self.events),'debug_loads':sum(c[0]=='load' for c in self.debug),
                'debug_queries':sum(c[0]=='get' for c in self.debug),'has_calls':self.has_calls,
                'mana_before':signed(self.last_read),'mana_exempt_14f0_byte':self.mana_exempt_reads[-1] if self.mana_exempt_reads else 0,
                'mana_added':1 if self.adds else 0,'last_online':O1 if self.online_calls==1 else O2,
                'last_online_byte':self.online_first if self.online_calls==1 else self.online_second,
                'debug_trace':self.debug,'property_reads':self.prop_reads,'property_adds':self.adds,
                'wrapper_pushes':self.pushes,'wrapper_operator_calls':self.operator_calls,'wrapper_number_calls':self.number_calls}

def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,capture_output=True,text=True,env=env)
    if p.returncode:raise RuntimeError(f'{command}\n{p.stdout}\n{p.stderr}')
    return p.stdout

def compare(original:Path,exe:Path,run_env=None):
    from elftools.elf.elffile import ELFFile
    manifest=json.loads(MANIFEST.read_text(encoding='utf-8'));raw=original.read_bytes()
    assert digest(original)==manifest['original_sha256']==SOURCE_SHA
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def read(at,size):
            seg=next(s for s in loads if s['p_vaddr']<=at and at+size<=s['p_vaddr']+s['p_filesz'])
            offset=int(seg['p_offset'])+at-int(seg['p_vaddr']);return raw[offset:offset+size]
        for row in manifest['functions']:
            address,size=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(address,size)
            assert hashlib.sha256(read(address,size)).hexdigest()==row['sha256'],row['original_symbol']
        field_observations=[]
        for row in manifest.get('field_observations',[]):
            symbol=symbols[row['original_symbol']]
            address,size=int(row['constructor_address'],0),row['constructor_size']
            assert (int(symbol['st_value']),int(symbol['st_size']))==(address,size)
            assert hashlib.sha256(read(address,size)).hexdigest()==row['constructor_sha256']
            checked=[]
            for instruction in (row['zero_register_instruction'],row['field_offset_materialization'],row['field_store']):
                at=int(instruction['address'],0);expected=bytes.fromhex(instruction['bytes_hex'])
                assert read(at,len(expected))==expected,(row['original_symbol'],instruction)
                checked.append({'address':instruction['address'],'bytes_hex':instruction['bytes_hex'],'arm_instruction':instruction['arm_instruction']})
            field_observations.append({'original_symbol':row['original_symbol'],'offset':row['field_offset'],'width_bytes':row['field_width_bytes'],'value':row['field_value'],'instructions':checked,'body_hash_verified_only_for_provenance':True})
        for row in manifest['literals']:
            literal=row['text'].encode('ascii')+b'\0';data=read(int(row['elf_address'],0),len(literal))
            assert data==literal and hashlib.sha256(data).hexdigest()==row['sha256']
        for row in manifest['globals'].values():
            symbol=symbols[row['symbol']];assert int(symbol['st_value'])==int(row['object'],0) and int(symbol['st_size'])==row['size']
    arm=ArmCpu(original,manifest);rows=[];all_covered=set()
    direct=[*direct_cases()]
    for case in direct:
        result=arm.run(case);all_covered.update(arm.covered);actual=json.loads(run([exe,*map(str,case)],env=run_env))
        keys=('decision','mp','saved_mp','calls','online_queries','remote_queries','application_is_saved_option_on_queries','debug_loads','debug_queries','has_calls','mana_before','mana_exempt_14f0_byte','mana_added','last_online','last_online_byte')
        expected={key:result[key] for key in keys};observed={key:actual[key] for key in keys}
        assert actual['status']==0 and observed==expected,(case,observed,expected,actual)
        rows.append({'kind':'integer_helper','input':case,'source':expected,'compiled':observed,'matched':True})
    for case in wrapper_cases():
        result=arm.run(case);all_covered.update(arm.covered);actual=json.loads(run([exe,*map(str,case)],env=run_env))
        assert actual['status']==0 and actual['mp']==result['mp'] and actual['saved_mp']==result['saved_mp'] and actual['calls']==result['calls'],(case,result,actual)
        if case[0] in (2,3,7,8):assert actual['decision']==result['wrapper_pushes'][0],(case,result,actual)
        else:assert result['wrapper_pushes']==[] and not result['calls'],(case,result,actual)
        rows.append({'kind':'lua_callback_wrapper','input':case,'wrapper_pushes':result['wrapper_pushes'],'mp':result['mp'],'calls':result['calls'],'matched':True})
    rows_covered={hex(x) for x in all_covered}
    return {'validation':'PASS','integer_helper_comparisons':len(direct),'wrapper_comparisons':len(wrapper_cases()),'mismatches':0,
        'visited_instruction_addresses':len(rows_covered),'instruction_address_sample':sorted(rows_covered)[:16],
        'verified_imports':arm.imports,'constructor_field_observations':field_observations,'cases':rows,
        'executed_scope':'The original ARM integer HasMana/UseMana bodies and both Lua wrappers execute. GetOnline, the ObjectBase remote virtual target, Application::IsSavedOptionOn, CharProperties::_GetProperty/PROPS_Add, DebugSwitches load/GetSwitch, string construction/destruction, and ReturnValues::pushBoolean are named provider fixtures. Value::getNumber, Arguments::operator[], caller branch logic, the online byte load, signed MP comparison, MP debit call and callback argument/type/f2iz flow execute as original instructions. __aeabi_f2iz relocation identity is checked; its finite int32-range conversion is modeled for positive amounts, negative fractions truncating to zero, and negative zero. The negative-integer assertion paths are excluded.'}

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-mana-services-v1/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/character-mana-services-v1/validation.json')
    parser.add_argument('--selected-executable',type=Path,help='Use an audit executable linked to the selected dh2_level_world shared library; do not compile implementation TUs directly')
    parser.add_argument('--selected-library',type=Path,help='The selected dh2_level_world shared library used by --selected-executable')
    parser.add_argument('--selected-compile-commands',type=Path,help='compile_commands.json from the selected CMake build')
    args=parser.parse_args()
    selected=bool(args.selected_executable or args.selected_library or args.selected_compile_commands)
    if selected and not (args.selected_executable and args.selected_library and args.selected_compile_commands):
        parser.error('selected-library verification requires --selected-executable, --selected-library and --selected-compile-commands')
    sources=[MODULE/'character_mana_services_v1.cpp',MODULE/'tests/character_mana_services_v1.cpp',MODULE/'debug_switches_runtime.cpp',ROOT/'port/game-data/properties.cpp',ROOT/'port/game-data/class_tables.cpp',ROOT/'port/game-data/vitals.cpp']
    compile_command=None;run_env=None;selected_metadata=None
    if selected:
        exe=args.selected_executable.resolve();library=args.selected_library.resolve();compile_db_path=args.selected_compile_commands.resolve()
        if not exe.is_file() or not library.is_file() or not compile_db_path.is_file():parser.error('selected executable/library/compile database must exist')
        compile_db=json.loads(compile_db_path.read_text(encoding='utf-8'))
        cpp_matches=[row for row in compile_db if Path(row['file']).resolve()==sources[0].resolve()]
        assert len(cpp_matches)==1, f'expected exactly one selected-TU compile command, got {len(cpp_matches)}'
        test_matches=[row for row in compile_db if Path(row['file']).resolve()==sources[1].resolve()]
        assert len(test_matches)==1, f'expected exactly one audit-test compile command, got {len(test_matches)}'
        ninja=shutil.which('ninja')
        if not ninja:parser.error('Ninja is required to verify selected target dependencies')
        build_dir=compile_db_path.parent
        selected_commands=run([ninja,'-C',build_dir,'-t','commands',exe.stem])
        normalized_commands=selected_commands.replace('\\','/').casefold()
        selected_sources={Path(row['file']).resolve() for row in compile_db
                          if str(row['file']).replace('\\','/').casefold() in normalized_commands}
        assert sources[0].resolve() in selected_sources and sources[1].resolve() in selected_sources
        all_dlls=sorted(library.parent.rglob('*.dll'))
        runtime_dirs={str(path.parent) for path in all_dlls}
        runtime_dirs.add(str(exe.parent))
        run_env=os.environ.copy();run_env['PATH']=os.pathsep.join(sorted(runtime_dirs))+os.pathsep+run_env.get('PATH','')
        import pefile
        def imports(path):
            image=pefile.PE(str(path));return sorted(entry.dll.decode('ascii',errors='replace') for entry in image.DIRECTORY_ENTRY_IMPORT)
        exe_imports=imports(exe);library_imports=imports(library)
        assert any(name.lower()=='libdh2_level_world.dll' for name in exe_imports),exe_imports
        selected_dso_imports={p.relative_to(build_dir).as_posix():imports(p) for p in [exe,*all_dlls]}
        library_inputs=selected_sources
        cmake_inputs=[]
        for source in library_inputs|{sources[1].resolve()}:
            parent=source.parent
            while parent==ROOT or ROOT in parent.parents:
                candidate=parent/'CMakeLists.txt'
                if candidate.is_file():cmake_inputs.append(candidate)
                if parent==ROOT:break
                parent=parent.parent
        headers=set()
        for folder in {source.parent for source in library_inputs}:
            headers.update(folder.glob('*.h'));headers.update(folder.glob('*.hpp'))
        cmake_inputs.extend([MODULE/'build/character-mana-services-v1-selected-library/CMakeLists.txt',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')])
        hash_inputs=set(sources+[ROOT/'port/engine-resources/tests/cpu.py',MODULE/'tests/elf_import_identity.py']+list(library_inputs)+list(headers)+cmake_inputs)
        before={p.relative_to(ROOT).as_posix():digest(p) for p in sorted(hash_inputs) if p.is_file()}
        exe_rel=exe.relative_to(ROOT).as_posix() if ROOT in exe.parents else str(exe)
        lib_rel=library.relative_to(ROOT).as_posix() if ROOT in library.parents else str(library)
        selected_metadata={'target':'dh2_level_world','mode':'actual_selected_shared_library','library_path':lib_rel,
          'library_sha256':digest(library),'executable_imports':exe_imports,'library_imports':library_imports,
          'selected_dso_imports':selected_dso_imports,'selected_build_commands':selected_commands.splitlines(),
          'runtime_dlls':[{'path':p.relative_to(library.parent).as_posix(),'sha256':digest(p)} for p in all_dlls],
          'selected_tu_compile_command':cpp_matches[0].get('command',cpp_matches[0].get('arguments')),
          'audit_test_compile_command':test_matches[0].get('command',test_matches[0].get('arguments')),
          'compile_database_path':str(compile_db_path),'compile_database_sha256':digest(compile_db_path),
          'selected_source_inputs':len(library_inputs),
          'guarded_adjacent_headers':len(headers),
          'header_guard_scope':'All *.h and *.hpp files adjacent to selected source translation units are conservatively hashed. This is not the exact compiler dependency closure.',
          'cmake_input_paths':sorted({p.relative_to(ROOT).as_posix() for p in cmake_inputs if p.is_file()}),
          'guarded_adjacent_header_paths':sorted(p.relative_to(ROOT).as_posix() for p in headers if ROOT in p.parents)}
        compile_command={'selected_library':selected_metadata,'executable_path':exe_rel,'cmake_build_command':['cmake','--build',str(compile_db_path.parent),'--target','character_mana_services_v1_selected_audit','--parallel','2']}
    else:
        compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
        if not compiler:parser.error('pass --compiler')
        exe=args.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
        command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
        before={p.relative_to(ROOT).as_posix():digest(p) for p in sources+[MODULE/'character_mana_services_v1.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),ROOT/'port/engine-resources/tests/cpu.py',MODULE/'tests/elf_import_identity.py']}
        built=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
        if built.returncode:raise RuntimeError(built.stdout+built.stderr)
        compile_command={'compiler_command':command,'compiler_version':run([compiler,'--version']).splitlines()[0]}
    host=json.loads(run([exe],env=run_env));assert host['validation']=='PASS'
    oracle=compare(args.original_elf.resolve(),exe,run_env)
    if selected:
        assert all(digest(ROOT/relative)==value for relative,value in before.items()),'Selected library/test source inputs changed during replay'
    else:
        assert all(digest(ROOT/relative)==value for relative,value in before.items()),'Inputs changed during build or replay'
    report={'validation':'PASS','host_report':host,'original_arm_comparison':oracle,'original_sha256':SOURCE_SHA,
      'source_sha256':before,**compile_command,
      'binary_sha256':digest(exe),'selected_dso_sha256':{p.relative_to(args.selected_compile_commands.resolve().parent).as_posix():digest(p) for p in ([args.selected_executable.resolve(),*sorted(args.selected_library.resolve().parent.rglob('*.dll'))] if selected else [])},
      'binary_path':exe.relative_to(ROOT).as_posix() if ROOT in exe.parents else str(exe),'complete_caller_bodies':4,
      'complete_dependency_bodies':0,'native_wired':False,'linked_selected_library':selected}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_integer_helpers':oracle['integer_helper_comparisons'],
      'original_wrappers':oracle['wrapper_comparisons'],'mismatches':0,'report':args.report.resolve().as_posix()}))

if __name__=='__main__':main()
