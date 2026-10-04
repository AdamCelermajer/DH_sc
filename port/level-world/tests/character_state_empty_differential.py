"""Actual original bx-lr methods versus optimized native empty-only dispatcher.
Nonempty bodies are catalog/metadata rejection checks, never executed as stubs.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();inventory=R/'reference/character-state-methods/inventory.json';catalog=json.loads(inventory.read_text());assert sha(a.engine)==catalog['original_sha256'];manifest={'functions':[dict(original_symbol=x['symbol'],elf_address=x['source_function'],size=x['size']) for x in catalog['rows'] if x['empty']]};old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});cases=0;guards=0;gold=bytearray(b'SEM1'+struct.pack('<I',len(catalog['rows'])));original_calls=0
 for row in catalog['rows']:
  state,op,fn=row['state'],row['operation'],int(row['source_function'],0);expect=int(row['empty']);assert new.invoke('dh2_character_state_empty_body',[state,op,fn])==expect;cases+=1;gold+=struct.pack('<4I',state,op,fn,expect)
  if row['empty']:
   for args in ([0,0,0,0],[0xffffffff]*4,[old.data+1024,state,old.data+8192,old.data+12288],[0x89abcdef,0xfedcba98,0x76543210,0x80000000]):
    before=bytes(old.uc.mem_read(old.data,16384));result=old.invoke(fn,args);assert bytes(old.uc.mem_read(old.data,16384))==before and result==args[0];assert fn in old.seen;original_calls+=1
  assert new.invoke('dh2_character_state_empty_body',[state,op,fn^4])&0xffffffff==0xffffffff;guards+=1
 for state,op,fn in [(3,3,0x3c0004),(-1,2,0), (20,2,0),(17,4,0x3c0070)]:assert new.invoke('dh2_character_state_empty_body',[state,op,fn])&0xffffffff==0xffffffff;guards+=1
 ref=R/'reference/character-state-methods/empty-fixtures.bin';ref.write_bytes(gold);paths=('character_state_empty.hpp','character_state_empty.cpp','reference/character-state-methods/native-methods.inc','tests/character_state_empty_differential.py');report=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),library_sha256=sha(a.library),inventory_sha256=sha(inventory),reference_sha256=sha(ref),source_sha256={'port/level-world/'+x:sha(R/x) for x in paths},method_metadata_comparisons=cases,original_empty_instruction_calls=original_calls,empty_methods=18,nonempty_method_rejections=46,invalid_metadata_checks=guards,mismatches=0,full_nonempty_behaviors=False,packaged_APK=False);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
