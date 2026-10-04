"""Current O2 owner constructor/catalogue helpers with original external factory and ordered stage evidence; class host proof separate."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();cpu=Cpu(a.library,True,{'functions':[]});out=cpu.data+4096
 constructors=json.loads((ROOT/'reference/character-script-kinds/kind-probe.json').read_text());external=next(x for x in constructors['factory_and_lifecycle_cases'] if x['case']=='AISExternal-factory');assert external['allocation_bytes']==0xc4 and all(external['initial_words'][hex(x)]=='0x0' for x in [0x98,0xb4,0xb8,0xbc,0xc0])
 comparisons=0
 for kind in [0,2,4,5]:
  cpu.uc.mem_write(out,bytes([0xa5])*72);assert cpu.invoke('dh2_character_script_constructor_fields',[out,kind])==0
  assert struct.unpack('<QQ14I',cpu.uc.mem_read(out,72))==(0,0,1,1,*([0]*11),int(kind in [2,5]));comparisons+=1
 gold=ROOT/'reference/character-script-owner/owner-fixtures.bin';raw=gold.read_bytes();assert raw[:4]==b'SOW1';at=8
 for _ in range(300):
  phase,index,callback,method,size=struct.unpack_from('<5I',raw,at);at+=20;name=raw[at:at+size];at+=size
  assert cpu.invoke('dh2_character_script_binding',[out,phase,index])==0
  pointer,actual_callback,actual_method,receiver,reserved=struct.unpack('<Q4I',cpu.uc.mem_read(out,24));assert string(cpu,pointer)==name and (actual_callback,actual_method,receiver,reserved)==(callback,method,int(phase==2),0);comparisons+=1
 assert at==len(raw);guards=0;cpu.uc.mem_write(out,bytes([0xa5])*72)
 for args in [[out,1],[out,3],[out,6],[0,4]]:
  assert cpu.invoke('dh2_character_script_constructor_fields',args)&0xffffffff==0xffffffff;assert bytes(cpu.uc.mem_read(out,72))==bytes([0xa5])*72;guards+=1
 order=ROOT/'reference/character-script-owner-extension/external-owner-probe.json';ordered=json.loads(order.read_text());assert ordered['validation']=='PASS'
 source=['character_script_owner.hpp','character_script_owner.cpp','character_script_owner_bindings.inc','tests/character_script_owner_external_differential.py','tools/build_character_script_owner_oracle.ps1']
 report=dict(validation='PASS',scope=__doc__,original_sha256=ordered['original_sha256'],original_external_constructor_evidence_sha256=sha(ROOT/'reference/character-script-kinds/kind-probe.json'),original_stage_order_evidence_sha256=sha(order),original_stage_order_cases=len(ordered['cases']),optimized_arm64_library_sha256=sha(a.library),comparisons=comparisons,native_atomic_rejections=guards,mismatches=0,legacy_gold_sha256=sha(gold),source_sha256={str((ROOT/x).relative_to(REPO)):sha(ROOT/x) for x in source},full_owner_class_executed_this_arm64_probe=False)
 output=ROOT/'reports/character-script-owner-external-arm64-differential.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=comparisons,guards=guards,original_stage_order_cases=len(ordered['cases']))))
if __name__=='__main__':main()
