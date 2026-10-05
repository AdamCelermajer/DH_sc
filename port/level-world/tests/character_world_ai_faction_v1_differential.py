"""Whole original Character.GetCharAIFactionId versus optimized native code."""
from pathlib import Path
import sys,json,hashlib,struct
root=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
oldpath=root/'.local-inputs/libDungeonHunter2.so'
newpath=root/'.local-inputs/libcharacter_world_ai_v1_oracle.so'
old=Cpu(oldpath,False,{'functions':[]})
new=Cpu(newpath,True,{'functions':[]})
word=lambda at:struct.unpack('<I',bytes(old.uc.mem_read(at,4)))[0]
countptr=old.data+0x4000
got=(0x3a3194+word(0x3a31b0))&0xffffffff
old.pointer((got+word(0x3a31b4))&0xffffffff,countptr)
actor=old.data+0x1000;props=old.data+0x3000;nativeprops=new.data+0x3000
records=[]
for count in (0,1,5,10,11,16,255,65536,0x7fffffff):
 for ident in (-2147483648,-1,0,1,4,5,9,10,11,15,16,254,255,65535,65536,2147483647):
  old.uc.mem_write(countptr,struct.pack('<I',count));old.uc.mem_write(actor+0xff8,struct.pack('<i',ident));new.uc.mem_write(nativeprops,struct.pack('<i',ident))
  original=old.invoke(0x3a3180,[actor])
  native=new.invoke('dh2_world_ai_faction_v1',[nativeprops,count])
  assert original==native,(count,ident,original,native)
  records.append([count,ident,original])
ref=root/'port/level-world/reference/character-world-target-owner-v1'
gold=ref/'faction-gold.json';gold.write_text(json.dumps(records)+'\n')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'validation':'PASS','cases':len(records),'mismatches':0,'original_sha256':sha(oldpath),'optimized_sha256':sha(newpath),'gold_sha256':sha(gold),'scope':__doc__,'whole_relationship_wrappers_proven':False}
(root/'port/level-world/reports/character-world-ai-faction-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
