#!/usr/bin/env python3
"""Execute the eight original loot array readers with bounded stream storage."""
import argparse,hashlib,importlib.util,json,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
REPO=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('property_reader',REPO/'tools/trace_character_properties.py')
properties=importlib.util.module_from_spec(spec);spec.loader.exec_module(properties)
TABLES=['DropTilePriorityTable','InventoryTable','ItemList','ItemTable','ItemTypeList','LootTable','MerchantTable','NumProbArray']
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Original(properties.Original):
    def __init__(self,original,oracle,property_bytes,loot_bytes):
        super().__init__(original,oracle,property_bytes)
        self.raw=loot_bytes;self.cursor=0;self.calls=Counter();self.loot_tables=[];m=self.machine
        entered=Counter()
        for symbol in self.symbols.values():
            if symbol['st_info']['type']=='STT_FUNC' and ('4readEP11IStreamBase' in symbol.name or 'StreamReader' in symbol.name):
                def hook(uc,address,size,unused):entered[address]+=1
                m.uc.hook_add(UC_HOOK_CODE,hook,begin=symbol['st_value'],end=symbol['st_value'])
        for name in TABLES:
            prefix=f'_ZN6Arrays{len(name)}{name}';start=self.cursor
            self.invoke(self.symbols[prefix+'4readEP11IStreamBase']['st_value'],[self.stream])
            count=self.word(self.symbols[prefix+'4sizeE']['st_value']);ptr=self.word(self.symbols[prefix+'7membersE']['st_value'])
            stride=self.word(ptr-8);assert self.word(ptr-4)==count==struct.unpack_from('<I',loot_bytes,start)[0]
            self.loot_tables.append({'class':name,'count':count,'stride':stride,'start':start,'end':self.cursor,'pointer':ptr})
            print(name,count,stride,start,self.cursor,flush=True)
        assert self.cursor==len(loot_bytes)
        self.loot_evidence=[]
        with original.open('rb') as f:
            elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
            for address,calls in sorted(entered.items()):
                s=next(s for s in self.symbols.values()if s['st_value']==address and s['st_info']['type']=='STT_FUNC');n=s['st_size']
                seg=next(s for s in loads if s['p_vaddr']<=address and address+n<=s['p_vaddr']+s['p_filesz'])
                f.seek(seg['p_offset']+address-seg['p_vaddr']);self.loot_evidence.append({'elf_address':hex(address),'size':n,'symbol':s.name,'calls':calls,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
        self.item_pointer=self.loot_tables[3]['pointer'];self.item_count=self.loot_tables[3]['count']
        self.items=[]
        for i in range(self.item_count):
            p=self.item_pointer+164*i
            string=lambda n,ptr:bytes(m.uc.mem_read(self.word(p+ptr),self.word(p+n))).hex()if self.word(p+n) else ''
            self.items.append({'index':i,'name_hex':string(4,8),'base_ints':list(struct.unpack('<4i',m.uc.mem_read(p+12,16))),
                               'base_bool':bytes(m.uc.mem_read(p+28,1))[0],
                               'base_float_bits':list(struct.unpack('<9I',m.uc.mem_read(p+32,36))),
                               'item_ints':list(struct.unpack('<2i',m.uc.mem_read(p+68,8))),
                               'description_hex':string(76,80),'tail_ints':list(struct.unpack('<20i',m.uc.mem_read(p+84,80)))})
        def ints(p,n):return list(struct.unpack('<'+'i'*n,m.uc.mem_read(p,n*4)))if n else []
        def pairs(p,n):return [list(struct.unpack('<2h',m.uc.mem_read(p+i*8+4,4)))for i in range(n)]
        for t in self.loot_tables:
            rows=[]
            for i in range(t['count']):
                p=t['pointer']+i*t['stride'];name=t['class']
                if name in ('DropTilePriorityTable','NumProbArray'):row=pairs(self.word(p+8),self.word(p+4))
                elif name=='InventoryTable':row=struct.unpack('<h',m.uc.mem_read(p+4,2))[0]
                elif name=='ItemList':
                    ptr=self.word(p+8);row=[list(struct.unpack('<ihb',m.uc.mem_read(ptr+j*12+4,7)))for j in range(self.word(p+4))]
                elif name=='ItemTable':continue
                elif name=='ItemTypeList':row=list(struct.unpack('<'+'b'*self.word(p+4),m.uc.mem_read(self.word(p+8),self.word(p+4))))if self.word(p+4)else []
                elif name=='LootTable':
                    lists=[[ints(self.word(p+off+4)+j*36+4,8)for j in range(self.word(p+off))]for off in (12,20)]
                    row=[ints(p+4,2),*lists,ints(self.word(p+32),self.word(p+28))]
                elif name=='MerchantTable':
                    ptr=self.word(p+16);row=[ints(p+4,2),[ints(ptr+j*12+4,2)for j in range(self.word(p+12))]]
                rows.append(row)
            if name!='ItemTable':t['rows']=rows
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();path=a.cache/'data/pydata/loot_table_pyarray.bin';raw=path.read_bytes()
    old=Original(a.original,a.oracle,(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes(),raw)
    names=json.loads((REPO/'reports/pydata-array-name-trace.json').read_text())
    nts=[t for f in names['files']if f['path']=='data/pydata/loot_table_pyarraynames.bin'for t in f['tables']]
    tables=[]
    for t,n in zip(old.loot_tables,nts):
        assert t['class']==n['class'] and t['count']==n['count'];tables.append({k:v for k,v in t.items()if k!='pointer'})
    result={'complete_game':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'test_sha256':sha(Path(__file__)),
            'cache':{'path':path.relative_to(a.cache).as_posix(),'bytes':len(raw),'sha256':sha(path)},'tables':tables,'items':old.items,
            'fully_consumed':True,'name_count_agreement':True,'function_evidence':old.loot_evidence,'dependency_calls':dict(old.calls),
            'stack_restoration':True,'scope':'Actual original eight array readers and nested record/primitive readers execute. Virtual reads and bounded zeroed allocation/disposal are explicit models. All native scalar, string and nested list destinations are recorded. No original inventory lifecycle, randomness, combat, Character or Lua game executes.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'cache':result['cache'],'tables':[{k:v for k,v in t.items()if k!='rows'}for t in tables],'fully_consumed':True,'name_count_agreement':True}))
if __name__=='__main__':main()
