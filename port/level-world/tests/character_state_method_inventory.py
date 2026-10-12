"""Original remaining sixteen state-family vtable methods: exact bx-lr empty
classification, complete captures and static dependency inventory only.
"""
import argparse,hashlib,json
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 plan=R/'reference/character-monster-state-ownership/registration-plan.json';states=json.loads(plan.read_text())['states'];rows=[];assembly=[];methods=('on_focus','on_blur','on_update','on_event');m=Cs(CS_ARCH_ARM,CS_MODE_ARM)
 with a.engine.open('rb') as f:
  elf=ELFFile(f);symbols={s['st_value']:s for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC'};segments=list(elf.iter_segments())
  for state in states:
   if state['id'] in (3,4,5,12):continue
   for operation,key in enumerate(methods):
    address=state[key];symbol=symbols[address];segment=next(s for s in segments if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz']);f.seek(segment['p_offset']+address-segment['p_vaddr']);raw=f.read(symbol['st_size']);instructions=list(m.disasm(raw,address));calls=[];indirect=[]
    for i in instructions:
     if i.mnemonic in ('bl','b') and i.op_str.startswith('#'):
      target=int(i.op_str[1:],0)
      if i.mnemonic=='bl' or not address<=target<address+len(raw):calls.append(dict(pc=hex(i.address),instruction=i.mnemonic,target=hex(target),symbol=symbols[target].name if target in symbols else None))
     if i.mnemonic=='blx' or i.mnemonic=='ldr' and i.op_str.startswith('pc,'):indirect.append(dict(pc=hex(i.address),instruction=i.mnemonic,operands=i.op_str))
    rows.append(dict(state=state['id'],type=state['type'],operation=operation,method=key,source_function=hex(address),symbol=symbol.name,size=len(raw),sha256=hashlib.sha256(raw).hexdigest(),empty=raw==bytes.fromhex('1eff2fe1'),raw_hex=raw.hex() if len(raw)<=4 else None,direct_dependencies=calls,indirect_calls=indirect))
    assembly.append('\n# '+symbol.name+'\n');assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}\n' for i in instructions)
 a.output.mkdir(parents=True,exist_ok=True);(a.output/'original-functions.asm').write_text(''.join(assembly));data=dict(validation='PASS',original_sha256=sha(a.engine),source_registration=dict(path=str(plan.relative_to(REPO)).replace('\\','/'),sha256=sha(plan)),script_sha256=sha(Path(__file__)),scope=__doc__,state_families=16,methods=len(rows),exact_bx_lr_empty_methods=sum(x['empty'] for x in rows),empty_focus_blur=sum(x['empty'] and x['operation']<2 for x in rows),static_dependencies_not_dynamic_execution_order=True,rows=rows);(a.output/'inventory.json').write_text(json.dumps(data,indent=2)+'\n');print(json.dumps({k:v for k,v in data.items() if k!='rows'}))
 # New helper data deliberately covers only this inventory, not unproved code.
 lines=['// Actual original vtable catalog; size/hash proof in reference/character-state-methods.','constexpr Method methods[]={']
 lines.extend(' {%d,%d,%su,%s},'%(x['state'],x['operation'],x['source_function'],'true' if x['empty'] else 'false') for x in rows);lines.append('};');(a.output/'native-methods.inc').write_text('\n'.join(lines)+'\n')
if __name__=='__main__':main()
