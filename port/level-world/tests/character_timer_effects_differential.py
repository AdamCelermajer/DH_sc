"""Actual regen/DoT expiry instructions, property Add/resolve, ordered explicit virtual/attack services versus O2 ARM64."""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def word(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def signed(x):return x if x<0x80000000 else x-0x100000000
def pack(xs):return struct.pack('<224i',*xs)
class Audit:
 def __init__(self,path,native,manifest):
  self.c=Cpu(path,native,manifest);self.native=native;c=self.c;d=c.data;self.character=d+0x1000;self.props=self.character+0x560;self.ai=d+0x9000;self.vt=d+0xa000;self.default=d+0xb000;self.svc=d+0x1e000;self.view=d+0x20000;self.state=d+0x21000;self.out=d+0x22000;self.services=d+0x23000;self.current=d+0x24000;self.sheet=[d+x for x in (0x30000,0x31000,0x32000,0x33000)]
  self.trace=[];self.adds=[];self.parameters=None;self.state_reads=0;self.deaths=0;self.results=[]
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',0,self.svc))
  else:
   c.pointer(self.character,self.vt);c.pointer(self.props+4,self.character);c.pointer(self.ai+4,self.character);c.pointer(self.vt+0x54,self.svc);c.pointer(self.vt+0x34,self.svc+4);c.uc.mem_write(self.svc,struct.pack('<2I',0xe12fff1e,0xe12fff1e));c.pointer(0x9a645c,self.default)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def finish(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def request(self,op,p=0,a=0,e=0,name=b'',result=None):
  a=signed(a&0xffffffff);e=signed(e&0xffffffff)
  self.trace.append((op,p,signed(a&0xffffffff),signed(e&0xffffffff),name.decode()))
  if op==0:return self.parameters['inhibited']
  if op==1:
   result=self.parameters['states'][min(self.state_reads,len(self.parameters['states'])-1)];self.state_reads+=1;return result
  if op==2:
   result=self.parameters['dead'][min(self.deaths,len(self.parameters['dead'])-1)];self.deaths+=1;return result
  if op==4 and self.parameters.get('mutation'):
   # Real debug callback may change the retained live HP/rate sheets.
   c=self.c;at=self.sheet[3] if self.native else self.props+0xa94+4;c.uc.mem_write(at+36*4,struct.pack('<i',-17));c.uc.mem_write(at+44*4,struct.pack('<i',513))
  if op==5:
   # Explicit complete-result fixture for unreconstructed application service
   # boundaries. No original damage acceptance is fabricated by this corpus.
   raw=struct.pack('<6i2I2i',a,-1,-1,-1,0,0,0,0x20080000,-1,e);self.c.uc.mem_write(result,raw)
  if op==6:
   self.results.append(bytes(self.c.uc.mem_read(result,40)))
   if self.parameters.get('dot_mutation'):
    at=self.sheet[3] if self.native else self.props+0xa94+4;self.c.uc.mem_write(at+131*4,struct.pack('<i',257))
  return 0
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.svc:return
   op,p,a,e,subject,target,name=struct.unpack('<4I3Q',uc.mem_read(c.reg(2),40));assert subject==self.character and target==(self.character if op in (5,6) else 0);value=self.request(op,p,a,e,string(c,name) if name else b'',c.reg(4));uc.mem_write(c.reg(3),struct.pack('<I',value&0xffffffff));self.finish();return
  if address==self.svc:self.finish(self.request(0));return
  if address==self.svc+4:self.finish(self.request(2));return
  if address==0x3c01ac:self.finish(self.request(1));return
  if address==0x337888:self.request(3);self.finish();return
  if address==0x337a88:self.request(4,name=string(c,word(c,c.reg(1)+0x14)));self.finish();return
  if address==0x3140ec:c.pointer(c.reg(0)+0x14,c.reg(1));self.finish(c.reg(0));return
  if address in (0x318254,0x3139ac):self.finish();return
  if address==0x3b2638:
   assert c.reg(1)==c.reg(2)==self.character and c.reg(3)==0x20080000;sp=c.uc.reg_read(c.sp);assert word(c,sp)==0xffffffff;e=word(c,sp+4);a=word(c,sp+8);self.request(5,127+signed(e),a,e,result=c.reg(0));self.finish();return
  if address==0x3b10b4:assert c.reg(1)==c.reg(2)==self.character and not c.reg(3);e=word(c,c.reg(0)+36);a=word(c,c.reg(0));self.request(6,127+signed(e),a,e,result=c.reg(0));self.finish();return
  if address==0x3e0708:self.adds.append((c.reg(1),signed(c.reg(2))))
 def put(self,defaults,types,sheets,parameters):
  c=self.c;self.trace=[];self.adds=[];self.results=[];self.state_reads=self.deaths=0;self.parameters=parameters
  if self.native:
   c.uc.mem_write(self.default,pack(defaults)+pack(types))
   for at,s in zip(self.sheet,sheets):c.uc.mem_write(at,pack(s))
   c.uc.mem_write(self.view,struct.pack('<7QII',self.default,self.default+896,*self.sheet,0,0,0));c.uc.mem_write(self.state,struct.pack('<4Q',self.character,self.view,parameters['aggro'],parameters['aggroed']))
  else:
   c.uc.mem_write(self.default,bytes(4)+pack(defaults)+bytes(4)+pack(types));c.pointer(self.ai+0x8c,parameters['aggro']);c.pointer(self.ai+0xa4,parameters['aggroed'])
   for offset,s in zip((8,0x38c,0x710,0xa94),sheets):c.uc.mem_write(self.props+offset,bytes(4)+pack(s))
   sentinel=self.props+0xe18;c.uc.mem_write(sentinel,struct.pack('<4I',0,0,sentinel,sentinel));c.pointer(self.props+0xe28,0)
 def snapshot(self):
  return b''.join(bytes(self.c.uc.mem_read(at,896)) for at in self.sheet) if self.native else b''.join(bytes(self.c.uc.mem_read(self.props+at+4,896)) for at in (8,0x38c,0x710,0xa94))
 def execute(self,event):
  if self.native:assert self.c.invoke('dh2_character_timer_effect',[self.out,self.state,event,self.services])==1
  else:self.c.invoke(0x3cb77c if event==0x33 else 0x3df3f0,[self.ai if event==0x33 else self.props])
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();ref=ROOT/'reference/character-timer-effects';manifest=json.loads((ref/'original-functions.json').read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so';old=Audit(engine,False,manifest);new=Audit(a.library,True,{'functions':[]});rng=random.Random(0xD03334);raw=(REPO/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));records=[];gold=bytearray(b'TEF1'+bytes(4));add_count=dot_count=0
 def case(event,sheets,params):
  nonlocal add_count,dot_count
  before=b''.join(pack(s) for s in sheets);old.put(defaults,types,sheets,params);new.put(defaults,types,sheets,params);old.execute(event);new.execute(event);expected=old.snapshot();assert new.snapshot()==expected and old.trace==new.trace and old.results==new.results,(len(records),event,old.trace,new.trace)
  out=struct.unpack('<6I',new.c.uc.mem_read(new.out,24));assert out[0]==8 and out[1]==len(old.trace) and out[2]==len(old.adds) and out[3]==len(old.results);assert list(zip((36,41),map(signed,out[4:])))==[(36,next((v for p,v in old.adds if p==36),0)),(41,next((v for p,v in old.adds if p==41),0))]
  record=dict(event=event,params=params,trace=old.trace,adds=old.adds,results=[x.hex() for x in old.results]);records.append(record);encoded=json.dumps(record,separators=(',',':')).encode();gold.extend(struct.pack('<II',event,len(encoded))+encoded+before+expected);add_count+=len(old.adds);dot_count+=len(old.results)
 for i in range(1200):
  sheets=[[rng.choice([-1,0,1,256,0x7fffffff,-0x80000000]) for _ in range(224)] for _ in range(4)]
  if i<600:
   for p in (36,41):sheets[0][p]=-1;sheets[1][p]=sheets[3][p]=rng.randrange(-256,32000)
   for p in (38,43):sheets[3][p]=rng.randrange(0,64000)
  for p in (39,40,44,45):sheets[3][p]=rng.choice((-1,0,1,256,1000,0x7fffffff,-0x80000000))
  case(0x33,sheets,dict(inhibited=255 if i%17==0 else 0,aggro=int(i%9==0),aggroed=int(i%13==0),states=rng.choice([[3],[5],[6],[7],[-1],[4,5,7],[5,6,7],[6,7,3]]),dead=[0],mutation=int(i%23==0)))
 for i in range(256):
  sheets=[defaults.copy() for _ in range(4)]
  for j in range(6):sheets[3][126+j]=rng.choice([-1,0,1,256,0x7fffffff,-0x80000000])
  case(0x34,sheets,dict(inhibited=0,aggro=0,aggroed=0,states=[3],dead=rng.choice([[0],[1],[0,1,0],[1,0,1]]),dot_mutation=int(i%7==0)))
 resets=bytearray(b'TER1'+struct.pack('<I',32));remove_cases=0
 for i in range(32):
  d=defaults if i<16 else [rng.randrange(-2147483648,2147483648) for _ in range(224)];sheets=[[rng.randrange(-2147483648,2147483648) for _ in range(224)] for _ in range(4)];params=dict(inhibited=0,aggro=0,aggroed=0,states=[3],dead=[0]);old.put(d,types,sheets,params);new.put(d,types,sheets,params);before=old.snapshot()
  for element in (-2147483648,-1,0,4,5,2147483647):
   old.c.invoke(0x3de83c,[old.props,element]);assert new.c.invoke('dh2_character_dot_remove',[new.view,element])==1 and old.snapshot()==new.snapshot()==before;remove_cases+=1
  old.c.invoke(0x3def34,[old.props,old.props+0xa94]);assert new.c.invoke('dh2_character_cached_reset',[new.view])==1 and old.snapshot()==new.snapshot();resets.extend(pack(d)+before+old.snapshot())
 (ref/'cached-reset-fixtures.bin').write_bytes(resets)
 queries=bytearray(b'TEQ1'+struct.pack('<I',72));query_cases=0
 for network_id in (-2147483648,-1,0,2147483647):
  for remote in (0,1,255):
   for dead in (0,1,255):
    old.c.uc.mem_write(old.character+0x110,struct.pack('<i',network_id));old.c.uc.mem_write(old.character+0x118,bytes([remote]));old.c.uc.mem_write(old.character+0x1449,bytes([dead]));new.c.uc.mem_write(new.current,struct.pack('<iBBH',network_id,remote,dead,0))
    for kind in (0,1):
     expected=old.c.invoke(0x33dd10 if not kind else 0x3a2ed4,[old.character]);assert new.c.invoke('dh2_character_timer_owner_query',[new.out,new.current,kind])==1 and word(new.c,new.out)==expected;queries.extend(struct.pack('<iBBHII',network_id,remote,dead,0,kind,expected));query_cases+=1
 (ref/'owner-query-fixtures.bin').write_bytes(queries)
 struct.pack_into('<I',gold,4,len(records));(ref/'timer-effects-fixtures.bin').write_bytes(gold);(ref/'original-probe.json').write_text(json.dumps(dict(validation='PASS',rows=records),indent=2)+'\n');report=dict(validation='PASS',scope=__doc__,comparisons=len(records),regen_cases=1200,dot_cases=256,positive_regen_adds=add_count,explicit_dot_applications=dot_count,cached_reset_cases=32,actual_RemoveDot_noop_cases=remove_cases,owner_query_cases=query_cases,mismatches=0,original_property_get_add_resolver_unmocked=True,original_virtual_debug_attack_services_explicit=True,full_buff_registry_owned=False,original_sha256=sha(engine),library_sha256=sha(a.library),gold_sha256=sha(ref/'timer-effects-fixtures.bin'),reset_gold_sha256=sha(ref/'cached-reset-fixtures.bin'),query_gold_sha256=sha(ref/'owner-query-fixtures.bin'),original_manifest_sha256=sha(ref/'original-functions.json'),source_sha256={str((ROOT/x).relative_to(REPO)):sha(ROOT/x) for x in ['character_timer_effects.hpp','character_timer_effects.cpp','tests/character_timer_effects_differential.py','tools/build_character_timer_effects_oracle.ps1']},packaged_APK=False);(ROOT/'reports/character-timer-effects-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','comparisons','positive_regen_adds','explicit_dot_applications']}))
if __name__=='__main__':main()
