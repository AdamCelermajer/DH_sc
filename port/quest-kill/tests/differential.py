#!/usr/bin/env python3
"""Run all four original kill-objective handlers against host/source ARM64."""
import argparse,ctypes as c,hashlib,importlib.util,io,json,random,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('death_test',ROOT/'../character-death/tests/differential.py')
death=importlib.util.module_from_spec(spec);spec.loader.exec_module(death)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Objective(c.Structure):_fields_=[('match_id',c.c_int32),('current',c.c_int32),('required',c.c_int32),('completed',c.c_uint32)]
class Event(c.Structure):_fields_=[('match_id',c.c_int32),('quantity',c.c_int32),('outbound',c.c_uint32),('synchronized',c.c_uint32)]
class Result(c.Structure):_fields_=[(n,c.c_uint32)for n in ('matched','changed','completion_requested','newly_completed')]
HANDLERS=(0x47f100,0x47f228,0x47f2bc,0x47f350)
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes()
    old=death.base.original.Original(death.BufferedElfPath(a.original),death.BufferedElfPath(a.oracle),raw);m=old.machine
    new=death.base.original.cpu.EngineCpu(death.BufferedElfPath(a.arm64),True,death.base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_quest_kill_event.argtypes=[c.POINTER(Objective),c.POINTER(Event),c.POINTER(Result)]
    obj=m.data+0x3000;event=m.data+0x4000;record=m.data+0x5000;vt=m.data+0x6000;callback=m.stop+0x180
    ns=new.data+0x100;ne=new.data+0x200;no=new.data+0x300;callbacks=[]
    def boundary(uc,address,size,unused):
        assert m.reg(0)==obj;callbacks.append(1);uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
    m.uc.hook_add(UC_HOOK_CODE,boundary,begin=callback,end=callback)
    m.uc.mem_write(vt+0x1c,struct.pack('<I',callback))
    rng=random.Random(20261002);count=0;paths={'local':0,'synchronized_increase':0,'synchronized_stale':0,'nonmatching':0};completions=0
    def run(address,s,e):
        nonlocal count,completions
        before_s=bytes(s);before_e=bytes(e);callbacks.clear()
        m.uc.mem_write(obj,b'\xa5'*64);m.uc.mem_write(event,b'\xa5'*32);m.uc.mem_write(record,b'\xa5'*64)
        m.uc.mem_write(obj,struct.pack('<I',vt));m.uc.mem_write(obj+0xc,struct.pack('<I',record));m.uc.mem_write(obj+0x14,bytes([s.completed]))
        m.uc.mem_write(obj+0x20,struct.pack('<iI',s.current,record));m.uc.mem_write(obj+0x2c,struct.pack('<i',s.required));m.uc.mem_write(record+0xc,struct.pack('<i',-1))
        m.uc.mem_write(record+(0x24 if address in (0x47f228,0x47f350)else 0x20),struct.pack('<i',s.match_id))
        m.uc.mem_write(event+0x10,bytes([e.outbound,e.synchronized]));m.uc.mem_write(event+0x14,struct.pack('<ii',e.quantity,e.match_id))
        old_s=bytearray(m.uc.mem_read(obj,64));old_e=bytearray(m.uc.mem_read(event,32));old_record=bytes(m.uc.mem_read(record,64))
        for ptr,value in ((ns,before_s),(ne,before_e),(no,b'\xa5'*16)):new.uc.mem_write(ptr-16,b'\xa5'*(len(value)+32));new.uc.mem_write(ptr,value)
        out=Result();assert old.invoke(address,[obj,event,0])==0
        assert not lib.dh2_quest_kill_event(c.byref(s),c.byref(e),c.byref(out));assert not new.call('dh2_quest_kill_event',[ns,ne,no])
        expected_s=Objective(s.match_id,c.c_int32(old.word(obj+0x20)).value,s.required,bytes(m.uc.mem_read(obj+0x14,1))[0])
        flags=bytes(m.uc.mem_read(event+0x10,2));expected_e=Event(e.match_id,c.c_int32(old.word(event+0x14)).value,*flags)
        assert bytes(s)==bytes(expected_s)==bytes(new.uc.mem_read(ns,16))and bytes(e)==bytes(expected_e)==bytes(new.uc.mem_read(ne,16)),(count,hex(address),before_s.hex(),before_e.hex(),bytes(s).hex(),bytes(expected_s).hex(),bytes(e).hex(),bytes(expected_e).hex(),bytes(new.uc.mem_read(ns,16)).hex(),bytes(new.uc.mem_read(ne,16)).hex())
        initial_s=Objective.from_buffer_copy(before_s);initial_e=Event.from_buffer_copy(before_e)
        match=initial_s.match_id==initial_e.match_id;changed=match and(not initial_e.synchronized or initial_e.quantity>initial_s.current)
        assert(out.matched,out.changed,out.completion_requested,out.newly_completed)==(match,changed,bool(callbacks),bool(callbacks)and not initial_s.completed)
        assert len(callbacks)<=1 and bytes(out)==bytes(new.uc.mem_read(no,16));completions+=len(callbacks)
        after_s=bytearray(m.uc.mem_read(obj,64));after_e=bytearray(m.uc.mem_read(event,32))
        for off,length in ((0x14,1),(0x20,4)):old_s[off:off+length]=after_s[off:off+length]=bytes(length)
        for off,length in ((0x10,1),(0x14,4)):old_e[off:off+length]=after_e[off:off+length]=bytes(length)
        assert old_s==after_s and old_e==after_e and old_record==bytes(m.uc.mem_read(record,64))
        for ptr in (ns,ne,no):assert bytes(new.uc.mem_read(ptr-16,16))==bytes(new.uc.mem_read(ptr+16,16))==b'\xa5'*16
        path='nonmatching'if not match else'local'if not initial_e.synchronized else'synchronized_increase'if changed else'synchronized_stale'
        paths[path]+=1;count+=1
    bounds=(-2147483648,-1,0,1,2,2147483646,2147483647)
    for address in HANDLERS:
        for current in bounds:
            for required in bounds:
                for quantity in bounds:
                    for synchronized in (0,1,255):
                        for completed in (0,1,255):
                            run(address,Objective(7,current,required,completed),Event(7,quantity,rng.randrange(256),synchronized))
        for _ in range(512):
            run(address,Objective(rng.randint(-100,100),c.c_int32(rng.getrandbits(32)).value,c.c_int32(rng.getrandbits(32)).value,rng.randrange(256)),
                Event(rng.randint(-100,100),c.c_int32(rng.getrandbits(32)).value,rng.randrange(256),rng.randrange(256)))
    evidence=[]
    with io.BytesIO(a.original.read_bytes())as f:
        elf=ELFFile(f);syms=list(elf.get_section_by_name('.dynsym').iter_symbols());loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address in (*HANDLERS,0x47ba10):
            symbol=next(s for s in syms if s['st_value']==address);seg=next(s for s in loads if s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);evidence.append({'elf_address':hex(address),'size':symbol['st_size'],'symbol':symbol.name,'sha256':hashlib.sha256(f.read(symbol['st_size'])).hexdigest()})
    report={'complete_game':False,'comparisons':count,'mismatches':0,'paths':paths,'completion_requests':completions,'seed':20261002,
        'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
        'source_sha256':{n:sha(ROOT/n)for n in ('quest.c','quest.h')},'function_evidence':evidence,'imports':m.import_calls,
        'scope':'Four already-dispatched native kill/clear property/template event handlers and SetIsCompleted execute. Signed wrapping counters, local outbound flag/quantity, synchronized max updates, completed byte, repeated completion callbacks and reserved fields compare. Record persistence id -1 avoids external save action; completion virtual callback is observed fixture. Dispatch, compile/world counts, marker/event ownership, actual quest data loading/persistence/rewards and full source gameplay remain unfinished.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('function_evidence','imports','scope')}))
if __name__=='__main__':main()
