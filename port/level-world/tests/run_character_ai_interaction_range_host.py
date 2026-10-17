"""Compare the maintained interaction-range caller with original ARM instructions."""
from __future__ import annotations
import argparse,hashlib,json,math,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-ai-interaction-range/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
A,B,P=0x10014000,0x10018000,0x10020000
AI=A+0x3c8;NODE,VISUAL,VT,TYPE,RADIUS,TABLE,VARIABLE,COUNT=0x10028000,0x10030000,0x10032000,0x10033000,0x10033010,0x10034000,0x10035000,0x10035004
def word(x):
    try:return struct.unpack('<I',struct.pack('<f',x))[0]
    except OverflowError:return 0xff800000 if x<0 else 0x7f800000
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def nan(x):return x&0x7f800000==0x7f800000 and x&0x7fffff!=0
def fixtures():
    rows=[];base=[1,1,0,1,0,0,0xffffffff,0,0,0,word(3),0,0,word(2),word(2),word(1),0]
    def add(name,changes=None):
        x=base.copy()
        for i,v in (changes or {}).items():x[i]=v
        rows.append((name,x))
    add('generic_inside');add('generic_inclusive_boundary',{10:word(5)})
    add('generic_outside',{10:word(6)});add('generic_just_inside',{10:word(4.9999995)})
    add('type8_inside',{6:8});add('type8_boundary',{6:8,10:word(4)})
    add('type8_outside',{6:8,10:word(5)});add('type8_ignores_property',{6:8,15:0x7fc00001})
    add('node_cached_inside',{2:1,10:word(79)});add('node_cached_boundary',{2:1,10:word(80)})
    add('node_cached_outside',{2:1,10:word(81)});add('node_type8_coincident_true',{2:1,6:8,10:0})
    add('node_type8_no_radius_property_queries',{2:1,6:8,13:0x7fc00001,14:0x7fc00001,15:0x7fc00001})
    add('lazy_visual_lookup_hit',{3:0,4:1,5:1});add('lazy_visual_lookup_miss',{3:0,4:1,5:0})
    add('lazy_no_visual_clears_old_node',{2:1,3:0});add('cached_absence_skips_visual_lookup',{4:1,5:1})
    add('null_uses_current',{0:0});add('null_no_current_no_queries',{0:0,1:0})
    add('three_axis_distance',{10:word(1),11:word(2),12:word(3)})
    add('three_axis_rounding',{7:0x3f800001,8:0x47800001,9:0xb7800001,10:0x3f7fffff,11:0x47800000,12:0x37800000})
    add('both_infinite_coords_nan',{7:0x7f800000,10:0x7f800000})
    add('square_overflow',{10:word(1e20),15:0x7f800000})
    add('subnormal_square_underflow',{7:1,10:2,11:0,12:0,13:0,14:0,15:1})
    for field,label in [(10,'position'),(13,'owner_radius'),(14,'target_radius'),(15,'property')]:
        for name,v in [('pluszero',0),('minuszero',0x80000000),('negative',word(-3)),('nan',0x7fc00001),('positive_inf',0x7f800000),('negative_inf',0xff800000),('max',0x7f7fffff)]:
            add(label+'_'+name,{field:v})
    for i in range(1,11):add('live_callback_mutation_'+str(i),{16:i})
    add('point_mutation_node_branch',{2:1,16:2})
    return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[off:off+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];s=symbols[row['original_symbol']]
            assert (int(s['st_value']),int(s['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['vtable_ranges']:
            at,n=int(row['elf_address'],0),row['size'];s=symbols[row['symbol']]
            assert at==int(s['st_value']) and n<=int(s['st_size'])
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
            for slot in row['slots']:
                assert struct.unpack('<I',data(at+8+int(slot['byte_offset'],0),4))[0]==int(slot['target'],0)
                assert int(symbols[slot['symbol']]['st_value'])==int(slot['target'],0)
        got=(0x3a3030+8+struct.unpack('<I',data(0x3a304c,4))[0])&0xffffffff
        table_offset=struct.unpack('<I',data(0x3a3050,4))[0]
        id_got=(0x3a2ff8+8+struct.unpack('<I',data(0x3a301c,4))[0])&0xffffffff
        count_offset=struct.unpack('<I',data(0x3a3020,4))[0]
        node_name=0x38b260+8+struct.unpack('<I',data(0x38b2c8,4))[0]
        assert data(node_name,21).split(b'\0')[0]==b'interaction_position'
    from elf_import_identity import verify_imports
    imported=verify_imports(original,{0x30e3ac:'__aeabi_fsub',0x30ed6c:'__aeabi_fmul',
        0x30eba4:'__aeabi_fadd',0x30e124:'sqrtf',0x30e9ac:'__aeabi_fcmple'})
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(x=0):old.put(0,x);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[]
    for name,x in fixtures():
        explicit,present,node,cache,visual,lookup,interaction=x[:7];mutation=x[16]
        old.uc.mem_write(old.stack,bytes(0x10000));old.pointer(AI+4,A);old.pointer(AI+0x40,P if present else 0)
        old.pointer(P,VT);old.pointer(VT+0x90,TYPE);old.pointer(VT+0x94,RADIUS)
        old.pointer(P+0x2e8,NODE if node else 0);old.uc.mem_write(P+0x2ec,bytes([cache&255]));old.pointer(P+0x2d8,VISUAL if visual else 0)
        for actor,index in ((A,0),(B,1),(P,0)):
            old.pointer(actor+0x180,0);old.uc.mem_write(actor+0x80,b'\0');old.pointer(actor+0xffc,index)
            old.uc.mem_write(actor+0x160,struct.pack('<III',*(x[10:13] if actor==P else x[7:10])))
        old.uc.mem_write(NODE+0x54,struct.pack('<III',*x[10:13]))
        old.pointer(TABLE+0x18,x[15]);old.pointer(TABLE+0x44+0x18,word(number(x[15])+4))
        old.pointer(VARIABLE,TABLE);old.pointer(COUNT,16);old.pointer(got+table_offset,VARIABLE);old.pointer(id_got+count_offset,COUNT)
        calls=[];fp=[];dependencies=[];measure={'squared':0,'distance':0,'remaining':0,'threshold':0,'node_present':0,'type':0}
        def call(op,subject,other=0):calls.append([op,subject,other])
        def soft(at):
            a,b=old.reg(0),old.reg(1)
            if at==0x30e3ac:op='sub';value=word(number(a)-number(b))
            elif at==0x30ed6c:op='mul';value=word(number(a)*number(b))
            elif at==0x30eba4:op='add';value=word(number(a)+number(b))
            elif at==0x30e124:op='sqrtf';value=word(math.sqrt(number(a)))
            else:op='le';value=int(number(a)<=number(b))
            fp.append([op,a,b,value]);returned(value)
        def observe(_,at,__,___):
            if at==0x3935dc:
                if old.uc.reg_read(old.lr)==0x3d4fb4:
                    call(0,old.reg(0))
                    if mutation==1:old.pointer(AI+4,B)
                else:dependencies.append(['target_position_in_spot',old.reg(0)])
            elif at==0x38b228:
                call(1,old.reg(1))
                if mutation==2:old.pointer(A+0x160,word(1))
                if mutation==3:old.pointer(AI+0x40,0)
            elif at==0x470a18:
                assert old.reg(0)==VISUAL and old.reg(1)==node_name
                assert old.uc.mem_read(P+0x2ec,1)==b'\1'
                dependencies.append(['visual_specific_node','interaction_position',lookup]);returned(NODE if lookup else 0)
            elif at==0x597180:
                dependencies.append(['absolute_position',old.reg(1)]) # actual28B body executes
            elif at==0x3d4c34:
                call(2,old.reg(0))
                if mutation==4:
                    old.pointer(A+0x160,word(100));old.pointer(P+0x160,word(100));old.pointer(NODE+0x54,word(100))
                if mutation==5:old.pointer(AI+4,B)
                if mutation==10:old.pointer(P+0x2e8,NODE)
                returned(x[13])
            elif at==RADIUS:
                call(3,old.reg(0))
                if mutation==6:old.pointer(AI+4,B)
                returned(x[14])
            elif at==0x3a3024:
                call(4,old.reg(0))
                if mutation==7:old.pointer(AI+4,B)
                if mutation==8:old.pointer(TABLE+0x18,word(9))
                # actual48B GetCharAI +56B GetCharAIId execute
            elif at==TYPE:
                call(5,old.reg(0),old.reg(1));measure['remaining']=old.reg(5);measure['threshold']=old.reg(7);measure['type']=interaction
                if mutation==9:
                    old.pointer(TABLE+0x18,0);old.pointer(P+0x160,word(100));old.pointer(NODE+0x54,word(100))
                returned(interaction)
            elif at==0x3d5038:measure['squared']=old.reg(0)
            elif at==0x3d503c:measure['distance']=old.reg(0);measure['node_present']=int(get(P+0x2e8)!=0)
            elif at in (0x30e3ac,0x30ed6c,0x30eba4,0x30e124,0x30e9ac):soft(at)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        value=old.invoke(0x3d4f98,[AI,P if explicit else 0]);old.uc.hook_del(hook)
        expected={'status':0,'value':value,**measure,'owner':get(AI+4),'target40':get(AI+0x40),'ai':AI,
                  'cache':old.uc.mem_read(P+0x2ec,1)[0],'node':get(P+0x2e8),'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True))
        for key in ('squared','distance','remaining','threshold'):
            assert actual[key]==expected[key] or nan(actual[key]) and nan(expected[key]),(name,key,actual,expected)
        ignore=('squared','distance','remaining','threshold')
        assert {k:v for k,v in actual.items() if k not in ignore}=={k:v for k,v in expected.items() if k not in ignore},(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'modeled_soft_float_trace':fp,'original_spot_dependency_trace':dependencies})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
        'executed_scope':'Complete original388B interaction-range caller, real164B GetInteractionSpot (including lazy2ec/node2e8 stores),36B GetTargetPosition,28B node absolute-position,48B GetCharAI and56B GetCharAIId. Visual node lookup and original-AI melee/virtual interaction radius/type are explicit fixture providers.',
        'external_soft_float':'External fsub/fmul/fadd/sqrtf/fcmple imports modeled IEEE binary32, after independent actual relocated PLT identity execution; finite outputs compare exact words, NaN compares unordered class without payload/sign claim.',
        'verified_import_identities':imported,
        'native_wired':False}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/character-ai-interaction-range/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-ai-interaction-range/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_ai_interaction_range.cpp',MODULE/'character_ai_melee_range.cpp',MODULE/'tests/character_ai_interaction_range.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-ffp-contract=off',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if build.returncode:raise RuntimeError(build.stdout+build.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==41
    comparison=oracle(a.original_elf.resolve(),exe)
    paths=sources+[MODULE/'character_ai_interaction_range.hpp',MODULE/'character_ai_melee_range.hpp',MODULE/'character_ai_sight.hpp',MODULE/'character_enemy_retention.hpp',MODULE/'tests/elf_import_identity.py',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,'native_wired':False,
      'source_sha256':{x.relative_to(ROOT).as_posix():hashlib.sha256(x.read_bytes()).hexdigest() for x in paths}}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
