"""Original ARM comparison for live melee-range and melee-radius callers."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-melee-range/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
A,B,P,R=0x10014000,0x10018000,0x10020000,0x10028000
AI=A+0x3c8;TABLE,NEXT,VARIABLE,VT,TYPE=0x10030000,0x10031000,0x10032000,0x10034000,0x10035000
def word(x):
    try:return struct.unpack('<I',struct.pack('<f',x))[0]
    except OverflowError:return 0xff800000 if x<0 else 0x7f800000
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def nan(x):return x&0x7f800000==0x7f800000 and x&0x7fffff!=0
def fixtures():
    rows=[];base=[1,0,8,1,1,word(3),word(2),word(2),0,1,0,7]
    def add(name,changes=None):
        x=base.copy()
        for i,v in (changes or {}).items():x[i]=v
        rows.append((name,x))
    add('resolved_character_strict_inside');add('strict_boundary',{6:word(1.5),7:word(1.5)})
    add('strict_outside',{5:word(5)});add('null_fallback_present',{3:0});add('null_fallback_absent',{3:0,4:0})
    add('unresolved_uses_interaction',{0:0});add('noncharacter_word_f4_uses_interaction',{1:1})
    add('noninteraction8_uses_interaction',{2:7});add('fallback_raw_result',{0:0,11:0xffffffff})
    add('debug_second_executed',{8:7,9:0xffffffff})
    for name,v in [('plus_zero',0),('minus_zero',0x80000000),('negative',word(-3)),('nan',0x7fc00001),('infinity',0x7f800000),('max',0x7f7fffff)]:
        add('position_'+name,{5:v});add('owner_radius_'+name,{6:v});add('target_radius_'+name,{7:v})
    for i in range(1,8):add('live_mutation_'+str(i),{10:i,8:1})
    add('three_axis_binary32_order',{5:word(1),10:9})
    return rows
def radius_fixtures():
    rows=[]
    for name,value in [('zero',0),('one',1),('minus_one',0xffffffff),('int_min',0x80000000),('int_max',0x7fffffff),('round_boundary',16777217)]:
        rows.append(('equipment_'+name,[value,word(2),8,0]))
    for name,value in [('plus_zero',0),('minus_zero',0x80000000),('negative',word(-3)),('nan',0x7fc00001),('infinity',0x7f800000)]:rows.append(('row_'+name,[1,value,8,0]))
    for i in range(1,6):rows.append(('radius_live_mutation_'+str(i),[1,word(2),8,i]))
    return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    m=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        e=ELFFile(f);syms={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()};loads=[p for p in e.iter_segments() if p['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[off:off+n]
        for row in m['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=syms[row['original_symbol']]
            assert (int(s['st_value']),int(s['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        got=0x3d6198+8+struct.unpack('<I',data(0x3d63c4,4))[0]
        radius_got=0x3d4c60+8+struct.unpack('<I',data(0x3d4c94,4))[0]
        table_offset=struct.unpack('<I',data(0x3d4c98,4))[0]
    old=Cpu(original,False,m);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(x=0):old.put(0,x);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def soft(at,trace):
        a,b=old.reg(0),old.reg(1)
        if at==0x30e3ac:op='sub';v=word(number(a)-number(b))
        elif at==0x30ed6c:op='mul';v=word(number(a)*number(b))
        elif at==0x30eba4:op='add';v=word(number(a)+number(b))
        elif at==0x30e964:op='i2f';v=word(a if a<0x80000000 else a-0x100000000)
        else:op='gt';v=int(number(a)>number(b))
        trace.append([op,a,b,v]);returned(v)
    records=[]
    for name,x in fixtures():
        resolving,f4,interaction,explicit,present,position,rad_a,rad_b,debug,debug2,mutation,generic=x
        old.uc.mem_write(old.stack,bytes(0x10000));old.pointer(AI+4,A);old.pointer(AI+0x40,P if present else 0)
        old.pointer(R+0xf4,f4);old.pointer(R,VT);old.pointer(VT+0x90,TYPE)
        for actor in (A,B,P):
            old.pointer(actor+0x180,0);old.uc.mem_write(actor+0x80,b'\0')
            old.uc.mem_write(actor+0x160,struct.pack('<III',position if actor==P else 0,
                word(2) if actor==P and mutation==9 else 0,word(3) if actor==P and mutation==9 else 0))
        calls=[];fp=[];measure={'distance':0,'sum':0,'square':0};diag=[0]
        candidate=P if explicit or present else 0
        def call(op,kind,subject,other=0,key=0):calls.append([op,kind,subject,other,key])
        def observe(_,at,__,___):
            if at==0x33dd70:assert old.reg(1)==candidate;returned(old.reg(0))
            elif at==0x33ff8c:
                call(0,0,candidate)
                if mutation==1:old.pointer(AI+4,B)
                returned(R if resolving else 0)
            elif at==TYPE:
                call(1,0,old.reg(0),old.reg(1))
                if mutation==2:old.pointer(AI+4,B)
                returned(interaction)
            elif at==0x3935dc:
                actor=old.reg(0);call(2,0,actor)
                if actor==P and mutation==3:old.pointer(A+0x160,word(1))
                if mutation==6:old.pointer(AI+0x40,0)
                # Execute the actual GetTargetPosition leaf after observation.
            elif at==0x3d4c34:
                original_ai=old.reg(0)==AI;call(3,1 if original_ai else 2,AI if original_ai else R)
                if original_ai and mutation==4:
                    old.pointer(A+0x160,word(100));old.pointer(P+0x160,word(100))
                if mutation==5:old.pointer(AI+4,B)
                returned(rad_a if original_ai else rad_b)
            elif at in (0x337888,0x3140ec,0x318254):returned()
            elif at==0x337a88:
                diag[0]+=1;call(4,3,0,0,diag[0])
                if mutation==7:old.pointer(P+0x160,word(100))
                returned(debug if diag[0]==1 else debug2)
            elif at==0x3d4f98:call(5,1,old.reg(0),old.reg(1));returned(generic)
            elif at in (0x30e3ac,0x30ed6c,0x30eba4,0x30e2f8):soft(at,fp)
            elif at==0x3d62b0:measure['distance']=old.reg(0)
            elif at==0x3d62d4:measure['sum']=old.reg(0)
            elif at==0x3d634c:measure['square']=old.reg(0)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        value=old.invoke(0x3d6188,[AI,P if explicit else 0]);old.uc.hook_del(hook)
        expected={'status':0,'value':value,**measure,'owner':get(AI+4),'target40':get(AI+0x40),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),'melee',*map(str,x)],text=True))
        for key in measure:assert actual[key]==expected[key] or nan(actual[key]) and nan(expected[key]),(name,key,actual,expected)
        assert {k:v for k,v in actual.items() if k not in measure}=={k:v for k,v in expected.items() if k not in measure},(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'modeled_soft_float_trace':fp})
    radius_records=[]
    for name,x in radius_fixtures():
        equipment,row,id_value,mutation=x
        old.uc.mem_write(old.stack,bytes(0x10000));old.pointer(AI+4,A)
        old.pointer(radius_got+table_offset,VARIABLE);old.pointer(VARIABLE,TABLE)
        old.pointer(TABLE+8*0x44+0x20,row);old.pointer(NEXT+8*0x44+0x20,word(100))
        calls=[];fp=[]
        def observe(_,at,__,___):
            if at==0x3fff30:
                calls.append([0,0,old.reg(0)-0x37c,0,0]);assert get(old.reg(1))==0;old.pointer(old.reg(1),equipment)
                if mutation==1:old.pointer(AI+4,B)
                returned(7) # source intentionally ignores the bool return
            elif at==0x3d4c6c:
                calls.append([1,0,0,0,0])
                if mutation==2:old.pointer(AI+4,B)
            elif at==0x3a2fec:
                calls.append([2,0,old.reg(0),0,0])
                if mutation==3:old.pointer(VARIABLE,NEXT)
                if mutation==4:old.pointer(TABLE+id_value*0x44+0x20,word(7))
                if mutation==5:old.pointer(AI+4,B)
                returned(id_value)
            elif at in (0x30e964,0x30eba4):soft(at,fp)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe);value=old.invoke(0x3d4c34,[AI]);old.uc.hook_del(hook)
        expected={'status':0,'value':value,'owner':get(AI+4),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),'radius',*map(str,x)],text=True))
        assert actual['value']==expected['value'] or nan(actual['value']) and nan(expected['value']),(name,actual,expected)
        assert {k:v for k,v in actual.items() if k!='value'}=={k:v for k,v in expected.items() if k!='value'},(name,actual,expected)
        radius_records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'modeled_soft_float_trace':fp})
    return {'validation':'PASS','melee_comparisons':len(records),'radius_comparisons':len(radius_records),'comparisons':len(records)+len(radius_records),'mismatches':0,'results':records,'radius_results':radius_records,
      'executed_scope':'Original 592B melee caller and 104B radius caller; original GetTargetPosition leaf executes. Const handle/GetObject, resolved InteractionType, melee-radius calls within melee caller, Debug load/string/query/cleanup and InteractionRange fallback are explicit providers. Radius caller models Inventory CanMeleeAttack and GetCharAIId while actual table capture/row arithmetic executes.',
      'external_soft_float':'External fsub/fmul/fadd/fcmpgt/i2f imports are modeled with separately rounded IEEE binary32 operations; finite words match exactly and NaN outputs compare unordered class, not payload/sign.',
      'native_wired':False}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-ai-melee-range/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-ai-melee-range/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_ai_melee_range.cpp',MODULE/'tests/character_ai_melee_range.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-ffp-contract=off',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if build.returncode:raise RuntimeError(build.stdout+build.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==40
    comparison=oracle(a.original_elf.resolve(),exe)
    paths=sources+[MODULE/'character_ai_melee_range.hpp',MODULE/'character_ai_sight.hpp',MODULE/'character_enemy_retention.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,'native_wired':False,
      'source_sha256':{x.relative_to(ROOT).as_posix():hashlib.sha256(x.read_bytes()).hexdigest() for x in paths}}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'melee_cases':comparison['melee_comparisons'],'radius_cases':comparison['radius_comparisons'],'mismatches':0}))
if __name__=='__main__':main()
