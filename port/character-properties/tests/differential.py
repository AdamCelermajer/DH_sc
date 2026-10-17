#!/usr/bin/env python3
"""Actual original readers and property operations versus source ARM64/host."""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from collections import Counter
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('original_properties',REPO/'tools/trace_character_properties.py')
original=importlib.util.module_from_spec(spec);spec.loader.exec_module(original)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Sheet(c.Structure):_fields_=[('values',c.c_int32*224)]
class Table(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*3),('offsets',c.c_uint32*3)]
class Dependencies(original.cpu.Dependencies):
    def call(self,m,name):
        if name in ('memmove','memcmp'):
            dst,src,count=[m.reg(i) for i in range(3)];assert count<=4*1024*1024
            bb=bytes(m.uc.mem_read(src,count))
            if name=='memmove':
                if count:m.uc.mem_write(dst,bb)
                m.write_reg(0,dst)
            else:
                aa=bytes(m.uc.mem_read(dst,count));m.write_reg(0,(0 if aa==bb else -1 if aa<bb else 1)&0xffffffff)
            m.uc.reg_write(m.pc_reg,m.uc.reg_read(m.lr_reg));return
        super().call(m,name)
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();path=a.cache/'data/pydata/character_properties_pyarray.bin';raw=path.read_bytes()
    old=original.Original(a.original,a.oracle,raw);m=old.machine
    new=original.cpu.EngineCpu(a.arm64,True,Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));tp=c.POINTER(Table);sp=c.POINTER(Sheet);ip=c.POINTER(c.c_int32);u=c.c_uint32;i=c.c_int32
    specs={'open':[tp,c.c_void_p,u],'load':[tp,u,sp],'get':[sp,u,ip],'set':[sp,u,i],
           'default':[tp,u,ip],'type':[tp,u,ip],'reset':[tp,sp],'is_set':[tp,sp,u,ip],'add':[tp,sp,u,i]}
    for n,args in specs.items():
        f=getattr(lib,'dh2_property_'+n);f.argtypes=args;f.restype=u
    buffer=c.create_string_buffer(raw);table=Table();sheet=Sheet();value=c.c_int32()
    assert not lib.dh2_property_open(c.byref(table),buffer,len(raw))
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nv=new.data+0x100;ns=new.data+0x1000;no=new.data+0x2000
    new.uc.mem_write(np,raw);assert not new.call('dh2_property_open',[nv,np,len(raw)])
    os=m.data+0x1000;calls=Counter()
    def check_sheet(label):
        expected=bytes(m.uc.mem_read(os+4,896))
        assert expected==bytes(sheet)==bytes(new.uc.mem_read(ns,896)),label
        assert bytes(m.uc.mem_read(os,4))==b'\xa5'*4 and bytes(m.uc.mem_read(os+900,16))==b'\xa5'*16
        assert bytes(new.uc.mem_read(ns-16,16))==bytes(new.uc.mem_read(ns+896,16))==b'\xa5'*16
    def check_value(op,expected,id):
        if op=='get':host=[c.byref(sheet),id,c.byref(value)];arm=[ns,id,no]
        elif op=='is_set':host=[c.byref(table),c.byref(sheet),id,c.byref(value)];arm=[nv,ns,id,no]
        else:host=[c.byref(table),id,c.byref(value)];arm=[nv,id,no]
        new.uc.mem_write(no,b'\xa5'*20)
        assert not getattr(lib,'dh2_property_'+op)(*host)
        assert not new.call('dh2_property_'+op,arm)
        assert (value.value&0xffffffff)==expected==struct.unpack('<I',new.uc.mem_read(no,4))[0],(op,id,expected,value.value)
        assert bytes(new.uc.mem_read(no+4,16))==b'\xa5'*16
        calls[op]+=1
    m.uc.mem_write(os,b'\xa5'*916);new.uc.mem_write(ns-16,b'\xa5'*928)
    # Every source-loaded field is compared to the actual original reader's
    # record destination, including sentinel values and defaults/types.
    for row,expected in enumerate(old.tables[0]['rows']):
        assert not lib.dh2_property_load(c.byref(table),row,c.byref(sheet))
        assert not new.call('dh2_property_load',[nv,row,ns])
        packed=struct.pack('<224i',*expected)
        assert packed==bytes(sheet)==bytes(new.uc.mem_read(ns,896)),row
        calls['record_fields']+=224
    for id in range(224):
        check_value('default',old.invoke(0x3def10,[0,id]),id)
        check_value('type',old.invoke(0x3deed8,[0,id]),id)
    rng=random.Random(20261002)
    old.invoke(0x3df250,[0,os,447]);check_sheet('initial last-row sheet')
    for id in range(224):
        initial=rng.choice([old.tables[0]['rows'][0][id],-2147483648,2147483647,rng.randint(-100000,100000)])
        old.invoke(0x3deca0,[0,os,id,initial])
        assert not lib.dh2_property_set(c.byref(sheet),id,initial)
        assert not new.call('dh2_property_set',[ns,id,initial&0xffffffff]);calls['set']+=1
        check_sheet(('set',id))
        check_value('get',old.invoke(0x3dedb4,[0,os,id]),id)
        check_value('is_set',old.invoke(0x3df114,[0,os,id]),id)
        for delta in (rng.choice([-2147483648,2147483647,-1,1,rng.randint(-50000,50000)]),1):
            old.invoke(0x3df140,[0,os,id,delta])
            assert not lib.dh2_property_add(c.byref(table),c.byref(sheet),id,delta)
            assert not new.call('dh2_property_add',[nv,ns,id,delta&0xffffffff]);calls['add']+=1
            check_sheet(('add',id,delta));check_value('get',old.invoke(0x3dedb4,[0,os,id]),id)
    for row in (0,1,2,100,447):
        old.invoke(0x3df250,[0,os,row]);assert not lib.dh2_property_load(c.byref(table),row,c.byref(sheet))
        assert not new.call('dh2_property_load',[nv,row,ns]);check_sheet(('load',row));calls['load']+=1
    old.invoke(0x3def34,[0,os]);assert not lib.dh2_property_reset(c.byref(table),c.byref(sheet))
    assert not new.call('dh2_property_reset',[nv,ns]);check_sheet('reset');calls['reset']+=1
    assert bytes(buffer.raw[:len(raw)])==bytes(new.uc.mem_read(np,len(raw)))==raw
    report={'complete_game':False,'scripts_executed':False,'comparison':'Actual original ARM32 readers/property operations versus compiled source ARM64 and host; exact integer bits and whole-sheet state.',
            'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),
            'test_sha256':sha(Path(__file__)),'cache_sha256':sha(path),'seed':20261002,'calls':dict(calls),'mismatches':0,
            'function_evidence':old.evidence,'input_preservation':True,'output_guards':True,'stack_restoration':True,
            'source_import_calls':new.import_calls,
            'dependency_model':'Original three array/four nested record/primitive readers, property get/set/default/type/is-set/add/reset/load execute. Virtual stream, bounded allocation/disposal and memcpy/memmove/memcmp are controlled models. Invalid original debug/fatal paths, aggregate character sheets, entities, scripts, combat and gameplay are not exercised. Source ARM64 runs in Unicorn; no hardware claim.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='function_evidence'}))
if __name__=='__main__':main()
