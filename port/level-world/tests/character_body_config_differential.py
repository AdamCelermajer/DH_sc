"""Original Character InitPhysicalObject/physical constructors/attach versus ARM64 definition producer.
Body/shape creation, mass, PF updates and destruction are explicit observed service fixtures.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from body_transform_differential import Cpu as Base,equal
from aggro_differential import float_bits
from combat_result_differential import floating
ROOT=Path(__file__).resolve().parents[1]
def config_equal(expected,actual):
 # Integer group indices -1/-2/-4 have NaN-looking bits. They must remain
 # exact integers; only actual arithmetic float fields permit NaN-class parity.
 return len(expected)==len(actual)==208 and all(expected[x:y]==actual[x:y] for x,y in ((0,8),(44,80),(136,152),(156,208))) and all(equal(expected[x:y],actual[x:y]) for x,y in ((8,44),(80,136),(152,156)))
class Cpu(Base):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='__aeabi_fcmplt':self.put(0,int(floating(self.reg(0))<floating(self.reg(1))));self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:return super().external(uc,address,size,unused)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=3000);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/character-body-config/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261014)
 owner=old.data+0x1000;physical=old.data+0x4000;previous=old.data+0x5000;body=old.data+0x6000;shape=old.data+0x7000;ai=old.data+0x8000;vt=old.data+0x9000;world=old.data+0xa000;request=new.data+0x1000;output=new.data+0x2000
 nowner=0x1000000012345678;nphysical=0x200000009abcdef0;nprevious=0x3000000012345678
 events=[];body_definition=None;shape_definition=None;original_radius=bytes(4);is_player=0;override=0;disabled=0;records=[];cases=[];create_count=0;pin_count=0;pf_count=0;allocations=0
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def returned(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,address,size,unused):
  nonlocal body_definition,shape_definition,create_count,pin_count,pf_count,allocations
  if address==0x3a3024:returned(ai)
  elif address in (0x337888,0x3140ec,0x3139ac,0x31167c):returned()
  elif address==0x337a88:returned(override if uc.reg_read(old.lr)==0x46f3bc else disabled)
  elif address==0x310570:
   assert old.reg(0)==40 and old.reg(1)==0;events.append(1);allocations+=1;returned(physical)
  elif address==0x3b41fc:assert old.reg(0)==owner;returned(is_player)
  elif address==0x34bcf0:
   raw=bytes(uc.mem_read(old.reg(1),44));assert struct.unpack_from('<I',raw,16)[0]==physical
   body_definition=struct.pack('<Q',nphysical)+raw[:16]+raw[20:40]+struct.pack('<5I',*raw[40:44],0)
   assert len(body_definition)==64;events.append(2);create_count+=1;old.uc.mem_write(body,bytes(0x100));old.uc.mem_write(body+0x1c,struct.pack('<2f',1.25,-3.5));returned(body)
  elif address==0x7e1dc8:
   assert old.reg(0)==body;raw=bytes(uc.mem_read(old.reg(1),100));kind=struct.unpack_from('<I',raw,4)[0];assert kind in (0,1) and struct.unpack_from('<I',raw,8)[0]==physical
   floats=raw[12:24]+(raw[32:44]+bytes(32) if kind==0 else bytes(12)+raw[32:64])
   vertex_count=0 if kind==0 else struct.unpack_from('<I',raw,96)[0]
   group=struct.unpack_from('<h',raw,30)[0];category,mask=struct.unpack_from('<2H',raw,26)
   shape_definition=struct.pack('<QII',nphysical,kind,raw[24])+floats+struct.pack('<IiII',vertex_count,group,category,mask);assert len(shape_definition)==88;events.append(3);old.uc.mem_write(shape,bytes(0x80));returned(shape)
  elif address==0x7e1818:assert old.reg(0)==body;events.append(4);returned()
  elif address==0x7e1b28:
   assert old.reg(0)==body;raw=bytes(uc.mem_read(old.reg(1),16));assert raw==bytes(4)+struct.pack('<2f',1.25,-3.5)+bytes(4);events.append(5);pin_count+=1;returned()
  elif address in (0x394ca4,0x394ce4):
   assert old.reg(0) in (physical,previous);events.append(6);returned()
  elif address==0x394cf0:events.append(7)
  elif address==0x393ea0:assert old.reg(0)==owner;events.append(8);pf_count+=1;returned()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 def compare(ctype,player,special,group_override,disable,existing,bounds,pos,label):
  nonlocal body_definition,shape_definition,is_player,override,disabled
  is_player=player;override=group_override;disabled=disable;body_definition=None;shape_definition=None;events.clear();old.uc.mem_write(owner,bytes(0x1800));old.pointer(owner,vt);old.pointer(owner+0x2dc,previous if existing==1 else physical if existing==2 else 0);old.uc.mem_write(owner+0x84,bytes((special,)));old.uc.mem_write(owner+0x12c,bounds[:8]);old.uc.mem_write(owner+0x138,bounds[8:]);old.uc.mem_write(owner+0x160,pos);old.uc.mem_write(ai+0x38,struct.pack('<I',ctype));old.uc.mem_write(physical,bytes(40));old.uc.mem_write(previous,bytes(40))
  old.invoke(0x3b4088,[owner]);enabled=int(body_definition is not None);po_character=int(enabled and not(shape_definition and struct.unpack_from('<I',shape_definition,8)[0]));pinned=old.uc.mem_read(physical+0x27,1)[0] if enabled else 0;radius=bytes(old.uc.mem_read(physical+12,4)) if enabled else bytes(4)
  expected=(body_definition or bytes(64))+(shape_definition or bytes(88))+radius+struct.pack('<4I',enabled,po_character,pinned,len(events))+struct.pack('<8I',*events,*([0]*(8-len(events))))+bytes(4);assert len(expected)==208
  # Unsupported character types do not allocate: userdata argument is unused.
  packed=struct.pack('<3Q6I',nowner,nphysical,nprevious if existing==1 else nphysical if existing==2 else 0,ctype,player,special,group_override,disable,0)+bounds+pos;new.uc.mem_write(request,packed);new.uc.mem_write(output,b'\xcc'*208);assert new.invoke('dh2_character_body_config',[output,request])==0;actual=bytes(new.uc.mem_read(output,208));assert config_equal(expected,actual),(label,'configuration',list(struct.unpack('<52I',expected)),list(struct.unpack('<52I',actual)))
  attached=word(owner+0x2dc);expected_attachment=previous if existing==1 else physical if existing==2 else 0
  if not disable:expected_attachment=physical if enabled else 0
  assert attached==expected_attachment,(label,'attachment',hex(attached),hex(expected_attachment))
  records.append(packed+expected)
  if isinstance(label,str):cases.append({'case':label,'type':ctype,'player':player,'special':special,'enabled':enabled,'shape':None if not enabled else struct.unpack_from('<I',expected,72)[0],'group':None if not enabled else struct.unpack_from('<i',expected,140)[0],'events':events[:],'radius_bits':struct.unpack('<I',radius)[0]})
 bounds=struct.pack('<4f',-30,-20,50,40);pos=struct.pack('<2f',150,-250)
 for ctype in range(12):
  for player in (0,1):
   for special in (0,1,255):
    for override in (0,1):
     for disable in (0,1):
      for existing in (0,1,2):compare(ctype,player,special,override,disable,existing,bounds,pos,f'branch {ctype}/{player}/{special}/{override}/{disable}/{existing}')
 specials=(0,0x80000000,1,0x80000001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0x7f801234)
 for i in range(a.cases):
  words=[rng.choice(specials) if i%5==0 else float_bits(rng.uniform(-3000,3000)) for _ in range(6)];compare(rng.randrange(12),rng.randrange(2),rng.choice((0,0,0,1,255)),rng.randrange(2),rng.randrange(2),rng.randrange(3),struct.pack('<4I',*words[:4]),struct.pack('<2I',*words[4:]),('synthetic',i))
 # Atomic native rejection: invalid boolean and reserved inputs.
 before=bytes(new.uc.mem_read(output,208));new.uc.mem_write(request+28,struct.pack('<I',2));assert new.invoke('dh2_character_body_config',[output,request])&0xffffffff==0xffffffff;assert bytes(new.uc.mem_read(output,208))==before
 reference=struct.pack('<II',0x31434243,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(reference)
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'mismatches':0,'source_branch_cases':len(cases),'allocations':allocations,'create_body_calls':create_count,'pin_zero_mass_calls':pin_count,'pf_update_requests':pf_count,'original_import_calls':old.import_calls,'all_shape_body_filter_fields_and_service_order_compared':True,'arm64_userdata_pointers_above_4gib':True,'radius_uses_max_extent_with_unordered_width_fallback':True,'scope':__doc__+' Original predicates, POCharacter and PhysicalObject constructors, _init/_addShape, pin and SetPhysicalObject instructions execute. CreateBody/CreateShape/mass/SetMass/destructors/PF update backends are caller fixtures. Original AI definition lookup is supplied; no physics world stepping or contact backend is claimed.','elapsed_seconds':round(time.monotonic()-started,2),'behavior_snapshots':cases[:72]};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}))
if __name__=='__main__':main()
