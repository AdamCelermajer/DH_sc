#!/usr/bin/env python3
"""Compare every quest, nested record and string with executed original readers."""
import argparse,ctypes as c,hashlib,importlib.util,io,json,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('death_test',ROOT/'../character-death/tests/differential.py')
death=importlib.util.module_from_spec(spec);spec.loader.exec_module(death)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Table(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('count',c.c_uint32)]
class Span(c.Structure):_fields_=[('offset',c.c_uint32),('size',c.c_uint32)]
class List(c.Structure):_fields_=[('offset',c.c_uint32),('count',c.c_uint32)]
class Objective(c.Structure):_fields_=[('common',c.c_int32*3),('strings',Span*2),('args',c.c_int32*3)]
class Record(c.Structure):_fields_=[('ids',c.c_int32*4),('lists',List*5),('accept',Objective),('end',Objective),('target_level',c.c_int32),('repeatable',c.c_uint32),('state',c.c_int32),('scripts',Span*14),('priority',c.c_int32),('act',c.c_int32)]
class OriginalQuest(death.base.original.Original):
    def __init__(self,original,oracle,cache):
        super().__init__(death.BufferedElfPath(original),death.BufferedElfPath(oracle),(cache/'data/pydata/character_properties_pyarray.bin').read_bytes())
        self.raw=(cache/'data/pydata/v2quests_pyarray.bin').read_bytes();self.cursor=0;self.calls.clear();self.entered=Counter();m=self.machine
        def entry(uc,address,size,unused):self.entered[address]+=1
        for s in self.symbols.values():
            if s['st_info']['type']=='STT_FUNC'and '4readEP11IStreamBase'in s.name:m.uc.hook_add(UC_HOOK_CODE,entry,begin=s['st_value'],end=s['st_value'])
        self.invoke(0x4b8acc,[self.stream]);prefix='_ZN6Arrays8v2Quests'
        self.count=self.word(self.symbols[prefix+'4sizeE']['st_value']);self.pointer=self.word(self.symbols[prefix+'7membersE']['st_value'])
        assert self.cursor==len(self.raw)and self.word(self.pointer-8)==284 and self.word(self.pointer-4)==self.count
    def signed(self,p,n):return list(struct.unpack('<'+'i'*n,self.machine.uc.mem_read(p,n*4)))
    def string(self,p):return bytes(self.machine.uc.mem_read(self.word(p+4),self.word(p))).hex()if self.word(p)else''
    def objective(self,p):return {'common':self.signed(p+4,3),'strings':[self.string(p+o)for o in (16,24)],'args':self.signed(p+32,3)}
    def record(self,i):
        p=self.pointer+i*284;groups=[]
        for group in range(5):
            n=self.word(p+20+group*8);ptr=self.word(p+24+group*8)
            groups.append([self.objective(ptr+j*44)if group==1 else self.signed(ptr+j*16+4,3)for j in range(n)])
        return {'index':i,'ids':self.signed(p+4,4),'lists':groups,'accept':self.objective(p+60),'end':self.objective(p+104),
            'target_level':self.signed(p+148,1)[0],'repeatable':bytes(self.machine.uc.mem_read(p+152,1))[0],'state':self.signed(p+156,1)[0],
            'scripts':[self.string(p+164+j*8)for j in range(14)],'priority':self.signed(p+276,1)[0],'act':self.signed(p+280,1)[0]}
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();old=OriginalQuest(a.original,a.oracle,a.cache);raw=old.raw
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_quests_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32]
    lib.dh2_quests_record.argtypes=[c.POINTER(Table),c.c_uint32,c.POINTER(Record)]
    lib.dh2_quests_list_record.argtypes=[c.POINTER(Table),c.c_uint32,c.c_uint32,c.c_uint32,c.POINTER(Span)]
    lib.dh2_quests_objective.argtypes=[c.POINTER(Table),c.c_uint32,c.c_uint32,c.POINTER(Objective)]
    new=death.base.original.cpu.EngineCpu(death.BufferedElfPath(a.arm64),True,death.base.Dependencies(a.oracle),{'functions':[]})
    new.uc.mem_map(new.data+0x10000,0x100000);data=new.data+0x10000;table_ptr=new.data+0x100;output=new.data+0x1000
    buf=c.create_string_buffer(raw);table=Table();assert not lib.dh2_quests_open(c.byref(table),buf,len(raw))and table.count==old.count
    new.uc.mem_write(data,raw);assert not new.call('dh2_quests_open',[table_ptr,data,len(raw)])
    assert c.sizeof(Record)==268 and c.sizeof(Objective)==40
    def span(s):assert s.offset<=len(raw)and s.size<=len(raw)-s.offset;return raw[s.offset:s.offset+s.size]
    def obj(o):return {'common':list(o.common),'strings':[span(s).hex()for s in o.strings],'args':list(o.args)}
    counts=Counter();rows=[]
    def call(name,args,out):
        new.uc.mem_write(output-16,b'\xa5'*(c.sizeof(out)+32))
        assert not new.call(name,[table_ptr,*args,output]);assert bytes(out)==bytes(new.uc.mem_read(output,c.sizeof(out)))
        assert bytes(new.uc.mem_read(output-16,16))==bytes(new.uc.mem_read(output+c.sizeof(out),16))==b'\xa5'*16
    for i in range(old.count):
        expected=old.record(i);r=Record();assert not lib.dh2_quests_record(c.byref(table),i,c.byref(r));call('dh2_quests_record',[i],r)
        actual={'index':i,'ids':list(r.ids),'lists':[],'accept':obj(r.accept),'end':obj(r.end),'target_level':r.target_level,'repeatable':r.repeatable,
            'state':r.state,'scripts':[span(s).hex()for s in r.scripts],'priority':r.priority,'act':r.act}
        counts['quests']+=1;counts['scripts']+=14;counts['embedded_objectives']+=2
        for group,lst in enumerate(r.lists):
            entries=[];assert lst.count==len(expected['lists'][group])
            for j in range(lst.count):
                s=Span();assert not lib.dh2_quests_list_record(c.byref(table),i,group,j,c.byref(s));call('dh2_quests_list_record',[i,group,j],s)
                if group==1:
                    o=Objective();assert not lib.dh2_quests_objective(c.byref(table),i,j,c.byref(o));call('dh2_quests_objective',[i,j],o);entries.append(obj(o));counts['list_objectives']+=1
                else:assert s.size==12;entries.append(list(struct.unpack('<3i',span(s))));counts['conditions'if group==0 else'rewards']+=1
            actual['lists'].append(entries)
        assert actual==expected,i;rows.append(actual)
    rejection_count=0
    for size in range(len(raw)):
        t=Table();c.memset(c.byref(t),0xa5,c.sizeof(t));before=bytes(t)
        assert lib.dh2_quests_open(c.byref(t),buf,size)and bytes(t)==before;rejection_count+=1
    for size in (*range(64),*range(len(raw)-64,len(raw))):
        new.uc.mem_write(table_ptr,b'\xa5'*c.sizeof(table));assert new.call('dh2_quests_open',[table_ptr,data,size])
        assert bytes(new.uc.mem_read(table_ptr,c.sizeof(table)))==b'\xa5'*c.sizeof(table)
    extra=c.create_string_buffer(raw+b'\0');assert lib.dh2_quests_open(c.byref(table),extra,len(raw)+1)
    evidence=[]
    with io.BytesIO(a.original.read_bytes())as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address,n in sorted(old.entered.items()):
            symbol=next(s for s in old.symbols.values()if s['st_value']==address and s['st_info']['type']=='STT_FUNC');seg=next(s for s in loads if s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);evidence.append({'elf_address':hex(address),'size':symbol['st_size'],'symbol':symbol.name,'calls':n,'sha256':hashlib.sha256(f.read(symbol['st_size'])).hexdigest()})
    names=json.loads((REPO/'reports/pydata-array-name-trace.json').read_text(encoding='utf-8'));names=next(t['names']for file in names['files']if file['path']=='data/pydata/v2quests_pyarraynames.bin'for t in file['tables']);assert len(names)==old.count
    for row,name in zip(rows,names):row['name']=name
    report={'complete_game':False,'mismatches':0,'counts':dict(counts),'cache':{'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()},'fully_consumed':True,'name_count_agreement':True,
        'truncation_rejections':rejection_count,'arm64_truncation_rejections':128,'trailing_byte_rejected':True,'output_guards':True,
        'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
        'source_sha256':{n:sha(ROOT/n)for n in ('quests.c','quests.h')},'rows':rows,'function_evidence':evidence,'dependency_calls':dict(old.calls),
        'scope':'Actual original quest array, 64 records and nested conditions/objectives/rewards/scripts readers execute. Native scalar/list/string destinations compare with every source host/ARM64 accessor. Read/allocation/disposal boundaries are explicit models. Native pointers/vtables are excluded from portable records. No quest compile/conditions/rewards execution, world dispatch, persistence or full source gameplay.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('rows','function_evidence','dependency_calls','scope')}))
if __name__=='__main__':main()
