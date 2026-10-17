#!/usr/bin/env python3
"""Execute the original item-power readers with bounded stream/allocation models."""
import argparse,hashlib,importlib.util,json,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
REPO=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('property_reader',REPO/'tools/trace_character_properties.py')
properties=importlib.util.module_from_spec(spec);spec.loader.exec_module(properties)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Original(properties.Original):
    def __init__(self,original,oracle,property_bytes,power_bytes):
        super().__init__(original,oracle,property_bytes)
        self.raw=power_bytes;self.cursor=0;self.calls=Counter();m=self.machine;entered=Counter();self.power_tables=[]
        def hook(uc,address,size,unused):entered[address]+=1
        for s in self.symbols.values():
            if s['st_info']['type']=='STT_FUNC' and ('4readEP11IStreamBase' in s.name or 'StreamReader' in s.name):
                m.uc.hook_add(UC_HOOK_CODE,hook,begin=s['st_value'],end=s['st_value'])
        for name in ['ItemPowerList','ItemPowerTable']:
            prefix=f'_ZN6Arrays{len(name)}{name}';start=self.cursor
            self.invoke(self.symbols[prefix+'4readEP11IStreamBase']['st_value'],[self.stream])
            count=self.word(self.symbols[prefix+'4sizeE']['st_value']);ptr=self.word(self.symbols[prefix+'7membersE']['st_value']);stride=self.word(ptr-8)
            assert count==self.word(ptr-4)==struct.unpack_from('<I',power_bytes,start)[0]
            rows=[]
            for i in range(count):
                p=ptr+i*stride
                if name=='ItemPowerList':
                    n,q=self.word(p+4),self.word(p+8)
                    row=[list(struct.unpack('<ib',m.uc.mem_read(q+j*12+4,5))) for j in range(n)]
                else:
                    n,q=self.word(p+12),self.word(p+16)
                    row={'type':struct.unpack('<b',m.uc.mem_read(p+4,1))[0], 'value':struct.unpack('<i',m.uc.mem_read(p+8,4))[0],
                         'stats':[list(struct.unpack('<3i',m.uc.mem_read(q+j*16+4,12))) for j in range(n)],
                         'tail':list(struct.unpack('<5i',m.uc.mem_read(p+20,20)))}
                rows.append(row)
            self.power_tables.append({'class':name,'count':count,'stride':stride,'start':start,'end':self.cursor,'pointer':ptr,'rows':rows})
            print(name,count,stride,start,self.cursor,flush=True)
        assert self.cursor==len(power_bytes)
        self.power_pointer=self.power_tables[1]['pointer'];self.power_evidence=[]
        with original.open('rb') as f:
            elf=ELFFile(f);loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
            for address,calls in sorted(entered.items()):
                s=next(s for s in self.symbols.values() if s['st_value']==address and s['st_info']['type']=='STT_FUNC');n=s['st_size']
                seg=next(s for s in loads if s['p_vaddr']<=address and address+n<=s['p_vaddr']+s['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr'])
                self.power_evidence.append({'elf_address':hex(address),'size':n,'symbol':s.name,'calls':calls,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();path=a.cache/'data/pydata/item_powers_pyarray.bin'
    old=Original(a.original,a.oracle,(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes(),path.read_bytes())
    names=json.loads((REPO/'reports/pydata-array-name-trace.json').read_text())
    ns=[t for f in names['files'] if f['path']=='data/pydata/item_powers_pyarraynames.bin' for t in f['tables']]
    assert [(t['class'],t['count']) for t in ns]==[(t['class'],t['count']) for t in old.power_tables]
    result={'complete_game':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'test_sha256':sha(Path(__file__)),
            'cache':{'path':path.relative_to(a.cache).as_posix(),'bytes':path.stat().st_size,'sha256':sha(path)},
            'tables':[{k:v for k,v in t.items() if k!='pointer'} for t in old.power_tables],
            'fully_consumed':True,'name_count_agreement':True,'function_evidence':old.power_evidence,'dependency_calls':dict(old.calls),
            'stack_restoration':True,'scope':'Actual original item-power array, nested record and primitive readers execute. Virtual byte reads and bounded allocation/disposal are modeled. All scalar/nested destinations recorded. No original random power generation, inventory mutation, Character or Lua game executes.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result['cache']))
if __name__=='__main__':main()
