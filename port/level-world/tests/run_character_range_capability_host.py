"""Live capability callers, aliased stores and fresh inventory reads vs original ARM."""
from __future__ import annotations
import argparse,hashlib,json,os,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-range-capability/original-functions.json'
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
A=0x10014000;INV=A+0x37c
I0,I1,SETS,SWAPPED,E0,E1,REF0,REF1,TABLE,NEXT,VARIABLE,OUT=0x10024000,0x10024100,0x10026000,0x10027000,0x10028000,0x10028100,0x1002a000,0x1002a100,0x10030000,0x10032000,0x10036000,0x10038000
def fixtures():
    rows=[];base=[0,0,512,1280,0xffffffff,4,5,0,0,1,0]
    def add(name,changes=None):
        x=base.copy()
        for i,v in (changes or {}).items():x[i]=v
        rows.append((name,x))
    for op in range(4):
        for binding in range(2):
            for ty in (1,4,5):add(f'op{op}_binding{binding}_type{ty}',{0:op,1:binding,5:ty})
            add(f'op{op}_binding{binding}_absent',{0:op,1:binding,9:0})
            add(f'op{op}_binding{binding}_slot1',{0:op,1:binding,10:1})
            for mutation in range(1,8):add(f'op{op}_binding{binding}_mutation{mutation}',{0:op,1:binding,8:mutation})
        for alias in range(8):
            add(f'op{op}_alias{alias}',{0:op,7:alias})
            add(f'op{op}_property_alias{alias}',{0:op,4:9,7:alias})
    for value in (0,1,0xfffffffe,0x80000000,0x7fffffff):
        add('property_projectile_'+str(value),{4:value,9:0})
        add('virtual_character_property_'+str(value),{0:1,1:1,4:value,5:1})
    for value in (0,1,255,256,257,0xffffffff,0xffffff01,0xffffff00,0x80000000,0x7fffffff):
        add('property_ASR8_lower_'+str(value),{2:value,4:9})
        add('property_ASR8_upper_'+str(value),{3:value,4:9})
    return rows
def oracle(original,exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
    raw=original.read_bytes();assert hashlib.sha256(raw).hexdigest()==SHA
    manifest=json.loads(MANIFEST.read_text())
    with original.open('rb') as f:
        e=ELFFile(f);symbols={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()};loads=[p for p in e.iter_segments() if p['p_type']=='PT_LOAD']
        def data(at,n):
            s=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz']);off=int(s['p_offset'])+at-int(s['p_vaddr']);return raw[off:off+n]
        for row in manifest['functions']:
            at,n=int(row['elf_address'],0),row['size'];symbol=symbols[row['original_symbol']]
            assert (int(symbol['st_value']),int(symbol['st_size']))==(at,n)
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
        for row in manifest['vtable_ranges']:
            at,n=int(row['elf_address'],0),row['size'];assert int(symbols[row['symbol']]['st_value'])==at
            assert hashlib.sha256(data(at,n)).hexdigest()==row['sha256']
            for slot in row['slots']:assert struct.unpack('<I',data(int(row['address_point'],0)+int(slot['byte_offset'],0),4))[0]==int(slot['target'],0)
        got=0x3f9e14+8+struct.unpack('<I',data(0x3f9e2c,4))[0];offset=struct.unpack('<I',data(0x3f9e30,4))[0]
    old=Cpu(original,False,manifest);old.uc.mem_map(0x10000000,0x60000)
    def get(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
    def words(at,n):return list(struct.unpack('<'+'I'*n,old.uc.mem_read(at,n*4)))
    records=[]
    for name,x in fixtures():
        op,binding,minimum,maximum,projectile,type0,type1,alias,mutation,present,current=x
        old.uc.mem_write(old.stack,bytes(0x10000));old.uc.mem_write(A,bytes(0x1800))
        old.pointer(A+0x1070,minimum);old.pointer(A+0x1074,maximum);old.pointer(A+0x1078,projectile)
        old.pointer(INV,0x966124 if binding else 0x967768);old.pointer(INV+0x14,SETS);old.uc.mem_write(INV+0x2e,bytes([current]))
        old.pointer(SETS,E0);old.pointer(SETS+12,E1);old.pointer(SWAPPED,E1);old.pointer(SWAPPED+12,E0)
        old.pointer(E0+4,REF0 if present else 0);old.pointer(E1+4,REF1);old.pointer(REF0,I0);old.pointer(REF1,I1)
        old.pointer(I0+4,0);old.pointer(I1+4,1);old.uc.mem_write(TABLE,bytes(328));old.uc.mem_write(NEXT,bytes(328))
        for table in (TABLE,NEXT):
            for index,ty,lo,hi,proj in [(0,type0 if table==TABLE else 2,2,5,9),(1,type1 if table==TABLE else 5,4,8,10)]:
                old.pointer(table+index*164+0x58,ty);old.uc.mem_write(table+index*164+0x98,struct.pack('<III',lo,hi,proj))
        old.pointer(got+offset,VARIABLE);old.pointer(VARIABLE,TABLE);old.uc.mem_write(OUT,struct.pack('<III',123,234,345))
        a,b,c=OUT,OUT+4,OUT+8
        if alias==1:c=b
        if alias==2:b=c=a
        if alias==3:b=a
        if alias==4:a,b,c=A+0x1074,A+0x1078,A+0x1070
        if alias==5:a,b,c=TABLE+0x9c,TABLE+0xa0,TABLE+0x98
        if alias==6:a,b,c=OUT+8,OUT,OUT+4
        if alias==7:c=a
        queries=[];stores=[0];dependencies=[]
        def observe(_,at,__,___):
            if at==0x3f9e18:
                instance=old.reg(0);id_value=old.reg(2);queries.append([0 if instance==I0 else 1,id_value]);count=len(queries)
                if count==1:
                    if mutation==1:old.uc.mem_write(INV+0x2e,b'\1')
                    if mutation==2:old.pointer(INV+0x14,SWAPPED)
                    if mutation==3:old.pointer(REF0,I1)
                    if mutation==4:old.pointer(I0+4,1)
                    if mutation==5:old.pointer(VARIABLE,NEXT)
                    if mutation==6:old.pointer(TABLE+0x58,5)
                    if mutation==7:old.pointer(A+0x1078,7)
                if count==2 and mutation==6:old.pointer(TABLE+0x58,2)
            elif at in (0x3a4d04,0x3a4d14,0x3a4d1c,0x3fff18,0x3fff20,0x3fff28):stores[0]+=1
            elif at in (0x3a4cd0,0x3a4d3c,0x3a4d34,0x3ffebc,0x400014,0x3fffa4,0x3fc6a8,0x3f9e08):
                dependencies.append([hex(at),old.reg(0)])
        hook=old.uc.hook_add(UC_HOOK_CODE,observe)
        start=(0x3a4cd0,0x3ffebc,0x3fffa4,0x3a4d3c)[op]
        value=old.invoke(start,[A if op in (0,3) else INV,a,b,c] if op<2 else [A if op==3 else INV]);old.uc.hook_del(hook)
        expected={'status':0,'value':value,'stores':stores[0],'outputs':words(OUT,3),'properties':words(A+0x1070,3),'row0':words(TABLE+0x98,3),'row1':words(TABLE+164+0x98,3),'current':old.uc.mem_read(INV+0x2e,1)[0],'instance0':get(I0+4),'queries':queries}
        actual=json.loads(subprocess.check_output([str(exe),*map(str,x)],text=True));assert actual==expected,(name,actual,expected)
        records.append({'case':name,'matched':True,'source_result':expected,'compiled_result':actual,'original_call_trace':dependencies})
    return {'validation':'PASS','comparisons':len(records),'mismatches':0,'results':records,
        'executed_scope':'All original Character100B/32B, ItemInventory116B/112B/4B, GetCurrentEquipSet32B (call argument1), GetItem44B and Character8B virtual thunk instructions execute. Genuine base/Character secondary vtables are used. Only global ItemTable capture timing is a controlled mutation fixture; no capability result, equip getter or GetItem return is mocked.',
        'source_aliases':'Three independent output pointers, exact aliases, property scalar aliases and item-row scalar aliases; original stores and fresh post-store loads execute directly. Invalid unchecked pointers/indices are excluded from original parity and tested as explicit port guards.',
        'native_wired':False}
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--compiler');parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=MODULE/'build/character-range-capability/host.exe');parser.add_argument('--report',type=Path,default=MODULE/'build/character-range-capability/validation.json');args=parser.parse_args()
    compiler=args.compiler or os.environ.get('CXX') or shutil.which('g++')
    if not compiler:parser.error('pass --compiler')
    exe=args.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    sources=[MODULE/'character_range_capability.cpp',MODULE/'character_ai_ranged_range.cpp',MODULE/'tests/character_range_capability.cpp']
    command=[compiler,'-std=c++17','-O1','-Wall','-Wextra','-Werror','-pedantic','-ffp-contract=off',*map(str,sources),'-o',str(exe)]
    build=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if build.returncode:raise RuntimeError(build.stdout+build.stderr)
    host=json.loads(subprocess.check_output([str(exe)],text=True));assert host['validation']=='PASS' and host['host_cases']==115
    comparison=oracle(args.original_elf.resolve(),exe)
    paths=sources+[MODULE/'character_range_capability.hpp',MODULE/'character_ai_ranged_range.hpp',MODULE/'character_combat_queries.hpp',MODULE/'character_ai_melee_range.hpp',MODULE/'character_ai_sight.hpp',MODULE/'character_enemy_retention.hpp',Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md')]
    report={'validation':'PASS','host_report':host,'original_arm_comparison':comparison,'original_sha256':SHA,'compiler_command':command,'native_wired':False,
        'source_sha256':{p.relative_to(ROOT).as_posix():hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'validation':'PASS','host_cases':host['host_cases'],'original_arm_cases':comparison['comparisons'],'mismatches':0}))
if __name__=='__main__':main()
