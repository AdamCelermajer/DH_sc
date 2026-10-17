#!/usr/bin/env python3
"""Original numeric/boolean GetProp/SetProp callbacks versus source projection."""
import argparse,ctypes as c,hashlib,importlib.util,json,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('state_test',ROOT/'../character-state/tests/differential.py')
state_test=importlib.util.module_from_spec(spec);spec.loader.exec_module(state_test)
base=state_test.base;State,Table=state_test.State,state_test.Table
class Result(c.Structure):_fields_=[('count',c.c_uint32),('value',c.c_int32)]
class Dependencies(base.Dependencies):
    def call(self,m,name):
        if name=='__aeabi_f2iz':
            value=m.get_float('f',0);assert -2147483648<=value<2147483648
            m.write_reg(0,int(value)&0xffffffff);m.uc.reg_write(m.pc_reg,m.uc.reg_read(m.lr_reg));return
        super().call(m,name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes()
    # Select the explicit conversion model while constructing this isolated
    # original reader/callback CPU; restore the imported module immediately.
    dep=base.original.cpu.Dependencies;base.original.cpu.Dependencies=Dependencies
    try:old=base.original.Original(a.original,a.oracle,raw)
    finally:base.original.cpu.Dependencies=dep
    m=old.machine;new=base.original.cpu.EngineCpu(a.arm64,True,Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32]
    lib.dh2_character_props_init.argtypes=[c.POINTER(Table),c.POINTER(State)]
    lib.dh2_character_property_method.argtypes=[c.POINTER(Table),c.POINTER(State),c.c_uint32,c.POINTER(c.c_float),c.POINTER(c.c_uint32),c.c_uint32,c.POINTER(Result)]
    buf=c.create_string_buffer(raw);table=Table();state=State();assert not lib.dh2_property_open(c.byref(table),buf,len(raw))
    assert not lib.dh2_character_props_init(c.byref(table),c.byref(state))
    defaults=old.tables[0]['rows'][0]
    character=m.data+0x3000;owner=character+0x560;offs=(8,0x38c,0x710,0xa94)
    m.uc.mem_write(character,b'\xa5'*0x1600)
    for off in offs:m.uc.mem_write(owner+off+4,struct.pack('<224i',*defaults))
    header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header))
    # Original constructor normally resets this global temporary sheet. Use the
    # actual reset body to establish its serialized baseline for true queries.
    temp=old.symbols['_ZN14CharProperties6s_tempE']['st_value'];old.invoke(0x3def34,[owner,temp])
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nv=new.data+0x100;ns=new.data+0x8000
    na,nt,nr=[new.data+x for x in (0x1000,0x2000,0x3000)]
    new.uc.mem_write(np,raw);assert not new.call('dh2_property_open',[nv,np,len(raw)])
    new.uc.mem_write(ns-16,b'\xa5'*(c.sizeof(state)+32));assert not new.call('dh2_character_props_init',[nv,ns])
    arguments,vector,values,returns=[m.data+x for x in (0x6000,0x6100,0x7000,0x9000)]
    m.uc.mem_write(arguments,struct.pack('<II',0,vector));emitted=[];calls=Counter()
    def pushed(uc,address,size,unused):
        assert m.reg(0)==returns;emitted.append(m.reg(1));uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
    m.uc.hook_add(UC_HOOK_CODE,pushed,begin=0x37cb24,end=0x37cb24)
    def run(op,args,tags):
        assert len(args)==len(tags) and len(args)<=32
        floats=(c.c_float*len(args))(*args);types=(c.c_uint32*len(tags))(*tags);result=Result()
        original=bytearray(112*len(args))
        for k,(value,tag) in enumerate(zip(floats,tags)):
            bits=struct.unpack('<I',struct.pack('<f',value))[0] if tag==3 else int(value) if tag==1 else 0
            struct.pack_into('<II',original,k*112+4,tag,bits)
        m.uc.mem_write(vector,struct.pack('<II',values,values+len(original)))
        m.uc.mem_write(values,bytes(original) or bytes(4));emitted.clear()
        old.invoke((0x3b9d8c,0x3b7eac)[op],[arguments,returns,character])
        assert not lib.dh2_character_property_method(c.byref(table),c.byref(state),op,floats,types,len(args),c.byref(result))
        new.uc.mem_write(na,bytes(floats) or bytes(4));new.uc.mem_write(nt,bytes(types) or bytes(4));new.uc.mem_write(nr,b'\xa5'*24)
        assert not new.call('dh2_character_property_method',[nv,ns,op,na,nt,len(args),nr])
        assert bytes(result)==bytes(new.uc.mem_read(nr,8))
        assert emitted==([result.value&0xffffffff] if result.count else []),(op,args,tags,emitted,result.count,result.value)
        expected=b''.join(bytes(m.uc.mem_read(owner+off+4,896)) for off in offs)
        assert expected==bytes(state)==bytes(new.uc.mem_read(ns,c.sizeof(state)))
        assert bytes(m.uc.mem_read(values,len(original)))==bytes(original)
        assert bytes(new.uc.mem_read(na,c.sizeof(floats)))==bytes(floats) and bytes(new.uc.mem_read(nt,c.sizeof(types)))==bytes(types)
        assert bytes(new.uc.mem_read(nr+8,16))==bytes(new.uc.mem_read(ns-16,16))==bytes(new.uc.mem_read(ns+c.sizeof(state),16))==b'\xa5'*16
        calls['GetProp' if op==0 else 'SetProp']+=1
    for id in range(224):
        for delta in (-123.75,257.9,2147483520.):run(1,[id+.75,delta],[3,3])
        for args,tags in [([id+.75],[3]),([id,0],[3,1]),([id,1],[3,1]),([id,1,99],[3,3,3])]:run(0,args,tags)
    for args,tags in [([],[]),([-1.2],[3]),([-.9],[3]),([224],[3]),([223.9],[3]),([1],[1]),([1],[4]),([1,0],[3,0]),([1,1,99],[3,1,3])]:run(0,args,tags)
    # Original empty SetProp enters its diagnostic path; source's guard is
    # tested by safety, while original comparison starts at one argument.
    for args,tags in [([1],[3]),([1],[1]),([1,2],[1,3]),([1,2],[3,1]),([-1,2],[3,3]),([224,2],[3,3]),([-.9,-2147483648],[3,3]),([1,257.9,1],[3,3,1]),([1,257.9,99],[3,3,3])]:run(1,args,tags)
    extra=[]
    with a.original.open('rb') as f:
        elf=ELFFile(f);syms=list(elf.get_section_by_name('.dynsym').iter_symbols());loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for address in (0x3b9d8c,0x3b7eac,0x37baf8,0x38d798,0x31bbf0,0x31bc80,0x8be2a0):
            s=next(s for s in syms if s['st_value']==address);n=s['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address< s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);extra.append({'elf_address':hex(address),'size':n,'symbol':s.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    r={'complete_game':False,'lua_userdata_binding_tested':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),
       'test_sha256':sha(Path(__file__)),'cache_sha256':hashlib.sha256(raw).hexdigest(),'cases':dict(calls),'comparisons':sum(calls.values()),'mismatches':0,
       'function_evidence':extra+old.evidence,'whole_four_sheet_state_comparison':True,'input_preservation':True,'output_guards':True,'stack_restoration':True,
       'dependency_model':'Actual original GetProp/SetProp, Arguments indexing, numeric/boolean Value accessors, unsigned float conversion and native property/calculation bodies execute. ReturnValues integer push is a capture boundary; signed finite float-to-int truncation, stream and allocation/copy dependencies are explicit models. Global s_temp is reset through the actual original body to serialized defaults. Pointer overloads, unsafe casts, empty original SetProp diagnostic path, buff-inclusive callbacks, original Lua interpreter and source Lua userdata/APK integration remain untested. Source ARM64 runs in Unicorn, without hardware execution.'}
    a.report.write_text(json.dumps(r,indent=2)+'\n');print(json.dumps({k:v for k,v in r.items() if k!='function_evidence'}))
if __name__=='__main__':main()
