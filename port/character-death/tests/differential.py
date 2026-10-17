#!/usr/bin/env python3
"""Original null-killer non-player Kill versus owned source on host/ARM64."""
import argparse,ctypes as c,hashlib,importlib.util,io,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('state_test',ROOT/'../character-state/tests/differential.py')
state_test=importlib.util.module_from_spec(spec);spec.loader.exec_module(state_test)
base=state_test.base;State,Table=state_test.State,state_test.Table
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class BufferedElfPath(type(Path())):
    # Preserve ELF bytes; keep random parser seeks in RAM rather than across
    # the WSL/Windows filesystem boundary. No executable bytes are patched.
    def open(self,mode='r',*args,**kw):
        if mode=='rb':
            with super().open(mode,*args,**kw)as f:return io.BytesIO(f.read())
        return super().open(mode,*args,**kw)
class Actor(c.Structure):_fields_=[(n,c.c_uint32)for n in ('dead','network','suppress_events','target_id')]+[(n,c.c_int32)for n in ('property_id','template_id')]
class Policy(c.Structure):_fields_=[('forced',c.c_uint32),('loot_manager_present',c.c_uint32),('objective_ids',c.c_int32*4)]
class Event(c.Structure):_fields_=[('kind',c.c_uint32),('target_id',c.c_uint32),('objective_id',c.c_int32),('match_id',c.c_int32)]
class Result(c.Structure):_fields_=[('processed',c.c_uint32),('drop_loot_requested',c.c_uint32),('event_count',c.c_uint32),('drop_loot_id',c.c_int32),('events',Event*4)]
KEYS=('KillXEnemies','ClearEnemies','KillEnemyTemplate','ClearEnemyTemplate')
VT=('_ZTV14QE_KillEnemies','_ZTV15QE_ClearEnemies','_ZTV20QE_KillEnemyTemplate','_ZTV21QE_ClearEnemyTemplate')
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes()
    old=base.original.Original(BufferedElfPath(a.original),BufferedElfPath(a.oracle),raw);m=old.machine
    new=base.original.cpu.EngineCpu(BufferedElfPath(a.arm64),True,base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32]
    lib.dh2_death_nonplayer.argtypes=[c.POINTER(Table),c.POINTER(State),c.POINTER(Actor),c.POINTER(Policy),c.POINTER(Result)]
    buf=c.create_string_buffer(raw);table=Table();state=State();assert not lib.dh2_property_open(c.byref(table),buf,len(raw))
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nv=new.data+0x100;ns=new.data+0x8000
    na=new.data+0x1000;policy_ptr=new.data+0x1100;no=new.data+0x1200
    new.uc.mem_write(np,raw);assert not new.call('dh2_property_open',[nv,np,len(raw)])
    char=m.data+0x3000;owner=char+0x560;vt=m.data+0x1000;level=m.data+0x6000;ai=m.data+0xa000
    virtual=m.stop+0x180;offs=(8,0x38c,0x710,0xa94);got=0x994a98
    m.uc.mem_write(old.word(got+0x758),struct.pack('<I',ai));m.uc.mem_write(old.word(got+0x112c),struct.pack('<I',9))
    m.uc.mem_write(old.word(got+0x1d64),struct.pack('<I',level))
    m.uc.mem_write(vt+0x28,struct.pack('<I',0x3a49f0));m.uc.mem_write(vt+0x34,struct.pack('<I',0x3a2ed4))
    m.uc.mem_write(vt+0x54,struct.pack('<I',virtual))
    vtables={old.symbols[n]['st_value']+8:kind for kind,n in enumerate(VT)}
    rng=random.Random(20261002);counts=Counter();boundaries=Counter();case={};events=[];drops=[];lookups=[]
    def string(ptr):return bytes(m.uc.mem_read(ptr,96)).split(b'\0')[0].decode('ascii')
    def boundary(uc,address,size,unused):
        boundaries[hex(address)]+=1
        if address==virtual:
            assert m.reg(0)==char;m.write_reg(0,case['actor'].network)
        elif address==0x3ecba0:
            assert [m.reg(i)for i in (1,2,3)]==[char,0,0xffffffff]and old.word(m.uc.reg_read(m.sp_reg))==0
            drops.append(c.c_int32(m.reg(0)).value)
        elif address==0x4c4bdc:
            assert string(m.reg(1))=='v2QuestObjectiveType';key=string(m.reg(2));kind=KEYS.index(key);lookups.append(kind)
            m.write_reg(0,case['policy'].objective_ids[kind]&0xffffffff)
        elif address==0x338ebc:
            assert m.reg(0)==level;event=bytes(uc.mem_read(m.reg(1),28));vtable,objective,killer,target=struct.unpack_from('<4I',event)
            assert killer==0 and event[16:18]==bytes(2)and struct.unpack_from('<i',event,20)[0]==-1
            events.append((vtables[vtable],target,c.c_int32(objective).value,struct.unpack_from('<i',event,24)[0]))
        uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
    for address in (virtual,0x3ecba0,0x4c4bdc,0x338ebc):m.uc.hook_add(UC_HOOK_CODE,boundary,begin=address,end=address)
    defaults=old.tables[0]['rows'][0];types=old.tables[0]['rows'][1]
    def run(actor,policy,hp,property_type,label,monster):
        case.update(actor=actor,policy=policy)
        m.uc.mem_write(char,b'\xa5'*0x1600);m.uc.mem_write(char,struct.pack('<I',vt))
        m.uc.mem_write(char+0x1449,bytes([actor.dead]));m.uc.mem_write(char+0x14e4,bytes([actor.suppress_events]))
        m.uc.mem_write(char+0x64,struct.pack('<I',actor.target_id));m.uc.mem_write(char+0x13c8,struct.pack('<hh',actor.property_id,actor.template_id))
        for row in range(9):m.uc.mem_write(ai+row*68+0x38,struct.pack('<I',4 if monster else 2))
        m.uc.mem_write(level+0x150,struct.pack('<I',policy.loot_manager_present))
        for name,off in zip(('base','saved','gears','final'),offs):
            sheet=getattr(state,name);sheet.values[:]=defaults;sheet.values[36]=hp if name=='final'else rng.choice([hp,0,1,-1,-2147483648,2147483647])
            sheet.values[9]=rng.choice([-2147483648,2147483647,-1,0,1,255])
            m.uc.mem_write(owner+off+4,bytes(sheet))
        header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header))
        packed=struct.pack('<i',property_type)
        m.uc.mem_write(old.character_pointer+900+4+4*36,packed);c.memmove(c.addressof(buf)+900+4*36,packed,4);new.uc.mem_write(np+900+4*36,packed)
        new.uc.mem_write(ns-16,b'\xa5'*(c.sizeof(state)+32));new.uc.mem_write(ns,bytes(state))
        new.uc.mem_write(na-16,b'\xa5'*(c.sizeof(actor)+32));new.uc.mem_write(na,bytes(actor));new.uc.mem_write(policy_ptr,bytes(policy))
        for phase in range(2):
            events.clear();drops.clear();lookups.clear();before=bytearray(m.uc.mem_read(char,0x1600));original_dead=before[0x1449]
            out=Result();c.memset(c.byref(out),0xa5,c.sizeof(out));new.uc.mem_write(no-16,b'\xa5'*(c.sizeof(out)+32))
            old.invoke(0x3a5b18,[char,0,policy.forced])
            assert not lib.dh2_death_nonplayer(c.byref(table),c.byref(state),c.byref(actor),c.byref(policy),c.byref(out))
            assert not new.call('dh2_death_nonplayer',[nv,ns,na,policy_ptr,no])
            expected=b''.join(bytes(m.uc.mem_read(owner+off+4,896))for off in offs)
            assert expected==bytes(state)==bytes(new.uc.mem_read(ns,c.sizeof(state))),(label,phase,hp,property_type)
            assert bytes(actor)==bytes(new.uc.mem_read(na,c.sizeof(actor)))and actor.dead==old.word(char+0x1448)>>8&255
            assert out.processed==int(not original_dead)and out.drop_loot_requested==len(drops)and out.event_count==len(events)
            assert out.drop_loot_id==(drops[0]if drops else 0)
            assert events==[(e.kind,e.target_id,e.objective_id,e.match_id)for e in out.events[:out.event_count]]
            assert lookups==[e[0]for e in events]
            assert bytes(out)==bytes(new.uc.mem_read(no,c.sizeof(out)))
            after=bytearray(m.uc.mem_read(char,0x1600))
            for off in offs:before[0x560+off+4:0x560+off+900]=after[0x560+off+4:0x560+off+900]=bytes(896)
            before[0x1449]=after[0x1449]=0;assert before==after,label
            for ptr,length in ((ns,c.sizeof(state)),(na,c.sizeof(actor)),(no,c.sizeof(out))):
                assert bytes(new.uc.mem_read(ptr-16,16))==bytes(new.uc.mem_read(ptr+length,16))==b'\xa5'*16
            assert bytes(new.uc.mem_read(policy_ptr,c.sizeof(policy)))==bytes(policy)
            counts[label if phase==0 else'repeated already dead calls']+=1
    ids=(-32768,-1,0,1,32767)
    for flags in range(32):
        for enemy in ids:
            for template in ids:
                actor=Actor(flags&1,(flags>>1)&1,(flags>>2)&1,rng.getrandbits(32),enemy,template)
                policy=Policy((flags>>3)&1,(flags>>4)&1,(c.c_int32*4)(*[rng.randint(-100,100)for _ in range(4)]))
                run(actor,policy,rng.choice([-2147483648,-1,0,25600,2147483647]),types[36],'actual property type death/event paths',flags&1)
    for flag in range(-1,64):
        for hp in (-2147483648,-1,0,1,257,25600,2147483647):
            run(Actor(0,0,0,0xffffffff,-1,32767),Policy(0,0,(c.c_int32*4)(0,1,-1,-2147483648)),hp,flag,'synthetic property flag death paths',1)
    evidence=[]
    with io.BytesIO(a.original.read_bytes())as f:
        elf=ELFFile(f);syms=list(elf.get_section_by_name('.dynsym').iter_symbols());loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address in (0x3a5b18,0x3a2ed4,0x3a49f0,0x3a2fcc,0x3a5ae4,0x31f594,0x339090,0x3e07a0,0x3dfe60):
            symbol=next(s for s in syms if s['st_value']==address);n=symbol['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);evidence.append({'elf_address':hex(address),'size':n,'symbol':symbol.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    report={'complete_game':False,'full_kill_port':False,'comparisons':sum(counts.values()),'cases':dict(counts),'mismatches':0,'seed':20261002,
        'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
        'cache_sha256':hashlib.sha256(raw).hexdigest(),'source_sha256':{n:sha(ROOT/n)for n in ('death.c','death.h')},'function_evidence':evidence+old.evidence,
        'whole_four_sheet_state_comparison':True,'reserved_fields_preserved':True,'dead_flag_and_loot_request_compared':True,'ordered_event_payloads_compared':True,
        'output_guards':True,'stack_restoration':True,'boundary_calls':dict(boundaries),'imports':m.import_calls,
        'scope':'Actual original Kill executes non-player/null-killer paths. IsDead/IsPlayer/AI indexing, HP Set/Recalc, GetCurrentLevel, DropLoot/GetLoot and RaiseAsync forwarding execute. Host/source ARM64 compare four sheets, dead flag, loot request/id and ordered quest event kind/objective/target/match IDs; native flags zero, killer null and -1 value checked. AI records, current level, loot manager presence, constant lookup, network virtual, Loot::Drop and event queue are fixtures. Native event padding/vtable bytes are represented by source semantic kinds, not copied. Player death counters, killer/threat list credit, actual loot generation, event consumers, world/FSM, progression/save and full source gameplay remain unfinished.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('function_evidence','scope','boundary_calls','imports')}))
if __name__=='__main__':main()
