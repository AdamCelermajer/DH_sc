#!/usr/bin/env python3
"""Original ARM32 item readers/bonus bodies versus host and source ARM64."""
import argparse,ctypes as c,hashlib,importlib.util,json,random,struct
from collections import Counter
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parent.parent
spec=importlib.util.spec_from_file_location('loot_reader',REPO/'tools/trace_loot_tables.py')
reader=importlib.util.module_from_spec(spec);spec.loader.exec_module(reader)
class Tables(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32),('counts',c.c_uint32*8),('offsets',c.c_uint32*8)]
class Span(c.Structure):_fields_=[('bytes',c.c_void_p),('size',c.c_uint32)]
class Item(c.Structure):_fields_=[('name',Span),('base_ints',c.c_int32*4),('base_bool',c.c_uint32),('base_float_bits',c.c_uint32*9),('item_ints',c.c_int32*2),('description',Span),('tail_ints',c.c_int32*20)]
class Slot(c.Structure):_fields_=[('present',c.c_uint32),('item_id',c.c_int32),('type',c.c_int32),('slotting',c.c_int32),('weapon_kind',c.c_int32)]
class Equipment(c.Structure):_fields_=[('slots',(Slot*3)*2),('current_set',c.c_uint32),('owner_hand_rule',c.c_uint32)]
class Sheet(c.Structure):_fields_=[('values',c.c_int32*224)]
class State(c.Structure):_fields_=[(n,Sheet)for n in ('base','saved','gears','final')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
FUNCTIONS=(0x3df7c4,0x3df81c,0x3df8ac,0x3df980,0x3fc6a8,0x3ffdec,0x3ffe3c,0x400110,0x400158,0x40019c,0x4001a0,0x3f9e08,0x3dedb4)
def normalize(item,read):
    return {'name_hex':read(item.name.bytes,item.name.size).hex(),'base_ints':list(item.base_ints),
            'base_bool':item.base_bool,'base_float_bits':list(item.base_float_bits),'item_ints':list(item.item_ints),
            'description_hex':read(item.description.bytes,item.description.size).hex(),'tail_ints':list(item.tail_ints)}
def unpack_row(t,raw):
    at=0
    def take(fmt):
        nonlocal at
        values=struct.unpack_from('<'+fmt,raw,at);at+=struct.calcsize('<'+fmt);return list(values)
    def rows(fmt):return [take(fmt)for _ in range(take('I')[0])]
    if t in (0,7):row=rows('hh')
    elif t==1:row=take('h')[0]
    elif t==2:row=rows('ihb')
    elif t==4:row=[r[0]for r in rows('b')]
    elif t==5:row=[take('ii'),rows('8i'),rows('8i'),[r[0]for r in rows('i')]]
    elif t==6:row=[take('ii'),rows('ii')]
    else:raise AssertionError(t)
    assert at==len(raw);return row
def main():
    p=argparse.ArgumentParser()
    for n in ('original','oracle','cache','host','arm64','report'):p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();raw=(a.cache/'data/pydata/loot_table_pyarray.bin').read_bytes()
    old=reader.Original(a.original,a.oracle,(a.cache/'data/pydata/character_properties_pyarray.bin').read_bytes(),raw);m=old.machine
    new=reader.properties.cpu.EngineCpu(a.arm64,True,reader.properties.cpu.Dependencies(a.oracle),{'functions':[]})
    lib=c.CDLL(str(a.host.resolve()))
    signatures={'dh2_loot_open':[c.POINTER(Tables),c.c_void_p,c.c_uint32],
        'dh2_loot_record':[c.POINTER(Tables),c.c_uint32,c.c_uint32,c.POINTER(Span)],
        'dh2_loot_item_read':[c.POINTER(Tables),c.c_uint32,c.POINTER(Item)],
        'dh2_equipment_load':[c.POINTER(Tables),c.POINTER(c.c_int32),c.c_uint32,c.c_uint32,c.POINTER(Equipment)],
        'dh2_equipment_get_set':[c.POINTER(Equipment),c.c_int32,c.POINTER(c.c_int32)],
        'dh2_equipment_get_item':[c.POINTER(Equipment),c.c_uint32,c.POINTER(c.c_int32)],
        'dh2_equipment_flag':[c.POINTER(Equipment),c.c_uint32,c.POINTER(c.c_int32)],
        'dh2_equipment_bonus':[c.POINTER(Equipment),c.POINTER(Sheet),c.c_uint32,c.c_uint32,c.POINTER(c.c_int32)],
        'dh2_equipment_get_int_bonus':[c.POINTER(Equipment),c.POINTER(State),c.POINTER(Sheet),c.c_uint32,c.c_uint32,c.POINTER(c.c_int32)]}
    for name,args in signatures.items():getattr(lib,name).argtypes=args;getattr(lib,name).restype=c.c_uint32
    buf=c.create_string_buffer(raw);v=Tables();assert not lib.dh2_loot_open(c.byref(v),buf,len(raw))
    new.uc.mem_map(new.data+0x10000,0x100000);np=new.data+0x10000;nv=new.data+0x100
    ni,span_ptr,ne,ns,nt,no,nids=[new.data+x for x in (0x400,0x600,0x800,0x1000,0x3000,0x4000,0x5000)]
    new.uc.mem_write(np,raw);assert not new.call('dh2_loot_open',[nv,np,len(raw)])
    assert list(v.counts)==[t['count']for t in old.loot_tables]
    assert list(v.offsets)==[t['start']+4 for t in old.loot_tables]
    counts=Counter();item=Item();span=Span()
    for t,table in enumerate(old.loot_tables):
        for i in range(table['count']):
            assert not lib.dh2_loot_record(c.byref(v),t,i,c.byref(span))
            host_raw=c.string_at(span.bytes,span.size)
            if t==3:
                assert not lib.dh2_loot_item_read(c.byref(v),i,c.byref(item))
                expected={k:val for k,val in old.items[i].items()if k!='index'}
                assert normalize(item,c.string_at)==expected,(t,i,'host item')
                # All cache items execute in source ARM64, including borrowed string spans.
                new.uc.mem_write(ni-16,b'\xa5'*(c.sizeof(item)+32))
                assert not new.call('dh2_loot_item_read',[nv,i,ni])
                arm=Item.from_buffer_copy(bytes(new.uc.mem_read(ni,c.sizeof(item))))
                assert normalize(arm,lambda ptr,n:bytes(new.uc.mem_read(ptr,n))if n else b'')==expected,(t,i,'arm64 item')
                assert bytes(new.uc.mem_read(ni-16,16))==bytes(new.uc.mem_read(ni+c.sizeof(item),16))==b'\xa5'*16
                counts['actual item destinations']+=1
            else:
                assert unpack_row(t,host_raw)==table['rows'][i],(t,i,'host nested record')
                # Other record spans are exercised on ARM64 at first/middle/last;
                # all scalar/list destinations are compared against host spans.
                if i in (0,table['count']//2,table['count']-1):
                    assert not new.call('dh2_loot_record',[nv,t,i,span_ptr])
                    arm=Span.from_buffer_copy(bytes(new.uc.mem_read(span_ptr,c.sizeof(span))))
                    assert bytes(new.uc.mem_read(arm.bytes,arm.size))==host_raw
                    counts['arm64 other table spans']+=1
                counts['actual other record destinations']+=1
        print('verified table',t,table['count'],flush=True)
    character=m.data+0x3000;owner=character+0x560;inventory=character+0x37c
    vectors=m.data+0x6000;entries=m.data+0x6100;slots=m.data+0x6200;instances=m.data+0x6400
    temp_address=old.symbols['_ZN14CharProperties6s_tempE']['st_value'];e=Equipment();state=State();temp=Sheet();rng=random.Random(20261002)
    entered=Counter();arm_loads=0
    def hook(uc,address,size,unused):entered[hex(address)]+=1
    for address in FUNCTIONS:m.uc.hook_add(UC_HOOK_CODE,hook,begin=address,end=address)
    def setup(ids,set,rule):
        nonlocal e,arm_loads
        array=(c.c_int32*6)(*ids);assert not lib.dh2_equipment_load(c.byref(v),array,set,rule,c.byref(e))
        if arm_loads<16:
            # Six full safe item lookups can exceed EngineCpu's default 1M
            # instruction ceiling; use a bounded 6M for these snapshot loads.
            new.uc.mem_write(nids,bytes(array));sp=new.stack+0xe000
            new.uc.reg_write(new.sp_reg,sp);new.uc.reg_write(new.lr_reg,new.stop)
            for i,arg in enumerate((nv,nids,set,rule,ne)):new.write_reg(i,arg)
            new.uc.emu_start(new.symbols['dh2_equipment_load'],new.stop,count=6000000)
            assert new.uc.reg_read(new.pc_reg)==new.stop and new.uc.reg_read(new.sp_reg)==sp and not new.reg(0)
            assert bytes(new.uc.mem_read(ne,c.sizeof(e)))==bytes(e);arm_loads+=1
        else:new.uc.mem_write(ne,bytes(e))
        m.uc.mem_write(character,bytes(0x1600));m.uc.mem_write(owner+4,struct.pack('<I',character));m.uc.mem_write(inventory+4,struct.pack('<I',character))
        m.uc.mem_write(inventory+0x14,struct.pack('<I',vectors));m.uc.mem_write(inventory+0x2e,bytes([set]))
        for s in range(2):m.uc.mem_write(vectors+12*s,struct.pack('<3I',entries+12*s,entries+12*s+12,entries+12*s+12))
        for i,id in enumerate(ids):
            sp=slots+8*i;ip=instances+16*i
            m.uc.mem_write(entries+4*i,struct.pack('<I',sp if id>=0 else 0));m.uc.mem_write(sp,struct.pack('<I',ip));m.uc.mem_write(ip+4,struct.pack('<i',id))
        for field in range(224):
            state.final.values[field]=rng.choice([-2147483648,2147483647,-1,0,1,rng.randint(-100000,100000)])
            temp.values[field]=rng.choice([-2147483648,2147483647,-1,0,1,rng.randint(-100000,100000)])
        # Original inventory reads this final property directly through Character.
        state.final.values[203]=c.c_int32(rule).value
        m.uc.mem_write(owner+0xa98,bytes(state.final));m.uc.mem_write(temp_address+4,bytes(temp))
        new.uc.mem_write(ns,bytes(state));new.uc.mem_write(nt,bytes(temp))
    def compare(fun,host_args,new_args,expected,label):
        result=c.c_int32(12345);new.uc.mem_write(no-16,b'\xa5'*36)
        assert not getattr(lib,fun)(*host_args,c.byref(result)),label
        assert not new.call(fun,[*new_args,no]),label
        assert result.value==c.c_int32(expected).value==struct.unpack('<i',new.uc.mem_read(no,4))[0],(label,expected,result.value)
        assert bytes(new.uc.mem_read(no-16,16))==bytes(new.uc.mem_read(no+4,16))==b'\xa5'*16
        counts[label]+=1
    def check_bonus_flags(label):
        before=bytes(m.uc.mem_read(character,0x1600));eb=bytes(e);sb=bytes(state);tb=bytes(temp)
        for op,address in enumerate((0x400110,0x400158,0x4001a0,0x4001a0)):
            compare('dh2_equipment_flag',[c.byref(e),op],[ne,op],old.invoke(address,[inventory,op==3]),label+' flags')
        for op,address in enumerate((0x3df7c4,0x3df81c,0x3df8ac)):
            for off in range(2):compare('dh2_equipment_bonus',[c.byref(e),c.byref(state.final),op,off],[ne,ns+State.final.offset,op,off],old.invoke(address,[owner,off]),label+' bonuses')
        assert bytes(e)==eb and bytes(state)==sb and bytes(temp)==tb and bytes(m.uc.mem_read(character,0x1600))==before
    for i in range(old.item_count):
        ids=[i,(i+17)%old.item_count,-1 if i%3==0 else (i+1)%old.item_count,-1,i,-1 if i%5==0 else (i+31)%old.item_count]
        setup(ids,i%2,(0,1,2,0xffffffff)[i%4]);check_bonus_flags('actual item equipment')
    # Explicit synthetic type/slot/kind and overflow coverage in real native records.
    original_item=bytes(m.uc.mem_read(old.item_pointer,164))
    synthetic_count=0
    for type in (-1,0,4,5,6,14):
        for slotting in (-4,-3,1):
            for kind in (-1,0,6):
                for rule in (0,1,2,0xffffffff):
                    setup([0,0,0,0,0,0],synthetic_count%2,rule)
                    p=old.item_pointer;m.uc.mem_write(p+0x58,struct.pack('<i',type));m.uc.mem_write(p+0x68,struct.pack('<i',slotting));m.uc.mem_write(p+0x94,struct.pack('<i',kind))
                    for row in e.slots:
                        for entry in row:entry.type=type;entry.slotting=slotting;entry.weapon_kind=kind
                    new.uc.mem_write(ne,bytes(e));check_bonus_flags('synthetic equipment');synthetic_count+=1
    m.uc.mem_write(old.item_pointer,original_item)
    for case in range(16):
        setup([-1,case,-1,-1,case+17,case+1],case%2,(0,1,2,0xffffffff)[case%4])
        for slot in (-2147483648,-1,0,1,2,3,2147483647):compare('dh2_equipment_get_set',[c.byref(e),slot],[ne,slot&0xffffffff],old.invoke(0x3fc6a8,[inventory,slot]),'current equipment set')
        for slot in (0,1,2,3,0xffffffff):
            ptr=old.invoke(0x3ffdec,[inventory,slot]);mutable=old.invoke(0x3ffe3c,[inventory,slot]);assert ptr==mutable
            id=struct.unpack('<i',m.uc.mem_read(ptr+4,4))[0]if ptr else -1
            compare('dh2_equipment_get_item',[c.byref(e),slot],[ne,slot],id,'equipped item')
        for id in range(224):
            for use_temp in range(2):compare('dh2_equipment_get_int_bonus',[c.byref(e),c.byref(state),c.byref(temp),id,use_temp],[ne,ns,nt,id,use_temp],old.invoke(0x3df980,[owner,id,use_temp]),'integer with bonus')
    assert bytes(buf)[:len(raw)]==bytes(new.uc.mem_read(np,len(raw)))==raw
    evidence=[]
    with a.original.open('rb')as f:
        elf=ELFFile(f);loads=[s for s in elf.iter_segments()if s['p_type']=='PT_LOAD']
        for address in FUNCTIONS:
            s=next(s for s in old.symbols.values()if s['st_value']==address and s['st_info']['type']=='STT_FUNC');n=s['st_size']
            seg=next(s for s in loads if s['p_vaddr']<=address and address+n<=s['p_vaddr']+s['p_filesz']);f.seek(seg['p_offset']+address-seg['p_vaddr'])
            evidence.append({'elf_address':hex(address),'size':n,'symbol':s.name,'sha256':hashlib.sha256(f.read(n)).hexdigest()})
    result={'complete_game':False,'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),'host_sha256':sha(a.host),'arm64_sha256':sha(a.arm64),'test_sha256':sha(Path(__file__)),
            'cache_sha256':hashlib.sha256(raw).hexdigest(),'comparisons':sum(counts.values()),'cases':dict(counts),'mismatches':0,'function_entries':dict(entered),'source_arm64_snapshot_loads':arm_loads,
            'function_evidence':evidence+old.loot_evidence,'input_preservation':True,'output_guards':True,'stack_restoration':True,
            'scope':'Actual original eight loot array readers and nested scalar/string/list readers execute. All 1322 item destinations agree with host and source ARM64; all other table destinations agree with host record spans, selected spans also run on ARM64. Sixteen source snapshot loads run on ARM64; remaining bonus cases copy checked host snapshots into ARM64. Actual original GetItem, equipped-item/current-set, shield/offhand/twohand, property bonus and GetIntWithBonus bodies execute against original records and explicit synthetic layouts. Source ARM64 runs in Unicorn. Stream/storage dependencies use bounded models. No inventory lifecycle/equip restrictions/gear contributions, full Character, original Lua/gameplay, APK or hardware execution.'}
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items()if k not in ('function_evidence','function_entries')}))
if __name__=='__main__':main()
