"""Actual Character._InitEquipment and _InitSkillsSlots vs optimized ARM64.
Live field/getter/service responses are controlled and explicit. Captured item
count, uint8 online fields, bool results and all outgoing arguments execute
original instructions; no campaign or native auto-equip success is inferred.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from items_differential import Original
from navigation_differential import Cpu
from unicorn import UC_HOOK_CODE
W=lambda *v:struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=Original(ROOT/'.local-inputs/libDungeonHunter2.so',{'functions':[]});new=Cpu(a.library,True,{'functions':[]})
 character=old.data+0x4000;vt=old.data+0x7000;online=old.data+0x8000;record=old.data+0x9000;svc=new.data+0x2000;own=0x123400005678;old.pointer(character,vt);old.pointer(vt+0x130,old.data+0x30000);new.uc.mem_write(svc,struct.pack('<QQ',0xABCD12345678,new.data+0x30000))
 oldtrace=[];newtrace=[];values=[];after=False
 def response(op,args):
  nonlocal after
  if op==0x7fd794:return values[0]
  if op==0x36eea8:return values[1]
  if op==0x3fc608:return values[5] if after else values[2]
  if op==0x3b3994:return values[3]
  if op==0x3dedb4:return values[4]
  if op==0x40407c:after=True;return 0
  if op==0x3fdeec:return (values[6]>>args[0])&1
  if op==0x3bbea0:return values[7]
  if op==0x3bbed0:return values[8]
  return values[9]
 def oldhook(uc,address,size,user):
  op=address;args=[0]*5
  if address==0x3b3994:
   oldtrace.append(W(op,*args));return
  if address==old.data+0x30000:op=0x3a9fa8
  if op not in (0x7fd794,0x36eea8,0x3fc608,0x3dedb4,0x40407c,0x3fdeec,0x3a9fa8,0x3a999c,0x3bbea0,0x3bbe54,0x3fc6c8,0x3bbed0,0x3bcc58):return
  if op==0x36eea8:assert old.reg(1)==character and old.reg(2)==0
  elif op in (0x3fdeec,0x3a9fa8,0x3bbed0):args[0]=old.reg(1)
  elif op in (0x3bbe54,0x3bcc58):args[:2]=[old.reg(1),old.reg(2)]
  elif op==0x3dedb4:args[0]=old.reg(2);assert old.reg(0)==character+0x560 and old.reg(1)==character+0xff4
  elif op==0x40407c:args[:3]=[old.reg(i) for i in (1,2,3)];args[3:]=struct.unpack('<2I',uc.mem_read(uc.reg_read(old.sp),8));assert old.reg(0)==character+0x37c
  oldtrace.append(W(op,*args));value=response(op,args)
  if op==0x7fd794:uc.mem_write(online+5,bytes([value&255]));old.returned(online)
  elif op==0x36eea8:uc.mem_write(record+0x66c,bytes([value&255]));old.returned(record)
  else:old.returned(value)
 def newhook(uc,address,size,user):
  if address!=new.data+0x30000:return
  assert new.reg(0)==0xABCD12345678;ptr=new.reg(1);identity,op,*args=struct.unpack('<Q6I',uc.mem_read(ptr,32));assert identity==own;newtrace.append(W(op,*args));value=response(op,args);uc.mem_write(new.reg(2),W(value,0));new.put(0,0);uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,oldhook);new.uc.hook_add(UC_HOOK_CODE,newhook);rng=random.Random(0xF128);cases=[];total=0
 for k in range(768):
  kind=int(k>=512);values=[rng.choice((0,1,2,255)),rng.choice((0,1,2,255)),rng.choice((0,0,1,-1,7)),rng.choice((0,0,1,-1)),rng.choice((-1,0,165)),rng.randrange(9),rng.getrandbits(8),rng.choice((0,0,1,255)),rng.choice((0,0,1,65535)),rng.choice((0,1,255))]
  # Online fields are actual bytes; all scalar callbacks are otherwise words.
  old.uc.mem_write(character+0x39c,W(values[3]));oldtrace.clear();after=False;old.invoke(0x3b395c if kind==0 else 0x3b3a90,[character])
  newtrace.clear();after=False;assert new.invoke('dh2_player_initial_equipment_v2' if kind==0 else 'dh2_player_initial_skill_slots_v2',[own,svc])==0;assert oldtrace==newtrace,(k,[x.hex() for x in oldtrace],[x.hex() for x in newtrace]);total+=len(oldtrace);cases.append(W(kind,*values,len(oldtrace))+b''.join(oldtrace))
 gold=b'PGV2'+W(len(cases))+b''.join(cases);ref=ROOT/'port/level-world/reference/player-initial-grants-v2';ref.mkdir(parents=True,exist_ok=True);(ref/'fixtures.bin').write_bytes(gold)
 sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();report={'validation':'PASS','comparisons':len(cases),'ordered_requests':total,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'library_sha256':sha(a.library),'gold_sha256':hashlib.sha256(gold).hexdigest(),'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):sha(p) for p in [ROOT/'port/level-world/player_initial_grants_v2.hpp',ROOT/'port/level-world/player_initial_grants_v2.cpp']},'script_sha256':sha(__file__),'scope':__doc__,'mismatches':0};(ROOT/'port/level-world/reports/player-initial-grants-v2-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
