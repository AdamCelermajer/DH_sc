#!/usr/bin/env python3
"""Numeric-only original Lua callbacks vs portable ARM64/host projection."""
import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path
import random
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('callback_cpu',ROOT/'../animation-timeline/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
ROWS=[(0x37ebc4,80,'ToFixed'),(0x37ee84,184,'FromFixed'),(0x37e1a4,308,'MulFixed'),
      (0x37e068,316,'DivFixed'),(0x37f814,100,'BitNot'),(0x37f750,196,'BitXOr'),
      (0x37e9ec,472,'BitAnd'),(0x37e814,472,'BitOr'),(0x37ee80,4,'Trace')]
class Result(c.Structure):_fields_=[('count',c.c_uint32),('integer',c.c_int32),('number',c.c_float)]
class Dependencies(cpu.TimelineDependencies):
    def call(self,machine,name):
        if name=='__aeabi_idiv':
            aa,bb=[c.c_int32(machine.reg(i)).value for i in range(2)]
            assert bb and not(aa==-2147483648 and bb==-1)
            quotient=abs(aa)//abs(bb);quotient=-quotient if (aa<0)!=(bb<0)else quotient
            machine.write_reg(0,quotient&0xffffffff)
            machine.uc.reg_write(machine.pc_reg,machine.uc.reg_read(machine.lr_reg))
        else:super().call(machine,name)
def main():
    p=argparse.ArgumentParser()
    for name in ('original','oracle','host','arm64','report'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    assert sha(a.original)==cpu.ORIGINAL_SHA256;evidence=[]
    with a.original.open('rb')as stream:
        elf=ELFFile(stream);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address,size,name in ROWS:
            segment=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            stream.seek(segment['p_offset']+address-segment['p_vaddr'])
            evidence.append({'elf_address':f'0x{address:08x}','size':size,'name':name,
                             'sha256':hashlib.sha256(stream.read(size)).hexdigest()})
    dependencies=Dependencies(a.oracle)
    old=cpu.EngineCpu(a.original,False,dependencies,{'functions':evidence})
    new=cpu.EngineCpu(a.arm64,True,dependencies,{'functions':evidence})
    host=c.CDLL(str(a.host.resolve()));fn=host.dh2_lua_numeric
    fn.argtypes=[c.c_uint32,c.POINTER(c.c_float),c.c_uint32,c.POINTER(Result)];fn.restype=c.c_uint32
    arguments,vector,values,returns=[old.data+x for x in (0x400,0x600,0x1000,0x9000)]
    source,out=new.data+0x1000,new.data+0x5000
    emitted=[]
    def pushed(uc,address,size,unused):
        assert old.reg(0)==returns
        emitted.append(('integer'if address==0x37cb24 else 'number',old.reg(1)))
        uc.reg_write(old.pc_reg,uc.reg_read(old.lr_reg))
    for address in (0x37cb24,0x37ccbc):old.uc.hook_add(UC_HOOK_CODE,pushed,begin=address,end=address)
    old.uc.mem_write(arguments,struct.pack('<II',0,vector))
    rng=random.Random(20261002);checks=0;counts={}
    cases=[[],[0.],[-0.],[-1.9],[8388608.],[16777216.],[-2147483648.],
           [512.,384.],[65536.,65536.],[8388608.,256.],[-1.,-256.],
           [1.,512.,3.,4.,5.,6.,7.,8.],[7.,-512.,3.],list(range(256))]
    cases +=[[rng.uniform(-100000,100000),rng.choice((-1,1))*rng.uniform(512,100000),
              *[rng.uniform(-100000,100000)for _ in range(rng.randrange(7))]]for _ in range(250)]
    bits=lambda value:struct.unpack('<I',struct.pack('<f',value))[0]
    for operation,(address,_,name)in enumerate(ROWS):
        counts[name]=0
        for raw in cases:
            array=(c.c_float*len(raw))(*raw);result=Result()
            status=fn(operation,array,len(raw),c.byref(result))
            if status:assert operation==3;continue # Authored invalid division guard, no original UB.
            payload=bytes(array);new.uc.mem_write(source,payload or b'\0'*4)
            new.uc.mem_write(out,b'\xa5'*28)
            assert new.call('dh2_lua_numeric',[operation,source,len(raw),out])==0
            assert bytes(new.uc.mem_read(out,12))==bytes(result)
            assert bytes(new.uc.mem_read(out+12,16))==b'\xa5'*16
            assert bytes(new.uc.mem_read(source,len(payload)))==payload
            original_values=bytearray(112*len(raw))
            for i,value in enumerate(array):struct.pack_into('<II',original_values,112*i+4,3,bits(value))
            old.uc.mem_write(vector,struct.pack('<II',values,values+len(original_values)))
            old.uc.mem_write(values,bytes(original_values)or b'\0'*4);emitted.clear()
            old.call(address,[arguments,returns,0])
            expected=[]
            if result.count:expected.append(('integer',result.integer&0xffffffff))
            if result.count==2:expected.append(('number',bits(result.number)))
            assert emitted==expected,(name,raw,emitted,expected)
            assert bytes(old.uc.mem_read(values,len(original_values)))==bytes(original_values)
            checks+=1;counts[name]+=1
    result={'complete_game':False,'lua_bridge_runtime_tested_here':False,
            'comparison':'Original ARM32 numeric callback outputs vs compiled ARM64/host numeric-only projection: exact integers and float bits',
            'checks':checks,'per_callback':counts,'mismatches':0,'numeric_only':True,
            'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),
            'function_evidence':evidence,'source_sha256':{name:sha(ROOT/name)for name in ('numeric.c','numeric.h')},
            'test_sha256':sha(Path(__file__)),'input_preservation':True,'output_guards':True,'stack_restoration':True,
            'import_calls':old.import_calls,'seed':20261002,
            'dependency_model':'Actual callback, Arguments indexing and numeric Value getters execute; ReturnValues integer/number push boundaries capture output without running allocation; arithmetic imports use host model; defined signed division model excludes zero and INT_MIN/-1'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
