#!/usr/bin/env python3
"""Execute the original field-name initializer and registered Structs lookups.

Low-level allocation, libc and exit-handler registration are explicit models.
No object records, original scripts or gameplay execute.
"""
import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

REPO=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('struct_cpu',REPO/'port/animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    p=argparse.ArgumentParser()
    for name in ('original','registrations','report'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();assert sha(a.original)==cpu.ORIGINAL_SHA256
    assert sha(a.registrations)=='d72e8b22b1d2fa5e58c60306535eed77c9d3079d3f5f188603fed534f7178b4d'
    registrations=json.loads(a.registrations.read_text())
    bindings=[r for r in registrations['class_registrations']if r['namespace']=='Structs']
    heap_base=0x6000000;heap_end=heap_base+0x4000000;heap=heap_base
    calls=Counter();exit_handlers=[]
    def string(machine,pointer):
        raw=bytes(machine.uc.mem_read(pointer,256));end=raw.find(b'\0');assert end>=0
        return raw[:end]
    def allocate(machine,count):
        nonlocal heap
        assert 0<count<=1024*1024 and heap+count+32<heap_end
        pointer=heap;heap=(heap+count+31)&~15
        machine.uc.mem_write(pointer,bytes(count));return pointer
    class Dependencies:
        def call(self,machine,name):
            calls[name]+=1
            if name=='strlen':machine.write_reg(0,len(string(machine,machine.reg(0))))
            elif name in ('memcpy','memmove'):
                dest,src,count=[machine.reg(i)for i in range(3)];assert count<=1024*1024
                if count:machine.uc.mem_write(dest,bytes(machine.uc.mem_read(src,count)))
                machine.write_reg(0,dest)
            elif name=='strcmp':
                aa,bb=[string(machine,machine.reg(i))for i in range(2)]
                machine.write_reg(0,(0 if aa==bb else -1 if aa<bb else 1)&0xffffffff)
            elif name=='__aeabi_atexit':
                exit_handlers.append([machine.reg(i)for i in range(3)]);machine.write_reg(0,0)
            elif name=='_Znwj':machine.write_reg(0,allocate(machine,machine.reg(0)))
            else:raise AssertionError('Unmodeled dependency: '+name)
            machine.uc.reg_write(machine.pc_reg,machine.uc.reg_read(machine.lr_reg))
    with a.original.open('rb')as f:
        elf=ELFFile(f);symbols={}
        for section_name in ('.dynsym','.symtab'):
            for s in elf.get_section_by_name(section_name).iter_symbols():
                if s['st_shndx']!='SHN_UNDEF':symbols[s.name]=s
        machine=cpu.EngineCpu(a.original,False,Dependencies(),{'functions':[]})
        for section in elf.iter_sections():
            if section['sh_type']!='SHT_REL':continue
            table=elf.get_section(section['sh_link'])
            for rel in section.iter_relocations():
                if rel['r_info_type']==21:
                    s=table.get_symbol(rel['r_info_sym'])
                    if s['st_shndx']!='SHN_UNDEF':machine.uc.mem_write(rel['r_offset'],struct.pack('<I',s['st_value']))
        loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        def evidence(address):
            symbol=next(s for s in symbols.values()if s['st_value']==address and s['st_info']['type']=='STT_FUNC')
            size=symbol['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr'])
            return {'elf_address':f'0x{address:08x}','symbol':symbol.name,'size':size,'sha256':hashlib.sha256(f.read(size)).hexdigest()}
        addresses=[0x4ddeb8,0x3140ec,0x3116e8,0x31167c,0x4c8bb0]
        addresses.extend(int(row['lookup_elf_address'],16)for row in bindings)
        functions=[evidence(address)for address in dict.fromkeys(addresses)]
    machine.uc.mem_map(heap_base,heap_end-heap_base)
    def pool(uc,address,size,unused):
        calls['original_pool_allocation_model']+=1
        count=struct.unpack('<I',uc.mem_read(machine.reg(0),4))[0]
        machine.write_reg(0,allocate(machine,count))
        uc.reg_write(machine.pc_reg,uc.reg_read(machine.lr_reg))
    machine.uc.hook_add(UC_HOOK_CODE,pool,begin=0x708ec0,end=0x708ec0)
    machine.call(0x4ddeb8,[])
    results=[];checks=0
    for row in bindings:
        classname=row['class'];symbol=symbols[f'_ZN7Structs{len(classname)}{classname}11m_dataNamesE']
        assert symbol['st_size']%24==0
        count=symbol['st_size']//24;base=symbol['st_value'];values=[]
        for i in range(count):
            obj=base+24*i;end,start=struct.unpack('<II',machine.uc.mem_read(obj+16,8))
            value=string(machine,start);assert end==start+len(value),(classname,i,start,end,value)
            values.append(value.decode('ascii'))
        lookup=int(row['lookup_elf_address'],16)
        for value in list(dict.fromkeys(values))+['__missing__']:
            query=machine.data+0x1000;machine.uc.mem_write(query,value.encode()+b'\0')
            expected=values.index(value)if value in values else 0xffffffff
            assert machine.call(lookup,[query])==expected,(classname,value,expected)
            checks+=1
        results.append({**row,'names_elf_address':f'0x{base:08x}','string_object_bytes':24,'count':count,'names':values})
    result={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),
            'test_sha256':sha(Path(__file__)),'registration_trace_sha256':sha(a.registrations),
            'tables':results,'table_count':len(results),'names':sum(r['count']for r in results),
            'original_lookup_checks':checks,'function_evidence':functions,'dependency_calls':dict(calls),
            'exit_handler_registrations':len(exit_handlers),'modeled_heap_bytes':heap-heap_base,
            'stack_restoration':True,
            'dependency_model':'Actual original 51,576-byte static initializer, string constructors, range initialization, allocation-block selection and all registered Structs GetMemberIDByString bodies execute. Pool/operator-new allocation, bounded strlen/memcpy/strcmp and exit-handler registration are explicit models. Destructors, map-manager lookup, object record layout/access, cache struct-name overrides, scripts and gameplay are not executed.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k not in ('tables','function_evidence')}))

if __name__=='__main__':main()
