#!/usr/bin/env python3
"""Execute original class-rule readers; records, not gameplay execution."""
import argparse,hashlib,importlib.util,json,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
REPO=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('property_reader',REPO/'tools/trace_character_properties.py')
properties=importlib.util.module_from_spec(spec);spec.loader.exec_module(properties)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Original(properties.Original):
    def __init__(self,original,oracle,property_bytes,class_bytes):
        super().__init__(original,oracle,property_bytes)
        self.raw=class_bytes;self.cursor=0;self.calls=Counter()
        self.invoke(0x4b4484,[self.stream]);assert self.cursor==len(class_bytes)
        self.class_pointer=self.word(self.symbols['_ZN6Arrays10ClassTable7membersE']['st_value'])
        self.class_count=self.word(self.symbols['_ZN6Arrays10ClassTable4sizeE']['st_value'])
        assert self.class_count==struct.unpack_from('<I',class_bytes)[0]
        self.classes=[];cursor=4
        for index in range(self.class_count):
            rec=self.class_pointer+index*12;count=self.word(rec+4);ptr=self.word(rec+8)
            assert count==struct.unpack_from('<I',class_bytes,cursor)[0];cursor+=4
            rows=[list(struct.unpack('<5i',self.machine.uc.mem_read(ptr+i*24+4,20)))for i in range(count)]
            assert b''.join(struct.pack('<5i',*r)for r in rows)==class_bytes[cursor:cursor+20*count]
            start=cursor-4;cursor+=20*count;self.classes.append({'index':index,'count':count,'start':start,'end':cursor,'rows':rows})
        assert cursor==len(class_bytes)
        addresses=(0x4b4484,0x4dcfec,0x4f19dc)
        with original.open('rb')as f:
            elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
            self.class_evidence=[]
            for addr in addresses:
                s=next(s for s in self.symbols.values()if s['st_value']==addr and s['st_info']['type']=='STT_FUNC');n=s['st_size']
                seg=next(s for s in loads if s['p_vaddr']<=addr and addr+n<=s['p_vaddr']+s['p_filesz'])
                f.seek(seg['p_offset']+addr-seg['p_vaddr']);self.class_evidence.append({'elf_address':hex(addr),'size':n,'symbol':s.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();path=a.cache/'data/pydata/character_classes_pyarray.bin';raw=path.read_bytes()
    old=Original(a.original,a.oracle,(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes(),raw)
    names=json.loads((REPO/'reports/pydata-array-name-trace.json').read_text())
    table=next(t for f in names['files']if f['path']=='data/pydata/character_classes_pyarraynames.bin'for t in f['tables']if t['class']=='ClassTable')
    assert table['count']==old.class_count
    result={'complete_game':False,'class_rules_executed':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),
            'test_sha256':sha(Path(__file__)),'cache':{'path':path.relative_to(a.cache).as_posix(),'bytes':len(raw),'sha256':sha(path)},
            'class_count':old.class_count,'rule_count':sum(c['count']for c in old.classes),'name_count_agreement':True,'fully_consumed':True,
            'classes':old.classes,'function_evidence':old.class_evidence,'dependency_calls':dict(old.calls),'stack_restoration':True,
            'scope':'Actual original ClassTable, ClassFuncList and ClassFunc readers execute. All record payloads match serialized five-int rule tuples. Property readers initialize dependencies. Virtual reads, bounded zeroed allocations, disposal and copying are explicit models. No class application, derived stats, original Character, Lua gameplay or APK integration.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('classes','function_evidence','dependency_calls')}))
if __name__=='__main__':main()
