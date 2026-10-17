"""Actual cache-owned preparation; original ARM caller/selector/constructor replay."""
from __future__ import annotations
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys

MODULE=Path(__file__).resolve().parents[1]
ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/player-skills-preparation-v3/original-functions.json'
ORIGINAL_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
AI,OWNER,AIS,CALLBACK=0x2010000,0x2020000,0x2030000,0x2032000
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def load_module(path,name):
    spec=importlib.util.spec_from_file_location(name,path)
    result=importlib.util.module_from_spec(spec);spec.loader.exec_module(result);return result

def original_compare(original,host,skills,faeries,faery_count):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
    from cpu import Cpu
    sys.path.insert(0,str(MODULE/'tests'))
    from elf_import_identity import verify_imports
    verified=verify_imports(original,{0x30e964:'__aeabi_i2f'})
    manifest=json.loads(MANIFEST.read_text(encoding='utf-8'))
    old=load_module(MODULE/'tests/run_character_ai_set_skills_and_spells_host.py','skills_caller_evidence')
    old.source_symbols(original,manifest)
    old_external=Cpu.external
    def external(cpu,uc,address,size,user):
        if cpu.imports.get(address)=='__aeabi_i2f':
            word=cpu.reg(0);signed=word if word<0x80000000 else word-0x100000000
            cpu.put(0,struct.unpack('<I',struct.pack('<f',float(signed)))[0]);uc.reg_write(cpu.pc,uc.reg_read(cpu.lr));return
        return old_external(cpu,uc,address,size,user)
    Cpu.external=external
    try:cpu=Cpu(original,False,manifest)
    finally:Cpu.external=old_external
    def word(a):return struct.unpack('<I',cpu.uc.mem_read(a,4))[0]
    def put(a,w):cpu.pointer(a,w&0xffffffff)
    def string(a):
        raw=bytearray()
        for n in range(1048576):
            ch=cpu.uc.mem_read(a+n,1)[0]
            if ch==0:return raw.decode('utf-8')
            raw.append(ch)
        raise AssertionError('unterminated source string')
    bump=cpu.data+0x50000
    def alloc(size):
        nonlocal bump
        bump=(bump+15)&~15;address=bump;bump+=max(size,1)
        assert bump<cpu.data+0x1000000
        cpu.uc.mem_write(address,bytes(max(size,1)));return address
    def cstr(text):
        raw=text.encode();p=alloc(len(raw)+1);cpu.uc.mem_write(p,raw+b'\0');return p
    def set_string(a,text):
        raw=text.encode();p=cstr(text);put(a,p+len(raw)+1);put(a+0x10,p+len(raw));put(a+0x14,p)
    def source_string(a):return bytes(cpu.uc.mem_read(word(a+0x14),word(a+0x10)-word(a+0x14))).decode()
    # Deploy all real rows using the original ARM field/stride layouts. Native
    # owning strings are represented by ARM fixture pointers, never truncated.
    skill_lists,skill_rows=skills[1],skills[2];faery_lists,faery_rows=faeries[1],faeries[2]
    sb=alloc(len(skill_rows)*76);fb=alloc(len(faery_rows)*36)
    for i,row in enumerate(skill_rows):
        put(sb+i*76+0x24,row['script_length']);put(sb+i*76+0x28,cstr(row['script']))
    for i,row in enumerate(faery_rows):
        fields=[0,row['description'],row['elemental'],row['model_file'],row['name_id'],
                row['spell_script_length'],cstr(row['spell_script']),row['spell_type'],row['type']]
        cpu.uc.mem_write(fb+i*36,struct.pack('<9I',*(w&0xffffffff for w in fields)))
    for name,lists,rows in [('Skill',skill_lists,sb),('Faery',faery_lists,fb)]:
        table=alloc(len(lists)*12)
        for i,row in enumerate(lists):
            members=alloc(len(row['members'])*4)
            for j,value in enumerate(row['members']):put(members+j*4,value)
            cpu.uc.mem_write(table+i*12,struct.pack('<3I',0,len(row['members']),members))
        for symbol,value in [(f'_ZN6Arrays{len(name)+9}{name}ListTable7membersE',table),
                             (f'_ZN6Arrays{len(name)+9}{name}ListTable4sizeE',len(lists)),
                             (f'_ZN6Arrays{len(name)+5}{name}Table7membersE',rows),
                             (f'_ZN6Arrays{len(name)+5}{name}Table4sizeE',len(skill_rows) if name=='Skill' else len(faery_rows))]:
            put(cpu.symbols[symbol],value)
    put(cpu.symbols['gAssertLevel'],0)
    # Original leaves execute for fallback/boundary inputs. No extra body credit
    # is assigned to the adapter or to these already-maintained getters.
    selectors=[]
    for raw,skill_id,faery_id in host['selectors']:
        put(OWNER+0x1068,raw);put(OWNER+0x106c,raw)
        a=cpu.invoke('_ZNK9Character18GetCharSkillListIdEv',[OWNER])
        b=cpu.invoke('_ZNK9Character18GetCharFaeryListIdEv',[OWNER])
        assert [a,b]==[skill_id,faery_id]
        selectors.append({'raw':raw,'skill':a,'faery':b})
    by_address={int(r['elf_address'],0):r['original_symbol'] for r in manifest['functions']}
    leaf_ops={
        '_ZN13DebugSwitches4loadEv':0,'_ZN13DebugSwitches9GetSwitchERKSs':1,
        '_ZNSs19_M_range_initializeEPKcS0_':2,'_ZNSs9_M_assignEPKcS0_':3,
        '_ZN3sfc6script3lua9ArgumentsC1Ev':9,'_ZN3sfc6script3lua9Arguments10pushStringEPKc':10,
        '_ZN3sfc6script3lua9Arguments11pushIntegerEi':11,'_ZN3sfc6script3lua5Value9setStringEPKc':12,
        '_ZN3sfc6script3lua5Value9setNumberEf':13,'_ZN3sfc6script3lua9ArgumentsD1Ev':14,
        '_ZN9LuaScript4LoadEPKc':15,'_ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE':16,
        '_ZN9LuaScript4CallEPKc':16,'_Znwj15MemoryHintState':17}
    getter_count=cpu.symbols['_ZNK15PyDataConstants11getConstantEPKcS1_']
    reserve=cpu.symbols['_ZNSt6vectorIP17CharAISkillScriptSaIS1_EE7reserveEj']
    ctor=cpu.symbols['_ZN17CharAISkillScriptC1EP9CharacterPKcj']
    observations=[]
    for case in host['cases']:
        cpu.uc.mem_write(AI,bytes(0x1000));cpu.uc.mem_write(AIS,bytes(0x1000));cpu.uc.mem_write(cpu.stack,bytes(0x10000))
        put(AI+4,OWNER);put(AI+0x1c,AIS);put(OWNER+0x1068,case['skill_list']);put(OWNER+0x106c,case['faery_list'])
        vt=alloc(0x100);put(AIS,vt);put(vt+0xcc,CALLBACK);set_string(AIS+0x68,'data/scripts/ai/')
        if case['scenario']=='repeat':
            for offset,presence in [(0xb4,case['skills']),(0xc0,case['faeries'])]:
                begin=alloc(len(presence)*4)
                for i,present in enumerate(presence):put(begin+4*i,0x2060000+i*32 if present else 0)
                put(AI+offset,begin);put(AI+offset+4,begin+len(presence)*4);put(AI+offset+8,begin+len(presence)*4)
        events=[];constructors=[];values={};arg_values={};constant_calls=0
        def returned(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
        def observe(uc,address,size,user):
            nonlocal constant_calls
            r0,r1,r2,r3=[cpu.reg(i) for i in range(4)]
            if address==getter_count:
                assert string(r1)=='FaeryTypes' and string(r2)=='COUNT';constant_calls+=1;returned(faery_count);return
            if address==reserve:
                if r1:
                    p=alloc(r1*4);put(r0,p);put(r0+4,p);put(r0+8,p+r1*4)
                returned();return
            if address==ctor:
                constructors.append([string(r2),r3]);return # Execute the complete original child.
            if address==CALLBACK:events.append([19,'',0,-1]);returned();return
            if address in (0x708f00,0x310440):returned();return
            name=by_address.get(address)
            if name=='_ZNSsC1EPKcRKSaIcE':set_string(r0,string(r1));returned(r0);return
            if name=='_ZNSsD1Ev':returned(r0);return
            if name not in leaf_ops:return
            op=leaf_ops[name]
            if op==0:events.append([op,'',0,-1]);returned()
            elif op==1:
                assert source_string(r1)=='Lua_LoadMemUsage';events.append([op,source_string(r1),0,-1]);returned()
            elif op in (2,3):
                text=bytes(cpu.uc.mem_read(r1,r2-r1)).decode()
                events.append([op,text,0,-1] if op==3 else [op,'',0,-1]);set_string(r0,text);returned(r0)
            elif op==9:
                array=alloc(12);data=alloc(512);put(r0+4,array);put(array,data);put(array+4,data);put(array+8,data+512)
                arg_values[r0]=[];values[data]=None;returned(r0)
            elif op in (10,11):
                array=word(r0+4);base=word(array);index=len(arg_values[r0]);value=string(r1) if op==10 else r1
                arg_values[r0].append(value);values[base+index*112]=value;put(array+4,base+(index+1)*112);returned(r0)
            elif op in (12,13):values[r0]=string(r1) if op==12 else r1;returned(r0)
            elif op==14:returned(r0)
            elif op==15:
                text=string(r1);events.append([op,text,0,-1]);returned(case['scenario']!='load_false')
            elif op==16:
                if name.endswith('ArgumentsE'):
                    base=word(word(r2+4));events.append([op,values[base],values[base+112],2])
                else:events.append([op,string(r1),0,0])
                returned()
            elif op==17:
                assert r0==0x1c and r1==0;returned(alloc(r0))
        hook=cpu.uc.hook_add(UC_HOOK_CODE,observe)
        cpu.invoke('_ZN6CharAI18SetSkillsAndSpellsEv',[AI],budget=2000000)
        cpu.uc.hook_del(hook)
        assert events==case['events'],(case['name'],case['scenario'],events,case['events'])
        def presence(offset):
            begin,end=word(AI+offset),word(AI+offset+4)
            return [word(a)!=0 for a in range(begin,end,4)] if begin else []
        assert presence(0xb4)==case['skills'] and presence(0xc0)==case['faeries']
        if case['scenario']!='repeat':assert constructors==case['instances']
        assert constant_calls==case['constant_queries']
        assert source_string(AIS+0x68)=='data/scripts/ai/'
        observations.append({'name':case['name'],'scenario':case['scenario'],'external_calls':len(events),
                             'skill_slots':len(case['skills']),'faery_slots':len(case['faeries']),
                             'constructors':len(constructors),'fresh_constant_queries':constant_calls,'matched':True})
    return {'validation':'PASS','cases':len(observations),'selector_cases':len(selectors),'mismatches':0,
            'observations':observations,'selectors':selectors,'verified_imports':verified,
            'original_instructions_seen':{r['original_symbol']:sum(int(r['elf_address'],0)<=p<int(r['elf_address'],0)+r['size'] for p in cpu.seen)
                                          for r in manifest['functions'] if r.get('executed')},
            'scope':'Original caller, list ID/list/Skill/Faery getters and child constructor execute over all retained cache rows. Debug, Lua, source String/Arguments/Value, allocation/reserve and constant lookup are named modeled providers; Lua does not execute.'}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',required=True,type=Path);parser.add_argument('--cache',required=True,type=Path)
    parser.add_argument('--original-elf',required=True,type=Path)
    parser.add_argument('--output',type=Path,default=MODULE/'build/player-skills-preparation-v3/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/player-skills-preparation-v3/validation.json')
    args=parser.parse_args();cache=args.cache.resolve();original=args.original_elf.resolve();output=args.output.resolve()
    assert digest(original)==ORIGINAL_SHA
    output.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'player_skill_tables_adapter.cpp',MODULE/'character_player_skills_preparation_v3.cpp',
             MODULE/'character_skill_cooldown_services.cpp',MODULE/'character_ai_set_skills_and_spells.cpp',
             MODULE/'character_ai_skill_script_constructor.cpp',MODULE/'character_faery_selection.cpp',
             ROOT/'port/game-data/data.cpp',ROOT/'port/game-data/skill_tables.cpp',ROOT/'port/game-data/properties.cpp',
             ROOT/'port/game-data/class_tables.cpp',MODULE/'tests/character_player_skills_preparation_v3.cpp']
    c_source=ROOT/'port/pydata-constants/constants.c';c_header=c_source.with_suffix('.h')
    tracked=sources+[p.with_suffix('.hpp') for p in sources if p.with_suffix('.hpp').is_file()]+[
        c_source,c_header,Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),ROOT/'port/engine-resources/tests/cpu.py',
        MODULE/'tests/elf_import_identity.py',MODULE/'tests/run_character_ai_set_skills_and_spells_host.py',ROOT/'port/game-data/tests/run_skill_tables_host.py']
    tracked=sorted(set(tracked));before={str(p.relative_to(ROOT).as_posix()):digest(p) for p in tracked}
    gcc=args.compiler.with_name('gcc.exe' if args.compiler.suffix=='.exe' else 'gcc')
    obj=output.with_suffix('.constants.o')
    commands=[[str(gcc),'-std=c11','-O1','-Wall','-Wextra','-Werror','-c',str(c_source),'-o',str(obj)],
              [str(args.compiler),'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),str(obj),'-o',str(output)]]
    for command in commands:
        compiled=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
        if compiled.returncode:raise RuntimeError(compiled.stdout+compiled.stderr)
    host=json.loads(subprocess.check_output([str(output),str(cache)],text=True))
    assert host['validation']=='PASS' and host['host_cases']==9 and host['guards']>=45 and host['provider_failure_cases']==120
    assert host.get('timer_field_lease_guards',0)>=10,'retained field18 lease guards missing'
    helper=load_module(ROOT/'port/game-data/tests/run_skill_tables_host.py','actual_skill_decoders')
    py=cache/'data/pydata'
    skill=helper.parse_skills((py/'skills_pyarray.bin').read_bytes(),(py/'skills_pyarraynames.bin').read_bytes())
    faery=helper.parse_faeries((py/'faeries_pyarray.bin').read_bytes(),(py/'faeries_pyarraynames.bin').read_bytes())
    # The host executes each real GetConstant query. Resolve COUNT independently
    # from this unchanged wire file for the original callee boundary.
    reader=helper.Reader((py/'faeries_pycst.bin').read_bytes());constants={}
    for _ in range(reader.word()):
        group=reader.string()
        for _ in range(reader.word()):
            key=reader.string();constants[(group,key)]=reader.integer()
    reader.finish();count=constants[('FaeryTypes','COUNT')]
    arm=original_compare(original,host,skill,faery,count)
    after={str(p.relative_to(ROOT).as_posix()):digest(p) for p in tracked};assert before==after,'source changed during proof'
    inputs=sorted({*py.glob('skills_*.bin'),*py.glob('faeries_*.bin'),*py.glob('character_properties_*.bin'),*py.glob('character_classes_*.bin')})
    used_scripts=set()
    for case in host['cases']:
        for op,text,_,_ in case['events']:
            if op==15:used_scripts.add(cache/'data/scripts/skills'/f'{text}.luac')
    inputs+=sorted(used_scripts)
    report={'validation':'PASS','adapter_new_complete_original_bodies':0,'attribution':{'adam_commit':'c3ae797332a82a30a586b9156cddc25445e36a4c','reused_design':'stable nullable vector/instance/Arguments/path ownership; current decoded tables and source caller retained'},
            'host_report':host,'original_arm_comparison':arm,'source_sha256':before,'original_sha256':ORIGINAL_SHA,
            'cache_input_sha256':{p.relative_to(cache).as_posix():digest(p) for p in inputs},'compiler_commands':commands,
            'executable_sha256':digest(output),'lua_executed':False,'native_wired':False,
            'scope':'Host base/class property resolution uses maintained cached-sheet APIs. Debug/VM/AIS external effects are explicit controlled providers, not a live player Session. Cache files are read, no giant assets are copied.'}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'guards':host['guards'],
                      'timer_field_lease_guards':host['timer_field_lease_guards'],
                      'provider_failure_cases':host['provider_failure_cases'],'original_arm_cases':arm['cases'],
                      'selector_cases':arm['selector_cases'],'mismatches':0}))
if __name__=='__main__':main()
