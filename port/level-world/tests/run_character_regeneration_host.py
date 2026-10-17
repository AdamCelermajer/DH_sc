"""Complete original HP/MP caller instructions with explicit owned providers."""
from __future__ import annotations
import argparse,hashlib,json,os,random,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-regeneration/original-functions.json'
C,P,S,D,D2,T=0x10014000,0x10014560,0x10014ff4,0x9a1d18,0x10030000,0x10025000
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def signed(x):return x if x<0x80000000 else x-0x100000000
def fixtures():
    rows=[]
    values=[(10,100,-1),(100,100,-1),(-1,12160,-1),(10,100,0),(10,100,1),(10,100,89),(10,100,90),(10,100,91),(101,100,-1),(-100,100,-1),(-100,-1,-1),(0,0,-1),(0,-1,-1),(-2147483648,2147483647,-1),(2147483647,2147483647,1),(2147483647,100,2147483647),(0,2147483647,2147483647),(-2147483648,-2147483648,-1),(0,-2147483648,-1),(0,100,-2147483648)]
    for mana in (0,1):
        for i,(current,maximum,amount) in enumerate(values):rows.append((f'arithmetic_{mana}_{i}',[mana,current&0xffffffff,maximum&0xffffffff,amount&0xffffffff,0,0]))
        for mutation in range(1,9):rows.append((f'freshness_{mana}_{mutation}',[mana,10,100,0xffffffff,0,mutation]))
        for query in (1,0xffffffff):rows.append((f'discard_query_{mana}_{query}',[mana,10,100,0xffffffff,query,0]))
    rng=random.Random(0x3bdca4)
    for i in range(40):rows.append((f'random_{i}',[i%2,rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32),rng.randrange(9)]))
    return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    manifest=json.loads(MANIFEST.read_text());raw=original.read_bytes();assert digest(original)==manifest['original_sha256']
    with original.open('rb') as stream:
        elf=ELFFile(stream);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        def data(at,n):
            seg=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);offset=int(seg['p_offset'])+at-int(seg['p_vaddr']);return raw[offset:offset+n]
        def word(at):return struct.unpack('<I',data(at,4))[0]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']];assert (symbol['st_value'],symbol['st_size'])==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        lit=manifest['debug_literal'];text=lit['text'].encode()+b'\0';assert data(int(lit['elf_address'],0),len(text))==text and hashlib.sha256(text).hexdigest()==lit['sha256']
        got=manifest['debug_singleton_got']
        for kind in ('regen_hp','regen_mp'):
            at=(int(lit[kind+'_pc_add'],0)+8+word(int(lit[kind+'_literal'],0)))&0xffffffff;assert at==int(lit['elf_address'],0)
            base=(int(got[kind+'_base_pc_add'],0)+8+word(int(got[kind+'_base_literal'],0)))&0xffffffff;assert base==int(got['got_base'],0)
            slot=base+word(int(got[kind+'_symbol_literal'],0));assert slot==int(got['slot'],0) and word(slot)==D
        assert (symbols[got['symbol']]['st_value'],symbols[got['symbol']]['st_size'])==(D,got['bytes'])
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def returned(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
    def string(at):
        b=bytearray()
        while old.uc.mem_read(at,1)!=b'\0':b+=old.uc.mem_read(at,1);at+=1
        return b.decode('ascii')
    records=[]
    for name,x in fixtures():
        mana,initial_current,initial_maximum,amount,query,mutation=x;current_id,maximum_id=(41,43) if mana else (36,38)
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(C,bytes(0x3000));old.pointer(S+4+current_id*4,initial_current);old.pointer(S+4+maximum_id*4,initial_maximum)
        old.pointer(int(got['slot'],0),D);calls=[];reads=[0];string_at=[0];live=[0];captured=[];positive=[0];added=[0]
        def mutate(op):
            if mutation==1 and op==0 and reads[0]==1:old.pointer(S+4+current_id*4,800)
            if mutation==2 and op==0 and reads[0]==2:old.pointer(S+4+current_id*4,1000);old.pointer(S+4+maximum_id*4,2000)
            if mutation==3 and op==1:old.pointer(int(got['slot'],0),D2);old.pointer(S+4+current_id*4,4000);old.pointer(S+4+maximum_id*4,1)
            if mutation==4 and op==2:old.pointer(int(got['slot'],0),D2);old.pointer(S+4+current_id*4,20)
            if mutation==5 and op==3:old.pointer(S+4+current_id*4,3000);old.pointer(S+4+maximum_id*4,5)
            if mutation==6 and op==4:old.pointer(S+4+current_id*4,1234)
            if mutation==7 and op==0 and reads[0]==1:old.pointer(int(got['slot'],0),D2)
        def call(op,subject=0,sheet=0,prop=0,delta=0,word=0,identity=0,raw_return=0):
            calls.append([op,subject,sheet,prop,delta,word,identity]);mutate(op);returned(raw_return)
        def observe(_,at,__,___):
            if at==0x3dedb4:
                assert old.reg(0)==P and old.reg(1)==S;prop=old.reg(2);assert prop==(current_id if not reads[0] else maximum_id)
                value=get(S+4+prop*4);reads[0]+=1;captured.append(value);call(0,P,S,prop,word=value,raw_return=value)
            elif at==0x337888:
                assert old.reg(0) in (D,D2);positive[0]=old.reg(4);assert signed(positive[0])>0;call(1,old.reg(0))
            elif at==0x3140ec:
                assert string(old.reg(1))==lit['text'];string_at[0]=old.reg(0);live[0]=1;call(2,identity=T,raw_return=old.reg(0))
            elif at==0x337a88:
                assert old.reg(1)==string_at[0] and live[0];call(3,old.reg(0),T,word=query,raw_return=query)
            elif at==0x318254:
                assert old.reg(0)==string_at[0] and live[0];live[0]=0;call(4,T)
            elif at==0x3e0708:
                assert old.reg(0)==P and old.reg(1)==current_id and old.reg(2)==positive[0]
                prop=old.reg(1);delta=old.reg(2);old.pointer(S+4+prop*4,(get(S+4+prop*4)+delta)&0xffffffff);added[0]=1;call(5,P,prop=prop,delta=delta)
        hook=old.uc.hook_add(UC_HOOK_CODE,observe);old.invoke(0x3bdbb8 if mana else 0x3bdca4,[C,amount]);old.uc.hook_del(hook)
        expected={'status':0,'current':captured[0],'maximum':captured[1],'positive_amount':positive[0],'added':added[0],'side_current':get(S+4+current_id*4),'side_maximum':get(S+4+maximum_id*4),'debug_selection':get(int(got['slot'],0)),'live_string':live[0],'calls':calls}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True));assert actual==expected,(name,actual,expected)
        records.append({'case':name,'input':x,'matched':True,'source_result':expected,'compiled_result':actual})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,'executed_scope':'Two complete original236B HP/MP caller instruction ranges. _GetProperty292B, PROPS_Add144B, DebugSwitches::load512B/GetSwitch196B, native string constructor52B/destructor68B are named provider fixtures. Actual caller instructions perform two reads, signed negative selection, wrap32 sum/difference cap, positive gate and all provider call ordering; debug blocks are not skipped. Source stack-protection housekeeping executes; guard remains unchanged. No full debug/file/map/string/property dependency or native wiring claim. No floating/import numerical helper is modeled.','debug_blocks_skipped':False}
def cache_audit(cache,manifest):
    row=manifest['cache_debug_configuration'];p=cache/row['path'];raw=p.read_bytes();assert len(raw)==row['bytes'] and digest(p)==row['sha256'] and raw[:12].hex()==row['header_hex']
    count=struct.unpack_from('<I',raw,12)[0];at=16;rows=[]
    for _ in range(count):
        n=struct.unpack_from('<I',raw,at)[0];at+=4;name=raw[at:at+n];assert name and all(32<=x<127 for x in name);at+=n;value=raw[at];at+=1;rows.append({'name':name.decode('ascii'),'value':value})
    assert at==len(raw) and rows==row['rows'] and len(rows)==23
    assert not any(item['name']==manifest['debug_literal']['text'] for item in rows) and not row['requested_key_present']
    return row
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler');p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--cache',type=Path,default=ROOT.parent/'cache/files');p.add_argument('--output',type=Path,default=MODULE/'build/character-regeneration/host.exe');p.add_argument('--report',type=Path,default=MODULE/'build/character-regeneration/validation.json');a=p.parse_args()
    compiler=a.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:p.error('pass --compiler')
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_regeneration.cpp',MODULE/'tests/character_regeneration.cpp',ROOT/'port/game-data/properties.cpp',ROOT/'port/game-data/class_tables.cpp',ROOT/'port/game-data/vitals.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,capture_output=True,text=True)
    if build.returncode:raise RuntimeError(build.stdout+build.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==95 and host['live_property_reuse_cases']==512 and host['mismatches']==0
    comparison=oracle(a.original_elf.resolve(),exe);assert comparison['comparisons']==100 and comparison['mismatches']==0
    manifest=json.loads(MANIFEST.read_text());cache=cache_audit(a.cache.resolve(),manifest)
    owned=[MODULE/'character_regeneration.hpp',MODULE/'character_regeneration.cpp',MODULE/'tests/character_regeneration.cpp',Path(__file__).resolve(),MANIFEST,MODULE/'reference/character-regeneration/NOTES.md']
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':manifest['original_sha256'],'compiler_command':command,'source_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):digest(f) for f in owned},'reused_source_sha256':{str(f.relative_to(ROOT)).replace('\\','/'):digest(f) for f in sources[2:]},'cache_debug_configuration':cache,'new_complete_caller_bodies':2,'new_complete_dependency_bodies':0,'native_wired':False}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'live_property_reuse_cases':512,'original_arm_cases':100,'mismatches':0}))
if __name__=='__main__':main()
