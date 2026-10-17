#!/usr/bin/env python3
"""Original gear stats/powers and empty-buff lifecycle versus host/source ARM64."""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('powers_reader',REPO/'tools/trace_item_powers.py')
reader=importlib.util.module_from_spec(spec);spec.loader.exec_module(reader)
spec=importlib.util.spec_from_file_location('class_diff',ROOT/'../character-classes/tests/differential.py')
classes=importlib.util.module_from_spec(spec);spec.loader.exec_module(classes)
State,Sheet,Table,Classes=classes.State,classes.Sheet,classes.Table,classes.Classes
class Loot(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*8),('offsets',c.c_uint32*8)]
class Powers(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*2),('offsets',c.c_uint32*2)]
class Power(c.Structure):_fields_=[('type',c.c_int32),('value',c.c_int32),('count',c.c_uint32),('stats',c.c_void_p),('tail',c.c_int32*5)]
class Entry(c.Structure):_fields_=[('item_id',c.c_int32),('power_count',c.c_uint32),('power_ids',c.POINTER(c.c_int32))]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
FUNCTIONS=(0x3e3154,0x3e32b4,0x3df480,0x3e08a8,0x3e087c,0x3e0810,0x3defac,0x3defbc,0x3df2a4,0x3df250,0x3df140,0x3deca0,0x3dfe60,0x3ffd20,0x40022c,0x3ffe3c,0x3f9ec4)
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();pr=(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes();wr=(a.cache/'data/pydata/item_powers_pyarray.bin').read_bytes();lr=(a.cache/'data/pydata/loot_table_pyarray.bin').read_bytes();cr=(a.cache/'data/pydata/character_classes_pyarray.bin').read_bytes()
    old=reader.Original(a.original,a.oracle,pr,wr);m=old.machine
    # Execute the actual class and loot readers in the same original address space.
    for raw,names in [(cr,['ClassTable']), (lr,['DropTilePriorityTable','InventoryTable','ItemList','ItemTable','ItemTypeList','LootTable','MerchantTable','NumProbArray'])]:
        old.raw=raw;old.cursor=0
        for name in names:old.invoke(old.symbols[f'_ZN6Arrays{len(name)}{name}4readEP11IStreamBase']['st_value'],[old.stream])
        assert old.cursor==len(raw)
    ip=old.word(old.symbols['_ZN6Arrays9ItemTable7membersE']['st_value'])
    new=reader.properties.cpu.EngineCpu(a.arm64,True,classes.state_test.base.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()))
    signatures={'dh2_property_open':[c.POINTER(Table),c.c_void_p,c.c_uint32], 'dh2_class_open':[c.POINTER(Classes),c.c_void_p,c.c_uint32], 'dh2_loot_open':[c.POINTER(Loot),c.c_void_p,c.c_uint32], 'dh2_power_open':[c.POINTER(Powers),c.c_void_p,c.c_uint32], 'dh2_power_read':[c.POINTER(Powers),c.c_uint32,c.POINTER(Power)],
                'dh2_gear_add_item':[c.POINTER(Table),c.POINTER(Loot),c.POINTER(Sheet),c.c_uint32,c.c_uint32], 'dh2_gear_add_power':[c.POINTER(Table),c.POINTER(Powers),c.POINTER(Sheet),c.c_uint32,c.c_uint32], 'dh2_gear_load':[c.POINTER(Table),c.POINTER(Loot),c.POINTER(Powers),c.POINTER(Sheet),c.POINTER(Entry),c.c_uint32],
                'dh2_character_recalc':[c.POINTER(Table),c.POINTER(Classes),c.POINTER(State),c.c_uint32], 'dh2_character_update_base':[c.POINTER(Table),c.POINTER(Classes),c.POINTER(State),c.c_int32], 'dh2_character_update_gears':[c.POINTER(Table),c.POINTER(Classes),c.POINTER(Loot),c.POINTER(Powers),c.POINTER(State),c.POINTER(Entry),c.c_uint32]}
    for name,args in signatures.items():getattr(lib,name).argtypes=args;getattr(lib,name).restype=c.c_uint32
    buffers=[c.create_string_buffer(raw) for raw in (pr,cr,lr,wr)];views=[Table(),Classes(),Loot(),Powers()];openers=['dh2_property_open','dh2_class_open','dh2_loot_open','dh2_power_open']
    new.uc.mem_map(new.data+0x10000,0x200000);data=[new.data+x for x in (0x10000,0x80000,0xa0000,0xf0000)];vp=[new.data+x for x in (0x100,0x200,0x300,0x400)]
    def arm(name,args):
        sp=new.stack+0xe000;new.uc.reg_write(new.sp_reg,sp);new.uc.reg_write(new.lr_reg,new.stop)
        for i,v in enumerate(args):
            if i<8:new.write_reg(i,v & 0xffffffffffffffff)
            else:new.uc.mem_write(sp+8*(i-8),struct.pack('<Q',v))
        new.uc.emu_start(new.symbols[name],new.stop,count=40000000)
        assert new.uc.reg_read(new.pc_reg)==new.stop and new.uc.reg_read(new.sp_reg)==sp,(name,'instruction/stack gate')
        return new.reg(0)
    for raw,b,v,op,np,nv in zip((pr,cr,lr,wr),buffers,views,openers,data,vp):
        assert not getattr(lib,op)(c.byref(v),b,len(raw));new.uc.mem_write(np,raw);assert not arm(op,[nv,np,len(raw)])
    pt,ct,lt,wt=views;pv,cv,lv,wv=vp
    cases=Counter();entered=Counter();rng=random.Random(20261002);state=State();owner=m.data+0x3000;offsets=(8,0x38c,0x710,0xa94);ns=new.data+0x1000;out=new.data+0x3000
    def entry_hook(uc,address,size,unused):entered[hex(address)]+=1
    for addr in FUNCTIONS:m.uc.hook_add(UC_HOOK_CODE,entry_hook,begin=addr,end=addr)
    def prepare(class_id=None):
        for name in ('base','saved','gears','final'):
            values=getattr(state,name).values
            for i,d in enumerate(old.tables[0]['rows'][0]):values[i]=rng.choice([d,-2147483648,2147483647,-1,0,1,rng.randint(-100000,100000)])
        if class_id is not None:state.base.values[26]=class_id
        m.uc.mem_write(owner,b'\xa5'*0xe44)
        for name,off in zip(('base','saved','gears','final'),offsets):m.uc.mem_write(owner+off+4,bytes(getattr(state,name)))
        header=owner+0xe18;m.uc.mem_write(header,struct.pack('<4I',0,0,header,header))
        new.uc.mem_write(ns-16,b'\xa5'*(c.sizeof(state)+32));new.uc.mem_write(ns,bytes(state))
    def compare(address,args,fun,host_args,arm_args,label):
        before=bytes(m.uc.mem_read(owner,0xe44));old.invoke(address,[owner,*args])
        assert not getattr(lib,fun)(*host_args),(label,fun,'host status')
        assert not arm(fun,arm_args),(label,fun,'arm status')
        expected=b''.join(bytes(m.uc.mem_read(owner+off+4,896)) for off in offsets)
        assert expected==bytes(state)==bytes(new.uc.mem_read(ns,c.sizeof(state))),(label,fun,args)
        after=bytes(m.uc.mem_read(owner,0xe44));b1,b2=bytearray(before),bytearray(after)
        for off in offsets:b1[off+4:off+900]=b2[off+4:off+900]=bytes(896)
        assert b1==b2,(label,'reserved native fields')
        assert bytes(new.uc.mem_read(ns-16,16))==bytes(new.uc.mem_read(ns+c.sizeof(state),16))==b'\xa5'*16
        cases[label]+=1
    # All nested native reader destinations match source records on both targets.
    power=Power()
    for i,row in enumerate(old.power_tables[1]['rows']):
        assert not lib.dh2_power_read(c.byref(wt),i,c.byref(power));assert not arm('dh2_power_read',[wv,i,out])
        def normalized(q,read):return {'type':q.type,'value':q.value,'stats':[list(struct.unpack('<3i',read(q.stats+j*12,12))) for j in range(q.count)],'tail':list(q.tail)}
        ap=Power.from_buffer_copy(bytes(new.uc.mem_read(out,c.sizeof(power))))
        assert row==normalized(power,c.string_at)==normalized(ap,lambda ptr,n:bytes(new.uc.mem_read(ptr,n)))
        cases['actual power reader destinations']+=1
    for i in range(1322):
        for left in range(2):
            prepare();compare(0x3e3154,[i,left],'dh2_gear_add_item',[c.byref(pt),c.byref(lt),c.byref(state.gears),i,left],[pv,lv,ns+State.gears.offset,i,left],'actual item gear stats')
        if i%200==0:print('item stats',i,flush=True)
    for i in range(937):
        for left in range(2):
            prepare();compare(0x3e32b4,[i,left],'dh2_gear_add_power',[c.byref(pt),c.byref(wt),c.byref(state.gears),i,left],[pv,wv,ns+State.gears.offset,i,left],'actual power gear stats')
        if i%200==0:print('power stats',i,flush=True)
    # Every opcode, unknown no-ops, both hands, repeated sentinel/wrap and set.
    fake=m.data+0x7000;stats=m.data+0x7100;original_wp=old.power_pointer
    m.uc.mem_write(old.symbols['_ZN6Arrays14ItemPowerTable7membersE']['st_value'],struct.pack('<I',fake))
    for op in range(-1,51):
        for delta in (-2147483648,2147483647,-1,0,1,257):
            rows=[[op,delta,123],[op,delta,-999]]
            raw=struct.pack('<IIbiI',0,1,-1,-1,len(rows))+b''.join(struct.pack('<3i',*r) for r in rows)+bytes(20)
            wb=c.create_string_buffer(raw);wt2=Powers();assert not lib.dh2_power_open(c.byref(wt2),wb,len(raw));new.uc.mem_write(data[3],raw);assert not arm('dh2_power_open',[wv,data[3],len(raw)])
            m.uc.mem_write(fake,struct.pack('<Ib3xiII5i',0xa5a5a5a5,-1,-1,len(rows),stats,0,0,0,0,0));m.uc.mem_write(stats,b''.join(struct.pack('<I3i',0xa5a5a5a5,*r) for r in rows))
            for left in range(2):
                prepare();compare(0x3e32b4,[0,left],'dh2_gear_add_power',[c.byref(pt),c.byref(wt2),c.byref(state.gears),0,left],[pv,wv,ns+State.gears.offset,0,left],'synthetic opcode/sentinel/overflow powers')
    m.uc.mem_write(old.symbols['_ZN6Arrays14ItemPowerTable7membersE']['st_value'],struct.pack('<I',original_wp));new.uc.mem_write(data[3],wr);assert not arm('dh2_power_open',[wv,data[3],len(wr)])
    # RecalcProperties applies the class from base field 26 before the 224-loop.
    for id in range(260):
        for flag in range(2):
            prepare(id);compare(0x3e0810,[flag],'dh2_character_recalc',[c.byref(pt),c.byref(ct),c.byref(state),flag],[pv,cv,ns,flag],'all class lifecycle recalculations')
        if id%40==0:print('lifecycle class',id,flush=True)
    for row in list(range(448))+[-2147483648,-1,448,2147483647]:
        prepare();compare(0x3e087c,[row],'dh2_character_update_base',[c.byref(pt),c.byref(ct),c.byref(state),row],[pv,cv,ns,row&0xffffffff],'base update/reset/load/class/recalc')
        if row>=0 and row%80==0:print('base row',row,flush=True)
    # Full original LoadGearsProperties/UpdateGearsProperties uses actual slot,
    # instance and power-ID getters against bounded synthetic inventory storage.
    character=m.data+0x10000;inventory=character+0x37c;vectors=character+0x2000;entries=character+0x2100;slots=character+0x2400;instances=character+0x2600;instance_powers=character+0x6000
    m.uc.mem_map(character,0x10000)
    for case in range(32):
        n=(0,1,2,3,8,12,16,32)[case%8];selected=case%2;ids=[];powerarrays=[];host_entries=(Entry*n)();native=[];npowers=new.data+0x5000
        prepare(case%260);m.uc.mem_write(owner+4,struct.pack('<I',character));m.uc.mem_write(inventory,bytes(0x100));m.uc.mem_write(inventory+4,struct.pack('<I',character));m.uc.mem_write(inventory+0x14,struct.pack('<I',vectors));m.uc.mem_write(inventory+0x2e,bytes([selected]))
        for st in range(2):m.uc.mem_write(vectors+12*st,struct.pack('<3I',entries+st*128,entries+st*128+n*4,entries+st*128+n*4))
        for st in range(2):
            for slot in range(n):
                idx=st*n+slot;item_id=-1 if (slot+case+st)%5==0 else (case*37+slot*17+st)%1322;pp=[(case*29+slot*13+j+st)%937 for j in range((slot+case)%3)]
                sp=slots+idx*8;inst=instances+idx*128;qp=instance_powers+idx*128
                m.uc.mem_write(entries+st*128+slot*4,struct.pack('<I',sp if item_id>=0 else 0));m.uc.mem_write(sp,struct.pack('<I',inst));m.uc.mem_write(inst+4,struct.pack('<i',item_id));m.uc.mem_write(inst+0x5c,struct.pack('<3I',qp,qp+32*len(pp),qp+32*len(pp)))
                if pp:m.uc.mem_write(qp,b''.join(struct.pack('<i',id)+bytes(28) for id in pp))
                if st==(selected if slot in (1,2) else 0):ids.append((slot,item_id,pp))
        for slot,item_id,pp in sorted(ids):
            arr=(c.c_int32*len(pp))(*pp);powerarrays.append(arr);host_entries[slot]=Entry(item_id,len(pp),arr)
            new.uc.mem_write(npowers,bytes(arr) if pp else bytes(4));native.append(struct.pack('<iIQ',item_id,len(pp),npowers));npowers+=max(len(pp),1)*4
        ne=new.data+0x4000
        if native:new.uc.mem_write(ne,b''.join(native))
        before=bytes(state);compare(0x3df480,[],'dh2_gear_load',[c.byref(pt),c.byref(lt),c.byref(wt),c.byref(state.gears),host_entries,n],[pv,lv,wv,ns+State.gears.offset,ne,n],'full gear load / actual instance getters')
        # Repeated UpdateGears: resets the gear sheet but reapplies base class.
        for repeat in range(2):compare(0x3e08a8,[],'dh2_character_update_gears',[c.byref(pt),c.byref(ct),c.byref(lt),c.byref(wt),c.byref(state),host_entries,n],[pv,cv,lv,wv,ns,ne,n],'gear update/reset/load/class/recalc')
        print('gear lifecycle',case,n,flush=True)
    for raw,b,np in zip((pr,cr,lr,wr),buffers,data):assert bytes(b)[:len(raw)]==bytes(new.uc.mem_read(np,len(raw)))==raw
    evidence=[]
    with a.original.open('rb') as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for address in FUNCTIONS:
            s=next(s for s in old.symbols.values() if s['st_value']==address and s['st_info']['type']=='STT_FUNC');n=s['st_size'];seg=next(s for s in loads if s['p_vaddr']<=address and address+n<=s['p_vaddr']+s['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr'])
            evidence.append({'elf_address':hex(address),'size':n,'symbol':s.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    result={'complete_game':False,'scripts_executed':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),'cache_sha256':{name:hashlib.sha256(raw).hexdigest() for name,raw in zip(('properties','classes','loot','powers'),(pr,cr,lr,wr))},'cases':dict(cases),'comparisons':sum(cases.values()),'mismatches':0,'function_entries':dict(entered),'function_evidence':evidence+old.power_evidence,'whole_four_sheet_comparison':True,'input_preservation':True,'reserved_fields_and_output_guards':True,'stack_restoration':True,'scope':'Actual original gear stat/power helpers, load/update/base/reset and complete 224-field recalculation execute with empty buffs, including all 1322 items/937 powers, all 260 classes and 448 base rows. Inventory slots and item instances are explicit bounded fixtures; actual getters execute. Invalid original memory/assert paths are not run. Random power generation, equip requirements, original Character ownership, buff-inclusive class application, Lua integration, Android APK and hardware execution remain unverified. Source ARM64 executes in Unicorn.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ('function_entries','function_evidence')}))
if __name__=='__main__':main()
