"""Original IncSkill gates, ordered services and mutable save-row reread.
Underlying property/availability/cap/update services remain explicit. Source
increment/uint16 wrap, direct stored potion byte and branches run ARM32/O2.
Debug string allocation and release are explicit discarded storage services.
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
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=Original(ROOT/'.local-inputs/libDungeonHunter2.so',{'functions':[]});new=Cpu(a.library,True,{'functions':[]});char=old.data+0x4000;save=old.data+0x6000;rows=old.data+0x7000;alternate=old.data+0x7100;out=new.data+0x1000;svc=new.data+0x2000;callback=new.data+0x30000;owner=0x123400005678;new.uc.mem_write(svc,struct.pack('<QQ',0xABCD56781234,callback))
 values=[];oldtrace=[];newtrace=[];difficulty_calls=0;live_level=0;capacity=0;property_points=0
 def response(op,args):
  nonlocal difficulty_calls,live_level,capacity,property_points
  if op in (0x3bcc74,0x3bcc94):return 1
  if op==0x3df6e0:return property_points if args[0]==157 else values[8]
  if op==0x3bca50:return values[1]
  if op==0x4c4bdc:return values[2+args[0]]
  if op==0x3bb918:difficulty_calls+=1;return values[5] if difficulty_calls==1 else values[6]
  if op==0x3bcd94:return live_level
  if op==0x3bc9ec:return values[7]
  if op==0x3e0798:property_points-=1;live_level=values[10] if values[9] else live_level;return 0
  if op==0x3bceb8:live_level=(live_level+1)&65535
  if op==0x3bcee4:capacity=args[0]
  return 0
 def oldhook(uc,address,size,user):
  op=address;args=[0]*5
  if address in (0x3bcc74,0x3bcc94):oldtrace.append(W(op,*args));return
  if address==0x3bcd94:oldtrace.append(W(address,0,0,0,0,0));return
  if address==0x3bceb8:oldtrace.append(W(address,0,0,0,0,0));return
  if address==0x3bcee4:oldtrace.append(W(address,old.reg(0)&255,0,0,0,0));return
  if address in (0x3bcc08,0x3139ac):old.returned();return
  if op not in (0x3df6e0,0x3bca50,0x4c4bdc,0x3bb918,0x3bc9ec,0x3e0798,0x3d8894,0x3e0810,0x337888,0x337a88):return
  if op==0x3df6e0:args[:2]=[old.reg(1),old.reg(2)]
  if op==0x3bca50 or op==0x3bc9ec:args[0]=old.reg(1)
  if op==0x4c4bdc:
   name=bytes(uc.mem_read(old.reg(2),64)).split(b'\0')[0];args[0]=[b'MaxSkillLevelBNormal',b'MaxSkillLevelCHard',b'MaxSkillLevelDVeryHard'].index(name)
  if op==0x3e0798:args[:2]=[old.reg(1),old.reg(2)]
  if op==0x3e0810:args[0]=old.reg(1)
  if op in (0x337888,0x337a88):args[0]=uc.reg_read(old.lr)-4
  oldtrace.append(W(op,*args));v=response(op,args)
  if op==0x3e0798 and values[9]:old.pointer(save+0x80,alternate)
  old.returned(v)
 def newhook(uc,address,size,user):
  if address!=callback:return
  assert new.reg(0)==0xABCD56781234;identity,op,*args=struct.unpack('<Q6I',uc.mem_read(new.reg(1),32));assert identity==owner;newtrace.append(W(op,*args));v=response(op,args);uc.mem_write(new.reg(2),W(v,0));new.put(0,0);uc.reg_write(new.pc,uc.reg_read(new.lr))
 old.uc.hook_add(UC_HOOK_CODE,oldhook);new.uc.hook_add(UC_HOOK_CODE,newhook);rng=random.Random(0xF129);cases=[];total=0;success=0;mutations=0
 for k in range(768):
  test=k%3==0;values=[rng.choice((-1,0,1,2,256)),rng.choice((0,1,255)),rng.choice((-1,0,1,5,65536)),rng.choice((0,3,8)),rng.choice((0,10,65536)),rng.choice((0,1,2,3)),rng.choice((0,1,2,3)),rng.choice((0,1,255)),rng.choice((-1,0,1,255,256,3072,2147483647)),rng.choice((0,1)),rng.choice((0,1,32767,65535)),rng.choice((0,1,32767,65535))]
  old.uc.mem_write(char,bytes(0x1600));old.pointer(char+0x14e8,save);old.pointer(save+0x80,rows);old.uc.mem_write(rows+4,struct.pack('<H',values[11]));old.uc.mem_write(alternate+4,struct.pack('<H',values[10]));uc=old.uc;uc.mem_write(char+0x3a8,b'\xa5');oldtrace.clear();difficulty_calls=0;live_level=values[11];capacity=165;property_points=values[0];expected=old.invoke(0x3bcc58,[char,0,int(test)]);snapshot=W(old.word(save+0x80)==alternate,struct.unpack('<H',uc.mem_read(old.word(save+0x80)+4,2))[0],uc.mem_read(char+0x3a8,1)[0],property_points)
  newtrace.clear();difficulty_calls=0;live_level=values[11];capacity=165;property_points=values[0];new.uc.mem_write(out,W(0xa5a5a5a5));assert new.invoke('dh2_player_increment_skill_v2',[out,owner,0,int(test),svc])==0;got=struct.unpack('<I',new.uc.mem_read(out,4))[0];native_snapshot=W(int(values[9] and got and not test),live_level,capacity,property_points);assert expected==got and oldtrace==newtrace and snapshot==native_snapshot,(k,values,expected,got,snapshot.hex(),native_snapshot.hex(),[v.hex() for v in oldtrace],[v.hex() for v in newtrace]);total+=len(oldtrace);success+=got;mutations+=got and not test;cases.append(W(test,*values,expected)+snapshot+W(len(oldtrace))+b''.join(oldtrace))
 gold=b'PIV2'+W(len(cases))+b''.join(cases);ref=ROOT/'port/level-world/reference/player-initial-grants-v2';(ref/'skill-fixtures.bin').write_bytes(gold);sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();report={'validation':'PASS','comparisons':len(cases),'ordered_requests':total,'accepted_skill_checks':success,'mutating_grants':mutations,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'library_sha256':sha(a.library),'gold_sha256':hashlib.sha256(gold).hexdigest(),'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):sha(p) for p in [ROOT/'port/level-world/player_initial_grants_v2.hpp',ROOT/'port/level-world/player_initial_grants_v2.cpp']},'script_sha256':sha(__file__),'scope':__doc__,'mismatches':0};(ROOT/'port/level-world/reports/player-skill-increment-v2-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
