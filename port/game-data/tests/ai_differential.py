"""Original character geometry, faction and target events vs native ARM64.

Virtual actor/RTTI, inventory reach, FSM queries and event callbacks are supplied
fixtures. Original geometric arithmetic, faction traversal and UpdateTarget
instructions execute. The discarded melee debug query is skipped explicitly.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from aggro_differential import Cpu as Base,float_bits
from combat_result_differential import bits,floating
ROOT=Path(__file__).resolve().parents[1]

class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__aeabi_fmul':
   self.put(0,float_bits(floating(self.reg(0))*floating(self.reg(1))));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  elif address in (self.callback+32,self.callback+48,self.callback+64):
   value=self.targetable if address==self.callback+32 else (self.owner_player if self.reg(0)==self.owner else self.target_player) if address==self.callback+48 else self.can_range
   self.put(0,value);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--data',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/ai-target/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261006)
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def put(at,value):old.uc.mem_write(at,struct.pack('<I',value&0xffffffff))
 def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 owner=old.owner=old.data+0x1000;target=old.data+0x4000;ai=owner+0x3c8;vt=old.data+0x8000;props=old.data+0x9000;opos=props+0x100;tpos=opos+0x10;rows=old.data+0x10000
 for c in (owner,target):old.pointer(c,vt);old.pointer(c+0x3cc,c)
 old.pointer(vt+0x28,old.callback+48);old.pointer(vt+0x34,0x3a2ed4);old.pointer(vt+0x88,old.callback+32);old.pointer(vt+0x90,old.callback+32);old.pointer(vt+0x124,old.callback+64)
 got=(0x3d5768+word(0x3d5a54))&0xffffffff;old.pointer(word(got+word(0x3d5a60)),rows);put(word(got+word(0x3d5a58)),16);put(word(got+word(0x3d5a5c)),0)
 # Faction getter's actual range check uses the original global count.
 g=(0x3a3194+word(0x3a31b0))&0xffffffff;put(word(g+word(0x3a31b4)),16)
 # AI getter is an explicit configuration snapshot in this suite.
 mode='range';radii=[200.,200.];state=3;facts=0;events=[];skipped=0;geometry=[];faction_records=[];target_records=[];last_distance=0
 def hook(uc,address,size,unused):
  nonlocal skipped,last_distance
  if address==0x33dd70:old.pointer(old.reg(0),old.reg(1));ret()
  elif address==0x33ff8c:ret(word(old.reg(0)))
  elif address==0x3935dc:ret(opos if old.reg(0)==owner else tpos)
  elif address==0x3a3024:ret(props)
  elif address==0x3a2fec:ret(40)
  elif address==0x3d4c34:ret(bits(radii[0] if old.reg(0)==ai else radii[1]))
  elif address==0x3d62b0:last_distance=old.reg(0)
  elif address==0x3d62dc:skipped+=1;old.uc.reg_write(old.pc,0x3d6340)
  elif mode=='target' and address==0x3c0230:ret(int(state==17))
  elif mode=='target' and address==0x3c01c0:ret(int(state==0))
  elif mode=='target' and address in (0x3d4ed8,0x3d6188,0x3d63d8,0x3d6604):ret(int(bool(facts&{0x3d4ed8:8,0x3d6188:128,0x3d63d8:32,0x3d6604:64}[address])))
  elif mode=='target' and address==0x3a4d5c:
   event=old.reg(1);arg=old.reg(2);events.append((event,int(arg==0)))
   if event==10 and facts&256 or event==12 and facts&512:old.pointer(ai+0x40,0);old.pointer(ai+0x44,0)
   ret()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 request=new.data+0x1000;out=new.data+0x1100;storage=new.data+0x2000
 old.owner_player=old.target_player=old.can_range=0;old.targetable=8
 for i in range(1800):
  # Near thresholds, nonzero Z, signed radii and nonfinite IEEE inputs.
  values=[rng.uniform(-25000,25000) for _ in range(6)]+[rng.choice((120.,150.,200.,-200.)),rng.choice((120.,200.,-120.)),rng.choice((1500.,2400.,-1500.))]
  if i<300:
   reach=values[6]+values[7];values[:6]=[0.,0.,0.,reach,0.,0.];values[3]=floating((bits(reach)+(i%3-1))&0xffffffff)
  if i>=1500:values[rng.randrange(9)]=floating(rng.choice((0,0x80000000,0x7fc01234,0x7f800000,0xff800000,0x7f7fffff,0xff7fffff,1)))
  packed=struct.pack('<9f',*values);old.uc.mem_write(opos,packed[:12]);old.uc.mem_write(tpos,packed[12:24]);old.uc.mem_write(props+0x3c,packed[32:36]);radii=values[6:8];old.pointer(ai+0x40,target)
  melee=old.invoke(0x3d6188,[ai,target]);sight=old.invoke(0x3d4ed8,[ai,target]);new.uc.mem_write(request,packed);assert new.invoke('dh2_ai_range',[out,request])==0;actual=struct.unpack('<3I',new.uc.mem_read(out,12))
  distance=last_distance
  assert actual[1:]==(melee,sight),(i,values,actual,melee,sight)
  assert actual[0]==distance or math.isnan(floating(actual[0])) and math.isnan(floating(distance)),(i,'distance',actual[0],distance)
  geometry.append(packed+struct.pack('<3I',*actual))
 # Every original relationship, every pair and each player classification.
 data=(a.data/'ai_factions_pyarray.bin').read_bytes();count=struct.unpack_from('<I',data)[0];assert count==16;at=4;factions=[]
 for index in range(count):
  n=struct.unpack_from('<I',data,at)[0];at+=4;row=[struct.unpack_from('<ii',data,at+8*j) for j in range(n)];at+=8*n;factions.append(row);entries=rows+0x1000+index*0x400;old.uc.mem_write(rows+12*index,struct.pack('<3I',0,n,entries))
  for j,(id,value) in enumerate(row):old.uc.mem_write(entries+12*j,struct.pack('<Iii',0,id,value))
 assert at==len(data);mode='enemy';old.targetable=1
 for oi in range(16):
  for ti in range(16):
   for players in range(4):
    old.owner_player=players&1;old.target_player=players>>1;put(owner+0xff8,oi);put(target+0xff8,ti);row=factions[oi];packed=b''.join(struct.pack('<ii',*x) for x in row);new.uc.mem_write(storage,packed)
    expected=old.invoke(0x3d574c,[ai,target]);actual=new.invoke('dh2_ai_enemy',[storage,len(row),ti,old.owner_player,old.target_player]);assert expected==actual,(oi,ti,players,expected,actual);faction_records.append(struct.pack('<4I',oi,ti,players,actual))
 mode='target';old.owner_player=old.target_player=0
 for state in (0,3,5,13,17):
  for facts in range(1024):
   for previous in range(4):
    old.targetable=int(bool(facts&2));old.can_range=int(bool(facts&16));old.pointer(ai+0x40,target if facts&1 else 0);old.pointer(ai+0x44,target if facts&1 else 0);old.uc.mem_write(target+0x1449,bytes((int(not bool(facts&4)),)));old.uc.mem_write(ai+0x48,bytes((previous&1,previous>>1)));events.clear()
    old.invoke(0x3cb908,[ai]);expected=[int(word(ai+0x40)!=0),int(word(ai+0x44)!=0),*old.uc.mem_read(ai+0x48,2),len(events),*[e[0] for e in events],*[0]*(3-len(events)),sum(e[1]<<i for i,e in enumerate(events))]
    packed=struct.pack('<i3I',state,facts,previous&1,previous>>1);new.uc.mem_write(request,packed);assert new.invoke('dh2_ai_target_update',[out,request])==0;actual=list(struct.unpack('<9I',new.uc.mem_read(out,36)));assert actual==expected,(state,facts,previous,expected,actual);target_records.append(packed+struct.pack('<9I',*actual))
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'geometry_cases':len(geometry),'faction_cases':len(faction_records),'target_event_cases':len(target_records),'comparisons':len(geometry)+len(faction_records)+len(target_records),'mismatches':0,'discarded_melee_debug_queries_skipped':skipped,'scope':__doc__,'original_import_calls':old.import_calls,'elapsed_seconds':round(time.monotonic()-started,2)}
 reference=struct.pack('<3I',len(geometry),len(faction_records),len(target_records))+b''.join(geometry+faction_records+target_records);a.reference_output.write_bytes(reference);report['reference_sha256']=hashlib.sha256(reference).hexdigest();a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
