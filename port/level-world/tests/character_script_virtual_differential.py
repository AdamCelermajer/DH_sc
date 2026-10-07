"""Original-executed VCB/initial virtual evidence versus O2 ARM64; alias/VM service implementation separately audited."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu as Base,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='dh2_script_alias_contains':
   query=string(self,self.reg(1)).decode();i=len(self.queries);value=(self.mask>>i)&1
   self.queries.append(dict(service='alias_membership',name=query,result=value,flags_before=hex(struct.unpack('<I',uc.mem_read(self.flags,4))[0])));self.put(0,value)
  elif name=='dh2_script_alias_call_discard_source':
   assert self.reg(3)==self.reg(4)==0;self.calls.append(string(self,self.reg(2)).decode());self.put(0,0)
  else:return super().external(uc,address,size,unused)
  uc.reg_write(self.pc,uc.reg_read(self.lr))
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args()
 evidence=ROOT/'reference/character-script-kinds/kind-probe.json';source=json.loads(evidence.read_text());assert source['validation']=='PASS'
 c=Cpu(a.library,True,{'functions':[]});c.flags=c.data+4096;alias=c.data+8192;session=c.data+12288;c.uc.mem_write(session,struct.pack('<QQ',c.data+16384,alias));gold=bytearray(b'VSC1'+struct.pack('<I',len(source['vcb_cases'])))
 for case in source['vcb_cases']:
  external=int(case['function']=='0x3dcec8');c.mask=case['mask'];c.queries=[];c.uc.mem_write(c.flags,struct.pack('<I',0xf0f0f0f0))
  assert c.invoke('dh2_character_script_init_vcb',[c.flags,alias,external])==0
  result=struct.unpack('<I',c.uc.mem_read(c.flags,4))[0];assert result==case['result'] and c.queries==case['queries']
  gold+=struct.pack('<III',external,c.mask,result)
 initial=0
 vtables=json.loads((ROOT/'reference/character-script-kinds/vtables.json').read_text())['vtables']
 for kind,name in enumerate(['AISDefault','AISMonster','AISPlayerIPhone','AISFaery','AISExternal','AISPlayer']):
  vt=next(x for x in vtables if x['kind']==name)
  for slot,label in [(8,'OnInit'),(12,'OnInitPost'),(16,'OnInitFinal'),(20,'OnTerminate')]:
   address=next(x['address'] for x in vt['slots'] if x['slot']==hex(slot))
   if kind!=4:assert address=={8:'0x3dbe78',12:'0x3dbe7c',16:'0x3dbe80',20:'0x3dbe84'}[slot]
   else:
    original=next(x for x in source['factory_and_lifecycle_cases'] if x['case']=='AISExternal-virtual-'+hex(slot));assert original['function']==address and any(x.get('name')==label for x in original['trace'])
   c.calls=[];assert c.invoke('dh2_character_script_initial_virtual',[session,kind,slot])==0;assert c.calls==([label] if kind==4 else []);initial+=1
 guards=0;c.uc.mem_write(c.flags,struct.pack('<I',0xa5a5a5a5))
 for name,args in [('dh2_character_script_init_vcb',[c.flags,0,1]),('dh2_character_script_init_vcb',[c.flags,alias,2]),('dh2_character_script_init_vcb',[0,alias,1]),('dh2_character_script_initial_virtual',[0,4,8]),('dh2_character_script_initial_virtual',[session,6,8]),('dh2_character_script_initial_virtual',[session,4,9])]:
  assert c.invoke(name,args)&0xffffffff==0xffffffff;assert struct.unpack('<I',c.uc.mem_read(c.flags,4))[0]==0xa5a5a5a5;guards+=1
 ref=ROOT/'reference/character-script-kinds';(ref/'virtual-fixtures.bin').write_bytes(gold)
 sources=['character_script_virtual.hpp','character_script_virtual.cpp','tests/character_script_virtual_differential.py','tools/build_character_script_virtual_oracle.ps1']
 report=dict(validation='PASS',scope=__doc__,original_sha256=source['original_sha256'],original_evidence_sha256=sha(evidence),optimized_arm64_library_sha256=sha(a.library),source_sha256={str((ROOT/x).relative_to(REPO)):sha(ROOT/x) for x in sources},vcb_comparisons=len(source['vcb_cases']),initial_virtual_comparisons=initial,comparisons=len(source['vcb_cases'])+initial,native_atomic_rejections=guards,mismatches=0,gold_sha256=hashlib.sha256(gold).hexdigest(),queries_are_alias_membership=True,full_additional_kind_ownership_executed=False)
 output=ROOT/'reports/character-script-virtual-arm64-differential.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',comparisons=report['comparisons'],guards=guards)))
if __name__=='__main__':main()
