"""CharProperties::HandleDots: complete original caller and explicit providers."""
from __future__ import annotations
import argparse, hashlib, json, os, random, shutil, struct, subprocess, sys
from pathlib import Path

MODULE=Path(__file__).resolve().parents[1]
ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-dot-tick/original-functions.json'
C,B,P,S,T=0x10014000,0x10018000,0x10014560,0x10014ff4,0x10025000
def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def signed(value):return value if value<0x80000000 else value-0x100000000
def fixtures():
    rows=[('all_zero',[0]*6+[0]*6+[0]),
          ('all_negative',[0xffffffff,0x80000000]*3+[0]*6+[0]),
          ('all_positive',[1,2,3,4,5,6]+[0]*6+[0]),
          ('all_dead',[1,2,3,4,5,6]+[1,7,0x80000000,0xffffffff,1,1]+[0]),
          ('mixed_signed',[0,0xffffffff,1,0x80000000,0x7fffffff,256]+[0,0,0,0,7,0]+[0])]
    for index in range(6):
        for amount in (1,256,0x7fffffff):
            for dead_word in (0,1,0x80000000,0xffffffff):
                values=[0]*6;values[index]=amount;dead=[0]*6;dead[index]=dead_word
                rows.append((f'element_{index}_amount_{amount}_dead_{dead_word}',values+dead+[0]))
    for mutation in range(1,9):rows.append((f'freshness_{mutation}',[1,2,3,4,5,6]+[0]*6+[mutation]))
    rng=random.Random(0x3df3f0)
    for index in range(64):
        values=[rng.choice((0,1,256,0xffffffff,0x80000000,rng.getrandbits(32))) for _ in range(6)]
        dead=[rng.choice((0,0,0,1,0x80000000,0xffffffff)) for _ in range(6)]
        rows.append((f'random_{index}',values+dead+[rng.randrange(9)]))
    return rows

def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'))
    from cpu import Cpu
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes()
    assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            seg=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
            offset=int(seg['p_offset'])+at-int(seg['p_vaddr']);return raw[offset:offset+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (symbol['st_value'],symbol['st_size'])==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    records=[]
    for name,inputs in fixtures():
        initial_values,initial_dead,mutation=inputs[:6],inputs[6:12],inputs[12]
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(C,bytes(0x8000))
        old.pointer(P+4,C);old.pointer(C,T);old.pointer(B,T);old.pointer(T+0x34,0x3a2ed4)
        for index,value in enumerate(initial_values):old.pointer(S+4+(126+index)*4,value)
        dead=list(initial_dead);trace=[];index=[0];result_pointer=[0];produced=[0];applied=[0]
        def mutate(operation):
            if index[0]!=0:return
            if mutation==1 and operation==0:old.pointer(P+4,B);old.pointer(S+4+126*4,800);old.pointer(S+4+127*4,501)
            if mutation==2 and operation==1:old.pointer(P+4,B);old.pointer(S+4+126*4,902)
            if mutation==3 and operation==2:old.pointer(P+4,B);old.pointer(S+4+127*4,303)
            if mutation==4 and operation==3:old.pointer(P+4,B);old.pointer(S+4+127*4,404)
            if mutation==5 and operation==0:old.pointer(S+4+127*4,0x80000000);old.pointer(S+4+128*4,777)
            if mutation==6 and operation==1:dead[1]=7
            if mutation==7 and operation==2:old.pointer(P+4,B);old.pointer(S+4+131*4,1555)
            if mutation==8 and operation==3:old.pointer(P+4,B);old.pointer(S+4+131*4,1666)
        def observe(_,at,__,___):
            if at==0x3dedb4:
                assert old.reg(0)==P and old.reg(1)==S
                property_id=old.reg(2);assert 126<=property_id<132;index[0]=property_id-126
                value=word(S+4+property_id*4);trace.append([0,P,S,property_id,0,-1,value]);mutate(0);returned(value)
            elif at==0x3a2ed4:
                character=old.reg(0);assert character==word(P+4);value=dead[index[0]]
                trace.append([1,character,0,126+index[0],0,-1,value]);mutate(1);returned(value)
            elif at==0x3b2e68:
                out,attacker,defender,amount=[old.reg(i) for i in range(4)]
                element=signed(word(old.uc.reg_read(old.sp)))
                assert attacker==defender==word(P+4) and element==index[0]-1 and signed(amount)>0
                if result_pointer[0]:assert result_pointer[0]==out
                result_pointer[0]=out
                # Explicit named F_DotAttack output fixture. Its body is not
                # executed or credited; the caller passes its local by address.
                old.uc.mem_write(out,struct.pack('<6iIIii',signed(amount),element,123,456,7,8,9,0x20080000,-1,element))
                produced[0]+=1;trace.append([2,attacker,defender,126+index[0],amount,element,0]);mutate(2);returned(0x12345678)
            elif at==0x3b10b4:
                out,attacker,defender,mode=[old.reg(i) for i in range(4)]
                assert out==result_pointer[0] and attacker==defender==word(P+4) and mode==0
                value=struct.unpack('<6iIIii',old.uc.mem_read(out,40))
                assert value[1:]==(index[0]-1,123,456,7,8,9,0x20080000,-1,index[0]-1)
                applied[0]+=1;trace.append([3,attacker,defender,126+index[0],mode,value[9],value[0]&0xffffffff])
                old.pointer(out+24,0xfeed);mutate(3);returned(0x87654321)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe);old.invoke(0x3df3f0,[P]);old.uc.hook_del(hook)
        reads=[row for row in trace if row[0]==0];queries=[row for row in trace if row[0]==1]
        expected={'status':0,'calls':len(trace),'last_operation':trace[-1][0],'property_reads':6,
                  'positive_properties':len(queries),'dead_skips':sum(row[6]!=0 for row in queries),
                  'attacks':produced[0],'applications':applied[0],'completed_properties':6,
                  'last_property':131,'captured_amount':reads[-1][6],'owner':word(P+4),
                  'values':[word(S+4+(126+i)*4) for i in range(6)],'dead':dead,
                  'produced':produced[0],'applied':applied[0],'trace':trace}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,inputs)],text=True))
        assert actual==expected,(name,actual,expected)
        records.append({'case':name,'input':inputs,'matched':True,'source_result':expected,'compiled_result':actual})
    body=manifest['functions'][0];seen=sum(address in old.seen for address in range(int(body['elf_address'],0),int(body['elf_address'],0)+body['size'],4))
    assert seen==body['size']//4
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'caller_instructions_seen':seen,
            'imports_executed':old.import_calls,'results':records,
            'executed_scope':'All 36 instructions in original144B HandleDots caller. _GetProperty292B, Character::IsDead12B virtual call, F_DotAttack412B and F_ApplyResult3388B are explicit named provider fixtures. Captured signed amount, six live sheet reads, raw dead return, fresh owner reads, self arguments, element and apply-mode/order are compared. Provider bodies and positive native damage/result effects are not claimed. No soft-float helper or guessed PLT import is modeled.'}

def cache_audit(cache):
    folder=cache/'data/pydata'
    def strings(raw):
        count=struct.unpack_from('<I',raw)[0];at=4;items=[]
        for _ in range(count):
            size=struct.unpack_from('<I',raw,at)[0];at+=4;items.append(raw[at:at+size].decode('ascii'));at+=size
        assert at<=len(raw);return items,at
    paths=[folder/'character_properties_pyarray.bin',folder/'character_properties_pyarraynames.bin',folder/'character_properties_pystructnames.bin']
    if not all(path.is_file() for path in paths):
        # Some cache extractions put the data directory one layer deeper.
        paths=[next(cache.rglob(path.name)) for path in paths]
    pinned=json.loads(MANIFEST.read_text())['cache_files']
    for path in paths:assert len(path.read_bytes())==pinned[path.name]['bytes'] and digest(path)==pinned[path.name]['sha256']
    raw,names_raw,fields_raw=[path.read_bytes() for path in paths]
    (names,names_end),(fields,fields_end)=strings(names_raw),strings(fields_raw)
    assert len(fields)==224 and fields_end==len(fields_raw) and len(names_raw)-names_end==83
    assert fields[126:132]==['Dot_Tick_Damage_Normal','Dot_Tick_Damage_Fire','Dot_Tick_Damage_Water','Dot_Tick_Damage_Lightning','Dot_Tick_Damage_Earth','Dot_Tick_Damage_Air']
    records_end=4+len(names)*896
    assert struct.unpack_from('<I',raw)[0]==len(names) and len(raw)-records_end==196
    rows=[]
    for name in ('Crypt_Ghost','Crypt_Ghost_MIDBOSS','Crypt_Ghost_RE'):
        if name not in names:continue
        index=names.index(name);values=list(struct.unpack_from('<6i',raw,4+index*896+126*4))
        rows.append({'name':name,'row':index,'authored_raw_dot_words':values})
    assert len(rows)==3 and all(all(value==0 for value in row['authored_raw_dot_words']) for row in rows)
    return {'files':{str(path.relative_to(cache)).replace('\\','/'):digest(path) for path in paths},
            'field_ids':list(range(126,132)),'field_names':fields[126:132],'rows':rows,
            'name_table_consumed_bytes':names_end,'name_table_additional_metadata_bytes':83,
            'property_table_consumed_bytes':records_end,'property_table_additional_metadata_bytes':196,
            'scope':'Unchanged authored base table rows only. Current resolved properties after class/buffs/gameplay must be read live; these zero base words do not justify a permanent timer no-op.'}

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True)
    p.add_argument('--cache',type=Path,default=ROOT.parent/'cache/files');p.add_argument('--output',type=Path,default=MODULE/'build/character-dot-tick/host.exe')
    p.add_argument('--report',type=Path,default=MODULE/'build/character-dot-tick/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_dot_tick.cpp',MODULE/'tests/character_dot_tick.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,capture_output=True,text=True)
    if build.returncode:raise RuntimeError(build.stdout+build.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host=={'validation':'PASS','host_cases':36,'guard_cases':19,'failure_cases':48,'mismatches':0}
    comparison=oracle(a.original_elf.resolve(),exe);assert comparison['comparisons']==149 and comparison['mismatches']==0
    cache=cache_audit(a.cache.resolve());owned=[MODULE/'character_dot_tick.hpp',MODULE/'character_dot_tick.cpp',MODULE/'tests/character_dot_tick.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-dot-tick/NOTES.md']
    reused=[ROOT/'port/game-data/combat_result.hpp',ROOT/'port/game-data/combat.hpp',ROOT/'port/engine-resources/tests/cpu.py']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':digest(a.original_elf),
            'compiler_command':command,'source_sha256':{str(path.relative_to(ROOT)).replace('\\','/'):digest(path) for path in owned},
            'reused_source_sha256':{str(path.relative_to(ROOT)).replace('\\','/'):digest(path) for path in reused},
            'cache_audit':cache,'new_complete_caller_bodies':1,'new_complete_dependency_bodies':0,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'validation':'PASS',**host,'original_arm_cases':comparison['comparisons'],'caller_instructions_seen':comparison['caller_instructions_seen']}))
if __name__=='__main__':main()
