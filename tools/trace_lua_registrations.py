#!/usr/bin/env python3
"""Execute original registration callers, capturing calls at the Binder boundary.

The Binder implementation and Lua library initialization are explicitly stubbed.
No game callback or cache script is executed.
"""
import argparse
from collections import Counter
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

REPO = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('registration_cpu', REPO/'port/animation-values/tests/differential.py')
cpu = importlib.util.module_from_spec(spec); spec.loader.exec_module(cpu)
ROWS = [(0x37b5a0,1252,'LuaScript::BindFunction'),
        (0x3d8ec8,100,'CharAIScript::CharAIScriptBindFunction'),
        (0x38d7ec,2648,'GameObject::createBindings'),
        (0x3b56bc,5332,'Character::createBindings'),
        (0x3e74c8,152,'Door::createBindings'),
        (0x39dd98,212,'TriggerTrap::createBindings')]
BINDER = {0x31a4d4:'function',0x319af4:'method'}
LIBRARIES = {0x31aff8:'table',0x31b000:'math',0x31b008:'string',0x31b010:'base'}

def main():
    p = argparse.ArgumentParser()
    for name in ('original','oracle','report'): p.add_argument('--'+name,type=Path,required=True)
    a = p.parse_args(); sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    assert sha(a.original) == cpu.ORIGINAL_SHA256
    evidence = []
    with a.original.open('rb') as stream:
        elf = ELFFile(stream)
        loads = [s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
        for address,size,name in ROWS:
            segment = next(s for s in loads if s['p_vaddr']<=address and address+size<=s['p_vaddr']+s['p_filesz'])
            stream.seek(segment['p_offset']+address-segment['p_vaddr'])
            evidence.append({'elf_address':f'0x{address:08x}','size':size,'name':name,
                             'sha256':hashlib.sha256(stream.read(size)).hexdigest()})
        machine = cpu.EngineCpu(a.original,False,cpu.Dependencies(a.oracle),{'functions':evidence})
        # Original load base is zero. Resolve defined GLOB_DAT entries used by
        # registration callers to their exact ELF symbols; do not invent externals.
        for section in elf.iter_sections():
            if section['sh_type']!='SHT_REL': continue
            symbols = elf.get_section(section['sh_link'])
            for relocation in section.iter_relocations():
                if relocation['r_info_type']==21:
                    symbol = symbols.get_symbol(relocation['r_info_sym'])
                    if symbol['st_shndx']!='SHN_UNDEF':
                        machine.uc.mem_write(relocation['r_offset'],struct.pack('<I',symbol['st_value']))
    symbol_path = REPO/'recovered/native/symbols/libDungeonHunter2.so/symbols.csv'
    with symbol_path.open(encoding='utf-8') as stream:
        symbols = {int(row['address']):row for row in csv.DictReader(stream)
                   if row['table']=='.dynsym' and row['type']=='STT_FUNC'}
    calls=[]; libraries=[]; scope=''; object_pointer=machine.data+0x400; binder_pointer=machine.data+0x800
    def boundary(uc,address,size,unused):
        if address in LIBRARIES:
            assert machine.reg(0)==object_pointer+4
            libraries.append({'scope':scope,'library':LIBRARIES[address]})
        else:
            binder,name_pointer,target = [machine.reg(i) for i in range(3)]
            expected = object_pointer+0x10 if scope.startswith(('LuaScript','CharAIScript')) else binder_pointer
            assert binder==expected
            raw=bytes(uc.mem_read(name_pointer,256)); end=raw.find(b'\0'); assert end>=0
            name=raw[:end].decode('ascii'); symbol=symbols[target]
            assert 'sfc::script::lua::Arguments const&' in symbol['demangled']
            row={'scope':scope,'binding':BINDER[address],'name':name,
                 'target_elf_address':f'0x{target:08x}','target_symbol':symbol['name'],
                 'target_demangled':symbol['demangled'],'target_bytes':int(symbol['size']),
                 'caller_return_elf_address':f'0x{uc.reg_read(machine.lr_reg):08x}'}
            if BINDER[address]=='function':
                context = machine.reg(3)
                assert context in (0,object_pointer), (scope,name,hex(context))
                row['context']='this object' if context else 'null'
            calls.append(row)
        uc.reg_write(machine.pc_reg,uc.reg_read(machine.lr_reg))
    for address in (*BINDER,*LIBRARIES):
        machine.uc.hook_add(UC_HOOK_CODE,boundary,begin=address,end=address)
    for address,size,scope in ROWS:
        machine.uc.mem_write(object_pointer,bytes(0x200))
        machine.uc.mem_write(binder_pointer,bytes(0x100))
        machine.call(address,[object_pointer,binder_pointer])
    audit_path=REPO/'reports/lua-global-bridge-audit.json'
    audit=json.loads(audit_path.read_text())
    names={row['name']for row in calls if row['binding']=='function'}
    read_names=set(audit['global_reads']); matched=sorted(read_names&names)
    result={'complete_game':False,'scripts_executed':False,'binder_implementation_executed':False,
            'scope':'Original ARM32 registration caller instructions; Binder function/method installation and standard library initialization explicitly stubbed',
            'original_sha256':sha(a.original),'oracle_sha256':sha(a.oracle),
            'symbol_index_sha256':sha(symbol_path),'global_audit_sha256':sha(audit_path),
            'test_sha256':sha(Path(__file__)),'function_evidence':evidence,
            'registration_calls':len(calls),'binding_counts':dict(Counter(row['binding']for row in calls)),
            'distinct_function_names':len(names),'script_global_names_matched':len(matched),
            'matched_script_global_names':matched,'global_reads_without_observed_function_binding':sorted(read_names-names),
            'standard_library_requests':libraries,'calls':calls,'stack_restoration':True,
            'dependency_model':'Only six identified Binder/library boundary addresses are intercepted; inherited GameObject registration executes normally; original callback targets are resolved through defined ELF GLOB_DAT symbols',
            'import_calls':machine.import_calls,'original_instruction_addresses_seen':len(machine.seen)}
    a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:v for k,v in result.items()if k not in ('calls','global_reads_without_observed_function_binding','function_evidence','matched_script_global_names')}))
if __name__=='__main__': main()
