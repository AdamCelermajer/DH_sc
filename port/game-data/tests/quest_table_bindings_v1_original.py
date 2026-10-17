"""Fresh full original Quest array/row/nested/name readers with stream/heap leaves.

Reuses the existing engine CPU loader and the semantic extraction layout from
port/quest-data/tests/differential.py. No executable original bytes are patched.
"""
from collections import Counter
import hashlib,importlib.util,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
spec=importlib.util.spec_from_file_location('quest_table_engine_cpu',ROOT/'port/engine-math/tests/differential.py')
cpu=importlib.util.module_from_spec(spec);spec.loader.exec_module(cpu)
PIN=ROOT/'port/game-data/reference/quest-table-bindings-v1/original-functions.json'
class MemoryLeaves:
    def call(self,m,name):
        if name in ('memcpy','memmove'):
            dst,src,count=[m.reg(i) for i in range(3)];assert count<=4*1024*1024
            if count:m.uc.mem_write(dst,bytes(m.uc.mem_read(src,count)))
            m.write_reg(0,dst)
        elif name=='memset':
            dst,byte,count=[m.reg(i) for i in range(3)];assert count<=4*1024*1024
            if count:m.uc.mem_write(dst,bytes([byte&255])*count)
            m.write_reg(0,dst)
        else:raise AssertionError('unmodeled quest reader import: '+name)
        m.uc.reg_write(m.pc_reg,m.uc.reg_read(m.lr_reg))
class Original:
    def __init__(self,path,packed,names):
        pins=json.loads(PIN.read_text());assert hashlib.sha256(path.read_bytes()).hexdigest()==pins['original_sha256']
        self.machine=cpu.Cpu(path,False,MemoryLeaves(),pins)
        m=self.machine;self.cursor=0;self.raw=packed;self.heap=0x6000000;self.calls=Counter();self.entries=Counter();self.words=set()
        with path.open('rb') as f:
            elf=ELFFile(f);self.symbols={s.name:s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
            loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
            for pin in pins['functions']:
                address=int(pin['elf_address'],0);symbol=self.symbols[pin['original_symbol']]
                assert (symbol['st_value'],symbol['st_size'])==(address,pin['size'])
                segment=next(s for s in loads if s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz'])
                f.seek(segment['p_offset']+address-segment['p_vaddr']);assert hashlib.sha256(f.read(pin['size'])).hexdigest()==pin['sha256']
            for section in elf.iter_sections():
                if section['sh_type']!='SHT_REL':continue
                symbols=elf.get_section(section['sh_link'])
                for relocation in section.iter_relocations():
                    if relocation['r_info_type']==21:
                        symbol=symbols.get_symbol(relocation['r_info_sym'])
                        if symbol['st_shndx']!='SHN_UNDEF':m.uc.mem_write(relocation['r_offset'],struct.pack('<I',symbol['st_value']))
        ranges=[(int(p['elf_address'],0),p['size']) for p in pins['functions']]
        def entered(uc,address,size,context):
            for start,length in ranges:
                if address==start:self.entries[hex(address)]+=1
                if start<=address<start+length:self.words.add(address);break
        m.uc.hook_add(UC_HOOK_CODE,entered)
        m.uc.mem_map(self.heap,0x1000000);self.stream=m.data+0x800;vtable=m.data+0x900;read=m.stop+0x100
        m.uc.mem_write(self.stream,struct.pack('<I',vtable));m.uc.mem_write(vtable+0x18,struct.pack('<I',read))
        def boundary(uc,address,size,context):
            self.calls[hex(address)]+=1
            if address==read:
                assert m.reg(0)==self.stream and m.reg(3)==0
                count=m.reg(2);assert count<=4096 and self.cursor+count<=len(self.raw)
                if count:uc.mem_write(m.reg(1),self.raw[self.cursor:self.cursor+count])
                self.cursor+=count;m.write_reg(0,count);m.write_reg(1,0)
            elif address==0x31056c:
                count=m.reg(0);assert count<=4*1024*1024 and self.heap+count+32<0x7000000
                m.write_reg(0,self.heap)
                if count:uc.mem_write(self.heap,bytes(count))
                self.heap=(self.heap+max(count,1)+31)&~15
            elif address==0x310440:pass
            uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
        for address in [read,0x31056c,0x310440]:m.uc.hook_add(UC_HOOK_CODE,boundary,begin=address,end=address)
        self.invoke(0x4b8acc,[self.stream]);assert self.cursor==len(packed)
        self.count=self.word(self.symbols['_ZN6Arrays8v2Quests4sizeE']['st_value']);self.pointer=self.word(self.symbols['_ZN6Arrays8v2Quests7membersE']['st_value'])
        assert self.word(self.pointer-8)==0x11c and self.word(self.pointer-4)==self.count
        self.raw=names;self.cursor=0;self.invoke(0x4b1928,[self.stream]);assert self.cursor==len(names)
        self.names_pointer=self.word(self.symbols['_ZN6Arrays8v2Quests13m_memberNamesE']['st_value'])
    def invoke(self,address,args):
        m=self.machine;stack=m.stack+0xe000;m.uc.reg_write(m.sp_reg,stack);m.uc.reg_write(m.lr_reg,m.stop)
        for i,arg in enumerate(args):m.write_reg(i,arg)
        m.uc.emu_start(address,m.stop,count=2000000);assert m.uc.reg_read(m.pc_reg)==m.stop and m.uc.reg_read(m.sp_reg)==stack
    def word(self,address):return struct.unpack('<I',self.machine.uc.mem_read(address,4))[0]
    def signed(self,address,count):return list(struct.unpack('<'+'i'*count,self.machine.uc.mem_read(address,count*4)))
    def string(self,address):return bytes(self.machine.uc.mem_read(self.word(address+4),self.word(address))).hex() if self.word(address) else ''
    def objective(self,address):return dict(common=self.signed(address+4,3),strings=[self.string(address+offset) for offset in [16,24]],args=self.signed(address+32,3))
    def name(self,index):
        pointer=self.word(self.names_pointer+index*4);out=bytearray()
        for i in range(4096):
            value=self.machine.uc.mem_read(pointer+i,1)[0]
            if not value:return out.decode('utf-8')
            out.append(value)
        raise AssertionError('source name not terminated')
    def record(self,index):
        p=self.pointer+index*0x11c;groups=[]
        for group in range(5):
            count=self.word(p+0x14+group*8);pointer=self.word(p+0x18+group*8)
            groups.append([self.objective(pointer+i*44) if group==1 else self.signed(pointer+i*16+4,3) for i in range(count)])
        return dict(index=index,ids=self.signed(p+4,4),lists=groups,accept=self.objective(p+0x3c),end=self.objective(p+0x68),target_level=self.signed(p+0x94,1)[0],repeatable=self.machine.uc.mem_read(p+0x98,1)[0],state=self.signed(p+0x9c,1)[0],scripts=[self.string(p+0xa4+i*8) for i in range(14)],priority=self.signed(p+0x114,1)[0],act=self.signed(p+0x118,1)[0],name=self.name(index))
