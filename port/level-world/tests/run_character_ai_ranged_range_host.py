"""Original ARM comparison for live CloseRange and Range caller orchestrations."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-ranged-range/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
A,B,P,R=0x10014000,0x10018000,0x10020000,0x10028000
AI=A+0x3c8;VT,TYPE,GATE,ENTRIES,ENTRY,ITEM,LOOT,D,N=0x10034000,0x10035000,0x10036000,0x10030000,0x10030100,0x10030200,0x10031000,0x10038000,0x10039000
def word(x):
    try:return struct.unpack('<I',struct.pack('<f',x))[0]
    except OverflowError:return 0xff800000 if x<0 else 0x7f800000
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def nan(x):return x&0x7f800000==0x7f800000 and x&0x7fffff!=0
def fixtures(mode):
    rows=[];base=[1,0,8,1,1,1,512,1280,4,word(3),0,1,0,0]
    def add(name,changes=None):
        x=base.copy()
        for i,v in (changes or {}).items():x[i]=v
        rows.append((name,x))
    add('normal');add('lower_equality_boundary',{6:768});add('inclusive_upper_boundary',{7:768})
    add('close_inside',{6:1024});add('ranged_above_upper',{9:word(6)})
    add('null_fallback_present',{3:0});add('null_fallback_absent',{3:0,4:0})
    add('capability_false_skips_outputs',{5:0});add('debug_second',{10:7,11:0xffffffff})
    add('range_does_not_resolve_handle',{0:0,1:1,2:7})
    if mode=='close':
        add('unresolved_interaction',{0:0});add('f4_interaction',{1:1});add('type7_interaction',{2:7})
    for name,v in [('pluszero',0),('minuszero',0x80000000),('negative',word(-3)),('nan',0x7fc00001),('inf',0x7f800000),('max',0x7f7fffff)]:add('position_'+name,{9:v})
    for name,v in [('zero',0),('negative',0xffffff00),('asr_negative_fraction',0xffffff01),('asr_positive_fraction',0x000002ff),('46341_square_sign',46341<<8),('65536_square_zero',65536<<8),('8388607_square',0x7fffffff)]:
        add('lower_'+name,{6:v});add('upper_'+name,{7:v})
    for name,v in [('int_min',0x80000000),('int_max',0x7fffffff),('minus_one',0xffffffff),('46341',46341),('65536',65536),('round',4097)]:
        add('inventory_lower_'+name,{6:v,7:5,13:1});add('inventory_upper_'+name,{6:2,7:v,13:1})
    add('inventory_argument_alias',{6:2,7:5,8:0xfffffffd,13:1})
    for i in range(1,14):add('live_mutation_'+str(i),{12:i,10:1})
    add('borrowed_same_point',{12:17})
    return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        e=ELFFile(f);syms={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()};loads=[p for p in e.iter_segments() if p['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[off:off+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=syms[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['vtable_ranges']:
            at,n=int(row['elf_address'],0),row['size'];assert int(syms[row['symbol']]['st_value'])==at
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
            for slot in row['slots']:assert struct.unpack('<I',data(int(row['address_point'],0)+int(slot['byte_offset'],0),4))[0]==int(slot['target'],0)
        got=0x3d63e8+8+struct.unpack('<I',data(0x3d65f0,4))[0]
        assert got==0x3d6614+8+struct.unpack('<I',data(0x3d67e0,4))[0]
        debug_offset=struct.unpack('<I',data(0x3d65f8,4))[0]
        for key,at in [(1,0x8c56e0),(2,0x8c56f8)]:assert data(at,30).split(b'\0')[0].decode()==manifest['diagnostic_keys'][str(key)]
    from elf_import_identity import verify_imports
    imported=verify_imports(original,{int(row['plt_address'],0):row['imported_symbol'] for row in manifest['modeled_imports']})
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(x=0):old.put(0,x);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[]
    for mode in ('close','ranged'):
      for name,x in fixtures(mode):
        resolving,f4,interaction,explicit,present,capability,minimum,maximum,projectile,position,debug,debug2,mutation,inventory=x
        old.uc.mem_write(old.stack,bytes(0x10000));old.pointer(AI+4,A);old.pointer(AI+0x40,P if present else 0)
        old.pointer(R+0xf4,f4);old.pointer(R,VT);old.pointer(VT+0x90,TYPE);old.pointer(VT+0x128,0x3a4cd0)
        for actor in (A,B,P):
            old.pointer(actor,VT);old.pointer(actor+0x180,0);old.uc.mem_write(actor+0x80,b'\0')
            old.uc.mem_write(actor+0x160,struct.pack('<III',position if actor==P else 0,
                word(2) if actor==P and mutation==13 else 0,word(3) if actor==P and mutation==13 else 0))
            old.pointer(actor+0x1070,minimum);old.pointer(actor+0x1074,maximum);old.pointer(actor+0x1078,0xffffffff if inventory or not capability else projectile)
            old.pointer(actor+0x37c,VT);old.pointer(actor+0x37c+0x14,ENTRIES)
        old.pointer(VT+8,GATE);old.pointer(ENTRIES,ENTRY);old.pointer(ENTRY+4,ITEM);old.pointer(ITEM,0)
        old.pointer(LOOT+0x98,minimum);old.pointer(LOOT+0x9c,maximum);old.pointer(LOOT+0xa0,projectile)
        old.pointer(got+debug_offset,D)
        calls=[];fp=[];dependencies=[];output=[0,0,0];diag=[0];global_debug=[D]
        measure={k:0 for k in ('distance','minimum','maximum','projectile','minimum_square','maximum_square','minimum_limit','maximum_limit')}
        candidate=P if explicit or present else 0
        def call(op,subject,other=0,key=0,alias=0):calls.append([op,subject,other,key,alias])
        def soft(at):
            a,b=old.reg(0),old.reg(1)
            if at==0x30e3ac:op='sub';v=word(number(a)-number(b))
            elif at==0x30ed6c:op='mul';v=word(number(a)*number(b))
            elif at==0x30eba4:op='add';v=word(number(a)+number(b))
            elif at==0x30e964:op='i2f';v=word(a if a<0x80000000 else a-0x100000000)
            elif at==0x30e2f8:op='gt';v=int(number(a)>number(b))
            elif at==0x30e9ac:op='le';v=int(number(a)<=number(b))
            else:op='ge';v=int(number(a)>=number(b))
            fp.append([op,a,b,v]);returned(v)
        def observe(_,at,__,___):
            if at==0x33dd70:assert old.reg(1)==candidate;returned(old.reg(0))
            elif at==0x33ff8c:
                call(0,candidate)
                if mutation==1:old.pointer(AI+4,B)
                returned(R if resolving else 0)
            elif at==TYPE:
                call(1,old.reg(0),old.reg(1))
                if mutation==2:old.pointer(AI+4,B)
                returned(interaction)
            elif at==0x3a4cd0:
                output[:]=[old.reg(1),old.reg(2),old.reg(3)]
                assert output[0]!=output[1] and (output[1]==output[2])==(mode=='close')
                call(2,old.reg(0),alias=int(output[1]==output[2]))
                if mutation==3:old.pointer(AI+4,B)
                # Execute actual100B Character overload, including property ASR8
                # and tail inventory116B overload, preserving aliased writes.
            elif at==GATE:dependencies.append(['inventory_virtual8',old.reg(0),capability]);returned(capability)
            elif at==0x3fc6a8:assert old.reg(1)==1;dependencies.append(['slot1',old.reg(0)]);returned(0)
            elif at==0x3f9e08:dependencies.append(['item0',old.reg(0)]);returned(LOOT)
            elif at==0x3935dc:
                actor=old.reg(0);call(3,actor)
                if actor!=P and mutation==4:old.pointer(AI+4,B)
                if actor==P and mutation==5:old.pointer(A+0x160,word(1))
                if mutation==6:old.pointer(AI+0x40,0)
                if actor==P and mutation==17:returned(A+0x160)
                # Otherwise actual36B TargetPosition executes.
            elif at==0x337888:
                dependencies.append(['debug_load',old.reg(0)])
                if diag[0]:assert old.reg(0)==D # captured object, not replaced GOT
                returned()
            elif at==0x3140ec:
                assert old.reg(1)==(0x8c56e0 if not diag[0] else 0x8c56f8)
                dependencies.append(['string',old.reg(1)]);returned()
            elif at==0x318254:dependencies.append(['string_destroy',old.reg(0)]);returned()
            elif at==0x337a88:
                diag[0]+=1;assert old.reg(0)==D;call(4,0 if diag[0]==1 else D,key=diag[0])
                if mutation==7:old.pointer(P+0x160,word(100));old.pointer(A+0x160,word(100))
                if mutation==8:old.pointer(AI+4,B)
                if mutation==9:
                    for actor in (A,B):old.pointer(actor+0x1070,0);old.pointer(actor+0x1074,0)
                if mutation==10:old.pointer(got+debug_offset,N);global_debug[0]=N
                if mutation==11:old.pointer(output[0],4)
                if mutation==12:old.pointer(output[1],2)
                returned(debug if diag[0]==1 else debug2)
            elif at==0x3d4f98:call(5,old.reg(0),old.reg(1));returned(7)
            elif at in (0x30e3ac,0x30ed6c,0x30eba4,0x30e964,0x30e2f8,0x30e9ac,0x30e4b4):
                if at==0x30e964:
                    tag='minimum' if not fp or not any(row[0]=='i2f' for row in fp) else 'maximum'
                    measure[tag+'_square']=old.reg(0)
                    soft(at);measure[tag+'_limit']=old.reg(0)
                else:soft(at)
            elif at in (0x3d652c,0x3d6704):measure['distance']=old.reg(0)
            elif at in (0x3d6578,0x3d6750):
                measure['minimum']=get(output[0]);measure['maximum']=get(output[1]);measure['projectile']=get(output[2])
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        value=old.invoke(0x3d63d8 if mode=='close' else 0x3d6604,[AI,P if explicit else 0]);old.uc.hook_del(hook)
        expected={'status':0,'value':value,**measure,'owner':get(AI+4),'target40':get(AI+0x40),'debug_global':global_debug[0],'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),mode,*map(str,x)],text=True))
        float_keys=('distance','minimum_limit','maximum_limit')
        for key in float_keys:assert actual[key]==expected[key] or nan(actual[key]) and nan(expected[key]),(mode,name,key,actual,expected)
        assert {k:v for k,v in actual.items() if k not in float_keys}=={k:v for k,v in expected.items() if k not in float_keys},(mode,name,actual,expected)
        records.append({'mode':mode,'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'modeled_soft_float_trace':fp,'original_dependency_trace':dependencies})
    return {'validation':'PASS','comparisons':len(records),'close_comparisons':sum(r['mode']=='close' for r in records),'range_comparisons':sum(r['mode']=='ranged' for r in records),'mismatches':0,'verified_imports':imported,'results':records,
        'executed_scope':'Complete original556B CloseRange/496B Range, actual100B Character CanRangeAttack overload and116B inventory overload, and actual36B TargetPosition (one explicit same-point fixture models this provider). Const handle/GetObject(false), InteractionType, inventory virtual8/slot1/ItemProps lookup, debug Load/string/query/destruction and InteractionRange remain named fixture providers.',
        'external_soft_float':'External fsub/fmul/fadd/i2f/fcmpgt/fcmple/fcmpge imports modeled separately rounded IEEE binary32, after independent actual relocated PLT identity execution. MUL/ASR and output pointer alias writes execute original ARM. Finite words match; NaN compares unordered class without payload claim.',
        'native_wired':False}
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-ai-ranged-range/host.exe');parser.add_argument('--report',type=Path,default=MODULE/'build/character-ai-ranged-range/validation.json');args=parser.parse_args()
    compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:parser.error('pass --compiler')
    exe=args.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_ai_ranged_range.cpp',MODULE/'character_ai_interaction_range.cpp',MODULE/'tests/character_ai_ranged_range.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-ffp-contract=off',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if build.returncode:raise RuntimeError(build.stdout+build.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==73
    comparison=oracle(args.original_elf.resolve(),exe)
    paths=sources+[MODULE/'character_ai_ranged_range.hpp',MODULE/'character_ai_interaction_range.hpp',MODULE/'character_ai_melee_range.hpp',MODULE/'character_ai_sight.hpp',MODULE/'character_enemy_retention.hpp',MODULE/'tests/elf_import_identity.py',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,'native_wired':False,
        'source_sha256':{x.relative_to(ROOT).as_posix():hashlib.sha256(x.read_bytes()).hexdigest() for x in paths}}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'close_cases':comparison['close_comparisons'],'range_cases':comparison['range_comparisons'],'mismatches':0}))
if __name__=='__main__':main()
