"""Original instructions versus actual-model host composition outputs.

Cached Scene joint/root matrices, STL collection, AI storage and creation
services are explicit fixtures. No whole factory or historical libm claim.
"""
import hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4,UC_ARM_REG_R5
from character_scene_differential import SceneCpu,CharacterDependencies,pack
from character_body_config_differential import Cpu as BodyCpu,config_equal
HERE=Path(__file__).resolve().parents[1];ROOT=HERE.parents[1]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 fixture=HERE/'reference/character-npc-body/source-fixtures.bin';blob=fixture.read_bytes();at=0
 def read(n):
  nonlocal at
  b=blob[at:at+n];at+=n;assert len(b)==n;return b
 def words(n=1):return struct.unpack('<'+'I'*n,read(n*4))
 assert words()[0]==0x3142504e;count=words()[0]
 image=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(image)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
 c=SceneCpu(image,False,CharacterDependencies(),{'functions':[]});c.uc.mem_map(c.data+0x10000,0x20000);b=BodyCpu(image,False,{'functions':[]});phase='';matrix=b''
 v,r,rv,m,p,mv,owner,tech,cache,skin,table,joints,boxes,out,vector,provider_vt,game,system,manager,manager_vt,mp=[c.data+x for x in (0x1000,0x2000,0x3000,0x4000,0x5000,0x6000,0x7000,0x8000,0x9000,0xa000,0xb000,0xc000,0x12000,0x17000,0x18000,0x19000,0x1a000,0x1b000,0x1c000,0x1d000,0x1e000)]
 collect=c.data+0x1f000;matrix_method=c.data+0x1f010
 def ptr(a,n):c.uc.mem_write(a,struct.pack('<I',n))
 def ret(n=0):c.write_reg(0,n);c.uc.reg_write(c.pc_reg,c.uc.reg_read(c.lr_reg))
 def hook(u,a,n,x):
  if phase=='skin' and a in (0x66df88,0x66c544,0x66fab4,0x66ee94):ret()
  elif phase=='mesh' and a==0x472438:u.reg_write(UC_ARM_REG_R5,game)
  elif phase=='mesh' and a==0x597290:
   ret(p if c.reg(0)==m else providers[c.reg(0)][0])
  elif phase=='mesh' and a==0x310450:ret()
  elif phase=='mesh' and a==matrix_method:c.uc.mem_write(mp,matrix+bytes(4));ret(mp)
  elif phase=='mesh' and a==collect:
   kind=int(c.reg(1)==0x73656164);selected=[address for address,(_,entry) in providers.items() if struct.unpack_from('<I',entry,36)[0]==kind];c.uc.mem_write(vector,struct.pack('<'+'I'*len(selected),*selected));c.uc.mem_write(c.reg(2),struct.pack('<3I',vector,vector+4*len(selected),vector+4*len(selected)));ret()
  elif phase=='scale' and a==0x3b4f3c:u.reg_write(UC_ARM_REG_R4,owner)
  elif phase=='scale' and a==0x3b4f84:u.reg_write(c.pc_reg,c.stop)
  elif phase=='bounds' and a==0x393ea0:ret()
 c.uc.hook_add(UC_HOOK_CODE,hook)
 for address in (0x66e384,0x66c5f4,0x66fb84,0x66ef44,0x47211c,0x3b4f3c,0x3a4398,0x470a54,0x3a2fec,0x3a3054,0x3a49f0):c.symbols[str(address)]=address
 for address in (collect,matrix_method):c.uc.mem_write(address,struct.pack('<I',0xe12fff1e))
 # Genuine GetCharAIId→GetCharAI→GetCharType reads source runtime AiProps.
 word=lambda address:struct.unpack('<I',c.uc.mem_read(address,4))[0]
 base=(0x3a2ff8+8+word(0x3a301c))&0xffffffff;count_slot=base+word(0x3a3020);count_value=c.data+0x20000;ptr(count_slot,count_value);ptr(count_value,76)
 base=(0x3a3030+8+word(0x3a304c))&0xffffffff;rows_slot=base+word(0x3a3050);row_values=c.data+0x21000;rows_storage=c.data+0x20010;ptr(rows_slot,rows_storage);ptr(rows_storage,row_values);c.uc.mem_write(row_values,bytes(76*0x44));ptr(row_values+40*0x44+0x38,4);ptr(row_values+68*0x44+0x38,4)
 # Body creation service boundaries match the existing original audit.
 bo,bp,bb,bs,ba,bvt,previous=[b.data+x for x in (0x1000,0x4000,0x6000,0x7000,0x8000,0x9000,0x5000)];events=[];body_definition=None;shape_definition=None;nowner=0;nphysical=0
 def br(v=0):b.put(0,v);b.uc.reg_write(b.pc,b.uc.reg_read(b.lr))
 def bhook(u,a,n,x):
  nonlocal body_definition,shape_definition
  if a==0x3a3024:br(ba)
  elif a in (0x337888,0x3140ec,0x3139ac,0x31167c,0x337a88):br()
  elif a==0x310570:assert b.reg(0)==40 and b.reg(1)==0;events.append(1);br(bp)
  elif a==0x3b41fc:br(0)
  elif a==0x34bcf0:
   data=bytes(u.mem_read(b.reg(1),44));body_definition=struct.pack('<Q',nphysical)+data[:16]+data[20:40]+struct.pack('<5I',*data[40:44],0);events.append(2);u.mem_write(bb,bytes(0x100));u.mem_write(bb+0x1c,pack([1.25,-3.5]));br(bb)
  elif a==0x7e1dc8:
   data=bytes(u.mem_read(b.reg(1),100));assert struct.unpack_from('<I',data,4)[0]==0;group=struct.unpack_from('<h',data,30)[0];category,mask=struct.unpack_from('<2H',data,26);shape_definition=struct.pack('<QII',nphysical,0,data[24])+data[12:24]+data[32:44]+bytes(32)+struct.pack('<IiII',0,group,category,mask);events.append(3);br(bs)
  elif a==0x7e1818:events.append(4);br()
  elif a==0x7e1b28:assert bytes(u.mem_read(b.reg(1),16))==bytes(4)+pack([1.25,-3.5])+bytes(4);events.append(5);br()
  elif a in (0x394ca4,0x394ce4):events.append(6);br()
  elif a==0x394cf0:events.append(7)
  elif a==0x393ea0:events.append(8);br()
 b.uc.hook_add(UC_HOOK_CODE,bhook)
 skin_comparisons=0;skin_techniques=0;mesh_comparisons=0;bounds_comparisons=0;type_comparisons=0;body_comparisons=0;requests=0
 for _ in range(count):
  index,aiid,collision=words(3);scales=read(12);marker=words()[0];position=read(12);rotation=read(12);marker_box=read(24);parent_scale=read(12);num_entries=words()[0];entries=[read(40) for _ in range(num_entries)];num_skins=words()[0];actual_skins=[]
  for _ in range(num_skins):
   jc,bc=words(2);actual_skins.append((jc,bc,read(68*jc),read(24*bc)))
  nowner,nphysical=struct.unpack('<2Q',read(16));visual=read(128);bounds=read(56);body=read(208)
  c.uc.mem_write(owner,bytes(0x1400));c.uc.mem_write(owner+0xffc,struct.pack('<I',aiid));assert c.invoke(str(0x3a2fec),[owner])==aiid
  observed_type=c.invoke(str(0x3a3054),[owner]);assert observed_type==4,(index,aiid,observed_type,hex(count_slot),hex(rows_slot),hex(row_values),hex(c.reg(0)))
  assert c.invoke(str(0x3a49f0),[owner])==0;type_comparisons+=3
  phase='scale';c.uc.mem_write(owner+0x59c,scales);c.invoke(str(0x3b4f3c),[]);assert bytes(c.uc.mem_read(owner+0x120,12))==visual[:12]
  for jc,bc,jm,jb in actual_skins:
   phase='skin';c.uc.mem_write(tech,bytes(0x30));c.uc.mem_write(cache,bytes(0x30));c.uc.mem_write(skin,bytes(0x98));ptr(tech+12,skin);ptr(tech+16,cache);ptr(cache+16,table);ptr(cache+20,table+4*jc);ptr(skin+140,bc);ptr(skin+144,boxes);c.uc.mem_write(joints,jm);c.uc.mem_write(table,struct.pack('<'+'I'*jc,*[joints+68*i for i in range(jc)]));c.uc.mem_write(boxes,jb)
   for method in (0x66e384,0x66c5f4,0x66fb84,0x66ef44):c.invoke(str(method),[out,tech]);assert bytes(c.uc.mem_read(out,24))==entries[0][:24];skin_techniques+=1
   skin_comparisons+=1
  # Original CalcMeshBox consumes the genuine source marker/provider bounds and
  # caller cached outer root matrix, already independently differential-tested.
  phase='mesh';matrix=visual[40:104];c.uc.mem_write(v,bytes(0x100));ptr(v+8,r);ptr(v+12,m if marker else 0);ptr(r,rv);ptr(rv+0x40,matrix_method);ptr(rv+0x90,0x5970bc);ptr(m,mv);ptr(mv+0x30,0x5839b0);c.uc.mem_write(m+0x130,marker_box);ptr(p,rv);c.uc.mem_write(p+0xc8,parent_scale);ptr(game+0x10,system);ptr(system+0x1c,manager);ptr(manager,manager_vt);ptr(manager_vt+0x20,collect);ptr(provider_vt+0x30,0x5839b0);providers={}
  for i,entry in enumerate(entries):
   provider=c.data+0x25000+0x200*i;parent=c.data+0x28000+0x100*i;ptr(provider,provider_vt);c.uc.mem_write(provider+0x130,entry[:24]);ptr(parent,rv);c.uc.mem_write(parent+0xc8,entry[24:36]);providers[provider]=(parent,entry)
  c.invoke(str(0x47211c),[v]);assert bytes(c.uc.mem_read(v+16,24))==visual[-24:],('mesh',index);mesh_comparisons+=1
  phase='bounds';c.uc.mem_write(owner,bytes(0x1400));c.uc.mem_write(owner+0x1038,struct.pack('<I',collision));c.uc.mem_write(owner+0x160,position);ptr(owner,provider_vt);ptr(provider_vt+0x9c,0x3a4398);ptr(v+4,owner);c.uc.mem_write(v+0x28,bytes((marker,)));c.invoke(str(0x470a54),[v]);expected=bytes(c.uc.mem_read(owner+0x144,24))+bytes(c.uc.mem_read(owner+0x12c,24))+struct.pack('<2I',c.uc.mem_read(owner+0x2f9,1)[0],1);assert expected==bounds,('bounds',index);bounds_comparisons+=1
  b.uc.mem_write(bo,bytes(0x1800));b.pointer(bo,bvt);b.uc.mem_write(bo+0x12c,bounds[24:32]);b.uc.mem_write(bo+0x138,bounds[36:44]);b.uc.mem_write(bo+0x160,position[:8]);b.uc.mem_write(ba+0x38,struct.pack('<I',4));b.uc.mem_write(bp,bytes(40));events.clear();body_definition=None;shape_definition=None;b.invoke(0x3b4088,[bo]);expected=body_definition+shape_definition+bytes(b.uc.mem_read(bp+12,4))+struct.pack('<4I',1,1,b.uc.mem_read(bp+0x27,1)[0],len(events))+struct.pack('<8I',*events,*([0]*(8-len(events))))+bytes(4);assert config_equal(expected,body),('body',index);requests+=len(events);body_comparisons+=1
 assert at==len(blob)
 report=dict(validation='PASS',original_sha256=sha(image),fixture_sha256=sha(fixture),script_sha256=sha(Path(__file__)),original_function_manifest_sha256=sha(HERE/'reference/character-npc-body/original-functions.json'),original_skin_provider_cases=skin_comparisons,original_skin_technique_observations=skin_techniques,original_mesh_box_cases=mesh_comparisons,original_owner_bounds_cases=bounds_comparisons,original_AI_type_player_observations=type_comparisons,original_body_config_cases=body_comparisons,original_body_creation_service_requests=requests,mismatches=0,scope=__doc__,cached_outer_root_matrix_service=True,host_composition_output_compared=True,new_helper_ARM64_execution=False,full_original_AssetManager_factory=False)
 output=HERE/'reports/character-npc-body-source-original-composition.json';assert not output.exists();output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
