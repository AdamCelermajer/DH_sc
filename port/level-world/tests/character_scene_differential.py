"""Original skin-technique joint-box/point bounds and non-marker CalcMeshBox.

Cached joint absolute matrices, mesh providers, scene collection and root
matrix are explicit services. The native bridge's actual Prince controllers
are tested with authored joint boxes and the model's factory/rest matrices.
Outer Euler conversion is separately verified by decor_scene_differential.
"""
import argparse,ctypes,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R5
from decor_scene_differential import Cpu,Dependencies,pack,same
ROOT=Path(__file__).resolve().parents[1]
class SceneCpu(Cpu):
 def invoke(self,name,integers,stack=b''):
  entry=self.stack+0xe000;self.uc.reg_write(self.sp_reg,entry);self.uc.reg_write(self.lr_reg,self.stop)
  for i,value in enumerate(integers):self.write_reg(i,value)
  if stack:self.uc.mem_write(entry,stack)
  self.uc.emu_start(self.symbols[name],self.stop,count=1000000)
  assert self.uc.reg_read(self.pc_reg)==self.stop,('instruction budget',name)
  assert self.uc.reg_read(self.sp_reg)==entry,('stack restoration',name)
  return self.reg(0)
class CharacterDependencies(Dependencies):
 def call(self,cpu,name):
  if name=='__aeabi_i2f':
   value=cpu.reg(0)&0xffffffff;value=value if value<0x80000000 else value-0x100000000;cpu.put_float(ctypes.c_float(value).value,'f');cpu.uc.reg_write(cpu.pc_reg,cpu.uc.reg_read(cpu.lr_reg));return
  return super().call(cpu,name)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--cases',type=int,default=200);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/character-scene/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];old=SceneCpu(a.engine,False,CharacterDependencies(),manifest);new=SceneCpu(a.library,True,CharacterDependencies(),{'functions':[]})
 def ptr(at,value):old.uc.mem_write(at,struct.pack('<I',value))
 def returned(value=0):old.write_reg(0,value);old.uc.reg_write(old.pc_reg,old.uc.reg_read(old.lr_reg))
 for addr in (0x66e384,0x66c5f4,0x66fb84,0x66ef44,0x47211c,0x646c1c,0x3a4398,0x3b4f3c):old.symbols[str(addr)]=addr
 out=old.data+0x100;tech=old.data+0x1000;cache=old.data+0x2000;skin=old.data+0x3000;table=old.data+0x4000;matrices=old.data+0x5000;boxes=old.data+0xa000
 prepared=(0x66df88,0x66c544,0x66fab4,0x66ee94)
 phase='';current_entries=[];providers={};root_matrix=pack([1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]);visual=old.data+0x1000;root=old.data+0x2000;rootvt=old.data+0x3000;manager=old.data+0x4000;managervt=old.data+0x4100;game=old.data+0x4200;system=old.data+0x4300;vector=old.data+0x4400;matrixptr=old.data+0x4500;providervt=old.data+0x4600
 collect=old.data+0xff00;matrix_method=old.data+0xff10
 def hook(uc,address,size,unused):
  if phase=='skin' and address in prepared:returned()
  elif phase=='mesh' and address==0x472438:uc.reg_write(UC_ARM_REG_R5,game)
  elif phase=='mesh' and address==0x597290:returned(providers[old.reg(0)][1])
  elif phase=='mesh' and address==0x310450:returned()
  elif phase=='mesh' and address==collect:
   kind=1 if old.reg(1)==0x73656164 else 0;selected=[address for address,(_,_,entry) in providers.items() if entry[9]==kind];old.uc.mem_write(vector,struct.pack('<'+'I'*len(selected),*selected));old.uc.mem_write(old.reg(2),struct.pack('<3I',vector,vector+4*len(selected),vector+4*len(selected)));returned()
  elif phase=='mesh' and address==matrix_method:old.uc.mem_write(matrixptr,root_matrix+bytes(4));returned(matrixptr)
  elif phase=='visual_scale' and address==0x3b4f3c:
   from unicorn.arm_const import UC_ARM_REG_R4
   uc.reg_write(UC_ARM_REG_R4,old.data+0x1000)
  elif phase=='visual_scale' and address==0x3b4f84:uc.reg_write(old.pc_reg,old.stop)
  elif phase=='owner_bounds' and address==0x393ea0:returned()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 for addr in (collect,matrix_method):old.uc.mem_write(addr,struct.pack('<I',0xe12fff1e))
 skin_records=[];mesh_records=[]
 def skin_compare(joints,joint_boxes,label):
  nonlocal phase
  phase='skin';count=len(joints);old.uc.mem_write(tech,bytes(0x30));old.uc.mem_write(cache,bytes(0x30));old.uc.mem_write(skin,bytes(0x98));ptr(tech+12,skin);ptr(tech+16,cache);ptr(cache+16,table);ptr(cache+20,table+4*count);ptr(skin+140,len(joint_boxes));ptr(skin+144,boxes);old.uc.mem_write(matrices,b''.join(joints));old.uc.mem_write(table,struct.pack('<'+'I'*count,*[matrices+68*i for i in range(count)]));old.uc.mem_write(boxes,b''.join(joint_boxes))
  expected=None
  for method in (0x66e384,0x66c5f4,0x66fb84,0x66ef44):
   old.invoke(str(method),[out,tech]);value=bytes(old.uc.mem_read(out,24));assert expected is None or same(expected,value),(label,'technique disagreement');expected=value
  new.uc.mem_write(new.data+0x4000,b''.join(joints));new.uc.mem_write(new.data+0xa000,b''.join(joint_boxes));new.uc.mem_write(new.data+0x1000,struct.pack('<2Q2I',new.data+0x4000,new.data+0xa000,count,len(joint_boxes)));assert new.invoke('dh2_character_skin_bounds',[new.data+0x2000,new.data+0x1000])==0;actual=bytes(new.uc.mem_read(new.data+0x2000,24));assert same(expected,actual),(label,'skin',expected.hex(),actual.hex());skin_records.append(struct.pack('<2I',count,len(joint_boxes))+b''.join(joints)+b''.join(joint_boxes)+expected);return expected
 def mesh_compare(entries,placement,label):
  nonlocal phase,root_matrix,providers
  new.uc.mem_write(new.data+0x1000,placement);assert new.invoke('dh2_decor_scene',[new.data+0x2000,new.data+0x1000])==0;metadata=bytes(new.uc.mem_read(new.data+0x2000,104));root_matrix=metadata[40:104]
  phase='mesh';old.uc.mem_write(visual,bytes(0x100));ptr(visual+8,root);ptr(root,rootvt);ptr(rootvt+0x40,matrix_method);ptr(game+0x10,system);ptr(system+0x1c,manager);ptr(manager,managervt);ptr(managervt+0x20,collect);ptr(providervt+0x30,0x5839b0);ptr(rootvt+0x90,0x5970bc);providers={}
  for i,entry in enumerate(entries):
   provider=old.data+0xc000+0x200*i;parent=old.data+0xe000+0x100*i;ptr(provider,providervt);old.uc.mem_write(provider+0x130,entry[:24]);ptr(parent,rootvt);old.uc.mem_write(parent+0xc8,entry[24:36]);providers[provider]=(i,parent,struct.unpack('<9fI',entry))
  old.invoke(str(0x47211c),[visual]);expected=metadata+bytes(old.uc.mem_read(visual+16,24));native_entries=b''.join(entries);new.uc.mem_write(new.data+0x4000,native_entries);request=struct.pack('<Q2I',new.data+0x4000,len(entries),0)+placement;new.uc.mem_write(new.data+0x1000,request);assert new.invoke('dh2_character_mesh_box',[new.data+0x2000,new.data+0x1000])==0;actual=bytes(new.uc.mem_read(new.data+0x2000,128));assert same(expected,actual),(label,'mesh',expected[-24:].hex(),actual[-24:].hex());mesh_records.append(struct.pack('<I',len(entries))+placement+native_entries+expected);return expected
 # Authored Prince model controllers and rest joint matrices, using the same
 # native node-matrix kernel as Scene::load. Matrix inputs remain explicit to
 # the original provider; this does not execute the full Collada allocator.
 path=ROOT.parent/'android-native/app/src/main/assets/models/prince_modular.bdae';image=path.read_bytes();word=lambda o:struct.unpack_from('<I',image,o)[0];text=lambda o:image[o:image.index(0,o)].decode();modelroot=word(32);nodes=[]
 def world_multiply(a,b):
  result=[]
  for col in range(4):
   for row in range(4):
    value=0.0
    for k in range(4):value=ctypes.c_float(value+ctypes.c_float(a[k*4+row]*b[col*4+k]).value).value
    result.append(value)
  return result
 def walk(record,parent=None):
  translation=image[record+12:record+24];quaternion=image[record+24:record+40];scale=image[record+40:record+52];new.uc.mem_write(new.data+0x1000,translation+quaternion+scale)
  # Cpu's normal wrapper exposes only three integer registers; ARM64 fourth
  # pointer is written explicitly for this established scene kernel.
  from unicorn.arm64_const import UC_ARM64_REG_X3
  new.uc.reg_write(UC_ARM64_REG_X3,new.data+0x1000+28);new.invoke('dh2_node_matrix',[new.data+0x2000,new.data+0x1000,new.data+0x1000+12]);local=list(struct.unpack('<16f',new.uc.mem_read(new.data+0x2000,64)));world=local if parent is None else world_multiply(nodes[parent]['world'],local);index=len(nodes);nodes.append({'id':text(word(record)),'sid':text(word(record+8)),'scale':list(struct.unpack('<3f',scale)),'world':world,'parent':parent})
  for j in range(word(record+56)):walk(word(record+60)+80*j,index)
 for i in range(word(modelroot+152)):
  visualscene=word(modelroot+156)+16*i
  for j in range(word(visualscene+8)):walk(word(visualscene+12)+80*j)
 prince=next(n for n in nodes if n['id']=='prince_modular-node');components=[];controller_facts=[];controllers=word(modelroot+116)
 for i in range(word(modelroot+112)):
  record=controllers+12*i;name=text(word(record+4))
  if '_default_warrior-mesh-skin' not in name:continue
  data=word(record+8);count=word(data+116);names=word(data+120);joints=[]
  for j in range(count):
   sid=text(word(names+4*j));node=next(n for n in nodes if n['sid']==sid);joints.append(pack(node['world'])+bytes(4))
  authored_boxes=[image[word(data+144)+24*j:word(data+144)+24*j+24] for j in range(word(data+140))];bounds=skin_compare(joints,authored_boxes,name);components.append(bounds);controller_facts.append({'controller':i,'name':name,'joint_count':count,'box_count':word(data+140),'bounds':list(struct.unpack('<6f',bounds))})
 assert len(components)==4;combined=[min(struct.unpack('<6f',b)[k] for b in components) for k in range(3)]+[max(struct.unpack('<6f',b)[k] for b in components) for k in range(3,6)]
 phase='modular';modular=old.data+0x1000;modular_array=old.data+0x2000;modular_vt=old.data+0x3000;old.uc.mem_write(modular,bytes(0x80));ptr(modular+0x24,modular_array);ptr(modular+0x28,modular_array+32);ptr(modular_vt+0x24,0x5839b0)
 for i,bounds in enumerate(components):
  provider=old.data+0xc000+0x200*i;ptr(modular_array+8*i,0);ptr(modular_array+8*i+4,provider);ptr(provider,modular_vt);old.uc.mem_write(provider+0x130,bounds)
 old.invoke(str(0x646c1c),[modular]);assert bytes(old.uc.mem_read(modular+0x40,24))==pack(combined)
 prince_entry=pack(combined+prince['scale'])+struct.pack('<I',1);prince_output=mesh_compare([prince_entry],pack([123,456,789,0,0,0,1,1,1]+[0]*6+[1,1,1]),'Prince rest selected warrior')
 rng=random.Random(20261023)
 for i in range(a.cases):
  count=rng.choice([0,1,2,3,8,14]);joints=[];joint_boxes=[]
  for j in range(count):
   matrix=[rng.uniform(-3,3) for _ in range(16)];matrix[3]=matrix[7]=matrix[11]=0;matrix[15]=1;joints.append(pack(matrix)+struct.pack('<I',int(i%7==0)));joint_boxes.append(pack([rng.uniform(-50,0) for _ in range(3)]+[rng.uniform(1,50) for _ in range(3)]))
  skin_compare(joints,joint_boxes if i%2 else [],('random skin',i));entries=[]
  for j in range(rng.randrange(0,7)):entries.append(pack([rng.uniform(-100,0) for _ in range(3)]+[rng.uniform(1,100) for _ in range(3)]+[rng.uniform(-3,3) for _ in range(3)])+struct.pack('<I',rng.randrange(2)))
  placement=pack([rng.uniform(-1000,1000) for _ in range(3)]+[rng.uniform(-360,360) for _ in range(3)]+[rng.uniform(.1,2) for _ in range(3)]+[0]*6+[1,1,1]);mesh_compare(entries,placement,('random mesh',i))
 for count in (255,256):skin_compare([pack([1,0,0,0,0,1,0,0,0,0,1,0,j,j*2,j*3,1])+bytes(4) for j in range(count)],[],('uint8 count',count))
 visual_records=[];bounds_records=[]
 def visual_compare(values):
  nonlocal phase
  phase='visual_scale';owner=old.data+0x1000;raw=struct.pack('<3i',*values);old.uc.mem_write(owner+0x59c,raw);old.invoke(str(0x3b4f3c),[]);expected=bytes(old.uc.mem_read(owner+0x120,12));new.uc.mem_write(new.data+0x1000,raw);assert new.invoke('dh2_character_visual_scale',[new.data+0x2000,new.data+0x1000])==0;assert expected==bytes(new.uc.mem_read(new.data+0x2000,12));visual_records.append(raw+expected);return expected
 def bounds_compare(mesh_box,collision_scale,already_scaled,flat,position):
  nonlocal phase
  phase='owner_bounds';owner=old.data+0x1000;old.uc.mem_write(owner,bytes(0x1400));old.uc.mem_write(owner+0x1038,struct.pack('<i',collision_scale));old.uc.mem_write(owner+0x2f9,bytes((flat,)));old.uc.mem_write(owner+0x160,pack(position));old.uc.mem_write(old.data+0x4000,mesh_box);old.invoke(str(0x3a4398),[owner,old.data+0x4000,already_scaled]);expected=bytes(old.uc.mem_read(owner+0x144,24))+bytes(old.uc.mem_read(owner+0x12c,24))+struct.pack('<2I',old.uc.mem_read(owner+0x2f9,1)[0],1);raw=mesh_box+pack(position)+struct.pack('<i2I',collision_scale,already_scaled,flat);new.uc.mem_write(new.data+0x1000,raw);assert new.invoke('dh2_character_owner_bounds',[new.data+0x2000,new.data+0x1000])==0;assert same(expected[:48],bytes(new.uc.mem_read(new.data+0x2000,48))) and expected[48:]==bytes(new.uc.mem_read(new.data+0x2000+48,8));bounds_records.append(raw+expected);return expected
 knight_scale=visual_compare([100,100,100]);knight_mesh=mesh_compare([prince_entry],pack([123,456,789,0,0,0])+knight_scale+pack([0]*6+[1,1,1]),'Knight actual source visual scale');knight_bounds=bounds_compare(knight_mesh[-24:],85,0,0,[123,456,789])
 for values in ([0,0,0],[-1,-2,-3],[80,80,80],[-2147483648,2147483647,16777217]):visual_compare(values)
 for i in range(100):bounds_compare(pack([rng.uniform(-20,0) for _ in range(3)]+[rng.uniform(0,20) for _ in range(3)]),rng.choice([-2147483648,2147483647,-100,-1,0,1,85,100]),i%2,rng.choice([0,1,255]),[rng.uniform(-1000,1000) for _ in range(3)])
 reference=struct.pack('<5I',0x31534348,len(skin_records),len(mesh_records),len(visual_records),len(bounds_records))+b''.join(skin_records)+b''.join(mesh_records)+b''.join(visual_records)+b''.join(bounds_records);a.reference_output.write_bytes(reference)
 facts={'model_sha256':hashlib.sha256(image).hexdigest(),'selected_controllers':controller_facts,'provider_parent_scale':prince['scale'],'selected_modular_union':combined,'mesh_box':list(struct.unpack('<6f',prince_output[-24:])),'knight_base_scale_properties':[100,100,100],'knight_visual_scale':list(struct.unpack('<3f',knight_scale)),'knight_resolved_collision_scale':85,'knight_scaled_mesh_box':list(struct.unpack('<6f',knight_mesh[-24:])),'knight_owner_relative_box':list(struct.unpack('<6f',knight_bounds[:24])),'joint_pose':'Complete authored factory/rest node matrices; no outer owner transform and no sampled rendered vertices.'};(ROOT/'reference/character-scene/prince-rest-fixture.json').write_text(json.dumps(facts,indent=2)+'\n')
 coverage={r['original_symbol']+'@'+r['elf_address']:sum(int(r['elf_address'],16)<=v<int(r['elf_address'],16)+r['size'] for v in old.seen) for r in manifest['functions']};report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'skin_comparisons':len(skin_records),'original_skin_technique_observations':len(skin_records)*4,'mesh_comparisons':len(mesh_records),'visual_scale_comparisons':len(visual_records),'owner_bounds_comparisons':len(bounds_records),'mismatches':0,'scope':__doc__,'prince':facts,'original_coverage':coverage,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='original_coverage'}))
if __name__=='__main__':main()
