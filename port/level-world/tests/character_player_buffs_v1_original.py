"""Run original ApplyBuff identity branch, nested classes and property traversal.

Caller-owned ARM sheets, actual class/table globals and a one-node map/deque
are initialized. All ApplyBuff, Value, class, resolver and traversal instructions
execute. No buff allocator, timer, FX or removal body is substituted or claimed.
Removal expected sheets use original RecalcProperties after caller unlinks the
fixture group; native retention/removal and provider prefixes are tested in C++.
"""
from __future__ import annotations
import hashlib,json,struct,sys
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'engine-resources/tests'),str(ROOT/'game-data/tools'),str(ROOT/'level-world/tools')]
from cpu import Cpu
from inspect_class_tables import parse
from prepare_actors import strings

ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
NAMES=('Avalon_FireResistance','Maria_WaterResistance','Sylph_WindResistance',
       'Primula_EarthResistance','Celeste_LightningResistance')

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def pack(v):return struct.pack('<224i',*v)

def generate(elf:Path,cache:Path,output:Path,manifest:dict)->dict:
    assert sha(elf)==ELF_SHA
    manifests=[manifest]+[json.loads((ROOT/'game-data/reference'/part/'original-functions.json').read_text()) for part in ('classes','properties','vitals')]
    provenance={'functions':list({r['original_symbol']:r for m in manifests for r in m['functions']}.values())}
    old=Cpu(elf,False,provenance)
    data=cache/'data/pydata';rows=parse(data)['rows']
    raw=(data/'character_properties_pyarray.bin').read_bytes()
    defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900))
    names,_=strings((data/'character_properties_pyarraynames.bin').read_bytes())
    def word(a):return struct.unpack('<I',old.uc.mem_read(a,4))[0]
    got=(0x3e2e34+word(0x3e3008))&0xffffffff
    old.pointer(word(got+word(0x3e300c)),len(rows))
    classrows=old.data+0x100000;cursor=old.data+0x140000
    old.pointer(word(got+word(0x3e3010)),classrows)
    for i,row in enumerate(rows):
        old.uc.mem_write(classrows+i*12,struct.pack('<III',0,len(row['entries']),cursor))
        for formula in row['entries']:
            old.uc.mem_write(cursor,struct.pack('<i5i',0,*formula));cursor+=24
    character=old.data+0x1000;owner=character+0x560
    sheets=[owner+x for x in (8,0x38c,0x710,0xa94)]
    default_at=old.data+0x4000;old.pointer(0x9a645c,default_at)
    old.uc.mem_write(default_at,bytes(4)+pack(defaults))
    old.uc.mem_write(default_at+900,bytes(4)+pack(types))
    lgot=(0x3e2d88+word(0x3e2e18))&0xffffffff
    old.pointer(lgot+word(0x3e2e1c),old.data+0x6000)
    args,vector,values=old.data+0x7000,old.data+0x7100,old.data+0x7200
    old.pointer(args+4,vector);old.uc.mem_write(vector,struct.pack('<III',values,values+224,values+224))
    buff=old.data+0x10000;node=old.data+0x8000;sentinel=owner+0xe18
    omap,block=old.data+0xa000,old.data+0xc000
    def groups(enabled,class_id):
        old.uc.mem_write(sentinel,struct.pack('<4I',0,node if enabled else 0,node if enabled else sentinel,node if enabled else sentinel))
        old.pointer(owner+0xe28,int(enabled))
        if enabled:
            old.uc.mem_write(node,bytes(0x100));old.uc.mem_write(node,struct.pack('<5I',1,sentinel,0,0,class_id))
            old.pointer(omap,block);old.pointer(block,buff)
            old.uc.mem_write(node+0x34,struct.pack('<4I',block,block,block+128,omap))
            old.uc.mem_write(node+0x44,struct.pack('<4I',block+4,block,block+128,omap))
    def state():return b''.join(bytes(old.uc.mem_read(s+4,896)) for s in sheets)
    cases=[];binary=bytearray()
    bases=[('ClassID=-1',defaults.copy(),-1)]+[(name,list(struct.unpack_from('<224i',raw,4+names.index(name)*896)),names.index(name)) for name in ('KnightPlayerBase','MagePlayerBase','RoguePlayerBase')]
    for base_name,base,base_index in bases:
        base=base.copy()
        if base_index==-1:base[26]=-1;base[19]=256
        for class_name in NAMES+tuple('AW_'+n for n in NAMES):
            class_id=next(i for i,r in enumerate(rows) if r['name']==class_name)
            for at,sheet in zip(sheets,(base,defaults,defaults,defaults)):old.uc.mem_write(at,bytes(4)+pack(sheet))
            groups(False,class_id);old.invoke(0x3e0810,[owner,1]);initial=state()
            bs=defaults.copy();bs[172]=256
            old.uc.mem_write(buff,bytes(4)+pack(bs));old.pointer(sheets[3]+4+172*4,256)
            groups(True,class_id)
            old.uc.mem_write(values,bytes(224));old.pointer(values+4,3)
            old.uc.mem_write(values+8,struct.pack('<f',float(class_id)))
            old.pointer(values+112+4,2);old.pointer(values+112+108,buff)
            old.invoke(0x3babdc,[args,0,character])
            applied=state();buff_sheet=bytes(old.uc.mem_read(buff+4,896))
            groups(False,class_id);old.invoke(0x3e0810,[owner,1]);removed=state()
            binary+=struct.pack('<ii',base_index,class_id)+initial+buff_sheet+applied+removed
            changed=[i for i,(a,b) in enumerate(zip(struct.unpack('<224i',initial[-896:]),struct.unpack('<224i',applied[-896:]))) if a!=b]
            cases.append({'base':base_name,'base_index':base_index,'base_class_id':base[26],'class':class_name,'class_id':class_id,'changed_resolved_properties':changed,'applied_sha256':hashlib.sha256(applied).hexdigest(),'removed_sha256':hashlib.sha256(removed).hexdigest()})
    output.parent.mkdir(parents=True,exist_ok=True);output.write_bytes(struct.pack('<I',len(cases))+binary)
    return {'cases':len(cases),'normal_resistance_cases':20,'aw_resistance_cases':20,
            'original_apply_buff_callback_executed':True,'original_nested_class_property_and_map_deque_instructions_unmocked':True,
            'original_buff_allocator_timer_fx_and_delete_bodies_executed':False,
            'fixture_sha256':sha(output),'fixture_bytes':output.stat().st_size,
            'case_details':cases,'executed_pinned_function_words':len(old.seen),
            'import_calls':old.import_calls}
