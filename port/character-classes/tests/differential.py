#!/usr/bin/env python3
"""Actual original class application versus host and compiled source ARM64."""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('class_reader',REPO/'tools/trace_character_classes.py')
reader=importlib.util.module_from_spec(spec);spec.loader.exec_module(reader)
spec=importlib.util.spec_from_file_location('state_test',ROOT/'../character-state/tests/differential.py')
state_test=importlib.util.module_from_spec(spec);spec.loader.exec_module(state_test)
State,Table,Sheet=state_test.State,state_test.Table,state_test.Sheet
class Classes(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('count',c.c_uint32)]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
FUNCTIONS=(0x3e2e20,0x3e3014,0x3e2d4c,0x3e2d04,0x3e2cc0,0x3e2c78,0x3e2c10,0x3e2bdc,0x3e2bcc,0x3e2bd0,0x3dfe60)
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();pr=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes();cr=(a.cache/'data/pydata/character_classes_pyarray.bin').read_bytes()
    old=reader.Original(a.original,a.oracle,pr,cr);m=old.machine
    new=reader.properties.cpu.EngineCpu(a.arm64,True,state_test.base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32]
    lib.dh2_class_open.argtypes=[c.POINTER(Classes),c.c_void_p,c.c_uint32]
    lib.dh2_class_apply.argtypes=[c.POINTER(Classes),c.POINTER(Table),c.POINTER(State),c.c_uint32,c.POINTER(Sheet),c.c_int32,c.c_uint32]
    pb=c.create_string_buffer(pr);pt=Table();cb=c.create_string_buffer(cr);ct=Classes();s=State();temp=Sheet()
    assert not lib.dh2_property_open(c.byref(pt),pb,len(pr)) and not lib.dh2_class_open(c.byref(ct),cb,len(cr))
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nc=new.data+0x80000
    pv,cv,ns,nt=[new.data+x for x in (0x100,0x200,0x1000,0x3000)]
    new.uc.mem_write(np,pr);new.uc.mem_write(nc,cr)
    assert not new.call('dh2_property_open',[pv,np,len(pr)]) and not new.call('dh2_class_open',[cv,nc,len(cr)])
    owner=m.data+0x3000;offs=(8,0x38c,0x710,0xa94);ot=old.symbols['_ZN14CharProperties6s_tempE']['st_value']
    rng=random.Random(20261002);cases=Counter();entries=Counter()
    def entered(uc,address,size,unused):entries[hex(address)]+=1
    for address in FUNCTIONS:m.uc.hook_add(UC_HOOK_CODE,entered,begin=address,end=address)
    def prepare():
        for name in ('base','saved','gears','final'):
            sheet=getattr(s,name)
            for id,d in enumerate(old.tables[0]['rows'][0]):sheet.values[id]=rng.choice([d,-2147483648,2147483647,-1,0,1,rng.randint(-100000,100000)])
        for id,d in enumerate(old.tables[0]['rows'][0]):temp.values[id]=rng.choice([d,-2147483648,2147483647,rng.randint(-100000,100000)])
        m.uc.mem_write(owner,b'\xa5'*0xe44)
        for name,off in zip(('base','saved','gears','final'),offs):m.uc.mem_write(owner+off+4,bytes(getattr(s,name)))
        header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header));m.uc.mem_write(ot,b'\xa5'*900);m.uc.mem_write(ot+4,bytes(temp))
        new.uc.mem_write(ns-16,b'\xa5'*(c.sizeof(s)+32));new.uc.mem_write(ns,bytes(s));new.uc.mem_write(nt-16,b'\xa5'*(c.sizeof(temp)+32));new.uc.mem_write(nt,bytes(temp))
    def check(id,target,flag,label):
        prepare();before=bytes(m.uc.mem_read(owner,0xe44))
        original_target=ot if target==4 else owner+offs[target]
        old.invoke(0x3e2e20,[owner,original_target,id,flag])
        status=lib.dh2_class_apply(c.byref(ct),c.byref(pt),c.byref(s),target,c.byref(temp),id,flag);assert not status,(label,id,target,flag,status)
        assert not new.call('dh2_class_apply',[cv,pv,ns,target,nt,id&0xffffffff,flag])
        expected=b''.join(bytes(m.uc.mem_read(owner+off+4,896))for off in offs)
        assert expected==bytes(s)==bytes(new.uc.mem_read(ns,c.sizeof(s))),(label,id,target,flag)
        expected_temp=bytes(m.uc.mem_read(ot+4,896));assert expected_temp==bytes(temp)==bytes(new.uc.mem_read(nt,896)),(label,id,target,flag,'temp')
        after=bytes(m.uc.mem_read(owner,0xe44));b,a2=bytearray(before),bytearray(after)
        for off in offs:b[off+4:off+900]=a2[off+4:off+900]=bytes(896)
        assert b==a2 and bytes(m.uc.mem_read(ot,4))==b'\xa5'*4
        for address,n in ((ns,c.sizeof(s)),(nt,896)):assert bytes(new.uc.mem_read(address-16,16))==bytes(new.uc.mem_read(address+n,16))==b'\xa5'*16
        cases[label]+=1
    original_class_bytes=bytes(m.uc.mem_read(old.class_pointer,old.class_count*12));nested=[(old.word(old.class_pointer+i*12+8),old.word(old.class_pointer+i*12+4)*24)for i in range(old.class_count)]
    nested_before=[bytes(m.uc.mem_read(ptr,n))if n else b''for ptr,n in nested]
    for id in range(old.class_count):
        for target in range(5):
            for flag in range(2):check(id,target,flag,'actual classes')
        if id%40==0:print('classes',id,flush=True)
    assert bytes(m.uc.mem_read(old.class_pointer,old.class_count*12))==original_class_bytes
    assert [bytes(m.uc.mem_read(ptr,n))if n else b''for ptr,n in nested]==nested_before
    assert bytes(pb)[:len(pr)]==bytes(new.uc.mem_read(np,len(pr)))==pr
    assert bytes(cb)[:len(cr)]==bytes(new.uc.mem_read(nc,len(cr)))==cr
    # Synthetic lists use the exact original 12-byte class and 24-byte rule layouts.
    fake=m.data+0x7000;rules=m.data+0x7100
    m.uc.mem_write(old.symbols['_ZN6Arrays10ClassTable4sizeE']['st_value'],struct.pack('<I',1))
    m.uc.mem_write(old.symbols['_ZN6Arrays10ClassTable7membersE']['st_value'],struct.pack('<I',fake))
    def install(rows):
        nonlocal cb,ct
        raw=struct.pack('<II',1,len(rows))+b''.join(struct.pack('<5i',*r)for r in rows)
        cb=c.create_string_buffer(raw);ct=Classes();assert not lib.dh2_class_open(c.byref(ct),cb,len(raw))
        new.uc.mem_write(nc,raw);assert not new.call('dh2_class_open',[cv,nc,len(raw)])
        m.uc.mem_write(fake,struct.pack('<3I',0xa5a5a5a5,len(rows),rules))
        m.uc.mem_write(rules,b''.join(struct.pack('<I5i',0xa5a5a5a5,*r)for r in rows))
    synthetic=[]
    for op in range(10):
        for variant in range(6):
            field=36;a2=19 if op in (4,5,6)else[-666,-2147483648,2147483647,-1,0,257][variant]
            row=[field,op,a2,19,[-2147483648,-1,0,1,256,2147483647][variant]]
            if op==0:row=[-1,0,-2,1,4096]
            synthetic.append([row,[36,7,512,0,0]])
    synthetic+=[[[36,-1,0,0,0],[36,7,512,0,0]],[[36,10,0,0,0],[36,7,512,0,0]]]
    for rows in synthetic:
        install(rows)
        for target in range(5):
            for flag in range(2):check(0,target,flag,'synthetic opcode/overflow/early return')
    for id in (-2147483648,-1,1,4096,2147483647):
        for target in range(5):
            for flag in range(2):check(id,target,flag,'invalid class original no-op')
    evidence=[]
    with a.original.open('rb')as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address in FUNCTIONS:
            symbol=next(s for s in old.symbols.values()if s['st_value']==address and s['st_info']['type']=='STT_FUNC');n=symbol['st_size']
            seg=next(s for s in loads if s['p_vaddr']<=address and address+n<=s['p_vaddr']+s['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr'])
            evidence.append({'elf_address':hex(address),'size':n,'symbol':symbol.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    result={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),
            'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
            'class_cache_sha256':hashlib.sha256(cr).hexdigest(),'property_cache_sha256':hashlib.sha256(pr).hexdigest(),
            'class_count':260,'rule_count':1659,'cases':dict(cases),'comparisons':sum(cases.values()),'mismatches':0,
            'function_entries':dict(entries),'function_evidence':evidence+old.class_evidence,
            'whole_four_sheet_and_temporary_comparison':True,'input_preservation':True,'original_reserved_fields_preserved':True,'output_guards':True,'stack_restoration':True,
            'scope':'Actual original class application, group traversal, linear/clamp/scale/add/set helpers and property recalculation execute with empty buffs. All classes across five destination sheets and both final-source flags, plus authored opcode/overflow/early-return/no-op fixtures. Original allocation/stream/copy dependencies are models. Cyclic original classes, unsafe property indices, buff-inclusive application, original Character construction, complete derived-stat lifecycle, Lua class binding and APK integration are untested. Source ARM64 executes in Unicorn, not hardware.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('function_evidence','function_entries')}))
if __name__=='__main__':main()
