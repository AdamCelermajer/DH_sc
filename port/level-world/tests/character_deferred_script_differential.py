"""Complete original load/Init wrappers and live AIS virtuals vs O2 ARM64.

The private allocation, Lua, skills, vitals and timer bodies are synchronous
explicit services. Original stores/reloads, pending Init and active Post/Final
execute; this is not an original full Character.Update or session-VM proof.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from character_script_lifecycle_differential import Machine,ENTRY,words
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class DeferredMachine(Machine):
 def execute(self,op,raw,params,arg):
  self.put_state(raw);self.params=params;self.calls=[];self.entry=ENTRY[op];self.first=True;self.nested=False
  result=self.c.invoke('dh2_character_deferred_script' if self.native else ENTRY[op],[self.s,op,arg,self.services] if self.native else [self.s,arg])
  return (result&0xffffffff if self.native or op==4 else 1),self.snapshot(),tuple(self.calls)
 def hook(self,uc,address,size,unused):
  trigger=self.params[2]==3 and not self.nested and ((self.native and address==self.svc and struct.unpack('<I',uc.mem_read(self.c.reg(2),4))[0]==12) or (not self.native and address==0x3b3a70)) if hasattr(self,'params') else False
  super().hook(uc,address,size,unused)
  if self.native and address==self.svc and self.c.uc.reg_read(self.c.pc)==self.c.symbols['dh2_character_script_lifecycle']:
   self.c.uc.reg_write(self.c.pc,self.c.symbols['dh2_deferred_fixture_virtual'])
  if trigger:
   self.nested=True
   if self.native:
    for r,value in enumerate((self.s,4,1,self.services)):self.c.put(r,value)
    self.c.uc.reg_write(self.c.pc,self.c.symbols['dh2_character_deferred_script'])
   else:
    self.c.put(0,self.s);self.c.put(1,1);self.c.uc.reg_write(self.c.pc,0x3cf3a4)
def main():
 p=argparse.ArgumentParser()
 for key in ('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 a=p.parse_args();manifest=json.loads((ROOT/'port/level-world/reference/character-script-lifecycle/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 old=DeferredMachine(a.engine,False,manifest);new=DeferredMachine(a.library,True,{'functions':[]});records=[];requests=0;reentries=0;rng=random.Random(0x3cf3a4)
 def state(active=0,pending=3,step=0,delayed=1,scripted=1,name=4):return struct.pack('<4Q',1,active,pending,name)+words(step,-1,-1,delayed,scripted,0,0,0)
 def compare(op,s,params,final):
  nonlocal requests,reentries
  try:expected=old.execute(op,s,params,final);actual=new.execute(op,s,params,final)
  except Exception:
   print('case',len(records),'operation',op,'params',params,'original_pc',hex(old.c.uc.reg_read(old.c.pc)));raise
  assert expected==actual,(len(records),op,params,final,expected,actual)
  result,after,calls=expected;requests+=len(calls);reentries+=int(old.nested)
  records.append(words(op,final)+s+words(*params)+words(result)+after+words(len(calls))+b''.join(calls))
 for op in (1,3,4,8,9):
  for i in range(160):
   step=rng.choice((0,1,2,3,4,5,6,7));pending=3;active=rng.choice((0,2));params=[rng.choice((0,1,255)),rng.choice((0,1,2,7)),rng.choice((0,1)),10,20,0,0,1]
   compare(op,state(active,pending,step,rng.choice((0,1,255)),rng.choice((0,1,255)),rng.choice((0,4))),params,rng.randrange(2))
 # Mutation callbacks run inside the actual source pending/active wrappers.
 for op in (1,3,4,8,9):
  for service in (3,4,10,12,13,14,15,16,21,22,23):
   for mutation in (1,2,3,4,6,8,9):
    if op in (1,4) and mutation==2:continue # A destroyed pending receiver is not a source-valid borrowed virtual.
    compare(op,state(0 if op in (1,4) else 2),[1,7,0,10,20,mutation,service,1],1)
 # Callback synchronously reenters LoadNInit at the vitals boundary. Publication
 # has already occurred, so the nested source active guard makes no callbacks.
 for final in (0,1):
  compare(4,state(),[1,7,3,10,20,0,0,1],final)
 blob=b'ASL1'+words(len(records))+b''.join(records);a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(blob)
 source=['port/level-world/character_deferred_script.hpp','port/level-world/character_deferred_script.cpp','port/level-world/character_script_lifecycle.hpp','port/level-world/character_script_lifecycle.cpp','port/level-world/tests/character_deferred_script_differential.py']
 report={'validation':'PASS','original_sha256':sha(a.engine),'library_sha256':sha(a.library),'corpus_sha256':sha(a.gold),'comparisons':len(records),'ordered_service_requests':requests,'nested_LoadNInit_active_guard_cases':reentries,'mismatches':0,'source_sha256':{s:sha(ROOT/s) for s in source},'original_capture_sha256':sha(ROOT/'port/level-world/reference/character-script-lifecycle/original-functions.json'),'scope':'Actual LoadProcess/LoadNInit/InitProcess/OnInitPost/Final instructions, pending OnInit before publication, active Post/Final after; explicit allocation/Lua/vitals/skills/timer services. Session VM composition is a separate host proof; no full Character.Update or package claim.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
