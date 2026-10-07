"""Whole original GetTargetPosition pointer selector, all enable-byte values."""
from pathlib import Path
import sys,json,hashlib
root=Path(__file__).resolve().parents[3];sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
oldpath=root/'.local-inputs/libDungeonHunter2.so';newpath=root/'.local-inputs/libcharacter_world_target_owner_v1_oracle.so'
old=Cpu(oldpath,False,{'functions':[]});new=Cpu(newpath,True,{'functions':[]})
actor=old.data+0x1000;position=new.data+0x2000;cached=new.data+0x3000;records=[]
for node in [0,1,0x7fffffff,0xffffffff]:
 for enabled in range(256):
  old.pointer(actor+0x180,node);old.uc.mem_write(actor+0x80,bytes([enabled]))
  original=old.invoke(0x3935dc,[actor])-actor
  result=new.invoke('dh2_world_target_position_v1',[position,cached,node,enabled]);native=0x184 if result==cached else 0x160 if result==position else -1
  assert original==native,(node,enabled,original,native)
  records.append([node,enabled,original])
ref=root/'port/level-world/reference/character-world-target-owner-v1';ref.mkdir(exist_ok=True)
gold=ref/'target-position-gold.json';gold.write_text(json.dumps(records)+'\n')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'validation':'PASS','cases':len(records),'mismatches':0,'original_sha256':sha(oldpath),'optimized_sha256':sha(newpath),'gold_sha256':sha(gold),'scope':__doc__}
(root/'port/level-world/reports/character-world-target-position-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
