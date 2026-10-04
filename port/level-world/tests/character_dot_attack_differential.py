"""Actual self/self DoT calculation and offline ApplyResult control flow.

Profile/debug/virtual/aggro/HitFor/regen/sneak/text/sound are explicit fixture
services. CalculateResult, SetCombatants, direct damage and cached getters run
original instructions. No F_ApplyResult blocks are skipped in supported cases.
Unsupported branch probes compare only the reached original prefix.
"""
import argparse,hashlib,json,math,random,struct,sys,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def w(c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def signed(x):return x if x<0x80000000 else x-0x100000000
def words(*xs):return struct.pack('<'+'I'*len(xs),*(x&0xffffffff for x in xs))
def packed(sheet):return struct.pack('<224i',*sheet)
def fbits(v):return struct.unpack('<I',struct.pack('<f',v))[0]

class Audit:
 def __init__(self,path,native,manifest,defaults,types):
  self.native=native;self.c=c=Cpu(path,native,manifest or {'functions':[]});d=c.data
  self.actor=d+0x1000;self.vt=d+0x5000;self.service=d+0x9000;self.result=d+0xa000;self.out=d+0xb000;self.context=d+0xc000;self.view=d+0xd000;self.bind=d+0xe000
  self.defaults=d+0x10000;self.types=d+0x11000;self.sheets=[d+x for x in (0x12000,0x14000,0x16000,0x18000)];self.game=d+0x30000;self.online=d+0x32000
  self.trace=[];self.config={};self.dead_calls=0;self.player_calls=0;self.threat=0;self.hit=0;self.prefix=False;self.kernels={}
  if native:
   c.uc.mem_write(self.bind,struct.pack('<QQ',0,self.service));c.uc.mem_write(self.view,struct.pack('<7QII',self.defaults,self.types,*self.sheets,0,0,0));c.uc.mem_write(self.defaults,packed(defaults));c.uc.mem_write(self.types,packed(types))
  else:
   c.uc.mem_write(self.actor,bytes(0x2000));c.pointer(self.actor,self.vt);c.pointer(self.vt+0x28,self.service);c.pointer(self.vt+0x34,self.service+4);c.uc.mem_write(self.service,words(0xe12fff1e,0xe12fff1e));c.pointer(self.actor+0x564,self.actor)
   self.sheets=[self.actor+0x560+x+4 for x in (8,0x38c,0x710,0xa94)]
   c.pointer(0x9a645c,self.defaults);c.uc.mem_write(self.defaults,bytes(4)+packed(defaults));c.uc.mem_write(self.types,bytes(4)+packed(types))
   # Defaults/types are adjacent source objects, each with its 4-byte vptr.
   c.uc.mem_write(self.defaults+900,bytes(4)+packed(types))
   got=(0x3a8bdc+w(c,0x3a921c))&0xffffffff;c.pointer(w(c,got+w(c,0x3a9224))+0x40,self.game)
   got=(0x7fd758+w(c,0x7fd78c))&0xffffffff;c.pointer(w(c,got+w(c,0x7fd790)),self.online)
   self.original_context=(0x3b05ac+8+w(c,0x3b0618))&0xffffffff
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def finish(self,v=0):c=self.c;c.put(0,v&0xffffffff);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def boundary(self):
  self.prefix=True;self.c.uc.reg_write(self.c.sp,self.c.stack+0xe000);self.c.uc.reg_write(self.c.pc,self.c.stop)
 def event(self,op,amount=0,name='',threat=0):
  self.trace.append([op,signed(amount&0xffffffff),name,threat]);x=self.config
  if op==1:return x['debug'].get(name,0),0
  if op==4:return x['online'],0
  if op==5:return x['app_god'],0
  if op==6:return x['party'],0
  if op==7:
   v=x['players'][min(self.player_calls,len(x['players'])-1)];self.player_calls+=1;return v,0
  if op==8:
   self.threat=threat
   if x.get('mutate_aggro'):
    self.c.uc.mem_write(self.sheets[3]+204*4,words(-17))
    if self.native:self.c.uc.mem_write(self.actor+16,words(x['mutate_aggro']))
    else:self.c.uc.mem_write(self.actor+0x110,words(x['mutate_aggro']))
   return 0,x['aggro_return']
  if op==9:self.hit=1
  if op==10:
   v=x['dead'][min(self.dead_calls,len(x['dead'])-1)];self.dead_calls+=1;return v,0
  return 0,0
 def hook(self,uc,address,size,unused):
  c=self.c
  if self.native:
   if address!=self.service:return
   op,amount,subject,target,name,threat,reserved=struct.unpack('<IiQQQII',uc.mem_read(c.reg(2),40));assert subject==self.actor and target==(self.actor if op in (8,9,14,15) else 0) and reserved==0
   value,num=self.event(op,amount,string(c,name).decode() if name else '',threat);uc.mem_write(c.reg(3),words(value,num));self.finish();return
  if address in (0x3b2e68,0x3b2638,0x3af3b0,0x3b059c,0x3b1fb8,0x3bd394,0x3dedb4):self.kernels[hex(address)]=self.kernels.get(hex(address),0)+1
  if address==0x7fd794:self.event(4);return
  if address==0x3b1440 and self.config['online']:self.boundary();return
  if address==0x3b1574:self.event(6);return
  if address==0x3b1580 and self.config['party']>1:self.boundary();return
  if address in (0x3b1a2c,0x3b17ec,0x3b1758,0x3b13e0) and self.trace and self.trace[-1][0]==7 and self.config['players'][min(self.player_calls-1,len(self.config['players'])-1)]:self.boundary();return
  if address==0x3140ec:c.pointer(c.reg(0),c.reg(1));self.finish(c.reg(0));return
  if address==0x3139ac:self.finish();return
  service={0x337888:0,0x337a88:1,0x3136b4:2,0x3136b8:3,0x320e14:5,0x3d7c68:8,0x3a8bc4:9,0x3bdca4:11,0x3bdbb8:12,0x3bc6b8:13,0x3af77c:14,0x3afee0:15,self.service:7,self.service+4:10}.get(address)
  if service is None:return
  amount=c.reg(1) if service in (9,11,12) else 0
  name=string(c,w(c,c.reg(1))).decode() if service==1 else string(c,c.reg(1)).decode() if service==5 else string(c,c.reg(0)).decode() if service in (2,3) else ''
  value,num=self.event(service,amount,name,c.reg(2) if service==8 else 0)
  self.finish(num if service==8 else value)
 def fixture(self,sheet,x):
  c=self.c;self.config=x;self.trace=[];self.dead_calls=self.player_calls=self.threat=self.hit=0;self.prefix=False
  for p in self.sheets:c.uc.mem_write(p,packed(sheet))
  c.uc.mem_write(self.result,b'\xcc'*40);c.uc.mem_write(self.out,b'\xdd'*24);c.uc.mem_write(self.context,b'\xee'*32)
  if self.native:c.uc.mem_write(self.actor,struct.pack('<QQiHBBQ',self.actor,self.view,x['network'],x['combo'],x['push'],x['god'],0))
  else:
   c.uc.mem_write(self.actor+0x110,words(x['network']));c.uc.mem_write(self.actor+0x14d0,struct.pack('<H',x['combo']));c.uc.mem_write(self.actor+0x53b,bytes((x['push'],)));c.uc.mem_write(self.actor+0x14f0,bytes((x['god'],)));c.uc.mem_write(self.online+5,bytes((x['online'],)));c.uc.mem_write(self.game+0x6c4,words(x['party']))
 def state(self):
  c=self.c
  return list(struct.unpack('<iHBB',c.uc.mem_read(self.actor+16,8))) if self.native else [signed(w(c,self.actor+0x110)),struct.unpack('<H',c.uc.mem_read(self.actor+0x14d0,2))[0],c.uc.mem_read(self.actor+0x53b,1)[0],c.uc.mem_read(self.actor+0x14f0,1)[0]]
 def combat_context(self):
  c=self.c
  if self.native:
   a,b,delta,reverse,element,*flags=struct.unpack('<QQiii4B',c.uc.mem_read(self.context,32));assert a==b==self.actor
  else:
   a,b,delta,reverse,element,*flags=struct.unpack('<IIiii4B',c.uc.mem_read(self.original_context+0x1c,24));assert a==b==self.actor
  return [delta,reverse,element,*flags]

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=REPO/'.local-inputs/libDungeonHunter2.so');p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-dot-attack-discovery/oracle.so');p.add_argument('--cases',type=int,default=1800);a=p.parse_args();start=time.monotonic();ref=ROOT/'reference/character-dot-attack';ref.mkdir(parents=True,exist_ok=True)
 manifests=[json.loads((REPO/'.local-inputs/character-dot-attack-discovery'/path/'original-functions.json').read_text()) for path in (Path('.'),Path('dependencies'),Path('source-context'))];functions={r['elf_address']:r for m in manifests for r in m['functions']};manifest={'original_sha256':sha(a.engine),'functions':list(functions.values())};assert manifest['original_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 (ref/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n');(ref/'original-functions.asm').write_text('\n'.join((REPO/'.local-inputs/character-dot-attack-discovery'/path/'reference/original-functions.asm').read_text() for path in (Path('.'),Path('dependencies'),Path('source-context'))))
 raw=(REPO/'port/android-native/app/src/main/assets/data/character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224i',raw,4));types=list(struct.unpack_from('<224i',raw,900));old=Audit(a.engine,False,manifest,defaults,types);new=Audit(a.library,True,{},defaults,types);rng=random.Random(20261004);records=[];requests=0;prefixes=0
 for i in range(a.cases):
  sheet=list(defaults);sheet[19]=rng.randrange(-2147483648,2147483648);sheet[204]=rng.randrange(-2147483648,2147483648)
  amount=rng.choice([-2147483648,-1,0,1,255,256,2147483647,rng.randrange(-2147483648,2147483648)]);element=rng.choice([-2147483648,-1,0,4,5,2147483647]);x=dict(network=rng.choice([-1,-1,-1,0,77]),combo=rng.randrange(65536),push=rng.randrange(256),god=rng.choice([0,0,0,rng.randrange(1,256)]),debug={'NoDamages':rng.choice([0,0,1]),'GOD':rng.choice([0,0,1]),'isTracingThreatChange':rng.randrange(2),'isTracingChar_Attack':rng.randrange(2)},app_god=rng.choice([0,0,1]),party=rng.choice([-1,0,1]),online=0,players=[0],dead=[rng.randrange(2),rng.randrange(2)],aggro_return=rng.choice([0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345]))
  if i%7==0:x['mutate_aggro']=rng.choice([-1,17])
  if i%19==0:x['online']=1
  elif i%23==0:x['party']=2
  elif i%29==0:x['players']=[1]
  old.fixture(sheet,x);new.fixture(sheet,x)
  wrapper=i%2==0
  old.c.invoke(0x3b2e68,[old.result,old.actor,old.actor,amount,element]) if wrapper else old.c.invoke(0x3b2638,[old.result,old.actor,old.actor,0x20080000,-1,element,amount])
  assert signed(new.c.invoke('dh2_character_dot_calculate' if wrapper else 'dh2_character_dot_calculate_result',[new.out,new.result,new.context,new.actor,amount,element,new.bind]))==1
  assert old.trace==new.trace,(i,'calculate trace',old.trace,new.trace);expected=bytes(old.c.uc.mem_read(old.result,40));assert expected==bytes(new.c.uc.mem_read(new.result,40)),(i,'calculation');assert old.combat_context()==new.combat_context(),(i,'context')
  calc_trace=old.trace;old.trace=[];new.trace=[];old.player_calls=new.player_calls=0;old.c.invoke(0x3b10b4,[old.result,old.actor,old.actor,0]);status=signed(new.c.invoke('dh2_character_dot_apply',[new.out,new.result,new.actor,new.bind]));assert status==(-3 if old.prefix else 1),(i,'status',status,old.prefix,old.trace,new.trace,x)
  assert old.trace==new.trace,(i,'apply trace',old.trace,new.trace,x);assert old.state()==new.state(),(i,'state',old.state(),new.state());assert expected==bytes(new.c.uc.mem_read(new.result,40))==bytes(old.c.uc.mem_read(old.result,40)),(i,'result retained');assert old.threat==new.threat,(i,'threat');assert all(bytes(old.c.uc.mem_read(o,896))==bytes(new.c.uc.mem_read(n,896)) for o,n in zip(old.sheets,new.sheets)),(i,'live sheets')
  output=list(struct.unpack('<4IIi',new.c.uc.mem_read(new.out,24)));assert output==[old.trace[-1][0]+1,len(old.trace),old.hit,0,old.threat,status],(i,'output',output,old.trace)
  records.append(dict(wrapper=wrapper,sheet=sheet,amount=amount,element=element,config=x,calculation=expected.hex(),context=old.combat_context(),calculate_trace=calc_trace,apply_trace=old.trace,state=old.state(),output=output,final_sheets=[bytes(new.c.uc.mem_read(n,896)).hex() for n in new.sheets],original_prefix_only=old.prefix));requests+=len(calc_trace)+len(old.trace);prefixes+=old.prefix
 guards=0
 for kind in range(8):
  new.fixture(defaults,x);new.c.uc.mem_write(new.result,expected);call='dh2_character_dot_apply';args=[new.out,new.result,new.actor,new.bind]
  if kind==0:args[1]=0
  elif kind==1:args[0]=new.result
  elif kind==2:args[2]=new.actor+1
  elif kind==3:args[3]=0
  elif kind==4:new.c.uc.mem_write(new.actor+24,words(1,0))
  elif kind==5:new.c.uc.mem_write(new.result+24,words(1))
  elif kind==6:call='dh2_character_dot_calculate';args=[new.out,new.result,new.actor,new.actor,amount,element,new.bind]
  elif kind==7:args[0]=new.sheets[3]
  regions=[(new.out,24),(new.result,40),(new.context,32),(new.actor,32)]+[(v,896) for v in new.sheets];before=[bytes(new.c.uc.mem_read(p,n)) for p,n in regions]
  assert signed(new.c.invoke(call,args))==-1 and not new.trace and before==[bytes(new.c.uc.mem_read(p,n)) for p,n in regions],('atomic',kind);guards+=1
 gold=ref/'dot-fixtures.json';gold.write_text(json.dumps(dict(defaults=defaults,types=types,records=records),separators=(',',':'))+'\n');build=json.loads(Path(str(a.library)+'.build.json').read_text(encoding='utf-8-sig'));sources=dict(build['source_sha256']);sources[str(Path(__file__).resolve().relative_to(REPO))]=sha(Path(__file__))
 service_counts={str(op):sum(row[0]==op for r in records for row in r['calculate_trace']+r['apply_trace']) for op in range(16)}
 report=dict(validation='PASS',scope=__doc__,original_sha256=sha(a.engine),arm64_library_sha256=sha(a.library),source_sha256=sources,comparisons=len(records),calculation_wrapper_comparisons=sum(r['wrapper'] for r in records),calculate_result_comparisons=sum(not r['wrapper'] for r in records),ordered_requests=requests,service_counts=service_counts,unsupported_original_prefixes=prefixes,atomic_guards=guards,mismatches=0,gold_sha256=sha(gold),original_kernel_entry_counts=old.kernels,original_import_calls=old.c.import_calls,native_import_calls=new.c.import_calls,full_HitFor_or_FX_backend=False,packaged_APK=False,elapsed_seconds=round(time.monotonic()-start,2));out=ROOT/'reports/character-dot-attack-arm64-differential.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','original_import_calls','native_import_calls')}))
if __name__=='__main__':main()

