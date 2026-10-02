#!/usr/bin/env python3
"""Execute the original constant reader; capture explicit map insertion boundaries."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
REPO=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('pydata_cpu',REPO/'port/animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
ROWS=[(0x4c540c,660,'PyDataConstants::reloadData'),(0x317734,244,'StreamReader::readString'),
      (0x313a90,160,'StreamReader::readUint32'),(0x3df1a0,44,'StreamReader::readAsUint32'),
      (0x459090,44,'StreamReader::readAsInt32')]
def main():
    p=argparse.ArgumentParser()
    for name in ('original','oracle','cache','report'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
    assert sha(a.original)==cpu.ORIGINAL_SHA256
    evidence=[]
    with a.original.open('rb')as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        # Sizes are checked against symbols instead of inferred from listing ranges.
        syms={s['st_value']:s for s in elf.get_section_by_name('.dynsym').iter_symbols()if s['st_shndx']!='SHN_UNDEF'}
        for address,_,name in ROWS:
            size=syms[address]['st_size']
            segment=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            f.seek(segment['p_offset']+address-segment['p_vaddr'])
            evidence.append({'elf_address':f'0x{address:08x}','size':size,'name':name,
                             'sha256':hashlib.sha256(f.read(size)).hexdigest()})
        machine=cpu.EngineCpu(a.original,False,cpu.Dependencies(a.oracle),{'functions':evidence})
        guard=machine.data+0x40;machine.uc.mem_write(guard,struct.pack('<I',0x13579bdf))
        for section in elf.iter_sections():
            if section['sh_type']!='SHT_REL':continue
            symbols=elf.get_section(section['sh_link'])
            for rel in section.iter_relocations():
                if rel['r_info_type']==21:
                    sym=symbols.get_symbol(rel['r_info_sym'])
                    if sym['st_shndx']!='SHN_UNDEF':target=sym['st_value']
                    elif sym.name=='__stack_chk_guard':target=guard
                    else:continue
                    machine.uc.mem_write(rel['r_offset'],struct.pack('<I',target))
    obj,stream,vtable=machine.data+0x400,machine.data+0x800,machine.data+0x900
    read_boundary=machine.stop+0x100
    machine.uc.mem_write(stream,struct.pack('<I',vtable))
    machine.uc.mem_write(vtable+0x18,struct.pack('<I',read_boundary))
    slots=0x6000000;machine.uc.mem_map(slots,0x400000)
    raw=b'';cursor=0;entries=[];group='';reads=[]
    def string(pointer):
        value=bytes(machine.uc.mem_read(pointer,256));end=value.find(b'\0');assert end>=0
        return value[:end].decode('ascii')
    def hook(uc,address,size,unused):
        nonlocal cursor,group
        if address==read_boundary:
            assert machine.reg(0)==stream and machine.reg(3)==0
            count=machine.reg(2);assert count<=255
            data=raw[cursor:cursor+count];assert len(data)==count,'Original reader requested unavailable bytes'
            if data:uc.mem_write(machine.reg(1),data)
            reads.append({'offset':cursor,'bytes':count});cursor+=count
            machine.write_reg(0,count);machine.write_reg(1,0)
        elif address==0x4c5274:
            assert machine.reg(0)==obj+4;group=string(machine.reg(1));machine.write_reg(0,slots)
        elif address==0x4c4c30:
            assert machine.reg(0)==slots
            slot=slots+0x100+16*len(entries)
            entries.append({'group':group,'name':string(machine.reg(1)),'slot':slot})
            machine.write_reg(0,slot)
        # Logging and temporary diagnostic strings are non-mutating stubs.
        uc.reg_write(machine.pc_reg,uc.reg_read(machine.lr_reg))
    boundaries=[read_boundary,0x4c5274,0x4c4c30,0x337888,0x3140ec,0x337a88,0x318254]
    for address in boundaries:machine.uc.hook_add(UC_HOOK_CODE,hook,begin=address,end=address)
    files=[]
    for path in sorted((a.cache/'data/pydata').glob('*_pycst.bin')):
        raw=path.read_bytes();cursor=0;entries=[];reads=[]
        machine.uc.mem_write(obj,bytes(64));machine.call(0x4c540c,[obj,stream,0])
        for row in entries:row['value']=struct.unpack('<i',machine.uc.mem_read(row.pop('slot'),4))[0]
        files.append({'path':path.relative_to(a.cache).as_posix(),'bytes':len(raw),'sha256':sha(path),
                      'consumed_bytes':cursor,'fully_consumed':cursor==len(raw),
                      'entries':len(entries),'rows':entries,'read_calls':len(reads)})
    result={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),
            'oracle_sha256':sha(a.oracle),'function_evidence':evidence,'files':files,
            'file_count':len(files),'full_consumption_files':sum(row['fully_consumed']for row in files),
            'entry_count':sum(row['entries']for row in files),'test_sha256':sha(Path(__file__)),
            'stack_restoration':True,'import_calls':machine.import_calls,
            'dependency_model':'Actual original constant reader, primitive readers and string reader execute; virtual stream read is a bounded byte-source model; two map insertion boundaries capture keys and values; logging/string diagnostic and stack guard initialization are controlled stubs. Original maps, lookup, allocation and script behavior do not execute.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k not in ('files','function_evidence')}))
if __name__=='__main__':main()
