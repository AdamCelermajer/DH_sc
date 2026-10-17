#!/usr/bin/env python3
"""Actual original health/mana bodies versus owned source ARM64 and host."""
import argparse,ctypes as c,hashlib,importlib.util,json,math,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('state_test',ROOT/'../character-state/tests/differential.py')
state_test=importlib.util.module_from_spec(spec);spec.loader.exec_module(state_test)
base=state_test.base;State,Table=state_test.State,state_test.Table
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
ROWS=[(0x3bd0c0,'SetMP'),(0x3bd0d0,'SetHP'),(0x3bd0e0,'GetTotalMP'),(0x3bd0f0,'GetTotalHP'),
      (0x3bd100,'GetMP'),(0x3bd110,'GetHP'),(0x3bd140,'ValidateHPMP'),(0x3bd2dc,'GetHPPercent'),
      (0x3bd338,'GetMPPercent'),(0x3bdbb8,'RegenMP'),(0x3bdca4,'RegenHP'),
      (0x3b6cd8,'script GetHP'),(0x3bd40c,'HasMana'),(0x3bdef4,'UseMana'),
      (0x3b7774,'script RegenMP'),(0x3b77ec,'script RegenHP'),(0x3b7864,'script UseMana'),(0x3b78ec,'script HasMana')]
class Dependencies(base.Dependencies):
    def call(self,m,name):
        n=name.removeprefix('__aeabi_')
        if n=='i2f':m.put_float(c.c_int32(m.reg(0)).value,'f')
        elif n=='f2iz':
            v=m.get_float('f',0);assert math.isfinite(v) and -2147483648<=v<2147483648
            m.write_reg(0,int(v)&0xffffffff)
        elif n in ('idiv','idivmod'):
            x,y=[c.c_int32(m.reg(i)).value for i in range(2)];assert y and (x,y)!=(-2147483648,-1)
            q=(abs(x)//abs(y))*(-1 if (x<0)!=(y<0) else 1)
            m.write_reg(0,q&0xffffffff)
            if n=='idivmod':m.write_reg(1,(x-q*y)&0xffffffff)
        else:return super().call(m,name)
        m.uc.reg_write(m.pc_reg,m.uc.reg_read(m.lr_reg))
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes()
    dep=base.original.cpu.Dependencies;base.original.cpu.Dependencies=Dependencies
    try:old=base.original.Original(a.original,a.oracle,raw)
    finally:base.original.cpu.Dependencies=dep
    m=old.machine;new=base.original.cpu.EngineCpu(a.arm64,True,Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()));tp=c.POINTER(Table);sp=c.POINTER(State);u=c.c_uint32;i=c.c_int32;ip=c.POINTER(i)
    lib.dh2_property_open.argtypes=[tp,c.c_void_p,u]
    specs={'set':[tp,sp,u,i],'get':[sp,u,u,ip],'validate':[tp,sp],'regen':[tp,sp,u,i],
           'fraction':[sp,u,c.POINTER(c.c_float)],'script_hp':[sp,ip],'has_mana':[sp,i,u,ip],'use_mana':[tp,sp,i,u,ip]}
    for name,args in specs.items():getattr(lib,'dh2_health_'+name).argtypes=args
    buf=c.create_string_buffer(raw);table=Table();state=State();assert not lib.dh2_property_open(c.byref(table),buf,len(raw))
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nv=new.data+0x100;ns=new.data+0x8000;no=new.data+0x1000
    new.uc.mem_write(np,raw);assert not new.call('dh2_property_open',[nv,np,len(raw)])
    character=m.data+0x3000;owner=character+0x560;offs=(8,0x38c,0x710,0xa94);rng=random.Random(20261002);calls=Counter();boundaries=Counter()
    online,vt,guard,arguments,vector,values,returns=[m.data+x for x in (0x2000,0x1000,0xa000,0x6000,0x6100,0x7000,0x9000)]
    mode={'online':0,'network':0,'config':0,'switch':0,'actor':0};emitted=[];virtual=m.stop+0x180
    m.uc.mem_write(vt+0x54,struct.pack('<I',virtual));m.uc.mem_write(guard,struct.pack('<I',0x12345678));m.uc.mem_write(arguments,struct.pack('<II',0,vector))
    with a.original.open('rb')as f:
        elf=ELFFile(f);syms=list(elf.get_section_by_name('.dynsym').iter_symbols());loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD'];evidence=[]
        for address,label in ROWS:
            symbol=next(s for s in syms if s['st_value']==address);n=symbol['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address< s['p_vaddr']+s['p_filesz'])
            f.seek(seg['p_offset']+address-seg['p_vaddr']);evidence.append({'elf_address':hex(address),'size':n,'symbol':symbol.name,'label':label,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
        for section in elf.iter_sections():
            if section['sh_type']=='SHT_REL':
                symbols=elf.get_section(section['sh_link'])
                for rel in section.iter_relocations():
                    if rel['r_info_type']==21 and symbols.get_symbol(rel['r_info_sym']).name=='__stack_chk_guard':m.uc.mem_write(rel['r_offset'],struct.pack('<I',guard))
    def boundary(uc,address,size,unused):
        boundaries[hex(address)]+=1
        if address==0x7fd794:m.write_reg(0,online)
        elif address==virtual:
            assert m.reg(0)==character;m.write_reg(0,mode['network'])
        elif address==0x320e14:m.write_reg(0,mode['config'])
        elif address==0x337a88:m.write_reg(0,mode['switch'])
        elif address==0x37cb24:
            assert m.reg(0)==returns;emitted.append(('int',m.reg(1)))
        elif address==0x37c7e4:
            assert m.reg(0)==returns;emitted.append(('bool',m.reg(1)))
        # Debug Load, temporary string constructor/destructor are no-op boundaries.
        uc.reg_write(m.pc_reg,uc.reg_read(m.lr_reg))
    for addr in (0x7fd794,virtual,0x320e14,0x337888,0x3140ec,0x337a88,0x318254,0x37cb24,0x37c7e4):m.uc.hook_add(UC_HOOK_CODE,boundary,begin=addr,end=addr)
    defaults=old.tables[0]['rows'][0];types=old.tables[0]['rows'][1]
    def prepare(type=None):
        m.uc.mem_write(character,b'\xa5'*0x1600);m.uc.mem_write(character,struct.pack('<I',vt));m.uc.mem_write(character+0x14f0,bytes([mode['actor']]))
        m.uc.mem_write(online,bytes(5)+bytes([mode['online']]))
        for name,off in zip(('base','saved','gears','final'),offs):
            sheet=getattr(state,name);sheet.values[:]=defaults
            for field in (36,38,41,43):sheet.values[field]=rng.choice([-2147483648,2147483647,-1,0,1,257,rng.randint(-100000,100000)])
            m.uc.mem_write(owner+off+4,bytes(sheet))
        header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header));new.uc.mem_write(ns-16,b'\xa5'*(c.sizeof(state)+32));new.uc.mem_write(ns,bytes(state))
        for field in (36,41):
            value=types[field]if type is None else type;packed=struct.pack('<i',value)
            m.uc.mem_write(old.character_pointer+900+4+4*field,packed);c.memmove(c.addressof(buf)+900+4*field,packed,4);new.uc.mem_write(np+900+4*field,packed)
    def compare_state(before,label):
        expected=b''.join(bytes(m.uc.mem_read(owner+off+4,896))for off in offs)
        assert expected==bytes(state)==bytes(new.uc.mem_read(ns,c.sizeof(state))),label
        after=bytearray(m.uc.mem_read(character,0x1600));masked=bytearray(before)
        for off in offs:masked[0x560+off+4:0x560+off+900]=after[0x560+off+4:0x560+off+900]=bytes(896)
        assert masked==after,label
        assert bytes(new.uc.mem_read(ns-16,16))==bytes(new.uc.mem_read(ns+c.sizeof(state),16))==b'\xa5'*16
    def mutation(name,address,extra,host_extra,arm_extra):
        before=bytes(m.uc.mem_read(character,0x1600));old.invoke(address,[character,*extra])
        assert not getattr(lib,'dh2_health_'+name)(c.byref(table),c.byref(state),*host_extra)
        assert not new.call('dh2_health_'+name,[nv,ns,*arm_extra]);compare_state(before,(name,extra));calls[name]+=1
    values_to_set=(-2147483648,2147483647,-1,0,1,257,8388608,16777216)
    for type in [None,*range(-1,64)]:
        for delta in values_to_set:
            prepare(type)
            for channel,address in ((0,0x3bd0d0),(1,0x3bd0c0)):
                mutation('set',address,[delta],[channel,delta],[channel,delta&0xffffffff])
            mutation('validate',0x3bd140,[],[],[])
            for channel,address in ((0,0x3bdca4),(1,0x3bdbb8)):
                mutation('regen',address,[delta],[channel,delta],[channel,delta&0xffffffff])
    for iteration in range(200):
        prepare();before=bytes(m.uc.mem_read(character,0x1600))
        for channel,total,address in ((0,0,0x3bd110),(1,0,0x3bd100),(0,1,0x3bd0f0),(1,1,0x3bd0e0)):
            expected=old.invoke(address,[character]);out=i();new.uc.mem_write(no,b'\xa5'*32)
            assert not lib.dh2_health_get(c.byref(state),channel,total,c.byref(out));assert not new.call('dh2_health_get',[ns,channel,total,no])
            assert expected==(out.value&0xffffffff)==struct.unpack('<I',new.uc.mem_read(no,4))[0];assert bytes(new.uc.mem_read(no+4,28))==b'\xa5'*28;calls['get']+=1
        for channel,address in ((0,0x3bd2dc),(1,0x3bd338)):
            expected=old.invoke(address,[character]);out=c.c_float();new.uc.mem_write(no,b'\xa5'*32)
            assert not lib.dh2_health_fraction(c.byref(state),channel,c.byref(out));assert not new.call('dh2_health_fraction',[ns,channel,no])
            actual=struct.unpack('<I',bytes(out))[0];arm=struct.unpack('<I',new.uc.mem_read(no,4))[0]
            assert expected==actual==arm or all(math.isnan(struct.unpack('<f',struct.pack('<I',v))[0])for v in (expected,actual,arm)),(channel,expected,actual,arm)
            assert bytes(new.uc.mem_read(no+4,28))==b'\xa5'*28;calls['fraction']+=1
        hp,maxhp=state.final.values[36],state.final.values[38];whole=maxhp>>8;product=c.c_int32(hp*100).value
        if not maxhp or (whole and (product,whole)!=(-2147483648,-1)):
            emitted.clear();old.invoke(0x3b6cd8,[arguments,returns,character]);out=(i*3)();new.uc.mem_write(no,b'\xa5'*32)
            assert not lib.dh2_health_script_hp(c.byref(state),out);assert not new.call('dh2_health_script_hp',[ns,no])
            assert emitted==[('int',v&0xffffffff)for v in out];assert bytes(out)==bytes(new.uc.mem_read(no,12));assert bytes(new.uc.mem_read(no+12,20))==b'\xa5'*20;calls['script_hp']+=1
        compare_state(before,'readers')
    # Explicit external policy inputs; original online virtual/config/debug/flag
    # decisions execute against fixed non-mutating boundary fixtures.
    modes=[dict(online=0,network=0,config=0,switch=0,actor=0)]
    for key in ('network','config','switch','actor'):
        modes.append(dict(online=1 if key=='network'else 0,network=key=='network',config=key=='config',switch=key=='switch',actor=key=='actor'))
    modes.append(dict(online=1,network=0,config=0,switch=0,actor=0))
    for settings in modes:
        mode.update(settings)
        for cost in values_to_set:
            prepare();before=bytes(m.uc.mem_read(character,0x1600));out=i();new.uc.mem_write(no,b'\xa5'*32)
            exempt=int(bool(mode['online']and mode['network']));expected=old.invoke(0x3bd40c,[character,cost])
            assert not lib.dh2_health_has_mana(c.byref(state),cost,exempt,c.byref(out));assert not new.call('dh2_health_has_mana',[ns,cost&0xffffffff,exempt,no])
            assert expected==out.value==struct.unpack('<I',new.uc.mem_read(no,4))[0];calls['has_mana']+=1
            exempt=int(bool(exempt or mode['config']or mode['switch']or mode['actor']));expected=old.invoke(0x3bdef4,[character,cost])
            assert not lib.dh2_health_use_mana(c.byref(table),c.byref(state),cost,exempt,c.byref(out));assert not new.call('dh2_health_use_mana',[nv,ns,cost&0xffffffff,exempt,no])
            assert expected==out.value==struct.unpack('<I',new.uc.mem_read(no,4))[0];assert bytes(new.uc.mem_read(no+4,28))==b'\xa5'*28;compare_state(before,'mana');calls['use_mana']+=1
    # Actual first-numeric-argument callbacks, no argument/tag => no result.
    mode.update(online=0,network=0,config=0,switch=0,actor=0)
    callback_cases=[([],[]),([1],[1]),([1],[0]),([1],[4])]+[([v],[3])for v in (-2147483648.,2147483520.,-257.9,-1.9,0,1.9,257.9,100000.)]+[([123.9,7,8],[3,1,0])]
    for op,address in (('regen_hp',0x3b77ec),('regen_mp',0x3b7774),('has_mana',0x3b78ec),('use_mana',0x3b7864)):
        for args,tags in callback_cases:
            prepare();before=bytes(m.uc.mem_read(character,0x1600));packed=bytearray(112*len(args))
            for index,(v,t)in enumerate(zip(args,tags)):struct.pack_into('<If',packed,112*index+4,t,v)
            m.uc.mem_write(vector,struct.pack('<II',values,values+len(packed)));m.uc.mem_write(values,bytes(packed)or bytes(4));emitted.clear();old.invoke(address,[arguments,returns,character])
            out=i();new.uc.mem_write(no,b'\xa5'*32)
            if args and tags[0]==3:
                cost=int(c.c_float(args[0]).value)
                if op.startswith('regen'):
                    channel=int(op=='regen_mp');assert not lib.dh2_health_regen(c.byref(table),c.byref(state),channel,cost)
                    assert not new.call('dh2_health_regen',[nv,ns,channel,cost&0xffffffff]);assert not emitted
                elif op=='has_mana':
                    assert not lib.dh2_health_has_mana(c.byref(state),cost,0,c.byref(out));assert not new.call('dh2_health_has_mana',[ns,cost&0xffffffff,0,no]);assert emitted==[('bool',out.value)]
                else:
                    assert not lib.dh2_health_use_mana(c.byref(table),c.byref(state),cost,0,c.byref(out));assert not new.call('dh2_health_use_mana',[nv,ns,cost&0xffffffff,0,no]);assert emitted==[('bool',out.value)]
            else:assert not emitted
            assert bytes(m.uc.mem_read(values,len(packed)))==bytes(packed);compare_state(before,op);calls['callback '+op]+=1
    report={'complete_game':False,'original_lua_vm_tested':False,'comparisons':sum(calls.values()),'cases':dict(calls),'mismatches':0,'seed':20261002,
            'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
            'cache_sha256':hashlib.sha256(raw).hexdigest(),'source_sha256':{n:sha(ROOT/n)for n in ('health.c','health.h')},'function_evidence':evidence+old.evidence,
            'whole_four_sheet_state_comparison':True,'original_reserved_fields_preserved':True,'output_guards':True,'stack_restoration':True,'boundary_calls':dict(boundaries),'imports':m.import_calls,
            'scope':'Actual original health getters/setters, upper validation, regeneration, mana decision/application and five script callbacks execute. Empty buff container; whole four sheets compared against host and source ARM64 in Unicorn. Set/regen exercise actual cache routing and synthetic flag types -1..63, wrapping shifts/addition and negative/full regeneration. Online/config/debug/virtual decisions use explicit fixed boundary fixtures; debug Load/string lifetime and ReturnValues pushes are modeled. External float32 math, finite signed conversion and signed division are explicit arithmetic models when imported; NaN fraction comparison checks classification, not payload. Original Character construction, Lua interpreter, network/cheat ownership, full combat/death/events and native device gameplay remain unfinished.'}
    a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items()if k not in ('function_evidence','scope','boundary_calls','imports')}))
if __name__=='__main__':main()
