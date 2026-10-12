"""Recover original design registration names and Character field lookups.

Constructor call sites execute with registration/allocation helpers skipped;
this captures their delivered names/getter identities, not owned map creation.
Character member lookup executes on actual cache-backed borrowed field strings.
"""
import argparse
import hashlib
import json
import re
import struct
import sys
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from capstone.arm import ARM_INS_BL, ARM_INS_BLX
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_script_selection_differential import Cpu,string


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    if args.output.exists():raise RuntimeError('Refusing to replace registry evidence')
    engine=ROOT/'.local-inputs/libDungeonHunter2.so'
    reference=ROOT/'port/script-runtime/reference/design-bindings'
    manifest=json.loads((reference/'original-functions.json').read_text())
    assert sha(engine)==manifest['original_sha256']
    cpu=Cpu(engine,False,manifest)
    with engine.open('rb') as stream:
        elf=ELFFile(stream);symbols=list(elf.get_section_by_name('.symtab').iter_symbols())
        constructor=next(s for s in symbols if s['st_value']==0x4be550 and s['st_size'])
        segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and
                     s['p_vaddr']<=0x4be550<s['p_vaddr']+s['p_filesz'])
        stream.seek(segment['p_offset']+0x4be550-segment['p_vaddr'])
        code=list(Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(stream.read(constructor['st_size']),0x4be550))
        getter_symbols={s['st_value']:s.name for s in symbols if s['st_size']}
    calls={i.address:i for i in code if i.id in (ARM_INS_BL,ARM_INS_BLX)}
    records=[];skipped=[]
    mode='registration'
    def hook(uc,address,size,unused):
        if mode!='registration' or address not in calls:return
        instruction=calls[address]
        if instruction.op_str=='#0x4bdccc':
            name=string(cpu,cpu.reg(1)).decode('ascii');getter=cpu.reg(2)
            assert cpu.reg(0)==cpu.data+0x1000 and getter in getter_symbols
            records.append(dict(instruction=hex(address),name=name,getter=hex(getter),
                                getter_symbol=getter_symbols[getter]))
        else:skipped.append(dict(instruction=hex(address),target=instruction.op_str))
        uc.reg_write(cpu.pc,address+4)
    cpu.uc.hook_add(UC_HOOK_CODE,hook)
    cpu.uc.mem_write(cpu.data+0x1000,bytes(4096))
    cpu.invoke(0x4be550,[cpu.data+0x1000,0])
    expected_calls=sum(i.op_str=='#0x4bdccc' for i in calls.values())
    assert len(records)==expected_calls
    # registerClassByName assigns map[name] on every call. Repeated source
    # names are observable registrations; retain order and the final getter.
    effective={r['name']:r for r in records}
    duplicates={name:[r for r in records if r['name']==name]
                for name in effective if sum(r['name']==name for r in records)>1}
    character=effective['CharacterProperties']
    assert character['getter']=='0x4af110'
    mode='member'
    fields_path=ROOT/'port/android-native/app/src/main/assets/data/character_properties_pystructnames.bin'
    raw=fields_path.read_bytes();count=struct.unpack_from('<I',raw)[0];assert count==224
    offset=4;fields=[]
    for _ in range(count):
        length=struct.unpack_from('<I',raw,offset)[0];offset+=4
        fields.append(raw[offset:offset+length]);offset+=length
    # Source static std::string array is borrowed after its actual file producer.
    # This fixture does not execute that producer or its allocation backend.
    data_names=0x9a743c
    for i,name in enumerate(fields):
        pointer=cpu.data+0x50000+i*256
        cpu.uc.mem_write(pointer,name+b'\0')
        cpu.pointer(data_names+i*24+0x10,pointer+len(name))
        cpu.pointer(data_names+i*24+0x14,pointer)
    text=cpu.data+0x80000;queries=[]
    for name in fields+[b'absent',b'skilltree',b'SkillTree\0ignored',b'']:
        cpu.uc.mem_write(text,name+b'\0')
        expected=fields.index(name.split(b'\0')[0]) if name.split(b'\0')[0] in fields else -1
        result=cpu.invoke(0x4af110,[text]);result=result if result<0x80000000 else result-0x100000000
        assert result==expected
        queries.append(dict(name=name.hex(),value=result))
    report=dict(validation='PASS',scope=__doc__,original_sha256=sha(engine),
                original_manifest_sha256=sha(reference/'original-functions.json'),
                probe_sha256=sha(Path(__file__)),registrations=records,
                registration_count=len(records),effective_registration_count=len(effective),
                duplicate_registrations=duplicates,skipped_constructor_calls=skipped,
                actual_character_field_queries=queries,field_input_sha256=sha(fields_path),
                manager_construction_proved=False,member_lookup_instructions_executed=True)
    args.output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',registrations=len(records),effective_names=len(effective),
                         member_queries=len(queries),character=character)))


if __name__=='__main__':main()
