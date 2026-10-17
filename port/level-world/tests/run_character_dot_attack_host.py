"""Original F_DotAttack and direct-mask calculation, with real Debug composition."""
from __future__ import annotations
import argparse,hashlib,json,random,struct,subprocess,sys,tempfile
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-dot-attack/original-functions.json'
A,B,D,D2,OUT=0x10014000,0x10018000,0x9a1d18,0x10030000,0x10028000
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def signed(value):return value if value<0x80000000 else value-0x100000000
def inputs():
    rows=[]
    for element in range(-1,5):
        for amount in (0,1,255,256,0x7fffffff,0x80000000,0xffffff00,0xffffffff):
            rows.append((f'element_{element}_amount_{amount}',[amount,element,1024,512,0xd22026,77,0]))
    for a,b in ((0,0),(0xffffffff,1),(0x80000000,0x7fffffff),(0x7fffffff,0x80000000)):
        for self_target in (0,1):rows.append((f'levels_{a}_{b}_{self_target}',[256,-1,a,b,0xffffffff,0xffffffff,self_target]))
    rng=random.Random(0x3b2e68)
    for i in range(48):rows.append((f'random_{i}',[rng.getrandbits(32),rng.randrange(-1,5),rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),i%2]))
    return rows

def comparison(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert sha(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            segment=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=int(segment['p_offset'])+at-int(segment['p_vaddr']);return raw[offset:offset+n]
        def word(at):return struct.unpack('<I',data(at,4))[0]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (symbol['st_value'],symbol['st_size'])==(at,n) and hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        literal=manifest['debug_literal'];at=int(literal['elf_address'],0);text=literal['text'].encode()+b'\0'
        assert data(at,len(text))==text and hashlib.sha256(text).hexdigest()==literal['sha256']
        for pc,pool in ((0x3b2ec4,0x3b2fe0),(0x3b278c,0x3b2e18)):assert (pc+8+word(pool))&0xffffffff==at
        got=(0x3b2e78+8+word(0x3b2fd4))&0xffffffff;slot=got+word(0x3b2fdc)
        assert got==0x994a98 and slot==0x99531c and word(slot)==D
        context=(0x3b05ac+8+word(0x3b0618))&0xffffffff;assert context==0x9a29a0
        # The two actual original profiling functions are each BX LR. No fake
        # successful no-op dependency is substituted for nonempty code.
        assert data(0x3136b4,4)==data(0x3136b8,4)==bytes.fromhex('1eff2fe1')
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def text(at):
        out=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':out+=old.uc.mem_read(at,1);at+=1
        return out.decode('ascii')
    def reset(level_a,level_b):
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(A,bytes(0x9000));old.uc.mem_write(OUT,b'\xcc'*40)
        old.pointer(A+0xff8+19*4,level_a);old.pointer(B+0xff8+19*4,level_b)
        old.pointer(slot,D);old.uc.mem_write(context,bytes(0x40))
    rows=[];actually_executed=set()
    for name,x in inputs():
        amount,element,level_a,level_b,seed,count,self_target=x;defender=A if self_target else B
        reset(level_a,level_b);calls=[];strings=[];local_strings={}
        def observe(_,at,__,___):
            if at not in (0x337888,0x3140ec,0x337a88,0x3139ac):actually_executed.add(at)
            if at==0x337888:assert old.reg(0)==D;calls.append('load');returned()
            elif at==0x3140ec:
                assert text(old.reg(1))==literal['text'];pointer=old.reg(0);assert pointer not in local_strings
                local_strings[pointer]=True;strings.append(pointer);calls.append('construct');returned(pointer)
            elif at==0x337a88:assert old.reg(0)==D and old.reg(1) in local_strings;calls.append('query');returned(0)
            elif at==0x3139ac:assert old.reg(0) in local_strings;del local_strings[old.reg(0)];calls.append('destroy');returned()
            elif at==0x3b2638:
                assert [old.reg(i) for i in (0,1,2,3)]==[OUT,A,defender,0x20080000]
                sp=old.uc.reg_read(old.sp);assert [get(sp+i*4) for i in range(3)]==[0xffffffff,element&0xffffffff,amount]
                calls.append('calculate') # Observe; original full body runs.
            elif at==0x3dedb4:
                owner=old.reg(0)-0x560;assert owner in (A,defender) and old.reg(1)==owner+0xff4 and old.reg(2)==19
                calls.append('level') # Observe; original actual getter runs.
        hook=old.uc.hook_add(UC_HOOK_CODE,observe);old.invoke(0x3b2e68,[OUT,A,defender,amount,element&0xffffffff]);old.uc.hook_del(hook)
        assert calls==['load','construct','query','destroy','calculate','level','level','load','construct','query','destroy'] and not local_strings
        result=list(struct.unpack('<6iIIii',old.uc.mem_read(OUT,40)))
        c=list(struct.unpack('<4I',old.uc.mem_read(context+0x1c,16)))
        c+=[signed(get(context+0x2c)),*old.uc.mem_read(context+0x30,4)]
        expected={'status':0,'report':[9,3,2,2,2,2,1,2],'output':result,'context':c,'random':[seed,count],'retained_strings':0}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True));assert actual==expected,(name,actual,expected)
        rows.append({'case':name,'input':x,'matched':True,'source_result':expected,'compiled_result':actual})
    caller_rows=[]
    for mutation in range(4):
        for query in (0,1,0x80000000,0xffffffff):
            reset(1024,512);local_strings={};captured=[];calls=[]
            def mutate(operation):
                if mutation==operation+1:old.pointer(slot,D2)
            def observe(_,at,__,___):
                if at not in (0x337888,0x3140ec,0x337a88,0x3139ac,0x3b2638):actually_executed.add(at)
                if at==0x337888:captured.append(old.reg(0));calls.append('load');mutate(0);returned()
                elif at==0x3140ec:assert text(old.reg(1))==literal['text'];local_strings[old.reg(0)]=True;calls.append('construct');mutate(1);returned(old.reg(0))
                elif at==0x337a88:assert old.reg(0)==D and old.reg(1) in local_strings;calls.append('query');mutate(2);returned(query)
                elif at==0x3139ac:assert old.reg(0) in local_strings;del local_strings[old.reg(0)];calls.append('destroy');returned()
                elif at==0x3b2638:
                    assert [old.reg(i) for i in range(4)]==[OUT,A,B,0x20080000]
                    sp=old.uc.reg_read(old.sp);assert [get(sp+i*4) for i in range(3)]==[0xffffffff,4,0xffffffff]
                    old.pointer(OUT,0xffffffff);calls.append('calculate');returned(0x12345678)
            hook=old.uc.hook_add(UC_HOOK_CODE,observe);old.invoke(0x3b2e68,[OUT,A,B,0xffffffff,4]);old.uc.hook_del(hook)
            assert captured==[D] and calls==['load','construct','query','destroy','calculate'] and not local_strings
            expected={'status':0,'calls':5,'captured_debug':D,'selected_debug':get(slot),'amount':-1,'retained_strings':0}
            actual=json.loads(subprocess.check_output([str(exe),'caller',str(mutation),str(query)],text=True));assert actual==expected,(mutation,query,actual,expected)
            caller_rows.append({'mutation':mutation,'raw_query':query,'matched':True,'source_result':expected})
    # Count actual per-instruction observations, excluding intercepted provider
    # entries. Cpu.seen is basic-block coverage and can include a hooked block's
    # skipped tail, so it cannot substantiate dependency instruction execution.
    coverage={row['original_symbol']:sum(address in actually_executed for address in range(int(row['elf_address'],0),int(row['elf_address'],0)+row['size'],4)) for row in manifest['functions']}
    assert coverage['_Z16CF_SetCombatantsP9CharacterS0_ibb']==31 # 128B includes literal word.
    assert coverage['_ZN9Character11F_DotAttackERNS_12AttackResultEPS_S2_ii']==48
    assert all(coverage[name]==0 for name in ('_ZN13DebugSwitches4loadEv','_ZN13DebugSwitches9GetSwitchERKSs','_ZNSsC1EPKcRKSaIcE','_ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv'))
    return {'validation':'PASS','direct_mask_comparisons':len(rows),'caller_capture_comparisons':len(caller_rows),'mismatches':0,
            'results':rows,'caller_capture_results':caller_rows,'executed_instruction_addresses_by_symbol':coverage,'imports_executed':old.import_calls,
            'scope':'Complete original valid-nonnull F_DotAttack caller; original _F_CalculateResult executes its exact mask0x20080000 continuation, actual ResetResult72B/CF_SetCombatants128B/cached GetProperty292B/direct CF__CalcDamage1664B subpath and both genuine4B profiling functions. Both mandatory Debug phases are observed with named Debug/string provider fixtures, not skipped. Separate compiled Runtime uses real owned Debug maps/string backing and verified direct arithmetic. Real filesystem composition is tested separately. Null diagnostics/stack-corruption trap and other calculation masks are excluded; whole CalculateResult body is not credited. No numerical helper or guessed PLT hook exists.'}

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True,type=Path);p.add_argument('--original-elf',required=True,type=Path);p.add_argument('--cache',required=True,type=Path)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-dot-attack/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-dot-attack/validation.json');a=p.parse_args()
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_dot_attack.cpp',MODULE/'tests/character_dot_attack.cpp',MODULE/'debug_switches_runtime.cpp',MODULE/'debug_switches_persistence.cpp',ROOT/'port/game-data/combat.cpp',ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp']
    command=[str(a.compiler),'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    compiled=subprocess.run(command,capture_output=True,text=True)
    if compiled.returncode:raise RuntimeError(compiled.stdout+compiled.stderr)
    configuration=a.cache.resolve()/'DebugSwitches.savegame';before=sha(configuration)
    assert before=='51a3827f0109e16d1520e76d5b19736df3afe38375b91b955ac519f954234d6b'
    folder=Path(tempfile.mkdtemp(prefix='real-files-',dir=exe.parent));host=json.loads(subprocess.check_output([str(exe),str(configuration),str(folder)],text=True))
    assert host=={'validation':'PASS','behavior_cases':54,'guard_cases':14,'failure_cases':10,'real_file_cases':5,'mismatches':0}
    arm=comparison(a.original_elf.resolve(),exe);assert arm['direct_mask_comparisons']==104 and arm['caller_capture_comparisons']==16 and not arm['mismatches'] and sha(configuration)==before
    owned=[MODULE/'character_dot_attack.hpp',MODULE/'character_dot_attack.cpp',MODULE/'tests/character_dot_attack.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-dot-attack/NOTES.md']
    reused=sources[2:]+[MODULE/'debug_switches_runtime.hpp',MODULE/'debug_switches_persistence.hpp',ROOT/'port/game-data/combat_result.hpp',ROOT/'port/game-data/combat.hpp',ROOT/'port/android-native/app/src/main/cpp/native_debug_files.hpp',ROOT/'port/engine-resources/tests/cpu.py']
    generated={path.relative_to(folder).as_posix():{'bytes':path.stat().st_size,'sha256':sha(path)} for path in sorted(folder.glob('*/DebugSwitches.savegame')) if path.is_file()}
    report={'validation':'PASS','host_report':host,'original_arm_comparison':arm,'command':command,'compiler_diagnostics':compiled.stderr,
            'source_sha256':{path.relative_to(ROOT).as_posix():sha(path) for path in owned},'reused_source_sha256':{path.relative_to(ROOT).as_posix():sha(path) for path in reused},
            'original_sha256':sha(a.original_elf),'original_configuration_sha256':before,'real_generated_files_directory':str(folder),'real_generated_files':generated,
            'new_complete_caller_bodies':1,'new_complete_dependency_bodies':0,'bounded_calculation_mask':'0x20080000','whole_calculate_result_body_claimed':False,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({**host,'original_direct_mask_cases':104,'original_caller_capture_cases':16}))
if __name__=='__main__':main()
