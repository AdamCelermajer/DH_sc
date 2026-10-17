#!/usr/bin/env python3
"""Actual original numeric/boolean/nil bonus callbacks versus owned source Lua."""
import argparse,ctypes as c,hashlib,importlib.util,json,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
REPO=Path(__file__).resolve().parents[1]
def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
reader=module('loot_reader',REPO/'tools/trace_loot_tables.py')
fixtures=module('equipment_fixtures',REPO/'port/lua-runtime/tests/equipment_corpus.py')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','runtime','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();props,values=fixtures.property_fixture();raw=fixtures.item_fixture()
    old=reader.Original(a.original,a.oracle,props,raw);m=old.machine
    lib=c.CDLL(str(a.runtime.resolve()));lib.dh2_lua_create.argtypes=[c.c_size_t];lib.dh2_lua_create.restype=c.c_void_p
    lib.dh2_lua_destroy.argtypes=[c.c_void_p]
    for name in ('dh2_lua_import_character_properties','dh2_lua_import_loot_tables'):getattr(lib,name).argtypes=[c.c_void_p,c.c_void_p,c.c_size_t,c.c_void_p,c.c_size_t]
    lib.dh2_lua_execute.argtypes=[c.c_void_p,c.c_void_p,c.c_size_t,c.c_uint32,c.c_void_p,c.c_size_t]
    runtime=lib.dh2_lua_create(8*1024*1024);assert runtime;error=c.create_string_buffer(512)
    assert not lib.dh2_lua_import_character_properties(runtime,props,len(props),error,512)
    assert not lib.dh2_lua_import_loot_tables(runtime,raw,len(raw),error,512)
    def execute(source):
        source=source.encode('ascii');status=lib.dh2_lua_execute(runtime,source,len(source),10000,error,512);assert not status,error.value
    character=m.data+0x3000;owner=character+0x560;inventory=character+0x37c
    vectors,entries,slots,instances=[m.data+x for x in (0x6000,0x6100,0x6200,0x6400)]
    arguments,vector,args,returns=[m.data+x for x in (0x7000,0x7100,0x7200,0x9000)]
    m.uc.mem_write(arguments,struct.pack('<II',0,vector));emitted=[];counts=Counter()
    push_boolean=old.symbols['_ZN3sfc6script3lua12ReturnValues11pushBooleanEb']['st_value']
    def pushed(uc,address,size,unused):
        assert m.reg(0)==returns;emitted.append(('boolean'if address==push_boolean else 'integer',m.reg(1)))
        uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
    for address in (0x37cb24,push_boolean):m.uc.hook_add(UC_HOOK_CODE,pushed,begin=address,end=address)
    functions=[(0x3b7b28,'GetCritRatingBonus'),(0x3b7b84,'GetAttackRatingBonus'),(0x3b7acc,'GetDamageBonus'),(0x3b6fa4,'HasShield')]
    corpus=[([],[]),([0],[0]),([0],[1]),([1],[1]),([0.0],[3]),([-0.0],[3]),([1.0],[3]),([-1.0],[3]),([.25],[3]),([float('nan')],[3]),([float('inf')],[3]),([0,1],[1,1]),([1,0],[1,0])]
    def literal(value,tag):
        if tag==0:return 'nil'
        if tag==1:return str(bool(value)).lower()
        if value!=value:return '(0/0)'
        if value==float('inf'):return '(1/0)'
        return repr(value)
    samples=[]
    for configuration,ids in enumerate(([-1,-1,-1,-1,-1,-1],[-1,0,1,-1,2,0],[-1,2,0,-1,0,-1])):
        for set in range(2):
            m.uc.mem_write(character,bytes(0x1600));m.uc.mem_write(owner+4,struct.pack('<I',character));m.uc.mem_write(inventory+4,struct.pack('<I',character))
            m.uc.mem_write(owner+0xa98,struct.pack('<224i',*values));m.uc.mem_write(inventory+0x14,struct.pack('<I',vectors));m.uc.mem_write(inventory+0x2e,bytes([set]))
            for s in range(2):m.uc.mem_write(vectors+12*s,struct.pack('<3I',entries+12*s,entries+12*s+12,entries+12*s+12))
            for i,id in enumerate(ids):
                sp=slots+8*i;ip=instances+16*i;m.uc.mem_write(entries+4*i,struct.pack('<I',sp if id>=0 else 0));m.uc.mem_write(sp,struct.pack('<I',ip));m.uc.mem_write(ip+4,struct.pack('<i',id))
            source='p=DH2CreatePropertyState(2);'+''.join(f'p:EquipItem({i//3},{i%3},{id});'for i,id in enumerate(ids))+f'p:SelectEquipmentSet({set})'
            execute(source);before=bytes(m.uc.mem_read(character,0x1600))
            for address,name in functions:
                for vals,tags in corpus:
                    packed=bytearray(112*len(vals))
                    for i,(value,tag)in enumerate(zip(vals,tags)):
                        struct.pack_into('<I',packed,112*i+4,tag);struct.pack_into('<f',packed,112*i+8,float(value))
                    m.uc.mem_write(vector,struct.pack('<II',args,args+len(packed)));m.uc.mem_write(args,bytes(packed)or bytes(4));emitted.clear()
                    old.invoke(address,[arguments,returns,character]);parameters=','.join(literal(v,t)for v,t in zip(vals,tags))
                    if not emitted:assertion=f"assert(select('#',p:{name}({parameters}))==0)"
                    else:
                        assert len(emitted)==1;kind,value=emitted[0]
                        number=c.c_float(c.c_int32(value).value).value
                        expected=str(bool(value)).lower()if kind=='boolean'else repr(number)
                        assertion=f"assert(select('#',p:{name}({parameters}))==1 and p:{name}({parameters})=={expected})"
                    execute(assertion);counts[name]+=1
                    assert bytes(m.uc.mem_read(args,len(packed)))==bytes(packed) and bytes(m.uc.mem_read(character,0x1600))==before
                    samples.append({'configuration':configuration,'set':set,'method':name,'argument_tags':tags,'lua_assertion':assertion,'returns':[list(v)for v in emitted]})
    lib.dh2_lua_destroy(runtime);evidence=[]
    with a.original.open('rb')as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address in [*map(lambda row:row[0],functions),0x31bc80,0x37cb24,push_boolean]:
            s=next(s for s in old.symbols.values()if s['st_value']==address and s['st_info']['type']=='STT_FUNC');n=s['st_size']
            segment=next(s for s in loads if s['p_vaddr']<=address and address+n<=s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr'])
            evidence.append({'elf_address':hex(address),'size':n,'symbol':s.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    result={'complete_game':False,'original_lua_gameplay_equivalence_tested':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'source_runtime_sha256':sha(a.runtime),
            'test_sha256':sha(Path(__file__)),'property_fixture_sha256':hashlib.sha256(props).hexdigest(),'item_fixture_sha256':hashlib.sha256(raw).hexdigest(),
            'comparisons':sum(counts.values()),'cases':dict(counts),'mismatches':0,'input_preservation':True,'stack_restoration':True,'function_evidence':evidence,'assertions':samples,
            'scope':'Actual original three bonus callbacks, HasShield, numeric/boolean/nil Value.getBool and native inventory/property bodies execute on bounded synthetic equipment and property fixtures. ReturnValues pushes are capture boundaries; int-to-Lua float32 conversion is explicit. These emitted counts/typed values agree with the owned source Lua userdata. Nonzero numbers (including NaN/infinity), zero, nil, missing/extra args and both equipment sets are covered. Original Lua interpreter, string/pointer coercion, inventory and Character lifecycle, combat/gameplay are not executed.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('assertions','function_evidence')}))
if __name__=='__main__':main()
