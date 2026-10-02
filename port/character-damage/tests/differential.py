#!/usr/bin/env python3
"""Original HitFor non-player paths versus owned C on host and ARM64.

Original IsDead/IsMonster/IsPlayer and AI indexing execute; AI records and all
manager/debug/config/network/death/attacker ownership are explicit fixtures.
"""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('state_test',ROOT/'../character-state/tests/differential.py')
state_test=importlib.util.module_from_spec(spec);spec.loader.exec_module(state_test)
base=state_test.base;State,Table=state_test.State,state_test.Table
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Policy(c.Structure):
    _fields_=[(n,c.c_int32 if n=='manager_mode'else c.c_uint32)for n in
        ('target_dead','target_monster','local_player_alive','online','manager_present','manager_mode',
         'monster_invincible','force_kill_config','force_kill_switch','target_network')]
class Hit(c.Structure):_fields_=[('death_reason',c.c_int32)]
class Result(c.Structure):_fields_=[('processed',c.c_uint32),('death_requested',c.c_uint32),('damage_whole',c.c_int32)]
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes()
    old=base.original.Original(a.original,a.oracle,raw);m=old.machine
    new=base.original.cpu.EngineCpu(a.arm64,True,base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));lib.dh2_property_open.argtypes=[c.POINTER(Table),c.c_void_p,c.c_uint32]
    lib.dh2_hit_nonplayer.argtypes=[c.POINTER(Table),c.POINTER(State),c.POINTER(Hit),c.POINTER(Policy),c.c_uint32,c.POINTER(Result)]
    buf=c.create_string_buffer(raw);table=Table();state=State();assert not lib.dh2_property_open(c.byref(table),buf,len(raw))
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nv=new.data+0x100;ns=new.data+0x8000
    nh=new.data+0x1000;policy_ptr=new.data+0x1100;no=new.data+0x1200
    new.uc.mem_write(np,raw);assert not new.call('dh2_property_open',[nv,np,len(raw)])
    char=m.data+0x3000;owner=char+0x560;vt=m.data+0x1000;guard=m.data+0x1100;online=m.data+0x2000
    manager=m.data+0x6000;player_record=m.data+0x7000;local_player=m.data+0x8000;ai=m.data+0xa000
    death_owner=m.data+0xb000;virtual=m.stop+0x180;kill_virtual=m.stop+0x184;offs=(8,0x38c,0x710,0xa94)
    kill_target=m.data+0xc000;kill_vt=m.data+0xd000
    got=0x3a8bdc+old.word(0x3a921c);assert got==0x994a98
    application=old.word(got+0x37f4)
    m.uc.mem_write(got+0x40ac,struct.pack('<I',guard));m.uc.mem_write(guard,struct.pack('<I',0x12345678))
    m.uc.mem_write(old.word(got+0x758),struct.pack('<I',ai));m.uc.mem_write(old.word(got+0x112c),struct.pack('<I',9))
    m.uc.mem_write(vt+0x28,struct.pack('<I',0x3a49f0));m.uc.mem_write(vt+0x34,struct.pack('<I',0x3a2ed4))
    m.uc.mem_write(vt+0x54,struct.pack('<I',virtual));m.uc.mem_write(local_player,struct.pack('<I',vt))
    m.uc.mem_write(death_owner+4,struct.pack('<I',kill_target));m.uc.mem_write(kill_target,struct.pack('<I',kill_vt))
    m.uc.mem_write(kill_vt+0x58,struct.pack('<I',kill_virtual))
    rng=random.Random(20261002);counts=Counter();boundaries=Counter();case={};deaths=[];observed=[]
    def boundary(uc,address,size,unused):
        boundaries[hex(address)]+=1
        if address==0x36e478:
            assert m.reg(0)==(manager if case['policy'].manager_present else 0) and m.reg(1)==0 and m.reg(2)==1
            m.write_reg(0,player_record)
        elif address==0x7fd794:m.write_reg(0,online)
        elif address==0x337a88:
            case['debug_calls']+=1
            m.write_reg(0,case['policy'].monster_invincible if case['debug_calls']==1 else case['policy'].force_kill_switch)
        elif address==0x320e14:m.write_reg(0,case['policy'].force_kill_config)
        elif address==virtual:
            assert m.reg(0)==char;m.write_reg(0,case['policy'].target_network)
        elif address==kill_virtual:
            assert [m.reg(i)for i in range(3)]==[kill_target,0,0];deaths.append(1)
        elif address==0x33dd2c:
            assert m.reg(1)==0
            observed.append(old.word(m.uc.reg_read(m.sp_reg)+0x10))
        elif address==0x33ff54:m.write_reg(0,0)
        uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
    for address in (0x36e478,0x7fd794,0x337888,0x3140ec,0x337a88,0x318254,0x320e14,
                    virtual,kill_virtual,0x33dd2c,0x33ff54):
        m.uc.hook_add(UC_HOOK_CODE,boundary,begin=address,end=address)
    defaults=old.tables[0]['rows'][0];types=old.tables[0]['rows'][1]
    def run(policy,damage,hp,property_type,label,local_missing=False):
        case.update(policy=policy,debug_calls=0);deaths.clear();observed.clear()
        m.uc.mem_write(char,b'\xa5'*0x1600);m.uc.mem_write(char,struct.pack('<I',vt))
        m.uc.mem_write(char+0x1449,bytes([policy.target_dead]));m.uc.mem_write(char+0x378,struct.pack('<I',death_owner))
        m.uc.mem_write(char+0x418,struct.pack('<I',0));m.uc.mem_write(local_player+0x1449,bytes([int(not policy.local_player_alive)]))
        # AI id is final property 1, including its real fallback-to-row-8 path.
        for row in range(9):m.uc.mem_write(ai+row*68+0x38,struct.pack('<I',4 if policy.target_monster else 2))
        m.uc.mem_write(application+0x40,struct.pack('<I',manager if policy.manager_present else 0))
        m.uc.mem_write(manager+0x714,struct.pack('<i',policy.manager_mode))
        m.uc.mem_write(player_record+0x660,struct.pack('<I',0 if local_missing else local_player))
        m.uc.mem_write(online,bytes(5)+bytes([policy.online]));hit=Hit(rng.randint(-100,100));out=Result(123,456,789)
        m.uc.mem_write(char+0x11c,bytes(hit))
        for name,off in zip(('base','saved','gears','final'),offs):
            sheet=getattr(state,name);sheet.values[:]=defaults
            sheet.values[36]=hp if name=='final'else rng.choice([hp,0,1,-1,-2147483648,2147483647,rng.randint(-100000,100000)])
            sheet.values[38]=rng.choice([-2147483648,2147483647,0,25600])
            m.uc.mem_write(owner+off+4,bytes(sheet))
        header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header))
        packed=struct.pack('<i',property_type)
        m.uc.mem_write(old.character_pointer+900+4+4*36,packed);c.memmove(c.addressof(buf)+900+4*36,packed,4);new.uc.mem_write(np+900+4*36,packed)
        before=bytearray(m.uc.mem_read(char,0x1600));new.uc.mem_write(ns-16,b'\xa5'*(c.sizeof(state)+32));new.uc.mem_write(ns,bytes(state))
        new.uc.mem_write(nh-16,b'\xa5'*36);new.uc.mem_write(nh,bytes(hit));new.uc.mem_write(no-16,b'\xa5'*44);new.uc.mem_write(no,bytes(out))
        new.uc.mem_write(policy_ptr,bytes(policy));old.invoke(0x3a8bc4,[char,damage,0])
        assert not lib.dh2_hit_nonplayer(c.byref(table),c.byref(state),c.byref(hit),c.byref(policy),damage,c.byref(out))
        assert not new.call('dh2_hit_nonplayer',[nv,ns,nh,policy_ptr,damage,no])
        expected=b''.join(bytes(m.uc.mem_read(owner+off+4,896))for off in offs)
        assert expected==bytes(state)==bytes(new.uc.mem_read(ns,c.sizeof(state))),(label,damage,hp,property_type,list(bytes(out)))
        assert bytes(hit)==bytes(m.uc.mem_read(char+0x11c,4))==bytes(new.uc.mem_read(nh,4)),label
        assert bytes(out)==bytes(new.uc.mem_read(no,12)),label
        assert out.processed==int(not policy.target_dead) and out.death_requested==len(deaths),label
        assert [out.damage_whole&0xffffffff] == observed if out.processed else not observed
        after=bytearray(m.uc.mem_read(char,0x1600))
        for off in offs:before[0x560+off+4:0x560+off+900]=after[0x560+off+4:0x560+off+900]=bytes(896)
        before[0x11c:0x120]=after[0x11c:0x120]=bytes(4)
        assert before==after,label
        for ptr,length in ((ns,c.sizeof(state)),(nh,4),(no,12)):
            assert bytes(new.uc.mem_read(ptr-16,16))==bytes(new.uc.mem_read(ptr+length,16))==b'\xa5'*16
        assert bytes(new.uc.mem_read(policy_ptr,c.sizeof(policy)))==bytes(policy)
        counts[label]+=1
    values=(0,1,255,256,257,25600,0x7fffffff,0x80000000,0xffffffff)
    policies=[Policy(0,1,1,0,1,0,0,0,0,0)]
    for key in ('target_dead','monster_invincible','force_kill_config','force_kill_switch','target_network'):
        pol=Policy.from_buffer_copy(bytes(policies[0]));setattr(pol,key,1);policies.append(pol)
    pol=Policy.from_buffer_copy(bytes(policies[0]));pol.local_player_alive=0;policies.append(pol)
    pol=Policy.from_buffer_copy(bytes(policies[2]));pol.target_monster=0;policies.append(pol)
    for mode in (-2147483648,-1,0,1,2,4,5,6,2147483647):
        for present in (0,1):policies.append(Policy(0,1,1,1,present,mode,0,0,0,0))
    for policy in policies:
        for hp in (-2147483648,-1,0,1,257,25600,2147483647):
            for damage in values:run(policy,damage,hp,types[36],'actual property type policy paths')
    # Missing and dead local player are independently exercised original paths.
    for hp in (-1,0,25600):
        for damage in values:run(Policy(0,1,0,0,1,0,0,0,0,0),damage,hp,types[36],'missing local player',True)
    for flag in range(-1,64):
        for damage in values:
            run(policies[0],damage,rng.choice([-2147483648,-1,0,1,25600,2147483647]),flag,'synthetic property flag paths')
            run(Policy(0,1,1,0,1,0,0,1,0,0),damage,25600,flag,'synthetic property forced kill paths')
    evidence=[]
    with a.original.open('rb')as f:
        elf=ELFFile(f);syms=list(elf.get_section_by_name('.dynsym').iter_symbols());loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address in (0x3a8bc4,0x3a2ed4,0x3a2fec,0x3a3024,0x3a3054,0x3a3064,0x3a49f0,0x40570c,0x3e0708,0x3e07a0,0x3dfe60):
            symbol=next(s for s in syms if s['st_value']==address);n=symbol['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);evidence.append({'elf_address':hex(address),'size':n,'symbol':symbol.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    report={'complete_game':False,'full_hitfor_port':False,'comparisons':sum(counts.values()),'cases':dict(counts),'mismatches':0,'seed':20261002,
        'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
        'cache_sha256':hashlib.sha256(raw).hexdigest(),'source_sha256':{n:sha(ROOT/n)for n in ('damage.c','damage.h')},'function_evidence':evidence+old.evidence,
        'whole_four_sheet_state_comparison':True,'reserved_fields_preserved':True,'death_callback_and_reason_compared':True,'damage_stack_local_compared':True,
        'output_guards':True,'stack_restoration':True,'boundary_calls':dict(boundaries),'imports':m.import_calls,
        'scope':'Actual original complete HitFor entry executes non-player paths with null/unresolved attacker. Actual IsDead/IsMonster/IsPlayer and AI index/type getters execute on fixture AI records. Original property writes and recalculation execute empty buffs. Host and source ARM64 match all four sheets, death reason, death request count and damage stack local. Manager lookup, online/config/debug/virtual network, string lifetimes, death owner and attacker handle are explicit boundary models. No original Character construction, player health warnings/audio, resolved attacker achievements, actual death state transition, world, full combat dispatch or native Android gameplay is claimed.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('function_evidence','scope','boundary_calls','imports')}))
if __name__=='__main__':main()
