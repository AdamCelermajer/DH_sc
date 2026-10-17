"""Character IsInteractive full caller vs original ARM, with named dependencies."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-interactive/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
C,P=0x10014000,0x10020000;AI=C+0x3c8;ROW=0x10030000
def fixtures():
    rows=[];base=[0x2380,0,1,1,0,0,4,1,0,0,0]
    def add(name,changes=None):
        x=base.copy()
        for key,value in (changes or {}).items():x[key]=value
        rows.append((name,x))
    add('normal');add('null_interactor',{7:0})
    for raw in (0,1,7,255):add('raw_final_byte_'+str(raw),{3:raw})
    for flags in (0,0x100,0x241,0x2000,0x2380,0x23c1,0xffffffff):add('state_flags_'+str(flags),{0:flags})
    for type_value in (0,1,2,3,4,5,6,7,8,9,0xffffffff):
        add('alive_type_'+str(type_value),{6:type_value})
        add('dead_friend_type_'+str(type_value),{0:0,1:1,2:0,4:7,5:0xffffffff,6:type_value})
        add('dead_friend_null_interactor_'+str(type_value),{4:7,5:0xffffffff,6:type_value,7:0})
    for raw in (1,7,255):add('deleted_'+str(raw),{1:raw})
    for raw in (0,7,255):add('enabled_'+str(raw),{2:raw})
    for raw in (1,7,255):add('dead_nonfriend_'+str(raw),{4:raw})
    for mutation in range(1,14):
        add('alive_mutation_'+str(mutation),{8:mutation})
        add('dead_friend_monster_mutation_'+str(mutation),{4:1,5:7,8:mutation})
        add('dead_friend_nonmonster_mutation_'+str(mutation),{4:1,5:7,6:1,8:mutation})
    return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            off=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[off:off+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['vtable_ranges']:
            at,n=int(row['elf_address'],0),row['size'];assert int(symbols[row['symbol']]['st_value'])==at
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
            for slot in row['slots']:
                assert struct.unpack('<I',data(int(row['address_point'],0)+int(slot['byte_offset'],0),4))[0]==int(slot['target'],0)
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def byte(at):return old.uc.mem_read(at,1)[0]
    def put_byte(at,value):old.uc.mem_write(at,bytes([value]))
    def returned(value):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[]
    for name,x in fixtures():
        flags,deleted,enabled,interactive,dead,friend_value,type_value,explicit,mutation,_,_=x
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(C,bytes(0x1800))
        old.pointer(C,0x965f38);old.pointer(C+0x520,flags);put_byte(C+0x81,deleted);put_byte(C+0x8a,enabled)
        put_byte(C+0x415,interactive);put_byte(C+0x1449,dead);old.pointer(ROW+0x38,type_value)
        calls=[];dependency=[]
        def mutate(query):
            if mutation==1 and len(calls)==1:put_byte(C+0x1449,int(not byte(C+0x1449)))
            if mutation==2 and query==1:put_byte(C+0x81,1)
            if mutation==3 and query==2:put_byte(C+0x8a,0)
            if mutation==4 and query==3:put_byte(C+0x1449,1)
            if mutation==5 and query==4:put_byte(C+0x1449,1)
            if mutation==6 and query==0 and len(calls)>1:old.pointer(C+0x520,0)
            if mutation==7 and query==0 and len(calls)>1:put_byte(C+0x415,255)
            if mutation==8 and query==3:old.pointer(C+0x520,0);put_byte(C+0x415,17)
            if mutation==9 and query==1:put_byte(C+0x8a,0);old.pointer(C+0x520,0)
            if mutation==10 and len(calls)==1:put_byte(C+0x8a,0)
            if mutation==11 and query==1:old.pointer(ROW+0x38,1)
            if mutation==12 and query==3:old.pointer(ROW+0x38,5)
            if mutation==13 and query==4:old.pointer(C+0x520,0x2000)
        starts={0x3a2ed4:0,0x3a3064:2,0x3a3094:3,0x3a30ac:4}
        ends={0x3a2edc:0,0x3a3078:2,0x3a30a8:3,0x3a30c0:4}
        def observe(_,at,__,___):
            if at in starts:
                assert old.reg(0)==C;calls.append([starts[at],C,0,0])
            elif at in ends:
                query=ends[at];assert calls[-1][0]==query;calls[-1][3]=old.reg(0);mutate(query)
            elif at==0x3d511c:
                assert old.reg(0)==AI and old.reg(1)==P and explicit
                calls.append([1,AI,P,friend_value]);mutate(1);returned(friend_value)
            elif at==0x3a3024:
                assert old.reg(0)==C;dependency.append(['GetCharAI',C,ROW,get(ROW+0x38)]);returned(ROW)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        value=old.invoke(0x3a4870,[C,P if explicit else 0]);old.uc.hook_del(hook)
        expected={'status':0,'value':value,'flags':get(C+0x520),'deleted':byte(C+0x81),'enabled':byte(C+0x8a),
            'interactive':byte(C+0x415),'dead':byte(C+0x1449),'type':get(ROW+0x38),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True))
        assert actual==expected,(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'property_provider_trace':dependency})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
        'executed_scope':'Complete original192B IsInteractive caller, actual12B Character::IsDead, actual24B IsMonster/IsFaerie/IsSummoned and16B GetCharType instructions. Actual Character vtable+0x34 dispatches IsDead. AI_IsFriend720B is a named borrowed predicate fixture; GetCharAI48B is a named fresh AIProps provider. No classifier/dead result is directly mocked.',
        'native_wired':False}
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-interactive/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/character-interactive/validation.json');args=parser.parse_args()
    compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:parser.error('pass --compiler')
    exe=args.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_interactive.cpp',MODULE/'tests/character_interactive.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    built=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if built.returncode:raise RuntimeError(built.stdout+built.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==73
    comparison=oracle(args.original_elf.resolve(),exe);assert comparison['comparisons']==94 and comparison['mismatches']==0
    paths=sources+[MODULE/'character_interactive.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,'native_wired':False,
        'source_sha256':{p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
