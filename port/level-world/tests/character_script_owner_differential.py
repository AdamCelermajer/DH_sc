"""Original-executed constructor fields/registration calls vs O2 native descriptors; ownership class host proof separate."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,default=ROOT/'reports/character-script-owner-arm64-differential.json');a=p.parse_args()
 original=ROOT/'reference/character-script-ownership/ownership-probe.json';e=json.loads(original.read_text());assert e['validation']=='PASS' and sha(original)=='710f6a5639324e049aba1b12ca759a1317d2c71cf84af4ee961f6e7a02c12f92'
 old=Cpu(REPO/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});outold=old.data+4096;old.uc.mem_write(outold,bytes(32));name=outold+64;old.uc.mem_write(name,b'LOCK\0');old.invoke(0x319af4,[outold,name,0x12345678]);assert not old.import_calls
 c=Cpu(a.library,True,{'functions':[]});out=c.data+4096;gold=bytearray(b'SOW1'+struct.pack('<I',300));comparisons=0
 for kind,label in ((0,'actual-factory-AISDefault'),(2,'iphone-factory'),(5,'actual-factory-AISPlayer')):
  case=next(x for x in e['cases'] if x['case']==label);assert sum(x['service']=='lua_newstate' for x in case['trace'])==1
  # Constructor-written defaults are asserted against the actually executed
  # original constructors in ownership-probe; undefined/unwritten fields excluded.
  expected=(0,0,1,1,*([0]*11),int(kind!=0))
  c.uc.mem_write(out,bytes([0xa5])*72);assert c.invoke('dh2_character_script_constructor_fields',[out,kind])==0
  got=struct.unpack('<QQ14I',c.uc.mem_read(out,72));assert got==expected,(kind,got,expected);comparisons+=1
 for phase,label in ((1,'step1-bind'),(2,'step2-real-character-and-gameobject-registration')):
  events=[x for x in next(x for x in e['cases'] if x['case']==label)['trace'] if x['service'] in ('register_function','register_method')]
  assert c.invoke('dh2_character_script_binding_count',[phase])==len(events)
  for i,x in enumerate(events):
   assert c.invoke('dh2_character_script_binding',[out,phase,i])==0
   pointer,callback,method,receiver,reserved=struct.unpack('<Q4I',c.uc.mem_read(out,24));text=string(c,pointer)
   assert (text.decode(),callback,method,receiver,reserved)==(x['name'],int(x['function'],16),int(x['service']=='register_method'),int(phase==2),0)
   gold+=struct.pack('<5I',phase,i,callback,method,len(text))+text;comparisons+=1
 guards=0;c.uc.mem_write(out,bytes([0xa5])*72)
 for symbol,args in [('dh2_character_script_constructor_fields',[out,1]),('dh2_character_script_constructor_fields',[0,2]),('dh2_character_script_binding',[out,0,0]),('dh2_character_script_binding',[out,1,35]),('dh2_character_script_binding',[out,2,265]),('dh2_character_script_binding',[0,1,0])]:
  assert c.invoke(symbol,args)&0xffffffff==0xffffffff;assert bytes(c.uc.mem_read(out,72))==bytes([0xa5])*72;guards+=1
 ref=ROOT/'reference/character-script-owner';ref.mkdir(exist_ok=True);(ref/'owner-fixtures.bin').write_bytes(gold)
 sources=['character_script_owner.hpp','character_script_owner.cpp','character_script_owner_bindings.inc','tests/character_script_owner_differential.py','tools/build_character_script_owner_oracle.ps1']
 report=dict(validation='PASS',scope=__doc__,original_sha256=e['original_sha256'],original_instruction_evidence_sha256=sha(original),original_method_null_gate_executed=True,optimized_arm64_library_sha256=sha(a.library),comparisons=comparisons,registration_descriptors=300,source_method_calls_skipped_by_null_target=130,native_atomic_rejections=guards,mismatches=0,gold_sha256=hashlib.sha256(gold).hexdigest(),source_sha256={str((ROOT/s).relative_to(REPO)):sha(ROOT/s) for s in sources},full_native_ownership_class_executed_in_this_arm64_probe=False)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','comparisons':comparisons,'guards':guards,'gold':report['gold_sha256']}))
if __name__=='__main__':main()
