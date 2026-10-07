"""Original Character spawn_delay registration and constructor store block, with explicit property/allocation services."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[1];REPO=R.parents[1];sys.path.insert(0,str(R/'tests'))
from character_script_selection_differential import Cpu,string
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 c=Cpu(a.engine,False,json.loads((R/'reference/character-spawn-state/original-functions.json').read_text()));assert sha(a.engine)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 char=c.data+0x1000;heap=c.data+0x8000;registrations=[];allocations=[]
 def w(addr):return struct.unpack('<I',c.uc.mem_read(addr,4))[0]
 def ret(v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(uc,address,size,unused):
  nonlocal heap
  if address==0x38cee8:ret()
  elif address in (0x33ef7c,0x3a92b4):
   assert c.reg(0)==char+4;registrations.append(dict(name=string(c,c.reg(1)).decode(),field_offset=hex(c.reg(2)-char),service=hex(address)));ret()
  elif address==0x310570:
   size=c.reg(0);allocations.append(size);v=heap;heap+=0x100;uc.mem_write(v,bytes(size));ret(v)
  elif address==0x3140ec:ret(c.reg(0))
  elif address==0x513ce4:
   assert c.reg(0)==char+4;descriptor=c.reg(2);vt=w(descriptor)
   names=[name for name,value in c.symbols.items() if value+8==vt and name.startswith('_ZTV')]
   registrations.append(dict(name=string(c,c.reg(1)).decode(),field_offset=hex(w(descriptor+4)+4),service=hex(address),vtable=hex(vt),vtable_symbols=names));ret()
 c.uc.hook_add(UC_HOOK_CODE,hook);c.invoke(0x3a9fe4,(char,));spawn=[x for x in registrations if x['name']=='spawn_delay'];assert len(spawn)==1 and spawn[0]['field_offset']=='0x1434'
 for initial in (0,1,0xffffffff,0x7fffffff):
  c.uc.mem_write(char+0x1434,struct.pack('<II',initial,initial));c.put(4,char);c.put(8,0);c.uc.emu_start(0x3aa414,0x3aa424,count=4);assert w(char+0x1434)==w(char+0x1438)==0
 report=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),script_sha256=sha(Path(__file__)),ordered_registrations=registrations,descriptor_allocations=allocations,spawn_delay=spawn[0],constructor_zero_store_cases=4,constructor_zero_register='r8=0 established by source mov at0x3aa240; selected store block executes actual3aa414..3aa420.',full_constructor_and_property_text_loader=False)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
