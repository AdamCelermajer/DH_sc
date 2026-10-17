#!/usr/bin/env python3
"""Actual native world-population helpers and four Compile bodies versus source."""
import argparse,ctypes as c,hashlib,importlib.util,io,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('death',ROOT/'../character-death/tests/differential.py')
death=importlib.util.module_from_spec(spec);spec.loader.exec_module(death)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Character(c.Structure):_fields_=[('present',c.c_uint32),('property_id',c.c_int32),('template_id',c.c_int32)]
class Cached(c.Structure):_fields_=[('property_id',c.c_int32),('quantity',c.c_int32)]
class Progress(c.Structure):_fields_=[('match_id',c.c_int32),('current',c.c_int32),('required',c.c_int32),('completed',c.c_uint32)]
class State(c.Structure):_fields_=[('progress',Progress),('active',c.c_uint32)]
class Context(c.Structure):_fields_=[('kind',c.c_uint32)]+[(n,c.c_int32)for n in ('match_id','record_level','current_level','record_required','population')]
class Result(c.Structure):_fields_=[(n,c.c_uint32)for n in ('eligible','required_updated','completion_requested','newly_completed')]
COMPILE=(0x47ed70,0x47f070,0x47ee18,0x47ef50);POPULATION=(0x47c7f4,0x47cc20)
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();old=death.base.original.Original(death.BufferedElfPath(a.original),death.BufferedElfPath(a.oracle),(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes());m=old.machine
    new=death.base.original.cpu.EngineCpu(death.BufferedElfPath(a.arm64),True,death.base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()))
    lib.dh2_quest_population.argtypes=[c.c_uint32,c.c_int32,c.POINTER(Character),c.c_uint32,c.POINTER(Cached),c.c_uint32,c.POINTER(c.c_int32)]
    lib.dh2_quest_compile.argtypes=[c.POINTER(State),c.POINTER(Context),c.POINTER(Result)]
    obj=m.data+0x3000;record=m.data+0x4000;vt=m.data+0x5000;level=m.data+0x6000;world=m.data+0x7000
    app=old.word(0x994a98+0x37f4);cache_global=old.word(0x994a98+0x43d8);charbase=old.heap+0x10000;nodebase=charbase+0x20000;treebase=nodebase+0x1000
    m.uc.mem_write(app+0x38,struct.pack('<I',world));m.uc.mem_write(old.word(0x994a98+0x1d64),struct.pack('<I',level))
    assert old.word(0x994a98+0x339c)==0x47b05c and old.word(0x994a98+0x3784)==0x47b820
    callback=m.stop+0x180;callbacks=[];pop_calls=Counter();case={}
    def boundary(uc,address,size,unused):
        if address==callback:
            assert m.reg(0)==obj;callbacks.append(1);uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
        else:pop_calls[hex(address)]+=1
    for address in (*POPULATION,callback):m.uc.hook_add(UC_HOOK_CODE,boundary,begin=address,end=address)
    m.uc.mem_write(vt+0x1c,struct.pack('<I',callback))
    ns=new.data+0x100;nc=new.data+0x200;nr=new.data+0x300;nchars=new.data+0x1000;ncache=new.data+0x2000;ncount=new.data+0x3000
    rng=random.Random(20261002);population_cases=0;compile_cases=0;paths=Counter();completion_requests=0
    def world_fixture(chars,cache):
        assert len(chars)<=16 and len(cache)<=16
        sentinel=world+0x60;nodes=[nodebase+i*12 for i in range(len(chars))]
        for i,v in enumerate(chars):
            char=charbase+i*0x1600;m.uc.mem_write(char,bytes(0x1600));m.uc.mem_write(char+0x13c8,struct.pack('<hh',v.property_id,v.template_id))
            m.uc.mem_write(nodes[i],struct.pack('<3I',nodes[i+1]if i+1<len(nodes)else sentinel,nodes[i-1]if i else sentinel,char if v.present else 0))
        m.uc.mem_write(sentinel,struct.pack('<3I',nodes[0]if nodes else sentinel,nodes[-1]if nodes else sentinel,0))
        m.uc.mem_write(cache_global,bytes(24));entries=sorted(cache,key=lambda v:v.property_id)
        def tree(lo,hi,parent):
            if lo>=hi:return 0
            mid=(lo+hi)//2;ptr=treebase+mid*24;left=tree(lo,mid,ptr);right=tree(mid+1,hi,ptr)
            m.uc.mem_write(ptr,struct.pack('<4Iii',0,parent,left,right,entries[mid].property_id,entries[mid].quantity));return ptr
        root=tree(0,len(entries),cache_global);m.uc.mem_write(cache_global+4,struct.pack('<I',root))
        ca=(Character*len(chars))(*chars);ce=(Cached*len(cache))(*cache)
        new.uc.mem_write(nchars,bytes(ca)or bytes(12));new.uc.mem_write(ncache,bytes(ce)or bytes(8))
        case.update(chars=ca,cache=ce)
    def population(kind,id):
        nonlocal population_cases
        ca,ce=case['chars'],case['cache'];before_chars=bytes(ca);before_cache=bytes(ce);out=c.c_int32(0x5a5a5a5a)
        original=c.c_int32(old.invoke(POPULATION[kind],[id,0xabcdef])).value
        assert not lib.dh2_quest_population(kind,id,ca,len(ca),ce,len(ce),c.byref(out))
        new.uc.mem_write(ncount-16,b'\xa5'*36)
        assert not new.call('dh2_quest_population',[kind,id&0xffffffff,nchars,len(ca),ncache,len(ce),ncount])
        assert out.value==original==struct.unpack('<i',new.uc.mem_read(ncount,4))[0],(kind,id,original,out.value)
        assert bytes(ca)==before_chars and bytes(ce)==before_cache
        assert bytes(new.uc.mem_read(nchars,len(ca)*12))==before_chars and bytes(new.uc.mem_read(ncache,len(ce)*8))==before_cache
        assert bytes(new.uc.mem_read(ncount-16,16))==bytes(new.uc.mem_read(ncount+4,16))==b'\xa5'*16
        population_cases+=1;return original
    def compile(kind,current,required,completed,active,id,record_level,current_level):
        nonlocal compile_cases,completion_requests
        pop=population(kind//2,id);s=State(Progress(-123,current,7654,completed),active);context=Context(kind,id,record_level,current_level,required,pop)
        before=bytes(s);callbacks.clear();m.uc.mem_write(obj,b'\xa5'*64);m.uc.mem_write(record,b'\xa5'*44)
        m.uc.mem_write(obj,struct.pack('<I',vt));m.uc.mem_write(obj+8,bytes([active]));m.uc.mem_write(obj+0xc,struct.pack('<I',record));m.uc.mem_write(obj+0x14,bytes([completed]))
        m.uc.mem_write(obj+0x20,struct.pack('<iI',current,0x12345678));m.uc.mem_write(obj+0x2c,struct.pack('<i',7654))
        m.uc.mem_write(record+0xc,struct.pack('<i',-1));m.uc.mem_write(record+0x20,struct.pack('<3i',*( (record_level,id,required)if kind&1 else(id,record_level,required))))
        m.uc.mem_write(level+0x3c,struct.pack('<i',current_level));old_obj=bytearray(m.uc.mem_read(obj,64));old_rec=bytes(m.uc.mem_read(record,44));old_context=bytes(context)
        for ptr,value in ((ns,before),(nc,bytes(context)),(nr,b'\xa5'*16)):
            new.uc.mem_write(ptr-16,b'\xa5'*(len(value)+32));new.uc.mem_write(ptr,value)
        old.invoke(COMPILE[kind],[obj]);out=Result();assert not lib.dh2_quest_compile(c.byref(s),c.byref(context),c.byref(out))and not new.call('dh2_quest_compile',[ns,nc,nr])
        expected=State(Progress(id,c.c_int32(old.word(obj+0x20)).value,c.c_int32(old.word(obj+0x2c)).value,bytes(m.uc.mem_read(obj+0x14,1))[0]),bytes(m.uc.mem_read(obj+8,1))[0])
        assert bytes(s)==bytes(expected)==bytes(new.uc.mem_read(ns,20)),(kind,current,required,completed,active,id,record_level,current_level,pop,bytes(s).hex(),bytes(expected).hex())
        level_matches=record_level==-1 or record_level==current_level;eligible=level_matches and pop>0 and (bool(kind&1)or required>0)
        assert(out.eligible,out.required_updated,out.completion_requested,out.newly_completed)==(eligible,not(kind&1)or level_matches,bool(callbacks),bool(callbacks)and not completed)
        assert len(callbacks)<=1 and bytes(out)==bytes(new.uc.mem_read(nr,16)) and bytes(context)==old_context
        assert bytes(new.uc.mem_read(nc,24))==old_context
        after=bytearray(m.uc.mem_read(obj,64))
        for off,length in ((8,1),(0x14,1),(0x24,4),(0x2c,4)):old_obj[off:off+length]=after[off:off+length]=bytes(length)
        assert old_obj==after and old_rec==bytes(m.uc.mem_read(record,44))and old.word(obj+0x24)==record
        for ptr,length in ((ns,20),(nc,24),(nr,16)):assert bytes(new.uc.mem_read(ptr-16,16))==bytes(new.uc.mem_read(ptr+length,16))==b'\xa5'*16
        paths['eligible'if eligible else'level_mismatch'if not level_matches else'empty_population'if pop<=0 else'nonpositive_required']+=1
        completion_requests+=len(callbacks);compile_cases+=1
    ids=(-32768,-2,0,1,7,32767,2147483647,-2147483648)
    for _ in range(384):
        chars=[Character(rng.choice((0,1,255)),rng.choice(ids[:-2]),rng.choice((*ids[:-2],-1)))for _ in range(rng.randrange(17))]
        cache=[Cached(id,rng.choice((-2147483648,-1,0,1,2,2147483647)))for id in rng.sample(ids,rng.randrange(len(ids)+1))]
        world_fixture(chars,cache)
        for kind in (0,1):
            for id in ids:population(kind,id)
    bounds=(-2147483648,-1,0,1,2,2147483646,2147483647)
    for mode in range(4):
        world_fixture([Character(1,7,7)]*mode+[Character(0,7,7),Character(1,8,8)],[Cached(7,(-1,0,1,2147483647)[mode]),Cached(8,9)])
        for kind in range(4):
            for current in bounds:
                for required in bounds:
                    for record_level in (-1,7,9):
                        for active in (0,1,255):
                            for completed in (0,1,255):compile(kind,current,required,completed,active,7,record_level,7)
    real=json.loads((ROOT/'../quest-data/differential-validation.json').read_text(encoding='utf-8'));real_cases=0
    for row in real['rows']:
        for objective in row['lists'][1]:
            kind={0:0,1:1,10:2,11:3}.get(objective['common'][0])
            if kind is None:continue
            id,level_id=(objective['args'][1],objective['args'][0])if kind&1 else(objective['args'][0],objective['args'][1])
            for loaded in (0,1,3):
                safe=id if -32768<=id<=32767 and id!=-1 else 7
                world_fixture([Character(1,safe,safe)]*loaded,[Cached(id,2)])
                for current_level in (level_id,level_id+1 if level_id<2147483647 else 0):
                    compile(kind,0,objective['args'][2],0,0,id,level_id,current_level);real_cases+=1
    evidence=[]
    with io.BytesIO(a.original.read_bytes())as f:
        elf=ELFFile(f);symbols=[s for name in ('.dynsym','.symtab')for s in elf.get_section_by_name(name).iter_symbols()];loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address in (*COMPILE,*POPULATION,0x3a2f48,0x3b3d38,0x3b36ec,0x31f594,0x47ba10):
            s=next(s for s in symbols if s['st_value']==address and s['st_info']['type']=='STT_FUNC');seg=next(s for s in loads if s['p_vaddr']<=address< s['p_vaddr']+s['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr'])
            evidence.append({'elf_address':hex(address),'size':s['st_size'],'symbol':s.name,'sha256':hashlib.sha256(f.read(s['st_size'])).hexdigest()})
    report={'complete_game':False,'population_comparisons':population_cases,'compile_comparisons':compile_cases,'real_record_compile_cases':real_cases,'mismatches':0,'paths':dict(paths),'completion_requests':completion_requests,'population_calls':dict(pop_calls),'seed':20261002,
        'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
        'quest_reader_report_sha256':sha(ROOT/'../quest-data/differential-validation.json'),'source_sha256':{n:sha(ROOT/n)for n in ('compile.c','compile.h','../quest-kill/quest.h')},'function_evidence':evidence,
        'scope':'Actual loaded character list traversal, resolved signed-short ID accessors, native cached property map lookup, comparator identity, current-level getter, four quest Compile bodies and SetIsCompleted execute. Host/source ARM64 compare resolved population and scalar state/flags. Native Character loading/name resolution, list/cache lifecycle, observers/persistent save actions, quest world integration and complete source gameplay remain external; completion virtual callback is observed fixture with save ID -1.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('function_evidence','scope')}))
if __name__=='__main__':main()
