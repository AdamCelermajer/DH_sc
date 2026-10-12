"""Actual ARM32 load-stage dispatch, InitPost retry, and InitScriptProcess branch probes."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 c=Cpu(a.engine,False,{'functions':[]});level=c.data+0x1000;character=c.data+0x2000;manager=c.data+0x5000;vt=c.data+0x6000;ai=character+0x3c8
 def write(addr,val):c.uc.mem_write(addr,struct.pack('<I',val))
 def word(addr):return struct.unpack('<I',c.uc.mem_read(addr,4))[0]
 stages=[]
 for stage,target in ((10,0x3f70cc),(18,0x3f76c8)):
  write(level+0x130,stage);c.put(4,level);c.uc.emu_start(0x3f6a28,target,count=8)
  assert c.uc.reg_read(c.pc)==target
  stages.append(dict(stage=stage,jump_table_entry=hex(0x3f6a38+4*stage),target=hex(target)))
 write(manager+0x38,character);c.put(7,manager);retries=[];callbacks=[]
 def hook(uc,address,size,unused):
  if address==0x34552c:
   assert c.reg(0)==character;retries.append(0 if len(retries)<2 else 1);c.put(0,retries[-1]);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif address in (0x3b3a70,0x3ce044,0x3d8894,0x3c0070,0x3c2810):
   callbacks.append(address);c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 h=c.uc.hook_add(UC_HOOK_CODE,hook)
 c.uc.emu_start(0x3f7114,0x3f6e90,count=32);assert retries==[0,0,1]
 write(ai,vt);write(ai+4,character);write(vt+0xc,0x3c0070);write(vt+0x10,0x3c2810)
 script=[]
 for init_post in (0,1):
  callbacks.clear();c.invoke(0x3ce7c0,(ai,init_post))
  expected=[0x3b3a70,0x3ce044,0x3d8894,0x3c0070]+([0x3c2810] if init_post else [])
  assert callbacks==expected;script.append(dict(init_post=init_post,ordered_services=[hex(x) for x in callbacks]))
 c.uc.hook_del(h)
 branches=[]
 for byte,target in ((0,0x3b54c8),(1,0x3b5444),(255,0x3b5444)):
  c.uc.mem_write(character+0x3ec,bytes([byte]));c.put(4,character);c.uc.emu_start(0x3b5438,target,count=8)
  assert c.uc.reg_read(c.pc)==target
  if byte==0:assert c.reg(0)==ai and c.reg(1)==0
  branches.append(dict(character_3ec=byte,target=hex(target),calls_init_script_process=byte==0,init_post_argument=0 if byte==0 else None))
 vt_address=c.symbols['_ZTV9Character'];init_post=word(vt_address+8+0x1c);assert init_post==0x3b4d60
 refs=[R/'reference/character-pre-spawn/producers/original-functions.json',R/'reference/character-pre-spawn/producers/reference/original-functions.asm',R/'reference/character-pre-spawn/object-init/original-functions.json',R/'reference/character-pre-spawn/object-init/reference/original-functions.asm']
 data=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),script_sha256=sha(Path(__file__)),capture_sha256={x.relative_to(REPO).as_posix():sha(x) for x in refs},load_stage_dispatch=stages,init_post_retry_results=retries,character_vtable=dict(symbol='_ZTV9Character',address=hex(vt_address),address_point=hex(vt_address+8),slot='0x1c',function=hex(init_post)),character_script_init_branch=branches,init_script_process=script,services='InitPost and deeper script/skill/virtual bodies are explicit return-only probe services; dispatch/retry/branch/order execute original instructions.',complete_level_load_executed=False,active_AIS_publication='Must compose genuine lifecycle; these probes do not invent whether an active AIS is null.')
 a.output.write_text(json.dumps(data,indent=2)+'\n');print(json.dumps(data))
if __name__=='__main__':main()
