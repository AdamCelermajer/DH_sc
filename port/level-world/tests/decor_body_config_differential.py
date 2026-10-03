"""Original marker CalcMeshBox, bounds padding and PODecor construction/attach.

Mesh bounding boxes/parent scale/root matrices and allocation/body/shape/mass/PF
services are explicit fixtures. Original arithmetic, constructor and attachment
instructions execute; complete native physics is audited in physics-backend.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from character_body_config_differential import Cpu as BaseCpu,config_equal
from body_transform_differential import equal
from aggro_differential import float_bits
from combat_result_differential import floating
ROOT=Path(__file__).resolve().parents[1]
class Cpu(BaseCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='decor_callback':return self.decor_service(uc,address,size,unused)
  if self.imports.get(address)=='__aeabi_fcmpeq':
   self.put(0,int(floating(self.reg(0))==floating(self.reg(1))));self.import_calls['__aeabi_fcmpeq']=self.import_calls.get('__aeabi_fcmpeq',0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=2000);a=p.parse_args();started=time.monotonic()
 manifest=json.loads((ROOT/'reference/decor-body-config/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261018)
 owner=old.data+0x1000;physical=old.data+0x4000;previous=old.data+0x5000;body=old.data+0x6000;shape=old.data+0x7000;visual=old.data+0x8000;vt=old.data+0x9000;world=old.data+0xa000;marker=old.data+0xb000;rootnode=old.data+0xc000;scale=old.data+0xd000;bounds=old.data+0xe000;matrix=old.data+0xf000;marker_vt=old.data+0x11000;root_vt=old.data+0x12000
 request=new.data+0x1000;output=new.data+0x2000;nowner=0x1000000012345678;nphysical=0x200000009abcdef0;nprevious=0x3000000012345678
 events=[];definition=None;shape_definition=None;override=disabled=mesh_pf=0;records=[];meshes=[];authored=[];mesh_mode=False;level_bounds=None
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 def returned(v=0):old.put(0,v);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def hook(uc,address,size,unused):
  nonlocal definition,shape_definition,mesh_pf,level_bounds
  if address in (old.callback+32,old.callback+48,old.callback+64) and uc.reg_read(old.pc)!=address:return
  if address in (0x38be5c,0x388730,0x337888,0x3140ec,0x3139ac,0x31167c):returned()
  elif address==0x337a88:returned(override if uc.reg_read(old.lr)==0x46f3bc else disabled)
  elif address==0x310570:assert old.reg(0)==40 and old.reg(1)==0;events.append(1);returned(physical)
  elif address==0x34bcf0:
   raw=bytes(uc.mem_read(old.reg(1),44));assert word(old.reg(1)+16)==physical;definition=struct.pack('<Q',nphysical)+raw[:16]+raw[20:40]+struct.pack('<5I',*raw[40:44],0);events.append(2);old.uc.mem_write(body,bytes(0x100));old.uc.mem_write(body+0x1c,struct.pack('<2f',1.25,-3.5));returned(body)
  elif address==0x7e1dc8:
   assert old.reg(0)==body;raw=bytes(uc.mem_read(old.reg(1),100));assert struct.unpack_from('<I',raw,4)[0]==1 and struct.unpack_from('<I',raw,8)[0]==physical;category,mask=struct.unpack_from('<2H',raw,26)
   shape_definition=struct.pack('<QII',nphysical,1,raw[24])+raw[12:24]+bytes(12)+raw[32:64]+struct.pack('<IiII',struct.unpack_from('<I',raw,96)[0],struct.unpack_from('<h',raw,30)[0],category,mask);events.append(3);returned(shape)
  elif address==0x7e1818:assert old.reg(0)==body;events.append(4);returned()
  elif address==0x7e1b28:assert old.reg(0)==body and bytes(uc.mem_read(old.reg(1),16))==bytes(4)+struct.pack('<2f',1.25,-3.5)+bytes(4);events.append(5);returned()
  elif address in (0x394ca4,0x394ce4):assert old.reg(0) in (physical,previous);events.append(6);returned()
  elif address==0x394cf0:events.append(7)
  elif address==0x393ea0:
   assert old.reg(0)==owner
   if definition is None:mesh_pf+=1
   else:events.append(8)
   returned()
  elif mesh_mode and address==0x597290:assert old.reg(0)==marker;returned(rootnode)
  elif mesh_mode and address==old.callback+32:assert old.reg(0)==marker;returned(bounds)
  elif mesh_mode and address==old.callback+48:assert old.reg(0)==rootnode;returned(scale)
  elif mesh_mode and address==old.callback+64:assert old.reg(0)==rootnode;returned(matrix)
  elif address==0x3f7384:
   old.put(5,old.data+0x13000);offset=word(0x3f7384+8+0x68c);old.pointer(old.data+0x13000+offset,owner);old.pointer(owner+0x44,world)
  elif address==0x34c048:
   assert old.reg(0)==world;level_bounds=struct.pack('<3I',old.reg(1),old.reg(2),old.reg(3))+bytes(uc.mem_read(uc.reg_read(old.sp),4));uc.reg_write(old.pc,old.stop)
 old.decor_service=hook
 for address in (old.callback+32,old.callback+48,old.callback+64):old.imports[address]='decor_callback'
 old.uc.hook_add(UC_HOOK_CODE,hook);old.pointer(vt+0x9c,0x38b110);old.pointer(marker_vt+0x30,old.callback+32);old.pointer(root_vt+0x90,old.callback+48);old.pointer(root_vt+0x40,old.callback+64)
 old.invoke(0x3f7384,[]);assert new.invoke('dh2_decor_level_world_bounds',[output])==0;assert bytes(new.uc.mem_read(output,16))==level_bounds==struct.pack('<4f',-2000,-2000,2000,2000)
 def mesh_compare(packed,label):
  nonlocal mesh_mode
  mesh_mode=True;old.uc.mem_write(visual,bytes(0x100));old.pointer(visual+8,rootnode);old.pointer(visual+12,marker);old.pointer(marker,marker_vt);old.pointer(rootnode,root_vt);old.uc.mem_write(bounds,packed[:24]);old.uc.mem_write(scale,packed[24:36]);old.uc.mem_write(matrix,packed[36:]+bytes(4));old.invoke(0x47211c,[visual]);expected=bytes(old.uc.mem_read(visual+16,24));mesh_mode=False
  new.uc.mem_write(request,packed);assert new.invoke('dh2_decor_marker_mesh_box',[output,request])==0;actual=bytes(new.uc.mem_read(output,24));assert equal(expected,actual),(label,'meshbox',expected.hex(),actual.hex());meshes.append(packed+expected);return expected
 def compare(present,found,group_override,disable,existing,mesh_box,pos,flat,label):
  nonlocal definition,shape_definition,override,disabled,mesh_pf
  definition=shape_definition=None;override=group_override;disabled=disable;mesh_pf=0;events.clear();old.uc.mem_write(owner,bytes(0x1000));old.pointer(owner,vt);old.pointer(owner+0x2d8,visual if present else 0);old.pointer(owner+0x2dc,previous if existing==1 else physical if existing==2 else 0);old.uc.mem_write(owner+0x160,pos);old.uc.mem_write(owner+0x2f9,bytes((flat,)));old.uc.mem_write(physical,bytes(40));old.uc.mem_write(visual,bytes(0x100));old.pointer(visual+4,owner);old.uc.mem_write(visual+0x10,mesh_box);old.uc.mem_write(visual+0x28,bytes((found,)));old.invoke(0x388a98,[owner])
  enabled=int(definition is not None);radius=bytes(old.uc.mem_read(physical+12,4)) if enabled else bytes(4);pinned=old.uc.mem_read(physical+0x27,1)[0] if enabled else 0
  config=(definition or bytes(64))+(shape_definition or bytes(88))+radius+struct.pack('<4I',enabled,0,pinned,len(events))+struct.pack('<8I',*events,*([0]*(8-len(events))))+bytes(4)
  relative=bytes(old.uc.mem_read(owner+0x144,24)) if present else bytes(24);absolute=bytes(old.uc.mem_read(owner+0x12c,24)) if present else bytes(24);expected=config+relative+absolute+struct.pack('<2I',old.uc.mem_read(owner+0x2f9,1)[0],mesh_pf)
  packed=struct.pack('<3Q4I',nowner,nphysical,nprevious if existing==1 else nphysical if existing==2 else 0,present,found,group_override,disable)+mesh_box+pos+struct.pack('<I',flat);assert len(packed)==80 and len(expected)==264
  new.uc.mem_write(request,packed);assert new.invoke('dh2_decor_body_config',[output,request])==0;actual=bytes(new.uc.mem_read(output,264));assert config_equal(expected[:208],actual[:208]) and equal(expected[208:256],actual[208:256]) and expected[256:]==actual[256:],(label,'config',expected.hex(),actual.hex());records.append(packed+expected)
  return config
 identity=struct.pack('<16f',1,0,0,0,0,1,0,0,0,0,1,0,150,250,350,1)
 inventory=json.loads((ROOT.parents[1]/'.local-inputs/decor-body-discovery/crypt-colbox-inventory.json').read_text())
 for model in inventory['models']:
  for row in model['authored_instances']:
   marker_row=model['colbox_nodes'][0] if model['colbox_nodes'] else None
   # Authored placements and scales; identity node matrix is an explicit scene
   # service fixture, not a claimed reconstruction of the authored Euler adapter.
   box=mesh_compare(struct.pack('<6f',*marker_row['instances'][0]['mesh_bounds'])+struct.pack('<3f',*row['scale'])+identity,row['name']) if marker_row else bytes(24)
   result=compare(1,int(marker_row is not None),0,0,0,box,struct.pack('<3f',*row['position']),0,row['name']);authored.append(dict(model=model['model'],name=row['name'],enabled=bool(marker_row),radius_bits=struct.unpack_from('<I',result,152)[0]))
 for present,found in ((0,0),(1,0),(1,1)):
  for override in (0,1):
   for disable in (0,1):
    for existing in (0,1,2):
     for flat in (0,1,255):compare(present,found,override,disable,existing,struct.pack('<6f',-3,-4,-5,3,4,5),struct.pack('<3f',123,-456,789),flat,('gates',present,found,override,disable,existing,flat))
 for width in (-10.0,-0.0,0.0,9.999999046325684,10.0,10.000000953674316):
  for height in (-0.0,0.0,9.999999046325684,10.0,10.000000953674316):
   compare(1,1,0,0,0,struct.pack('<6f',0,0,-5,width,height,5),struct.pack('<3f',123,-456,789),0,('padding',width,height))
 specials=(0,0x80000000,1,0x80000001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0x7f801234)
 for i in range(a.cases):
  words=[rng.choice(specials) if i%5==0 else float_bits(rng.uniform(-3000,3000)) for _ in range(25)];packed=struct.pack('<25I',*words);box=mesh_compare(packed,('random mesh',i));compare(1,1,rng.randrange(2),rng.randrange(2),rng.randrange(3),box,struct.pack('<3I',*words[:3]),rng.choice((0,1,255)),('random config',i))
 before=bytes(new.uc.mem_read(output,264));new.uc.mem_write(request+24,struct.pack('<I',2));assert new.invoke('dh2_decor_body_config',[output,request])&0xffffffff==0xffffffff and bytes(new.uc.mem_read(output,264))==before
 reference=struct.pack('<3I',0x31434444,len(meshes),len(records))+level_bounds+b''.join(meshes)+b''.join(records);a.reference_output.write_bytes(reference)
 coverage={r['original_symbol']+'@'+r['elf_address']:sum(int(r['elf_address'],16)<=v<int(r['elf_address'],16)+r['size'] for v in old.seen) for r in manifest['functions']}
 report=dict(original_sha256=manifest['original_sha256'],arm64_library_sha256=hashlib.sha256(a.library.read_bytes()).hexdigest(),reference_sha256=hashlib.sha256(reference).hexdigest(),mesh_comparisons=len(meshes),config_comparisons=len(records),mismatches=0,level_world_bounds=list(struct.unpack('<4f',level_bounds)),authored_placements=len(authored),authored_enabled=sum(r['enabled'] for r in authored),authored_no_marker=sum(not r['enabled'] for r in authored),original_coverage=coverage,scope=__doc__+' Authored model bounds/scales/positions exercise identity-root-matrix service fixtures. Authored Euler/root scene matrix reconstruction and visual node database factories are not claimed.',elapsed_seconds=round(time.monotonic()-started,2),authored=authored);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('authored','original_coverage')}))
if __name__=='__main__':main()
