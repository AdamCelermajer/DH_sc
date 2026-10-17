"""AIS constructor/SetCharacter callers vs original ARM; named resource services."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/ais-external-initialization/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AIS,C,AI=0x10014000,0x10020000,0x10030000

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert digest(original)==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[offset:offset+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['vtable_ranges']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
            for slot in row['slots']:
                assert struct.unpack('<I',data(int(row['address_point'],0)+int(slot['byte_offset'],0),4))[0]==int(slot['target'],0)
        path=manifest['path_literal'];literal=int(path['elf_address'],0)
        assert data(literal,17)==b'data/scripts/ai/\0'
        assert hashlib.sha256(data(literal,17)).hexdigest()==path['sha256']
        assert (0x3d9134+8+struct.unpack('<I',data(0x3d91a0,4))[0])&0xffffffff==literal
        char_table=int(symbols['_ZTV12CharAIScript']['st_value'])+8
        external_table=int(symbols['_ZTV11AISExternal']['st_value'])+8
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def byte(at):return old.uc.mem_read(at,1)[0]
    def returned(value):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def string(at):
        raw=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':raw+=old.uc.mem_read(at,1);at+=1
        return raw.decode('ascii')
    def snapshot():
        return [get(AIS+0x98),get(AIS),get(AIS+0xb4),get(AIS+0xb8),get(AIS+0xbc),get(AIS+0xc0),
            get(AIS+0xa0),byte(AIS+0x9c),int(get(AIS+0xa4)==AIS+0x9c),int(get(AIS+0xa8)==AIS+0x9c),get(AIS+0xac),0]
    def reset():
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(AIS,bytes(0x100));old.uc.mem_write(C,bytes(0x20));old.pointer(C,0x965f38)
        for at,value in [(0,0x10101010),(0x98,0x20202020),(0xa0,0x30303030),(0xa4,0x40404040),
            (0xa8,0x50505050),(0xac,11),(0xb4,0x60606060),(0xb8,0x70707070),(0xbc,0x80808080),(0xc0,0x90909090)]:old.pointer(AIS+at,value)
        old.uc.mem_write(AIS+0x9c,b'\x07')
    records=[]
    entries=[(0,0x3d8f44),(0,0x3d8fb0),(1,0x3dd0e4),(1,0x3dd128),(2,0x3d90f8)]
    for mode,entry in entries:
        for skip in ((0,1) if mode<2 else (0,)):
            for mutation in range(10):
                reset();calls=[];path_value=[''];writes=[]
                def mutate(op):
                    if mutation==1 and op==0:old.pointer(AIS+0x98,0xf101)
                    if mutation==2 and op==0:old.pointer(AIS,0xf102)
                    if mutation==3 and op==0:
                        old.pointer(AIS+0xb4,0xf103);old.pointer(AIS+0xa0,0xf104);old.pointer(AIS+0xa4,0xf105)
                        old.pointer(AIS+0xa8,0xf106);old.pointer(AIS+0xac,17);old.uc.mem_write(AIS+0x9c,b'\x03')
                    if mutation==4 and op==0:
                        old.pointer(AIS+0xb8,0xf107);old.pointer(AIS+0xbc,0xf108);old.pointer(AIS+0xc0,0xf109)
                    if mutation==5 and op==1:
                        old.pointer(AIS+0x98,0xf110);old.pointer(AIS+0xb4,0xf111);old.pointer(AIS+0xa0,0xf112);old.pointer(AIS+0xac,19)
                    if mutation==6 and op==1:
                        old.pointer(AIS,0xf113);old.pointer(AIS+0xb8,0xf114);old.pointer(AIS+0xbc,0xf115);old.pointer(AIS+0xc0,0xf116)
                    if mutation==7 and op==2:old.pointer(AIS+0x98,0xf117)
                    if mutation==8 and op==2:old.pointer(AIS+0xb8,0xf118);old.pointer(AIS+0xb4,0xf119)
                    if mutation==9 and op==3:old.pointer(AIS+0x98,0xf120);old.pointer(AIS+0xc0,0xf121)
                def observe(_,at,__,___):
                    if at==0x37c674:
                        assert old.reg(0)==AIS and old.reg(1)==skip
                        calls.append([0,AIS,AIS+0x10,skip,snapshot(),'']);mutate(0);returned(0xfeed)
                    elif at==0x3d8ec8:
                        assert old.reg(0)==AIS and not skip
                        calls.append([1,AIS,AIS+0x10,0,snapshot(),'']);mutate(1);returned(0xfeed)
                    elif at==0x3b56bc:
                        assert old.reg(0)==C and old.reg(1)==AIS+0x10
                        calls.append([2,C,AIS+0x10,0,snapshot(),'']);mutate(2);returned(0xfeed)
                    elif at==0x3109e0:
                        assert old.reg(0)==AIS+0x68 and old.reg(1)==literal and old.reg(2)==literal+16
                        text=bytes(old.uc.mem_read(old.reg(1),old.reg(2)-old.reg(1))).decode('ascii')
                        calls.append([3,AIS+0x68,0,0,snapshot(),text]);mutate(3);path_value[0]=text;returned(0xfeed)
                def store(_,kind,at,size,value,unused):
                    if AIS<=at<AIS+0xc4:writes.append({'pc':hex(old.uc.reg_read(old.pc)),'offset':hex(at-AIS),'size':size,'value':value})
                code=old.uc.hook_add(UC_HOOK_CODE,observe);write=old.uc.hook_add(UC_HOOK_MEM_WRITE,store)
                returned_identity=old.invoke(entry,[AIS,C if mode==2 else skip]);old.uc.hook_del(code);old.uc.hook_del(write)
                if mode<2:assert returned_identity==AIS
                expected={'status':0,'returned':AIS if mode<2 else 0,'state':snapshot(),'path':path_value[0],'calls':calls}
                actual=json.loads(subprocess.check_output([str(exe),str(mode),str(skip),str(mutation),str(char_table),str(external_table)],text=True))
                assert actual==expected,(hex(entry),mode,skip,mutation,actual,expected)
                records.append({'entry':hex(entry),'skip_bind':bool(skip),'mutation':mutation,'matched':True,
                    'source_result':expected,'compiled_result':actual,'original_and_provider_stores':writes})
    # Execute the actual100B binder caller, without substituting its registered
    # names/function/context. Only the concrete Binder resource insertion is modeled.
    reset();registrations=[]
    def binding_observe(_,at,__,___):
        if at==0x31a4d4:
            assert old.reg(0)==AIS+0x10 and old.reg(3)==AIS
            registrations.append({'name':string(old.reg(1)),'function':hex(old.reg(2)),
                'binder':hex(old.reg(0)),'context':hex(old.reg(3))});returned(0xfeed)
    hook=old.uc.hook_add(UC_HOOK_CODE,binding_observe);old.invoke(0x3d8ec8,[AIS]);old.uc.hook_del(hook)
    assert registrations==manifest['state_registrations'],registrations
    # Prove actual selector factory fresh-allocation branch forwards true to the
    # real constructor, installs pending only afterwards. Not a port factory body.
    reset();old.uc.mem_write(AI,bytes(0x100));old.pointer(AI+4,C);factory=[]
    def factory_observe(_,at,__,___):
        if at==0x310570:
            assert old.reg(0)==0xc4 and old.reg(1)==0
            factory.append(['allocate',0xc4,0,get(AI+0x20)]);returned(AIS)
        elif at==0x3dd0e4:
            assert old.reg(0)==AIS and old.reg(1)==1
            factory.append(['external_ctor',AIS,1,get(AI+0x20)])
        elif at==0x37c674:
            assert old.reg(0)==AIS and old.reg(1)==1
            factory.append(['lua_ctor',AIS,1,get(AI+0x20)]);returned(AIS)
        elif at==0x3d8ec8:raise AssertionError('factory must skip constructor state bindings')
    hook=old.uc.hook_add(UC_HOOK_CODE,factory_observe);old.invoke(0x3ccaf4,[AI]);old.uc.hook_del(hook)
    assert get(AI+0x20)==AIS and get(AIS)==external_table and get(AIS+0x98)==0
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
        'executed_state_registrations':registrations,'original_factory_fresh_branch':factory,'original_factory_pending':get(AI+0x20),
        'executed_scope':'Complete original108B CharAIScript ctor aliases and68B AISExternal ctor aliases, plus valid-nonnull192B SetCharacter caller branch. LuaScript240B constructor, CharAIScriptBindFunction100B for comparison, Character createBindings5332B, and string assignment192B are named resource fixture services; their full bodies are not claimed. Separate100B BindFunction registration evidence executes actual caller with modeled Binder insertion; separate240B factory fresh-allocation branch executes actual68B constructor and108B base with only original4B _Znwj15MemoryHintState allocation wrapper and Lua resources modeled. No imported or floating-point helpers are modeled.',
        'native_wired':False}

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/ais-external-initialization/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/ais-external-initialization/validation.json');args=parser.parse_args()
    compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:parser.error('pass --compiler')
    exe=args.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'ais_external_initialization.cpp',MODULE/'tests/ais_external_initialization.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    built=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if built.returncode:raise RuntimeError(built.stdout+built.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==101
    comparison=oracle(args.original_elf.resolve(),exe);assert comparison['comparisons']==90 and comparison['mismatches']==0
    paths=sources+[MODULE/'ais_external_initialization.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,
        'source_sha256':{p.relative_to(ROOT).as_posix():digest(p) for p in paths},'native_wired':False,'whole_original_vm_parity':False}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
