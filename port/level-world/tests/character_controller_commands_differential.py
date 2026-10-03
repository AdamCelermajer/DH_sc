"""Actual controller gates, Character thunks, remote and target getters.

GameObject.Stop, PathTo, LookAt(Point) and RaiseEvent are synchronous services.
Their exact identity, coordinates, arguments and call order are compared.
"""
import argparse, hashlib, itertools, json, random, struct, sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu

def words(*v): return struct.pack('<'+'I'*len(v), *(x & 0xffffffff for x in v))
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()

class Machine:
 def __init__(self, path, native, manifest):
  self.c=TimelineCpu(path,native,manifest); self.native=native; c=self.c; d=c.data
  self.state=d+0x1000; self.services=d+0x2000; self.callback=d+0x3000
  self.controller=d+0x4000; self.owner=d+0x10000; self.target=d+0x20000
  self.vtable=d+0x5000; self.control_vtable=d+0x6000
  self.ids={self.owner:1,self.target:2}; self.calls=[]
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:
   c.pointer(0x9980e8,0x9a318b)
   c.pointer(self.owner,self.vtable)
   c.pointer(self.vtable+0x54,0x33dd10); c.pointer(self.vtable+0xd0,0x3addbc)
   c.pointer(self.owner+0x374,self.control_vtable)
   for off,entry in ((0x14,0x3ad9a4),(0x30,0x3ad9dc),(0x34,0x3ad888)):
    c.pointer(self.control_vtable+off,entry)
   c.pointer(self.controller+4,self.owner+0x374)
 def finish(self,status=1):
  c=self.c; c.put(0,status); c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def record(self,service,subject,argument=0,position=bytes(12)):
  self.calls.append(words(service,argument)+struct.pack('<Q',self.ids[subject])+position+words(0))
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.callback:return
   service,argument,subject,x,y,z,reserved=struct.unpack('<IIQ4I',uc.mem_read(c.reg(1),32))
   assert reserved==0
   self.record(service,subject,argument,words(x,y,z))
   if service==0:uc.mem_write(c.reg(2),words(self.remote_value)+bytes(12))
   elif service==1:uc.mem_write(c.reg(2),words(0)+self.point)
   self.finish(-1 if service==self.fail_service else 1)
  elif address==0x33dd10:
   self.record(0,c.reg(0)) # Complete original getter executes.
  elif address==0x3935dc:
   self.record(1,c.reg(0)) # Complete original getter executes.
  elif address in (0x3938f8,0x3a4d5c,0x3939f0,0x393cec):
   service={0x3938f8:2,0x3a4d5c:3,0x3939f0:4,0x393cec:5}[address]
   position=bytes(uc.mem_read(c.reg(1),12)) if service in (4,5) else bytes(12)
   if service==3:assert c.reg(1)==0x3f and c.reg(2)==0
   self.record(service,c.reg(0),0x3f if service==3 else 0,position)
   self.finish()
 def execute(self,raw,fail=-1,nullstate=False,nullservices=False,missing=False):
  c=self.c; vals=struct.unpack('<9I',raw[:36]); command,present,blocked,locked,forced,network,remote,cache,cached=vals
  self.point=raw[48:60] if cache and cached else raw[36:48]
  self.remote_value=1 if network!=0xffffffff else remote;self.calls=[];self.fail_service=fail
  if self.native:
   c.uc.mem_write(self.state,struct.pack('<QQ4I',self.controller,self.owner,blocked,locked,forced,0))
   c.uc.mem_write(self.services,struct.pack('<QQ',0,0 if missing else self.callback))
   result=c.invoke('dh2_character_controller_character',[0 if nullstate else self.state,command,self.target if present else 0,0 if nullservices else self.services])
  else:
   c.uc.mem_write(0x9a318b,bytes([blocked]));c.uc.mem_write(self.controller+8,bytes([locked,forced]))
   c.pointer(self.owner+0x110,network);c.uc.mem_write(self.owner+0x118,bytes([remote]))
   c.pointer(self.target+0x180,self.target+0x100 if cache else 0);c.uc.mem_write(self.target+0x80,bytes([cached]))
   c.uc.mem_write(self.target+0x160,raw[36:48]);c.uc.mem_write(self.target+0x184,raw[48:60])
   result=c.invoke((0x4052bc,0x405540,0x40559c)[command],[self.controller,self.target if present else 0])
  return result,tuple(self.calls)

def main():
 p=argparse.ArgumentParser()
 for key in ('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args(); ref=ROOT/'port/level-world/reference/character-controller-commands'
 manifest=json.loads((ref/'original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});records=[];requests=0
 rng=random.Random(0x405540)
 def compare(raw):
  nonlocal requests
  _,expected=old.execute(raw);result,actual=new.execute(raw)
  assert result==1 and actual==expected,(len(records),raw.hex(),expected,actual,result)
  records.append(raw+words(len(expected))+b''.join(expected));requests+=len(expected)
 for command,present,blocked,locked,forced,remote_mode,cache,cached in itertools.product(range(3),range(2),(0,1,255),(0,1,255),(0,1,255),range(3),range(2),range(2)):
  network,remote=((0xffffffff,0),(0xffffffff,255),(17,0))[remote_mode]
  raw=words(command,present,blocked,locked,forced,network,remote,cache,cached)+struct.pack('<6f',7.,-11.,13.,-17.,19.,-23.)
  compare(raw)
 for _ in range(512):
  raw=words(rng.randrange(3),rng.randrange(2),*(rng.randrange(256) for _ in range(3)),rng.choice((0xffffffff,rng.getrandbits(32))),rng.randrange(256),rng.randrange(2),rng.randrange(256))+words(*(rng.getrandbits(32) for _ in range(6)))
  compare(raw)
 guards=0;failure_checks=0
 base=words(1,1,0,0,0,0xffffffff,0,1,1)+struct.pack('<6f',7.,-11.,13.,-17.,19.,-23.)
 for kw in ({'nullstate':True},{'nullservices':True},{'missing':True}):
  result,calls=new.execute(base,**kw);assert result&0xffffffff==0xffffffff and not calls;guards+=1
 for command in (3,0xffffffff):
  raw=words(command)+base[4:];result,calls=new.execute(raw);assert result&0xffffffff==0xffffffff and not calls;guards+=1
 for command,services in ((0,(1,5)),(1,(0,1,4)),(2,(0,2,3))):
  raw=words(command)+base[4:];_,allcalls=new.execute(raw)
  for service in services:
   result,calls=new.execute(raw,fail=service);idx=next(i for i,v in enumerate(allcalls) if struct.unpack_from('<I',v)[0]==service)
   assert result&0xffffffff==0xffffffff and calls==allcalls[:idx+1];failure_checks+=1
 blob=b'CCD1'+words(len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 source_names=('character_controller_commands.hpp','character_controller_commands.cpp','tests/character_controller_commands_differential.py','tests/character_controller_commands.cpp')
 report={'validation':'PASS','original_sha256':sha(a.engine),'library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'comparisons':len(records),'ordered_service_requests':requests,'native_guard_checks':guards,'service_failure_prefix_checks':failure_checks,'mismatches':0,'source_sha256':{n:sha(ROOT/'port/level-world'/n) for n in source_names if (ROOT/'port/level-world'/n).exists()},'scope':__doc__,'character_point_override':'3addbc tail-branches to GameObject.LookAt(Point) 393cec; it is not empty. Point body remains an explicit service.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
