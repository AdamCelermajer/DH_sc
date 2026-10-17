#!/usr/bin/env python3
"""Actual original random core and Rand callback versus source ARM64/host."""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('random_cpu',ROOT/'../animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
class State(c.Structure):_fields_=[('seeds',c.c_uint32*2),('counters',c.c_uint32*2)]
class Result(c.Structure):_fields_=[('count',c.c_uint32),('value',c.c_int32)]
ROWS=[(0x33ff90,196,'Random::GetRandom'),(0x38e578,148,'ordinary clone'),
      (0x393310,308,'GameObject::_Rand'),(0x38d798,16,'Value::getUInteger'),
      (0x31bbf0,144,'Value::getNumber'),(0x3b6cc8,16,'Character::_GetHitCount'),
      (0x3b6d78,40,'Character::_GetState'),(0x3c01ac,20,'SM_GetState'),
      (0x38ebf0,12,'GameObject::_GetName')]
class Dependencies(cpu.Dependencies):
    def call(self,m,name):
        if name in ('uidivmod','__aeabi_uidivmod'):
            numerator,denominator=m.reg(0),m.reg(1);assert denominator
            m.write_reg(0,numerator//denominator);m.write_reg(1,numerator%denominator)
            m.uc.reg_write(m.pc_reg,m.uc.reg_read(m.lr_reg))
        else:super().call(m,name)
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--runtime',type=Path)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest();assert sha(a.original)==cpu.ORIGINAL_SHA256
    evidence=[]
    with a.original.open('rb')as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address,size,name in ROWS:
            segment=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(segment['p_offset']+address-segment['p_vaddr']);evidence.append({'elf_address':hex(address),'size':size,'name':name,'sha256':hashlib.sha256(f.read(size)).hexdigest()})
    dep=Dependencies(a.oracle);old=cpu.EngineCpu(a.original,False,dep,{'functions':evidence});new=cpu.EngineCpu(a.arm64,True,dep,{'functions':[]})
    with a.original.open('rb')as f:
        elf=ELFFile(f)
        for section in elf.iter_sections():
            if section['sh_type']!='SHT_REL':continue
            symbols=elf.get_section(section['sh_link'])
            for rel in section.iter_relocations():
                if rel['r_info_type']==21:
                    symbol=symbols.get_symbol(rel['r_info_sym'])
                    if symbol['st_shndx']!='SHN_UNDEF':old.uc.mem_write(rel['r_offset'],struct.pack('<I',symbol['st_value']))
    host=c.CDLL(str(a.host.resolve()));host.dh2_random_next.argtypes=[c.POINTER(State),c.c_uint32,c.c_uint32];host.dh2_random_next.restype=c.c_int32
    host.dh2_random_callback.argtypes=[c.POINTER(State),c.POINTER(c.c_uint32),c.c_uint32,c.POINTER(c.c_float),c.POINTER(c.c_uint32),c.c_uint32,c.POINTER(Result)]
    addresses=[old.symbols[n]for n in ('_ZN6Random6s_seedE','_ZN6Random12s_syncedSeedE','_ZN6Random21s_debugRandomCountersE')]
    def store_state(s):
        old.uc.mem_write(addresses[0],bytes(s.seeds));old.uc.mem_write(addresses[2],bytes(s.counters))
    def read_state():return bytes(old.uc.mem_read(addresses[0],8))+bytes(old.uc.mem_read(addresses[2],8))
    ns=new.data+0x400;out=new.data+0x9000;seed_ptr=new.data+0x600
    rng=random.Random(20261002);bounds=[0,1,2,100,0x80000000,0xffffffff,0xdaf26b,0x7fffffff]+[rng.getrandbits(32)for _ in range(120)]
    checks=0
    for initial in (0,1,0xffffffff,0x80000000,0xdaf26a,0x12345678):
        state=State((initial,initial^0xa5a5a5a5),(0xffffffff,0xfffffffe));store_state(state);new.uc.mem_write(ns,bytes(state))
        for i,bound in enumerate(bounds):
            sync=(0,1,255)[i%3];expected=old.call(0x33ff90,[bound,sync])
            value=host.dh2_random_next(c.byref(state),bound,sync)&0xffffffff
            actual=new.call('dh2_random_next',[ns,bound,sync])&0xffffffff
            assert expected==value==actual,(bound,sync,expected,value,actual)
            assert read_state()==bytes(state)==bytes(new.uc.mem_read(ns,16));checks+=1
    arguments,vector,values,returns,character,online=[old.data+x for x in (0x400,0x500,0x1000,0x9000,0x3000,0x8000)]
    old.uc.mem_write(arguments,struct.pack('<II',0,vector));emitted=[];online_calls=[0]
    def capture(uc,address,size,unused):
        if address==0x7fd794:old.write_reg(0,online);online_calls[0]+=1
        elif address==0x37c8cc:
            assert old.reg(0)==returns;ptr=old.reg(1);data=bytes(old.uc.mem_read(ptr,64));emitted.append(('string',data.split(b'\0',1)[0]))
        else:
            assert old.reg(0)==returns;emitted.append(('integer',old.reg(1)))
        uc.reg_write(old.pc_reg,uc.reg_read(old.lr_reg))
    for address in (0x7fd794,0x37cb24,0x37c8cc):old.uc.hook_add(UC_HOOK_CODE,capture,begin=address,end=address)
    cases=[([],[]),([0],[3]),([1],[3]),([100],[3]),([-1.9],[3]),([4294967040.],[3]),
           ([0,100],[3,3]),([100,100],[3,3]),([100,1],[3,3]),([-100,500],[3,3]),
           ([4294967040.,0],[3,3]),([0],[0]),([1],[1]),([1,2],[3,1]),([1,2],[1,3]),
           ([1,2,3],[1,0,2]),(list(range(32)),[0]*32)]
    cases+=[([rng.uniform(-1000,1000000),rng.uniform(-1000,1000000)],[3,3])for _ in range(100)]
    callbacks=0
    runtime=None;lua_checks=0
    if a.runtime:
        lib=c.CDLL(str(a.runtime.resolve()));lib.dh2_lua_create.argtypes=[c.c_size_t];lib.dh2_lua_create.restype=c.c_void_p
        lib.dh2_lua_destroy.argtypes=[c.c_void_p];lib.dh2_lua_execute.argtypes=[c.c_void_p,c.c_void_p,c.c_size_t,c.c_uint32,c.c_void_p,c.c_size_t]
        lib.dh2_lua_import_character_properties.argtypes=[c.c_void_p,c.c_void_p,c.c_size_t,c.c_void_p,c.c_size_t]
        runtime=lib.dh2_lua_create(2*1024*1024);assert runtime;error=c.create_string_buffer(512)
        props=struct.pack('<I',3)+bytes(896)+struct.pack('<224i',*([8]*224))+bytes(896)+bytes(8)
        assert not lib.dh2_lua_import_character_properties(runtime,props,len(props),error,512)
        def execute(s):
            raw=s.encode('ascii');assert not lib.dh2_lua_execute(runtime,raw,len(raw),10000,error,512),error.value
        execute('DH2SeedRandom(4294967040);actor=DH2CreatePropertyState(2)')
    for mode in (0,1,255):
        old.uc.mem_write(online,bytes(5)+bytes([mode]));state=State((0xffffff00,123),(0xfffffffe,456));store_state(state)
        seed=c.c_uint32(0x80000000);old.uc.mem_write(character+0xfc,bytes(seed))
        for raw,tags in cases:
            args=(c.c_float*len(raw))(*raw);tagarray=(c.c_uint32*len(tags))(*tags);result=Result()
            packed=bytearray(112*len(raw))
            for i,(v,t)in enumerate(zip(args,tags)):struct.pack_into('<If',packed,112*i+4,t,v)
            old.uc.mem_write(vector,struct.pack('<II',values,values+len(packed)));old.uc.mem_write(values,bytes(packed)or bytes(4))
            new.uc.mem_write(ns,bytes(state));new.uc.mem_write(seed_ptr,bytes(seed));new.uc.mem_write(out,b'\xa5'*24)
            np,nt=new.data+0x1000,new.data+0x2000;new.uc.mem_write(np,bytes(args)or bytes(4));new.uc.mem_write(nt,bytes(tagarray)or bytes(4))
            emitted.clear();online_calls[0]=0;old.call(0x393310,[arguments,returns,character])
            assert not host.dh2_random_callback(c.byref(state),c.byref(seed),mode,args,tagarray,len(raw),c.byref(result))
            assert new.call('dh2_random_callback',[ns,seed_ptr,mode,np,nt,len(raw),out])==0
            assert emitted==([('integer',result.value&0xffffffff)]if result.count else [])
            assert read_state()==bytes(state)==bytes(new.uc.mem_read(ns,16))
            assert bytes(old.uc.mem_read(character+0xfc,4))==bytes(seed)==bytes(new.uc.mem_read(seed_ptr,4))
            assert bytes(new.uc.mem_read(out,8))==bytes(result)and bytes(new.uc.mem_read(out+8,16))==b'\xa5'*16
            assert bytes(new.uc.mem_read(np,len(bytes(args))))==bytes(args)and bytes(new.uc.mem_read(nt,len(bytes(tagarray))))==bytes(tagarray)
            assert bytes(old.uc.mem_read(values,len(packed)))==bytes(packed)
            assert online_calls[0]==(2 if result.count else 0);callbacks+=1
            if runtime and mode==0:
                parameters=','.join(repr(float(v))if t==3 else 'true'if t==1 else 'nil'for v,t in zip(args,tags))
                if emitted:assertion=f'local n=Rand({parameters});assert(n=={c.c_float(c.c_int32(emitted[0][1]).value).value!r})'
                else:assertion=f"assert(select('#',Rand({parameters}))==0)"
                execute(assertion);lua_checks+=1
    projections=[];node=character+0x2000;name=node+0x100
    old.uc.mem_write(name,b'diagnostic actor\0'+bytes(48));old.uc.mem_write(character+0x44,struct.pack('<I',name))
    for state_id in (-1,0,1,17,65535,-2147483648,2147483647):
        for hit in (0,1,255,32768,65535):
            old.uc.mem_write(character+0x51c,struct.pack('<I',0 if state_id==-1 else node));old.uc.mem_write(node,struct.pack('<i',state_id));old.uc.mem_write(character+0x14d0,struct.pack('<H',hit))
            for address,want in ((0x3b6cc8,hit),(0x3b6d78,state_id)):
                emitted.clear();old.call(address,[arguments,returns,character]);assert emitted==[('integer',want&0xffffffff)]
            emitted.clear();old.call(0x38ebf0,[arguments,returns,character]);assert emitted==[('string',b'diagnostic actor')];projections.append([state_id,hit])
            if runtime and c.c_float(state_id).value==state_id:
                execute(f"actor:SetCombatContext({state_id},{hit},'diagnostic actor');assert(actor:GetState()=={state_id} and actor:GetHitCount()=={hit} and actor:GetName()=='diagnostic actor')");lua_checks+=1
    if runtime:lib.dh2_lua_destroy(runtime)
    report={'complete_game':False,'original_interpreter_or_combat_dispatch_tested':False,'core_comparisons':checks,'callback_comparisons':callbacks,'character_projection_cases':len(projections),
            'mismatches':0,'stack_restoration':True,'input_preservation':True,'output_guards':True,'seed':20261002,
            'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),
            'source_sha256':{n:sha(ROOT/n)for n in ('random.c','random.h')},'test_sha256':sha(Path(__file__)),'function_evidence':evidence,'imports':old.import_calls,
            'scope':'Actual Random core, ordinary clone, Rand callback, Arguments/Value numeric conversion bodies execute. Defined ELF global relocations installed. Imported unsigned quotient/remainder uses an explicit mathematical model, with nonzero divisors. GetOnline is a fixed boundary fixture and ReturnValues pushes capture values; original online runtime and Lua VM are not exercised. Sequential draws cover both streams, zero bounds/counter wrap, upper/lower order, finite negative clamp, missing/extra/tag-rejected arguments, offline/online object seed and global state. GetState/HitCount/Name projections execute on explicit bounded object/node/string fixtures; full Character/state-machine ownership remains absent.'}
    if a.runtime:report.update({'runtime_sha256':sha(a.runtime),'owned_lua_callback_and_context_checks':lua_checks,'lua_scope':'Offline Rand original integer returns are compared after float32 Lua conversion; online remains standalone only. State inputs representable as float32 signed int and hit/name inputs compare original captured values. Authored seeding/context setters are controls, not original callbacks.','bridge_sha256':{n:sha(ROOT/n)for n in ('lua-bridge.c','../lua-character/bridge.c')}})
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('function_evidence','scope','imports')}))
if __name__=='__main__':main()
