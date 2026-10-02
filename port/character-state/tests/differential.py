#!/usr/bin/env python3
"""Original PROPS_Set/Add/Int and GetInt versus owned source sheet state.

No buff groups in this routing test. Composition's separate test exercises real
original buff tree/deque traversal. Lua callback argument handling is untested.
"""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('property_test',ROOT/'../character-properties/tests/differential.py')
base=importlib.util.module_from_spec(spec);spec.loader.exec_module(base)
Sheet,Table=base.Sheet,base.Table
class State(c.Structure):_fields_=[(n,Sheet) for n in ('base','saved','gears','final')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
ADDRESSES=(0x3e07a0,0x3e0708,0x3e0808,0x3e0798)
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes()
    old=base.original.Original(a.original,a.oracle,raw);m=old.machine
    new=base.original.cpu.EngineCpu(a.arm64,True,base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32]
    lib.dh2_character_props_init.argtypes=[c.POINTER(Table),c.POINTER(State)]
    lib.dh2_character_props_write.argtypes=[c.POINTER(Table),c.POINTER(State),c.c_void_p,c.c_uint32,c.c_uint32,c.c_int32,c.c_uint32]
    lib.dh2_character_props_get_int.argtypes=[c.POINTER(State),c.c_uint32,c.POINTER(c.c_int32)]
    buf=c.create_string_buffer(raw);table=Table();state=State();assert not lib.dh2_property_open(c.byref(table),buf,len(raw))
    assert not lib.dh2_character_props_init(c.byref(table),c.byref(state))
    defaults=old.tables[0]['rows'][0];types=old.tables[0]['rows'][1]
    assert bytes(state)==struct.pack('<224i',*defaults)*4
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nv=new.data+0x100;ns=new.data+0x8000;no=new.data+0x1000
    new.uc.mem_write(np,raw);assert not new.call('dh2_property_open',[nv,np,len(raw)])
    new.uc.mem_write(ns-16,b'\xa5'*(c.sizeof(state)+32));assert not new.call('dh2_character_props_init',[nv,ns])
    assert bytes(state)==bytes(new.uc.mem_read(ns,c.sizeof(state)))
    owner=m.data+0x3000;offs=(8,0x38c,0x710,0xa94);rng=random.Random(20261002);calls=Counter()
    def prepare(id,type):
        for name in ('base','saved','gears','final'):
            s=getattr(state,name);s.values[:]=defaults
            s.values[id]=rng.choice([defaults[id],-2147483648,2147483647,-1,0,1,rng.randint(-100000,100000)])
        m.uc.mem_write(owner,b'\xa5'*0xe44)
        for name,off in zip(('base','saved','gears','final'),offs):m.uc.mem_write(owner+off+4,bytes(getattr(state,name)))
        header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header))
        new.uc.mem_write(ns,bytes(state));type_bytes=struct.pack('<i',type)
        m.uc.mem_write(old.character_pointer+900+4+4*id,type_bytes)
        c.memmove(c.addressof(buf)+900+4*id,type_bytes,4);new.uc.mem_write(np+900+4*id,type_bytes)
    def check(id,delta,op,label):
        before=bytes(m.uc.mem_read(owner,0xe44))
        old.invoke(ADDRESSES[op],[owner,id,delta])
        assert not lib.dh2_character_props_write(c.byref(table),c.byref(state),None,0,id,delta,op)
        assert not new.call('dh2_character_props_write',[nv,ns,0,0,id,delta&0xffffffff,op])
        expected=b''.join(bytes(m.uc.mem_read(owner+off+4,896)) for off in offs)
        assert expected==bytes(state)==bytes(new.uc.mem_read(ns,c.sizeof(state))),(label,id,op,delta)
        after=bytes(m.uc.mem_read(owner,0xe44));masked_before=bytearray(before);masked_after=bytearray(after)
        for off in offs:masked_before[off+4:off+900]=masked_after[off+4:off+900]=bytes(896)
        assert masked_before==masked_after
        assert bytes(new.uc.mem_read(ns-16,16))==bytes(new.uc.mem_read(ns+c.sizeof(state),16))==b'\xa5'*16
        expected_int=old.invoke(0x3df6e0,[owner,id,0]);value=c.c_int32()
        assert not lib.dh2_character_props_get_int(c.byref(state),id,c.byref(value))
        new.uc.mem_write(no,b'\xa5'*20);assert not new.call('dh2_character_props_get_int',[ns,id,no])
        assert expected_int==(value.value&0xffffffff)==struct.unpack('<I',new.uc.mem_read(no,4))[0]
        assert bytes(new.uc.mem_read(no+4,16))==b'\xa5'*16
        calls[label]+=1;calls['GetInt current final']+=1
    for id,type in enumerate(types):
        prepare(id,type)
        for op in range(4):check(id,rng.choice([-2147483648,2147483647,-1,0,1,rng.randint(-100000,100000)]),op,'actual cache type writes')
    for type in range(-1,64):
        for delta in (-2147483648,2147483647,257):
            prepare(36,type)
            for op in range(4):check(36,delta,op,'synthetic flag writes')
    extra=[]
    with a.original.open('rb') as f:
        elf=ELFFile(f);syms=list(elf.get_section_by_name('.dynsym').iter_symbols());loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for address in (*ADDRESSES,0x3df6e0,0x3dfe60):
            s=next(s for s in syms if s['st_value']==address);n=s['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address< s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);extra.append({'elf_address':hex(address),'size':n,'symbol':s.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    result={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),
            'test_sha256':sha(Path(__file__)),'cache_sha256':hashlib.sha256(raw).hexdigest(),'seed':20261002,'cases':dict(calls),'comparisons':sum(calls.values()),'mismatches':0,
            'function_evidence':extra+old.evidence,'whole_four_sheet_state_comparison':True,'original_reserved_fields_preserved':True,'output_guards':True,'stack_restoration':True,
            'dependency_model':'Actual original PROPS_Set/Add/SetInt/AddInt/GetInt(false), RecalcProperty and property helpers execute on empty buff containers. Source compares all four sheets and arithmetic shift output; defaults initialized from checked records by authored source ownership. Virtual reads, allocation/disposal and memcpy/memmove/memcmp are bounded models. Buff-inclusive write routing, original Character construction, Lua arguments/ReturnValues, derived base-stat calculations, Android integration and gameplay remain unfinished. Source ARM64 runs in Unicorn, without hardware execution.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='function_evidence'}))
if __name__=='__main__':main()
