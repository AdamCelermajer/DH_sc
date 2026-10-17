#!/usr/bin/env python3
"""Execute all original readers in the character-property cache file.

Stream bytes, allocation and disposal are bounded explicit models. No entities,
scripts or gameplay execute. ELF addresses use the original zero load base.
"""
import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

REPO=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('property_cpu',REPO/'port/animation-values/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
TABLES=[('CharacterTable',0x4b4340,900),('StatAutoAssignSchemeTable',0x4b41f4,12),('StatListTable',0x4b40a8,12)]
FUNCTIONS=[0x4b4340,0x4b41f4,0x4b40a8,0x4f2750,0x4dce90,0x4ef520,0x4eacc0,
           0x313a90,0x3df1a0,0x459090,0x3deca0,0x3dedb4,0x3deed8,0x3def10,
           0x3def34,0x3df114,0x3df140,0x3df250]

class Original:
    def __init__(self,original,oracle,raw):
        assert sha(original)==cpu.ORIGINAL_SHA256
        self.raw=raw;self.cursor=0;self.heap=0x6000000;self.calls=Counter();self.evidence=[]
        self.machine=cpu.EngineCpu(original,False,cpu.Dependencies(oracle),{'functions':[]})
        m=self.machine
        with original.open('rb') as f:
            elf=ELFFile(f);self.symbols={s.name:s for s in elf.get_section_by_name('.dynsym').iter_symbols() if s['st_shndx']!='SHN_UNDEF'}
            for section in elf.iter_sections():
                if section['sh_type']!='SHT_REL':continue
                syms=elf.get_section(section['sh_link'])
                for rel in section.iter_relocations():
                    if rel['r_info_type']==21:
                        s=syms.get_symbol(rel['r_info_sym'])
                        if s['st_shndx']!='SHN_UNDEF':m.uc.mem_write(rel['r_offset'],struct.pack('<I',s['st_value']))
            loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
            for address in FUNCTIONS:
                s=next(s for s in self.symbols.values() if s['st_value']==address and s['st_info']['type']=='STT_FUNC')
                size=s['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
                f.seek(seg['p_offset']+address-seg['p_vaddr'])
                self.evidence.append({'elf_address':f'0x{address:08x}','size':size,'symbol':s.name,'sha256':hashlib.sha256(f.read(size)).hexdigest()})
        m.uc.mem_map(self.heap,0x4000000)
        self.stream=m.data+0x800;vtable=m.data+0x900;self.read_boundary=m.stop+0x100
        m.uc.mem_write(self.stream,struct.pack('<I',vtable));m.uc.mem_write(vtable+0x18,struct.pack('<I',self.read_boundary))
        def boundary(uc,address,size,unused):
            self.calls[hex(address)]+=1
            if address==self.read_boundary:
                assert m.reg(0)==self.stream and m.reg(3)==0
                count=m.reg(2);assert count<=4096 and self.cursor+count<=len(self.raw)
                if count:uc.mem_write(m.reg(1),self.raw[self.cursor:self.cursor+count])
                self.cursor+=count;m.write_reg(0,count);m.write_reg(1,0)
            elif address==0x31056c:
                count=m.reg(0);assert count<=8*1024*1024 and self.heap+count+32<0xa000000
                m.write_reg(0,self.heap);uc.mem_write(self.heap,bytes(count));self.heap=(self.heap+max(count,1)+31)&~15
            elif address==0x310440:
                # Original reads begin from zeroed globals. Nested replacement
                # disposal may occur; allocation lifetime is retained for evidence.
                pass
            uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
        for addr in (self.read_boundary,0x31056c,0x310440):
            m.uc.hook_add(UC_HOOK_CODE,boundary,begin=addr,end=addr)
        self.tables=[]
        for name,address,stride in TABLES:
            prefix=f'_ZN6Arrays{len(name)}{name}'
            start=self.cursor;self.invoke(address,[self.stream])
            count=self.word(self.symbols[prefix+'4sizeE']['st_value'])
            ptr=self.word(self.symbols[prefix+'7membersE']['st_value'])
            assert count==struct.unpack_from('<I',raw,start)[0]
            rows=[]
            for i in range(count):
                rec=ptr+i*stride
                if name=='CharacterTable':values=list(struct.unpack('<224i',m.uc.mem_read(rec+4,896)))
                else:
                    length,data=self.word(rec+4),self.word(rec+8)
                    if name=='StatListTable':values=list(struct.unpack('<'+'i'*length,m.uc.mem_read(data,length*4))) if length else []
                    else:values=[list(struct.unpack('<3i',m.uc.mem_read(data+j*16+4,12))) for j in range(length)]
                rows.append(values)
            self.tables.append({'class':name,'count':count,'start':start,'end':self.cursor,'rows':rows})
        assert self.cursor==len(raw)
        self.character_pointer=self.word(self.symbols['_ZN6Arrays14CharacterTable7membersE']['st_value'])
        offsets=self.symbols['_ZN7Structs19CharacterProperties13m_dataOffsetsE']
        assert offsets['st_size']==896
        self.offsets=list(struct.unpack('<224I',m.uc.mem_read(offsets['st_value'],896)))
        assert self.offsets==list(range(0,896,4))
        # Check each actual destination against the serialized bytes after the
        # original record/primitive readers have executed.
        assert b''.join(struct.pack('<224i',*r) for r in self.tables[0]['rows'])==raw[4:self.tables[0]['end']]

    def word(self,address):return struct.unpack('<I',self.machine.uc.mem_read(address,4))[0]
    def invoke(self,address,args):
        m=self.machine;sp=m.stack+0xe000
        m.uc.reg_write(m.sp_reg,sp);m.uc.reg_write(m.lr_reg,m.stop)
        for i,arg in enumerate(args):m.write_reg(i,arg&0xffffffff)
        m.uc.emu_start(address,m.stop,count=20000000)
        assert m.uc.reg_read(m.pc_reg)==m.stop and m.uc.reg_read(m.sp_reg)==sp
        return m.reg(0)

def main():
    p=argparse.ArgumentParser()
    for name in ('original','oracle','cache','report'):p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();path=a.cache/'data/pydata/character_properties_pyarray.bin';raw=path.read_bytes()
    old=Original(a.original,a.oracle,raw)
    names=json.loads((REPO/'reports/pydata-array-name-trace.json').read_text())
    name_tables=[t for f in names['files'] if f['path']=='data/pydata/character_properties_pyarraynames.bin' for t in f['tables']]
    tables=[]
    for t,n in zip(old.tables,name_tables):
        assert t['class']==n['class'] and t['count']==n['count']
        table={k:v for k,v in t.items() if k!='rows'}
        table['name_count_agreement']=True
        if t['class']=='CharacterTable':
            table['values']=t['count']*224
            table['record_payload_sha256']=hashlib.sha256(b''.join(struct.pack('<224i',*r) for r in t['rows'])).hexdigest()
            table['defaults']=t['rows'][0];table['types']=t['rows'][1]
        else:table['rows']=t['rows']
        tables.append(table)
    result={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),
            'test_sha256':sha(Path(__file__)),'cache':{'path':path.relative_to(a.cache).as_posix(),'sha256':sha(path),'bytes':len(raw)},
            'tables':tables,'field_offsets':old.offsets,'fully_consumed':True,'function_evidence':old.evidence,
            'dependency_calls':dict(old.calls),'stack_restoration':True,
            'dependency_model':'Actual original three array readers, four nested record readers and three primitive readers execute. Virtual byte reads and zeroed bounded allocation/disposal are explicit models. All 100,352 character values match actual original reader destinations; ordered name counts agree for these three tables. No entity, script, default/type getters or property mutation executes in this reader trace.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'bytes':len(raw),'tables':[(t['class'],t['count'],t['start'],t['end']) for t in tables],'fully_consumed':True}))

if __name__=='__main__':main()
