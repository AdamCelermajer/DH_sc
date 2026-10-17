#!/usr/bin/env python3
"""Trace original array registrations and name readers with explicit ownership stubs."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
REPO=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('array_cpu',REPO/'port/animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
def main():
    p=argparse.ArgumentParser()
    for name in ('original','oracle','cache','report'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    assert sha(a.original)==cpu.ORIGINAL_SHA256
    with a.original.open('rb')as f:
        elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.dynsym').iter_symbols()if s['st_shndx']!='SHN_UNDEF'}
        names={s['st_value']:s.name for s in symbols.values()}
        machine=cpu.EngineCpu(a.original,False,cpu.Dependencies(a.oracle),{'functions':[]})
        for section in elf.iter_sections():
            if section['sh_type']!='SHT_REL':continue
            table=elf.get_section(section['sh_link'])
            for rel in section.iter_relocations():
                if rel['r_info_type']==21:
                    sym=table.get_symbol(rel['r_info_sym'])
                    if sym['st_shndx']!='SHN_UNDEF':machine.uc.mem_write(rel['r_offset'],struct.pack('<I',sym['st_value']))
        segments=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        def evidence(address):
            symbol=symbols[names[address]];size=symbol['st_size']
            seg=next(s for s in segments if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr'])
            return {'elf_address':f'0x{address:08x}','size':size,'symbol':symbol.name,
                    'sha256':hashlib.sha256(f.read(size)).hexdigest()}
        functions=[evidence(0x4be550)]
        # Need all evidence bytes while the original stream is still open.
        all_evidence={s['st_value']:evidence(s['st_value'])for s in symbols.values()
                      if re.match(r'_ZN6Arrays\d+\w+(?:9readNames|9skipNames|13finalizeNames|4size|13m_memberNames)',s.name)
                      and s['st_info']['type']=='STT_FUNC'}
        all_evidence.update({s['st_value']:evidence(s['st_value'])for s in symbols.values()
                             if 'GetMemberIDByString' in s.name})
    obj,stream,vtable=[machine.data+x for x in (0x400,0x800,0x900)]
    read_boundary=machine.stop+0x100;heap_base=0x6000000;heap=heap_base
    machine.uc.mem_map(heap_base,0x4000000)
    machine.uc.mem_write(stream,struct.pack('<I',vtable))
    machine.uc.mem_write(vtable+0x18,struct.pack('<I',read_boundary))
    classes=[];files=[];raw=b'';cursor=0;allocations=[]
    def cstring(pointer,limit=4096):
        raw=bytes(machine.uc.mem_read(pointer,limit));end=raw.find(b'\0');assert end>=0
        return raw[:end]
    finalizers={s['st_value']for s in symbols.values()if re.match(r'_ZN6Arrays\d+\w+13finalizeNamesEv$',s.name)}
    def boundary(uc,address,size,unused):
        nonlocal cursor,heap
        if address==0x4bdccc:
            assert machine.reg(0)==obj
            target=machine.reg(2);symbol=names[target]
            match=re.match(r'_ZN(6Arrays|7Structs)19GetMemberIDByStringINS_(\d+)(\w+)EEEiPKc$',symbol);assert match,symbol
            classname=match[3];assert len(classname)==int(match[2])
            classes.append({'registered_name':cstring(machine.reg(1)).decode('ascii'),
                            'namespace':match[1][1:],'class':classname,'lookup_elf_address':f'0x{target:08x}'})
        elif address==0x4be3e0:
            assert machine.reg(0)==obj
            files.append({'path':cstring(machine.reg(1)).decode('ascii'),
                          'reader_elf_address':f'0x{machine.reg(2):08x}',
                          'reader_symbol':names[machine.reg(2)],
                          'finalizer_elf_address':f'0x{machine.reg(3):08x}'})
        elif address==read_boundary:
            assert machine.reg(0)==stream and machine.reg(3)==0
            count=machine.reg(2);data=raw[cursor:cursor+count];assert len(data)==count
            if data:uc.mem_write(machine.reg(1),data)
            cursor+=count;machine.write_reg(0,count);machine.write_reg(1,0)
        elif address==0x31056c:
            count=machine.reg(0);assert count<4*1024*1024 and heap+count+16<heap_base+0x4000000
            machine.write_reg(0,heap);allocations.append({'address':heap,'bytes':count})
            uc.mem_write(heap,bytes(count));heap=(heap+count+31)&~15
        elif address==0x30e31c:
            aa,bb=[cstring(machine.reg(i))for i in range(2)]
            machine.write_reg(0,(0 if aa==bb else -1 if aa<bb else 1)&0xffffffff)
        # Name finalization frees original ownership; here the previous pointer is reset.
        uc.reg_write(machine.pc_reg,uc.reg_read(machine.lr_reg))
    for address in (0x4bdccc,0x4be3e0,read_boundary,0x31056c,0x30e31c,*finalizers):
        machine.uc.hook_add(UC_HOOK_CODE,boundary,begin=address,end=address)
    machine.uc.mem_write(obj,bytes(128));machine.call(0x4be550,[obj,0])
    byfile={}
    for row in files:
        if row['path'].endswith('_pyarraynames.bin'):byfile.setdefault(row['path'],[]).append(row)
    results=[];lookup_checks=0
    for filename,readers in byfile.items():
        path=a.cache/'data/pydata'/filename;raw=path.read_bytes();cursor=0;heap=heap_base;tables=[]
        for reader in readers:
            match=re.match(r'_ZN6Arrays(\d+)(\w+)9(?:readNames|skipNames)EP11IStreamBase$',reader['reader_symbol']);assert match,reader
            classname=match[2];prefix=f'_ZN6Arrays{len(classname)}{classname}'
            count=struct.unpack_from('<I',raw,cursor)[0];assert count<100000
            size_addr=symbols[prefix+'4sizeE']['st_value'];name_addr=symbols[prefix+'13m_memberNamesE']['st_value']
            machine.uc.mem_write(size_addr,struct.pack('<I',count));machine.uc.mem_write(name_addr,bytes(4))
            start=cursor;allocations=[];target=int(reader['reader_elf_address'],16)
            machine.call(target,[stream]);functions.append(all_evidence[target])
            functions.append(all_evidence[symbols[prefix+'9readNamesEP11IStreamBase']['st_value']])
            array=struct.unpack('<I',machine.uc.mem_read(name_addr,4))[0]
            pointers=struct.unpack('<'+'I'*count,machine.uc.mem_read(array,4*count))if count else []
            values=[cstring(ptr).decode('ascii')for ptr in pointers]
            binding=next(row for row in classes if row['class']==classname and row['namespace']=='Arrays')
            lookup=int(binding['lookup_elf_address'],16);functions.append(all_evidence[lookup])
            for name in list(dict.fromkeys(values[i]for i in (0,len(values)//2,len(values)-1)))if values else []:
                query=machine.data+0x1000;machine.uc.mem_write(query,name.encode()+b'\0')
                assert machine.call(lookup,[query])==values.index(name);lookup_checks+=1
            query=machine.data+0x1000;machine.uc.mem_write(query,b'__missing__\0')
            assert machine.call(lookup,[query])==0xffffffff;lookup_checks+=1
            tables.append({**binding,**reader,'start':start,'end':cursor,'count':count,'names':values})
        assert cursor==len(raw),(filename,cursor,len(raw))
        results.append({'path':path.relative_to(a.cache).as_posix(),'sha256':sha(path),'bytes':len(raw),'tables':tables})
    functions=list({row['elf_address']:row for row in functions}.values())
    result={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),
            'oracle_sha256':sha(a.oracle),'test_sha256':sha(Path(__file__)),
            'class_registrations':classes,'file_reader_registrations':files,'files':results,
            'name_file_count':len(results),'tables':sum(len(row['tables'])for row in results),
            'names':sum(table['count']for row in results for table in row['tables']),
            'original_lookup_checks':lookup_checks,'function_evidence':functions,'import_calls':machine.import_calls,
            'stack_restoration':True,
            'dependency_model':'Actual original constructor caller, name readers, primitive reads and GetMemberIDByString bodies execute. Registration installation, virtual byte stream, allocation, name finalization and strcmp are explicit bounded models/stubs. Class sizes are set to the file count: array-record parsing/size agreement and object ownership are not established. Original scripts and array records are not executed.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('classes','class_registrations','file_reader_registrations','files','function_evidence')}))
if __name__=='__main__':main()
