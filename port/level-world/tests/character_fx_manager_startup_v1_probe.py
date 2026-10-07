"""Execute original constructor/PreCacheLibraries before native owner pooling.
Debug/files and C++ temporary string ownership are explicit caller boundaries.
"""
import json,hashlib,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,words,word
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 root=R/'.local-inputs/character-fx-owner-v1';manifest=json.loads((root/'manager-constructor/original-functions.json').read_text());manifest['functions']+=json.loads((root/'precache/original-functions.json').read_text())['functions'];c=Cpu(R/'.local-inputs/libDungeonHunter2.so',False,manifest);assert sha(R/'.local-inputs/libDungeonHunter2.so')==manifest['original_sha256'];manager=c.data+0x1000;log=[];enabled=0
 def ret(value=0):c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(uc,address,size,user):
  if address==0x337888:log.append('Debug.Load');ret()
  elif address==0x3140ec:
   assert c.string(c.reg(1))==b'AnimatedFX';uc.mem_write(c.reg(0),bytes(24));ret()
  elif address==0x337ec8:log.append('Debug.GetModule:AnimatedFX');ret(enabled)
 c.uc.hook_add(UC_HOOK_CODE,hook);rows=[]
 for ctor in (0x493260,0x4932c4):
  for enabled in (0,1):
   c.uc.mem_write(manager,b'\xa5'*64);c.invoke(ctor,[manager]);before=bytes(c.uc.mem_read(manager,52));assert before[4]==0 and word(before,8)==manager+8 and word(before,12)==manager+8 and all(word(before,p)==0 for p in range(16,52,4));log=[];c.invoke(0x495a88,[manager]);after=bytes(c.uc.mem_read(manager,52));assert after[4]==enabled and before[:4]==after[:4] and before[5:]==after[5:] and log==['Debug.Load','Debug.GetModule:AnimatedFX'];rows.append({'constructor':hex(ctor),'module_result':enabled,'constructor_precache':before[4],'final_precache':after[4],'ordered_services':log})
 out=R/'port/level-world/reference/character-fx-owner-v1/manager-startup-probe.json';out.write_text(json.dumps({'validation':'PASS','original_sha256':manifest['original_sha256'],'cases':rows,'scope':__doc__,'script_sha256':sha(Path(__file__))},indent=2)+'\n');print(json.dumps({'validation':'PASS','original_startup_cases':len(rows)}))
if __name__=='__main__':main()
